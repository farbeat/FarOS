/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: dsp reg
 * Author: audio
 */

#ifndef __DSP_REG_H__
#define __DSP_REG_H__

#define DSP_CTRL_COMMON 0xF8A20178
#define DSP_CTRL0_ADDR 0xF8A20184
#define PERI_CRG310_ADDR 0xF8A224D8

/* Define the union U_PERI_CRG310 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: dsp0_cken
        /*
        dsp0时钟门控：
        0：关断；
        1：打开。
        */
        unsigned int    dsp0_cken             : 1; /* [0] */
        unsigned int    reserved_0            : 15; /* [15..1] */
        // Comment of field: dsp0_srst_req
        /*
        dsp0软复位请求：
        0：不复位；
        1：复位。
        */
        unsigned int    dsp0_srst_req         : 1; /* [16] */
        // Comment of field: dsp0_dbg_srst_req
        /*
        dsp0_dbg软复位请求：
        0：不复位；
        1：复位。
        */
        unsigned int    dsp0_dbg_srst_req     : 1; /* [17] */
        // Comment of field: dsp0_mmu_srst_req
        /*
        dsp0 mmu软复位请求：
        0：不复位；
        1：复位。
        */
        unsigned int    dsp0_mmu_srst_req     : 1; /* [18] */
        unsigned int    reserved_1            : 5; /* [23..19] */
        // Comment of field: dsp0_cksel
        /*
        dsp0时钟选择：
        0：600M；
        1：540M；
        2：432M;
        3: KPLL备份时钟。
        */
        unsigned int    dsp0_cksel            : 2; /* [25..24] */
        // Comment of field: reserved_2
        /*
        保留。
        */
        unsigned int    reserved_2            : 6; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_peri_crg310;

/* Define the union U_DSP_CTRL0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        保留。
        */
        unsigned int    reserved_0            : 5; /* [4..0] */
        // Comment of field: reserved_1
        /*
        DSP MFDE MMU START信号
        */
        unsigned int    reserved_1            : 1; /* [5] */
        // Comment of field: reserved_2
        /*
        手动配置dsp0 runstall信号。如果用户需要手动配置runstall信号，需要配置dsp0_runstall_en=0
        */
        unsigned int    reserved_2            : 1; /* [6] */
        // Comment of field: reserved_3
        /*
        DSP是否接受breakin信号
        */
        unsigned int    reserved_3            : 1; /* [7] */
        // Comment of field: peri_runstall_dsp0
        /*
        DSP是否接受runstall信号
        */
        unsigned int    peri_runstall_dsp0    : 1; /* [8] */
        // Comment of field: peri_statvectorsel_dsp0
        /*
        select start vector。
        0：default（0xfa820400）；
        1：alternative（0x03800000）。
        */
        unsigned int    peri_statvectorsel_dsp0 : 1; /* [9] */
        // Comment of field: peri_ocdhaltonreset_dsp0
        /*
        复位后的debug模式选择：
        0：复位后进入Normal模式；
        1：复位后进入OCD模式。
        */
        unsigned int    peri_ocdhaltonreset_dsp0 : 1; /* [10] */
        // Comment of field: reserved_4
        /*
        保留。
        */
        unsigned int    reserved_4            : 1; /* [11] */
        // Comment of field: peri_dsp0_wdg_en
        /*
        使能WDG(For DSP)。
        0：不使能；
        1：使能。
        */
        unsigned int    peri_dsp0_wdg_en      : 1; /* [12] */
        // Comment of field: peri_syn_dbg_sel_dsp0
        /*
        DSP dbg同步选择信号，当需要dbg通过JTAG连接仿真器时需要将此bit配为1
        */
        unsigned int    peri_syn_dbg_sel_dsp0 : 1; /* [13] */
        // Comment of field: reserved_5
        /*
        保留。
        */
        unsigned int    reserved_5            : 18; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_dsp_ctrl0;

/* Define the union U_DSP_CTRL_COMMON */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        dsp0和dsp1联合调试的方式
        0：xocdmode与操作。这是xtensa给的连接方式
        1：xocdmode或操作
        */
        unsigned int    reserved_0            : 1; /* [0] */
        // Comment of field: reserved_1
        /*
        dsp0和dsp1联合调试的方式
        0：breakout与操作。这是xtensa给的连接方式
        1：breakout或操作
        */
        unsigned int    reserved_1            : 1; /* [1] */
        // Comment of field: reserved_2
        /*
        reserved
        */
        unsigned int    reserved_2            : 1; /* [2] */
        // Comment of field: reserved_3
        /*
        reserved
        */
        unsigned int    reserved_3            : 13; /* [15..3] */
        // Comment of field: reserved_4
        /*
        reserved
        */
        unsigned int    reserved_4            : 2; /* [17..16] */
        // Comment of field: reserved_5
        /*
        reserved
        */
        unsigned int    reserved_5            : 2; /* [19..18] */
        // Comment of field: reserved_6
        /*
        reserved
        */
        unsigned int    reserved_6            : 2; /* [21..20] */
        // Comment of field: reserved_7
        /*
        reserved
        */
        unsigned int    reserved_7            : 2; /* [23..22] */
        // Comment of field: reserved_8
        /*
        保留。
        */
        unsigned int    reserved_8            : 8; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_dsp_ctrl_common;

/* Define the union u_dsp_work_on */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    dsp_work_on           : 1  ; /* [0]  */
        unsigned int    reserved_0            : 31 ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_dsp_work_on;

#endif /* __DSP_REG_H__ */
