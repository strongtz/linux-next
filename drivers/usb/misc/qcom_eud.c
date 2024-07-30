// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015-2021, The Linux Foundation. All rights reserved.
 */

#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/usb/role.h>

#define EUD_REG_INT1_EN_MASK	0x0024
#define EUD_REG_INT_STATUS_1	0x0044
#define EUD_REG_CTL_OUT_1	0x0074
#define EUD_REG_VBUS_INT_CLR	0x0080
#define EUD_REG_CSR_EUD_EN	0x1014
#define EUD_REG_SW_ATTACH_DET	0x1018
#define EUD_REG_EUD_EN2		0x0000

#define EUD_ENABLE		BIT(0)
#define EUD_INT_PET_EUD		BIT(0)
#define EUD_INT_VBUS		BIT(2)
#define EUD_INT_SAFE_MODE	BIT(4)
#define EUD_INT_ALL		(EUD_INT_VBUS | EUD_INT_SAFE_MODE)

struct eud_chip {
	struct device			*dev;
	struct usb_role_switch		*role_sw;
	struct phy			*usb2_phy;

	/* mode lock */
	struct mutex			mutex;
	void __iomem			*base;
	void __iomem			*mode_mgr;
	unsigned int			int_status;
	int				irq;
	bool				enabled;
	bool				usb_attached;
	enum usb_role			current_role;
};

static int eud_phy_enable(struct eud_chip *chip)
{
	int ret;

	ret = phy_init(chip->usb2_phy);
	if (ret)
		return ret;

	ret = phy_power_on(chip->usb2_phy);
	if (ret)
		phy_exit(chip->usb2_phy);

	return ret;
}

static void eud_phy_disable(struct eud_chip *chip)
{
	phy_power_off(chip->usb2_phy);
	phy_exit(chip->usb2_phy);
}

static int eud_usb_role_set(struct eud_chip *chip, enum usb_role role)
{
	struct usb_role_switch *sw;
	int ret = 0;

	mutex_lock(&chip->mutex);

	/* Avoid duplicate role handling */
	if (role == chip->current_role)
		goto err;

	sw = usb_role_switch_get(chip->dev);
	if (IS_ERR_OR_NULL(sw)) {
		dev_err(chip->dev, "failed to get usb switch\n");
		ret = -EINVAL;
		goto err;
	}

	ret = usb_role_switch_set_role(sw, role);
	usb_role_switch_put(sw);

	if (ret) {
		dev_err(chip->dev, "failed to set role\n");
		goto err;
	}
	chip->current_role = role;
err:
	mutex_unlock(&chip->mutex);

	return ret;
}

static int enable_eud(struct eud_chip *priv)
{
	int ret;

	ret = eud_phy_enable(priv);
	if (ret)
		return ret;

	writel(EUD_ENABLE, priv->base + EUD_REG_CSR_EUD_EN);
	writel(EUD_INT_VBUS | EUD_INT_SAFE_MODE,
			priv->base + EUD_REG_INT1_EN_MASK);
	writel(1, priv->mode_mgr + EUD_REG_EUD_EN2);

	return ret;
}

static void disable_eud(struct eud_chip *priv)
{
	writel(0, priv->base + EUD_REG_CSR_EUD_EN);
	writel(0, priv->mode_mgr + EUD_REG_EUD_EN2);
	eud_phy_disable(priv);
}

static ssize_t enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct eud_chip *chip = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", chip->enabled);
}

static ssize_t enable_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct eud_chip *chip = dev_get_drvdata(dev);
	bool enable;
	int ret;

	if (kstrtobool(buf, &enable))
		return -EINVAL;

	/* EUD enable is applicable only in DEVICE mode */
	if (enable && chip->current_role != USB_ROLE_DEVICE)
		return -EINVAL;

	if (enable) {
		ret = enable_eud(chip);
		if (ret) {
			dev_err(chip->dev, "failed to enable eud\n");
			return count;
		}
	} else {
		disable_eud(chip);
	}
	chip->enabled = enable;

	return count;
}

static DEVICE_ATTR_RW(enable);

static struct attribute *eud_attrs[] = {
	&dev_attr_enable.attr,
	NULL,
};
ATTRIBUTE_GROUPS(eud);

static void usb_attach_detach(struct eud_chip *chip)
{
	u32 reg;

	/* read ctl_out_1[4] to find USB attach or detach event */
	reg = readl(chip->base + EUD_REG_CTL_OUT_1);
	chip->usb_attached = reg & EUD_INT_SAFE_MODE;
}

static void pet_eud(struct eud_chip *chip)
{
	u32 reg;
	int ret;

	/* When the EUD_INT_PET_EUD in SW_ATTACH_DET is set, the cable has been
	 * disconnected and we need to detach the pet to check if EUD is in safe
	 * mode before attaching again.
	 */
	reg = readl(chip->base + EUD_REG_SW_ATTACH_DET);
	if (reg & EUD_INT_PET_EUD) {
		/* Detach & Attach pet for EUD */
		writel(0, chip->base + EUD_REG_SW_ATTACH_DET);
		/* Delay to make sure detach pet is done before attach pet */
		ret = readl_poll_timeout(chip->base + EUD_REG_SW_ATTACH_DET,
					reg, (reg == 0), 1, 100);
		if (ret) {
			dev_err(chip->dev, "Detach pet failed\n");
			return;
		}
	}
	/* Attach pet for EUD */
	writel(EUD_INT_PET_EUD, chip->base + EUD_REG_SW_ATTACH_DET);
}

static irqreturn_t handle_eud_irq(int irq, void *data)
{
	struct eud_chip *chip = data;
	u32 reg;

	reg = readl(chip->base + EUD_REG_INT_STATUS_1);
	switch (reg & EUD_INT_ALL) {
	case EUD_INT_VBUS:
		usb_attach_detach(chip);
		return IRQ_WAKE_THREAD;
	case EUD_INT_SAFE_MODE:
		pet_eud(chip);
		return IRQ_HANDLED;
	default:
		return IRQ_NONE;
	}
}

static irqreturn_t handle_eud_irq_thread(int irq, void *data)
{
	struct eud_chip *chip = data;
	int ret;

	if (chip->usb_attached)
		ret = eud_usb_role_set(chip, USB_ROLE_DEVICE);
	else
		ret = eud_usb_role_set(chip, USB_ROLE_HOST);

	/* set and clear vbus_int_clr[0] to clear interrupt */
	writel(BIT(0), chip->base + EUD_REG_VBUS_INT_CLR);
	writel(0, chip->base + EUD_REG_VBUS_INT_CLR);

	return IRQ_HANDLED;
}

static int eud_usb_role_switch_set(struct usb_role_switch *sw,
				   enum usb_role role)
{
	struct eud_chip *chip = usb_role_switch_get_drvdata(sw);

	return eud_usb_role_set(chip, role);
}

static int eud_probe(struct platform_device *pdev)
{
	struct eud_chip *chip;
	struct usb_role_switch_desc eud_role_switch = {NULL};
	int ret;

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->dev = &pdev->dev;

	chip->usb2_phy = devm_phy_get(chip->dev, "usb2-phy");
	if (IS_ERR(chip->usb2_phy))
		return dev_err_probe(chip->dev, PTR_ERR(chip->usb2_phy),
				     "no usb2 phy configured\n");

	chip->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(chip->base))
		return PTR_ERR(chip->base);

	chip->mode_mgr = devm_platform_ioremap_resource(pdev, 1);
	if (IS_ERR(chip->mode_mgr))
		return PTR_ERR(chip->mode_mgr);

	chip->irq = platform_get_irq(pdev, 0);
	if (chip->irq < 0)
		return chip->irq;

	ret = devm_request_threaded_irq(&pdev->dev, chip->irq, handle_eud_irq,
			handle_eud_irq_thread, IRQF_ONESHOT, NULL, chip);
	if (ret)
		return dev_err_probe(chip->dev, ret, "failed to allocate irq\n");

	eud_role_switch.fwnode = dev_fwnode(chip->dev);
	eud_role_switch.set = eud_usb_role_switch_set;
	eud_role_switch.get = NULL;
	eud_role_switch.driver_data = chip;
	chip->role_sw = usb_role_switch_register(chip->dev, &eud_role_switch);

	if (IS_ERR(chip->role_sw))
		return dev_err_probe(chip->dev, PTR_ERR(chip->role_sw),
				"failed to register role switch\n");

	mutex_init(&chip->mutex);

	enable_irq_wake(chip->irq);

	platform_set_drvdata(pdev, chip);

	return 0;
}

static void eud_remove(struct platform_device *pdev)
{
	struct eud_chip *chip = platform_get_drvdata(pdev);

	if (chip->enabled)
		disable_eud(chip);

	if (chip->role_sw)
		usb_role_switch_unregister(chip->role_sw);

	device_init_wakeup(&pdev->dev, false);
	disable_irq_wake(chip->irq);
}

static const struct of_device_id eud_dt_match[] = {
	{ .compatible = "qcom,eud" },
	{ }
};
MODULE_DEVICE_TABLE(of, eud_dt_match);

static struct platform_driver eud_driver = {
	.probe	= eud_probe,
	.remove_new = eud_remove,
	.driver	= {
		.name = "qcom_eud",
		.dev_groups = eud_groups,
		.of_match_table = eud_dt_match,
	},
};
module_platform_driver(eud_driver);

MODULE_DESCRIPTION("QTI EUD driver");
MODULE_LICENSE("GPL v2");
