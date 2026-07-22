/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:BT BOOTLOADER UTILS
 * Author:
 * Create: 2018-10-15
 */

#include "sha256.h"
#ifdef SECURITY_ROM
#include "memory_config.h"
#else
#include "flash.h"
#include "application_version.h"
#endif
#include "memory_core.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "build_version.h"
#include "securec.h"
#include "otp_map.h"
#include "panic.h"
#include "uart.h"
#include "arch_barrier.h"
#include "bootloader_utils.h"

#if (!(defined BUILD_APPLICATION_ROM))
#define SECURE_BOOT_CORE_STR_SECURE   "Boot: Signed\r\n"
#define SECURE_BOOT_CORE_STR_INSECURE "Boot: Unsigned\r\n"
#define CRLF_STR                      "\r\n"
#endif

#define SSB_RAM_START   APP_ITCM_ORIGIN

#define CRC_MASK 0xff000000
#define CRC_RIGHT_SHIFT_BIT 24
#define SSB_LENGTH_MASK 0xffffff

#ifndef SHA256_SIGNATURE_LEN
#define SHA256_SIGNATURE_LEN 32
#endif

#ifndef SHA_BLOCK_SIZE
#define SHA_BLOCK_SIZE 256
#endif
#define SSB_IAMGE_MIN_LEN  FLASH_PAGE_SIZE

/** Vector active mask for the ICSR register */
#if CHIP_SOCMN1 || CHIP_BS25 || CHIP_WS63 || CHIP_BS21 || CHIP_WS53
#define ICSR_VECTACTIVE_MASK 0x62
#elif (CHIP_BRANDY == 1)
#define ICSR_VECTACTIVE_MASK 0x21
#else
#error "ICSR_VECTACTIVE_MASK not defined"
#endif

/** Vector key for the AIRCR register */
#define AIRCR_VECTKEY 0x5FA

#define SSB_SHA256_CRC_START_BYTE_LEN   1

/* Sets the cpu in a known state before continuing */
void bootloader_utils_cpu_init(void)
{
#if (ARCH == CM3) || (ARCH == CM7)
    // Disable SysTick interrupt
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    // Reset if it is in interrupt context
    if ((SCB->ICSR & ICSR_VECTACTIVE_MASK) != 0) {
        dsb();
        SCB->AIRCR = ((AIRCR_VECTKEY << SCB_AIRCR_VECTKEY_Pos) |
                      SCB_AIRCR_SYSRESETREQ_Msk);
        dsb();
    }

    // Disable the maskable interrupts and enable global interrupts
    NVIC->ICER[0] = 0xffffffff;

    /* enable global interrupts */
    __set_PRIMASK(0);
#endif
}

void bootloader_utils_print_boot_message(uart_bus_t uart_bus)
{
#if (!(defined BUILD_APPLICATION_ROM) && !(defined UNIT_TEST))
    if (uart_bus != UART_BUS_NONE) {
        uapi_uart_write(uart_bus, (uint8_t *)CRLF_STR, (uint16_t)strlen(CRLF_STR), 0);
        uapi_uart_write(uart_bus, (uint8_t *)SECURE_BOOT_CORE_STR_INSECURE,
                        (uint16_t)strlen(SECURE_BOOT_CORE_STR_INSECURE), 0);
    }
#else
    UNUSED(uart_bus);
#endif
}

void bootloader_utils_init_modules(void)
{
}

bool bootloader_utils_core_has_code(core_images_e cimage)
{
    memory_map *mem_map;
    mem_map = memory_get_cached_core_map(cimage);
    if (mem_map == NULL) {
        panic(PANIC_CODELOADER, __LINE__);
        return false;  //lint !e527  unreachable code
    }
    return *((uint32_t *)(uintptr_t)(mem_map->code_addr)) != 0xFFFFFFFF;
}

uint32_t bootloader_utils_get_rom_version(void)
{
    build_version_info_rom *version_info;
    uint32_t version_location = ROM_START + VERSION_INFORMATION_OFFSET;
    version_info = (build_version_info_rom *)((uintptr_t)version_location);
    return version_info->version;
}

#if defined(BUILD_APPLICATION_SSB)
uint32_t bootloader_utils_get_ssb_version(void)
{
    build_version_info *version_info = NULL;
    uint32_t version_location = APP_ITCM_ORIGIN + VERSION_INFORMATION_OFFSET;
    version_info = (build_version_info *)((uintptr_t)version_location);
    return version_info->version;
}
#endif

uint32_t bootloader_utils_get_flash_version(void)
{
    build_version_info *version_info = NULL;
    uint32_t version_location;
    memory_map *map = memory_get_cached_core_map(CORE_IMAGES_BT);
    if (map == NULL) {
        return 0;
    }
    version_location = map->code_addr + VERSION_INFORMATION_OFFSET;
    version_info = (build_version_info *)((uintptr_t)version_location);
    return version_info->version;
}

bool bootloader_utils_is_code_in_flash(void)
{
#ifndef SECURITY_ROM
    build_version_info *version_info = (build_version_info *)VERSION_INFORMATION_OFFSET;
    return ((version_info->version & APPLICATION_CORE_FLASH_BUILD) == APPLICATION_CORE_FLASH_BUILD);
#else
    return true;
#endif
}

bool bootloader_utils_ssb_get_length(uint32_t *length)
{
    uint8_t ssb_length_crc;
    uint8_t calc_crc = 0;
    bool valid_ssb_length = false;
    uint32_t ssb_length = 0;
    *length = 0;
#if CORE == MASTER_BY_ALL
    uint32_t ssb_len_otp_addr = OTP_SSB_CODE_SIZE_START;
#elif CORE == SECURITY
    uint32_t ssb_len_otp_addr = OTP_SEC_SSB_CODE_SIZE_START;
#endif

    // Check that the SSB length CRC matches SSB length
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer((uint8_t *)&ssb_length, ssb_len_otp_addr, sizeof(uint32_t)) == ERRCODE_SUCC) {
        ssb_length_crc = (ssb_length & CRC_MASK) >> CRC_RIGHT_SHIFT_BIT;  // mask off CRC byte, top 8 bits, and shift
        ssb_length &= SSB_LENGTH_MASK;       // mask out CRC byte
        if (uapi_efuse_calc_crc((uint8_t *)&ssb_length, OTP_SSB_CODE_SIZE_LENGTH, &calc_crc) == ERRCODE_SUCC) {
            if ((uint8_t)ssb_length_crc == calc_crc) {
                valid_ssb_length = true;
                *length = ssb_length;
            }
        }
    }
#else
    if (otp_read_buffer((uint8_t *)&ssb_length, ssb_len_otp_addr, sizeof(uint32_t)) == OTP_RET_SUCC) {
        ssb_length_crc = (ssb_length & CRC_MASK) >> CRC_RIGHT_SHIFT_BIT;  // mask off CRC byte, top 8 bits, and shift
        ssb_length &= SSB_LENGTH_MASK;       // mask out CRC byte
        if (otp_calc_crc((uint8_t *)&ssb_length, OTP_SSB_CODE_SIZE_LENGTH, &calc_crc) == OTP_RET_SUCC) {
            if ((uint8_t)ssb_length_crc == calc_crc) {
                valid_ssb_length = true;
                *length = ssb_length;
            }
        }
    }
#endif
    return valid_ssb_length;
}

bool bootloader_utils_ssb_get_sha(uint8_t *sha, const uint8_t max_sha_len)
{
    bool valid_sha = false;
    uint8_t sha_crc = 0;
    uint8_t calc_crc = 0;
    UNUSED(max_sha_len);
#if CORE == MASTER_BY_ALL
    uint32_t ssb_sha256_start = OTP_SSB_SHA256_START;
    uint32_t ssb_sha256_crc_start = OTP_SSB_SHA256_CRC;
#elif CORE == SECURITY
    uint32_t ssb_sha256_start = OTP_SEC_SSB_SHA256_START;
    uint32_t ssb_sha256_crc_start = OTP_SEC_SSB_SHA256_CRC;
#endif

    // Get SHA & SHA CRC and compare
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer(sha, ssb_sha256_start, OTP_SSB_SHA256_LENGTH) == ERRCODE_SUCC) {
        if (uapi_efuse_read_buffer(&sha_crc, ssb_sha256_crc_start, SSB_SHA256_CRC_START_BYTE_LEN) == ERRCODE_SUCC) {
            if (uapi_efuse_calc_crc(sha, OTP_SSB_SHA256_LENGTH, &calc_crc) != ERRCODE_SUCC) {
                return valid_sha;
            }
            if (calc_crc == sha_crc) {
                valid_sha = true;
            }
        }
    }
#else
    if (otp_read_buffer(sha, ssb_sha256_start, OTP_SSB_SHA256_LENGTH) == OTP_RET_SUCC) {
        if (otp_read_byte(&sha_crc, ssb_sha256_crc_start) == OTP_RET_SUCC) {
            if (otp_calc_crc(sha, OTP_SSB_SHA256_LENGTH, &calc_crc) != OTP_RET_SUCC) {
                return valid_sha;
            }
            if (calc_crc == sha_crc) {
                valid_sha = true;
            }
        }
    }
#endif
    return valid_sha;
}

static bool iflash_core_check_sha256_region(const uint8_t *stored_sha256, uint32_t buffer_len,
                                            uint32_t start, uint32_t length)
{
    errcode_t ret;
    bool valid_sha = false;
    uint8_t result[SHA256_HASH_SIZE] = { 0 };

    ret = uapi_drv_cipher_sha256((uint8_t*)(uintptr_t)start, length, result, sizeof(result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    if (memcmp(result, stored_sha256, SHA256_HASH_SIZE) == 0) {
        valid_sha = true;
    }
    UNUSED(buffer_len);
    return valid_sha;
}

/*
 * 1. Check that the SSB length CRC matches SSB length
 * 2. Check that the SHA CRC matches the SHA. Not set will always fail.
 * 3. Then check that the SHA matches the image.
 */
bool bootloader_utils_ssb_sha256_validate(void)
{
    uint32_t ssb_length;
    uint8_t stored_sha256[OTP_SSB_SHA256_LENGTH];
    bool ssb_valid = false;
    bool ssb_valid_length;

    ssb_valid_length = bootloader_utils_ssb_get_length(&ssb_length);
    if (!ssb_valid_length) { return false; }
#ifndef SECURITY_ROM
    if (ssb_length > SSB_FLASH_REGION_LENGTH) { return false; }
#else
    if (ssb_length > SEC_SSB_FLASH_REGION_LENGTH) { return false; }
#endif
    if (ssb_length < SSB_IAMGE_MIN_LEN) { return false; }

    // get sha and if valid, check image
    if (bootloader_utils_ssb_get_sha(stored_sha256, sizeof(stored_sha256))) {
        // check ram code
#ifndef SECURITY_ROM
        if (iflash_core_check_sha256_region(stored_sha256, sizeof(stored_sha256), SSB_RAM_START, ssb_length)) {
            // the ssb image content of the max length was read before, so here we need to clear unsafe redundant info
            memset_s((uint8_t *)(uintptr_t)(SSB_RAM_START + ssb_length),
                     (uint32_t)(SSB_FLASH_REGION_LENGTH - ssb_length),
                     0,
                     (uint32_t)(SSB_FLASH_REGION_LENGTH - ssb_length));
            ssb_valid = true;
        }
#else
        errcode_t ret;
        uint8_t sha256[SHA256_HASH_SIZE] = { 0 };

        ret = uapi_drv_cipher_sha256((uint8_t*)(uintptr_t)SEC_RAM_ORIGIN, ssb_length, sha256, sizeof(sha256));
        if (ret != ERRCODE_SUCC) {
            return false;
        }

        if (memcmp(sha256, stored_sha256, SHA256_HASH_SIZE) == 0) {
            PRINT("SSB sha256 check success!" NEWLINE);
            // the ssb image content of the max length was read before, so here we need to clear unsafe redundant info
            memset_s((uint8_t *)(uintptr_t)(SEC_RAM_ORIGIN + ssb_length),
                     (uint32_t)(SEC_SSB_FLASH_REGION_LENGTH - ssb_length),
                     0,
                     (uint32_t)(SEC_SSB_FLASH_REGION_LENGTH - ssb_length));
            ssb_valid = true;
        }
#endif
    }
    return ssb_valid;
}
