/*
 * Copyright (c) CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  BT BOOTLOADER CONFIGURATION MODULE API
 * Author:
 * Create: 2018-10-15
 */

#ifndef SRC_APPLICATION_BOOTLOADER_CONFIGURATION_H
#define SRC_APPLICATION_BOOTLOADER_CONFIGURATION_H

#include "std_def.h"

typedef enum {
    EMBED_FLASH_XIP = 0,
    SFC_FLASH_QSPI,
    EXTERNAL_FLASH_SPI
} flash_mode_t;

/**
 * @brief  Perform ROM specific configuration
 */
void bootloader_rom_specific_config(void);

/**
 * @brief  Inits the bootloader configuration module
 */
void bootloader_configuration_init(void);

/**
 * @brief  Checks if the minimal ROM must be executed
 * @return true if the minimal ROM is enabled
 */
bool bootloader_configuration_execute_minimal_rom(void);

/**
 * @brief  Checks if codeloader is disable
 * @return true if codeloader is disable
 */
bool bootloader_configuration_disable_codeloader_mode(void);

/**
 * @brief  Checks if disable ate mode
 * @return true if disable ate mode
 */
bool bootloader_configuration_disable_ate_mode(void);

/**
 * @brief  Checks if the ps ram mode bit has been set in OTP
 * @return true if ps ram mode bit has been set in OTP
 */
bool bootloader_configuration_non_eflash_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_EN_SSB_HASH_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_EN_SSB_HASH_BIT has been set in OTP
 */
bool bootloader_configuration_en_ssb_hash_in_rom(void);

#if EMBED_FLASH_EXIST == YES
/**
 * @brief  Enable eflash mode in rom
 * @return true if OTP_BOOTLOADER_FSB_EN_EFLASH_BIT has been set in OTP
 */
bool bootloader_configuration_enable_eflash_mode(void);

/**
 * @brief  Disable eflash mode in rom
 * @return true if OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT has been set in OTP
 */
bool bootloader_configuration_disable_eflash_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_EN_EFLASH_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_EN_EFLASH_BIT has been set in OTP
 */
bool bootloader_configuration_is_enable_eflash_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT has been set in OTP
 */
bool bootloader_configuration_is_disable_eflash_mode(void);

/**
 * @brief  Checks flash mode has been set in OTP
 * @return three mode
 */
flash_mode_t bootloader_configuration_get_flash_mode(void);
#endif

/**
 * @brief  Checks if the SSB SHA256 has been set in OTP
 * @return true if SSB SHA256 has been set in OTP
 */
bool bootloader_configuration_ssb_sha256_set(void);

/**
 * @brief  Enable single rx receivece mode in rom
 * @return true if OTP_BOOTLOADER_FSB_EN_UART_RX_BIT has been set in OTP
 */
bool bootloader_configuration_enable_single_rx_mode(void);

/**
 * @brief  Disable single rx receivece mode in rom
 * @return true if OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT has been set in OTP
 */
bool bootloader_configuration_disable_single_rx_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_EN_UART_RX_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_EN_UART_RX_BIT has been set in OTP
 */
bool bootloader_configuration_is_enable_single_rx_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT has been set in OTP
 */
bool bootloader_configuration_is_disable_single_rx_mode(void);

/**
 * @brief  Enable wait mode in rom
 * @return true if OTP_BOOTLOADER_FSB_EN_WAIT_BIT has been set in OTP
 */
bool bootloader_configuration_enable_wait_mode(void);

/**
 * @brief  Disable wait mode in rom
 * @return true if OTP_BOOTLOADER_FSB_DIS_WAIT_BIT has been set in OTP
 */
bool bootloader_configuration_disable_wait_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_EN_WAIT_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_EN_WAIT_BIT has been set in OTP
 */
bool bootloader_configuration_is_enable_wait_mode(void);

/**
 * @brief  Checks if the OTP_BOOTLOADER_FSB_DIS_WAIT_BIT has been set in OTP
 * @return true if OTP_BOOTLOADER_FSB_DIS_WAIT_BIT has been set in OTP
 */
bool bootloader_configuration_is_disable_wait_mode(void);
#endif
