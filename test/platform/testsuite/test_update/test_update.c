/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test update source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#include "test_update.h"
#include "errcode.h"
#include "upg.h"
#ifndef CONFIG_MAIN_CORE
#include "soc_osal.h"
#endif
#ifdef CONFIG_MAIN_CORE
#include "soc_flash_map.h"
#include "upg_definitions.h"
#endif
#include "securec.h"
#include "test_suite_errors.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "upg_porting.h"
#include "upg_definitions.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "partition.h"
#include "upg_common_porting.h"
#include "memory_config_common.h"
#include "debug_print.h"
#include "chip_io.h"
#if UPG_CFG_SUPPORT_FILE_SYSTEM == YES
#include "mmc.h"
#endif
#include "test_suite_log.h"

#define VFS_BASE_NAME "/bin/vs/sd0p0/"
#define UPG_FILE_NAME_MAX_LEN 128
#define UPDATE_BUFF_SIZE 512

#ifdef CONFIG_MAIN_CORE
static fota_upgrade_flag_area_t *upg_flag_info = NULL;
static upg_package_header_t *pkg_header = NULL;
static upg_image_hash_node_t *img_hash_table = NULL;
static uint32_t image_num = 0;
static upg_image_header_t *img_header = NULL;
#endif
/**
 * @brief 以16进制打印buf
 * @param buf - 待打印的buf， uint8_t* 类型
 * @param size - 打印的长度，必须小于等于buf大小
 * @return 无
 */
static void print_buf_hex(uint8_t *buf, uint32_t size)
{
    test_suite_log_stringf("data:\r\n");
    const uint32_t p_tmp = 8; // 8 bit
    uint32_t tmp1 = size / p_tmp;
    uint32_t tmp2 = size % p_tmp;
    for (uint32_t i = 0; i < tmp1; i++) {
        test_suite_log_stringf("0x%-3x 0x%-3x 0x%-3x 0x%-3x 0x%-3x 0x%-3x 0x%-3x 0x%-3x\r\n",
            buf[i * p_tmp], buf[i * p_tmp + 1], buf[i * p_tmp + 2], buf[i * p_tmp + 3],
            buf[i * p_tmp + 4], buf[i * p_tmp + 5], buf[i * p_tmp + 6], buf[i * p_tmp + 7]);
        osal_msleep(1);
    }

    if (tmp2 != 0) {
        for (uint32_t i = 0; i < tmp2; i++) {
            test_suite_log_stringf("0x%-3x ", buf[i + tmp1 * p_tmp]);
        }
        test_suite_log_stringf("\r\n");
    }
    osal_msleep(1);
}

#ifndef CONFIG_MAIN_CORE
typedef struct upg_test_ctx {
    FILE *rd_fp;
    uint32_t rd_pos;
    uint32_t packge_len;
} upg_test_ctx_t;

static upg_test_ctx_t g_upg_ctx1 = {0};

static upg_test_ctx_t *upg_get_ctx(void)
{
    return &g_upg_ctx1;
}

#if UPG_CFG_SUPPORT_FILE_SYSTEM == YES
static errcode_t upg_check_buff(uint8_t *buff, uint16_t len)
{
    if (buff == NULL) {
        return ERRCODE_UPG_NULL_POINTER;
    }
    if (len == 0) {
        return ERRCODE_UPG_INVALID_BUFF_LEN;
    }
    return ERRCODE_SUCC;
}

static errcode_t upg_open_file_to_read(void)
{
    upg_test_ctx_t *ctx = upg_get_ctx();
    char full_name[UPG_FILE_NAME_MAX_LEN] = VFS_BASE_NAME;
    strcat_s(full_name, UPG_FILE_NAME_MAX_LEN, UPG_FILE_NAME);

    ctx->rd_fp = fopen(full_name, "rb");
    if (ctx->rd_fp == NULL) {
        return ERRCODE_UPG_FILE_OPEN_FAIL;
    }

    fseek(ctx->rd_fp, 0, SEEK_END);
    ctx->packge_len = ftell(ctx->rd_fp);
    if (ctx->packge_len == 0) {
        fclose(ctx->rd_fp);
        return ERRCODE_UPG_EMPTY_FILE;
    }
    fseek(ctx->rd_fp, 0, SEEK_SET);
    return ERRCODE_SUCC;
}

static errcode_t upg_package_storage_get(uint32_t offset, uint8_t *buff, uint16_t len)
{
    upg_test_ctx_t *ctx = NULL;
    errcode_t ret;
    uint16_t left_len = len;
    uint16_t read_len = 0;

    ret = upg_check_buff(buff, len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (offset == 0) {
        ret = upg_open_file_to_read();
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    ctx = upg_get_ctx();
    if (offset != ctx->rd_pos) {
        ret = ERRCODE_UPG_INVALID_OFFSET;
        goto end;
    }

    left_len = (ctx->rd_pos + left_len > ctx->packge_len) ? ctx->packge_len - ctx->rd_pos : left_len;
    while (left_len > 0) {
        read_len = fread(buff + read_len, 1, left_len, ctx->rd_fp);
        if (read_len == 0) {
            if (ferror(ctx->rd_fp)) {
                ret = ERRCODE_UPG_FILE_READ_FAIL;
                goto end;
            }
        }
        left_len -= read_len;
    }
    ctx->rd_pos = ftell(ctx->rd_fp);

end:
    if (ret != ERRCODE_SUCC || ctx->rd_pos >= ctx->packge_len) {
        fclose(ctx->rd_fp);
        ctx->rd_pos = 0;
        ctx->packge_len = 0;
    }
    return ret;
}

static errcode_t uapi_upg_package_storage_get(uint32_t offset, uint8_t *buff, uint16_t len)
{
    if (buff == NULL || len == 0) {
        return ERRCODE_UPG_INVALID_PARAMETER;
    }
    return upg_package_storage_get(offset, buff, len);
}

static int upg_get_file_length(void)
{
    int length = 0;
    FILE *rd_fd = NULL;
    char full_name[UPG_FILE_NAME_MAX_LEN] = VFS_BASE_NAME;
    strcat_s(full_name, UPG_FILE_NAME_MAX_LEN, UPG_FILE_NAME);

    rd_fd = fopen(full_name, "rb");
    if (rd_fd == NULL) {
        return -1;
    }

    fseek(rd_fd, 0, SEEK_END);
    length = ftell(rd_fd);
    if (length == 0) {
        fclose(rd_fd);
        return -1;
    }
    fclose(rd_fd);
    return length;
}

static int uapi_update_write_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    test_suite_log_stringf("===== test_update_write start. =====\n");
    int package_len = upg_get_file_length();
    if (package_len == -1) {
        test_suite_log_stringf("[test_update_api]upg_get_file_length fail,ret:%d.\n", package_len);
        return package_len;
    }
    test_suite_log_stringf("[test_update_api]upg_get_file_length success.\n");

    upg_prepare_info_t prepare_info = { package_len };
    errcode_t ret = uapi_upg_prepare(&prepare_info);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("[test_update_api]uapi_upg_prepare fail,ret:%x.\n", ret);
        return ret;
    }
    test_suite_log_stringf("[test_update_api]uapi_upg_prepare success.\n");

    uint32_t rd_loc = 0;
    uint32_t wr_loc = 0;
    uint8_t * buff = (uint8_t *)malloc(sizeof(uint8_t) * UPDATE_BUFF_SIZE);
    if (buff == NULL) {
        printf("Malloc failed.\n");
        return 1;
    }
    memset_s(buff, UPDATE_BUFF_SIZE, 0, UPDATE_BUFF_SIZE);
    while (package_len > 0) {
        ret = uapi_upg_package_storage_get(rd_loc, buff, UPDATE_BUFF_SIZE);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("[test_update_api]uapi_upg_package_storage_get fail,ret:%x.\n", ret);
            free(buff);
            return ret;
        }
        rd_loc = rd_loc + UPDATE_BUFF_SIZE;

        ret = uapi_upg_write_package_sync(wr_loc, buff, UPDATE_BUFF_SIZE);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("[test_update_api]uapi_upg_write_package_sync fail,ret:%x.\n", ret);
            free(buff);
            return ret;
        }
        wr_loc = wr_loc + UPDATE_BUFF_SIZE;
        package_len = package_len - UPDATE_BUFF_SIZE;
    }

    free(buff);
    buff = NULL;
    test_suite_log_stringf("===== test_update_write success. =====\n");
    return TEST_SUITE_OK;
}
#endif

static void upg_serial_putc(const char c)
{
    printf("%c", c);
}

static void *upg_func_kmalloc(const uint32_t size)
{
    return osal_kmalloc((unsigned long)size, OSAL_GFP_KERNEL);
}

static int uapi_update_init_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_flash_init(0);
    test_suite_log_stringf("flash init success\r\n");
    errcode_t ret = uapi_partition_init();
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("partition init failed. ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_log_stringf("partition init success\r\n");

    upg_func_t upg_func = {0};
    upg_func.malloc = (upg_func_malloc)malloc;
    upg_func.free = (upg_func_free)free;
    upg_func.serial_putc = NULL;
    errcode_t res = uapi_upg_init(&upg_func);
    if (res != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_init failed. ret = 0x%x\r\n", res);
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_log_stringf("upg init success\r\n");
    return TEST_SUITE_OK;
}

static int uapi_update_prepare_test(int argc, char *argv[])
{
    errcode_t ret;
    upg_prepare_info_t prepare_info;

    prepare_info.package_len = (uint32_t)strtol(argv[0], NULL, 0);
    ret = uapi_upg_prepare(&prepare_info);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_prepare error = 0x%x\r\n", ret);
    }
    return TEST_SUITE_OK;
}

/* 升级包临时存储区 */
#define UPG_PKG_REGION_PAGES 500
#define UPG_PKG_REGION_LENGTH (UPG_PKG_REGION_PAGES * FLASH_PAGE_SIZE)
/* 使用recovery升级, 升级包暂放在app区域的后半段空闲处 */
#define UPG_PKG_REGION_START (APP_FLASH_REGION_START + FLASH_PAGE_SIZE * 200)

static errcode_t upg_read_fota_pkg_data_from_flash(uint32_t read_offset, uint8_t *buffer, uint32_t *read_len)
{
    errcode_t ret_val;
    uint32_t start_addr = UPG_PKG_REGION_START - FLASH_START;
    uint32_t size = UPG_PKG_REGION_LENGTH;
    uint32_t actual_len;

    if (read_offset >= size || *read_len == 0) {
        test_suite_log_stringf("-------------------------%s  - %d------------------------\r\n", __FUNCTION__, __LINE__);
        test_suite_log_stringf("read_offset:%x, size:%x, *read_len:%x\r\n", read_offset, size, *read_len);
        return ERRCODE_UPG_INVALID_PARAMETER;
    }

    actual_len = ((read_offset + *read_len) > size) ? (size - read_offset) : *read_len;
    start_addr += read_offset;

    ret_val = upg_flash_read(start_addr, actual_len, (uint8_t *)buffer);
    if (ret_val != ERRCODE_SUCC) {
        test_suite_log_stringf("-------------------------%s  - %d------------------------\r\n", __FUNCTION__, __LINE__);
        return ret_val;
    }

    *read_len = actual_len;
    return ERRCODE_SUCC;
}

#if UPG_CONFIG_SUPPORT_FILE_SYSTEM == YES
static errcode_t upg_write_fota_pkg_data_to_emmc(uint32_t write_offset, uint8_t *buffer, uint32_t *write_len)
{
    int32_t fd;
    int32_t offset;
    int32_t len;
    errcode_t ret = ERRCODE_SUCC;
    char *path = UPG_FILE_NAME;
    fd = open(path, O_WRONLY | O_CREAT, 0777); /* 0777表示打开文件时的操作权限 */
    if (fd < 0) {
        test_suite_log_stringf("open upg file fail!: %s\r\n", (const uint8_t *)path);
        return ERRCODE_UPG_FILE_OPEN_FAIL;
    }

    offset = lseek(fd, write_offset, SEEK_SET);
    if (offset < 0) {
        ret = ERRCODE_UPG_FILE_READ_FAIL;
        test_suite_log_stringf("seek upg file fail!: ");
        goto end;
    }

    len = write(fd, buffer, *write_len);
    if (len <= 0) {
        ret = ERRCODE_UPG_FILE_READ_FAIL;
        test_suite_log_stringf("write upg file fail!: ");
        *write_len = 0;
        goto end;
    }
    *write_len = (uint32_t)len;
end:
    close(fd);
    return ret;
}

static uint8_t g_upg_flash_move[UPG_FLASH_PAGE_SIZE];
static uint8_t g_upg_flash_move_cmp[UPG_FLASH_PAGE_SIZE];
#define UPG_PKG_REAL_SIZE (780 * 1024) // 当前升级文件(ssb+recovery+app)760+KB
static errcode_t cmp_update_in_flash_and_emmc(void)
{
    errcode_t ret;
    uint32_t read_len = 0;
    uint32_t total_len = UPG_PKG_REAL_SIZE;
    uint32_t tmp_len = 0;
    uint8_t *buffer = g_upg_flash_move;
    uint8_t *buffer2 = g_upg_flash_move_cmp;
    while (read_len < total_len) {
        tmp_len = ((total_len - read_len) > UPG_FLASH_PAGE_SIZE) ?
            UPG_FLASH_PAGE_SIZE : (total_len - read_len);
        if (read_len % UPG_FLASH_PAGE_SIZE) { // TODO: 升级过程中期望读取4k，实际未读取这么多，会导致下一次跨页读取，从而读取失败，升级异常退出
            int tail = UPG_FLASH_PAGE_SIZE - read_len % UPG_FLASH_PAGE_SIZE;
            tmp_len = tail > tmp_len ? tmp_len : tail;
        }

        ret = upg_read_fota_pkg_data_from_flash(read_len, (uint8_t *)buffer, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("read failed , read off:%d, want read %d\r\n", read_len, tmp_len);
            return ret;
        }

        ret = upg_read_fota_pkg_data(read_len, (uint8_t *)buffer2, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("read file failed , read off:%d, want read %d\r\n", read_len, tmp_len);
            return ret;
        }

        if (memcmp(buffer, buffer2, tmp_len) != 0) {
            test_suite_log_stringf("memcmp failed, off:%d, cmp len:%d\r\n", read_len, tmp_len);
            return ERRCODE_FAIL;
        }

        read_len += tmp_len;
    }
    return ERRCODE_SUCC;
}

static errcode_t read_flash_update_to_emmc(void)
{
    uint32_t read_offset = 0;
    errcode_t ret;
    uint32_t read_len = 0;
    uint32_t total_len = UPG_PKG_REAL_SIZE;
    uint32_t tmp_len = 0;
    uint8_t *buffer = g_upg_flash_move;

    // 写之前先清除
    char *path = UPG_FILE_NAME;
    int32_t fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (fd < 0) {
        test_suite_log_stringf("open upg file fail!: %s\r\n", (const uint8_t *)path);
        return ERRCODE_UPG_FILE_OPEN_FAIL;
    }
    close(fd);
    test_suite_log_stringf("start read upgrade to emmc,imge len:%x \r\n", total_len);

    while (read_len < total_len) {
        tmp_len = ((total_len - read_len) > UPG_FLASH_PAGE_SIZE) ?
            UPG_FLASH_PAGE_SIZE : (total_len - read_len);
        if (read_len % UPG_FLASH_PAGE_SIZE) { // TODO: 升级过程中期望读取4k，实际未读取这么多，会导致下一次跨页读取，从而读取失败，升级异常退出
            uint32_t tail = UPG_FLASH_PAGE_SIZE - read_len % UPG_FLASH_PAGE_SIZE;
            tmp_len = tail > tmp_len ? tmp_len : tail;
        }
        ret = upg_read_fota_pkg_data_from_flash(read_offset + read_len, (uint8_t *)buffer, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("read failed , read offset:%x, len:%x\r\n", read_offset + read_len, tmp_len);
            return ret;
        }

        ret = upg_write_fota_pkg_data_to_emmc(read_len, (uint8_t *)buffer, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("write failed \r\n");
            return ret;
        }

        read_len += tmp_len;
    }
    test_suite_log_stringf("read upgrade to emmc finish\r\n");
    test_suite_log_stringf("+++++++++++++++++++++++++++++++++++++\r\n");
    test_suite_log_stringf("start cmp upgrade betwwen flash and emmc\r\n");
    ret = cmp_update_in_flash_and_emmc();
    test_suite_log_stringf("cmp finish\r\n");
    test_suite_log_stringf("+++++++++++++++++++++++++++++++++++++\r\n");
    return ret;
}

static int uapi_update_download_test(int argc, char *argv[])
{
    test_suite_log_stringf("try write update file copied from flash to emmc.\r\n");
    if (read_flash_update_to_emmc() != ERRCODE_SUCC) {
        test_suite_log_stringf("read_flash_update_to_emmc failed.\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_log_stringf("++++++++++++++++ download update file success.++++++++++++++\r\n");
    return TEST_SUITE_OK;
}
#endif

static void print_pkg_header(upg_package_header_t *pkg_header)
{
    test_suite_log_stringf("-------------------------%s  - %d---------------------------\r\n", __FUNCTION__, __LINE__);
    test_suite_log_stringf("+++++++++++++++++++++++fota_key_area:+++++++++++++++++++\r\n");
    test_suite_log_stringf("image_id: %x\r\n", pkg_header->key_area.image_id);
    test_suite_log_stringf("struct_version: %x\r\n", pkg_header->key_area.struct_version);
    test_suite_log_stringf("struct_length: %x\r\n", pkg_header->key_area.struct_length);
    test_suite_log_stringf("signature_length: %x\r\n", pkg_header->key_area.signature_length);
    test_suite_log_stringf("key_owner_id: %x\r\n", pkg_header->key_area.key_owner_id);
    test_suite_log_stringf("key_id: %x\r\n", pkg_header->key_area.key_id);
    test_suite_log_stringf("key_alg: %x\r\n", pkg_header->key_area.key_alg);
    test_suite_log_stringf("ecc_curve_type: %x\r\n", pkg_header->key_area.ecc_curve_type);
    test_suite_log_stringf("key_length: %x\r\n", pkg_header->key_area.key_length);
    test_suite_log_stringf("fota_key_version_ext: %x\r\n", pkg_header->key_area.fota_key_version_ext);
    test_suite_log_stringf("mask_fota_key_version_ext: %x\r\n", pkg_header->key_area.mask_fota_key_version_ext);
    test_suite_log_stringf("msid_ext: %x\r\n", pkg_header->key_area.msid_ext);
    test_suite_log_stringf("mask_msid_ext: %x\r\n", pkg_header->key_area.mask_msid_ext);
    test_suite_log_stringf("maintenance_mode: %x\r\n", pkg_header->key_area.maintenance_mode);
    test_suite_log_stringf("fota_info_addr: %x\r\n", pkg_header->key_area.fota_info_addr);
    test_suite_log_stringf("\r\n+++++++++++++++++++++++fota_info_area:+++++++++++++++++++\r\n");
    test_suite_log_stringf("image_id: %x\r\n", pkg_header->info_area.image_id);
    test_suite_log_stringf("struct_version: %x\r\n", pkg_header->info_area.struct_version);
    test_suite_log_stringf("struct_length: %x\r\n", pkg_header->info_area.struct_length);
    test_suite_log_stringf("signature_length: %x\r\n", pkg_header->info_area.signature_length);
    test_suite_log_stringf("fota_version_ext: %x\r\n", pkg_header->info_area.fota_version_ext);
    test_suite_log_stringf("mask_fota_version_ext: %x\r\n", pkg_header->info_area.mask_fota_version_ext);
    test_suite_log_stringf("msid_ext: %x\r\n", pkg_header->info_area.msid_ext);
    test_suite_log_stringf("mask_msid_ext: %x\r\n", pkg_header->info_area.mask_msid_ext);
    test_suite_log_stringf("image_hash_table_addr: %x\r\n", pkg_header->info_area.image_hash_table_addr);
    test_suite_log_stringf("image_hash_table_length: %x\r\n", pkg_header->info_area.image_hash_table_length);
    test_suite_log_stringf("image_num: %x\r\n", pkg_header->info_area.image_num);
    test_suite_log_stringf("hardware_id: %x\r\n", pkg_header->info_area.hardware_id);
}

static void print_image_head(upg_image_header_t *img_header, uint32_t image_num)
{
    test_suite_log_stringf("-------------------------%s  - %d---------------------------\r\n", __FUNCTION__, __LINE__);
    test_suite_log_stringf("\r\n++++++++++++++++ print_image_head [%d] +++++++++++++++\r\n", image_num);
    test_suite_log_stringf("header_magic:%x\r\n", img_header->header_magic);
    test_suite_log_stringf("image_id:%x\r\n", img_header->image_id);
    test_suite_log_stringf("image_offset:%x\r\n", img_header->image_offset);
    test_suite_log_stringf("image_len:%x\r\n", img_header->image_len);
    test_suite_log_stringf("new_image_len:%x\r\n", img_header->new_image_len);
    test_suite_log_stringf("old_image_len:%x\r\n", img_header->old_image_len);
    test_suite_log_stringf("image_hash:%x\r\n", img_header->image_hash);
    test_suite_log_stringf("version_ext:%x\r\n", img_header->version_ext);
    test_suite_log_stringf("version_mask:%x\r\n", img_header->version_mask);
    test_suite_log_stringf("decompress_flag:%x\r\n", img_header->decompress_flag);
    test_suite_log_stringf("root_key_type:%x\r\n", img_header->root_key_type);
    test_suite_log_stringf("re_enc_flag:%x\r\n", img_header->re_enc_flag);
    test_suite_log_stringf("\r\n+++++++++++++++++++++++++++++++\r\n");
}

static int uapi_update_print_head_test(int argc, char *argv[])
{
    upg_package_header_t *pkg_header = NULL;
    upg_image_hash_node_t *img_hash_table = NULL;
    errcode_t ret;
    int32_t image_index;
    uint32_t image_num;
    do {
        ret = upg_get_package_header(&pkg_header);
        if (ret != ERRCODE_SUCC || pkg_header == NULL) {
            test_suite_log_stringf("upg_get_package_header fail\r\n");
            goto end;
        }
        print_pkg_header(pkg_header);
        ret = upg_get_pkg_image_hash_table(pkg_header, &img_hash_table);
        if (ret != ERRCODE_SUCC || img_hash_table == NULL) {
            test_suite_log_stringf("upg_get_pkg_image_hash_table fail\r\n");
            goto end;
        }
        image_num = pkg_header->info_area.image_num;
        for (image_index = 0; image_index < (int32_t)image_num; image_index++) {
            upg_image_header_t *img_header = NULL;
            ret = upg_get_pkg_image_header((const upg_image_hash_node_t *)&(img_hash_table[image_index]), &img_header);
            if (ret != ERRCODE_SUCC || img_header == NULL) {
                test_suite_log_stringf("upg_get_pkg_image_header fail");
                goto end;
            }
            print_image_head(img_header, image_index);
            upg_free(img_header);
        }
    } while (0);

end:
    upg_free(img_hash_table);
    upg_free(pkg_header);
    return ret;
}

static int uapi_update_request_test(int argc, char *argv[])
{
    errcode_t ret;

    unused(argc);
    unused(argv);
    ret = uapi_upg_request_upgrade(false);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_request_upgrade error = 0x%x\r\n", ret);
    } else {
        test_suite_log_stringf("uapi_upg_request_upgrade OK!!\r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_update_start_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret = upg_image_backups_update();
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_image_backups_update failed, ret:%x\r\n", ret);
        return ret;
    } else {
        test_suite_log_stringf("upg_image_backups_update success\r\n");
    }

    ret = uapi_upg_start();
    // 1, 没有升级包、或者升级标记区结果已经设置的情况不需要重启
    // 2, 升级模块没有初始化不需要重启
    // 3, 升级成功不需要重启
    // 4, 升级失败的时候需要重启重新进入升级流程，尝试三次未成功，后会变为1的情况
    test_suite_log_stringf("--------------------------\r\n");
    if (!(ret == ERRCODE_UPG_NOT_NEED_TO_UPDATE || ret == ERRCODE_UPG_NOT_INIT || ret == ERRCODE_SUCC)) {
        test_suite_log_stringf("upgrade failed, ret:%x\r\n", ret);
    } else {
        test_suite_log_stringf("upgrade success\r\n");
    }
    return ret;
}

static int uapi_upg_flash_test(int argc, char *argv[])
{
    test_suite_log_stringf("uapi_upg_flash_test start. \r\n");
    uint32_t start_address;
    errcode_t ret = upg_get_upgrade_flag_flash_start_addr(&start_address);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_get_upgrade_flag_flash_start_addr failed. ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("flag start_address = 0x%x, erase first\r\n", start_address);
    ret = upg_flash_erase(start_address, UPG_FLASH_PAGE_SIZE);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_flash_erase flag failed. ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    fota_upgrade_flag_area_t flag = {0};
    ret = upg_flash_read(start_address, sizeof(fota_upgrade_flag_area_t), (uint8_t *)&flag);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_flash_read flag failed. ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t size = sizeof(flag.firmware_flag);
    uint32_t flash_offset = (uint32_t)start_address + offsetof(fota_upgrade_flag_area_t, firmware_flag);
    test_suite_log_stringf("write loop, write size:%d. offset:%x\r\n--------------------------------------\r\n",
                           size, flash_offset);
    for (uint32_t i = 0; i < UPG_FIRMWARE_MAX_NUM; i++) {
        uint32_t addr = flash_offset + i * UPG_FLAG_RETYR_TIMES;
        uint8_t start_flag = 0xf;
        uint8_t write_len = 1;
        uint32_t real_val = 0;
        for (uint32_t j = 0; j < UPG_FLAG_RETYR_TIMES; j++) {
            addr = flash_offset + i * UPG_FLAG_RETYR_TIMES + j * write_len;
            test_suite_log_stringf("write start flag, write addr:%x.\r\n", addr);
            ret = upg_flash_write(addr, write_len, (uint8_t *)&(start_flag), false);
            test_suite_log_stringf("read start flag, read addr:%x. ret:%x\r\n", addr, ret);

            ret |= upg_flash_read(addr, write_len, (uint8_t *)&(real_val));
            test_suite_log_stringf(" read val: %x, ret:%x\r\n", (uint8_t)real_val, ret);
            if (ret != ERRCODE_SUCC) {
                test_suite_log_stringf("upg_flash_write upgrader flag fail. ret = %x", ret);
                return ret;
            }
        }
        addr = flash_offset + i * UPG_FLAG_RETYR_TIMES;
        write_len = 3;
        uint8_t finish_flag[3] = {0, 0, 0};
        test_suite_log_stringf("write finish flag, write addr:%x.\r\n", addr);
        ret = upg_flash_write(addr, write_len, (uint8_t *)&(finish_flag), false);
        test_suite_log_stringf("read finish flag, read addr:%x. ret:%x\r\n", addr, ret);
        ret |= upg_flash_read(addr, write_len, (uint8_t *)&(real_val));
        test_suite_log_stringf(" read val: %x,  ret:%x\r\n", real_val & 0xFFFFFF, ret);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("upg_flash_write upgrader flag fail. ret = %x", ret);
            return ret;
        }
    }
    return ERRCODE_SUCC;
}

static int uapi_upg_flag_print(int argc, char *argv[])
{
    uint32_t addr;
    errcode_t  ret = upg_get_upgrade_flag_flash_start_addr(&addr);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_get_upgrade_flag_flash_start_addr fail. ret = %x", ret);
        return ret;
    }
    fota_upgrade_flag_area_t upg_flag_info;
    ret = upg_flash_read(addr, sizeof(fota_upgrade_flag_area_t), (uint8_t *)(&upg_flag_info));
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_flash_read flag fail. ret = %x", ret);
        return ret;
    }

    printf("head_magic: 0x%x\r\n", upg_flag_info.head_magic);
    printf("head_before_offset: 0x%x\r\n", upg_flag_info.head_before_offset);
    printf("package_length: 0x%x\r\n", upg_flag_info.package_length);
    printf("firmware_num: 0x%x\r\n", upg_flag_info.firmware_num);
    for (uint32_t i = 0; i < upg_flag_info.firmware_num; i++) {
        printf("firmware_flag[%d]: 0x%x\r\n", i, *((uint32_t *)upg_flag_info.firmware_flag[i]));
    }

    printf("nv_flag: 0x%x\r\n", *((uint32_t *)upg_flag_info.nv_flag));
    printf("update_result: 0x%x\r\n", upg_flag_info.update_result);
    printf("nv_data_offset: 0x%x\r\n", upg_flag_info.nv_data_offset);
    printf("nv_data_len: 0x%x\r\n", upg_flag_info.nv_data_len);
    printf("nv_hash_offset: 0x%x\r\n", upg_flag_info.nv_hash_offset);
    printf("nv_hash_len: 0x%x\r\n", upg_flag_info.nv_hash_len);
    printf("complete_flag: 0x%x\r\n", upg_flag_info.complete_flag);
    printf("head_end_magic: 0x%x\r\n", upg_flag_info.head_end_magic);
    return ERRCODE_SUCC;
}

static int uapi_upg_backup_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret = upg_image_backups_update();
    test_suite_log_stringf("--------------------------\r\n");
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_image_backups_update failed, ret:%x\r\n", ret);
    } else {
        test_suite_log_stringf("upg_image_backups_update success\r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_update_result_test(int argc, char *argv[])
{
    errcode_t ret;
    unused(argc);
    unused(argv);
    upg_result_t upg_result;
    uint32_t last_image_index;
    ret = uapi_upg_get_result(&upg_result, &last_image_index);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_get_result error = 0x%x\r\n", ret);
    } else {
        test_suite_log_stringf("upg_result = %d, last_image_index:%d\r\n", upg_result, last_image_index);
        test_suite_log_stringf("uapi_upg_get_result OK!!\r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_update_storage_max_size_test(int argc, char *argv[])
{
    errcode_t ret;

    unused(argc);
    unused(argv);
    uint32_t storage_max_size = uapi_upg_get_storage_size();
    if (storage_max_size == 0) {
        test_suite_log_stringf("uapi_upg_get_storage_size error size = 0\r\n");
    } else {
        test_suite_log_stringf("storage_max_size = %u\r\n", storage_max_size);
        test_suite_log_stringf("uapi_upg_get_storage_size OK!!\r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_upg_read_test(int argc, char *argv[])
{
    if (argc != 4) {
        test_suite_log_stringf("please input 4  param: case, len(len < 65535), offset and for_num!\r\n");
        test_suite_log_stringf("\tfor example: test uapi_upg_read 0 32 0 16 \r\n");
        return ERRCODE_FAIL;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t ret = ERRCODE_SUCC;
    uint16_t len = strtol(argv[1], NULL, 0);
    uint32_t rd_loc = strtol(argv[2], NULL, 0);
    uint16_t num = strtol(argv[3], NULL, 0);
    uint8_t *buff = NULL;
    uint16_t malloc_len = len + 1; // 防止输入len为0
    buff = (uint8_t *)osal_kmalloc(sizeof(uint8_t) * malloc_len, OSAL_GFP_KERNEL);
    if (buff == NULL) {
        osal_kfree(buff);
        return TEST_SUITE_ALLOC_FAILED;
    }
    memset_s(buff, malloc_len, 0, malloc_len);

    switch (param) {
        case 0:
            ret = uapi_upg_read_package(rd_loc, buff, len);
            if (ret != ERRCODE_SUCC) {
                test_suite_log_stringf("---test fail ---\r\n");
            } else {
                print_buf_hex(buff, len);
            }
            break;
        case 1:
            while (rd_loc < num * len) {
                ret = uapi_upg_read_package(rd_loc, buff, len);
                if (ret != ERRCODE_SUCC) {
                    test_suite_log_stringf("---test fail ---\r\n");
                } else {
                    print_buf_hex(buff, len);
                    rd_loc += len;
                }
            }
            break;
        case 2:
            ret = uapi_upg_read_package(rd_loc, buff, len);
            if (ret != ERRCODE_SUCC) {
                test_suite_log_stringf("---test fail ---\r\n");
            }
            break;
        case 3:
            ret = uapi_upg_read_package(rd_loc, NULL, len);
            if (ret != ERRCODE_SUCC) {
                test_suite_log_stringf("---test fail ---\r\n");
            }
            break;
        default:
            break;
    }

    osal_kfree(buff);
    return ret;
}
#endif
#ifdef CONFIG_MAIN_CORE
// anti rollback 回滚检查
uint32_t upg_get_otp_addr(uint32_t image_id, uint32_t *start_addr)
{
    switch (image_id) {
        case REE_BOOT_IMAGE_ID:
            *start_addr = OTP_REE_BOOT_VERSION;
            break;
        case REE_APP_IMAGE_ID:
            *start_addr = OTP_REE_APP_VERSION;
            break;
        default:
            return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

uint32_t upg_get_board_version_mask(uint32_t image_id, uint32_t *key_mask, uint32_t *code_mask)
{
    uint32_t addr;
    uint32_t size;
    errcode_t ret = upg_get_partition_info(image_id, &addr, &size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    image_sign_info *sign_info = (image_sign_info *)(upg_get_flash_base_addr() + addr);
    *key_mask = sign_info->key_area.mask_key_version_ext;
    *code_mask = sign_info->code_info_area.mask_version_ext;
    return ERRCODE_SUCC;
}

uint32_t upg_get_board_version(uint32_t image_id, uint32_t *key_ver, uint32_t *code_ver)
{
    uint32_t addr;
    uint32_t size;
    errcode_t ret = upg_get_partition_info(image_id, &addr, &size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    image_sign_info *sign_info = (image_sign_info *)(upg_get_flash_base_addr() + addr);
    *key_ver = sign_info->key_area.key_version_ext;
    *code_ver = sign_info->code_info_area.version_ext;
    return ERRCODE_SUCC;
}

uint32_t upg_get_board_rollback_version(uint32_t image_id, uint32_t *anti_rollback_ver)
{
    uint32_t opt_offset;
    errcode_t ret = upg_get_otp_addr(image_id, &opt_offset);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    ret = uapi_drv_otp_read_word(
        opt_offset, (uint32_t *)anti_rollback_ver, (opt_offset) ^ ((uintptr_t)anti_rollback_ver));
    if (ret != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static int uapi_upg_get_anti_rollback_ver(int argc, char *argv[])
{
    if (argc != 1) {
        test_suite_log_stringf("please input 1  param: image id\r\n");
        test_suite_log_stringf("\tfor example: test uapi_upg_read 12345678 \r\n");
        return ERRCODE_FAIL;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t image_id = strtol(argv[0], NULL, 0);

    uint32_t key_mask;
    uint32_t code_mask;
    uint32_t key_ver;
    uint32_t code_ver;
    uint32_t anti_rollback_ver;
    uint32_t ret = upg_get_board_version_mask(image_id, &key_mask, &code_mask);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("---upg_get_board_version_mask fail ---\r\n");
    }
    ret = upg_get_board_version(image_id, &key_ver, &code_ver);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("---upg_get_board_version_mask fail ---\r\n");
    }
    ret = upg_get_board_rollback_version(image_id, &anti_rollback_ver);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("---upg_set_board_rollback_version fail ---\r\n");
    }

    printf("image_id = 0x%X \r\n", image_id);
    printf("key_ver = 0x%X \r\n", key_ver);
    printf("key_mask = 0x%X \r\n", key_mask);
    printf("code_ver = 0x%X \r\n", code_ver);
    printf("key_mask = 0x%X \r\n", code_mask);
    printf("anti_rollback_ver = 0x%X \r\n", anti_rollback_ver);

    return ERRCODE_SUCC;
}
static int uapi_upg_verify_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret;
    upg_package_header_t *pkg_header = NULL;
    ret = upg_get_package_header(&pkg_header);
    if (ret != ERRCODE_SUCC || pkg_header == NULL) {
        upg_msg0("upg_get_package_header fail\r\n");
    }
    ret = uapi_upg_verify_file((const upg_package_header_t *)pkg_header);
    if (ret != ERRCODE_SUCC) {
        upg_msg0("uapi_upg_verify_file fail!!\r\n");
    }
    return ERRCODE_SUCC;
}
static int uapi_upg_file_len(int argc, char *argv[])
{
    errcode_t ret;
    upg_prepare_info_t prepare_info;

    prepare_info.package_len = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t storage_max_size = uapi_upg_get_storage_size();
    upg_msg1("storage_max_size = ", storage_max_size);
    if (storage_max_size == 0) {
        test_suite_log_stringf("uapi_upg_get_storage_size error size = 0\r\n");
    }
    if (prepare_info.package_len > storage_max_size) {
        test_suite_log_stringf("upg size is too big ! \r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_upg_alloc_and_get_upgrade_flag_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret;
    if (upg_is_inited() == false) {
        ret = ERRCODE_UPG_NOT_INIT;
        test_suite_log_stringf("upg_is_inited fail\r\n");
        return TEST_SUITE_OK;
    }
    ret = upg_alloc_and_get_upgrade_flag(&upg_flag_info);
    if (ret != ERRCODE_SUCC || upg_flag_info == NULL) {
        test_suite_log_stringf("upg_alloc_and_get_upgrade_flag fail\r\n");
    } else {
        test_suite_log_stringf("upg_alloc_and_get_upgrade_flag success\r\n");
        upg_print_flag(upg_flag_info);
    }
    return TEST_SUITE_OK;
}

static int uapi_upg_get_package_header_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret;
    ret = upg_get_package_header(&pkg_header);
    if (ret != ERRCODE_SUCC || pkg_header == NULL) {
        test_suite_log_stringf("upg_get_package_header fail\r\n");
    } else {
        test_suite_log_stringf("upg_get_package_header success\r\n");
    }
    return TEST_SUITE_OK;
}

static int uapi_upg_get_pkg_image_header_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret = ERRCODE_SUCC;
    uint32_t image_num = pkg_header->info_area.image_num;
    int32_t img_index;
    int32_t fw_index;

    ret = upg_get_pkg_image_hash_table((const upg_package_header_t *)pkg_header, &img_hash_table);
    if (ret != ERRCODE_SUCC || img_hash_table == NULL) {
        test_suite_log_stringf("upg_get_pkg_image_hash_table fail\r\n");
        return ret;
    }
    for (img_index = 0, fw_index = 0; img_index < (int32_t)image_num; img_index++, fw_index++) {
        /* 获取Image Header */
        ret = upg_get_pkg_image_header(&(img_hash_table[img_index]), &img_header);
        if (ret != ERRCODE_SUCC || img_header == NULL) {
            test_suite_log_stringf("upg_get_pkg_image_header fail\r\n");
        } else {
            test_suite_log_stringf("upg_get_pkg_image_header success\r\n");
        }
    }
    return TEST_SUITE_OK;
}

static int uapi_upg_compress_image_update_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret;
    upg_image_header_t *image = (upg_image_header_t*)img_header;
    ret = uapi_upg_compress_image_update(img_header);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_compress_image_update fail\r\n");
        return ret;
    }

    uint32_t read_offset = image->image_offset;
    uint32_t app_address = 0;
    uint32_t app_size = 0;
    uint32_t read_len = 0;
    uint32_t tmp_len = 0;
    ret = upg_get_partition_info(image->image_id, &app_address, &app_size);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_get_partition_info fail\r\n");
        return ret;
    }
    print_buf_hex(app_address, 4);
    test_suite_log_stringf("%d\r\n", app_size);
    uint8_t *buffer = upg_malloc(UPG_FLASH_PAGE_SIZE);
    if (buffer == NULL) {
        test_suite_log_stringf("upg_malloc fail\r\n");
        return ERRCODE_MALLOC;
    }
    while (read_len < image->image_len) {
        tmp_len = ((image->image_len - read_len) > UPG_FLASH_PAGE_SIZE) ?
            UPG_FLASH_PAGE_SIZE : (image->image_len - read_len);
        ret = upg_read_fota_pkg_data(read_offset + read_len, (uint8_t *)buffer, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            upg_free(buffer);
            return ret;
        }
        print_buf_hex(buffer, (uint32_t)UPG_FLASH_PAGE_SIZE);
    }
    upg_print_flag(upg_flag_info);
    test_suite_log_stringf("compress upgrade success\r\n");
    return TEST_SUITE_OK;
}

static int uapi_upg_full_image_update_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret;
    upg_image_header_t *image = (upg_image_header_t*)img_header;
    ret = uapi_upg_full_image_update(image);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("uapi_upg_full_image_update fail\r\n");
        return ret;
    } else {
        test_suite_log_stringf("uapi_upg_full_image_update success\r\n");
    }

    uint32_t read_offset = image->image_offset;
    uint32_t app_address = 0;
    uint32_t app_size = 0;
    uint32_t read_len = 0;
    uint32_t tmp_len = 0;
    ret = upg_get_partition_info(image->image_id, &app_address, &app_size);
    if (ret != ERRCODE_SUCC) {
        test_suite_log_stringf("upg_get_partition_info fail\r\n");
        return ret;
    } else {
        test_suite_log_stringf("upg_get_partition_info success\r\n");
    }
    print_buf_hex(app_address, 4);
    test_suite_log_stringf("%d\r\n", app_size);
    uint8_t *buffer = upg_malloc(UPG_FLASH_PAGE_SIZE);
    if (buffer == NULL) {
        test_suite_log_stringf("upg_malloc fail\r\n");
        return ERRCODE_MALLOC;
    } else {
        test_suite_log_stringf("upg_malloc success\r\n");
    }
    while (read_len < image->image_len) {
        tmp_len = ((image->image_len - read_len) > UPG_FLASH_PAGE_SIZE) ?
            UPG_FLASH_PAGE_SIZE : (image->image_len - read_len);
        ret = upg_read_fota_pkg_data(read_offset + read_len, (uint8_t *)buffer, &tmp_len);
        if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("upg_read_fota_pkg_data fail\r\n");
            upg_free(buffer);
            return ret;
        } else {
            test_suite_log_stringf("upg_read_fota_pkg_data success\r\n");
        }
        print_buf_hex(buffer, (uint32_t)UPG_FLASH_PAGE_SIZE);
    }
    upg_print_flag(upg_flag_info);
    test_suite_log_stringf("fall upgrade success\r\n");
    return TEST_SUITE_OK;
}

#endif
/* *
 * Adds the update functions to the test suite.
 */
void add_test_update(void)
{
#ifndef CONFIG_MAIN_CORE
    uapi_test_suite_add_function("uapi_upg_init", "update init", uapi_update_init_test);
    uapi_test_suite_add_function("uapi_upg_prepare", "update prepare", uapi_update_prepare_test);
    uapi_test_suite_add_function("uapi_upg_print_head", "update print head", uapi_update_print_head_test);
    uapi_test_suite_add_function("uapi_upg_request", "update request", uapi_update_request_test);
#ifndef TARGET_CHIP_SOCMN1
    uapi_test_suite_add_function("uapi_upg_start", "update start", uapi_update_start_test);
#endif
    uapi_test_suite_add_function("uapi_upg_result", "test update get result", uapi_update_result_test);
    uapi_test_suite_add_function(
        "uapi_upg_storage_max_size", "test update get storage max size", uapi_update_storage_max_size_test);

    uapi_test_suite_add_function("uapi_upg_flash_test", "update flash test", uapi_upg_flash_test);
    uapi_test_suite_add_function("uapi_upg_flag_print", "update flag print", uapi_upg_flag_print);
    uapi_test_suite_add_function("uapi_upg_backup_test", "update backup test", uapi_upg_backup_test);
#if UPG_CFG_SUPPORT_FILE_SYSTEM == YES
    uapi_test_suite_add_function("uapi_upg_download", "update download", uapi_update_download_test);
    uapi_test_suite_add_function("uapi_upg_write", "test update api process.", uapi_update_write_test);
    uapi_test_suite_add_function("uapi_upg_read", "test uapi_upg_read.", uapi_upg_read_test);
#endif

#endif
#ifdef CONFIG_MAIN_CORE
    uapi_test_suite_add_function(
        "uapi_upg_get_anti_rollback_ver", "test update get anti rollback ver", uapi_upg_get_anti_rollback_ver);
    uapi_test_suite_add_function(
        "uapi_upg_compress_image_update", " test update compress test", uapi_upg_compress_image_update_test);
    uapi_test_suite_add_function("uapi_upg_verify", "test update verify", uapi_upg_verify_test);
    uapi_test_suite_add_function("uapi_upg_file_size", "test update file size", uapi_upg_file_len);
    uapi_test_suite_add_function(
        "uapi_upg_alloc_and_get_upgrade_flag", " test update flag test", uapi_upg_alloc_and_get_upgrade_flag_test);
    uapi_test_suite_add_function(
        "uapi_upg_get_package", " test update get package test", uapi_upg_get_package_header_test);
    uapi_test_suite_add_function(
        "uapi_upg_get_pkg_image_header", " test update image test", uapi_upg_get_pkg_image_header_test);
    uapi_test_suite_add_function(
        "uapi_upg_compress_image_update", " test update compress test", uapi_upg_compress_image_update_test);
    uapi_test_suite_add_function(
        "uapi_upg_full_image_update", " test update full image test", uapi_upg_full_image_update_test);
#endif
}
