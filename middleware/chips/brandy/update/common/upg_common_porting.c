/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: UPG common functions for different chip
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "common_def.h"
#include "upg_debug.h"
#include "partition.h"
#include "securec.h"
#include "flash.h"
#include "upg_porting.h"
#include "hal_reboot.h"
#include "tcxo.h"
#include "non_os_reboot.h"
#include "non_os.h"
#include "cpu_utils.h"
#if (UPG_CFG_VERIFICATION_SUPPORT == YES)
#include "efuse.h"
#endif
#include "otp_map.h"
#include "pal_sec_config_port.h"
#include "upg_alloc.h"
#include "upg_config.h"
#include "upg_definitions_porting.h"
#include "upg_common.h"
#include "upg_common_porting.h"

#define MS_ID_ADDR                         0xF8
#define CHIP_ID_ADDR                       OTP_CHIP_ID_START
#define SUB_RSA4096_PUBKEY_N_OFFSET        24

#define uapi_array_size(_array)  (sizeof(_array) / sizeof((_array)[0]))

static upg_image_partition_ids_map_t g_img_partition_map[] = {
    {SSB_IMAGE_ID, PARTITION_SSB_IMAGE},
    {SSB_SIGNATURE_IMAGE_ID, PARTITION_SSB_IMAGE_SIGNATURE},
    {RECOVERY_IMAGE_ID, PARTITION_RECOVERY_IMAGE},
    {RECOVERY_SIGNATURE_IMAGE_ID, PARTITION_RECOVERY_IMAGE_SIGNATURE},
    {APPLICATION_IMAGE_ID, PARTITION_ACPU_IMAGE},
    {APPLICATION_SIGNATURE_IMAGE_ID, PARTITION_ACPU_IMAGE_SIGNATURE},
    {BT_IMAGE_ID, PARTITION_BT_IMAGE},
    {BT_SIGNATURE_IMAGE_ID, PARTITION_BT_IMAGE_SIGNATURE},
    {DSP_MAIN_IMAGE_ID, PARTITION_DSP_MAIN_IMAGE},
    {DSP_MAIN_SIGNATURE_IMAGE_ID, PARTITION_DSP_MAIN_IMAGE_SIGNATURE},
    {DSP_OVERLAY_IMAGE_ID, PARTITION_DSP_OVERLAY_IMAGE},
    {DSP_OVERLAY_SIGNATURE_IMAGE_ID, PARTITION_DSP_OVERLAY_IMAGE_SIGNATURE},
};

/* 获取镜像ID和分区ID的map表
 * map: 镜像ID和分区ID的映射表
 * 返回map表中映射数量
 */
uint32_t upg_get_ids_map(upg_image_partition_ids_map_t **map)
{
    *map = g_img_partition_map;
    return uapi_array_size(g_img_partition_map);
}

/*
 * recovery/APP支持升级的镜像ID
 * 注意增删ID时同步更新升级ID的数量
 */
#if defined BUILD_APPLICATION_STANDARD && defined BUILD_RECOVERY_IMAGE
#define UPDATE_IMAGE_SET \
    { APPLICATION_SIGNATURE_IMAGE_ID, APPLICATION_IMAGE_ID, \
      BT_SIGNATURE_IMAGE_ID, BT_IMAGE_ID, \
      DSP_MAIN_SIGNATURE_IMAGE_ID, DSP_MAIN_IMAGE_ID, \
      DSP_OVERLAY_SIGNATURE_IMAGE_ID, DSP_OVERLAY_IMAGE_ID, \
      UPG_IMAGE_ID_NV, \
      UPG_IMAGE_ID_RES_INDEX, UPG_IMAGE_ID_RES_DATA}
#define UPDATE_IMAGE_SET_CNT 11 /* 镜像ID列表长度 */

#else
#define UPDATE_IMAGE_SET \
    { RECOVERY_SIGNATURE_IMAGE_ID, RECOVERY_IMAGE_ID, SSB_SIGNATURE_IMAGE_ID, SSB_IMAGE_ID }
#define UPDATE_IMAGE_SET_CNT 4 /* 镜像ID列表长度 */

#endif

static uint32_t g_brandy_support_upg_id[] = UPDATE_IMAGE_SET;
static upg_image_collections_t g_brandy_upg_collect = { g_brandy_support_upg_id, UPDATE_IMAGE_SET_CNT };
/* 获取当前程序支持的升级镜像 */
upg_image_collections_t *uapi_upg_get_image_id_collection(void)
{
    return &g_brandy_upg_collect;
}

/* 获取升级包路径 */
char *upg_get_pkg_file_path(void)
{
    return UPG_FILE_NAME;
}

/* 获取资源索引路径 */
char *upg_get_res_file_index_path(void)
{
    return UPG_RES_INDEX_PATH;
}

/* 获取升级包所在目录 */
char *upg_get_pkg_file_dir(void)
{
    return UPG_FILE_PATH;
}

/* 本次支持升级的镜像ID集合，若为空则无镜像限制 */
bool upg_img_in_set(uint32_t img_id)
{
    upg_image_collections_t *collect = uapi_upg_get_image_id_collection();
    if (collect == NULL || collect->img_ids_cnt == 0) {
        return true;
    }

    for (uint32_t i = 0; i < collect->img_ids_cnt; i++) {
        if (collect->img_ids[i] == img_id) {
            return true;
        }
    }
    return false;
}

/*
 * 获取FOTA升级标记区的Flash起始地址，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回升级标记区的起始地址
 */
errcode_t upg_get_upgrade_flag_flash_start_addr(uint32_t *start_address)
{
    errcode_t ret_val;
    partition_information_t info;

    ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }

    /* FOTA升级标记区在FOTA分区的最后 */
    *start_address = info.part_info.addr_info.addr + info.part_info.addr_info.size - UPG_FLASH_PAGE_SIZE;
    return ERRCODE_SUCC;
}

/*
 * 获取FOTA升级进度恢复标记区的Flash起始地址，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回标记区的起始地址
 * size 返回包含标记区和flag区的总长度
 */
errcode_t upg_get_progress_status_start_addr(uint32_t *start_address, uint32_t *size)
{
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }

    *start_address = info.part_info.addr_info.addr;
    *size = info.part_info.addr_info.size;
    return ERRCODE_SUCC;
}

/*
 * 获取在Flash上预留的FOTA分区的地址和长度，该地址为在flash上的相对地址，是相对flash基地址的偏移
 * start_address 返回FOTA分区的起始地址
 * size          返回FOTA分区大小（包含升级标记区和缓存区、状态区）
 */
errcode_t upg_get_fota_partiton_area_addr(uint32_t *start_address, uint32_t *size)
{
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC) {
        return ret_val;
    }
    *start_address = info.part_info.addr_info.addr;
    *size = info.part_info.addr_info.size;
    return ERRCODE_SUCC;
}

/*
 * 重启
 */
void upg_reboot(void)
{
    cpu_utils_set_system_status_by_cause(REBOOT_CAUSE_UPG_COMPLETION);
    hal_reboot_chip();
}

/*
 * 防止看门狗超时，踢狗
 */
void upg_watchdog_kick(void)
{
    uapi_watchdog_kick();
}

uint32_t upg_get_flash_base_addr(void)
{
    return FLASH_START;
}

uint32_t upg_get_flash_size(void)
{
    return UPG_FLASH_SIZE;
}
void non_os_enter_critical(void);

void non_os_exit_critical(void);
// 升级读flash接口需支持跨页读取
errcode_t upg_flash_read(const uint32_t flash_offset, const uint32_t size, uint8_t *ram_data)
{
    errcode_t ret = ERRCODE_FAIL;
    uint32_t read_len = 0;
    uint32_t status_flag_offset;
    uint32_t status_flag_size;
    flash_info_t flash_info;

    uapi_flash_get_info(0, &flash_info);
    ret = upg_get_progress_status_start_addr(&status_flag_offset, &status_flag_size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    non_os_enter_critical();
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto end;
    }

    uint32_t tail_size = UPG_FLASH_PAGE_SIZE - (flash_offset + UPG_FLASH_PAGE_SIZE) % UPG_FLASH_PAGE_SIZE;
    if (tail_size < size) { /* 跨页时，按页读取 */
        if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) && (flash_offset >= status_flag_offset) &&
            (flash_offset < status_flag_offset + status_flag_size)) {
            read_len = uapi_flash_read_data_without_align(0, flash_offset, ram_data, tail_size);
        } else {
            read_len = uapi_flash_read_data(0, flash_offset, ram_data, tail_size);
        }
        if (read_len != tail_size) {
            goto end;
        }
    }

    uint32_t remain_size = size - read_len;
    if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) && (flash_offset >= status_flag_offset) &&
        (flash_offset < status_flag_offset + status_flag_size)) {
        read_len = uapi_flash_read_data_without_align(0, flash_offset, ram_data + read_len, remain_size);
    } else {
        read_len = uapi_flash_read_data(0, flash_offset, ram_data + read_len, remain_size);
    }
    if (read_len != remain_size) {
        goto end;
    }

    ret = ERRCODE_SUCC;
end:
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    return ret;
}

STATIC errcode_t upg_erase_before_write(const uint32_t flash_offset, uint32_t size)
{
    uint32_t erase_offset = flash_offset;
    uint32_t erase_size = size;
    uint32_t fst_page_tail_size = UPG_FLASH_PAGE_SIZE - (flash_offset + UPG_FLASH_PAGE_SIZE) % UPG_FLASH_PAGE_SIZE;
    bool not_page_start = flash_offset % UPG_FLASH_PAGE_SIZE != 0 ? true : false;
    bool not_mult_page = fst_page_tail_size >= size ? true : false;
    if (not_page_start && not_mult_page) {
        return ERRCODE_SUCC;
    }

    if (not_page_start) {
        erase_size -= fst_page_tail_size;
        erase_offset += fst_page_tail_size;
    }

    return upg_flash_erase(erase_offset, erase_size);
}

// 升级写flash接口需支持跨页写入和写前擦功能
errcode_t upg_flash_write(const uint32_t flash_offset, uint32_t size, const uint8_t *ram_data, bool do_erase)
{
    if (size == 0) {
        return ERRCODE_SUCC;
    }
    errcode_t ret = ERRCODE_FAIL;
    uint32_t status_flag_offset;
    uint32_t status_flag_size;
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    uint8_t *cmp_data = upg_malloc(size);
    if (cmp_data == NULL) {
        return ERRCODE_FAIL;
    }

    ret = upg_get_progress_status_start_addr(&status_flag_offset, &status_flag_size);
    if (ret != ERRCODE_SUCC) {
        goto erase_end;
    }

    if (do_erase) {
        if (upg_erase_before_write(flash_offset, size) != ERRCODE_SUCC) {
            goto erase_end;
        }
    }

    non_os_enter_critical();
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto write_end;
    }

    uint32_t write_len = 0;
    if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) && (flash_offset >= status_flag_offset) &&
        (flash_offset < status_flag_offset + status_flag_size)) {
        write_len = uapi_flash_write_data_without_align(0, flash_offset, ram_data, size);
    } else {
        write_len = uapi_flash_write_data(0, flash_offset, ram_data, size);
    }
    if (write_len != size) {
        goto write_end;
    }

    uint32_t read_len = 0;
    if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) && (flash_offset >= status_flag_offset) &&
        (flash_offset < status_flag_offset + status_flag_size)) {
        read_len = uapi_flash_read_data_without_align(0, flash_offset, cmp_data, size);
    } else {
        read_len = uapi_flash_read_data(0, flash_offset, cmp_data, size);
    }
    if (read_len != size) {
        goto write_end;
    }
    ret = ERRCODE_SUCC;
write_end:
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    if (memcmp(cmp_data, ram_data, size) != 0) {
        ret = ERRCODE_FAIL;
    }
erase_end:
    upg_free(cmp_data);
    return ret;
}

errcode_t upg_flash_erase(const uint32_t flash_offset, const uint32_t size)
{
    errcode_t ret = ERRCODE_FAIL;
    non_os_enter_critical();
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto end;
    }

    for (uint32_t i = 0; i < size; i += UPG_FLASH_PAGE_SIZE) {
        uint32_t erase_offset = flash_offset + i;
        if (uapi_flash_sector_erase(0, erase_offset, true) != 0) {
            goto end;
        }
    }

    ret = ERRCODE_SUCC;
end:
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    return ret;
}

#if (UPG_CFG_VERIFICATION_SUPPORT == YES)
/*
 * 判断是否安全使能
 */
STATIC bool upg_sec_boot_is_enable(void)
{
    static uint8_t cf;
    errcode_t otp_ret;
    otp_ret = efuse_port_read_buffer_by_reload(&cf, PAL_OTP_SEC_BOOT_DEBUG_FLAG, sizeof(uint8_t));
    if (otp_ret != ERRCODE_SUCC) {
        return false;
    }

    static uint8_t double_cf;
    otp_ret = efuse_port_read_buffer_by_reload(&double_cf, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, sizeof(uint8_t));
    if (otp_ret != ERRCODE_SUCC) {
        return false;
    }

    cf &= double_cf;
    // only OTP_SEC_BOOT_DEBUG_FLAG equal to 0 that is disable sec boot
    if (cf == 0) {
        return false;
    }

    return true;
}

/*
 * 获取校验用的root_public_key
 */
uint8_t *upg_get_root_public_key(void)
{
    static uint8_t public_key[PUBLIC_KEY_LEN];
    if (upg_sec_boot_is_enable()) {
        upg_msg0("upg_sec_boot_is_enable");
        /* 使用Upgrader_External_Public_Key校验Key Area的签名  */
        uint32_t upgrader_address = 0;
        partition_information_t info;
        errcode_t ret_val = uapi_partition_get_info(PARTITION_SIGNATURE_CERTIFICATE, &info);
        if (ret_val != ERRCODE_SUCC) {
            return NULL;
        }
        upgrader_address = info.part_info.addr_info.addr + upg_get_flash_base_addr();
        uint8_t *upgrader_key = (uint8_t *)(uintptr_t)(upgrader_address + SUB_RSA4096_PUBKEY_N_OFFSET);

        (void)memcpy_s(public_key, PUBLIC_KEY_LEN, upgrader_key, PUBLIC_KEY_LEN);
        return public_key;
    }

    upg_msg0("upg_sec_boot_is_unable");
    upg_package_header_t *pkg_header = NULL;
    errcode_t ret;
    ret = upg_get_package_header(&pkg_header);
    if (ret != ERRCODE_SUCC || pkg_header == NULL) {
        upg_msg0("upg_get_package_header fail");
        return NULL;
    }

    (void)memcpy_s(public_key, PUBLIC_KEY_LEN, (pkg_header->key_area.fota_external_public_key), PUBLIC_KEY_LEN);
    upg_free(pkg_header);
    return public_key;
}

STATIC errcode_t check_fota_msid(const uint32_t msid_ext, const uint32_t mask_msid_ext)
{
    unused(msid_ext);
    unused(mask_msid_ext);
    uint32_t msid = 0;
    errcode_t ret = efuse_port_read_buffer_by_reload((uint8_t *)&msid, MS_ID_ADDR, sizeof(uint32_t));
    if (ret != ERRCODE_SUCC) {
        upg_msg1("get msid failed. ret = ", ret);
    }
    upg_msg1("msid = ", msid);
    if ((msid_ext & mask_msid_ext) != (msid & mask_msid_ext)) {
        upg_msg0("upg verify: msid is wrong!\r\n");
    }

    return ERRCODE_SUCC; // msid地址未确定，先返回成功
}

/*
 * 检查FOTA升级包中的信息与板端是否匹配
 * pkg_header 升级包包头指针
 * 检查成功，返回 ERRCODE_SUCC
 */
errcode_t upg_check_fota_information(const upg_package_header_t *pkg_header)
{
    upg_key_area_data_t *key_area = (upg_key_area_data_t *)&(pkg_header->key_area);
    upg_fota_info_data_t *fota_info = (upg_fota_info_data_t *)&(pkg_header->info_area);
    errcode_t ret;

    ret = check_fota_msid(key_area->msid_ext, key_area->mask_msid_ext);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = check_fota_msid(fota_info->msid_ext, fota_info->mask_msid_ext);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}
#endif

errcode_t upg_get_efuse_addr(uint32_t image_id, uint32_t *start_addr)
{
    switch (image_id) {
        case SSB_IMAGE_ID:
            *start_addr = OTP_SSB_VERSION_COUNTER_START;
            break;
        case RECOVERY_IMAGE_ID:
            *start_addr = OTP_RECOVERY_ROLLBACK_VERSION;
            break;
        case APPLICATION_IMAGE_ID:
            *start_addr = OTP_APP_ROLLBACK_VERSION;
            break;
        case BT_IMAGE_ID:
            *start_addr = OTP_BT_ROLLBACK_VERSION;
            break;
        case DSP_MAIN_IMAGE_ID:
            *start_addr = OTP_DSP_MAIN_ROLLBACK_VERSION;
            break;
        case DSP_OVERLAY_IMAGE_ID:
            *start_addr = OTP_DSP_OVERLAY_ROLLBACK_VERSION;
            break;
        default:
            return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

errcode_t upg_get_board_rollback_version(uint32_t image_id, uint32_t *anti_rollback_ver)
{
    uint32_t efuse_offset;
    errcode_t ret = upg_get_efuse_addr(image_id, &efuse_offset);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    return efuse_port_read_buffer_by_reload((uint8_t *)anti_rollback_ver, efuse_offset, sizeof(uint32_t));
}

errcode_t upg_get_board_version_mask(uint32_t image_id, uint32_t *key_mask, uint32_t *code_mask)
{
    unused(image_id);
    *key_mask = 0x0;
    *code_mask = 0xFFFFFFFF;
    return ERRCODE_SUCC;
}

errcode_t upg_get_board_version(uint32_t image_id, uint32_t *key_ver, uint32_t *code_ver)
{
    unused(image_id);
    upg_package_header_t     *pkg_header = NULL;
    upg_image_hash_node_t *img_hash_table = NULL;
    upg_image_header_t *img_header = NULL;
    errcode_t ret = upg_get_package_header(&pkg_header);
    if (ret != ERRCODE_SUCC || pkg_header == NULL) {
        upg_msg0("upg_get_package_header fail\r\n");
        return ret;
    }
    ret = upg_get_pkg_image_hash_table(pkg_header, &img_hash_table);
    if (ret != ERRCODE_SUCC || img_hash_table == NULL) {
        upg_msg0("upg_get_pkg_image_hash_table fail\r\n");
        upg_free(pkg_header);
        return ret;
    }
    upg_fota_info_data_t *fota_info = (upg_fota_info_data_t *)&(pkg_header->info_area);
    for (uint32_t i = 0; i < fota_info->image_num; i++) {
        ret = upg_get_pkg_image_header((const upg_image_hash_node_t *)&(img_hash_table[i]), &img_header);
        if (ret != ERRCODE_SUCC || img_header == NULL) {
            upg_msg0("upg_get_pkg_image_header fail");
            upg_free(pkg_header);
            upg_free(img_hash_table);
            return ret;
        }
        if (img_header->image_id == image_id) {
            break;
        }
        upg_free(img_header);
        img_header = NULL;
    }
    *key_ver = pkg_header->key_area.fota_key_version_ext;
    *code_ver = img_header->version_ext;
    upg_free(pkg_header);
    upg_free(img_hash_table);
    upg_free(img_header);
    return ret;
}

errcode_t upg_set_board_rollback_version(uint32_t image_id, uint32_t *anti_rollback_ver)
{
    uint32_t efuse_offset;
    errcode_t ret = upg_get_efuse_addr(image_id, &efuse_offset);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    uint32_t version = *anti_rollback_ver;
    return uapi_efuse_write_buffer(efuse_offset, (uint8_t *)&version, sizeof(uint32_t));
}