/**
 * Copyright (c)@CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal key deriv v2 regs operation header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_KEY_DERIV_V2_REGS_OP_H
#define HAL_KEY_DERIV_V2_REGS_OP_H

#include <stdint.h>
#include "hal_key_deriv_v2_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_sha_regs_op hash sha Regs operation
 * @ingroup  drivers_hal_hash
 * @{
 */

extern key_deriv_regs_t *g_key_deriv_v2_regs;

/**
 * @brief  Init the key deriv which will set the base address of registers.
 * @return 0 indicates the base address of registers has been configured success, -1 indicates failure.
 */
int32_t hal_key_deriv_v2_regs_init(void);

/**
 * @brief  Deinit the key deriv which will set the base address of registers.
 */
void hal_key_deriv_v2_regs_deinit(void);

/**
 * @brief  Set the value of @ref key_deriv_int_en.deriv_int_en.
 * @param  [in]  deriv_int_en The value of @ref key_deriv_int_en.deriv_int_en.
 */
static inline void hal_set_key_deriv_int_en(uint32_t deriv_int_en)
{
    key_deriv_int_en_t key_deriv_int_en;
    key_deriv_int_en.d32 = g_key_deriv_v2_regs->key_deriv_int_en.d32;
    key_deriv_int_en.b.deriv_int_en = deriv_int_en;
    g_key_deriv_v2_regs->key_deriv_int_en.d32 = key_deriv_int_en.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_int_clr.deriv_int_clr.
 * @param  [in]  deriv_int_clr The value of @ref key_deriv_int_clr.deriv_int_clr.
 */
static inline void hal_set_key_deriv_int_clr(uint32_t deriv_int_clr)
{
    key_deriv_int_clr_t key_deriv_int_clr;
    key_deriv_int_clr.d32 = g_key_deriv_v2_regs->key_deriv_int_clr.d32;
    key_deriv_int_clr.b.deriv_int_clr = deriv_int_clr;
    g_key_deriv_v2_regs->key_deriv_int_clr.d32 = key_deriv_int_clr.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_ctrl.deriv_en
 * @param  [in] deriv_en The value of @ref key_deriv_ctrl.deriv_en.
 */
static inline void hal_set_key_deriv_ctrl_deriv_en(uint32_t deriv_en)
{
    key_deriv_ctrl_t key_deriv_ctrl;
    key_deriv_ctrl.d32 = g_key_deriv_v2_regs->key_deriv_ctrl.d32;
    key_deriv_ctrl.b.deriv_en = deriv_en;
    g_key_deriv_v2_regs->key_deriv_ctrl.d32 = key_deriv_ctrl.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_ctrl.deriv_start
 * @param  [in] deriv_start The value of @ref key_deriv_ctrl.deriv_start.
 */
static inline void hal_set_key_deriv_ctrl_deriv_start(uint32_t deriv_start)
{
    key_deriv_ctrl_t key_deriv_ctrl;
    key_deriv_ctrl.d32 = g_key_deriv_v2_regs->key_deriv_ctrl.d32;
    key_deriv_ctrl.b.deriv_start = deriv_start;
    g_key_deriv_v2_regs->key_deriv_ctrl.d32 = key_deriv_ctrl.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_num.deriv_num
 * @param  [in]  deriv_num The value of @ref key_deriv_ctrl.deriv_num.
 */
static inline void hal_set_key_deriv_num_deriv_num(uint32_t deriv_num)
{
    key_deriv_num_t key_deriv_num;
    key_deriv_num.d32 = g_key_deriv_v2_regs->key_deriv_num.d32;
    key_deriv_num.b.deriv_num = deriv_num;
    g_key_deriv_v2_regs->key_deriv_num.d32 = key_deriv_num.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_cfg_deriv_key_id
 * @param  [in]  deriv_key_id The value of @ref key_deriv_cfg_deriv_key_id
 */
static inline void hal_set_key_deriv_cfg_deriv_key_id(uint32_t deriv_key_id)
{
    key_deriv_cfg_t key_deriv_cfg;
    key_deriv_cfg.d32 = g_key_deriv_v2_regs->key_deriv_cfg.d32;
    key_deriv_cfg.b.deriv_key_id = deriv_key_id;
    g_key_deriv_v2_regs->key_deriv_cfg.d32 = key_deriv_cfg.d32;
}

/**
 * @brief  Set the value of @ref key_deriv_cfg.root_key_sel
 * @param  [in]  root_key_sel The value of @ref key_deriv_cfg.root_key_sel
 */
static inline void hal_set_key_deriv_cfg_root_key_sel(uint32_t root_key_sel)
{
    key_deriv_cfg_t key_deriv_cfg;
    key_deriv_cfg.d32 = g_key_deriv_v2_regs->key_deriv_cfg.d32;
    key_deriv_cfg.b.root_key_sel = root_key_sel;
    g_key_deriv_v2_regs->key_deriv_cfg.d32 = key_deriv_cfg.d32;
}

/**
 * @brief  set the start addr of deriv
 * @param  [in] deriv_para_start_addr the start addr of deriv
 */
static inline void hal_set_deriv_para_start_addr_deriv_para_start_addr(uint32_t deriv_para_start_addr)
{
    g_key_deriv_v2_regs->deriv_para_start_addr = deriv_para_start_addr;
}

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif