/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  SSB SEC BOOT FOTA
 * Author:
 * Create: 2020-06-02
 */
#include "sec_boot_fota.h"
#include "sec_boot_public.h"
#include "sec_boot_image_sign_validate.h"
#include "sha256.h"
#include "securec.h"
#include "oal_interface.h"
#include "debug_print.h"
#include "flash.h"
#include "watchdog.h"
#if (CORE != WIFI) && !defined(BUILD_APPLICATION_ROM)
#include "irmalloc.h"
#endif
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif

#define COPY_LEN          1024

typedef struct {
    uint32_t image_fota_addr;
    uint32_t image_boot_addr;
} sec_boot_fota_addr_t;

static sec_boot_fota_addr_t g_fota_image_sign_addr[PAL_FOTA_BIN_MAX_INDEX] = {
    { PAL_FOTA_SSB_SIGN_START, PAL_MCU_SSB_IMAGE_SIGN_START },
    { PAL_FOTA_RECOVERY_SIGN_START, PAL_MCU_RECOVERY_IMAGE_SIGN_START },
    { PAL_FOTA_BT_SIGN_START, PAL_MCU_BT_IMAGE_SIGN_START },
#if DSP_EXIST == YES
    { PAL_FOTA_DSP_SIGN_START, PAL_MCU_DSP_IMAGE_SIGN_START },
#endif
#if DUAL_DSP_EXIST == YES
    { PAL_FOTA_DSP1_SIGN_START, PAL_MCU_DSP1_IMAGE_SIGN_START },
#endif
    { PAL_FOTA_APP_SIGN_START, PAL_MCU_APP_IMAGE_SIGN_START },
};

uint32_t sec_boot_fota_image_sign_addr_get(pal_fota_bin_index_t fota_bin_index)
{
    if (fota_bin_index > APP_INDEX) {
        return 0;
    }
    return g_fota_image_sign_addr[fota_bin_index].image_fota_addr;
}

static uint32_t sec_boot_image_sign_addr_get(pal_fota_bin_index_t fota_bin_index)
{
    if (fota_bin_index > APP_INDEX) {
        return 0;
    }
    return g_fota_image_sign_addr[fota_bin_index].image_boot_addr;
}

static bool sec_boot_flash_memcpy_and_verify(uint32_t src_addr, uint32_t dest_addr, uint32_t length)
{
    uint8_t *copy_data = irmalloc(COPY_LEN);
    if (copy_data == NULL) {
        PRINT("Failed to apply for memory"NEWLINE);
        return false;
    }

    // Copy image.
#ifndef SUPPORT_TWO_FLASH
#ifndef USE_EMBED_FLASH
    uint32_t copy_src_addr = src_addr;
#else
    uint32_t copy_src_addr = src_addr - FLASH_START + EMBED_FLASH_START;
#endif
#else
    uint32_t copy_src_addr = src_addr - FLASH_START + DATA_FLASH_START;
#endif
    uint32_t copy_des_addr = dest_addr - FLASH_START;
    uint32_t current_copy_length = 0;
    while (current_copy_length < length) {
        uint32_t copy_length = MIN(COPY_LEN, (length - current_copy_length));
        if (memcpy_s((uint8_t *)copy_data, copy_length, (uint8_t *)((uintptr_t)copy_src_addr), copy_length) != EOK) {
            irfree(copy_data);
            return false;
        }
#ifndef USE_EMBED_FLASH
        uapi_flash_write_data(CHIP_FLASH_ID, copy_des_addr, copy_data, copy_length);
#else
        uapi_eflash_write(copy_des_addr, (uint32_t *)copy_data, copy_length);
#endif
        copy_des_addr += COPY_LEN;
        copy_src_addr += COPY_LEN;
        current_copy_length += copy_length;
        uapi_watchdog_kick();
    }
    irfree(copy_data);

    errcode_t ret;
    uint8_t sha256_src[SHA256_HASH_SIZE] = { 0 };
    uint8_t sha256_dest[SHA256_HASH_SIZE] = { 0 };
    uint8_t *src_buf = NULL;
    uint8_t *dst_buf = NULL;
#ifndef SUPPORT_TWO_FLASH
#ifndef USE_EMBED_FLASH
    src_buf = (uint8_t *)(uintptr_t)(src_addr);
    dst_buf = (uint8_t *)(uintptr_t)(dest_addr);
#else
    src_buf = (uint8_t *)(uintptr_t)(src_addr - FLASH_START + EMBED_FLASH_START);
    dst_buf = (uint8_t *)(uintptr_t)(dest_addr - FLASH_START + EMBED_FLASH_START);
#endif
#else
    src_buf = (uint8_t *)(uintptr_t)(src_addr - FLASH_START + DATA_FLASH_START);
    dst_buf = (uint8_t *)(uintptr_t)(dest_addr);
#endif

    /* calculate sha256 and compare. */
    ret = uapi_drv_cipher_sha256(src_buf, length, sha256_src, sizeof(sha256_src));
    if (ret != ERRCODE_SUCC) {
        return false;
    }
#ifdef SUPPORT_TWO_FLASH
    uapi_flash_exit_from_xip_mode(CHIP_DATA_FLASH_ID);
    uapi_flash_switch_to_xip_mode(CHIP_FLASH_ID);
#endif
    ret = uapi_drv_cipher_sha256(dst_buf, length, sha256_dest, sizeof(sha256_dest));
    if (ret != ERRCODE_SUCC) {
        return false;
    }
#ifdef SUPPORT_TWO_FLASH
    uapi_flash_exit_from_xip_mode(CHIP_FLASH_ID);
    uapi_flash_switch_to_xip_mode(CHIP_DATA_FLASH_ID);
#endif
    if (memcmp(sha256_src, sha256_dest, SHA256_HASH_SIZE) != 0) {
        PRINT("sha256 compare fail!"NEWLINE);
        return false;
    }
    return true;
}

static bool sec_boot_ssb_image_backup(void)
{
    // Erase ssb back up region.
#ifndef USE_EMBED_FLASH
    if (uapi_flash_block_erase(CHIP_FLASH_ID, PAL_SSB_BACKUP_FLASH_REGION_START - PAL_FLASH_START,
                               PAL_SSB_BACKUP_FLASH_REGION_LENGTH, true) != ERRCODE_SUCC) {
#else
    if (uapi_eflash_erase(PAL_SSB_BACKUP_FLASH_REGION_START - PAL_FLASH_START,
                          PAL_SSB_BACKUP_FLASH_REGION_LENGTH) != ERRCODE_SUCC) {
#endif
        PRINT("erase ssb backup region error"NEWLINE);
        return false;
    }

    bool ret = sec_boot_flash_memcpy_and_verify(PAL_SSB_FLASH_REGION_START, PAL_SSB_BACKUP_FLASH_REGION_START,
                                                PAL_SSB_BACKUP_FLASH_REGION_LENGTH);
    return ret;
}

static bool sec_boot_ssb_sign_and_pubkey_backup(void)
{
    // Erase back up region.
#ifndef USE_EMBED_FLASH
    if (uapi_flash_block_erase(CHIP_FLASH_ID, PAL_MCU_SEC_BOOT_SIGN_BACKUP_START - FLASH_START,
                               PAL_MCU_SEC_BOOT_SIGN_BACKUP_LEN, true) != ERRCODE_SUCC) {
#else
    if (uapi_eflash_erase(PAL_MCU_SEC_BOOT_SIGN_BACKUP_START - FLASH_START,
                          PAL_MCU_SEC_BOOT_SIGN_BACKUP_LEN) != ERRCODE_SUCC) {
#endif
        PRINT("erase ssb backup region error"NEWLINE);
        return false;
    }

    bool ret = sec_boot_flash_memcpy_and_verify(PAL_MCU_SUB_PUBKEY_START, PAL_MCU_SUB_PUBKEY_BACKUP_START,
                                                PAL_FLASH_PAGE_SIZE);
    if (!ret) {
        PRINT("pubkey cert backup error"NEWLINE);
        return ret;
    }

    ret = sec_boot_flash_memcpy_and_verify(PAL_MCU_SSB_IMAGE_SIGN_START, PAL_MCU_SSB_IMAGE_SIGN_BACKUP_START,
                                           PAL_FLASH_PAGE_SIZE);
    if (!ret) {
        PRINT("ssb image sign backup error"NEWLINE);
        return ret;
    }
    return true;
}

static bool sec_boot_fota_update_rootkey(void)
{
    if (memcmp((uint8_t *)PAL_MCU_ROOT_PUBKEY_START, (uint8_t *)PAL_FOTA_ROOT_PUBKEY_START,
               RSA4096_PUBKEY_LENGTH) == 0) {
        return true;
    }

    if (uapi_flash_block_erase(CHIP_FLASH_ID, PAL_MCU_ROOT_PUBKEY_START - PAL_FLASH_START, FLASH_PAGE_SIZE, true) !=
        ERRCODE_SUCC) {
        PRINT("erase root pubkey fail"NEWLINE);
        return false;
    }

    if (!sec_boot_flash_memcpy_and_verify(PAL_FOTA_ROOT_PUBKEY_START,
                                          PAL_MCU_ROOT_PUBKEY_START, RSA4096_PUBKEY_LENGTH)) {
        return false;
    }
    return true;
}

static bool sec_boot_fota_update_cert(void)
{
    if (memcmp((uint8_t *)PAL_MCU_SUB_PUBKEY_START, (uint8_t *)PAL_FOTA_SUB_PUBKEY_START,
               sizeof(standard_image_sign_info_t)) == 0) {
        return true;
    }

    if (uapi_flash_block_erase(CHIP_FLASH_ID, PAL_MCU_SUB_PUBKEY_START - PAL_FLASH_START, PAL_FLASH_PAGE_SIZE, true) !=
        ERRCODE_SUCC) {
        PRINT("erase boot certificate fail"NEWLINE);
        return false;
    }

    if (!sec_boot_flash_memcpy_and_verify(PAL_FOTA_SUB_PUBKEY_START, PAL_MCU_SUB_PUBKEY_START,
                                          sizeof(standard_image_sign_info_t))) {
        return false;
    }
    return true;
}

bool sec_boot_fota_save_sign_after_recovery(pal_fota_bin_index_t bin_index)
{
    // copy security boot information from flash to ram, then save it.
    uint32_t src_addr = sec_boot_fota_image_sign_addr_get(bin_index);
    uint8_t *temp = (uint8_t *)irmalloc(sizeof(standard_image_sign_info_t));
    if (temp == NULL) {
        PRINT("Failed to apply for memory"NEWLINE);
        return false;
    }
    errno_t ret = memcpy_s(temp, sizeof(standard_image_sign_info_t),
                           (uint8_t *)(uintptr_t)src_addr, sizeof(standard_image_sign_info_t));
    if (ret != EOK) {
        PRINT("memcpy_s failed!!!"NEWLINE);
        irfree(temp);
        return false;
    }
    // erase sec boot info region in flash.
    if (uapi_flash_block_erase(CHIP_FLASH_ID, src_addr, PAL_FLASH_PAGE_SIZE, true) != ERRCODE_SUCC) {
        PRINT("flash erase fail, bin index:%d, addr:0x%x"NEWLINE, bin_index, src_addr);
        irfree(temp);
        return false;
    }

    // write sec boot info into flash.
    uint32_t dest_addr = sec_boot_image_sign_addr_get(bin_index);
    uapi_flash_write_data(CHIP_FLASH_ID, dest_addr, temp, sizeof(standard_image_sign_info_t));
    irfree(temp);
    return true;
}

bool sec_boot_fota_save_sign_after_full_upgrade(pal_fota_bin_index_t bin_index)
{
    standard_image_sign_info_t *image_sign = (standard_image_sign_info_t *)sec_boot_fota_image_sign_addr_get(bin_index);
    if (image_sign == NULL) {
        return false;
    }

    errcode_t ret;
    uint8_t sha_result[SHA256_HASH_SIZE] = { 0 };
    uint8_t *src_buf = (uint8_t *)(uintptr_t)(image_sign->cert_content.image_load_addr);

    if (bin_index <= APP_INDEX) {
        ret = uapi_drv_cipher_sha256(src_buf, image_sign->cert_content.image_len, sha_result, sizeof(sha_result));
        if (ret != ERRCODE_SUCC) {
            return false;
        }

        if (memcmp(image_sign->cert_content.image_hash, sha_result, SHA256_HASH_SIZE) != 0) {
            PRINT("image %d sha256 check fail after full_upgrade!"NEWLINE, bin_index);
            return false;
        }
    }

    if (bin_index == SSB_INDEX) {
        sec_boot_fota_update_rootkey();
        sec_boot_fota_update_cert();
    }
    return sec_boot_fota_save_sign_after_recovery(bin_index);
}

bool sec_boot_fota_verify_swap_info_sign(pal_fota_bin_index_t fota_bin_index)
{
    uint32_t image_sign_addr = sec_boot_fota_image_sign_addr_get(fota_bin_index);
    if (!sec_boot_image_sign_verify(image_sign_addr, PAL_FOTA_SUB_PUBKEY_START)) {
        PRINT("image %d signature check fail" NEWLINE, fota_bin_index);
        return false;
    }
    return true;
}

bool sec_boot_verify_recovery_image(void)
{
    if (sec_boot_mode_get() == SEC_BOOT_DISABLE) {
        return true;
    }

    if (!sec_boot_image_sign_verify(PAL_MCU_RECOVERY_IMAGE_SIGN_START, PAL_MCU_SUB_PUBKEY_START)) {
        PRINT("recovery/bootloader signature verify fail" NEWLINE);
        return false;
    }
    return true;
}

bool sec_boot_verify_standard_image(void)
{
    if (sec_boot_mode_get() == SEC_BOOT_DISABLE) {
        return true;
    }

    if (!sec_boot_image_sign_verify(PAL_MCU_BT_IMAGE_SIGN_START, PAL_MCU_SUB_PUBKEY_START)) {
        PRINT("bt signature verify fail" NEWLINE);
        return false;
    }
#if DSP_EXIST == YES
    if (!sec_boot_image_sign_verify(PAL_MCU_DSP_IMAGE_SIGN_START, PAL_MCU_SUB_PUBKEY_START)) {
        PRINT("dsp signature verify fail" NEWLINE);
        return false;
    }
#endif
#if DUAL_DSP_EXIST == YES
    if ((HIFI1_IMAGE_PAGES != 0) &&
        (sec_boot_image_sign_verify(PAL_MCU_DSP1_IMAGE_SIGN_START, PAL_MCU_SUB_PUBKEY_START) == false)) {
        PRINT("dsp1 signature verify fail" NEWLINE);
        return false;
    }
#endif

    if (!sec_boot_image_sign_verify(PAL_MCU_APP_IMAGE_SIGN_START, PAL_MCU_SUB_PUBKEY_START)) {
        PRINT("app signature verify fail" NEWLINE);
        return false;
    }

    return true;
}

bool sec_boot_fota_enable_backup_region(pal_fota_bin_index_t fota_bin_index)
{
    if (fota_bin_index != SSB_INDEX) { return true; }

    if (!sec_boot_ssb_image_backup()) {
        PRINT("ssb image backup error"NEWLINE);
        return false;
    }

    if (!sec_boot_ssb_sign_and_pubkey_backup()) {
        PRINT("ssb sign and pubkey backup error"NEWLINE);
        return false;
    }
    return true;
}
