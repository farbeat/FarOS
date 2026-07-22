/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash sha v2 regs Definition \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_SHA_V2_REGS_DEF_H
#define HAL_SHA_V2_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_sha_v2_regs_def Hash Sha v2 Regs Definition
 * @ingroup  drivers_hal_sha
 * @{
 */

/**
 * @brief  This union represents the bit fields in the sec int en register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_int_en {
    struct {
        uint32_t    reserved_0            : 2;        /*!< reserved */
        uint32_t    sha_int_en            : 1;        /*!< sha interrupt enable */
        uint32_t    reserved_1            : 29;       /*!< reserved */
    } b;
    uint32_t    d32;                                  /*!< Raw register data. */
} sha_int_en_t;

/**
 * @brief  This union represents the bit fields in the sec int clear register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_int_clr {
    struct {
        uint32_t    reserved_0            : 2;         /*!< reserved */
        uint32_t    sha_int_clr           : 1;         /*!< sha interrupt clear */
        uint32_t    reserved_1            : 29;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} sha_int_clr_t;

/**
 * @brief  This union represents the bit fields in the sec int status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_int_sts {
    struct {
        uint32_t    reserved_0            : 2;         /*!< reserved */
        uint32_t    sha_int_sts           : 1;         /*!< sha interrupt clear */
        uint32_t    reserved_1            : 29;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} sha_int_sts_t;

/**
 * @brief  This union represents the bit fields in the sha ctrl register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_ctrl {
    struct {
        uint32_t    sha_en                : 1;         /*!< Global enable of the SHA module */
        uint32_t    sha_start             : 1;         /*!< SHA operation start signal */
        uint32_t    sha_end               : 1;         /*!< SHA operation completion status */
        uint32_t    reserved_0            : 29;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} sha_ctrl_t;

/**
 * @brief  This union represents the bit fields in the sha param cfg register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_cfg {
    struct {
        uint32_t    sha_ivin_sel          : 1;         /*!< Indicates the sha_ivin source.
                                                            The options are as follows:
                                                            0: Use the default configuration;
                                                            1: Configured by software. */
        uint32_t    sha_padding_dis       : 1;         /*!< 0: automatic padding; 1: automatic padding;
                                                            valid only when data_len is 64-byte aligned */
#if HASH_SHA512_SUPPORT == YES
        uint32_t    reserved_0            : 2;         /*!< reserved */
        uint32_t    sha_compute_mode      : 1;         /*!< sha calculation mode */
        uint32_t    reserved_1            : 27;        /*!< reserved */
#else
        uint32_t    reserved_0            : 30;        /*!< reserved */
#endif
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} sha_cfg_t;

/**
 * @brief  This union represents the bit fields in the sha fsm status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union sha_fsm_st {
    struct {
        uint32_t    sha_fsm_st            : 4;         /*!< sha fsm status */
        uint32_t    reserved_0            : 28;        /*!< reserved */
    } b;
    uint32_t    d32;                                   /*!< Raw register data. */
} sha_fsm_st_t;

/**
 * @brief  Registers associated with hash sha regs.
 */
typedef struct sha_regs {
    volatile sha_int_en_t           sha_int_en;                   /*!< Enable sha interrupt */
    volatile sha_int_clr_t          sha_int_clr;                  /*!< Clear sha interrupt */
    volatile sha_int_sts_t          sha_int_sts;                  /*!< Indicates the sha interrupt status. */
    volatile uint32_t               reserved[17];                 /*!< Reserved. */
    volatile sha_ctrl_t             sha_ctrl;                     /*!< SHA module control */
    volatile sha_cfg_t              sha_cfg;                      /*!< SHA parameter configuration */
    volatile uint32_t               sha_data_len;                 /*!< Length of operation data */
    volatile uint32_t               sha_ivin_start_addr;          /*!< IVIN start address */
    volatile uint32_t               sha_data_in_start_addr;       /*!< Start address for fetching data */
    volatile uint32_t               sha_data_out_start_addr;      /*!< Storage start address */
    volatile uint32_t               sha_data_len_all;             /*!< Length of operation data */
    volatile sha_fsm_st_t           sha_fsm_st;                   /*!< Operational state machine */
} sha_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif