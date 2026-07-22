/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: software update by zdiag
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "errcode.h"
#include "upg.h"
#include "upg_common_porting.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"
#include "zdiag_adapt_layer.h"
#include "errcode.h"
#include "diag_update.h"

#define READ_FILE_LEN 0x100
#define TEST_FILE_PATH "/user/test/update.fwpkg"

STATIC errcode_t diag_cmd_update_prepare(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                         diag_option_t *option)
{
    unused(cmd_param_size);
    upg_prepare_info_t *prepare_info = (upg_prepare_info_t *)cmd_param;
    errcode_t ret = uapi_upg_prepare(prepare_info);
    if (ret != ERRCODE_SUCC) {
        printf("uapi_upg_prepare error = 0x%x\r\n", ret);
    } else {
        printf("uapi_upg_prepare OK!!\r\n");
    }
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, sizeof(ret), true);
}

STATIC errcode_t diag_cmd_update_request(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                         diag_option_t *option)
{
    unused(cmd_param_size);
    unused(cmd_param);
    errcode_t ret;

    ret = uapi_upg_request_upgrade(false);
    if (ret != ERRCODE_SUCC) {
        printf("uapi_upg_request_upgrade error = 0x%x\r\n", ret);
    } else {
        printf("uapi_upg_request_upgrade OK!!\r\n");
    }
    uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, (uint16_t)sizeof(errcode_t), true);
    return ret;
}

STATIC errcode_t diag_cmd_update_start(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    unused(cmd_param);
    errcode_t ret;
    do {
        ret = upg_image_backups_update();
        if (ret != ERRCODE_SUCC) {
            printf("upg_image_backups_update error = 0x%x\r\n", ret);
            break;
        }
        ret = uapi_upg_start();
        if (ret != ERRCODE_SUCC) {
            printf("uapi_update_start_test error = 0x%x\r\n", ret);
        } else {
            uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, (uint16_t)sizeof(errcode_t), true);
            printf("uapi_update_start_test OK!\r\n");
            upg_reboot();
        }
    } while (0);

    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, (uint16_t)sizeof(errcode_t), true);
}

STATIC errcode_t diag_cmd_update_get_max_size(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                              diag_option_t *option)
{
    unused(cmd_param_size);
    unused(cmd_param);
    uint32_t size = uapi_upg_get_storage_size();
    if (size != 0) {
        printf("uapi_upg_get_storage_size OK! size = 0x%x\r\n", size);
    } else {
        printf("uapi_update_start_test failed!\r\n");
    }

    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&size, (uint16_t)sizeof(uint32_t), true);
}

STATIC errcode_t diag_cmd_update_write_test_before_prepare(int32_t *read_fd, uint32_t *file_len, uint8_t **buffer)
{
    int32_t   fd;
    uint32_t  len;
    errcode_t ret;

    fd = open(TEST_FILE_PATH, O_RDONLY);
    if (fd < 0) {
        printf("open failed\r\n");
        return ERRCODE_FAIL;
    }

    struct stat file_info = {0};
    if (stat(TEST_FILE_PATH, &file_info) != 0) {
        printf("get stat failed\r\n");
        close(fd);
        return ERRCODE_FAIL;
    }
    len = file_info.st_size;
    if (len == 0) {
        close(fd);
        return ERRCODE_FAIL;
    }

    /* 升级准备工作 */
    printf("prepare start\r\n");
    upg_prepare_info_t prepare_info;
    prepare_info.package_len = len;
    ret = uapi_upg_prepare(&prepare_info);
    if (ret != ERRCODE_SUCC) {
        printf("prepare failed, ret = 0x%x\r\n", ret);
        close(fd);
        return ret;
    }

    *buffer = (uint8_t *)malloc(READ_FILE_LEN);
    if (*buffer == NULL) {
        printf("malloc failed\r\n");
        close(fd);
        return ERRCODE_MALLOC;
    }

    *read_fd = fd;
    *file_len = len;

    return ERRCODE_SUCC;
}

STATIC errcode_t diag_cmd_update_write_test(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                            diag_option_t *option)
{
    /* 前提：已经在测试路径使用hso上传升级包 */
    int32_t   read_fd;
    uint32_t  file_len;
    uint32_t  write_len;
    uint32_t  read_len;
    uint32_t  offset = 0;
    uint8_t  *buffer = NULL;
    errcode_t ret = ERRCODE_FAIL;

    ret = diag_cmd_update_write_test_before_prepare(&read_fd, &file_len, &buffer);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    printf("read and write start\r\n");
    
    while (file_len > 0) {
        write_len = uapi_min(READ_FILE_LEN, file_len);

        uint32_t seek_ret = (uint32_t)lseek(read_fd, offset, SEEK_SET);
        if (seek_ret != offset) {
            printf("seek failed\r\n");
            ret = ERRCODE_FAIL;
            break;
        }
        read_len = read(read_fd, buffer, write_len);
        if (read_len == 0) {
            printf("read failed\r\n");
            ret = ERRCODE_FAIL;
            break;
        }
        ret = uapi_upg_write_package_sync(offset, buffer, read_len);
        if (ret != ERRCODE_SUCC) {
            printf("write failed, ret = 0x%x\r\n", ret);
            break;
        }
        file_len -= read_len;
        offset += read_len;
    }

    free(buffer);
    close(read_fd);
    printf("write done, send\r\n");
    return uapi_diag_report_packet(cmd_id, option, (uint8_t *)&ret, (uint16_t)sizeof(errcode_t), true);
}

diag_cmd_reg_obj_t g_diag_update_cmd_tbl[] = {
    {DIAG_CMD_UPDATE_PREPARE,  DIAG_CMD_UPDATE_PREPARE,  diag_cmd_update_prepare},      /* update_prepare cmd */
    {DIAG_CMD_UPDATE_REQUEST,  DIAG_CMD_UPDATE_REQUEST,  diag_cmd_update_request},      /* update_request cmd */
    {DIAG_CMD_UPDATE_START,    DIAG_CMD_UPDATE_START,    diag_cmd_update_start},        /* update_start cmd */
    {DIAG_CMD_UPDATE_GET_SIZE, DIAG_CMD_UPDATE_GET_SIZE, diag_cmd_update_get_max_size}, /* update_get_size cmd */
    {DIAG_CMD_UPDATE_WRITE,    DIAG_CMD_UPDATE_WRITE,    diag_cmd_update_write_test},   /* update_write_package cmd */
};

errcode_t zdiag_update_init(void)
{
    return uapi_diag_register_cmd(g_diag_update_cmd_tbl,
        sizeof(g_diag_update_cmd_tbl) / sizeof(g_diag_update_cmd_tbl[0]));
}

