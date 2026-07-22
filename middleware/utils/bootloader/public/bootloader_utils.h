/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:   BT BOOTLOADER UTILS HEADER
 * Author:
 * Create: 2018-10-15
 */

#ifndef SRC_APPLICATION_BOOTLOADER_UTILS_H
#define SRC_APPLICATION_BOOTLOADER_UTILS_H

#include "uart.h"
#include "platform_types.h"
/**
 * @brief  Print a boot status message
 * @param  uart_bus uart to use, or UART_BUS_NONE to print nothing
 */
void bootloader_utils_print_boot_message(uart_bus_t uart_bus);

/**
 * @brief  Initialises the drivers and modules used in the bootloader
 * @note   The uart is initialised and deinitialised inside codeloader if needed.
 */
void bootloader_utils_init_modules(void);

/**
 * @brief  Checks if a core has code to run
 * @param  cimage core image requested
 * @return true if code has been detected
 * @note   it uses the flash configuration page to detect the begginning of code.
 */
bool bootloader_utils_core_has_code(core_images_e cimage);

/**
 * @brief  Set the cpu in a known state:
 * CPU running in program context.
 * Clocks configured.
 * Interrupts:
 *   Global interrupts enabled
 *   All maskable disabled
 *   Hardfault enabled
 *   Systick disabled
 */
void bootloader_utils_cpu_init(void);

/**
 * @brief  Returns the ROM version
 * @return ROM version
 */
uint32_t bootloader_utils_get_rom_version(void);

/**
 * @brief  Returns the SSB version
 * @return SSB version
 */
#if defined(BUILD_APPLICATION_SSB)
uint32_t bootloader_utils_get_ssb_version(void);
#endif

/**
 * @brief  Read and check SSB length is valid
 * @param  length Returns the length of the stored SSB length (or 0)
 * @return 0 if invalid, true if valid
 */
bool bootloader_utils_ssb_get_length(uint32_t *length);

/**
 * @brief  Gets the SHA stored in OTP for the SSB
 * @param  sha If return is true, Returns the sha of the SSB
 * @return True if the sha is present and valid
 */
bool bootloader_utils_ssb_get_sha(uint8_t *sha, const uint8_t max_sha_len);

/**
 * @brief  Checks if the SSB SHA256 in OTP is valid and that the SSB image matches
 * @return true if the SSB SHA256 in OTP is valid and that the SSB image matches
 */
bool bootloader_utils_ssb_sha256_validate(void);

/**
 * @brief  Return the flash version for the core image security A
 * @return flash version for the core image security A
 */
uint32_t bootloader_utils_get_flash_version(void);

/**
 * @brief  Returns true if the current version of the code for the security core is for flash.
 */
bool bootloader_utils_is_code_in_flash(void);

#endif
