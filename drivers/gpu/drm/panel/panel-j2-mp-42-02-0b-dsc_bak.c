// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2025 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>

#include <drm/display/drm_dsc.h>
#include <drm/display/drm_dsc_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct j2_mp_42_02_0b_dsc {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct drm_dsc_config dsc;
	struct gpio_desc *reset_gpio;
	// wtf
	struct gpio_desc *disprate_gpio;
};

static inline
struct j2_mp_42_02_0b_dsc *to_j2_mp_42_02_0b_dsc(struct drm_panel *panel)
{
	return container_of(panel, struct j2_mp_42_02_0b_dsc, panel);
}

static void j2_mp_42_02_0b_dsc_reset(struct j2_mp_42_02_0b_dsc *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
}

static int j2_mp_42_02_0b_dsc_on(struct j2_mp_42_02_0b_dsc *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc0, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd4, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc0, 0x33);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb2, 0x58, 0x00, 0x08, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xca, 0x30);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc6, 0x13);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd2,
				     0x00, 0x12, 0x61, 0x25, 0x43, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb7,
				     0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb8, 0x48, 0x48, 0x48, 0x48);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb3, 0x13);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb4, 0x13);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xcd, 0x05, 0x61);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb3, 0x88, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5, 0x04, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb7, 0x04, 0x00, 0x00, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb8, 0x04, 0x00, 0x00, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0xaa, 0x55, 0xa5, 0x81);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x0d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf3, 0xab);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfd, 0x00, 0xda);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0xaa, 0x55, 0xa5, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x0a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x36);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf6, 0x42);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x00, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x3b, 0x00, 0x0c, 0x00, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x90, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x93,
				     0x89, 0x28, 0x00, 0x0c, 0x02, 0x00, 0x02,
				     0x0e, 0x01, 0x1f, 0x00, 0x07, 0x08, 0xbb,
				     0x08, 0x7a, 0x10, 0xf0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x03, 0x11);
	// mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2c, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x35, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x53, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x51, 0x00, 0x00, 0x00, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2a, 0x00, 0x00, 0x04, 0x37);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2b, 0x00, 0x00, 0x09, 0x23);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb4,
				     0xcb, 0xbb, 0xbb, 0xaa, 0x99, 0x77, 0x66,
				     0x00, 0x00, 0x00, 0xb4, 0xd0, 0xd0, 0xd0,
				     0xd0, 0xd0, 0x86, 0x86, 0x3c, 0x3c, 0xf2,
				     0xf2, 0xa8, 0xa8, 0x36, 0x36, 0x36, 0x36,
				     0x0a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2f, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x26, 0x01);
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	// mipi_dsi_msleep(&dsi_ctx, 100);
	mipi_dsi_msleep(&dsi_ctx, 180);
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0,
				     0x55, 0xaa, 0x52, 0x08, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5, 0x84);

	return dsi_ctx.accum_err;
}

static int j2_mp_42_02_0b_dsc_off(struct j2_mp_42_02_0b_dsc *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);

	return dsi_ctx.accum_err;
}

static int j2_mp_42_02_0b_dsc_prepare(struct drm_panel *panel)
{
	struct j2_mp_42_02_0b_dsc *ctx = to_j2_mp_42_02_0b_dsc(panel);
	struct device *dev = &ctx->dsi->dev;
	struct drm_dsc_picture_parameter_set pps;
	int ret;

	j2_mp_42_02_0b_dsc_reset(ctx);

	ret = j2_mp_42_02_0b_dsc_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		return ret;
	}

	drm_dsc_pps_payload_pack(&pps, &ctx->dsc);

	ret = mipi_dsi_picture_parameter_set(ctx->dsi, &pps);
	if (ret < 0) {
		dev_err(panel->dev, "failed to transmit PPS: %d\n", ret);
		return ret;
	}

	// ret = mipi_dsi_compression_mode(ctx->dsi, true);
	// if (ret < 0) {
	// 	dev_err(dev, "failed to enable compression mode: %d\n", ret);
	// 	return ret;
	// }

	// msleep(28); /* TODO: Is this panel-dependent? */

	msleep(128); /* TODO: Is this panel-dependent? */
	gpiod_set_value_cansleep(ctx->disprate_gpio, 1);

	return 0;
}

static int j2_mp_42_02_0b_dsc_unprepare(struct drm_panel *panel)
{
	struct j2_mp_42_02_0b_dsc *ctx = to_j2_mp_42_02_0b_dsc(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = j2_mp_42_02_0b_dsc_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);

	return 0;
}

static const struct drm_display_mode j2_mp_42_02_0b_dsc_mode = {
	.clock = (1080 + 16 + 8 + 8) * (2340 + 600 + 32 + 560) * 60 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 16,
	.hsync_end = 1080 + 16 + 8,
	.htotal = 1080 + 16 + 8 + 8,
	.vdisplay = 2340,
	.vsync_start = 2340 + 600,
	.vsync_end = 2340 + 600 + 32,
	.vtotal = 2340 + 600 + 32 + 560,
	.width_mm = 710,
	.height_mm = 1537,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int j2_mp_42_02_0b_dsc_get_modes(struct drm_panel *panel,
					struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &j2_mp_42_02_0b_dsc_mode);
}

static const struct drm_panel_funcs j2_mp_42_02_0b_dsc_panel_funcs = {
	.prepare = j2_mp_42_02_0b_dsc_prepare,
	.unprepare = j2_mp_42_02_0b_dsc_unprepare,
	.get_modes = j2_mp_42_02_0b_dsc_get_modes,
};

static int j2_mp_42_02_0b_dsc_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct j2_mp_42_02_0b_dsc *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");
	
	ctx->disprate_gpio = devm_gpiod_get(dev, "disprate", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->disprate_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->disprate_gpio),
						"Failed to get disprate-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO_BURST |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS | MIPI_DSI_MODE_LPM;

	drm_panel_init(&ctx->panel, dev, &j2_mp_42_02_0b_dsc_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	ctx->panel.prepare_prev_first = true;

	drm_panel_add(&ctx->panel);

	/* This panel only supports DSC; unconditionally enable it */
	dsi->dsc = &ctx->dsc;

	ctx->dsc.dsc_version_major = 1;
	ctx->dsc.dsc_version_minor = 1;

	/* TODO: Pass slice_per_pkt = 2 */
	ctx->dsc.slice_height = 12;
	ctx->dsc.slice_width = 540;
	/*
	 * TODO: hdisplay should be read from the selected mode once
	 * it is passed back to drm_panel (in prepare?)
	 */
	WARN_ON(1080 % ctx->dsc.slice_width);
	ctx->dsc.slice_count = 1080 / ctx->dsc.slice_width;
	ctx->dsc.bits_per_component = 8;
	ctx->dsc.bits_per_pixel = 8 << 4; /* 4 fractional bits */
	ctx->dsc.block_pred_enable = true;

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void j2_mp_42_02_0b_dsc_remove(struct mipi_dsi_device *dsi)
{
	struct j2_mp_42_02_0b_dsc *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id j2_mp_42_02_0b_dsc_of_match[] = {
	{ .compatible = "mdss,j2-mp-42-02-0b-dsc" }, // FIXME
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, j2_mp_42_02_0b_dsc_of_match);

static struct mipi_dsi_driver j2_mp_42_02_0b_dsc_driver = {
	.probe = j2_mp_42_02_0b_dsc_probe,
	.remove = j2_mp_42_02_0b_dsc_remove,
	.driver = {
		.name = "panel-j2-mp-42-02-0b-dsc",
		.of_match_table = j2_mp_42_02_0b_dsc_of_match,
	},
};
module_mipi_dsi_driver(j2_mp_42_02_0b_dsc_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for xiaomi 42 02 0b cmd mode dsc dsi panel");
MODULE_LICENSE("GPL");
