/*
 * Copyright (c) CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:   BT BOOTLOADER CONFIGURATION MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "bootloader_configuration.h"
#include "bootloader_utils.h"
#include "otp_map.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "non_os.h"
#include "platform_types.h"
/** mask for bit x */
/** Read bit y in x */
#define read_bit(x, y) (((x) >> (y)) & 1U)

#define ROM_VERSION_FORCE_MINIMAL_ROM 0x12
#define CRC_MASK 0xff000000
#define CRC_RIGHT_SHIFT_BIT 24
#ifdef EFUSE_INTERFACE_REPLACE
#define OTP_BOOTLOADER_BYTE_LEN         1
#define OTP_BOOTLOADER_FLASH_BYTE_LEN   1
#define OTP_BOOTLOADER_DOUBLE_BYTE_LEN  1
#endif
/**
 * Default value for g_bootloader_configuration_flags before reading from the otp
 */
static uint8_t g_bootloader_configuration_flags = 0;
static uint8_t g_bootloader_configuration_flags_1 = 0;

#if EMBED_FLASH_EXIST == YES
static uint8_t g_bootloader_configuration_flags_2 = 0;
#endif

/**
 * Perform ROM version specific configuration
 */
void bootloader_rom_specific_config(void)
{
    static uint8_t cf;
    if (bootloader_utils_get_rom_version() == ROM_VERSION_FORCE_MINIMAL_ROM) {
#ifdef EFUSE_INTERFACE_REPLACE
        errcode_t efuse_ret;
        efuse_ret = uapi_efuse_read_buffer(&cf, OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_BYTE_LEN);
        // Check the OTP data is valid and if not write a default value.
        if ((efuse_ret == ERRCODE_SUCC)
            && (read_bit(cf, OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT) == 0)) {
            uint8_t value = (1 << OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT);
            efuse_ret = uapi_efuse_write_buffer(OTP_BOOTLOADER_BYTE, &value, OTP_BOOTLOADER_BYTE_LEN);
            UNUSED(efuse_ret);
        }
#else
        otp_ret_t otp_ret;
        otp_ret = otp_read_byte(&cf, OTP_BOOTLOADER_BYTE);
        // Check the OTP data is valid and if not write a default value.
        if ((otp_ret == OTP_RET_SUCC) && (read_bit(cf, OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT) == 0)) {
            otp_ret = otp_write_byte(OTP_BOOTLOADER_BYTE, (1 << OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT));
            UNUSED(otp_ret);
        }
#endif
    }
}

void bootloader_configuration_init(void)
{
    // Read OTP flags
    // Use OTP full access API as iflash config has not yet been initialised so OTP regions have not been declared
#ifdef EFUSE_INTERFACE_REPLACE
    (void)uapi_efuse_read_buffer(&g_bootloader_configuration_flags, OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_BYTE_LEN);
#if (CHIP_LIBRA || CHIP_SOCMN1 || CHIP_BS25 || CHIP_BRANDY)
    (void)uapi_efuse_read_buffer(&g_bootloader_configuration_flags_1, OTP_BOOTLOADER_BYTE_1, OTP_BOOTLOADER_BYTE_LEN);
#endif

#if EMBED_FLASH_EXIST == YES
    (void)uapi_efuse_read_buffer(&g_bootloader_configuration_flags_2,
        OTP_BOOTLOADER_FLASH_BYTE, OTP_BOOTLOADER_FLASH_BYTE_LEN);
#endif

    uint8_t config = 0;
    (void)uapi_efuse_read_buffer(&config, OTP_BOOTLOADER_DOUBLE_BYTE, OTP_BOOTLOADER_DOUBLE_BYTE_LEN);
    g_bootloader_configuration_flags &= config;

#if EMBED_FLASH_EXIST == YES
    uint8_t config_1 = 0;
    (void)uapi_efuse_read_buffer(&config_1, OTP_BOOTLOADER_FLASH_DOUBLE_BYTE, OTP_BOOTLOADER_DOUBLE_BYTE_LEN);
    g_bootloader_configuration_flags_2 &= config_1;
#endif
#else
    (void)otp_read_byte(&g_bootloader_configuration_flags, OTP_BOOTLOADER_BYTE);
#if (CHIP_LIBRA || CHIP_SOCMN1 || CHIP_BS25 || CHIP_BRANDY)
    (void)otp_read_byte(&g_bootloader_configuration_flags_1, OTP_BOOTLOADER_BYTE_1);
#endif

#if EMBED_FLASH_EXIST == YES
    (void)otp_read_byte(&g_bootloader_configuration_flags_2, OTP_BOOTLOADER_FLASH_BYTE);
#endif

    uint8_t config = 0;
    (void)otp_read_byte(&config, OTP_BOOTLOADER_DOUBLE_BYTE);
    g_bootloader_configuration_flags &= config;

#if EMBED_FLASH_EXIST == YES
    uint8_t config_1 = 0;
    (void)otp_read_byte(&config_1, OTP_BOOTLOADER_FLASH_DOUBLE_BYTE);
    g_bootloader_configuration_flags_2 &= config_1;
#endif
#endif
}

bool bootloader_configuration_execute_minimal_rom(void)
{
    // check the flag
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT) == 1);
}

bool bootloader_configuration_disable_codeloader_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_DIS_CODELOADER_BIT) == 1);
}

bool bootloader_configuration_disable_ate_mode(void)
{
    // check the flag
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_DIS_ATE_MODE_BIT) == 1);
}

bool bootloader_configuration_en_ssb_hash_in_rom(void)
{
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_EN_SSB_HASH_BIT) == 1);
}

#if EMBED_FLASH_EXIST == YES
bool bootloader_configuration_disable_eflash_mode(void)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        uapi_efuse_init();
    }
    if (uapi_efuse_write_bit(OTP_BOOTLOADER_FLASH_BYTE, OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        otp_init();
    }
    if (otp_write_bit(OTP_BOOTLOADER_FLASH_BYTE, OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT) != OTP_RET_SUCC) {
        return false;
    }
#endif
    return true;
}

bool bootloader_configuration_is_enable_eflash_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags_2, OTP_BOOTLOADER_FSB_EN_EFLASH_BIT) == 1);
}

bool bootloader_configuration_is_disable_eflash_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags_2, OTP_BOOTLOADER_FSB_DIS_EFLASH_BIT) == 1);
}

flash_mode_t bootloader_configuration_get_flash_mode(void)
{
    if (bootloader_configuration_is_enable_eflash_mode() &&
       (!bootloader_configuration_is_disable_eflash_mode())) {
        return EXTERNAL_FLASH_SPI;
    } else {
        return EMBED_FLASH_XIP;
    }
}
#endif

bool bootloader_configuration_ssb_sha256_set(void)
{
    uint32_t crc = 0;
    bool ssb_sha256_set = true;
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t efuse_ret;
#if CORE == MASTER_BY_ALL
    efuse_ret = uapi_efuse_read_buffer((uint8_t *)&crc, OTP_SSB_CODE_SIZE_START, sizeof(uint32_t));
#elif CORE == SECURITY
    efuse_ret = uapi_efuse_read_buffer((uint8_t *)&crc, OTP_SEC_SSB_CODE_SIZE_START, sizeof(uint32_t));
#endif
    crc = (crc & CRC_MASK) >> CRC_RIGHT_SHIFT_BIT;

    // if there is an error, default to set so we default to secure operation
    if (efuse_ret == ERRCODE_SUCC && crc == 0) {
        ssb_sha256_set = false;
    }
#else
    otp_ret_t otp_ret;
#if CORE == MASTER_BY_ALL
    otp_ret = otp_read_buffer((uint8_t *)&crc, OTP_SSB_CODE_SIZE_START, sizeof(uint32_t));
#elif CORE == SECURITY
    otp_ret = otp_read_buffer((uint8_t *)&crc, OTP_SEC_SSB_CODE_SIZE_START, sizeof(uint32_t));
#endif
    crc = (crc & CRC_MASK) >> CRC_RIGHT_SHIFT_BIT;

    // if there is an error, default to set so we default to secure operation
    if (otp_ret == OTP_RET_SUCC && crc == 0) {
        ssb_sha256_set = false;
    }
#endif
    return ssb_sha256_set;
}

bool bootloader_configuration_enable_single_rx_mode(void)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        uapi_efuse_init();
    }
    if (uapi_efuse_write_bit(OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_FSB_EN_UART_RX_BIT) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        otp_init();
    }
    if (otp_write_bit(OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_FSB_EN_UART_RX_BIT) != OTP_RET_SUCC) {
        return false;
    }
#endif
    return true;
}

bool bootloader_configuration_disable_single_rx_mode(void)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        uapi_efuse_init();
    }
    if (uapi_efuse_write_bit(OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        otp_init();
    }
    if (otp_write_bit(OTP_BOOTLOADER_BYTE, OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT) != OTP_RET_SUCC) {
        return false;
    }
#endif
    return true;
}

bool bootloader_configuration_is_enable_single_rx_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_EN_UART_RX_BIT) == 1);
}

bool bootloader_configuration_is_disable_single_rx_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags, OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT) == 1);
}

bool bootloader_configuration_enable_wait_mode(void)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        uapi_efuse_init();
    }
    if (uapi_efuse_write_bit(OTP_BOOTLOADER_BYTE_1, OTP_BOOTLOADER_FSB_EN_WAIT_BIT) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        otp_init();
    }
    if (otp_write_bit(OTP_BOOTLOADER_BYTE_1, OTP_BOOTLOADER_FSB_EN_WAIT_BIT) != OTP_RET_SUCC) {
        return false;
    }
#endif
    return true;
}

bool bootloader_configuration_disable_wait_mode(void)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        uapi_efuse_init();
    }
    if (uapi_efuse_write_bit(OTP_BOOTLOADER_BYTE_1, OTP_BOOTLOADER_FSB_DIS_WAIT_BIT) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == false) {
        otp_init();
    }
    if (otp_write_bit(OTP_BOOTLOADER_BYTE_1, OTP_BOOTLOADER_FSB_DIS_WAIT_BIT) != OTP_RET_SUCC) {
        return false;
    }

#endif
    return true;
}

bool bootloader_configuration_is_enable_wait_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags_1, OTP_BOOTLOADER_FSB_EN_WAIT_BIT) == 1);
}

bool bootloader_configuration_is_disable_wait_mode(void)
{
    return (read_bit(g_bootloader_configuration_flags_1, OTP_BOOTLOADER_FSB_DIS_WAIT_BIT) == 1);
}
