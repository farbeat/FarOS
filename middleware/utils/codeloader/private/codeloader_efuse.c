/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Codeloader efuse source file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-04, Create file. \n
 */

#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "sha256/sha256.h"
#include "securec.h"
#include "chip_io.h"
#include "codeloader_efuse.h"

#define BYTES_PER_EFUSE  2

typedef struct {
    uint8_t hash[SHA256_HASH_SIZE];     /* hash of configuration file. */
    uint8_t stru_ver;     /* default 0. */
    uint8_t stru_size;    /* sizeof(struct efuse_config_header). */
    uint16_t number;      /* Item number to be burn. */
    uint32_t file_size;   /* Configuration file size. */
    uint32_t reserved[2]; /* Reserved 2 u32. */
    uint8_t data[0];      /* Item: size = file_size - stru_size. */
} efuse_config_header;

typedef struct {
    uint8_t stru_ver;     /* default 0. */
    uint8_t stru_size;    /* sizeof(struct efuse_config_item) */
    uint16_t start_byte;  /* Start byte of efuse */
    uint16_t byte_width;  /* Byte width */
    uint16_t value_len;   /* Length of value Byte(s), 4-byte-aligned. */
    uint8_t value[0];     /* Item, offset: stru_size. */
} efuse_config_item;

static bool efuse_cfg_verify(const uint8_t *input_buffer, uint32_t bytes_to_write)
{
    efuse_config_header *header = (efuse_config_header *)input_buffer;
    char sha256[SHA256_HASH_SIZE] = { 0 };
    sha256_context_t md;

    if ((header->file_size > bytes_to_write) || (header->file_size < header->stru_size)) {
        return false;
    }

    sha256_init(&md);
    SHA256Update(&md, (unsigned char *)(input_buffer + SHA256_HASH_SIZE), header->file_size - SHA256_HASH_SIZE);
    sha256_final(&md, (unsigned char *)sha256, SHA256_HASH_SIZE);

    if (memcmp(sha256, input_buffer, SHA256_HASH_SIZE) != 0) {
        return false;
    }

    return true;
}

bool codeloader_efuse_set_byte(const uint8_t *input_buffer_to_write, uint32_t bytes_to_write)
{
    if (!efuse_cfg_verify(input_buffer_to_write, bytes_to_write)) {
        return false;
    }

    efuse_config_header *header = (efuse_config_header *)input_buffer_to_write;
    efuse_config_item *item = (efuse_config_item *)(input_buffer_to_write + header->stru_size);
    for (uint8_t i = 0; i < header->number; i++) {
        if (item == NULL) {
            return false;
        }
#ifdef EFUSE_INTERFACE_REPLACE
        if (uapi_efuse_write_buffer(item->start_byte, item->value, item->value_len) != ERRCODE_SUCC) {
            return false;
        }
#else
        if (otp_write_buffer(item->start_byte, item->value, item->value_len) != OTP_RET_SUCC) {
            return false;
        }
#endif

        item = (efuse_config_item *)((uintptr_t)item + item->stru_size + item->value_len);
    }

    return true;
}