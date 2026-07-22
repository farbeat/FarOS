/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: audio crg clk
 * Author: audio
 */

#ifndef __AUDIO_CRG_REG_H__
#define __AUDIO_CRG_REG_H__

/* Define the union u_hifi_cr_ch1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hifi_ch1_cr_ctrl_ch_en : 1;   /* [0] */
        unsigned int hifi_ch1_cr_ctrl_clk_sel : 3; /* [3:1] */
        unsigned int hifi_ch1_cr_ctrl_div_en : 1;  /* [4] */
        unsigned int hifi_ch1_cr_ctrl_div_num : 4; /* [8:5] */
        unsigned int reserved_0 : 3;               /* [11:9] */
        unsigned int hifi_ch_sel_sts : 2;          /* [13:12] */
        unsigned int reserved_1 : 18;              /* [31:14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hifi_cr_ch1;

/* Define the union u_ch_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int com_bus_ch_sel : 1; /* [0] */
        unsigned int hifi_ch_sel : 1;    /* [1] */
        unsigned int codec_ch_sel : 1;   /* [2] */
        unsigned int reserved : 29;      /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_ch_sel;

#endif /* __AUDIO_CRG_REG_H__ */
