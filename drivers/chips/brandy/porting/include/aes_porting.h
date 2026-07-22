/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes port template header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef AES_PORTING_H
#define AES_PORTING_H

#include <stdint.h>
#include "hal_aes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IV_UPDATE_SUPPORT               NO

#define CRYPTO_HARD_CHANNEL_MAX         0x1

#define AES_TAG_SIZE                    16
#define AES_IV_SIZE                     16
#define AES_AAD_SIZE                    16
#define AES_PARAM_ALIGN                 16
#define AES_ADDR_ALIGN                  16
#define AES_INPUT_BUF_SIZE              1024
#define AES_OUTPUT_BUF_SIZE             1024

extern uint8_t g_drv_aes_iv[AES_IV_SIZE];
extern uint8_t g_drv_aes_aad[AES_AAD_SIZE];
extern uint8_t g_drv_aes_tag[AES_TAG_SIZE];
extern uint8_t g_drv_aes_input_addr[AES_INPUT_BUF_SIZE];
extern uint8_t g_drv_aes_output_addr[AES_OUTPUT_BUF_SIZE];

#define AES_INPUT_ADDR                  g_drv_aes_input_addr
#define AES_IV_ADDR                     g_drv_aes_iv
#define AES_OUTPUT_ADDR                 g_drv_aes_output_addr
#define AES_AAD_ADDR                    g_drv_aes_aad
#define AES_TAG_ADDR                    g_drv_aes_tag

/**
 * @defgroup drivers_port_hash Hash
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief definition of bytes order.
 */
typedef enum endian_mode {
    AES_LITTLE_ENDIAN_32BIT = 0,
    AES_LITTLE_ENDIAN_64BIT = 1,
    AES_BIG_ENDIAN_32BIT = 2,
    AES_BIG_ENDIAN_64BIT = 3,
} aes_endian_mode_t;

/**
 * @brief  register hal funcs objects from hal aes module.
 */
void aes_port_register_hal_funcs(void);

/**
 * @brief  register hal funcs objects from hal aes module.
 */
void aes_port_unregister_hal_funcs(void);

/**
 * @brief  Get the hal funcs of aes.
 * @return the hal funcs of aes, see @ref hal_aes_funcs_t
 */
hal_aes_funcs_t *aes_port_get_funcs(void);

/**
 * @brief  Get the base addr of aes aes.
 * @return the base addr of aes aes
 */
uintptr_t aes_get_base_addr(void);

/**
 * @brief  Register the interrupt of aes.
 */
void aes_port_register_irq(void);

/**
 * @brief  Register the interrupt of aes.
 */
void aes_port_unregister_irq(void);

/**
 * @brief  aes interrupt handler.
 */
void aes_irq_handler(void);

/**
 * @brief aes set clock en
 * @param [in] clken clock en ctrl
 */
void aes_port_set_clk_en(uint32_t clken);

/**
 * @brief aes set soft reset
 * @param [in] soft_reset soft_reset ctrl
 */
void aes_port_set_soft_reset(uint32_t soft_reset);

/**
 * @brief  Set the endian config of aes.
 * @param [in] cfg_endian
 * @return the endian config of aes, see @ref endian_mode_t
 */
void aes_set_endian(uint32_t cfg_endian);

/**
 * @brief  Get the endian config of aes.
 * @return the endian config of aes, see @ref endian_mode_t
 */
aes_endian_mode_t aes_get_endian(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
