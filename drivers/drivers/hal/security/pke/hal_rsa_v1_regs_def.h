/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides rsa v2 regs Definitions \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-06, Create file. \n
 */
#ifndef HAL_RSA_V1_REGS_DEF_H
#define HAL_RSA_V1_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa_v1_regs_def hal rsa v1 regs definition
 * @ingroup  drivers_hal_pke
 * @{
 */

#define RSA_4_BYTES_SHIFT    4
#define RSA_CRC_SHIFT        8
#define RSA_CRC_MASK         0xFF
#define RSA_CRC_START        0xFFFF
#define RSA_INPUT_TIMES      32

/**
 * @brief  This union represents the bit fields in the rsa v1 mode config register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t mode : 2;
        uint32_t key_width : 2;
        uint32_t data0_clr : 1;
        uint32_t data1_clr : 1;
        uint32_t data2_clr : 1;
        uint32_t reserved : 25;
    } b;
    uint32_t d32;
} rsa_mod_reg_t;

/**
 * @brief  This union represents the bit fields in the rsa v1 address config register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t addr_n : 8;
        uint32_t addre_d : 8;
        uint32_t addre_pkt : 8;
        uint32_t addere_rslt : 8;
    } b;
    uint32_t d32;
} rsa_addr_reg_t;

/**
 * @brief  This union represents the bit fields in the rsa v1 error config register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union {
    struct {
        uint32_t addr_n : 8;
        uint32_t addre_d : 8;
        uint32_t addre_pkt : 8;
        uint32_t addere_rslt : 8;
    } b;
    uint32_t d32;
} rsa_error_reg_t;

/**
 * @brief  Registers associated with rsav1.
 */
typedef struct {
    volatile uint32_t                             reserved_0[20];       /*!< reserved. */
    volatile uint32_t                             rsa_busy;             /*!< reserved. */
    volatile rsa_mod_reg_t                        rsa_mode;
    volatile uint32_t                             rsa_wsec;
    volatile uint32_t                             rsa_wpkt;
    volatile uint32_t                             rsa_rpkt;
    volatile uint32_t                             rsa_rrslt;
    volatile uint32_t                             rsa_start;
    volatile rsa_addr_reg_t                       rsa_addr;
    volatile rsa_error_reg_t                      rsa_error;
    volatile uint32_t                             rsa_crc16;
} rsav1_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
