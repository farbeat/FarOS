/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for common module
 * Author: audio
 */

#ifndef __CORE_CPS_H__
#define __CORE_CPS_H__

#include "td_type.h"
#include "sys_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

enum {
    CPS_PCH_0,
    CPS_PCH_1,
    CPS_PCH_2,
    CPS_PCH_3,
    CPS_PCH_4,
};

typedef struct core_cps_inst *cps_handle;
typedef td_s32 (*cps_query_quota)(td_handle mod, td_u16 *cps_quota, td_u16 *weight);
typedef td_s32 (*cps_handler)(td_void *context);

td_s32 core_cps_init(td_void);
td_s32 core_cps_deinit(td_void);
td_void core_cps_register_change_proc(cps_handler callback, td_void *context);

td_s32 core_cps_create(cps_handle *cps, td_u32 mod_id);
td_s32 core_cps_destroy(cps_handle cps);
td_void core_cps_register_quota_proc(cps_handle cps, cps_query_quota query_quota);

td_void core_cps_start(cps_handle cps);
td_void core_cps_stop(cps_handle cps, td_u32 sample_rate, td_u32 samples);
td_void core_cps_reset(cps_handle cps);

td_void core_cps_refresh_quota(cps_handle cps_inst);
td_void core_cps_query_total_quota(td_u32 *total_quota);
td_bool core_cps_check_no_inst(td_void);
td_void core_cps_query_mod_status(mcps_block mcps_list[], td_u32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_CPS_H__ */
