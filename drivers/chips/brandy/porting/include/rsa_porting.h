/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa port template header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef RSA_PORTING_H
#define RSA_PORTING_H

#include <stdint.h>
#include "hal_rsa.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SECURITY_HARDENING  NO
#define RSA_ADDR_ALIGN      64
#define RSA_MEM_SIZE        1024

extern uint8_t g_rsa_key_mem[RSA_MEM_SIZE];
extern uint8_t g_rsa_input_mem[RSA_MEM_SIZE];
extern uint8_t g_rsa_output_mem[RSA_MEM_SIZE];

#define RSA_KEY_MEM     g_rsa_key_mem
#define RSA_INPUT_MEM   g_rsa_input_mem
#define RSA_OUTPUT_MEM  g_rsa_output_mem

/**
 * @defgroup drivers_port_pke Pke
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief definition of bytes order.
 */
typedef enum endian_mode {
    RSA_LITTLE_ENDIAN_32BIT = 0,
    RSA_LITTLE_ENDIAN_64BIT = 1,
    RSA_BIG_ENDIAN_32BIT = 2,
    RSA_BIG_ENDIAN_64BIT = 3,
} rsa_endian_mode_t;

/**
 * @brief  register hal funcs objects from hal rsa module.
 */
void rsa_port_register_hal_funcs(void);

/**
 * @brief  register hal funcs objects from hal rsa module.
 */
void rsa_port_unregister_hal_funcs(void);

/**
 * @brief  Get the hal funcs of rsa.
 * @return the hal funcs of rsa, see @ref hal_rsa_funcs_t
 */
hal_rsa_funcs_t *rsa_port_get_funcs(void);

/**
 * @brief  Get the base addr of rsa rsa.
 * @return the base addr of rsa rsa
 */
uintptr_t rsa_get_base_addr(void);

/**
 * @brief  Register the interrupt of rsa.
 */
void rsa_port_register_irq(void);

/**
 * @brief  Register the interrupt of rsa.
 */
void rsa_port_unregister_irq(void);

/**
 * @brief  rsa interrupt handler.
 */
void rsa_irq_handler(void);

/**
 * @brief rsa set clock en
 * @param [in] clken clock en ctrl
 */
void rsa_port_set_clk_en(uint32_t clken);

/**
 * @brief rsa set soft reset
 * @param [in] soft_reset soft_reset ctrl
 */
void rsa_port_set_soft_reset(uint32_t soft_reset);

/**
 * @brief  Get the endian config of rsa.
 * @return the endian config of rsa, see @ref endian_mode_t
 */
rsa_endian_mode_t rsa_get_endian(void);

/**
 * @brief  Set the endian config of rsa.
 * @param [in] cfg_endian the endian config of rsa, see @ref endian_mode_t
 */
void rsa_set_endian(uint32_t cfg_endian);

/**
 * @brief  Set the attribute of rsa.
 * @param [in] rsa_attr the attribute of rsa, see @ref rsa_attr_t
 * @param [in] n the public module of rsa
 * @param [in] key the exponent of rsa
 * @param [in] key_len the length of key
 * @param [in] data the input plain data
 * @param [in] data_len the length of plain data
 */
errcode_t rsa_set_attr(rsa_attr_t *rsa_attr, const uint8_t *n, const uint8_t *key, const uint32_t key_len,
    const uint8_t *data, const uint32_t data_len);
/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif