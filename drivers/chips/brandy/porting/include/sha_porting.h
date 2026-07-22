/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash sha port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef SHA_PORTING_H
#define SHA_PORTING_H

#include <stdint.h>
#include "hal_sha.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SECURITY_HARDENING               YES
#define SHA512_SUPPORT                   NO
#ifndef SHA_LAST_PACKAGE_IS_BIG_ENDIAN
#define SHA_LAST_PACKAGE_IS_BIG_ENDIAN   NO
#endif
#define SHA_FLASH_XIP_ADDR_REMAP_SUPPORT NO

/**
 * @defgroup drivers_port_hash Hash Sha
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief definition of bytes order.
 */
typedef enum endian_mode {
    LITTLE_ENDIAN_32BIT = 0,
    LITTLE_ENDIAN_64BIT = 1,
    BIG_ENDIAN_32BIT = 2,
    BIG_ENDIAN_64BIT = 3,
} endian_mode_t;

/**
 * @brief  register hal funcs objects from hal sha module.
 */
void sha_port_register_hal_funcs(void);

/**
 * @brief  register hal funcs objects from hal sha module.
 */
void sha_port_unregister_hal_funcs(void);

/**
 * @brief  Get the hal funcs of sha.
 * @return the hal funcs of sha, see @ref hal_sha_funcs_t
 */
hal_sha_funcs_t *sha_port_get_funcs(void);

/**
 * @brief  Get the base addr of sha sha.
 * @return the base addr of sha sha
 */
uintptr_t sha_get_base_addr(void);

/**
 * @brief  Register the interrupt of sha.
 */
void sha_port_register_irq(void);

/**
 * @brief  Register the interrupt of sha.
 */
void sha_port_unregister_irq(void);

/**
 * @brief  sha interrupt handler.
 */
void sha_irq_handler(void);

/**
 * @brief sha set clock en
 * @param [in] clken clock en ctrl
 */
void sha_port_set_clk_en(uint32_t clken);

/**
 * @brief sha set soft reset
 * @param [in] soft_reset soft_reset ctrl
 */
void sha_port_set_soft_reset(uint32_t soft_reset);

/**
 * @brief  Get the endian config of sha.
 * @return the endian config of sha, see @ref endian_mode_t
 */
endian_mode_t sha_get_endian(void);

/**
 * @brief  Set the endian config of sha.
 * @param [in] cfg_endian
 * @return the endian config of sha, see @ref endian_mode_t
 */
void sha_set_endian(uint32_t cfg_endian);

/**
 * @brief  Claen sha cache.
 * @param  addr The address need clean.
 * @param  size The size of cache memory.
 */
void sha_port_clean_cache(uint32_t *addr, unsigned int size);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif