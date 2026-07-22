/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: mipi tx register header file.
 * Author: @CompanyNameTag
 * Create: 2022-04-14
 */

#ifndef __MIPI_TX_REG_H__
#define __MIPI_TX_REG_H__

/* define the union reg_ctrl_reset */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t ctrl_reset : 1;  /* [0]  */
        uint32_t reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ctrl_reset;

/* define the union reg_crg_cfg */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t txescclk_div : 8; /* [7..0]  */
        uint32_t toclk_div : 8;    /* [15..8]  */
        uint32_t reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_crg_cfg;

/* define the union reg_video_vc */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vcid : 2;        /* [1..0]  */
        uint32_t reserved_0 : 2;  /* [3..2]  */
        uint32_t vcid_sel : 1;    /* [4]  */
        uint32_t reserved_1 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_video_vc;

/* define the union reg_data_type */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t data_type : 8;   /* [7..0]  */
        uint32_t reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_data_type;

/* define the union reg_lp_cmd_byte */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t invact_lpcmd_byte : 8;  /* [7..0]  */
        uint32_t reserved_0 : 8;         /* [15..8]  */
        uint32_t outvact_lpcmd_byte : 8; /* [23..16]  */
        uint32_t reserved_1 : 8;         /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lp_cmd_byte;

/* define the union reg_pck_en */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t eotp_tx_en : 1;   /* [0]  */
        uint32_t eotp_rx_en : 1;   /* [1]  */
        uint32_t crc_rx_en : 1;    /* [2]  */
        uint32_t ecc_rx_en : 1;    /* [3]  */
        uint32_t bta_en : 1;       /* [4]  */
        uint32_t tear_fx_en : 1;   /* [5]  */
        uint32_t ack_rqst_en : 1;  /* [6]  */
        uint32_t frame_ack_en : 1; /* [7]  */
        uint32_t reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_pck_en;

/* define the union reg_gen_vc */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t gen_vcid : 2;    /* [1..0]  */
        uint32_t reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_gen_vc;

/* define the union reg_mode_cfg */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t cmd_video_mode : 1;     /* [0]  */
        uint32_t reserved_0 : 3;         /* [3..1]  */
        uint32_t video_mode_tran_type : 1; /* [4]  */
        uint32_t reserved_1 : 3;         /* [7..5]  */
        uint32_t video_mode_type : 2;    /* [9..8]  */
        uint32_t reserved_2 : 2;         /* [11..10]  */
        uint32_t dualpix_en : 1;         /* [12]  */
        uint32_t reserved_3 : 7;         /* [19..13]  */
        uint32_t cphy_ppi_dw_sel : 1;    /* [20]  */
        uint32_t reserved_4 : 11;        /* [31..21]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_mode_cfg;

/* define the union reg_video_lp_en */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vsa_lp_en : 1;   /* [0]  */
        uint32_t vbp_lp_en : 1;   /* [1]  */
        uint32_t vfp_lp_en : 1;   /* [2]  */
        uint32_t vact_lp_en : 1;  /* [3]  */
        uint32_t hbp_lp_en : 1;   /* [4]  */
        uint32_t hfp_lp_en : 1;   /* [5]  */
        uint32_t reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_video_lp_en;

/* define the union reg_videom_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t videom_pkt_size : 14; /* [13..0]  */
        uint32_t reserved_0 : 18;      /* [31..14]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_pkt_size;

/* define the union reg_videom_num_chunks */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t videom_num_chunks : 13; /* [12..0]  */
        uint32_t reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_num_chunks;

/* define the union reg_videom_null_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t videom_null_size : 13; /* [12..0]  */
        uint32_t reserved_0 : 19;       /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_null_size;

/* define the union reg_videom_hsa_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t videom_hsa_size : 12; /* [11..0]  */
        uint32_t reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_hsa_size;

/* define the union reg_videom_hbp_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t videom_hbp_size : 12; /* [11..0]  */
        uint32_t reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_hbp_size;

/* define the union reg_videom_hline_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t video_hline_size : 15; /* [14..0]  */
        uint32_t reserved_0 : 17;       /* [31..15]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_hline_size;

/* define the union reg_videom_vsa_lines */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vsa_lines : 10;  /* [9..0]  */
        uint32_t reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_vsa_lines;

/* define the union reg_videom_vbp_lines */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vbp_lines : 10;  /* [9..0]  */
        uint32_t reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_vbp_lines;

/* define the union reg_videom_vfp_lines */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vfp_lines : 10;  /* [9..0]  */
        uint32_t reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_vfp_lines;

/* define the union reg_videom_vactive_lines */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t v_active_lines : 14; /* [13..0]  */
        uint32_t reserved_0 : 18;     /* [31..14]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_videom_vactive_lines;

/* define the union reg_command_pkt_size */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t cmd_pkt_size : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_command_pkt_size;

/* define the union reg_command_tran_mode */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t max_rd_pkt_size_tran : 1; /* [0]  */
        uint32_t gen_sw_0p : 1;            /* [1]  */
        uint32_t gen_sw_1p : 1;            /* [2]  */
        uint32_t gen_sw_2p : 1;            /* [3]  */
        uint32_t gen_sr_0p : 1;            /* [4]  */
        uint32_t gen_sr_1p : 1;            /* [5]  */
        uint32_t gen_sr_2p : 1;            /* [6]  */
        uint32_t gen_lw : 1;               /* [7]  */
        uint32_t dcs_sw_0p : 1;            /* [8]  */
        uint32_t dcs_sw_1p : 1;            /* [9]  */
        uint32_t dcs_sr_0p : 1;            /* [10]  */
        uint32_t dcs_lw : 1;               /* [11]  */
        uint32_t cfg_frmtail_gencmd : 1;   /* [12]  */
        uint32_t dpu2mipi_cmd_en : 1;      /* [13]  */
        uint32_t reserved_0 : 18;          /* [31..14]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_command_tran_mode;

/* define the union reg_command_header */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t command_datatype : 6;       /* [5..0]  */
        uint32_t command_virtualchannel : 2; /* [7..6]  */
        uint32_t command_wordcount_low : 8;  /* [15..8]  */
        uint32_t command_wordcount_hi : 8;   /* [23..16]  */
        uint32_t reserved_0 : 8;             /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_command_header;

/* define the union reg_command_payload */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t command_pld_b1 : 8; /* [7..0]  */
        uint32_t command_pld_b2 : 8; /* [15..8]  */
        uint32_t command_pld_b3 : 8; /* [23..16]  */
        uint32_t command_pld_b4 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_command_payload;

/* define the union reg_command_status */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t command_empty : 1;   /* [0]  */
        uint32_t command_full : 1;    /* [1]  */
        uint32_t pld_write_empty : 1; /* [2]  */
        uint32_t pld_write_full : 1;  /* [3]  */
        uint32_t pld_read_empty : 1;  /* [4]  */
        uint32_t pld_read_full : 1;   /* [5]  */
        uint32_t rd_cmd_busy : 1;     /* [6]  */
        uint32_t command_idle : 1;    /* [7]  */
        uint32_t reserved_0 : 24;     /* [31..8]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_command_status;

/* define the union reg_hs_lp_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lprx_to_cnt : 16; /* [15..0]  */
        uint32_t hstx_to_cnt : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hs_lp_to_set;

/* define the union reg_hsrd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hsrd_to_set : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hsrd_to_set;

/* define the union reg_lprd_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lprd_to_set : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lprd_to_set;

/* define the union reg_hswr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hswr_to_set : 16; /* [15..0]  */
        uint32_t reserved_0 : 8;   /* [23..16]  */
        uint32_t pre_to_mode : 1;  /* [24]  */
        uint32_t reserved_1 : 7;   /* [31..25]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hswr_to_set;

/* define the union reg_lpwr_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lpwr_to_set : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lpwr_to_set;

/* define the union reg_bta_to_set */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t bta_to_set : 16; /* [15..0]  */
        uint32_t reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_bta_to_set;

/* define the union reg_clklane_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t txrequesthsclk : 1;       /* [0]  */
        uint32_t clklane_continue : 1;     /* [1]  */
        uint32_t cphy_clklane_sel_qst : 1; /* [2]  */
        uint32_t reserved_0 : 29;          /* [31..3]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_clklane_ctrl;

/* define the union reg_clklane_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t clklane_lp2hs_time : 10; /* [9..0]  */
        uint32_t reserved_0 : 6;          /* [15..10]  */
        uint32_t clklane_hs2lp_time : 10; /* [25..16]  */
        uint32_t reserved_1 : 6;          /* [31..26]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_clklane_time;

/* define the union reg_datalane_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t datalane_lp2hs_time : 10; /* [9..0]  */
        uint32_t reserved_0 : 6;           /* [15..10]  */
        uint32_t datalane_hs2lp_time : 10; /* [25..16]  */
        uint32_t reserved_1 : 6;           /* [31..26]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_datalane_time;

/* define the union reg_phy_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t shutdownz : 1;   /* [0]  */
        uint32_t rstz : 1;        /* [1]  */
        uint32_t enableclk : 1;   /* [2]  */
        uint32_t forcepll : 1;    /* [3]  */
        uint32_t reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_ctrl;

/* define the union reg_lane_num */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lane_num : 2;       /* [1..0]  */
        uint32_t reserved_0 : 6;     /* [7..2]  */
        uint32_t stopstate_time : 8; /* [15..8]  */
        uint32_t reserved_1 : 16;    /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lane_num;

/* define the union reg_ulps_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t txulpsclklane : 1;      /* [0]  */
        uint32_t txulpsexitclklane : 1;  /* [1]  */
        uint32_t txulpsdatalane : 1;     /* [2]  */
        uint32_t txulpsexitdatalane : 1; /* [3]  */
        uint32_t reserved_0 : 28;        /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ulps_ctrl;

/* define the union reg_tx_triggers */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t tx_triggers : 4; /* [3..0]  */
        uint32_t reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_tx_triggers;

/* define the union reg_ppi_status */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t phy_pll_lock : 1;       /* [0]  */
        uint32_t phy_tran_direction : 1; /* [1]  */
        uint32_t stopstateclklane : 1;   /* [2]  */
        uint32_t ulpsactivenotclk : 1;   /* [3]  */
        uint32_t stopstate0lane : 1;     /* [4]  */
        uint32_t ulpsactivenot0lane : 1; /* [5]  */
        uint32_t rxulpsesc0lane : 1;     /* [6]  */
        uint32_t stopstate1lane : 1;     /* [7]  */
        uint32_t ulpsactivenot1lane : 1; /* [8]  */
        uint32_t stopstate2lane : 1;     /* [9]  */
        uint32_t ulpsactivenot2lane : 1; /* [10]  */
        uint32_t stopstate3lane : 1;     /* [11]  */
        uint32_t ulpsactivenot3lane : 1; /* [12]  */
        uint32_t reserved_0 : 19;        /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_status;

/* define the union reg_phy_reg_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t testclr : 1;     /* [0]  */
        uint32_t testclk : 1;     /* [1]  */
        uint32_t reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_reg_cfg0;

/* define the union reg_phy_reg_cfg1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t testdin : 8;     /* [7..0]  */
        uint32_t testdout : 8;    /* [15..8]  */
        uint32_t testen : 1;      /* [16]  */
        uint32_t reserved_0 : 15; /* [31..17]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_reg_cfg1;

/* define the union reg_int0_status */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t error_report_0 : 1;   /* [0]  */
        uint32_t error_report_1 : 1;   /* [1]  */
        uint32_t error_report_2 : 1;   /* [2]  */
        uint32_t error_report_3 : 1;   /* [3]  */
        uint32_t error_report_4 : 1;   /* [4]  */
        uint32_t error_report_5 : 1;   /* [5]  */
        uint32_t error_report_6 : 1;   /* [6]  */
        uint32_t error_report_7 : 1;   /* [7]  */
        uint32_t error_report_8 : 1;   /* [8]  */
        uint32_t error_report_9 : 1;   /* [9]  */
        uint32_t error_report_10 : 1;  /* [10]  */
        uint32_t error_report_11 : 1;  /* [11]  */
        uint32_t error_report_12 : 1;  /* [12]  */
        uint32_t error_report_13 : 1;  /* [13]  */
        uint32_t error_report_14 : 1;  /* [14]  */
        uint32_t error_report_15 : 1;  /* [15]  */
        uint32_t errescentry : 1;      /* [16]  */
        uint32_t errsyncesc : 1;       /* [17]  */
        uint32_t errcontrol : 1;       /* [18]  */
        uint32_t errcontentionlp0 : 1; /* [19]  */
        uint32_t errcontentionlp1 : 1; /* [20]  */
        uint32_t reserved_0 : 11;      /* [31..21]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_int0_status;

/* define the union reg_int1_status */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t eotp_rx_err : 1;            /* [0]  */
        uint32_t rxpkt_size_err : 1;         /* [1]  */
        uint32_t rxcrc_err : 1;              /* [2]  */
        uint32_t rxecc_single_err : 1;       /* [3]  */
        uint32_t rxecc_multi_err : 1;        /* [4]  */
        uint32_t pld_wr_err : 1;             /* [5]  */
        uint32_t pld_rd_err : 1;             /* [6]  */
        uint32_t cmd_wr_err : 1;             /* [7]  */
        uint32_t cmd_pld_wr_err : 1;         /* [8]  */
        uint32_t cmd_pld_rd_err : 1;         /* [9]  */
        uint32_t rxpld_rd_err : 1;           /* [10]  */
        uint32_t rxpld_wr_err : 1;           /* [11]  */
        uint32_t to_lp_rx : 1;               /* [12]  */
        uint32_t to_hs_tx : 1;               /* [13]  */
        uint32_t phy_pll_lock_err : 1;       /* [14]  */
        uint32_t cmd_tran_end : 1;           /* [15]  */
        uint32_t line_length_change : 1;     /* [16]  */
        uint32_t hss_abnormal : 1;           /* [17]  */
        uint32_t fifo_nempty_when_vsync : 1; /* [18]  */
        uint32_t reserved_0 : 1;             /* [19]  */
        uint32_t vss : 1;                    /* [20]  */
        uint32_t reserved_1 : 3;             /* [23..21]  */
        uint32_t rxtrigger0 : 1;             /* [24]  */
        uint32_t rxtrigger1 : 1;             /* [25]  */
        uint32_t rxtrigger2 : 1;             /* [26]  */
        uint32_t rxtrigger3 : 1;             /* [27]  */
        uint32_t reserved_2 : 4;             /* [31..28]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_int1_status;

/* define the union reg_int0_mask */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t mask_error_report_0 : 1;   /* [0]  */
        uint32_t mask_error_report_1 : 1;   /* [1]  */
        uint32_t mask_error_report_2 : 1;   /* [2]  */
        uint32_t mask_error_report_3 : 1;   /* [3]  */
        uint32_t mask_error_report_4 : 1;   /* [4]  */
        uint32_t mask_error_report_5 : 1;   /* [5]  */
        uint32_t mask_error_report_6 : 1;   /* [6]  */
        uint32_t mask_error_report_7 : 1;   /* [7]  */
        uint32_t mask_error_report_8 : 1;   /* [8]  */
        uint32_t mask_error_report_9 : 1;   /* [9]  */
        uint32_t mask_error_report_10 : 1;  /* [10]  */
        uint32_t mask_error_report_11 : 1;  /* [11]  */
        uint32_t mask_error_report_12 : 1;  /* [12]  */
        uint32_t mask_error_report_13 : 1;  /* [13]  */
        uint32_t mask_error_report_14 : 1;  /* [14]  */
        uint32_t mask_error_report_15 : 1;  /* [15]  */
        uint32_t mask_errescentry : 1;      /* [16]  */
        uint32_t mask_errsyncesc : 1;       /* [17]  */
        uint32_t mask_errcontrol : 1;       /* [18]  */
        uint32_t mask_errcontentionlp0 : 1; /* [19]  */
        uint32_t mask_errcontentionlp1 : 1; /* [20]  */
        uint32_t reserved_0 : 11;           /* [31..21]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_int0_mask;

/* define the union reg_int1_mask */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t mask_eotp_rx_err : 1;            /* [0]  */
        uint32_t mask_rxpkt_size_err : 1;         /* [1]  */
        uint32_t mask_rxcrc_err : 1;              /* [2]  */
        uint32_t mask_rxecc_single_err : 1;       /* [3]  */
        uint32_t mask_rxecc_multi_err : 1;        /* [4]  */
        uint32_t mask_pld_wr_err : 1;             /* [5]  */
        uint32_t mask_pld_rd_err : 1;             /* [6]  */
        uint32_t mask_cmd_wr_err : 1;             /* [7]  */
        uint32_t mask_cmd_pld_wr_err : 1;         /* [8]  */
        uint32_t mask_cmd_pld_rd_err : 1;         /* [9]  */
        uint32_t mask_rxpld_rd_err : 1;           /* [10]  */
        uint32_t mask_rxpld_wr_err : 1;           /* [11]  */
        uint32_t mask_to_lp_rx : 1;               /* [12]  */
        uint32_t mask_to_hs_tx : 1;               /* [13]  */
        uint32_t mask_phy_pll_lock_err : 1;       /* [14]  */
        uint32_t mask_cmd_tran_end : 1;           /* [15]  */
        uint32_t mask_line_length_change : 1;     /* [16]  */
        uint32_t mask_hss_abnormal : 1;           /* [17]  */
        uint32_t mask_fifo_nempty_when_vsync : 1; /* [18]  */
        uint32_t reserved_0 : 1;                  /* [19]  */
        uint32_t mask_vss : 1;                    /* [20]  */
        uint32_t reserved_1 : 3;                  /* [23..21]  */
        uint32_t mask_rxtrigger0 : 1;             /* [24]  */
        uint32_t mask_rxtrigger1 : 1;             /* [25]  */
        uint32_t mask_rxtrigger2 : 1;             /* [26]  */
        uint32_t mask_rxtrigger3 : 1;             /* [27]  */
        uint32_t reserved_2 : 4;                  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_int1_mask;

/* define the union reg_phy_cal */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t txskewcalhs : 1; /* [0]  */
        uint32_t reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_cal;

/* define the union reg_auto_ulps_mode */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t auto_ulps : 1;     /* [0]  */
        uint32_t reserved_0 : 3;    /* [3..1]  */
        uint32_t ulps_exit_mux : 2; /* [5..4]  */
        uint32_t reserved_1 : 10;   /* [15..6]  */
        uint32_t pll_off_ulps : 1;  /* [16]  */
        uint32_t reserved_2 : 15;   /* [31..17]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_auto_ulps_mode;

/* define the union reg_auto_ulps_wakeup_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t twakeup_clk_div : 16; /* [15..0]  */
        uint32_t twakeup_cnt : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_auto_ulps_wakeup_time;

/* define the union reg_auto_ulps_exit_pulse */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t ulps_exit_pulse : 1;     /* [0]  */
        uint32_t ulps_exit_pulse_clr : 1; /* [1]  */
        uint32_t ulps_entry_cnt_clr : 1;  /* [2]  */
        uint32_t ulps_intr_clr : 1;       /* [3]  */
        uint32_t reserved : 28;           /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_auto_ulps_exit_pulse;

/* define the union reg_dsc_config */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t compression_mode : 1; /* [0]  */
        uint32_t reserved_0 : 7;       /* [7..1]  */
        uint32_t algorithm_mode : 2;   /* [9..8]  */
        uint32_t reserved_1 : 6;       /* [15..10]  */
        uint32_t pps : 2;              /* [17..16]  */
        uint32_t reserved_2 : 6;       /* [23..18]  */
        uint32_t video_lpcmd : 1;      /* [24]  */
        uint32_t reserved_3 : 7;       /* [31..25]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dsc_config;

/* define the union reg_read_cmd_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t read_cmd_time : 15; /* [14..0]  */
        uint32_t reserved_0 : 17;    /* [31..15]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_read_cmd_time;

/* define the union reg_auto_ulps_min_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t auto_ulps_min_time : 12; /* [11..0]  */
        uint32_t reserved_0 : 20;         /* [31..12]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_auto_ulps_min_time;

/* define the union reg_phy_mode */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t phy_mode : 1;    /* [0]  */
        uint32_t reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_mode;

/* define the union reg_vid_shadow_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vid_shadow_en : 1;  /* [0]  */
        uint32_t reserved_0 : 7;     /* [7..1]  */
        uint32_t vid_shadow_req : 1; /* [8]  */
        uint32_t reserved_1 : 23;    /* [31..9]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_vid_shadow_ctrl;

/* define the union reg_dsi_dbg0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vid_delay : 17; /* [16..0]  */
        uint32_t phyfsm_st : 3;  /* [19..17]  */
        uint32_t dbips_st : 4;   /* [23..20]  */
        uint32_t vidregion : 3;  /* [26..24]  */
        uint32_t dpips_st : 5;   /* [31..27]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dsi_dbg0;

/* define the union reg_phy_pll_start_time */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t phy_pll_start_time_qst : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phy_pll_start_time;

/* define the union reg_dbg_crc_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dbg_crc_init : 1;     /* [0]  */
        uint32_t dbg_crc_lane_sel : 2; /* [2..1]  */
        uint32_t dbg_crc_en : 1;       /* [3]  */
        uint32_t reserved_0 : 28;      /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dbg_crc_ctrl;

/* define the union reg_secu_cfg_en */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t secu_cfg_en : 1; /* [0]  */
        uint32_t reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_secu_cfg_en;

/* define the union reg_polarity_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vsync_plr : 1;   /* [0]  */
        uint32_t hsync_plr : 1;   /* [1]  */
        uint32_t reserved_0 : 1;  /* [2]  */
        uint32_t data_en_plr : 1; /* [3]  */
        uint32_t reserved_1 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_polarity_ctrl;

/* define the union reg_dual_pixelmode */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t reserved_0 : 5;     /* [4..0]  */
        uint32_t dual_mode_en : 1;   /* [5]  */
        uint32_t reserved_1 : 7;     /* [12..6]  */
        uint32_t dual_mode_ctrl : 1; /* [13]  */
        uint32_t reserved_2 : 18;    /* [31..14]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dual_pixelmode;

/* define the union reg_hrz_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dsi_hsize : 13;  /* [12..0]  */
        uint32_t reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hrz_ctrl;

/* define the union reg_vrt_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dsi_vsize : 13;  /* [12..0]  */
        uint32_t reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_vrt_ctrl;

/* define the union reg_int_pro_msk */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t int_pro_msk : 17; /* [16..0]  */
        uint32_t reserved_0 : 15;  /* [31..17]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_int_pro_msk;

/* define the union reg_vstate */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vstate : 11;     /* [10..0]  */
        uint32_t reserved_0 : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_vstate;

/* define the union reg_hstate */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hstate : 6;      /* [5..0]  */
        uint32_t reserved_0 : 26; /* [31..6]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hstate;

/* define the union reg_dphytx_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dphytx_ctrl : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dphytx_ctrl;

/* define the union reg_phytx_trstop_flag */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t phytx_trstop_flag : 1; /* [0]  */
        uint32_t reserved_0 : 31;       /* [31..1]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_phytx_trstop_flag;

/* define the union reg_mipi_dsi_mem_control */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t emaa : 3;        /* [2..0]  */
        uint32_t emasa : 1;       /* [3]  */
        uint32_t reserved_0 : 4;  /* [7..4]  */
        uint32_t emab       : 3;  /* [10..8]  */
        uint32_t ret1n      : 1;  /* [11]  */
        uint32_t colldisn   : 1;  /* [12]  */
        uint32_t reserved_1 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_mipi_dsi_mem_control;

/* define the union reg_display_control */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dpishutdn : 1;    /* [0]  */
        uint32_t dpicolorm : 1;    /* [1]  */
        uint32_t vsync_f_sub : 1;  /* [2]  */
        uint32_t vsync_b_sub : 1;  /* [3]  */
        uint32_t v1_sel : 3;       /* [6..4]  */
        uint32_t reserved_0 : 1;   /* [7]  */
        uint32_t u1_sel : 3;       /* [10..8]  */
        uint32_t reserved_1 : 1;   /* [11]  */
        uint32_t y1_sel : 3;       /* [14..12]  */
        uint32_t reserved_2 : 1;   /* [15]  */
        uint32_t v2_sel : 3;       /* [18..16]  */
        uint32_t reserved_3 : 1;   /* [19]  */
        uint32_t u2_sel : 3;       /* [22..20]  */
        uint32_t reserved_4 : 1;   /* [23]  */
        uint32_t y2_sel : 3;       /* [26..24]  */
        uint32_t reserved_5 : 1;   /* [27]  */
        uint32_t dataen_f_sub : 1; /* [28]  */
        uint32_t dataen_b_sub : 1; /* [29]  */
        uint32_t hsync_f_sub : 1;  /* [30]  */
        uint32_t hsync_b_sub : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_display_control;

/* define the union reg_operation_mode */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t reserved_0 : 1;          /* [0]  */
        uint32_t mem_ck_en : 1;           /* [1]  */
        uint32_t reserved_1 : 3;          /* [4..2]  */
        uint32_t init_skew_en : 1;        /* [5]  */
        uint32_t period_skew_en : 1;      /* [6]  */
        uint32_t reserved_2 : 1;          /* [7]  */
        uint32_t colorbar_en : 1;         /* [8]  */
        uint32_t colorbar_orien : 1;      /* [9]  */
        uint32_t colorbar_test_en : 1;    /* [10]  */
        uint32_t reserved_3 : 1;          /* [11]  */
        uint32_t colorbar_mode : 3;       /* [14..12]  */
        uint32_t cmd_colorbar_en : 1;     /* [15]  */
        uint32_t read_empty_vsync_en : 1; /* [16]  */
        uint32_t reserved_4 : 3;          /* [19..17]  */
        uint32_t hss_abnormal_rst : 1;    /* [20]  */
        uint32_t rxesc_buf_en : 1;        /* [21]  */
        uint32_t reserved_5 : 2;          /* [23..22]  */
        uint32_t turndisable : 1;         /* [24]  */
        uint32_t forcerxmode : 1;         /* [25]  */
        uint32_t forcetxstopmode : 1;     /* [26]  */
        uint32_t video_te_en : 1;         /* [27]  */
        uint32_t reserved_6 : 3;          /* [30..28]  */
        uint32_t input_en : 1;            /* [31]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_operation_mode;

/* define the union reg_hfp_hact_in */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hact_in : 16; /* [15..0]  */
        uint32_t hfp_in : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hfp_hact_in;

/* define the union reg_hbp_hsa_in */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hsa_in : 16; /* [15..0]  */
        uint32_t hbp_in : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hbp_hsa_in;

/* define the union reg_vert_det */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vact_det : 16; /* [15..0]  */
        uint32_t vall_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_vert_det;

/* define the union reg_hori0_det */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hact_det : 16;  /* [15..0]  */
        uint32_t hline_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hori0_det;

/* define the union reg_hori1_det */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hsa_det : 16; /* [15..0]  */
        uint32_t hbp_det : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hori1_det;

/* define the union reg_vsa_det */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vsa_det : 16;    /* [15..0]  */
        uint32_t reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_vsa_det;

/* define the union reg_v_h_send */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t h_send : 16; /* [15..0]  */
        uint32_t v_send : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_v_h_send;

/* define the union reg_lanereqhs_width */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lanerequesths_width : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;          /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lanereqhs_width;

/* define the union reg_hs_data */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t l0hsdata : 8; /* [7..0]  */
        uint32_t l1hsdata : 8; /* [15..8]  */
        uint32_t l2hsdata : 8; /* [23..16]  */
        uint32_t l3hsdata : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_hs_data;

/* define the union reg_datatype0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dt_pixel : 6;   /* [5..0]  */
        uint32_t reserved_0 : 2; /* [7..6]  */
        uint32_t dt_hss : 6;     /* [13..8]  */
        uint32_t reserved_1 : 2; /* [15..14]  */
        uint32_t dt_vse : 6;     /* [21..16]  */
        uint32_t reserved_2 : 2; /* [23..22]  */
        uint32_t dt_vss : 6;     /* [29..24]  */
        uint32_t reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_datatype0;

/* define the union reg_datatype1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t dt_null : 6;    /* [5..0]  */
        uint32_t reserved_0 : 2; /* [7..6]  */
        uint32_t dt_blank : 6;   /* [13..8]  */
        uint32_t reserved_1 : 2; /* [15..14]  */
        uint32_t dt_etop : 6;    /* [21..16]  */
        uint32_t reserved_2 : 2; /* [23..22]  */
        uint32_t dt_hse : 6;     /* [29..24]  */
        uint32_t reserved_3 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_datatype1;

/* define the union reg_csi_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t csi_en : 1;           /* [0]  */
        uint32_t reserved_0 : 3;       /* [3..1]  */
        uint32_t csi_vse_position : 1; /* [4]  */
        uint32_t reserved_1 : 3;       /* [7..5]  */
        uint32_t frame_num_inc : 1;    /* [8]  */
        uint32_t reserved_2 : 3;       /* [11..9]  */
        uint32_t dt_config : 1;        /* [12]  */
        uint32_t reserved_3 : 3;       /* [15..13]  */
        uint32_t csi_hss_en : 1;       /* [16]  */
        uint32_t reserved_4 : 15;      /* [31..17]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_csi_ctrl;

/* define the union reg_lane_id */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t lane0_id : 2;    /* [1..0]  */
        uint32_t reserved_0 : 2;  /* [3..2]  */
        uint32_t lane1_id : 2;    /* [5..4]  */
        uint32_t reserved_1 : 2;  /* [7..6]  */
        uint32_t lane2_id : 2;    /* [9..8]  */
        uint32_t reserved_2 : 2;  /* [11..10]  */
        uint32_t lane3_id : 2;    /* [13..12]  */
        uint32_t reserved_3 : 18; /* [31..14]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_lane_id;

/* define the union reg_read_memory_delay_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t delay_regin : 17;   /* [16..0]  */
        uint32_t reserved_0 : 3;     /* [19..17]  */
        uint32_t delay_abnormal : 1; /* [20]  */
        uint32_t delay_from_reg : 1; /* [21]  */
        uint32_t reserved_1 : 10;    /* [31..22]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_read_memory_delay_ctrl;

/* define the union reg_ppi_v_send0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vsa_length : 16;     /* [15..0]  */
        uint32_t vsa_vbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_v_send0;

/* define the union reg_ppi_v_send1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t vactive_length : 16; /* [15..0]  */
        uint32_t vtotal_length : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_v_send1;

/* define the union reg_ppi_h_send0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hsa_length : 16;     /* [15..0]  */
        uint32_t hsa_hbp_length : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_h_send0;

/* define the union reg_ppi_h_send1 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t hactive_length : 16; /* [15..0]  */
        uint32_t htotal_length : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_h_send1;

/* define the union reg_ppi_h_send2 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t pix_pkt_length : 16;   /* [15..0]  */
        uint32_t pix_pkt_distance : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_h_send2;

/* define the union reg_ppi_h_send3 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t line_length_send_diff : 16; /* [15..0]  */
        uint32_t reserved_0 : 16;            /* [31..16]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_ppi_h_send3;

/* define the union reg_sdlp_dbg */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t mipi_ready              : 1;       /* [0] */
        uint32_t mipi_ok                 : 1;       /* [1] */
        uint32_t ulps_exit_empty_err     : 1;       /* [2] */
        uint32_t ulps_exit_exceed_err    : 1;       /* [3] */
        uint32_t reserved                : 4;       /* [7..4] */
        uint32_t ulps_exit_cnt           : 8;       /* [15..8] */
        uint32_t ulps_entry_cnt          : 16;      /* [31..16] */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_sdlp_dbg;

/* define the union reg_dft_control */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t code_length : 17;    /* [16..0]  */
        uint32_t reserved_0 : 3;      /* [19..17]  */
        uint32_t pattern_type : 2;    /* [21..20]  */
        uint32_t dft_rst_apb  : 1;    /* [22]  */
        uint32_t reserved_1 : 1;      /* [23]  */
        uint32_t hs_lp_mode : 2;      /* [25..24]  */
        uint32_t txskewcalhs_dft : 1; /* [26]  */
        uint32_t bist_mode : 1;       /* [27]  */
        uint32_t compare_done : 1;    /* [28]  */
        uint32_t compare : 1;         /* [29]  */
        uint32_t compare_result : 1;  /* [30]  */
        uint32_t reserved_2 : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dft_control;

/* define the union reg_dft_prbs_init */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t prbs_init : 7;   /* [6..0]  */
        uint32_t reserved_0 : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_dft_prbs_init;

/* define the union reg_mipitx_sdlp_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        uint32_t sdlp_rstn               : 1;       /* [0] */
        uint32_t sdlp_enter_ovr          : 1;       /* [1] */
        uint32_t sdlp_enter_ovr_en       : 1;       /* [2] */
        uint32_t sdlp_pll_lock_ovr       : 1;       /* [3] */
        uint32_t sdlp_pll_lock_ovr_en    : 1;       /* [4] */
        uint32_t sdlp_ini_done_ove       : 1;       /* [5] */
        uint32_t sdlp_ini_done_ove_en    : 1;       /* [6] */
        uint32_t sdlp_ovrwt_ovr          : 1;       /* [7] */
        uint32_t sdlp_ovrwt_ovr_en       : 1;       /* [8] */
        uint32_t sdlp_prstn_ovr          : 1;       /* [9] */
        uint32_t sdlp_prstn_ovr_en       : 1;       /* [10] */
        uint32_t sdlp_crstn_ovr          : 1;       /* [11] */
        uint32_t sdlp_crstn_ovr_en       : 1;       /* [12] */
        uint32_t sdlp_ref_chen_ovr       : 1;       /* [13] */
        uint32_t sdlp_ref_chen_ovr_en    : 1;       /* [14] */
        uint32_t sdlp_cfg_chen_ovr       : 1;       /* [15] */
        uint32_t sdlp_cfg_chen_ovr_en    : 1;       /* [16] */
        uint32_t sdlp_dpi_chen_ovr       : 1;       /* [17] */
        uint32_t sdlp_dpi_chen_ovr_en    : 1;       /* [18] */
        uint32_t sdlp_lb_chen_ovr        : 1;       /* [19] */
        uint32_t sdlp_lb_chen_ovr_en     : 1;       /* [20] */
        uint32_t reserved                : 11;      /* [31:21] */
    } bits;

    /* define an unsigned member */
    uint32_t u32;
} reg_mipitx_sdlp_cfg0;

// ==============================================================================
/* define the global struct */
typedef struct {
    volatile reg_ctrl_reset ctrl_reset;                         /* 0x0 */
    volatile uint32_t reserved_0;                               /* 0x4 */
    volatile reg_crg_cfg crg_cfg;                               /* 0x8 */
    volatile reg_video_vc video_vc;                             /* 0xc */
    volatile reg_data_type data_type;                           /* 0x10 */
    volatile uint32_t reserved_1;                               /* 0x14 */
    volatile reg_lp_cmd_byte lp_cmd_byte;                       /* 0x18 */
    volatile uint32_t reserved_2[4];                            /* 0x1c~0x28 */
    volatile reg_pck_en pck_en;                                 /* 0x2c */
    volatile reg_gen_vc gen_vc;                                 /* 0x30 */
    volatile reg_mode_cfg mode_cfg;                             /* 0x34 */
    volatile reg_video_lp_en video_lp_en;                       /* 0x38 */
    volatile reg_videom_pkt_size videom_pkt_size;               /* 0x3c */
    volatile reg_videom_num_chunks videom_num_chunks;           /* 0x40 */
    volatile reg_videom_null_size videom_null_size;             /* 0x44 */
    volatile reg_videom_hsa_size videom_hsa_size;               /* 0x48 */
    volatile reg_videom_hbp_size videom_hbp_size;               /* 0x4c */
    volatile reg_videom_hline_size videom_hline_size;           /* 0x50 */
    volatile reg_videom_vsa_lines videom_vsa_lines;             /* 0x54 */
    volatile reg_videom_vbp_lines videom_vbp_lines;             /* 0x58 */
    volatile reg_videom_vfp_lines videom_vfp_lines;             /* 0x5c */
    volatile reg_videom_vactive_lines videom_vactive_lines;     /* 0x60 */
    volatile reg_command_pkt_size command_pkt_size;             /* 0x64 */
    volatile reg_command_tran_mode command_tran_mode;           /* 0x68 */
    volatile reg_command_header command_header;                 /* 0x6c */
    volatile reg_command_payload command_payload;               /* 0x70 */
    volatile reg_command_status command_status;                 /* 0x74 */
    volatile reg_hs_lp_to_set hs_lp_to_set;                     /* 0x78 */
    volatile reg_hsrd_to_set hsrd_to_set;                       /* 0x7c */
    volatile reg_lprd_to_set lprd_to_set;                       /* 0x80 */
    volatile reg_hswr_to_set hswr_to_set;                       /* 0x84 */
    volatile reg_lpwr_to_set lpwr_to_set;                       /* 0x88 */
    volatile reg_bta_to_set bta_to_set;                         /* 0x8c */
    volatile uint32_t reserved_3;                               /* 0x90 */
    volatile reg_clklane_ctrl clklane_ctrl;                     /* 0x94 */
    volatile reg_clklane_time clklane_time;                     /* 0x98 */
    volatile reg_datalane_time datalane_time;                   /* 0x9c */
    volatile reg_phy_ctrl phy_ctrl;                             /* 0xa0 */
    volatile reg_lane_num lane_num;                             /* 0xa4 */
    volatile reg_ulps_ctrl ulps_ctrl;                           /* 0xa8 */
    volatile reg_tx_triggers tx_triggers;                       /* 0xac */
    volatile reg_ppi_status ppi_status;                         /* 0xb0 */
    volatile reg_phy_reg_cfg0 phy_reg_cfg0;                     /* 0xb4 */
    volatile reg_phy_reg_cfg1 phy_reg_cfg1;                     /* 0xb8 */
    volatile reg_int0_status int0_status;                       /* 0xbc */
    volatile reg_int1_status int1_status;                       /* 0xc0 */
    volatile reg_int0_mask int0_mask;                           /* 0xc4 */
    volatile reg_int1_mask int1_mask;                           /* 0xc8 */
    volatile uint32_t reserved_4[3];                            /* 0xcc~0xd4 */
    volatile uint32_t int_force0;                               /* 0xd8 */
    volatile uint32_t int_force1;                               /* 0xdc */
    volatile reg_auto_ulps_mode auto_ulps_mode;                 /* 0xe0 */
    volatile uint32_t auto_ulps_entry_delay;                    /* 0xe4 */
    volatile reg_auto_ulps_wakeup_time auto_ulps_wakeup_time;   /* 0xe8 */
    volatile reg_auto_ulps_exit_pulse auto_ulps_exit_pulse;     /* 0xec */
    volatile reg_dsc_config dsc_config;                         /* 0xf0 */
    volatile reg_read_cmd_time read_cmd_time;                   /* 0xf4 */
    volatile reg_auto_ulps_min_time auto_ulps_min_time;         /* 0xf8 */
    volatile reg_phy_mode phy_mode;                             /* 0xfc */
    volatile reg_vid_shadow_ctrl vid_shadow_ctrl;               /* 0x100 */
    volatile uint32_t reserved_6[38];                           /* 0x104~0x198 */
    volatile reg_dsi_dbg0 dsi_dbg0;                             /* 0x19c */
    volatile reg_phy_pll_start_time phy_pll_start_time;         /* 0x1a0 */
    volatile reg_dbg_crc_ctrl dbg_crc_ctrl;                     /* 0x1a4 */
    volatile uint32_t dbg_crc_val;                              /* 0x1a8 */
    volatile reg_secu_cfg_en secu_cfg_en;                       /* 0x1ac */
    volatile reg_polarity_ctrl polarity_ctrl;                   /* 0x1b0 */
    volatile uint32_t reserved_7;                               /* 0x1b4 */
    volatile reg_dual_pixelmode dual_pixelmode;                 /* 0x1b8 */
    volatile uint32_t reserved_8[7];                            /* 0x1bc~0x1d4 */
    volatile uint32_t frm_valid_dbg;                            /* 0x1d8 */
    volatile uint32_t reserved_9[4];                            /* 0x1dc~0x1e8 */
    volatile reg_hrz_ctrl hrz_ctrl;                             /* 0x1ec */
    volatile reg_vrt_ctrl vrt_ctrl;                             /* 0x1f0 */
    volatile uint32_t reserved_10;                              /* 0x1f4 */
    volatile reg_int_pro_msk int_pro_msk;                       /* 0x1f8 */
    volatile reg_vstate vstate;                                 /* 0x1fc */
    volatile reg_hstate hstate;                                 /* 0x200 */
    volatile uint32_t reserved_11[8];                           /* 0x204~0x220 */
    volatile uint32_t phytx_stopsnt;                            /* 0x224 */
    volatile reg_dphytx_ctrl dphytx_ctrl;                       /* 0x228 */
    volatile reg_phytx_trstop_flag phytx_trstop_flag;           /* 0x22c */
    volatile uint32_t phytx_status;                             /* 0x230 */
    volatile uint32_t reserved_12[6];                           /* 0x234~0x248 */
    volatile uint32_t gint_msk;                                 /* 0x24c */
    volatile uint32_t reserved_13[44];                          /* 0x250~0x2fc */
    volatile reg_mipi_dsi_mem_control mipi_dsi_mem_control;     /* 0x300 */
    volatile reg_display_control display_control;               /* 0x304 */
    volatile reg_operation_mode operation_mode;                 /* 0x308 */
    volatile reg_hfp_hact_in hfp_hact_in;                       /* 0x30c */
    volatile reg_hbp_hsa_in hbp_hsa_in;                         /* 0x310 */
    volatile reg_vert_det vert_det;                             /* 0x314 */
    volatile reg_hori0_det hori0_det;                           /* 0x318 */
    volatile reg_hori1_det hori1_det;                           /* 0x31c */
    volatile reg_vsa_det vsa_det;                               /* 0x320 */
    volatile reg_v_h_send v_h_send;                             /* 0x324 */
    volatile reg_lanereqhs_width lanereqhs_width;               /* 0x328 */
    volatile reg_hs_data hs_data;                               /* 0x32c */
    volatile reg_datatype0 datatype0;                           /* 0x330 */
    volatile reg_datatype1 datatype1;                           /* 0x334 */
    volatile reg_csi_ctrl csi_ctrl;                             /* 0x338 */
    volatile uint32_t skew_begin;                               /* 0x33c */
    volatile uint32_t skew_end;                                 /* 0x340 */
    volatile uint32_t reserved_14[3];                           /* 0x344~0x34c */
    volatile reg_lane_id lane_id;                               /* 0x350 */
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl; /* 0x354 */
    volatile reg_ppi_v_send0 ppi_v_send0;                       /* 0x358 */
    volatile reg_ppi_v_send1 ppi_v_send1;                       /* 0x35c */
    volatile reg_ppi_h_send0 ppi_h_send0;                       /* 0x360 */
    volatile reg_ppi_h_send1 ppi_h_send1;                       /* 0x364 */
    volatile reg_ppi_h_send2 ppi_h_send2;                       /* 0x368 */
    volatile reg_ppi_h_send3 ppi_h_send3;                       /* 0x36c */
    volatile reg_sdlp_dbg sdlp_dbg;                             /* 0x370 */
    volatile uint32_t reserved_15[11];                          /* 0x374~0x39c */
    volatile uint32_t te_length;                                /* 0x3a0 */
    volatile uint32_t reserved_16[15];                          /* 0x3a4~0x3dc */
    volatile reg_dft_control dft_control;                       /* 0x3e0 */
    volatile uint32_t dft_skew_period;                          /* 0x3e4 */
    volatile reg_dft_prbs_init dft_prbs_init;                   /* 0x3e8 */
    volatile uint32_t reserved_17[2];                           /* 0x3ec~0x3f0 */
    volatile reg_mipitx_sdlp_cfg0 mipitx_sdlp_cfg0;             /* 0x3f4 */
} mipi_tx_regs_type_t;

#endif /* __MIPI_TX_REG_H__ */
