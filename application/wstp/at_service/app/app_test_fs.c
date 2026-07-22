/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: LiteOS Board.
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
#include "app_test_fs.h"
#include <fcntl.h>
#include <unistd.h>
#include "app_at_process.h"
#include "common_def.h"
#include "soc_osal.h"
#include "watchdog.h"

#ifdef CFG_DRIVERS_MMC
#include "dpal.h"
#include "block.h"
#endif
#include "tcxo.h"

#define TEST_BUFFER_LEN         64
#define ONE_SECOND              (1000000)

static int g_test_fd = 0;
static FILE *g_pfd = NULL;
static unsigned char g_fs_buffer[TEST_BUFFER_LEN] = {0};
static unsigned char g_fs_read_buffer[TEST_BUFFER_LEN] = {0};

int test_libc_write(FILE *fp, uint16_t rw_times)
{
    uint32_t len = 0;
    uint32_t tmp_len;
    uint16_t index;
    uint8_t* write_buf = NULL;
    if (fp == NULL) {
        return 0;
    }

    write_buf = (uint8_t*)malloc(BYTES_LEN_4K);
    if (write_buf == NULL) {
        wstp_print("[W]write malloc write buf error\r\n");
        return 0;
    }

    for (index = 0; index < BYTES_LEN_4K; ++index) {
        write_buf[index] = index;
    }

    uapi_watchdog_kick();
    for(index = 0; index < rw_times; ++index) {
        if (fwrite(write_buf, BYTES_LEN_4K, 1, fp) != 1) {
            wstp_print("[W]test_libc_write fail! errno is %d\r\n", get_errno());
            continue;
        }
        len++;
    }

    len = len * BYTES_LEN_4K;
    free(write_buf);
    wstp_print("[W]fwrite len:[%d]B.\r\n", len);
    return len;
}

int test_libc_read(FILE *fp, uint16_t rw_times)
{
    uint32_t len = 0;
    uint16_t index;
    uint8_t* write_buf = NULL;
    uint8_t* read_buf = NULL;
    if (fp == NULL) {
        return 0;
    }

    write_buf = (uint8_t*)malloc(BYTES_LEN_4K);
    if (write_buf == NULL) {
        wstp_print("[R]write malloc write buf error\r\n");
        return 0;
    }

    read_buf = (uint8_t*)malloc(BYTES_LEN_4K);
    if (read_buf == NULL) {
        wstp_print("[R]read malloc read buf error\r\n");
        free(write_buf);
        return 0;
    }

    for (index = 0; index < BYTES_LEN_4K; ++index) {
        write_buf[index] = index;
    }

    uapi_watchdog_kick();
    for(index = 0; index < rw_times; ++index) {
        if (fread(read_buf, BYTES_LEN_4K, 1, fp) != 1) {
            wstp_print("[R]test_libc_read fail! errno is %d\r\n", get_errno());
            len--;
        }
        len++;
        if(memcmp(write_buf, read_buf, BYTES_LEN_4K) != 0) {
            wstp_print("[R]rw compare fail, time = %d/%d\r\n", len, rw_times);
            break;
        }
    }
    len = len * BYTES_LEN_4K;
    free(write_buf);
    free(read_buf);
    wstp_print("[R]fread len:[%d]B.\r\n",  len);
    return len;
}

int test_libc_rw(FILE *fp)
{
    uint32_t len, tmp_len;
    uint32_t rw_times = FS_TEST_1M_BYTES / BYTES_LEN_4K;

    rewind(fp);
    tmp_len = test_libc_write(fp, rw_times);
    rewind(fp);
    len = test_libc_read(fp, rw_times);

    if (tmp_len == len) {
        wstp_print("[RW]rw check all data success!!\r\n");
        return ERRCODE_SUCC;
    } else {
        wstp_print("[RW]rw check all data fail!!\r\n");
        return ERRCODE_FAIL;
    }
}

void file_basic_test(const char* path)
{
    int len;
    char filebuf[20] = "abcdeabcde";
    char readbuf[20] = {0};
    FILE *fp = NULL;

    wstp_print("%s, open %s:\r\n", __FUNCTION__, path);

    fp = fopen(path, "w+");
    if (fp == NULL) {
        wstp_print("open fail\r\n");
        return;
    }

    len = fwrite(filebuf, 10, 1, fp);
    wstp_print("wirte len %d\r\n", len);

    fseek(fp, 0L, SEEK_SET);
    len = fread(readbuf, 10, 1, fp);
    wstp_print("len:[%d], readbuf:%s\n", len, readbuf);

    memset_s(readbuf, sizeof(readbuf), 0, sizeof(readbuf));

    fseek(fp, 3L, SEEK_SET);
    len = fread(readbuf, 10, 1, fp);
    wstp_print("len:[%d], readbuf:%s\n", len, readbuf);

    rewind(fp);
    memset_s(readbuf, sizeof(readbuf), 0, sizeof(readbuf));
    len = fread(readbuf, 10, 1, fp);
    wstp_print("len:[%d], readbuf:%s\n", len, readbuf);

    test_libc_rw(fp);

    fclose(fp);
}

void app_fs_test_basic(void)
{
    file_basic_test("/user/user_test.log");
    file_basic_test("/update/upd_test.log");
    file_basic_test("/music/music_test.log");
}

#ifdef CFG_DRIVERS_MMC
void file_emmc_test(emmc_test_param* param)
{
    uint8_t* write_buf = NULL;
    uint8_t* read_buf = NULL;

    uint32_t file_size = (4 * 1024 * 1024);
    uint32_t start_sector;
    uint32_t loop_times;
    uint32_t nsector;
    uint32_t index;
    uint32_t len;

    uint64_t rw_start_time;
    uint64_t rw_end_time;

    if ((param == NULL) || (param->rw_bytes == 0)) {
        wstp_print("param:[%p], bytes:[%d]\r\n", param, param->rw_bytes);
        return;
    }

    if (param->rw_bytes < 512) {
        param->rw_bytes = 512;
    }

    loop_times = file_size / param->rw_bytes;
    nsector = param->rw_bytes / 512;

    if (param->randm) {
        start_sector = random()%5000000;
    } else {
        start_sector = 1000000;
    }

    wstp_print("%s, rand:%u, rw_bytes:%u, start_sector:%u, sector_num:%u\r\n",
        __FUNCTION__, param->randm, param->rw_bytes, start_sector, nsector);

    write_buf = dpal_memalign(CACHE_ALIGNED_SIZE, param->rw_bytes);
    if (write_buf == NULL) {
        wstp_print("%s, malloc write buf error\r\n", __FUNCTION__);
        return;
    }

    read_buf = dpal_memalign(CACHE_ALIGNED_SIZE, param->rw_bytes);
    if (read_buf == NULL) {
        wstp_print("%s, malloc read buf error\r\n", __FUNCTION__);
        dpal_free(write_buf);
        return;
    }

    len = 0;
    uint32_t irq_sts = osal_irq_lock();
    rw_start_time = uapi_tcxo_get_ms();
    for(index = 0; index < loop_times; ++index) {
        uint32_t write_sec = emmc_raw_write((char*)write_buf, start_sector+index, nsector);
        if (nsector != write_sec) {
            wstp_print("%s, write err:[%u]-[%u]\r\n", __FUNCTION__, nsector, write_sec);
            break;
        }
        len += param->rw_bytes;
    }
    rw_end_time = uapi_tcxo_get_ms();
    osal_irq_restore(irq_sts);
    wstp_print("%s, emmc write len:[%d]B, start:[%llu]ms, end:[%llu]ms, cost:[%llu]ms, speed:[%f]KB/s\r\n",
        __FUNCTION__, len, rw_start_time, rw_end_time, rw_end_time - rw_start_time,
        (float)(len/1024) / ((float)(rw_end_time - rw_start_time)/1000));

    len = 0;
    irq_sts = osal_irq_lock();
    rw_start_time = uapi_tcxo_get_ms();
    for(index = 0; index < loop_times; ++index) {
        uint32_t read_sec = emmc_raw_read((char*)read_buf, start_sector+index, nsector);
        if (nsector != read_sec) {
            wstp_print("%s, read err:[%u]-[%u]\r\n", __FUNCTION__, nsector, read_sec);
            break;
        }
        len += param->rw_bytes;
    }
    rw_end_time = uapi_tcxo_get_ms();
    osal_irq_restore(irq_sts);
    wstp_print("%s, emmc read len:[%d]B, start:[%llu]ms, end:[%llu]ms, cost:[%llu]ms, speed:[%f]KB/s\r\n",
        __FUNCTION__, len, rw_start_time, rw_end_time, rw_end_time - rw_start_time,
        (float)(len/1024) / ((float)(rw_end_time - rw_start_time)/1000));

    if(memcmp(write_buf, read_buf, param->rw_bytes) != 0) {
        wstp_print("%s, emmc test fail\r\n", __FUNCTION__);
    } else {
        wstp_print("%s, emmc test ok\r\n", __FUNCTION__);
    }

    dpal_free(write_buf);
    dpal_free(read_buf);
}
#endif

void app_fs_test_performace(const char* cmd)
{
    emmc_test_param param = { 0 };

    sscanf((char*)cmd, "%d,%d", &param.randm, &param.rw_bytes);
#ifdef CFG_DRIVERS_MMC
    file_emmc_test(&param);
#endif
}

uint32_t test_file_fread(char type, char *ptr)
{
    unused(ptr);
    int i;
    int len;

    unused(ptr);
    for (i = 0; i < TEST_BUFFER_LEN; i++) {
        g_fs_read_buffer[i] = 0;
    }

    if (type != 'f') {
        len = read(g_test_fd, g_fs_read_buffer, TEST_BUFFER_LEN);
        if (len < TEST_BUFFER_LEN) {
            wstp_print("[fs_test]read error, %d\r\n", len);
        }
        wstp_print("[fs_test]read, len = %d\r\n", len);

    } else {
        len = fread(g_fs_read_buffer, 1, TEST_BUFFER_LEN, g_pfd);
        if (len != TEST_BUFFER_LEN) {
            wstp_print("[fs_test]fread file failed. readlen = %d\r\n", len);
        } else {
            wstp_print("[fs_test]fread file success. readlen = %d\r\n", len);
        }
    }

    wstp_print("[fs_test]read data:\r\n");
    for (i = 0; i < TEST_BUFFER_LEN; i++) {
        wstp_print("%02X ", g_fs_read_buffer[i]);
    }
    wstp_print("\r\n");
    return ERRCODE_SUCC;
}

uint32_t test_file_open(char type, char *ptr)
{
    uint8_t mod = 0;
    int flags = 0;
    char *openflag = "r";
    char file_name[MAX_FILE_PATH_SIZE] = {0};
    uint32_t name_len = 0;
    char *next;

    mod = strtoul(ptr, NULL, 0);
    ptr += 2;
    ptr++;

    strncpy_s(file_name, MAX_FILE_PATH_SIZE, "/user/", strlen("/user/"));

    next = strchr(ptr, '"');
    name_len = next - ptr;
    strncpy_s(file_name + strlen("/user/"), MAX_FILE_PATH_SIZE - strlen("/user/"), ptr, name_len);

    if (type != 'f') {
        if (mod == 0) {
            flags = O_RDONLY;
        } else if (mod == 1) {
            flags = O_RDWR;
        } else if (mod == 2) {
            flags = O_CREAT | O_WRONLY | O_TRUNC;
        } else if (mod == 3) {
            flags = O_CREAT | O_RDWR | O_TRUNC;
        } else if (mod == 4) {
            flags = O_CREAT | O_RDWR | O_APPEND;
        } else if (mod == 5) {
            flags = O_CREAT | O_RDWR;
        }

        wstp_print("[fs_test]open file %s mod = %d open flag = 0x%x\r\n", file_name, mod, flags);
        g_test_fd = open(file_name, flags, 0);
        if (g_test_fd < 0) {
            wstp_print("[fs_test]open error, %d, flag = 0x%x\r\n", g_test_fd, flags);
        } else {
            wstp_print("[fs_test]open file success\r\n");
        }
    } else {
        if (mod == 0) {
            openflag = "r"; /* O_RDONLY */
        } else if (mod == 1) {
            openflag = "r+"; /* O_RDWR */
        } else if (mod == 2) {
            openflag = "w"; /* O_CREAT | O_WRONLY | O_TRUNC */
        } else if (mod == 3) {
            openflag = "w+"; /* O_CREAT | O_RDWR | O_TRUNC */
        } else if (mod == 4) {
            openflag = "a+"; /* O_CREAT | O_RDWR | O_APPEND */
        } else {
            wstp_print("[fs_test]not support the flag\r\n");
            return ERRCODE_FAIL;
        }

        wstp_print("[fs_test]open file %s mod = %d fopen flag = %s\r\n", file_name, mod, openflag);
        g_pfd = fopen(file_name, openflag);
        if (g_pfd == NULL) {
            wstp_print("[fs_test]fopen file failed\r\n");
        } else {
            wstp_print("[fs_test]fopen file success\r\n");
        }
    }

    return ERRCODE_SUCC;
}

uint32_t test_file_write(char type, char *ptr)
{
    uint8_t mod = 0;
    int i;
    int len;

    mod = strtoul(ptr, NULL, 0);

    for (i = 0; i < TEST_BUFFER_LEN; i++) {
        g_fs_buffer[i] = i * mod;
    }

    if (type != 'f') {
        len = write(g_test_fd, g_fs_buffer, TEST_BUFFER_LEN);
        wstp_print("[fs_test]write, len = %d\r\n", len);
        if (len < TEST_BUFFER_LEN) {
            wstp_print("[fs_test]write error, %d\r\n", len);
        }
    } else {
        len = fwrite(g_fs_buffer, 1, TEST_BUFFER_LEN, g_pfd);
        if (len != TEST_BUFFER_LEN) {
            wstp_print("[fs_test]fwrite file failed. writeLen = %d\r\n", len);
        } else {
            wstp_print("[fs_test]fwrite file success. writeLen = %d\r\n", len);
        }
    }
    return ERRCODE_SUCC;
}

uint32_t test_file_seek(char type, char *ptr)
{
    unused(ptr);
    uint32_t ret;
    if (type != 'f') {
        ret = lseek(g_test_fd, 0, SEEK_SET);
    } else {
        ret = fseek(g_pfd, 0, SEEK_SET);
    }
    if (ret == 0) {
        wstp_print("[fs_test]lseek success\r\n");
    } else {
        wstp_print("[fs_test]lseek failed ret = %d\r\n", ret);
    }
    return ret;
}

uint32_t test_file_close(char type, char *ptr)
{
    unused(ptr);
    uint32_t ret;
    if (type != 'f') {
        ret = close(g_test_fd);
        if (ret == 0) {
            g_test_fd = 0;
            wstp_print("[fs_test]close success\r\n");
        } else {
            wstp_print("[fs_test]fclose failed ret = %d\r\n", ret);
        }
    } else {
        ret = fclose(g_pfd);
        if (ret == 0) {
            g_pfd = NULL;
            wstp_print("[fs_test]fclose success\r\n");
        } else {
            wstp_print("[fs_test]fclose failed ret = %d\r\n", ret);
        }
    }
    return ret;
}

uint32_t test_file_system(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    if (para == NULL) {
        return -1;
    }

    unused(para_len);
    char *ptr = (char *)para;
    int32_t ret = ERRCODE_SUCC;
    char type;
    char cmd;

    type = *ptr;
    ptr += 2;

    if (type != 'f' && type != 'n') {
        return ERRCODE_FAIL;
    }

    cmd = *ptr;
    ptr += 2;

    switch (cmd) {
        case 'o':
            ret = test_file_open(type, ptr);
            break;
        case 'w':
            ret = test_file_write(type, ptr);
            break;
        case 'r':
            ret = test_file_fread(type, ptr);
            break;
        case 's':
            ret = test_file_seek(type, ptr);
            break;
        case 'c':
            ret = test_file_close(type, ptr);
            break;
        case 'b':
            app_fs_test_basic();
            break;
        case 'p':
            app_fs_test_performace(ptr);
            break;
        default:
            return ERRCODE_FAIL;
    }
    return ret;
}

void byte_copy(char* dest, char* src, uint32_t n)
{
    uint32_t *dbp = (uint32_t*)dest;
    uint32_t *sbp = (uint32_t*)src;
    while (n--) {
        *dbp++ = *sbp++;
    }
}

uint32_t fs_test_rw_speed(const char* path, uint16_t width)
{
    uint64_t start_time, end_time;
    uint32_t total_time;
    char* buf = osal_kmalloc(width, OSAL_GFP_KERNEL);
    if (buf == NULL) {
        return ERRCODE_FAIL;
    }

    int fd = open(path, O_CREAT | O_RDWR | O_TRUNC);
    if (fd < 0) {
        wstp_print("fs test open first fail!\r\n");
        osal_kfree(buf);
        return ERRCODE_FAIL;
    }
    uint32_t times = FS_TEST_1M_BYTES / width;
    uint32_t irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    for (uint32_t i = 0; i < times; i++) {
        (void)write(fd, buf, width);
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time);
    uint32_t bw = (ONE_SECOND * BYTES_LEN_1K / total_time);
    wstp_print("[fs test]fs write width:%d time=%dus bw:%d KB/s\r\n", width, total_time, bw);
    (void)close(fd);

    fd = open(path, O_RDWR);
    if (fd < 0) {
        wstp_print("fs test open second fail!\r\n");
        osal_kfree(buf);
        return ERRCODE_FAIL;
    }
    irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    for (uint32_t i = 0; i < times; i++) {
        (void)read(fd, buf, width);
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = end_time - start_time;
    bw = (ONE_SECOND * BYTES_LEN_1K / total_time);

    wstp_print("[fs test]fs read width:%d time=%dus bw:%d KB/s\r\n", width, total_time, bw);
    (void)close(fd);
    osal_kfree(buf);

    return ERRCODE_SUCC;
}