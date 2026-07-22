/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: identity functions
 * Author:
 * Create:
 */
#include "identity.h"
#include <stdio.h>
#include <string.h>
#include "irmalloc.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#ifndef BUILD_APPLICATION_SSB
#if USE_RPC_MODE == YES
#include "rpc_auto_generated_application.h"
#endif
#endif
#include "memory_config_common.h"
#include "securec.h"
#include "debug_print.h"
#include "build_version.h"

#define CHANGEID_LOAD_OFFSET 0x4
#define RPC_GET_SIZE_2_BYTE    2    // 2 byte
#define CID_VERSION_LEN        10
#ifndef BUILD_APPLICATION_SSB

char g_image_version[IMAGE_VERSION_MAX][MAX_VERSION_STRING];
static void get_version_info(image_version_e image)
{
#if !(CHIP_BS25 || CHIP_BS21 || CHIP_WS53)
    uint8_t *version_offset_per_core = NULL;
    switch (image) {
        case IMAGE_VERSION_SSB:
            version_offset_per_core = (uint8_t *)(uintptr_t)(SSB_CHANGEID_LOAD_ORIGIN + CHANGEID_LOAD_OFFSET);
            break;
        case IMAGE_VERSION_RECOVERY:
            version_offset_per_core = (uint8_t *)(uintptr_t)(RECOVERY_CHANGEID_LOAD_ORIGIN + CHANGEID_LOAD_OFFSET);
            break;
        case IMAGE_VERSION_BT:
            version_offset_per_core = (uint8_t *)(uintptr_t)(BT_CHANGEID_LOAD_ORIGIN + CHANGEID_LOAD_OFFSET);
            break;
        case IMAGE_VERSION_HIFI:
            version_offset_per_core = (uint8_t *)(uintptr_t)(DSP_CHANGEID_LOAD_ORIGIN + CHANGEID_LOAD_OFFSET);
            break;
#if (GNSS_EXIST == YES)
        case IMAGE_VERSION_GNSS:
            version_offset_per_core = (uint8_t *)(uintptr_t)(GNSS_CHANGEID_LOAD_ORIGIN + CHANGEID_LOAD_OFFSET);
            break;
#endif
        default:
            break;
    }

    if (version_offset_per_core == NULL) {
        PRINT("get version param error\r\n");
        return;
    }

    (void)memset_s((void *)g_image_version[image], sizeof(g_image_version[image]), 0, sizeof(g_image_version[image]));
    if (memcpy_s((void *)g_image_version[image], sizeof(g_image_version[image]),
                 (void *)version_offset_per_core, sizeof(g_image_version[image])) == EOK) {
        PRINT("image %d, version is %s\r\n", image, g_image_version[image]);
    } else {
        PRINT("image %d, version is NULL\r\n", image);
    }
    return;
#else
    UNUSED(image);
#endif
}

void version_info_init_in_app(void)
{
    /* get firmware version in ssb recovery security of hifi */
    get_version_info(IMAGE_VERSION_SSB);
    get_version_info(IMAGE_VERSION_RECOVERY);
    get_version_info(IMAGE_VERSION_BT);
    get_version_info(IMAGE_VERSION_HIFI);
#if (GNSS_EXIST == YES)
    get_version_info(IMAGE_VERSION_GNSS);
#endif
}

ret_type_e get_firmware_version(image_version_e image, uint16_t *ver_len, char *ver, uint16_t ver_max_len)
{
    ret_type_e result = RET_TYPE_OK;
    int ret;

    if ((ver_len == NULL) || (ver == NULL)) {
        return RET_TYPE_ERROR;
    }

    if (ver_max_len < MAX_VERSION_STRING) {
        return RET_TYPE_ERROR;
    }

    *ver_len = CID_VERSION_LEN;
    ret = memcpy_s(ver, ver_max_len, g_image_version[image], *ver_len);
    if (ret != 0) {
        return RET_TYPE_ERROR;
    }
    return result;
}
#endif

#if (defined(GET_CUSTOMER_SAGA_HADRWARE_ID) && (GET_CUSTOMER_SAGA_HADRWARE_ID == YES))
void set_custom_hardware_ver(uint8_t hardware_id)
{
    uint16_t *ptr_hardware = (uint16_t *)CUSTOM_HARDWARE_VERSION;
    *ptr_hardware = (uint16_t)hardware_id;
}
#endif