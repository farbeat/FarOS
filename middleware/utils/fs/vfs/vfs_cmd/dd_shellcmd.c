/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: implementation for dd commands.
 * Create: 2021-03-01
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

#include "fcntl.h"
#include "inode/inode.h"
#include "unistd.h"
#include "dpal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef CONFIG_SHELL
#define LOSCFG_DD_DEFAULT_BS 1024
#define LOSCFG_DD_DEFAULT_CNT 1
#define LOSCFG_DD_TASK_PRIORITY 10
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE 2048
#define LOS_TASK_STATUS_DETACHED 0x0100U
#define SYS_CLOCK_OS (24 * 1000 * 1000) /* Uranus 200Mhz->debug 24M */

#ifndef REG_BASE_MTIMER
#define REG_BASE_MTIMER 0x110D1000
#endif
#ifndef REG_TIMER_MTIME
#define REG_TIMER_MTIME 0x8
#endif
#ifndef REG_TIMER_MTIME_H
#define REG_TIMER_MTIME_H 0xC
#endif
#ifndef UINT64_SHIFT_32_BIT
#define UINT64_SHIFT_32_BIT 32
#endif

typedef unsigned long long UINT64;

typedef struct {
    char filePath[PATH_MAX];
    unsigned char mode;
    unsigned int blockSize;
    unsigned int blockCount;
} DdCmdAttr;

enum DdMode {
    MODE_READ = 1,
    MODE_WRITE,
    MODE_MAX,
};

typedef struct {
    const char *translate;
    int oFlag;
    int sFlag;
} ModeTable;

#define DD_USAGE() do { \
    printf("dd file=[FILEPATH] mode=[1:READ/2:WRITE] bs=[SIZE] count=[N]\r\n");\
} while (0)

typedef ssize_t (*READ_WRITE_FUNC)(int fd, FAR void *buf, size_t nbytes);

static ModeTable g_modeTbl[] = {
    [MODE_READ]       = {"read", O_RDONLY, S_IRUSR | S_IWUSR},
    [MODE_WRITE]      = {"write", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR},
    [MODE_MAX]        = {"not support", 0, 0},
};

static inline uint64_t OsDdGetCurrTime(VOID)
{
    unsigned int mtimeLo, mtimeHi;

    mtimeLo = dpal_readl(REG_BASE_MTIMER + REG_TIMER_MTIME);
    mtimeHi = dpal_readl(REG_BASE_MTIMER + REG_TIMER_MTIME_H);

    return mtimeLo + (((UINT64)mtimeHi) << UINT64_SHIFT_32_BIT);
}

static inline VOID OsDdPrintRate(uint64_t startTime, const char *opt, uint64_t size)
{
    double time = (OsDdGetCurrTime() - startTime) * 1.0 / SYS_CLOCK_OS;
    double mb = size * 1.0 / (1024 * 1024);
    double gb = mb / 1024;
    double speed = mb / time;
    printf(" total %llu bytes (%0.2lf GB, %0.2lfMB) %s, time used: %.6f(s), %0.2lfMB/S\r\n",
        size, gb, mb, opt, time, speed);
}

static VOID OsParamsDump(const DdCmdAttr *attr)
{
    printf("---------------------------\r\n");
    printf("filePath   : %s\r\n", attr->filePath);
    printf("mode       : %s\r\n", g_modeTbl[attr->mode].translate);
    printf("blockSize  : %u\r\n", attr->blockSize);
    printf("blockCount : %u\r\n", attr->blockCount);
    printf("---------------------------\r\n");
}

static BOOL OsParamsValid(DdCmdAttr *attr)
{
    if ((attr->filePath == NULL) ||(*(attr->filePath) == '\0')) {
        printf("filename must not empty.\n");
        return false;
    }

    if ((attr->mode >= MODE_MAX) || (attr->mode < MODE_READ)){
        printf("mode must be 1(read) or 2(write).\n");
        return false;
    }

    if (attr->blockSize == 0) {
        attr->blockSize = LOSCFG_DD_DEFAULT_BS;
        printf("bs not configured, used default %d.\n", LOSCFG_DD_DEFAULT_BS);
    }

    if(attr->blockCount == 0) {
        attr->blockCount = LOSCFG_DD_DEFAULT_CNT;
        printf("count not configured, used default %d.\n", LOSCFG_DD_DEFAULT_CNT);
    }

    return true;
}

static unsigned int OsParseOpt(int argc, const char **argv, DdCmdAttr *attr)
{
#define OPTION_MATCH(d, s) strncmp((d), (s), strlen(s))

    unsigned int i;
    char *endPtr = NULL;
    for (i = 0; i < argc; i++) {
        char const *name = argv[i];
        char const *value = strchr(name, '=');
        if (value == NULL) {
            return DPAL_NOK;
        }

        value++;
        printf("[%u]:[%s][%s]\n", i, name, value);

        if (OPTION_MATCH(name, "file") == 0) {
            if (snprintf_s(attr->filePath, PATH_MAX, PATH_MAX - 1, "%s", value) < 0) {
                printf("filePath error\n");
                return DPAL_NOK;
            }
        } else if (OPTION_MATCH(name, "mode") == 0) {
            attr->mode = strtoul(value, &endPtr, 0);
        } else if (OPTION_MATCH(name, "bs") == 0) {
            attr->blockSize = strtoul(value, &endPtr, 0);
        } else if (OPTION_MATCH(name , "count") == 0) {
            attr->blockCount = strtoul(value, &endPtr, 0);
        } else {
            printf("invalid opt %u!\n", i);
            return DPAL_NOK;
        }

    }

    if (!OsParamsValid(attr)) {
        return DPAL_NOK;
    }

    OsParamsDump(attr);
    return 0;
}

static inline ssize_t OsRead(int fd, FAR void *buf, size_t nbytes)
{
    return read(fd, buf, nbytes);
}

static inline ssize_t OsWrite(int fd, FAR void *buf, size_t nbytes)
{
    return write(fd, buf, nbytes);
}

static ssize_t OsReadWriteFile(int fd, unsigned int bs, unsigned int c, bool r)
{
    ssize_t nBytes;
    unsigned int blockSize = bs;
    unsigned int count = c;
    ssize_t totalSize = 0;
    READ_WRITE_FUNC callback = NULL;

    char *buf = dpal_malloc(blockSize);
    if (buf == NULL) {
        printf("memory not enough for buffer size 0x%x\n", blockSize);
        return 0;
    }

    (VOID)memset_s(buf, blockSize, 0, blockSize);

    if (r) {
        callback = OsRead;
    } else {
        callback = OsWrite;
    }

    do {
        nBytes = callback(fd, buf, blockSize);
        if (nBytes < 0) {
            printf("fail to operate: %s, bytes = %zd\n", strerror(errno), nBytes);
            goto END;
        } else if (nBytes != blockSize) {
            totalSize += nBytes;
            goto END;
        }
        totalSize += nBytes;

    } while (count-- > 1);
END:
    (VOID)dpal_free(buf);
    return totalSize;
}

VOID OsDdProcess(const VOID *arg)
{
    int ret;
    char *fullPath = NULL;
    int fd = -1;
    uint64_t totalSize = 0;
    uint64_t startTime = 0;
    DdCmdAttr *attr = (DdCmdAttr *)arg;

    char *shellWorkingDirt = dpal_get_shell_work_dir();
    if (shellWorkingDirt == NULL) {
        printf("dd process failed, get work dir is null.\n");
        goto RELEASE_ATTR;
    }

    printf("#tid = %u\n", dpal_get_curr_task());

    ret = vfs_normalize_path(shellWorkingDirt, attr->filePath, &fullPath);
    if (ret < 0) {
        printf("dd process path failed %d.\n", -ret);
        goto RELEASE_ATTR;
    }

    fd = open(fullPath, g_modeTbl[attr->mode].oFlag, g_modeTbl[attr->mode].sFlag);
    if (fd < 0) {
        printf("dd process open %s failed, %d, %s.\n", fullPath, errno, strerror(errno));
        goto RELEASE_PATH;
    }

    startTime = OsDdGetCurrTime();

    if (attr->mode == MODE_READ) {
        totalSize += OsReadWriteFile(fd, attr->blockSize, attr->blockCount, true);
    } else if (attr->mode == MODE_WRITE) {
        totalSize += OsReadWriteFile(fd, attr->blockSize, attr->blockCount, false);
    }

    ret = close(fd);
    if (ret < 0) {
        printf("file close failed, %d, %s.\n", errno, strerror(errno));
    }

    OsDdPrintRate(startTime, g_modeTbl[attr->mode].translate, totalSize);
RELEASE_PATH:
    dpal_free(fullPath);
RELEASE_ATTR:
    (VOID)dpal_free(attr);
}

static unsigned int OsCreateDdTask(const DdCmdAttr *attr)
{
    unsigned int ret;
    unsigned int tid;
    dpal_task_attr_t task_attr;
    (void)memset_s(&task_attr, sizeof(task_attr), 0, sizeof(task_attr));
    task_attr.task_prio = LOSCFG_DD_TASK_PRIORITY;
    task_attr.stack_size = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task_attr.task_name = "DdAgent";
    task_attr.reserved = (void *)(uintptr_t)LOS_TASK_STATUS_DETACHED;
#ifdef CONFIG_KERNEL_SMP
    task_attr.task_cpuid = CPUID_TO_AFFI_MASK(dpal_get_curr_task());
#endif
    ret = dpal_task_create(&tid, &task_attr, (dpal_tsk_entry_func)OsDdProcess, (void *)attr);
    return ret;
}

int OsShellCmdDd(int argc, const char **argv)
{
    unsigned int ret;
    DdCmdAttr *ddCmd = NULL;
    if ((argc < 2) ||(argc > 4)) {
        printf("dd need at least 2 params to set filename and operate mode to run, and less than 4 params.\n");
        DD_USAGE();
        return DPAL_NOK;
    }

    ddCmd = dpal_malloc(sizeof(DdCmdAttr));
    if (ddCmd == NULL) {
        printf("no memory to used for dd\n");
        return DPAL_NOK;
    }

    (VOID)memset_s(ddCmd, sizeof(DdCmdAttr), 0, sizeof(DdCmdAttr));
    ret = OsParseOpt(argc, argv, ddCmd);
    if (ret != 0) {
        printf("opt parse error!\n");
        goto ERROR_PARSE;
    }

    ret = OsCreateDdTask(ddCmd);
    if (ret != 0) {
        printf("dd task create failed 0x%x\n", ret);
        goto ERROR_RELEASE_ATTR;
    }
    return 0;

ERROR_PARSE:
    DD_USAGE();
ERROR_RELEASE_ATTR:
    (VOID)dpal_free(ddCmd);
    return DPAL_NOK;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
