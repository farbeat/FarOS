/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier :  Apache-2.0
 *
 * Change Logs :
 * Date           Author        Notes
 * 2011-07-25     weety        first version
 */

#ifndef __MMCSD_CARD_H__
#define __MMCSD_CARD_H__

#include "mmcsd_host.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SD_SCR_BUS_WIDTH_1    (1 << 0)
#define SD_SCR_BUS_WIDTH_4    (1 << 2)
#define EXT_CSD_LEN_BY_4BYTES    128

struct rt_mmcsd_cid {
    uint8_t  mid;       /* ManufacturerID */
    uint8_t  prv;       /* Product Revision */
    uint16_t oid;       /* OEM/Application ID */
    uint32_t psn;       /* Product Serial Number */
    uint8_t  pnm[5];    /* Product Name */
    uint8_t  reserved1; /* reserved */
    uint16_t mdt;       /* Manufacturing Date */
    uint8_t  crc;       /* CID CRC */
    uint8_t  reserved2; /* not used, always 1 */
};

struct rt_mmcsd_csd {
    uint8_t        csd_structure;    /* CSD register version */
    uint8_t      spec_vers;
    uint8_t        taac;
    uint8_t        nsac;
    uint8_t        tran_speed;    /* max data transfer rate */
    uint16_t        card_cmd_class;    /* card command classes */
    uint8_t        rd_blk_len;    /* max read data block length */
    uint8_t        rd_blk_part;
    uint8_t        wr_blk_misalign;
    uint8_t        rd_blk_misalign;
    uint8_t        dsr_imp;    /* DSR implemented */
    uint8_t        c_size_mult;    /* CSD 1.0 , device size multiplier */
    uint32_t        c_size;        /* device size */
    uint8_t        r2w_factor;
    uint8_t        wr_blk_len;    /* max wtire data block length */
    uint8_t        wr_blk_partial;
    uint8_t        csd_crc;
};

struct rt_sd_scr {
    uint8_t        sd_version;
    uint8_t        sd_bus_widths;
};

struct rt_sdio_cccr {
    uint8_t        sdio_version;
    uint8_t        sd_version;
    uint8_t        direct_cmd  : 1,     /*  Card Supports Direct Commands during data transfer
                                                   only SD mode, not used for SPI mode */
                multi_block : 1,    /*  Card Supports Multi-Block */
                read_wait : 1,      /*  Card Supports Read Wait
                                       only SD mode, not used for SPI mode */
                suspend_resume : 1, /*  Card supports Suspend/Resume
                                       only SD mode, not used for SPI mode */
                s4mi : 1,            /* generate interrupts during a 4-bit
                                      multi-block data transfer */
                e4mi : 1,            /*  Enable the multi-block IRQ during
                                       4-bit transfer for the SDIO card */
                low_speed : 1,      /*  Card  is  a  Low-Speed  card */
                low_speed_4 : 1;    /*  4-bit support for Low-Speed cards */

    uint8_t        bus_width : 1,     /* Support SDIO bus width, 1 : 4bit, 0 : 1bit */
                cd_disable : 1,    /*  Connect[0]/Disconnect[1] the 10K-90K ohm pull-up
                                     resistor on CD/DAT[3] (pin 1) of the card */
                power_ctrl : 1,    /* Support Master Power Control */
                high_speed : 1;    /* Support High-Speed  */
};

struct rt_sdio_cis {
    uint16_t        manufacturer;
    uint16_t        product;
    uint16_t        func0_blk_size;
    uint32_t        max_tran_speed;
};

/*
 * SDIO function CIS tuple (unknown to the core)
 */
struct rt_sdio_function_tuple {
    struct rt_sdio_function_tuple *next;
    uint8_t code;
    uint8_t size;
    uint8_t *data;
};

struct rt_sdio_function;
typedef void (rt_sdio_irq_handler_t)(struct rt_sdio_function *);

/*
 * SDIO function devices
 */
struct rt_sdio_function {
    struct rt_mmcsd_card        *card;        /* the card this device belongs to */
    rt_sdio_irq_handler_t    *irq_handler;    /* IRQ callback */
    uint8_t        num;        /* function number */

    uint8_t        func_code;   /*  Standard SDIO Function interface code  */
    uint16_t        manufacturer;        /* manufacturer id */
    uint16_t        product;        /* product id */

    uint32_t        max_blk_size;    /* maximum block size */
    uint32_t        cur_blk_size;    /* current block size */

    uint32_t        enable_timeout_val; /* max enable timeout in msec */

    struct rt_sdio_function_tuple *tuples;
    
    void            *priv;
};

#define SDIO_MAX_FUNCTIONS        7

struct rt_mmcsd_ext_csd {
    uint8_t         rev;
    uint8_t         erase_group_def;
    uint8_t         sec_feature_support;
    uint8_t         rel_sectors;
    uint8_t         rel_param;
    uint8_t         part_config;
    uint8_t         cache_ctrl;
    uint8_t         rst_n_function;
    uint32_t        part_time_ms;            /* ms */
    uint32_t        sa_timeout_ns;           /* 100ns */
    uint32_t        generic_cmd6_time_ms;    /* 10ms */
    uint32_t        power_off_longtime_ms;   /* ms */
    uint32_t        hs_max_dtr;
    uint32_t        sectors;
    uint32_t        card_type;
    uint32_t        hc_erase_size;           /* In sectors */
    uint32_t        hc_erase_timeout;        /* In milliseconds */
    uint32_t        sec_trim_mult;           /* Secure trim multiplier  */
    uint32_t        sec_erase_mult;          /* Secure erase multiplier */
    uint32_t        trim_timeout;            /* In milliseconds */
    bool            enhanced_area_en;        /* enable bit */
    uint64_t        enhanced_area_offset;    /* Units :  Byte */
    uint32_t        enhanced_area_size;      /* Units :  KB */
    uint32_t        cache_size;              /* Units :  KB */
    bool            hpi_en;                  /* HPI enablebit */
    bool            hpi;                     /* HPI support bit */
    uint32_t     hpi_cmd;                 /* cmd used as HPI */
    uint32_t     data_sector_size;        /* 512 bytes or 4KB */
    uint32_t     data_tag_unit_size;      /* DATA TAG UNIT size */
    uint32_t     boot_ro_lock;            /* ro lock support */
    uint32_t     boot_size;
    uint8_t         raw_partition_support;   /* 160 */
    uint8_t         raw_erased_mem_count;    /* 181 */
    uint8_t         strobe_support;          /* 184 */
    uint8_t         raw_ext_csd_structure;   /* 194 */
    uint8_t         raw_card_type;           /* 196 */
    uint8_t         out_of_int_time;         /* 198 */
    uint8_t         raw_s_a_timeout;         /* 217 */
    uint8_t         raw_hc_erase_gap_size;   /* 221 */
    uint8_t         raw_erase_timeout_mult;  /* 223 */
    uint8_t         raw_hc_erase_grp_size;   /* 224 */
    uint8_t         raw_sec_trim_mult;       /* 229 */
    uint8_t         raw_sec_erase_mult;      /* 230 */
    uint8_t         raw_sec_feature_support; /* 231 */
    uint8_t         raw_trim_mult;           /* 232 */
    uint8_t         raw_sectors[4];          /* 212 - 4 bytes */
};


struct rt_mmcsd_card {
    struct rt_mmcsd_host *host;
    uint32_t    rca;        /* card addr */
    uint32_t    resp_cid[4];    /* card CID register */
    uint32_t    resp_csd[4];    /* card CSD register */
    uint32_t    resp_scr[2];    /* card SCR register */

    uint16_t    tacc_clks;    /* data access time by ns */
    uint32_t    tacc_ns;    /* data access time by clk cycles */
    uint32_t    max_data_rate;    /* max data transfer rate */
    uint32_t    card_capacity;    /* card capacity, unit : KB */
    uint32_t    card_blksize;    /* card block size */
    uint32_t    erase_size;    /* erase size in sectors */
    uint16_t    card_type;
#define CARD_TYPE_MMC                   0 /* MMC card */
#define CARD_TYPE_SD                    1 /* SD card */
#define CARD_TYPE_SDIO                  2 /* SDIO card */
#define CARD_TYPE_SDIO_COMBO            3 /* SD combo (IO+mem) card */

    uint16_t flags;
#define CARD_FLAG_HIGHSPEED  (1 << 0)   /* SDIO bus speed 50MHz */
#define CARD_FLAG_SDHC       (1 << 1)   /* SDHC card */
#define CARD_FLAG_SDXC       (1 << 2)   /* SDXC card */

    struct rt_sd_scr    scr;
    struct rt_mmcsd_csd    csd;
    uint32_t     hs_max_data_rate;  /* max data transfer rate in high speed mode */
    struct rt_mmcsd_ext_csd ext_csd;
    uint32_t     ext_csd_val[EXT_CSD_LEN_BY_4BYTES];
    uint8_t      sdio_function_num;    /* totol number of SDIO functions */
    struct rt_sdio_cccr    cccr;  /* common card info */
    struct rt_sdio_cis     cis;  /* common tuple info */
    struct rt_sdio_function    *sdio_function[SDIO_MAX_FUNCTIONS + 1]; /* SDIO functions (devices) */
};

#ifdef __cplusplus
}
#endif

#endif
