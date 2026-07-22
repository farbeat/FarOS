/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa v2 regs Definitions \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_RSA_V2_REGS_DEF_H
#define HAL_RSA_V2_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa_v2_regs_def hal rsa v2 regs definition
 * @ingroup  drivers_hal_pke
 * @{
 */

/**
 * @brief  This union represents the bit fields in the rsa v2 ahbm timeout waterlinte config register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_cfg_ahbm_timeout_waterline {
    struct {
        uint32_t    ahbm_timeout_waterline : 16;       /*!< reserved */
        uint32_t    reserved_0            : 16;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_cfg_ahbm_timeout_waterline_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 otskcfg start register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_otskcfg_start {
    struct {
        uint32_t    otask_start           : 1;         /*!< Controls the start of external tasks. */
        uint32_t    reserved_0            : 31;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_otskcfg_start_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 otskcfg stop register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_otskcfg_stop {
    struct {
        uint32_t    otask_force_end       : 1;         /*!< The external task forcibly stops the control. */
        uint32_t    reserved_0            : 31;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_otskcfg_stop_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 otskcfg attribute register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_otskcfg_attribute {
    struct {
        uint32_t    otask_len             : 8;         /*!< External task length, measured by processing base length j.
                                                            It also represents the length of the modulus n, the base m,
                                                            the power e, and the result result result. */
        uint32_t    plaintext_len_sel     : 1;         /*!< Plaintext Length Selection */
        uint32_t    reserved_0            : 23;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_otskcfg_attribute_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 interrupt mask register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_intmsk {
    struct {
        uint32_t    reserved_0            : 4;         /*!< reserved */
        uint32_t    nc_cipher_done_msk    : 1;         /*!< Modular exponentiation calculation completion interrupt
                                                            mask using (n, e) or (n, d) */
        uint32_t    reserved_1            : 3;         /*!< reserved */
        uint32_t    nc_fail_msk           : 1;         /*!< NC task failure masking */
        uint32_t    reserved_2            : 5;         /*!< reserved */
        uint32_t    ahbm_timeout_msk      : 1;         /*!< AHB master transmission timeout interrupt mask */
        uint32_t    reserved_3            : 17;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_intmsk_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 Raw Interrupt register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_rint {
    struct {
        uint32_t    reserved_0            : 4;         /*!< reserved */
        uint32_t    nc_cipher_done_rint   : 1;         /*!< Modular exponentiation calculation complete raw interrupt
                                                            using (n, e) or (n, d) */
        uint32_t    reserved_1            : 3;         /*!< reserved */
        uint32_t    nc_fail_rint          : 1;         /*!< Raw interrupt of NC task failure */
        uint32_t    reserved_2            : 5;         /*!< reserved */
        uint32_t    ahbm_timeout_rint     : 1;         /*!< AHB master transmission timeout raw interrupt */
        uint32_t    reserved_3            : 17;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_rint_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 Interrupt status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_intsts {
    struct {
        uint32_t    reserved_0            : 4;         /*!< reserved */
        uint32_t    nc_cipher_done_intsts : 1;         /*!< Modular exponentiation calculation completion interrupt
                                                            status using (n, e) or (n, d) */
        uint32_t    reserved_1            : 3;         /*!< reserved */
        uint32_t    nc_fail_intsts        : 1;         /*!< interrupt status of NC task failure */
        uint32_t    reserved_2            : 5;         /*!< reserved */
        uint32_t    ahbm_timeout_intsts   : 1;         /*!< AHB master transmission timeout interrupt status. */
        uint32_t    reserved_3            : 17;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_intsts_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 Interrupt set register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_intset {
    struct {
        uint32_t    reserved_0            : 4;         /*!< reserved */
        uint32_t    nc_cipher_done_intset : 1;         /*!< 0: no operation;
                                                            1: rsav2_rint [4] is set. */
        uint32_t    reserved_1            : 3;         /*!< reserved */
        uint32_t    nc_fail_intset        : 1;         /*!< 0: no operation;
                                                            1: rsav2_rint [8] is set. */
        uint32_t    reserved_2            : 5;         /*!< reserved */
        uint32_t    ahbm_timeout_intset   : 1;         /*!< 0: no operation;
                                                            1: rsav2_rint [14] is set. */
        uint32_t    reserved_3            : 17;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_intset_t;

/**
 * @brief  This union represents the bit fields in the rsa v2 dfx count error otask attribute register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dfxcnt_err_otask_attribute {
    struct {
        uint32_t    dfxcnt_err_otask_attribute : 10;   /*!< Invalid configuration of rsav2_otskcfg_attribute
                                                            [otask_len]. When the count is full, the count remains
                                                            unchanged. */
        uint32_t    reserved_0            : 22;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dfxcnt_err_otask_attribute_t;

/**
 * @brief  This union represents the bit fields in the nc statistics register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dfxcnt_nc_get {
    struct {
        uint32_t    dfxcnt_nc_get         : 20;        /*!< Number of successfully accepted NC tasks. Count full
                                                            winding. */
        uint32_t    reserved_0            : 12;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dfxcnt_nc_get_t;

/**
 * @brief  This union represents the bit fields in the nc succ statistics register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dfxcnt_nc_success {
    struct {
        uint32_t    dfxcnt_nc_sucess      : 20;        /*!< The NC task statistics are successfully collected.
                                                            Count full is winding. */
        uint32_t    reserved_0            : 12;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dfxcnt_nc_success_t;

/**
 * @brief  This union represents the bit fields in the nc fail statistics register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dfxcnt_nc_fail {
    struct {
        uint32_t    dfxcnt_nc_fail        : 10;        /*!< Number of NC task failures. Full count hold. */
        uint32_t    reserved_0            : 22;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dfxcnt_nc_fail_t;

/**
 * @brief  This union represents the bit fields in the nc fail reason register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dfx_nc_fail_reason {
    struct {
        uint32_t    dfx_nc_fail_reason    : 8;         /*!< Reason for the latest NC task failure: */
        uint32_t    reserved_0            : 24;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dfx_nc_fail_reason_t;

/**
 * @brief  This union represents the bit fields in the current busy status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_curr_busy {
    struct {
        uint32_t    rsa_otask_busy        : 1;         /*!< Bus is in read/write operation indicator.  */
        uint32_t    rsa_itask_busy        : 1;         /*!< Indicates the running status of an internal task. */
        uint32_t    rsa_busy              : 1;         /*!< Operation status indicator. */
        uint32_t    rsa_wr_busy           : 1;         /*!< Bus is in read/write operation indicator. */
        uint32_t    reserved_0            : 28;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_curr_busy_t;

/**
 * @brief  This union represents the bit fields in the codec fsm status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_normal_codec_fsm_st {
    struct {
        uint32_t    normal_codec_main_fsm_st : 6;      /*!< Modular power master state machine status query */
        uint32_t    reserved_0            : 26;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_normal_codec_fsm_st_t;

/**
 * @brief  This union represents the bit fields in the mode fsm status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_mod_fsm_st {
    struct {
        uint32_t    mod_fsm_st            : 7;         /*!< Querying the status of the modulo operation state
                                                            machine. */
        uint32_t    reserved_0            : 25;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_mod_fsm_st_t;

/**
 * @brief  This union represents the bit fields in the modex ctrl status left register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_modex_ctrl_st_left {
    struct {
        uint32_t    left_modex_ctrl_fsm_st : 8;        /*!< Querying the status of the left chain state machine in
                                                            modular exponentiation operation. */
        uint32_t    left_modex_ctrl_exp_cnt_st : 15;   /*!< Querying the number of left chain cycles of modular
                                                            exponentiation operation. */
        uint32_t    reserved_0            : 9;         /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_modex_ctrl_st_left_t;

/**
 * @brief  This union represents the bit fields in the modex ctrl status right register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_modex_ctrl_st_right {
    struct {
        uint32_t    right_modex_ctrl_fsm_st : 8;       /*!< Querying the status of the right chain state machine in
                                                            modular exponentiation operation */
        uint32_t    right_modex_ctrl_exp_cnt_st : 15;  /*!< Queries the number of cycles of the right chain of the
                                                            modular exponentiation operation. */
        uint32_t    reserved_0            : 9;         /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_modex_ctrl_st_right_t;

/**
 * @brief  This union represents the bit fields in the data read status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dat_rd_st {
    struct {
        uint32_t    dat_rd_st             : 7;         /*!< Querying the Data Read Status */
        uint32_t    reserved_0            : 25;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dat_rd_st_t;

/**
 * @brief  This union represents the bit fields in the data write status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_dat_wr_st {
    struct {
        uint32_t    dat_wr_st             : 10;        /*!< Querying the Data Write Status */
        uint32_t    reserved_0            : 22;        /*!< reserved. */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_dat_wr_st_t;

/**
 * @brief  This union represents the bit fields in the sram initialization status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_ram_ini_st {
    struct {
        uint32_t    ram_ini_st            : 1;         /*!< Initialization status query.
                                                            0x0: The rsav2 sram initialization is not complete.
                                                            0x1: The rsav2 sram initialization is complete. */
        uint32_t    reserved_0            : 31;        /*!< reserved. */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_ram_ini_st_t;

/**
 * @brief  This union represents the bit fields in the ahbm rw cmd view register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union rsav2_ahbm_rw_cmd_view {
    struct {
        uint32_t    ahbm_hready           : 1;         /*!< ahbm hready signal */
        uint32_t    ahbm_wr_incr_256_cnt  : 1;         /*!< 256-bit write burst count. */
        uint32_t    ahbm_wr_incr_512_cnt  : 5;         /*!< 512-bit write burst count. */
        uint32_t    ahbm_wr_ack_cnt       : 4;         /*!< Number of write data responses. */
        uint32_t    ahbm_rd_incr_256_cnt  : 1;         /*!< 256-bit read burst count. */
        uint32_t    ahbm_rd_incr_512_cnt  : 5;         /*!< 512-bit read burst count. */
        uint32_t    ahbm_rd_ack_cnt       : 4;         /*!< Read data response count. */
        uint32_t    reserved_0            : 11;        /*!< reserved. */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} rsav2_ahbm_rw_cmd_view_t;

/**
 * @brief  Registers associated with trng.
 */
typedef struct rsa_regs {
    volatile uint32_t                             reserved_0[79];                       /*!< reserved. */
    volatile rsav2_cfg_ahbm_timeout_waterline_t   rsav2_cfg_ahbm_timeout_waterline;     /*!< AHB Master Response
                                                                                             Timeout Threshold
                                                                                             Configuration */
    volatile uint32_t                             reserved_1[1968];                     /*!< reserved. */
    volatile rsav2_otskcfg_start_t                rsav2_otskcfg_start;                  /*!< External task control
                                                                                             register1 */
    volatile rsav2_otskcfg_stop_t                 rsav2_otskcfg_stop;                   /*!< External task control
                                                                                             register2 */
    volatile rsav2_otskcfg_attribute_t            rsav2_otskcfg_attribute;              /*!< External task attribute
                                                                                             configuration register */
    volatile uint32_t                             rsav2_otskcfg_plaintext_addr;         /*!< RSA algorithm plaintext
                                                                                             address register */
    volatile uint32_t                             reserved_2;                           /*!< reserved. */
    volatile uint32_t                             rsav2_otskcfg_key_addr;               /*!< RSA algorithm key address
                                                                                             register */
    volatile uint32_t                             reserved_3[5];                        /*!< reserved. */
    volatile uint32_t                             rsav2_otskcfg_result_addr;            /*!< External task calculation
                                                                                             result address register */
    volatile uint32_t                             reserved_4[500];                      /*!< reserved. */
    volatile rsav2_intmsk_t                       rsav2_intmsk;                         /*!< External task operation
                                                                                             interrupt mask register */
    volatile rsav2_rint_t                         rsav2_rint;                           /*!< External task raw interrupt
                                                                                             register */
    volatile rsav2_intsts_t                       rsav2_intsts;                         /*!< External task interrupt
                                                                                             status register */
    volatile rsav2_intset_t                       rsav2_intset;                         /*!< Interrupt Set Register */
    volatile uint32_t                             reserved_5[510];                      /*!< reserved. */
    volatile rsav2_dfxcnt_err_otask_attribute_t   rsav2_dfxcnt_err_otask_attribute;     /*!< Task attribute register */
    volatile uint32_t                             reserved_6[9];                        /*!< reserved. */
    volatile rsav2_dfxcnt_nc_get_t                rsav2_dfxcnt_nc_get;                  /*!< Statistics on Successfully
                                                                                             Accepted NC Tasks */
    volatile rsav2_dfxcnt_nc_success_t            rsav2_dfxcnt_nc_success;              /*!< NC Task Statistics
                                                                                             Completed Successfully */
    volatile rsav2_dfxcnt_nc_fail_t               rsav2_dfxcnt_nc_fail;                 /*!< NC Task Failure
                                                                                             Statistics */
    volatile rsav2_dfx_nc_fail_reason_t           rsav2_dfx_nc_fail_reason;             /*!< Reason for the latest NC
                                                                                             task failure */
    volatile uint32_t                             reserved_7[497];                      /*!< reserved. */
    volatile rsav2_curr_busy_t                    rsav2_curr_busy;                      /*!< RSA operation status
                                                                                             indicator register */
    volatile uint32_t                             reserved_8[32];                       /*!< reserved. */
    volatile rsav2_normal_codec_fsm_st_t          rsav2_normal_codec_fsm_st;            /*!< Modular power status
                                                                                             query */
    volatile rsav2_mod_fsm_st_t                   rsav2_mod_fsm_st;                     /*!< Querying the status of the
                                                                                             modulo operation state
                                                                                             machine */
    volatile uint32_t                             reserved_9;                           /*!< reserved. */
    volatile rsav2_modex_ctrl_st_left_t           rsav2_modex_ctrl_st_left;             /*!< Modular power left chain
                                                                                             status query */
    volatile rsav2_modex_ctrl_st_right_t          rsav2_modex_ctrl_st_right;            /*!< Modular power right chain
                                                                                             status query */
    volatile uint32_t                             reserved_10[6];                       /*!< reserved. */
    volatile rsav2_dat_rd_st_t                    rsav2_dat_rd_st;                      /*!< Querying the Data Read
                                                                                             Status */
    volatile rsav2_dat_wr_st_t                    rsav2_dat_wr_st;                      /*!< Querying the Data Write
                                                                                             Status */
    volatile uint32_t                             reserved_11[19];                      /*!< reserved. */
    volatile rsav2_ram_ini_st_t                   rsav2_ram_ini_st;                     /*!< Querying the SRAM
                                                                                             Initialization Status */
    volatile uint32_t                             reserved_12[192];                     /*!< reserved. */
    volatile rsav2_ahbm_rw_cmd_view_t             rsav2_ahbm_rw_cmd_view;               /*!< AHB Master Read/Write
                                                                                             Command Observation */
} rsa_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
