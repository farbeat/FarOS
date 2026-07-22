/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 * Description: Provides hal aes v2 regs Definition \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_AES_V2_REGS_DEF_H
#define HAL_AES_V2_REGS_DEF_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_aes_v2_regs_def hal aes v2 Regs Definition
 * @ingroup  drivers_hal_cipher
 * @{
 */

/**
 * @brief  This union represents the bit fields in the aes int en register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_int_en {
    struct {
        uint32_t    reserved_0            : 3;        /*!< reserved */
        uint32_t    aes_int_en            : 1;        /*!< aes interrupt enable */
        uint32_t    reserved_1            : 28;       /*!< reserved */
    } b;
    uint32_t    d32;                                  /*!< Raw register data. */
} aes_int_en_t;

/**
 * @brief  This union represents the bit fields in the aes int clear register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_int_clr {
    struct {
        uint32_t    reserved_0            : 3;       /*!< reserved */
        uint32_t    aes_int_clr           : 1;       /*!< aes interrupt clear */
        uint32_t    reserved_1            : 28;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_int_clr_t;

/**
 * @brief  This union represents the bit fields in the aes int status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_int_sts {
    struct {
        uint32_t    reserved_0            : 3;       /*!< reserved */
        uint32_t    aes_int_sts           : 1;       /*!< aes interrupt status */
        uint32_t    reserved_1            : 28;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_int_sts_t;

/**
 * @brief  This union represents the bit fields in the aes ctrl register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_ctrl {
    struct {
        uint32_t    aes_en                : 1;       /*!< Global enabling of the AES module */
        uint32_t    aes_start             : 1;       /*!< AES operation start signal */
        uint32_t    aes_end               : 1;       /*!< AES operation completion status */
        uint32_t    reserved_0            : 29;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_ctrl_t;

/**
 * @brief  This union represents the bit fields in the aes cfg register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_cfg {
    struct {
        uint32_t    aes_decrypt           : 1;       /*!< aes decrypt/encrypt mode: 0:encrypt 1:decrypt */
        uint32_t    aes_mode              : 3;       /*!< AES mode:
                                                          0: ECB; 1: CBC; 3: OFB;
                                                          4: CTR; 5: CCM; 6: GCM; Other: invalid */
        uint32_t    aes_key_length        : 2;       /*!< AES key length: 0:128, 1:192, 2:256 */
        uint32_t    aes_key_sel           : 1;       /*!< Indicates the source of the AES operation key.
                                                          0: Use the local key based on the key ID. 1: Configured
                                                          by software. */
        uint32_t    aes_key_id            : 3;       /*!< aes key id */
        uint32_t    reserved_0            : 22;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_cfg_t;

/**
 * @brief  This union represents the bit fields in the aes add length register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_aad_len {
    struct {
        uint32_t    aes_aad_len           : 15;      /*!< Length of additional authentication data in CCM/GCM mode,
                                                          in bytes.
                                                          (In CCM mode, the software needs to manually append the
                                                          2-byte AAD length before the AAD data.) */
        uint32_t    reserved_0            : 17;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_aad_len_t;

/**
 * @brief  This union represents the bit fields in the aes auth result register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_auth_result {
    struct {
        uint32_t    aes_auth_success      : 1;       /*!< Authentication success flag during decryption in
                                                          CCM/GCM mode */
        uint32_t    aes_auth_error        : 1;       /*!< Authentication failure flag during decryption in
                                                          CCM/GCM mode */
        uint32_t    reserved_0            : 30;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_auth_result_t;

/**
 * @brief  This union represents the bit fields in the aes fsm status register.
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union aes_fsm_st {
    struct {
        uint32_t    aes_crypto_st         : 3;       /*!< ECB/CBC/OFB/CTR mode operation state machine */
        uint32_t    aes_ccm_st            : 4;       /*!< CCM mode operation state machine */
        uint32_t    aes_gcm_st            : 4;       /*!< GCM mode operation state machine */
        uint32_t    reserved_0            : 21;      /*!< reserved */
    } b;
    uint32_t    d32;                                 /*!< Raw register data. */
} aes_fsm_st_t;

/**
 * @brief  Registers associated with aes regs.
 */
typedef struct aes_regs {
    volatile aes_int_en_t           aes_int_en;                /*!< Enable aes interrupt */
    volatile aes_int_clr_t          aes_int_clr;               /*!< Clear aes interrupt */
    volatile aes_int_sts_t          aes_int_sts;               /*!< Indicates the aes interrupt status. */
    volatile uint32_t               reserved[25];              /*!< Reserved. */
    volatile aes_ctrl_t             aes_ctrl;                  /*!< AES module control */
    volatile aes_cfg_t              aes_cfg;                   /*!< AES parameter configuration */
    volatile uint32_t               aes_data_len;              /*!< Length of operation data */
    volatile aes_aad_len_t          aes_aad_len;               /*!< AAD length */
    volatile uint32_t               aes_key_start_addr;        /*!< Key Start Address */
    volatile uint32_t               aes_iv_start_addr;         /*!< IV start address */
    volatile uint32_t               aes_auth_iv_start_addr;    /*!< Auth IV start address */
    volatile uint32_t               aes_aad_start_addr;        /*!< Start address of aad data */
    volatile uint32_t               aes_data_in_start_addr;    /*!< Start address of input data */
    volatile uint32_t               aes_data_out_start_addr;   /*!< Start address of output data */
    volatile uint32_t               aes_tag_start_addr;        /*!< Start address of aes tag */
    volatile aes_auth_result_t      aes_auth_result;           /*!< Certification Result */
    volatile aes_fsm_st_t           aes_fsm_st;                /*!< AES operational state machine */
} aes_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif