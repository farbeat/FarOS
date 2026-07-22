/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa v2 regs operation header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_RSA_V2_REGS_OP_H
#define HAL_RSA_V2_REGS_OP_H

#include "hal_rsa_v2_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa_v2_regs_op hal rsa v2 regs op funcs
 * @ingroup  drivers_hal_pke
 * @{
 */

extern rsa_regs_t *g_rsa_v2_regs;

/**
 * @brief  Init the rsa which will set the base address of registers.
 * @return 0 indicates the base address of registers has been configured success, -1 indicates failure.
 */
int32_t hal_rsa_v2_regs_init(void);

/**
 * @brief  Deinit the hal rsa which will clear the base address of registers has been
 *         set by @ref hal_rsa_v2_regs_init.
 */
void hal_rsa_v2_regs_deinit(void);

/**
 * @brief  Set the value of @ref rsav2_cfg_ahbm_timeout_waterline.ahbm_timeout_waterline
 * @param  [in] ahbm_timeout_waterline The value of @ref rsav2_cfg_ahbm_timeout_waterline.ahbm_timeout_waterline
 */
static inline void hal_set_rsav2_cfg_ahbm_timeout_waterline_ahbm_timeout_waterline(uint32_t ahbm_timeout_waterline)
{
    rsav2_cfg_ahbm_timeout_waterline_t rsav2_cfg_ahbm_timeout_waterline;
    rsav2_cfg_ahbm_timeout_waterline.d32 = g_rsa_v2_regs->rsav2_cfg_ahbm_timeout_waterline.d32;
    rsav2_cfg_ahbm_timeout_waterline.b.ahbm_timeout_waterline = ahbm_timeout_waterline;
    g_rsa_v2_regs->rsav2_cfg_ahbm_timeout_waterline.d32 = rsav2_cfg_ahbm_timeout_waterline.d32;
}

/**
 * @brief  Set the value of @ref rsav2_otskcfg_start.otask_start
 * @param  [in] otask_start The value of @ref rsav2_otskcfg_start.otask_start
 */
static inline void hal_set_rsav2_otskcfg_start_otask_start(uint32_t otask_start)
{
    rsav2_otskcfg_start_t rsav2_otskcfg_start;
    rsav2_otskcfg_start.d32 = g_rsa_v2_regs->rsav2_otskcfg_start.d32;
    rsav2_otskcfg_start.b.otask_start = otask_start;
    g_rsa_v2_regs->rsav2_otskcfg_start.d32 = rsav2_otskcfg_start.d32;
}

/**
 * @brief  Set the value of @ref rsav2_otskcfg_stop.otask_force_end
 * @param  [in] otask_force_end The value of @ref rsav2_otskcfg_stop.otask_force_end
 */
static inline void hal_set_rsav2_otskcfg_stop_otask_force_end(uint32_t otask_force_end)
{
    rsav2_otskcfg_stop_t rsav2_otskcfg_stop;
    rsav2_otskcfg_stop.d32 = g_rsa_v2_regs->rsav2_otskcfg_stop.d32;
    rsav2_otskcfg_stop.b.otask_force_end = otask_force_end;
    g_rsa_v2_regs->rsav2_otskcfg_stop.d32 = rsav2_otskcfg_stop.d32;
}

/**
 * @brief  Set the value of @ref rsav2_otskcfg_attribute.otask_len
 * @param  [in] otask_len The value of @ref rsav2_otskcfg_attribute.otask_len
 */
static inline void hal_set_rsav2_otskcfg_attribute_otask_len(uint32_t otask_len)
{
    rsav2_otskcfg_attribute_t rsav2_otskcfg_attribute;
    rsav2_otskcfg_attribute.d32 = g_rsa_v2_regs->rsav2_otskcfg_attribute.d32;
    rsav2_otskcfg_attribute.b.otask_len = otask_len;
    g_rsa_v2_regs->rsav2_otskcfg_attribute.d32 = rsav2_otskcfg_attribute.d32;
}

/**
 * @brief  Set the value of @ref rsav2_otskcfg_attribute.plaintext_len_sel
 * @param  [in] plaintext_len_sel The value of @ref rsav2_otskcfg_attribute.plaintext_len_sel
 */
static inline void hal_set_rsav2_otskcfg_attribute_plaintext_len_sel(uint32_t plaintext_len_sel)
{
    rsav2_otskcfg_attribute_t rsav2_otskcfg_attribute;
    rsav2_otskcfg_attribute.d32 = g_rsa_v2_regs->rsav2_otskcfg_attribute.d32;
    rsav2_otskcfg_attribute.b.plaintext_len_sel = plaintext_len_sel;
    g_rsa_v2_regs->rsav2_otskcfg_attribute.d32 = rsav2_otskcfg_attribute.d32;
}

/**
 * @brief  Set the plain text address of otask
 * @param  [in] otask_ptext_addr The plain text address of otask
 */
static inline void hal_set_rsav2_otskcfg_plaintext_addr_otask_ptext_addr(uint32_t otask_ptext_addr)
{
    g_rsa_v2_regs->rsav2_otskcfg_plaintext_addr = otask_ptext_addr;
}

/**
 * @brief  Set the key address of otask
 * @param  [in] otask_key_addr The key address of otask
 */
static inline void hal_set_rsav2_otskcfg_key_addr_otask_key_addr(uint32_t otask_key_addr)
{
    g_rsa_v2_regs->rsav2_otskcfg_key_addr = otask_key_addr;
}

/**
 * @brief  Set the result address of otask
 * @param  [in] otask_result_addr The result address of otask
 */
static inline void hal_set_rsav2_otskcfg_result_addr_otask_result_addr(uint32_t otask_result_addr)
{
    g_rsa_v2_regs->rsav2_otskcfg_result_addr = otask_result_addr;
}

/**
 * @brief  Set the value of @ref rsav2_intmsk.nc_cipher_done_msk
 * @param  [in] nc_cipher_done_msk The value of @ref rsav2_intmsk.nc_cipher_done_msk
 */
static inline void hal_set_rsav2_intmsk_nc_cipher_done_msk(uint32_t nc_cipher_done_msk)
{
    rsav2_intmsk_t rsav2_intmsk;
    rsav2_intmsk.d32 = g_rsa_v2_regs->rsav2_intmsk.d32;
    rsav2_intmsk.b.nc_cipher_done_msk = nc_cipher_done_msk;
    g_rsa_v2_regs->rsav2_intmsk.d32 = rsav2_intmsk.d32;
}

/**
 * @brief  Set the value of @ref rsav2_intmsk.nc_fail_msk
 * @param  [in] nc_fail_msk The value of @ref rsav2_intmsk.nc_fail_msk
 */
static inline void hal_set_rsav2_intmsk_nc_fail_msk(uint32_t nc_fail_msk)
{
    rsav2_intmsk_t rsav2_intmsk;
    rsav2_intmsk.d32 = g_rsa_v2_regs->rsav2_intmsk.d32;
    rsav2_intmsk.b.nc_fail_msk = nc_fail_msk;
    g_rsa_v2_regs->rsav2_intmsk.d32 = rsav2_intmsk.d32;
}

/**
 * @brief  Set the value of @ref rsav2_intmsk.ahbm_timeout_msk
 * @param  [in] ahbm_timeout_msk The value of @ref rsav2_intmsk.ahbm_timeout_msk
 */
static inline void hal_set_rsav2_intmsk_ahbm_timeout_msk(uint32_t ahbm_timeout_msk)
{
    rsav2_intmsk_t rsav2_intmsk;
    rsav2_intmsk.d32 = g_rsa_v2_regs->rsav2_intmsk.d32;
    rsav2_intmsk.b.ahbm_timeout_msk = ahbm_timeout_msk;
    g_rsa_v2_regs->rsav2_intmsk.d32 = rsav2_intmsk.d32;
}

/**
 * @brief  Set the value of @ref rsav2_rint.nc_cipher_done_rint
 * @param  [in] nc_cipher_done_rint The value of @ref rsav2_rint.nc_cipher_done_rint
 */
static inline void hal_set_rsav2_rint_nc_cipher_done_rint(uint32_t nc_cipher_done_rint)
{
    rsav2_rint_t rsav2_rint;
    rsav2_rint.d32 = g_rsa_v2_regs->rsav2_rint.d32;
    rsav2_rint.b.nc_cipher_done_rint = nc_cipher_done_rint;
    g_rsa_v2_regs->rsav2_rint.d32 = rsav2_rint.d32;
}

/**
 * @brief  Set the value of @ref rsav2_rint.nc_fail_rint
 * @param  [in] nc_fail_rint The value of @ref rsav2_rint.nc_fail_rint
 */
static inline void hal_set_rsav2_rint_nc_fail_rint(uint32_t nc_fail_rint)
{
    rsav2_rint_t rsav2_rint;
    rsav2_rint.d32 = g_rsa_v2_regs->rsav2_rint.d32;
    rsav2_rint.b.nc_fail_rint = nc_fail_rint;
    g_rsa_v2_regs->rsav2_rint.d32 = rsav2_rint.d32;
}

/**
 * @brief  Set the value of @ref rsav2_rint.ahbm_timeout_rint
 * @param  [in] ahbm_timeout_rint The value of @ref rsav2_rint.ahbm_timeout_rint
 */
static inline void hal_set_rsav2_rint_ahbm_timeout_rint(uint32_t ahbm_timeout_rint)
{
    rsav2_rint_t rsav2_rint;
    rsav2_rint.d32 = g_rsa_v2_regs->rsav2_rint.d32;
    rsav2_rint.b.ahbm_timeout_rint = ahbm_timeout_rint;
    g_rsa_v2_regs->rsav2_rint.d32 = rsav2_rint.d32;
}

/**
 * @brief  Set the value of @ref rsav2_intset.nc_cipher_done_intset
 * @param  [in] nc_cipher_done_intset The value of @ref rsav2_intset.nc_cipher_done_intset
 */
static inline void hal_set_rsav2_intset_nc_cipher_done_intset(uint32_t nc_cipher_done_intset)
{
    rsav2_intset_t rsav2_intset;
    rsav2_intset.d32 = g_rsa_v2_regs->rsav2_intset.d32;
    rsav2_intset.b.nc_cipher_done_intset = nc_cipher_done_intset;
    g_rsa_v2_regs->rsav2_intset.d32 = rsav2_intset.d32;
}

/**
 * @brief  Set the value of @ref rsav2_intset.nc_fail_intset
 * @param  [in] nc_fail_intset The value of @ref rsav2_intset.nc_fail_intset
 */
static inline void hal_set_rsav2_intset_nc_fail_intset(uint32_t nc_fail_intset)
{
    rsav2_intset_t rsav2_intset;
    rsav2_intset.d32 = g_rsa_v2_regs->rsav2_intset.d32;
    rsav2_intset.b.nc_fail_intset = nc_fail_intset;
    g_rsa_v2_regs->rsav2_intset.d32 = rsav2_intset.d32;
}

/**
 * @brief  Set the value of @ref rsav2_intset.ahbm_timeout_intset
 * @param  [in] ahbm_timeout_intset The value of @ref rsav2_intset.ahbm_timeout_intset
 */
static inline void hal_set_rsav2_intset_ahbm_timeout_intset(uint32_t ahbm_timeout_intset)
{
    rsav2_intset_t rsav2_intset;
    rsav2_intset.d32 = g_rsa_v2_regs->rsav2_intset.d32;
    rsav2_intset.b.ahbm_timeout_intset = ahbm_timeout_intset;
    g_rsa_v2_regs->rsav2_intset.d32 = rsav2_intset.d32;
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
