/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#include "securec.h"
#include "mem_addr.h"
#include "osal_list.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_audio_osal.h"
#include "core_overlay_info.h"
#include "drv_overlay_load.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AUDIO_CORE_MASK_DSP0 (1L << AUDIO_CORE_DSP0)
#define AUDIO_CORE_MASK_DSP1 (1L << AUDIO_CORE_DSP1)
#define AUDIO_CORE_MASK_ALL_DSP (AUDIO_CORE_MASK_DSP0 | AUDIO_CORE_MASK_DSP1)

typedef struct {
    td_u32 core_mask;
    td_u32 addr;
    td_u32 size;
    td_u32 map_addr; /* A cpu映射的新物理地址 */
    td_bool tcm;     /* 标记内存段是否为DSP的IRAM、DRAM，这段只能四字节读写 */
} elf_section_map;

static const elf_section_map g_elf_section_map[] = {
    /* sram */
    {AUDIO_CORE_MASK_ALL_DSP, SRAM_ADDR, SIZE_3M, SRAM_MAP_ADDR, TD_FALSE},

    /* share ram */
    {AUDIO_CORE_MASK_ALL_DSP, SHM_ADDR, COM_MEM_MAP_SIZE, SHM_MAP_ADDR, TD_FALSE},

    /* l2ram */
    {AUDIO_CORE_MASK_ALL_DSP, SOC_L2RAM_AUDIO_VIRT_ADDR, SOC_L2RAM_SIZE, SOC_L2RAM_ACORE_PHYS_ADDR, TD_FALSE},

    /*
     * dsp0 dram 0xe8058000, size 32k + 128k
     */
    {AUDIO_CORE_MASK_DSP0, DRAM_ADDR, DRAM_SIZE, ACORE_DSP0_DRAM_ADDR, TD_TRUE},

    /*
     * dsp0 iram 0xe8080000, size 32k + 16k
     */
    {AUDIO_CORE_MASK_DSP0, IRAM_ADDR, IRAM_SIZE, ACORE_DSP0_IRAM_ADDR, TD_TRUE},

    /*
     * dsp1 dram 0xe8058000, size 32k + 128k
     */
    {AUDIO_CORE_MASK_DSP1, DRAM_ADDR, DRAM_SIZE, ACORE_DSP1_DRAM_ADDR, TD_TRUE},

    /*
     * dsp1 iram 0xe8080000, size 32k + 16k
     */
    {AUDIO_CORE_MASK_DSP1, IRAM_ADDR, IRAM_SIZE, ACORE_DSP1_IRAM_ADDR, TD_TRUE},
};

typedef struct {
    td_u32 addr;
    td_u32 size;
    audio_core_id core_id;
    struct osal_list_head node;
} elf_section_item;

static OSAL_LIST_HEAD(g_section_item_list);

static td_u32 overlay_map_addr(audio_core_id core_id, td_u32 addr, td_bool *tcm_flag)
{
    td_u32 i;
    const elf_section_map *sec_map = TD_NULL;
    td_u32 core_mask = (1L << (td_u32)core_id);

    *tcm_flag = TD_FALSE;

    for (i = 0; i < sizeof(g_elf_section_map) / sizeof(g_elf_section_map[0]); i++) {
        sec_map = &g_elf_section_map[i];
        if ((sec_map->core_mask & core_mask) == 0) {
            continue;
        }

        if (sec_map->map_addr == 0) {
            continue;
        }

        if ((addr >= sec_map->addr) && (addr < (sec_map->addr + sec_map->size))) {
            *tcm_flag = sec_map->tcm;
            return sec_map->map_addr + (addr - sec_map->addr);
        }
    }

    return addr;
}

td_u32 drv_overlay_map_addr(audio_core_id core_id, td_u32 addr, td_bool *tcm_flag)
{
    return overlay_map_addr(core_id, addr, tcm_flag);
}

td_s32 drv_overlay_add_section_item(audio_core_id core_id, td_u32 addr, td_u32 size)
{
    elf_section_item *sec_item = TD_NULL;

    sec_item = (elf_section_item *)audio_osal_malloc(sizeof(*sec_item));
    if (sec_item == TD_NULL) {
        return ERR_SYS_MEM_ALLOC;
    }

    sec_item->addr = addr;
    sec_item->size = size;
    sec_item->core_id = core_id;
    osal_list_add_tail(&sec_item->node, &g_section_item_list);
    return AUDIO_SUCCESS;
}

td_void drv_overlay_clear_section_list(td_void)
{
    elf_section_item *sec_item = TD_NULL;
    elf_section_item *tmp_item = TD_NULL;

    osal_list_for_each_entry_safe(sec_item, tmp_item, &g_section_item_list, node) {
        osal_list_del(&sec_item->node);
        audio_osal_free(sec_item);
    }
}

static td_s32 overlay_sync_tcm_elf(audio_core_id core_id, td_u32 addr, td_u32 size, td_bool bak_flag)
{
    td_bool tcm_flag;
    td_u32 tcm_phys, bak_phys;
    td_void *tcm_virt, *bak_virt;
    td_char *offset = TD_NULL;
    elf_section_item *sec_item = TD_NULL;

    bak_phys = overlay_map_addr(core_id, addr, &tcm_flag);
    bak_virt = audio_osal_address_map((td_ulong)bak_phys, size);
    if (bak_virt == TD_NULL) {
        sap_err_log_fun(audio_osal_address_map, ERR_SYS_MEM_EXCEPTION);
        return ERR_SYS_MEM_EXCEPTION;
    }
    offset = (td_char *)bak_virt;

    osal_list_for_each_entry(sec_item, &g_section_item_list, node) {
        tcm_phys = overlay_map_addr(sec_item->core_id, sec_item->addr, &tcm_flag);
        if (tcm_flag == TD_FALSE) {
            continue;
        }

        audio_log_info("sec[addr: 0x%08X, size: 0x%08X]", tcm_phys, sec_item->size);
        tcm_virt = audio_osal_address_map((td_ulong)tcm_phys, sec_item->size);
        if (tcm_virt == TD_NULL) {
            audio_osal_address_unmap((td_ulong)bak_phys, bak_virt);
            sap_err_log_fun(audio_osal_address_map, ERR_SYS_MEM_EXCEPTION);
            return ERR_SYS_MEM_EXCEPTION;
        }

        if (bak_flag) {
            drv_overlay_copy_section((td_u32 *)offset, tcm_virt, sec_item->size);
        } else {
            drv_overlay_copy_section(tcm_virt, (td_u32 *)offset, sec_item->size);
            audio_osal_dcache_region_wb((td_ulong)tcm_phys, tcm_virt, sec_item->size);
        }
        offset += sec_item->size;

        audio_osal_address_unmap((td_ulong)tcm_phys, tcm_virt);
    }

    if (bak_flag) {
        audio_osal_dcache_region_wb((td_ulong)bak_phys, bak_virt, size);
    }

    audio_osal_address_unmap((td_ulong)bak_phys, bak_virt);
    return AUDIO_SUCCESS;
}

td_s32 drv_overlay_backup_tcm(audio_core_id core_id)
{
    td_s32 ret;
    audio_func_enter();

    if (core_id == AUDIO_CORE_DSP1) {
        return AUDIO_SUCCESS;
    }

    ret = overlay_sync_tcm_elf(core_id, CODEC_ENTRY_ADDR, CODEC_BIN_SIZE, TD_TRUE);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(overlay_sync_tcm_elf, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 drv_overlay_restore_tcm(audio_core_id core_id)
{
    td_s32 ret;
    audio_func_enter();

    if (core_id == AUDIO_CORE_DSP1) {
        return AUDIO_SUCCESS;
    }

    ret = overlay_sync_tcm_elf(core_id, CODEC_ENTRY_ADDR, CODEC_BIN_SIZE, TD_FALSE);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(overlay_sync_tcm_elf, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static const struct {
    audio_lib_id overlay_type;
    const td_char *name;
} g_overlay_name[] = {
    /* audio HAID libs */
    {AUDIO_LIB_DSP0_APP, "dsp_main.bin"},
    {AUDIO_LIB_DSP1_APP, "dsp_main1.bin"},

    /* audio HAID libs */
    {AUDIO_LIB_HAID, "haid.bin"},

    /* audio decode libs */
    {AUDIO_LIB_DEC_PCM, "pcm_dec.bin"},
    {AUDIO_LIB_DEC_MP3, "mp3_dec.bin"},
    {AUDIO_LIB_DEC_AAC, "aac_dec.bin"},
    {AUDIO_LIB_DEC_SBC, "sbc_dec.bin"},
    {AUDIO_LIB_DEC_OPUS, "opus_dec.bin"},
    {AUDIO_LIB_DEC_FLAC, "flac_dec.bin"},
    {AUDIO_LIB_DEC_L2HC, "l2hc_dec.bin"},
    {AUDIO_LIB_DEC_LC3, "lc3_dec.bin"},
    {AUDIO_LIB_DEC_SILK, "silk_dec.bin"},
    {AUDIO_LIB_DEC_VORBIS, "vorbis_dec.bin"},
    {AUDIO_LIB_DEC_AMRWB, "amrwb_dec.bin"},

    /* audio encode libs */
    {AUDIO_LIB_ENC_PCM, "pcm_enc.bin"},
    {AUDIO_LIB_ENC_MP3, "mp3_enc.bin"},
    {AUDIO_LIB_ENC_AAC, "aac_enc.bin"},
    {AUDIO_LIB_ENC_SBC, "sbc_enc.bin"},
    {AUDIO_LIB_ENC_OPUS, "opus_enc.bin"},
    {AUDIO_LIB_ENC_L2HC, "l2hc_enc.bin"},
    {AUDIO_LIB_ENC_LC3, "lc3_enc.bin"},
    {AUDIO_LIB_ENC_SILK, "silk_enc.bin"},
    {AUDIO_LIB_ENC_AMRWB, "amrwb_enc.bin"},

    {AUDIO_LIB_SEA_AFE_SEE, "see_1mic.bin"},
    {AUDIO_LIB_SEA_AAI_KWS, "phs_gru.bin"},
    {AUDIO_LIB_SEA_AAI_ASR, "phs_gru.bin"},

    {AUDIO_LIB_SEA_AFE_EXT0, "sea_lib0.bin"},
    {AUDIO_LIB_SEA_AFE_EXT1, "sea_lib1.bin"},
    {AUDIO_LIB_SEA_AAI_EXT0, "sea_lib2.bin"},
    {AUDIO_LIB_SEA_AAI_EXT1, "sea_lib3.bin"},

    /* aef libs */
    {AUDIO_LIB_AEF, "aef.bin"},
};

const td_char *drv_overlay_find_and_reset_lib(audio_core_id core_id, audio_lib_id type, td_u32 index)
{
    td_u32 i;

    for (i = 0; i < sizeof(g_overlay_name) / sizeof(g_overlay_name[0]); i++) {
        if (type != g_overlay_name[i].overlay_type) {
            continue;
        }
        return g_overlay_name[i].name;
    }

    audio_unused(core_id);
    audio_unused(index);
    audio_unused(core_id);
    return TD_NULL;
}

static td_handle g_overlay_handle[OVERLAY_MAX] = {
    [OVERLAY_CODEC] = AUDIO_INVALID_HANDLE,
    [OVERLAY_EXTCODEC] = AUDIO_INVALID_HANDLE,
    [OVERLAY_SEA] = AUDIO_INVALID_HANDLE,
    [OVERLAY_AAI] = AUDIO_INVALID_HANDLE,
    [OVERLAY_HAID] = AUDIO_INVALID_HANDLE,
    [OVERLAY_AEF] = AUDIO_INVALID_HANDLE,
};

td_s32 drv_overlay_request_firmware(td_handle handle, core_overlay_id overlay_id)
{
    if (overlay_id >= OVERLAY_MAX) {
        sap_fatal_log_s32(overlay_id);
        return ERR_SYS_INVALID_PARAM;
    }

    if (handle == 0 || handle == AUDIO_INVALID_HANDLE) {
        sap_fatal_log_h32(handle);
        return ERR_SYS_INVALID_PARAM;
    }

    if (g_overlay_handle[overlay_id] == handle) {
        return AUDIO_SUCCESS;
    }

    if (g_overlay_handle[overlay_id] != AUDIO_INVALID_HANDLE) {
        return ERR_SYS_NOT_AVAILABLE;
    }

    g_overlay_handle[overlay_id] = handle;
    return AUDIO_SUCCESS;
}

td_void drv_overlay_release_firmware(td_handle handle, core_overlay_id overlay_id)
{
    if (overlay_id >= OVERLAY_MAX) {
        sap_fatal_log_s32(overlay_id);
        return;
    }

    if (g_overlay_handle[overlay_id] == handle) {
        g_overlay_handle[overlay_id] = AUDIO_INVALID_HANDLE;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
