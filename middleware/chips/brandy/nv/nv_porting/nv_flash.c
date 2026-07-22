/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: NV on different flash
 */
#include "nv_porting.h"
#include "flash.h"
#include "non_os.h"
#include "uapi_crc.h"

extern uint32_t g_extend_heap_begin;
extern uint32_t g_extend_heap_size;

errcode_t kv_flash_read(const uint32_t flash_offset, const uint32_t size, uint8_t *ram_data)
{
    errcode_t ret = ERRCODE_SUCC;
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    non_os_enter_critical();

    uint32_t psram_data_start = (uint32_t)(&g_extend_heap_begin);
    uint32_t psram_data_end = (uint32_t)(&g_extend_heap_begin) + (uint32_t)(&g_extend_heap_size);
    bool is_free = false;
    uint8_t *tmp_ram_data;
    if ((psram_data_start <= (uint32_t)ram_data) && ((uint32_t)ram_data <= psram_data_end)) {
        tmp_ram_data = (uint8_t *)kv_malloc(size);
        is_free = true;
    } else {
        tmp_ram_data = ram_data;
    }

    if (uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
        goto end;
    }

    uint32_t read_len = 0;
    if (flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) {
        read_len = uapi_flash_read_data_without_align(0, flash_offset, tmp_ram_data, size);
    } else {
        read_len = uapi_flash_read_data(0, flash_offset, tmp_ram_data, size);
    }
    if (read_len != size) {
        ret = ERRCODE_FAIL;
        goto end;
    }

end:
    if (uapi_flash_switch_to_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
    }
    if (is_free) {
        if (memcpy_s(ram_data, size, tmp_ram_data, size) != EOK) {
            kv_free(tmp_ram_data);
            return ERRCODE_FAIL;
        }
        kv_free(tmp_ram_data);
    }
    non_os_exit_critical();
    return ret;
}

errcode_t kv_flash_write(const uint32_t flash_offset, uint32_t size, const uint8_t *ram_data, bool do_erase)
{
    (void)do_erase;
    errcode_t ret = ERRCODE_SUCC;
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    non_os_enter_critical();
    uint32_t write_len = 0;
    if (uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
        goto end;
    }

    if (flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) {
        write_len = uapi_flash_write_data_without_align(0, flash_offset, ram_data, size);
    } else {
        write_len = uapi_flash_write_data(0, flash_offset, ram_data, size);
    }
    if (write_len != size) {
        ret = ERRCODE_FAIL;
        goto end;
    }

end:
    if (uapi_flash_switch_to_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
    }
    non_os_exit_critical();
    return ret;
}

errcode_t kv_flash_erase(const uint32_t flash_offset, uint32_t size)
{
    errcode_t ret = ERRCODE_SUCC;
    non_os_enter_critical();
    if (uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
        goto end;
    }

    if (uapi_flash_block_erase(0, flash_offset, size, true) != 0) {
        ret = ERRCODE_FAIL;
        goto end;
    }
end:
    if (uapi_flash_switch_to_xip_mode(0) != ERRCODE_SUCC) {
        ret = ERRCODE_FAIL;
    }
    non_os_exit_critical();
    return ret;
}