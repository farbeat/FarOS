/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal key deriv v2 regs Definition \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_KEY_DERIV_V2_REGS_DEF_H
#define HAL_KEY_DERIV_V2_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_key_deriv_v2_regs_def key deriv Regs Definition
 * @ingroup  drivers_hal_km
 * @{
 */

/**
 * @brief  This union represents the bit fields in the key deriv int en register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t    deriv_int_en          : 1;        /*!< key_deriv interrupt enable */
        uint32_t    reserved_1            : 31;       /*!< reserved */
    } b;
    uint32_t    d32;                                  /*!< Raw register data. */
} key_deriv_int_en_t;

/**
 * @brief  This union represents the bit fields in the key deriv int clear register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t    deriv_int_clr         : 1;       /*!< key_deriv interrupt clear */
        uint32_t    reserved_1            : 31;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_int_clr_t;

/**
 * @brief  This union represents the bit fields in the key deriv int status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t    deriv_int_sts         : 1;       /*!< key_deriv interrupt status */
        uint32_t    reserved_1            : 31;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_int_sts_t;

/**
 * @brief  This union represents the bit fields in the key deriv ctrl register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union key_deriv_ctrl {
    struct {
        uint32_t    deriv_en              : 1;       /*!< key_deriv enable */
        uint32_t    deriv_start           : 1;       /*!< key_deriv start */
        uint32_t    deriv_end             : 1;       /*!< key_deriv end */
        uint32_t    reserved_0            : 29;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_ctrl_t;

/**
 * @brief  This union represents the bit fields in the key deriv times register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union key_deriv_num {
    struct {
        uint32_t    deriv_num             : 16;      /*!< Number of key derivation iterations */
        uint32_t    reserved_0            : 16;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_num_t;

/**
 * @brief  This union represents the bit fields in the key deriv cfg register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union key_deriv_cfg {
    struct {
        uint32_t    deriv_key_id          : 3;       /*!< key_deriv key id */
        uint32_t    root_key_sel          : 1;       /*!< Root key: 0: HUK; 1: GID */
        uint32_t    reserved_0            : 28;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_cfg_t;

/**
 * @brief  This union represents the bit fields in the key deriv id status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union key_deriv_id_sts {
    struct {
        uint32_t    deriv_id_sts          : 8;       /*!< Key ID status query.
                                                          Each bit indicates the corresponding key ID.
                                                          0: underived; 1: derived. */
        uint32_t    reserved_0            : 24;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_id_sts_t;

/**
 * @brief  This union represents the bit fields in the key deriv fsm status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union key_deriv_fsm_st {
    struct {
        uint32_t    deriv_fsm_st          : 3;       /*!< key deriv fsm status */
        uint32_t    reserved_0            : 29;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} key_deriv_fsm_st_t;

/**
 * @brief  Registers associated with key deriv module.
 */
typedef struct {
    volatile key_deriv_int_en_t     key_deriv_int_en     ;    /*!< key deriv interrupt en */
    volatile key_deriv_int_clr_t    key_deriv_int_clr    ;    /*!< key deriv interrupt clear */
    volatile key_deriv_int_sts_t    key_deriv_int_sts    ;    /*!< key deriv interrupt status */
    volatile uint32_t               reserved_2           ;    /*!< reserved */
    volatile key_deriv_ctrl_t       key_deriv_ctrl       ;    /*!< Key derivation control */
    volatile key_deriv_num_t        key_deriv_num        ;    /*!< Key derivation times */
    volatile key_deriv_cfg_t        key_deriv_cfg        ;    /*!< Key Parameter Configuration */
    volatile uint32_t               deriv_para_start_addr;    /*!< Derived Factor Address */
    volatile key_deriv_id_sts_t     key_deriv_id_sts     ;    /*!< Key derivation status */
    volatile key_deriv_fsm_st_t     key_deriv_fsm_st     ;    /*!< derived state machine */
} key_deriv_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif