/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal sha v2 regs operation header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_SHA_V2_REGS_OP_H
#define HAL_SHA_V2_REGS_OP_H

#include <stdio.h>
#include <stdint.h>
#include "hal_sha_v2_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_sha_regs_op Hash Sha v2 Regs Operation
 * @ingroup  drivers_hal_sha
 * @{
 */

extern sha_regs_t *g_sha_v2_regs;

/**
 * @brief  Init the sec common which will set the base address of registers.
 * @return 0 indicates the base address of registers has been configured success, -1 indicates failure.
 */
int32_t hal_sha_v2_regs_init(void);

/**
 * @brief  Init the sec common which will set the base address of registers.
 */
void hal_sha_v2_regs_deinit(void);

/**
 * @brief  Set the value of @ref sha_int_en.sha_int_en
 * @param  [in] sha_int_en The value of @ref sha_int_en.sha_int_en
 */
static inline void hal_set_sha_int_en(uint32_t sha_int_en)
{
    sha_int_en_t sha_int_en_tmp;
    sha_int_en_tmp.d32 = g_sha_v2_regs->sha_int_en.d32;
    sha_int_en_tmp.b.sha_int_en = sha_int_en;
    g_sha_v2_regs->sha_int_en.d32 = sha_int_en_tmp.d32;
}

/**
 * @brief  Set the value of @ref sha_int_clr.sha_int_clr
 * @param  [in] sha_int_clr The value of @ref sha_int_clr.sha_int_clr
 */
static inline void hal_set_sha_int_clr(uint32_t sha_int_clr)
{
    sha_int_clr_t sha_int_clr_tmp;
    sha_int_clr_tmp.d32 = g_sha_v2_regs->sha_int_clr.d32;
    sha_int_clr_tmp.b.sha_int_clr = sha_int_clr;
    g_sha_v2_regs->sha_int_clr.d32 = sha_int_clr_tmp.d32;
}

/**
 * @brief  Set the value of @ref sha_ctrl.sha_en
 * @param  [in] sha_en The value of @ref sha_ctrl.sha_en
 */
static inline void hal_set_sha_ctrl_sha_en(uint32_t sha_en)
{
    sha_ctrl_t sha_ctrl;
    sha_ctrl.d32 = g_sha_v2_regs->sha_ctrl.d32;
    sha_ctrl.b.sha_en = sha_en;
    g_sha_v2_regs->sha_ctrl.d32 = sha_ctrl.d32;
}

/**
 * @brief  Set the value of @ref sha_ctrl.sha_start
 * @param  [in] sha_start The value of @ref sha_ctrl.sha_start
 */
static inline void hal_set_sha_ctrl_sha_start(uint32_t sha_start)
{
    sha_ctrl_t sha_ctrl;
    sha_ctrl.d32 = g_sha_v2_regs->sha_ctrl.d32;
    sha_ctrl.b.sha_start = sha_start;
    g_sha_v2_regs->sha_ctrl.d32 = sha_ctrl.d32;
}

/**
 * @brief  Set the value of @ref sha_cfg.sha_ivin_sel
 * @param  [in] sha_ivin_sel The value of @ref sha_cfg.sha_ivin_sel
 */
static inline void hal_set_sha_cfg_sha_ivin_sel(uint32_t sha_ivin_sel)
{
    sha_cfg_t sha_cfg;
    sha_cfg.d32 = g_sha_v2_regs->sha_cfg.d32;
    sha_cfg.b.sha_ivin_sel = sha_ivin_sel;
    g_sha_v2_regs->sha_cfg.d32 = sha_cfg.d32;
}

/**
 * @brief  Set the value of @ref sha_cfg.sha_padding_dis
 * @param  [in] sha_padding_dis The value of @ref sha_cfg.sha_padding_dis
 */
static inline void hal_set_sha_cfg_sha_padding_dis(uint32_t sha_padding_dis)
{
    sha_cfg_t sha_cfg;
    sha_cfg.d32 = g_sha_v2_regs->sha_cfg.d32;
    sha_cfg.b.sha_padding_dis = sha_padding_dis;
    g_sha_v2_regs->sha_cfg.d32 = sha_cfg.d32;
}

#if HASH_SHA512_SUPPORT == YES
/**
 * @brief  Set the value of compute mode of sha_cfg register when sha512 exist.
 * @param  [in] sha_mode The compute mode of sha_cfg register.
 */
static inline void hal_set_sha_cfg_sha_compute_mode(uint32_t sha_mode)
{
    sha_cfg_t sha_cfg;
    sha_cfg.d32 = g_sha_v2_regs->sha_cfg.d32;
    sha_cfg.b.sha_compute_mode = sha_mode;
    g_sha_v2_regs->sha_cfg.d32 = sha_cfg.d32;
}
#endif

/**
 * @brief  set the value of sha_data_len
 * @param  [in] sha_data_len length of sha_data
 */
static inline void hal_set_sha_data_len_sha_data_len(uint32_t sha_data_len)
{
    g_sha_v2_regs->sha_data_len = sha_data_len;
}

/**
 * @brief  Set the value of sha_ivin_start_addr
 * @param  [in] sha_ivin_start_addr start addr of input sha_iv
 */
static inline void hal_set_sha_ivin_start_addr_sha_ivin_start_addr(uint32_t sha_ivin_start_addr)
{
    g_sha_v2_regs->sha_ivin_start_addr = sha_ivin_start_addr;
}

/**
 * @brief  Set the value of sha_data_in_start_addr
 * @param  [in] sha_data_in_start_addr start addr of input sha_data
 */
static inline void hal_set_sha_data_in_start_addr_sha_data_in_start_addr(uint32_t sha_data_in_start_addr)
{
    g_sha_v2_regs->sha_data_in_start_addr = sha_data_in_start_addr;
}

/**
 * @brief  Set the value of sha_data_out_start_addr
 * @param  [in] sha_data_out_start_addr output addr of sha_data
 */
static inline void hal_set_sha_data_out_start_addr_sha_data_out_start_addr(uint32_t sha_data_out_start_addr)
{
    g_sha_v2_regs->sha_data_out_start_addr = sha_data_out_start_addr;
}

/**
 * @brief  Set the value of sha_data_len_all
 * @param  [in] sha_data_len_all length of all sha_data
 */
static inline void hal_set_sha_data_len_all_sha_data_len_all(uint32_t sha_data_len_all)
{
    g_sha_v2_regs->sha_data_len_all = sha_data_len_all;
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