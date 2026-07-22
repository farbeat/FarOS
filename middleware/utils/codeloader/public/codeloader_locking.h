/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  Codeloader Module Lock Dsfinitions
 * Author:
 * Create:
 */

#ifndef CODELOADER_LOCKING_H
#define CODELOADER_LOCKING_H

#include "codeloader.h"
#include "platform_types.h"

typedef enum {
    CODELOADER_LOCK_NO_LOCKS = 0x00000000,
    CODELOADER_LOCK_WRITE_OTP = 0x00000001,
    CODELOADER_LOCK_WRITE_RFCONFIG = 0x00000002,
    CODELOADER_LOCK_WRITE_KV_BT = 0x00000004,
    CODELOADER_LOCK_WRITE_KV_PROTOCOL = 0x00000008,
    CODELOADER_LOCK_WRITE_KV_APPS = 0x00000010,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_BT = 0x00000020,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_PROTOCOL = 0x00000040,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_APPS = 0x00000080,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_RECOVER = 0x00000100,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN0 = 0x00000200,
#if (CHIP == TARGET_CHIP_LIBRA)
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN1 = 0x10000000,
    CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN1_SSB = 0x20000000,
#endif
    CODELOADER_LOCK_WRITE_CODELOADER_LOCK_BT = 0x00000400,
    CODELOADER_LOCK_WRITE_CODELOADER_LOCK_PROTOCOL = 0x00000800,
    CODELOADER_LOCK_WRITE_CODELOADER_LOCK_APPS = 0x00001000,
    CODELOADER_LOCK_WRITE_SWD_LOCK_BT = 0x00002000,
    CODELOADER_LOCK_WRITE_SWD_LOCK_PROTOCOL = 0x00004000,
    CODELOADER_LOCK_WRITE_SWD_LOCK_APPS = 0x00008000,
    CODELOADER_LOCK_WRITE_ERASE_CHIP = 0x00010000,
    CODELOADER_LOCK_WRITE_MEMORY_CONFIG = 0x00020000,
    CODELOADER_LOCK_WRITE_FLASH_LIMITS = 0x00040000,
    CODELOADER_LOCK_READ_OTP = 0x00080000,
    CODELOADER_LOCK_READ_RFCONFIG = 0x00100000,
    CODELOADER_LOCK_READ_KV_BT = 0x00200000,
    CODELOADER_LOCK_READ_KV_PROTOCOL = 0x00400000,
    CODELOADER_LOCK_READ_KV_APPS = 0x00800000,
    CODELOADER_LOCK_WRITE_ERASE_NV_CONFIG = 0x01000000,
    CODELOADER_LOCK_WRITE_CERTIFICATE = 0x02000000,
    CODELOADER_LOCK_WRITE_LOG_BEHAVIOUR_CONFIG = 0x04000000,
    CODELOADER_LOCK_NV_SCAN_RANGE = 0x08000000,
} codeloader_lock_e;

/**
 * @brief  read codeloade lock value from security KV store
 * if no ky is present, nothing is locked
 */
void codeloader_lock_init(const codeloader_options_t *options);

/**
 * @brief  return codeloader lock bitmap value
 * @return vlaue of lock bitmap. If no key is present, CODELOADER_LOCK_NO_LOCKS is returned
 */
uint32_t codeloader_lock_read(void);

/**
 * @brief  check if requested lock is set
 * @param  Permission mask
 */
bool codeloader_unlocked_check(uint32_t mask);

/**
 * @brief  Returns the Codeloader permission for a specified core.
 * @param  core Core to check the permissions for.
 * @return true if the codeloader has been initialized with permission for the core specified
 */
bool codeloader_check_permissions(cores_t core);

/**
 * @brief  Update the codeloader permissions for a given core.
 * @param  core Core to update the permissions to
 * @param  allowed if true the codeloader will update its internal state with the new permission set.
 */
void codeloader_update_permissions(cores_t core, bool allowed);

#endif