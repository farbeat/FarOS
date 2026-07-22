/**
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: utils
 * Author:
 * Create:
 */
#include "chip_exception_utils.h"
#include <stdbool.h>
#include "hal_reboot.h"
#include "chip_io.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"

#define STATUS_ADDRESS  0x5702C1E8
#define VBUS_INSERT_POS 4
#define PWREN_PRESS_POS 3

bool is_first_power_on(void)
{
    return hal_reboot_get_first_power_on_flag();
}

bool is_vbus_insert(void)
{
    if (reg32_getbit(STATUS_ADDRESS, VBUS_INSERT_POS)) {
        return true;
    }
    return false;
}

bool is_pwren_press(void)
{
    if (reg32_getbit(STATUS_ADDRESS, PWREN_PRESS_POS)) {
        return true;
    }
    return false;
}

static int32_t mkdir_all_path(const char *path)
{
    int path_len = (int)strlen(path);
    if (path_len <= 0) {
        return -1;
    }

    char *str_path = (char *)malloc((uint32_t)path_len + 1);
    if (str_path == NULL) {
        return -1;
    }
    (void)memset_s(str_path, (uint32_t)path_len + 1, 0, (uint32_t)path_len + 1);
    if (strcpy_s(str_path, (uint32_t)path_len + 1, path) != 0) {
        free(str_path);
        return -1;
    }

    for (int i = 0; i < path_len; i++) {
        if (i > 0 && str_path[i] == '/') {
            str_path[i] = '\0';
            if (access(str_path, 0) == 0) {
                str_path[i] = '/';
                continue;
            }
            if (mkdir(str_path, S_IREAD | S_IWRITE) != 0) {
                free(str_path);
                return -1;
            }
            str_path[i] = '/';
        }
    }
    free(str_path);
    return 0;
}

int32_t write_fileinfo(const char *path, uint32_t offset, const uint8_t *buf, uint32_t size)
{
    int fd;
    int ret;
    ssize_t len;
    fd = open(path, O_RDWR | O_CREAT, 0);
    if (fd < 0) {
        if (mkdir_all_path(path) != 0) {
            return -1;
        }
        fd = open(path, O_RDWR | O_CREAT, 0);
        if (fd < 0) {
            return -1;
        }
    }
    int pos = (int)lseek(fd, offset, SEEK_SET);
    if (pos < 0) {
        return -1;
    }
    len = write(fd, buf, size);
    if (len < 0) {
        return -1;
    }
    ret = close(fd);
    if (ret < 0) {
        return -1;
    }
    return len;
}