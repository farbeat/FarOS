/**
 * Copyright (c)@CompanyNameMagicTag 2022-2022. All rights reserved. \n
 * Description: Provides aes v2 regs operation header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_AES_V2_REGS_OP_H
#define HAL_AES_V2_REGS_OP_H

#include <stdint.h>
#include "hal_aes_v2_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_aes_regs_op hash aes v2 Regs operation
 * @ingroup  drivers_hal_cipher
 * @{
 */

extern aes_regs_t *g_aes_v2_regs;

/**
 * @brief  Init the aes which will set the base address of registers.
 * @return 0 indicates the base address of registers has been configured success, -1 indicates failure.
 */
int32_t hal_aes_v2_regs_init(void);

/**
 * @brief  Deinit the hal aes which will clear the base address of registers has been
 *         set by @ref hal_aes_v2_regs_init.
 */
void hal_aes_v2_regs_deinit(void);

/**
 * @brief  Set the value of @ref aes_int_clr.aes_int_clr
 * @param  [in]  aes_int_clr The value of @ref aes_int_clr.aes_int_clr.
 */
static inline void hal_set_aes_int_clr_aes_int_clr(uint32_t aes_int_clr)
{
    aes_int_clr_t aes_int_clr_tmp;
    aes_int_clr_tmp.d32 = g_aes_v2_regs->aes_int_clr.d32;
    aes_int_clr_tmp.b.aes_int_clr = aes_int_clr;
    g_aes_v2_regs->aes_int_clr.d32 = aes_int_clr_tmp.d32;
}

/**
 * @brief  Set the value of @ref aes_int_en_aes_int_en.
 * @param  [in]  aes_int_en The value of @ref aes_int_en.aes_int_en.
 */
static inline void hal_set_aes_int_en_aes_int_en(uint32_t aes_int_en)
{
    aes_int_en_t aes_int_en_tmp;
    aes_int_en_tmp.d32 = g_aes_v2_regs->aes_int_en.d32;
    aes_int_en_tmp.b.aes_int_en = aes_int_en;
    g_aes_v2_regs->aes_int_en.d32 = aes_int_en_tmp.d32;
}

/**
 * @brief  Set the value of @ref aes_ctrl.aes_en
 * @param  [in] aes_en The value of @ref aes_ctrl.aes_en
 */
static inline void hal_set_aes_ctrl_aes_en(uint32_t aes_en)
{
    aes_ctrl_t aes_ctrl;
    aes_ctrl.d32 = g_aes_v2_regs->aes_ctrl.d32;
    aes_ctrl.b.aes_en = aes_en;
    g_aes_v2_regs->aes_ctrl.d32 = aes_ctrl.d32;
}

/**
 * @brief  Set the value of @ref aes_ctrl.aes_start
 * @param  [in] aes_start The value of @ref aes_ctrl.aes_start
 */
static inline void hal_set_aes_ctrl_aes_start(uint32_t aes_start)
{
    aes_ctrl_t aes_ctrl;
    aes_ctrl.d32 = g_aes_v2_regs->aes_ctrl.d32;
    aes_ctrl.b.aes_start = aes_start;
    g_aes_v2_regs->aes_ctrl.d32 = aes_ctrl.d32;
}

/**
 * @brief  Set the value of @ref aes_cfg.aes_decrypt
 * @param  [in] aes_decrypt The value of @ref aes_cfg.aes_decrypt
 */
static inline void hal_set_aes_cfg_aes_decrypt(uint32_t aes_decrypt)
{
    aes_cfg_t aes_cfg;
    aes_cfg.d32 = g_aes_v2_regs->aes_cfg.d32;
    aes_cfg.b.aes_decrypt = aes_decrypt;
    g_aes_v2_regs->aes_cfg.d32 = aes_cfg.d32;
}

/**

 * @brief  Set the value of @ref aes_cfg.aes_mode
 * @param  [in] aes_mode The value of @ref aes_cfg.aes_mode
 */
static inline void hal_set_aes_cfg_aes_mode(uint32_t aes_mode)
{
    aes_cfg_t aes_cfg;
    aes_cfg.d32 = g_aes_v2_regs->aes_cfg.d32;
    aes_cfg.b.aes_mode = aes_mode;
    g_aes_v2_regs->aes_cfg.d32 = aes_cfg.d32;
}

/**
 * @brief  Set the value of @ref aes_cfg.aes_key_length
 * @param  [in] aes_key_length The value of @ref aes_cfg.aes_key_length
 */
static inline void hal_set_aes_cfg_aes_key_length(uint32_t aes_key_length)
{
    aes_cfg_t aes_cfg;
    aes_cfg.d32 = g_aes_v2_regs->aes_cfg.d32;
    aes_cfg.b.aes_key_length = aes_key_length;
    g_aes_v2_regs->aes_cfg.d32 = aes_cfg.d32;
}

/**
 * @brief  Set the value of @ref aes_cfg.aes_key_sel
 * @param  [in] aes_key_sel The value of @ref aes_cfg.aes_key_sel
 */
static inline void hal_set_aes_cfg_aes_key_sel(uint32_t aes_key_sel)
{
    aes_cfg_t aes_cfg;
    aes_cfg.d32 = g_aes_v2_regs->aes_cfg.d32;
    aes_cfg.b.aes_key_sel = aes_key_sel;
    g_aes_v2_regs->aes_cfg.d32 = aes_cfg.d32;
}

/**
 * @brief  Set the value of @ref aes_cfg.aes_key_id.
 * @param  [in] aes_key_id The value of @ref aes_cfg.aes_key_id
 */
static inline void hal_set_aes_cfg_aes_key_id(uint32_t aes_key_id)
{
    aes_cfg_t aes_cfg;
    aes_cfg.d32 = g_aes_v2_regs->aes_cfg.d32;
    aes_cfg.b.aes_key_id = aes_key_id;
    g_aes_v2_regs->aes_cfg.d32 = aes_cfg.d32;
}

/**
 * @brief  Set the length of aes data.
 * @param  [in] aes_data_len The value of aes data length
 */
static inline void hal_set_aes_data_len_aes_data_len(uint32_t aes_data_len)
{
    g_aes_v2_regs->aes_data_len = aes_data_len;
}

/**
 * @brief  Set the length of aes data.
 * @param  [in] aes_data_len The value of aes data length
 */
static inline void hal_set_aes_aad_len_aes_aad_len(uint32_t aes_aad_len)
{
    aes_aad_len_t aes_aad_len_tmp;
    aes_aad_len_tmp.d32 = g_aes_v2_regs->aes_aad_len.d32;
    aes_aad_len_tmp.b.aes_aad_len = aes_aad_len;
    g_aes_v2_regs->aes_aad_len.d32 = aes_aad_len_tmp.d32;
}

/**
 * @brief  Set the start address of aes key
 * @param  [in] aes_key_start_addr The start address of aes key
 */
static inline void hal_set_aes_key_start_addr(uint32_t aes_key_start_addr)
{
    g_aes_v2_regs->aes_key_start_addr = aes_key_start_addr;
}

/**
 * @brief  Set the start address of aes iv
 * @param  [in] aes_iv_start_addr The start address of aes iv
 */
static inline void hal_set_aes_iv_start_addr(uint32_t aes_iv_start_addr)
{
    g_aes_v2_regs->aes_iv_start_addr = aes_iv_start_addr;
}

/**
 * @brief  Set the start address of auth iv
 * @param  [in] aes_iv_start_addr The start address of aes auth iv
 */
static inline void hal_set_aes_auth_iv_start_addr(uint32_t aes_auth_iv_start_addr)
{
    g_aes_v2_regs->aes_auth_iv_start_addr = aes_auth_iv_start_addr;
}

/**
 * @brief  Set the start address of aes aad data
 * @param  [in] aes_iv_start_addr the start address of aes aad data
 */
static inline void hal_set_aes_aad_start_addr(uint32_t aes_aad_start_addr)
{
    g_aes_v2_regs->aes_aad_start_addr = aes_aad_start_addr;
}

/**
 * @brief  Set the start address of aes data
 * @param  [in] aes_iv_start_addr the start address of aes data
 */
static inline void hal_set_aes_data_in_start_addr(uint32_t aes_data_in_start_addr)
{
    g_aes_v2_regs->aes_data_in_start_addr = aes_data_in_start_addr;
}

/**
 * @brief  Set the start address of aes output data
 * @param  [in] aes_iv_start_addr the start address of aes output data
 */
static inline void hal_set_aes_data_out_start_addr(uint32_t aes_data_out_start_addr)
{
    g_aes_v2_regs->aes_data_out_start_addr = aes_data_out_start_addr;
}

/**
 * @brief  Set the start address of aes tag
 * @param  [in] aes_iv_start_addr the start address of aes tag
 */
static inline void hal_set_aes_tag_start_addr(uint32_t aes_tag_start_addr)
{
    g_aes_v2_regs->aes_tag_start_addr = aes_tag_start_addr;
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