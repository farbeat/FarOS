/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#include "securec.h"
#include "mem_addr.h"
#include "partition.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_osal.h"
#include "drv_msg_sys.h"
#include "drv_overlay_load.h"
#include "core_overlay_info.h"

#if (SAP_CHIP_TYPE == socmn1)
#include "image_verify.h"

#define DSP0_PARTITION_ID PARTITION_DSP0_IMAGE
#define DSP1_PARTITION_ID PARTITION_DSP1_IMAGE

#define HAID_DST_ADDR 0x5818c000
#define FIELE_SIGNED_HEADER_OFFSET 0x800

static td_bool g_dsp_fw_signed = TD_FALSE;
static td_bool g_dsp_lib_signed = TD_FALSE;

#else
#define FIELE_SIGNED_HEADER_OFFSET 0x0
#define DSP0_PARTITION_ID PARTITION_DSP_OVERLAY_IMAGE
#define DSP1_PARTITION_ID PARTITION_DSP_MAIN_IMAGE

#endif /* SAP_CHIP_TYPE */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static td_s32 overlay_load_elf(audio_core_id core_id, const td_u8 *lib, td_u32 size)
{
    td_u32 i, sec_cnt;
    td_bool tcm_flag;
    td_u32 phys_addr;
    td_u32 *virt_addr = TD_NULL;
    td_u32 offset = 0;
    td_s32 ret;
    struct {
        td_u32 addr;
        td_u32 size;
    } sec_hdr;

    if (lib == TD_NULL || size > OVERLAY_MAX_SIZE) {
        return ERR_SYS_BUF_OVERFLOW;
    }

    ret = memcpy_s(&sec_cnt, sizeof(td_u32), lib + offset, sizeof(td_u32));
    if (ret != EOK) {
        return ret;
    }

    offset += (td_u32)sizeof(td_u32);

    for (i = 0; i < sec_cnt; i++) {
        ret = memcpy_s(&sec_hdr, sizeof(sec_hdr), lib + offset, sizeof(sec_hdr));
        if (ret != EOK) {
            return ret;
        }
        offset += (td_u32)sizeof(sec_hdr);

        if (is_4byte_aligned(sec_hdr.addr) != TD_TRUE || is_4byte_aligned(sec_hdr.size) != TD_TRUE) {
            audio_log_err("addr=%#x/size=%#x, not aligned to 4 bytes", sec_hdr.addr, sec_hdr.size);
            return ERR_SYS_VERIFY_FAIL;
        }

        phys_addr = drv_overlay_map_addr(core_id, sec_hdr.addr, &tcm_flag);
        virt_addr = audio_osal_address_map((td_ulong)phys_addr, sec_hdr.size);
        drv_overlay_copy_section(virt_addr, (const td_u32 *)(lib + offset), sec_hdr.size);

        audio_osal_dcache_region_wb((td_ulong)phys_addr, virt_addr, sec_hdr.size);
        audio_osal_address_unmap((td_ulong)phys_addr, virt_addr);
        offset += sec_hdr.size;
    }

    return AUDIO_SUCCESS;
}

static td_void overlay_update_section_list(td_void)
{
    drv_overlay_clear_section_list();
}

static td_u8 *overlay_find_lib(const td_char *lib_name, td_u32 addr, td_u32 *lib_size)
{
    td_u32 i;
    td_u32 sec_cnt = 0;
    td_u32 offset = 0;
    td_s32 ret;
    struct {
        td_char name[OVERLAY_NAME_LEN];
        td_u32 size;
    } sec_hdr;
    td_u8 *lib = (td_u8 *)(td_uintptr_t)addr;

    if (lib_name == TD_NULL || lib == TD_NULL || lib_size == TD_NULL) {
        return TD_NULL;
    }

    ret = memcpy_s(&sec_cnt, sizeof(td_u32), lib + offset, sizeof(td_u32));
    if (ret != EOK) {
        sap_err_log_fun(memcpy_s, ret);
        return TD_NULL;
    }
    offset += (td_u32)sizeof(td_u32);

    for (i = 0; i < sec_cnt && offset < *lib_size; i++) {
        ret = memcpy_s(&sec_hdr, sizeof(sec_hdr), lib + offset, sizeof(sec_hdr));
        if (ret != EOK) {
            sap_err_log_fun(memcpy_s, ret);
            return TD_NULL;
        }
        offset += (td_u32)sizeof(sec_hdr);
        if (audio_osal_strncmp(sec_hdr.name, sizeof(sec_hdr.name), lib_name, sizeof(sec_hdr.name)) == 0) {
            sap_warn_log_str(sec_hdr.name);
            *lib_size = sec_hdr.size;
            return lib + offset;
        }
        offset += sec_hdr.size;
    }

    return TD_NULL;
}

static td_u8 *overlay_unpack_lib(const td_char *lib_name, td_u32 *lib_size)
{
    td_u8 *lib = TD_NULL;
    errcode_t ret;
    struct {
        partition_ids_t partition_id;
        partition_information_t partition_info;
        td_u32 addr;
        td_u32 size;
    } dsp_image_info[] = {
        {DSP0_PARTITION_ID, {0}, FLASH_DSP_OVERLAY_ADDR, FLASH_DSP_OVERLAY_SIZE},
        {DSP1_PARTITION_ID, {0}, FLASH_DSP_MAIN_ADDR, FLASH_DSP_MAIN_SIZE},
    };
    const td_u32 flash_base_addr = FLASH_BASEADDR + FIELE_SIGNED_HEADER_OFFSET;
    const td_u32 cnt = (td_u32)(sizeof(dsp_image_info) / sizeof(dsp_image_info[0]));

    for (td_u32 i = 0; i < cnt; i++) {
        ret = uapi_partition_get_info(dsp_image_info[i].partition_id, &dsp_image_info[i].partition_info);
        if (ret == ERRCODE_SUCC && dsp_image_info[i].partition_info.type == PARTITION_BY_ADDRESS) {
            dsp_image_info[i].addr = dsp_image_info[i].partition_info.part_info.addr_info.addr + flash_base_addr;
            dsp_image_info[i].size = dsp_image_info[i].partition_info.part_info.addr_info.size;
        } else {
            sap_err_log_fun(uapi_partition_get_info, ret);
        }

        lib = overlay_find_lib(lib_name, dsp_image_info[i].addr, &dsp_image_info[i].size);
        if (lib != TD_NULL) {
            if (lib_size != TD_NULL) {
                *lib_size = dsp_image_info[i].size;
            }
            return lib;
        }
    }

    return TD_NULL;
}

td_s32 drv_overlay_get_lib(const td_char *lib_name, td_u8 *lib_buf, td_u32 *lib_buf_size)
{
    td_s32 ret;
    td_u32 lib_size;
    td_u8 *lib = TD_NULL;

    sys_check_null_ptr_return(lib_name);
    sys_check_null_ptr_return(lib_buf);
    sys_check_null_ptr_return(lib_buf_size);

    lib = overlay_unpack_lib(lib_name, &lib_size);
    if (lib == TD_NULL) {
        sap_err_log_str(lib_name);
        return ERR_SYS_NOT_SUPPORTED;
    }

    if (lib_size > *lib_buf_size) { /* input buf size check */
        sap_err_log_str(lib_size);
        return ERR_SYS_BUF_OVERFLOW;
    }

    ret = memcpy_s(lib_buf, *lib_buf_size, lib, lib_size);
    if (ret != EOK) {
        sap_err_log_fun(memcpy_s, ret);
        return ERR_SYS_MEM_COPY;
    }

    *lib_buf_size = lib_size; /* output buf actual data size */

    return AUDIO_SUCCESS;
}

static td_s32 drv_overlay_clear_section(audio_lib_id type)
{
    td_s32 ret;

    if (type == AUDIO_LIB_DSP0_APP || type == AUDIO_LIB_DSP1_APP) {
        return AUDIO_SUCCESS;
    }

    ret = drv_msg_sys_clear_overlay_section(type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_sys_clear_overlay_section, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

#if (SAP_CHIP_TYPE == socmn1)
td_s32 drv_overlay_load_main_program(audio_core_id core_id)
{
    td_s32 ret;
    /* dsp_fw singed verify */
    if (g_dsp_fw_signed == TD_FALSE) {
        ret = uapi_verify_image_valid(IMAGE_DSP_FW, 0);
        if (ret != ERRCODE_SUCC) {
            sap_err_log_fun(uapi_verify_image_valid, ret);
            return ret;
        }
        g_dsp_fw_signed = TD_TRUE;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        return drv_overlay_load_lib_program(core_id, AUDIO_LIB_DSP0_APP, 0);
    } else if (core_id == AUDIO_CORE_DSP1) {
        return drv_overlay_load_lib_program(core_id, AUDIO_LIB_DSP1_APP, 0);
    } else {
        sap_err_log_s32(core_id);
        return ERR_SYS_INVALID_PARAM;
    }
}
#else
td_s32 drv_overlay_load_main_program(audio_core_id core_id)
{
    errcode_t ret1;
    td_s32 ret2;
    struct {
        partition_ids_t partition_id;
        partition_information_t partition_info;
        td_u32 addr;
        td_u32 size;
    } dsp_image_info = {
        DSP1_PARTITION_ID, {0}, FLASH_DSP_MAIN_ADDR, FLASH_DSP_MAIN_SIZE,
    };
    const td_u32 flash_base_addr = FLASH_BASEADDR + FIELE_SIGNED_HEADER_OFFSET;

    ret1 = uapi_partition_get_info(dsp_image_info.partition_id, &dsp_image_info.partition_info);
    if (ret1 == ERRCODE_SUCC && dsp_image_info.partition_info.type == PARTITION_BY_ADDRESS) {
        dsp_image_info.addr = dsp_image_info.partition_info.part_info.addr_info.addr + flash_base_addr;
        dsp_image_info.size = dsp_image_info.partition_info.part_info.addr_info.size;
    } else {
        sap_err_log_fun(uapi_partition_get_info, ret1);
    }

    ret2 = overlay_load_elf(core_id, (const td_u8 *)(uintptr_t)dsp_image_info.addr, dsp_image_info.size);
    if (ret2 != AUDIO_SUCCESS) {
        sap_err_log_fun(overlay_load_elf, ret2);
        return ret2;
    }

    overlay_update_section_list();
    return AUDIO_SUCCESS;
}
#endif

#if (SAP_CHIP_TYPE == socmn1)
static td_s32 copy_haid_to_mem(td_void)
{
    td_s32 ret;
    uintptr_t dsp_lib_addr;
    uintptr_t virt_addr = HAID_DST_ADDR;
    partition_information_t dsp1_info;

    ret = uapi_partition_get_info(PARTITION_DSP1_IMAGE, &dsp1_info);
    if (ret != ERRCODE_SUCC || dsp1_info.type != PARTITION_BY_ADDRESS) {
        sap_err_log_fun(uapi_partition_get_info, ret);
        return ret;
    }

    dsp_lib_addr = (uintptr_t)(dsp1_info.part_info.addr_info.addr + FLASH_BASEADDR);
    ret = decrypt_dsp_image(dsp_lib_addr, virt_addr);
    if (ret != ERRCODE_SUCC) {
        sap_err_log_fun(decrypt_dsp_image, ret);
        memset_s(virt_addr, sizeof(td_u32), 0, sizeof(td_u32));
        return ret;
    }

    /* dsp_lib singed verify */
    if (g_dsp_lib_signed == TD_FALSE) {
        ret = uapi_verify_image_valid(IMAGE_DSP_LIB, virt_addr);
        if (ret != ERRCODE_SUCC) {
            sap_err_log_fun(uapi_verify_image_valid, ret);
            return ret;
        }
        g_dsp_lib_signed = TD_TRUE;
    }

    return ret;
}
#endif

td_s32 drv_overlay_load_lib_program(audio_core_id core_id, audio_lib_id type, td_u32 index)
{
    td_s32 ret;
    td_u32 lib_size;
    td_u8 *lib = TD_NULL;
    const td_char *lib_name = TD_NULL;

    if (type >= AUDIO_LIB_MAX) {
        sap_err_log_s32(type);
        return ERR_SYS_INVALID_PARAM;
    }

#if (SAP_CHIP_TYPE == socmn1)
    if (type == AUDIO_LIB_HAID) {
        return copy_haid_to_mem();
    }
#endif

    lib_name = drv_overlay_find_and_reset_lib(core_id, type, index);
    if (lib_name == TD_NULL) {
        sap_err_log_s32(type);
        return ERR_SYS_NOT_SUPPORTED;
    }

    lib = overlay_unpack_lib(lib_name, &lib_size);
    if (lib == TD_NULL) {
        sap_err_log_str(lib_name);
        return ERR_SYS_NOT_SUPPORTED;
    }

    ret = drv_overlay_clear_section(type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_clear_section, ret);
        return ret;
    }

    ret = overlay_load_elf(core_id, lib, lib_size);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(overlay_load_elf, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
