/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: dfx freertos mem
 * This file should be changed only infrequently and with great care.
 */

#include "dfx_mem.h"
#include "errcode.h"
#include "std_def.h"
#include "xlt_memory.h"

errcode_t dfx_mem_get_sys_pool_info(mdm_mem_info_t *info)
{
    xlt_mem_pool_status memstatus;
    xlt_mem_info_get(OS_EXTEND_MEM_ADDR, &memstatus);

    info->total = memstatus.uw_total_used_size + memstatus.uw_total_free_size;
    info->used = memstatus.uw_total_used_size;
    info->free = memstatus.uw_total_free_size;
    info->max_free_node_size = memstatus.uw_max_free_node_size;
    info->used_node_num = memstatus.uw_used_node_num;
    info->free_node_num = memstatus.uw_free_node_num;
    info->peek_size = 0;
    return ERRCODE_SUCC;
}
