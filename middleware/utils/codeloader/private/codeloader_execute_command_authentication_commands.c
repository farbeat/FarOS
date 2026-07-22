/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: BT CODELOADER AUTHENTICATION COMMANDS MODULE
 * Author:
 * Create: 2018-10-15
 */

#if (!defined TARGET_LOCATION_ROM)

#include "codeloader_execute_command_authentication_commands.h"
#include "codeloader.h"
#include "codeloader_command_processor_utils.h"
#include "codeloader_locking.h"
#include "cpu_utils.h"
#include "securec.h"
#include "sec_boot_image_sign_validate.h"
#include "flash.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif

typedef struct {
    core_images_e signature_use;
    uint8_t signature_type;
    uint16_t length_of_data;
    uint8_t data[];
} __attribute__((packed)) codeloader_add_signature_t;  //lint !e959  Lint is wrong here about the enum size.

static uint32_t g_image_sign_addr[CORE_IMAGES_MAX_NUMBER] = {
    MCU_BT_IMAGE_SIGN_START,
#if DSP_EXIST == YES
    MCU_DSP_IMAGE_SIGN_START,
#endif
    MCU_APP_IMAGE_SIGN_START,
    MCU_RECOVERY_IMAGE_SIGN_START,
#if (CHIP_SOCMN1 == 1)
    MCU_DSP1_IMAGE_SIGN_START,
#elif (CHIP_LIBRA == 1)
    MCU_GNSS_IMAGE_SIGN_START,
    MCU_SEC_IMAGE_SIGN_START,
    MCU_DEBUG_CERT_SIGN_START,
#endif
#if SUPPORT_PARTITION_FEATURE == YES
    MCU_PARTITION_TABLE_SIGN_START,
#endif
};

/*
 * Authentication Functions
 */
int8_t codeloader_command_processor_add_signature(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    codeloader_add_signature_t *sig_data = NULL;
    UNUSED(output_buffer);
    core_images_e cimage;
    int8_t signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;

    if (input_buffer->length < sizeof(codeloader_add_signature_t)) {
        return signature_return;
    }

    sig_data = (codeloader_add_signature_t *)input_buffer->buffer;
    sig_data->length_of_data = bitswap_16bit(sig_data->length_of_data);
    cimage = sig_data->signature_use;

    if (!codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage))) {
        // requied for image loading, so no separate CodeLoader production lock
        signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        return signature_return;
    }
    switch ((signature_type_e)sig_data->signature_type) {
        case SIGNATURE_TYPE_SHA256:
            // validate length
            if (sig_data->length_of_data != SHA256_HASH_SIZE) {
                signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
            } else {
                // not support KV, To Do Set NV.
                signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            }
            break;
        case SIGNATURE_TYPE_RSA4096:
            if (sig_data->length_of_data > FLASH_PAGE_SIZE) {
                signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
            } else {
                uint32_t start_addr = g_image_sign_addr[cimage];
                if (start_addr == 0) { break; }
#ifndef USE_EMBED_FLASH
                uapi_flash_sector_erase(CHIP_FLASH_ID, (start_addr - FLASH_START), true);
                uapi_flash_write_data(CHIP_FLASH_ID, (start_addr - FLASH_START), sig_data->data,
                    (uint32_t)sig_data->length_of_data);
#else
                uapi_eflash_erase((start_addr - FLASH_START), FLASH_PAGE_SIZE);
                uapi_eflash_write((start_addr - FLASH_START), (uint32_t *)sig_data->data,
                                  (uint32_t)sig_data->length_of_data);
#endif
                signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            }
            break;
        default:
            signature_return = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            break;
    }
    return signature_return;
}
#endif  // !TARGET_LOCATION_ROM