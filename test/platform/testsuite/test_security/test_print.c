/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 * Description: Test cipher debug source code \n
 * Author: @CompanyNameTag \n
 * Date: 2023-07-10 \n
 */

#include "test_print.h"


#define DUMP_BYTES_ONCE_LEN     16
#define DUMP_SIZE_PER_BYTE      3


void print_buffer(const char *name, const uint8_t *buf, uint32_t len)
{
    uint8_t dump_buf[DUMP_BYTES_ONCE_LEN * DUMP_SIZE_PER_BYTE + 1] = {0};

    if ((name == NULL) || (buf == NULL)) {
        return;
    }

    PRINT("name: %s, addr: 0x%08x, len: %08x\n", name, buf, len);

    for (uint32_t i = 0; i < len; i += DUMP_BYTES_ONCE_LEN) {
        uint32_t tmp_len = min(i + DUMP_BYTES_ONCE_LEN, len);
        uint8_t *ptr = dump_buf;
        for (uint32_t j = i; j < tmp_len; j++) {
            *ptr++ = "0123456789ABCDEF"[(buf[j] >> 4) & 0xf];   /* high 4 bits, right shift 4 bits. */
            *ptr++ = "0123456789ABCDEF"[buf[j] & 0xf];          /* low 4 bits. */
            *ptr++ = ' ';
        }

        *ptr = '\0';
        PRINT("%s\n", dump_buf);
    }
}
