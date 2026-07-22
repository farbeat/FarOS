/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for audio overlay
 * Author: audio
 */

#ifndef __DRV_OVERLAY_LOAD_H__
#define __DRV_OVERLAY_LOAD_H__

#include "audio_struct.h"
#include "core_overlay_info.h"

#define OVERLAY_MAX_SIZE 0x200000 /* 2M */
#define OVERLAY_NAME_LEN 16

static inline td_void drv_overlay_copy_section(td_u32 *to, const td_u32 *from, td_u32 size)
{
    td_u32 i;
    td_u32 *dest = to;
    const td_u32 *src = from;
    for (i = 0; i < (size >> 2); i++) { /* shift 2 bit to sizeof int */
        *dest++ = *src++;
    }
}

td_u32 drv_overlay_map_addr(audio_core_id core_id, td_u32 addr, td_bool *tcm_flag);

td_s32 drv_overlay_add_section_item(audio_core_id core_id, td_u32 addr, td_u32 size);
td_void drv_overlay_clear_section_list(td_void);

td_s32 drv_overlay_backup_tcm(audio_core_id core_id);
td_s32 drv_overlay_restore_tcm(audio_core_id core_id);

const td_char *drv_overlay_find_and_reset_lib(audio_core_id core_id, audio_lib_id type, td_u32 index);

td_s32 drv_overlay_load_main_program(audio_core_id core_id);

td_s32 drv_overlay_verify_dsp_fw(td_void);

td_s32 drv_overlay_get_lib(const td_char *lib_name, td_u8 *lib_buf, td_u32 *lib_buf_size);

td_s32 drv_overlay_load_lib_program(audio_core_id core_id, audio_lib_id type, td_u32 index);

td_s32 drv_overlay_request_firmware(td_handle handle, core_overlay_id overlay_id);
td_void drv_overlay_release_firmware(td_handle handle, core_overlay_id overlay_id);

#endif /* __DRV_OVERLAY_LOAD_H__ */
