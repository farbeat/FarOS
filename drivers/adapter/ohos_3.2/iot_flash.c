/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides iot_flash driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-10-18, Create file. \n
 */
#include "iot_errno.h"
#include "flash.h"
#include "iot_flash.h"

#define IOT_FLASH_ID                    FLASH_1
#define IOT_FLASH_ERASE_WAITE_STATE     true

unsigned int IoTFlashInit(void)
{
    if (uapi_flash_init(IOT_FLASH_ID) != ERRCODE_SUCC) {
        return IOT_FAILURE;
    }
    return IOT_SUCCESS;
}

unsigned int IoTFlashDeinit(void)
{
    if (uapi_flash_deinit(IOT_FLASH_ID) != ERRCODE_SUCC) {
        return IOT_FAILURE;
    }
    return IOT_SUCCESS;
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size, const unsigned char *ramData,
                           unsigned char doErase)
{
    if (ramData == NULL) {
        return IOT_FAILURE;
    }
    if (doErase) {
        if (IoTFlashErase(flashOffset, size) != (unsigned int)IOT_SUCCESS) {
            return IOT_FAILURE;
        }
    }
    if (uapi_flash_write_data(IOT_FLASH_ID, (uint32_t)flashOffset, (uint8_t *)ramData,
        (uint32_t)size) != (uint32_t)size) {
        return IOT_FAILURE;
    }

    return IOT_SUCCESS;
}

unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
    if (ramData == NULL) {
        return IOT_FAILURE;
    }
    if (uapi_flash_read_data(IOT_FLASH_ID, (uint32_t)flashOffset, (uint8_t *)ramData,
        (uint32_t)size) != (uint32_t)size) {
        return IOT_FAILURE;
    }
    return IOT_SUCCESS;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
    if (uapi_flash_block_erase(IOT_FLASH_ID, (uint32_t)flashOffset, (uint32_t)size,
        IOT_FLASH_ERASE_WAITE_STATE) != ERRCODE_SUCC) {
        return IOT_FAILURE;
    }
    return IOT_SUCCESS;
}