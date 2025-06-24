/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2025 Qualcomm Innovation Center, Inc. All rights reserved.
 */
#ifndef _LINUX_QCOM_QUP_FW_LOAD
#define _LINUX_QCOM_QUP_FW_LOAD

#include <linux/kernel.h>

/*Magic numbers*/
#define MAGIC_NUM_SE			0x57464553

#define MAX_GENI_CFG_RAMn_CNT		455

#define MI_PBT_NON_PAGED_SEGMENT	0x0
#define MI_PBT_HASH_SEGMENT		0x2
#define MI_PBT_NOTUSED_SEGMENT		0x3
#define MI_PBT_SHARED_SEGMENT		0x4

#define MI_PBT_FLAG_PAGE_MODE		BIT(20)
#define MI_PBT_FLAG_SEGMENT_TYPE	GENMASK(26, 24)
#define MI_PBT_FLAG_ACCESS_TYPE		GENMASK(23, 21)

#define MI_PBT_PAGE_MODE_VALUE(x) FIELD_GET(MI_PBT_FLAG_PAGE_MODE, x)

#define MI_PBT_SEGMENT_TYPE_VALUE(x) FIELD_GET(MI_PBT_FLAG_SEGMENT_TYPE, x)

#define MI_PBT_ACCESS_TYPE_VALUE(x) FIELD_GET(MI_PBT_FLAG_ACCESS_TYPE, x)

#define M_COMMON_GENI_M_IRQ_EN	(GENMASK(6, 1) | \
				M_IO_DATA_DEASSERT_EN | \
				M_IO_DATA_ASSERT_EN | M_RX_FIFO_RD_ERR_EN | \
				M_RX_FIFO_WR_ERR_EN | M_TX_FIFO_RD_ERR_EN | \
				M_TX_FIFO_WR_ERR_EN)

/* DMA_TX/RX_IRQ_EN fields */
#define DMA_DONE_EN		BIT(0)
#define SBE_EN			BIT(2)
#define RESET_DONE_EN		BIT(3)
#define FLUSH_DONE_EN		BIT(4)

/* GENI_CLK_CTRL fields */
#define SER_CLK_SEL		BIT(0)

/* GENI_DMA_IF_EN fields */
#define DMA_IF_EN		BIT(0)

#define QUPV3_COMMON_CFG		0x120
#define FAST_SWITCH_TO_HIGH_DISABLE	BIT(0)

#define QUPV3_SE_AHB_M_CFG		0x118
#define AHB_M_CLK_CGC_ON		BIT(0)

#define QUPV3_COMMON_CGC_CTRL		0x21C
#define COMMON_CSR_SLV_CLK_CGC_ON	BIT(0)

/* access ports */
#define geni_setbits32(_addr, _v) writel_relaxed(readl_relaxed(_addr) |  (_v), _addr)
#define geni_clrbits32(_addr, _v) writel_relaxed(readl_relaxed(_addr) & ~(_v), _addr)

/**
 * struct se_fw_hdr - Serial Engine firmware configuration header
 *
 * This structure defines metadata for the Serial Engine (SE) firmware
 * configuration. Although it is embedded within an ELF segment, it is
 * not part of the ELF format itself.
 *
 * @magic: Set to 'SEFW'
 * @version: Structure version number
 * @core_version: QUPV3 hardware version
 * @serial_protocol: Encoded in GENI_FW_REVISION
 * @fw_version: Firmware version, from GENI_FW_REVISION
 * @cfg_version: Configuration version, from GENI_INIT_CFG_REVISION
 * @fw_size_in_items: Number of 32-bit words in GENI_FW_RAM
 * @fw_offset: Byte offset to GENI_FW_RAM array
 * @cfg_size_in_items: Number of GENI_FW_CFG index/value pairs
 * @cfg_idx_offset: Byte offset to GENI_FW_CFG index array
 * @cfg_val_offset: Byte offset to GENI_FW_CFG values array
 */
struct se_fw_hdr {
	__le32 magic;
	u32 version;
	u32 core_version;
	u16 serial_protocol;
	u16 fw_version;
	u16 cfg_version;
	u16 fw_size_in_items;
	u16 fw_offset;
	u16 cfg_size_in_items;
	u16 cfg_idx_offset;
	u16 cfg_val_offset;
};
#endif /* _LINUX_QCOM_QUP_FW_LOAD */
