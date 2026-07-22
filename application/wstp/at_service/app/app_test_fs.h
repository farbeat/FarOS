/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Platform Config HeadFile
 * Author: @CompanyNameTag
 * Create: 2021-09-03
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#ifndef _APP_TEST_FS_H
#define _APP_TEST_FS_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TEST_PARA_2             2
#define BYTES_LEN_128           128
#define BYTES_LEN_512           512
#define BYTES_LEN_1K            1024
#define BYTES_LEN_4K            4096
#define BYTES_LEN_8K            8192
#define BUFF_LEN_48K            (48 * 1024)
#define BUFF_LEN_64K            (64 * 1024)
#define FS_TEST_1M_BYTES        1048576
#define MAX_FILE_PATH_SIZE      25
#define TCXO_ERROR              40

#define FS_PARTITION_NAME_SYSTEM "/system"
#define FS_PARTITION_NAME_USER "/user"
#define FS_PARTITION_NAME_UPDATE "/update"
#define FS_PARTITION_NAME_MUSIC "/music"

typedef struct {
    uint32_t randm;
    uint32_t rw_bytes;
} emmc_test_param;

void app_fs_test_basic(void);
int test_libc_write(FILE *fp, uint16_t rw_times);
int test_libc_read(FILE *fp, uint16_t rw_times);
int test_libc_rw(FILE *fp);
void app_fs_test_performace(const char* cmd);
uint32_t test_file_system(uint8_t *para, uint32_t para_len);
void fs_get_file_path(uint8_t DirFlag, char *FileName, char *FilePath);
void file_emmc_test(emmc_test_param* param);
void byte_copy(char* dest, char* src, uint32_t n);
uint32_t fs_test_rw_speed(const char* path, uint16_t width);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _APP_TEST_FS_H */

