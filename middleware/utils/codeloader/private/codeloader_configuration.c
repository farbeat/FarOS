/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:    BT CODELOADER CONFIGURATION MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "panic.h"
#include "otp_map.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "codeloader_configuration.h"

/** Helper macro */
/** OTP Byte that has the Codeloader flags */
/** Codeloader permissions security lock mask */
#define codeloader_config_flags_core_locked_mask(x) BIT(x)
#define OTP_CODELOADER_FLAGS_BYTE_LEN               1

typedef struct {
    uint8_t codeloader_flags;
} codeloader_config;

static const codeloader_config g_codeloader_configuration_default_config = { 0 };

static void codeloader_configuration_get_config(codeloader_config *config)
{
    *config = g_codeloader_configuration_default_config;
    return;
}

static bool codeloader_configuration_set_config(const codeloader_config *config)
{
    UNUSED(config);
    return true;
}

/* Get the codeloader permissions */
void codeloader_configuration_get_permissions(bool *sec_allowed, bool *prot_allowed, bool *apps_allowed)
{
    uint8_t codeloader_flags = 0;
    uint8_t cl_otp_config_flags = 0;
#if CHIP_BS21
    errcode_t otp_return_code = OTP_RET_SUCC;
#else
    // Read OTP flags
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_return_code = uapi_efuse_read_buffer(&cl_otp_config_flags,
        OTP_CODELOADER_FLAGS_BYTE, OTP_CODELOADER_FLAGS_BYTE_LEN);
#else
    otp_ret_t otp_return_code = otp_read_byte(&cl_otp_config_flags, OTP_CODELOADER_FLAGS_BYTE);
#endif
#endif
    // Default to NOT allowed.
    *sec_allowed = false;
    *prot_allowed = false;
    *apps_allowed = false;

    // Check the OTP data is valid
#ifdef EFUSE_INTERFACE_REPLACE
    if (otp_return_code == ERRCODE_SUCC) {  // valid otp data
#else
    if (otp_return_code == OTP_RET_SUCC) {  // valid otp data
#endif
        // load temporary lock flags as well
        codeloader_config cl_flash_config;
        codeloader_configuration_get_config(&cl_flash_config);

        // the codeloader flags indicate locks and will be the or of both
        codeloader_flags = cl_otp_config_flags | cl_flash_config.codeloader_flags;
        *sec_allowed = ((codeloader_flags &
            codeloader_config_flags_core_locked_mask(OTP_CODELOADER_FLAGS_LOCKED_CORES_BT_CORE_BIT)) == 0);
        *prot_allowed = ((codeloader_flags &
            codeloader_config_flags_core_locked_mask(OTP_CODELOADER_FLAGS_LOCKED_CORES_PROTOCOL_CORE_BIT)) == 0);
        *apps_allowed = ((codeloader_flags &
            codeloader_config_flags_core_locked_mask(OTP_CODELOADER_FLAGS_LOCKED_CORES_APPS_CORE_BIT)) == 0);
    }
}

/* Lock permanently the permissions for a given core. */
void codeloader_configuration_permanent_lock_permissions(cores_t core)
{
    uint8_t cl_otp_config_flags = 0;

    // check the parameters
    if (core >= CORES_MAX_NUMBER_PHYSICAL) {
        return;
    }

    // Read OTP flags
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_return_code = uapi_efuse_read_buffer(&cl_otp_config_flags,
        OTP_CODELOADER_FLAGS_BYTE, OTP_CODELOADER_FLAGS_BYTE_LEN);
    // Check the OTP data is valid
    if (otp_return_code == ERRCODE_SUCC) {  // valid otp data
        // write the new value only if it's necessary
        if ((cl_otp_config_flags & codeloader_config_flags_core_locked_mask(core)) == 0) {
            cl_otp_config_flags |= (uint8_t)codeloader_config_flags_core_locked_mask(core);
            // using write bit in order to reduce the time
            (void)uapi_efuse_write_buffer(OTP_CODELOADER_FLAGS_BYTE,
                &cl_otp_config_flags, OTP_CODELOADER_FLAGS_BYTE_LEN);
        }
    } else {
        panic(PANIC_CODELOADER, __LINE__);
    }
#else
    otp_ret_t otp_return_code = otp_read_byte(&cl_otp_config_flags, OTP_CODELOADER_FLAGS_BYTE);
    // Check the OTP data is valid
    if (otp_return_code == OTP_RET_SUCC) {  // valid otp data
        // write the new value only if it's necessary
        if ((cl_otp_config_flags & codeloader_config_flags_core_locked_mask(core)) == 0) {
            cl_otp_config_flags |= (uint8_t)codeloader_config_flags_core_locked_mask(core);
            // using write bit in order to reduce the time
            (void)otp_write_byte(OTP_CODELOADER_FLAGS_BYTE, cl_otp_config_flags);
        }
    } else {
        panic(PANIC_CODELOADER, __LINE__);
    }
#endif
}

/* Lock temporarily the permissions for a given core */
void codeloader_configuration_temporary_lock_permissions(cores_t core)
{
    // check the parameters
    if (core >= CORES_MAX_NUMBER_PHYSICAL) {
        return;
    }

    // read the old configuration
    codeloader_config cl_flash_config;
    codeloader_configuration_get_config(&cl_flash_config);

    // write the new configuration only if it is necessary
    if ((cl_flash_config.codeloader_flags & codeloader_config_flags_core_locked_mask(core)) == 0) {
        codeloader_config cl_new_flash_config;

        cl_new_flash_config.codeloader_flags = cl_flash_config.codeloader_flags |
                                               (uint8_t)codeloader_config_flags_core_locked_mask(core);
        (void)codeloader_configuration_set_config(&cl_new_flash_config);
    }
}

/* Unlock the temporal lock for a given core */
void codeloader_configuration_temporary_unlock_permissions(cores_t core)
{
    // check the parameters
    if (core >= CORES_MAX_NUMBER_PHYSICAL) {
        return;
    }

    // read the old configuration
    codeloader_config cl_flash_config;
    codeloader_configuration_get_config(&cl_flash_config);

    // write the new configuration only if it is necessary
    if ((cl_flash_config.codeloader_flags & codeloader_config_flags_core_locked_mask(core)) != 0) {
        codeloader_config cl_new_flash_config;

        cl_new_flash_config.codeloader_flags = cl_flash_config.codeloader_flags &
                                               ~codeloader_config_flags_core_locked_mask(core);
        (void)codeloader_configuration_set_config(&cl_new_flash_config);
    }
}
