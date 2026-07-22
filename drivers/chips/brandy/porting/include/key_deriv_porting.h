/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef KEY_DERIV_PORTING_H
#define KEY_DERIV_PORTING_H

#include <stdint.h>
#include "errcode.h"
#include "hal_key_deriv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_port_km Key_deriv
 * @ingroup  drivers_port
 * @{
 */

typedef enum endian_mode {
    KEY_DERIV_LITTLE_ENDIAN_32BIT = 0,
    KEY_DERIV_LITTLE_ENDIAN_64BIT = 1,
    KEY_DERIV_BIG_ENDIAN_32BIT = 2,
    KEY_DERIV_BIG_ENDIAN_64BIT = 3,
} key_deriv_endian_mode_t;

/**
 * @brief  register hal funcs objects from hal key_deriv module.
 */
void key_deriv_port_register_hal_funcs(void);

/**
 * @brief  register hal funcs objects from hal key_deriv module.
 */
void key_deriv_port_unregister_hal_funcs(void);

/**
 * @brief  Get the hal funcs of key_deriv.
 * @return the hal funcs of key_deriv, see @ref hal_key_deriv_funcs_t
 */
hal_key_deriv_funcs_t *key_deriv_port_get_funcs(void);

/**
 * @brief  Get the base addr of key_deriv key_deriv.
 * @return the base addr of key_deriv key_deriv
 */
uintptr_t key_deriv_get_base_addr(void);

/**
 * @brief  Register the interrupt of key_deriv.
 */
void key_deriv_port_register_irq(void);

/**
 * @brief  Register the interrupt of key_deriv.
 */
void key_deriv_port_unregister_irq(void);

/**
 * @brief  key_deriv interrupt handler.
 */
void key_deriv_irq_handler(void);

/**
 * @brief key_deriv set clock en
 * @param [in] clken clock en ctrl
 */
void key_deriv_port_set_clk_en(uint32_t clken);

/**
 * @brief key_deriv set soft reset
 * @param [in] soft_reset soft_reset ctrl
 */
void key_deriv_port_set_soft_reset(uint32_t soft_reset);

/**
 * @brief  Set the endian config of key deriv.
 * @param [in] cfg_endian
 * @return the endian config of key deriv, see @ref endian_mode_t
 */
void key_deriv_set_endian(uint32_t cfg_endian);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif