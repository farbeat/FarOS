/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: source file for core module
 * Author: audio
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SAP_MOD_PREFIX 0x6D /* crc8 of sap */

#define pool_init(pool)        \
    do {                       \
        if ((pool) == TD_NULL) \
            break;             \
        (pool)->prev = (pool); \
        (pool)->next = (pool); \
    } while (0)

#define add_head_process(pool, mod) \
    do {                            \
        (mod)->next = (pool)->next; \
        (mod)->prev = (pool);       \
        (pool)->next->prev = (mod); \
        (pool)->next = (mod);       \
    } while (0)

#define pool_add_head(pool, mod)         \
    do {                                 \
        if ((mod) == TD_NULL)            \
            break;                       \
        if ((pool) == TD_NULL)           \
            break;                       \
        if ((pool)->next == TD_NULL)     \
            break;                       \
        add_head_process((pool), (mod)); \
    } while (0)

#define pop_head_process(pool, mod) \
    do {                            \
        (mod) = (pool)->next;       \
        if ((mod) == TD_NULL)       \
            break;                  \
        if ((mod)->next == TD_NULL) \
            break;                  \
        (mod)->next->prev = (pool); \
        (pool)->next = (mod)->next; \
    } while (0)

#define pool_pop_head(pool, mod)             \
    do {                                     \
        if ((pool) == TD_NULL)               \
            break;                           \
        if ((pool) == (pool)->next) {        \
            (mod) = TD_NULL;                 \
        } else {                             \
            pop_head_process((pool), (mod)); \
        }                                    \
    } while (0)

#define add_tail_process(pool, mod) \
    do {                            \
        (mod)->next = (pool);       \
        (mod)->prev = (pool)->prev; \
        (pool)->prev->next = (mod); \
        (pool)->prev = (mod);       \
    } while (0)

#define pool_add_tail(pool, mod)         \
    do {                                 \
        if ((mod) == TD_NULL)            \
            break;                       \
        if ((pool) == TD_NULL)           \
            break;                       \
        if ((pool)->prev == TD_NULL)     \
            break;                       \
        add_tail_process((pool), (mod)); \
    } while (0)

#define pop_tail_process(pool, mod) \
    do {                            \
        (mod) = (pool)->prev;       \
        if ((mod) == TD_NULL)       \
            break;                  \
        if ((mod)->prev == TD_NULL) \
            break;                  \
        (mod)->prev->next = (pool); \
        (pool)->prev = (mod)->prev; \
    } while (0)

#define pool_pop_tail(pool, mod)             \
    do {                                     \
        if ((pool) == TD_NULL)               \
            break;                           \
        if ((pool) == (pool)->next) {        \
            (mod) = TD_NULL;                 \
        } else {                             \
            pop_tail_process((pool), (mod)); \
        }                                    \
    } while (0)

#define rmv_mode_process(pool, mod)      \
    do {                                 \
        (mod)->prev->next = (mod)->next; \
        (mod)->next->prev = (mod)->prev; \
    } while (0)

#define pool_rmv_mode(pool, mod)                              \
    do {                                                      \
        if ((mod) == TD_NULL)                                 \
            break;                                            \
        if ((mod)->prev == TD_NULL || (mod)->next == TD_NULL) \
            break;                                            \
        if ((mod) == (mod)->next)                             \
            break;                                            \
        rmv_mode_process((pool), (mod));                      \
    } while (0)

static td_s32 core_mod_check_mod_id(sap_mod_id mid, const sap_mod_pool *mod_pool)
{
    if ((mid.bits.index >= mod_pool->node_count) || (mid.bits.type != mod_pool->mod_type)) {
        sap_err_log_h32(mid.u32);
        sap_err_log_u32(mod_pool->node_count);
        sap_err_log_h32(mod_pool->mod_type);
        return ERR_SYS_INVALID_ID;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_mod_init(sap_mod_pool_handle *pool, td_u8 type, td_u32 size, td_u16 count)
{
    td_u8 i;
    td_u32 pool_size;
    td_void *pool_addr = TD_NULL;
    td_u8 *mod_ptr = TD_NULL;
    td_void *base_addr = TD_NULL;
    sap_mod_head *mod_node = TD_NULL;
    sap_mod_pool *mod_pool = TD_NULL;

    audio_func_enter();
    if ((pool == TD_NULL) || (size == 0 || count == 0)) {
        sap_err_log_ret(ERR_SYS_INVALID_PARAM);
        return ERR_SYS_INVALID_PARAM;
    }

    pool_size = (td_u32)sizeof(sap_mod_pool) + (size * count);
    base_addr = audio_osal_calloc(pool_size);
    if (base_addr == TD_NULL) {
        sap_err_log_ret(ERR_SYS_MEM_ALLOC);
        return ERR_SYS_MEM_ALLOC;
    }

    /* at least 4-byte aligned by calloc, shouldn't be re-aligned. */
    pool_addr = base_addr;
    mod_pool = (sap_mod_pool *)pool_addr;
    mod_pool->base_addr = base_addr;

    audio_osal_mutex_init((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    pool_init(&mod_pool->used_list);
    pool_init(&mod_pool->free_list);

    mod_pool->mod_type = type;
    mod_pool->node_size = size;
    mod_pool->node_count = (td_u8)count;
    mod_pool->free_num = (td_u8)count;
    mod_pool->used_num = 0;

    for (i = 0; i < mod_pool->node_count; i++) {
        mod_ptr = &mod_pool->pool_start[i * size];
        mod_node = (sap_mod_head *)((td_void *)mod_ptr);
        mod_node->mid.bits.type = SAP_MOD_NULL;
        mod_node->mid.bits.index = i;
        mod_node->mid.bits.reserved = SAP_MOD_PREFIX;
        pool_add_tail(&mod_pool->free_list, mod_node);
    }

    *pool = mod_pool;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_deinit(sap_mod_pool_handle pool)
{
    td_void *base_addr = TD_NULL;
    td_void *pool_addr = TD_NULL;
    sap_mod_pool *mod_pool = pool;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    base_addr = mod_pool->base_addr;

    pool_addr = base_addr;
    if (pool_addr != (td_void *)mod_pool) {
        sap_err_log_ret(ERR_SYS_INVALID_PARAM);
        return ERR_SYS_INVALID_PARAM;
    }

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    if (mod_pool->used_num > 0) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        sap_err_log_ret(ERR_SYS_INST_BUSY);
        return ERR_SYS_INST_BUSY;
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    audio_osal_mutex_deinit((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    audio_osal_free(base_addr);
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_get_module(sap_mod_pool_handle pool, td_u32 mod_id, sap_mod_handle *module)
{
    td_s32 ret;
    td_u32 offset;
    sap_mod_id mid = { .u32 = mod_id };
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = TD_NULL;

    sys_check_null_ptr_return(pool);
    sys_check_null_ptr_return(module);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    ret = core_mod_check_mod_id(mid, mod_pool);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ret;
    }

    offset = (mid.bits.index * mod_pool->node_size);

    mod_node = (sap_mod_head *)((td_void *)&mod_pool->pool_start[offset]);
    if (mod_node->mid.bits.type == SAP_MOD_NULL) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ERR_SYS_INVALID_ID;
    }

    *module = mod_node;
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    return AUDIO_SUCCESS;
}

td_s32 core_mod_alloc(sap_mod_pool_handle pool, sap_mod_handle *module)
{
    td_u32 head_size;
    td_u32 body_size;
    td_void *mod_body = TD_NULL;
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = TD_NULL;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    sys_check_null_ptr_return(module);

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    if (mod_pool->used_num >= mod_pool->node_count) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        sap_err_log_u32(mod_pool->used_num);
        sap_err_log_u32(mod_pool->node_count);
        return ERR_SYS_INST_FULL;
    }
    pool_pop_head(&mod_pool->free_list, mod_node);
    if (mod_node == TD_NULL) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        audio_log_err("mod_node is null");
        return AUDIO_FAILURE;
    }
    pool_add_tail(&mod_pool->used_list, mod_node);
    mod_pool->free_num -= 1;
    mod_pool->used_num += 1;

    head_size = (td_u32)sizeof(sap_mod_head);
    body_size = mod_pool->node_size - head_size;
    mod_body = (td_void *)((td_uintptr_t)mod_node + head_size);

    (td_void)memset_s(mod_body, body_size, 0, body_size);

    mod_node->mid.bits.type = mod_pool->mod_type;
    mod_node->mid.bits.reserved = SAP_MOD_PREFIX;

    *module = mod_node;
#if (SAP_CHIP_TYPE != socmn1)
    if (mod_node->mid.bits.type != SAP_MOD_MSG) { /* msg模块的alloc在ipc初始化之前，此时打印ipc log会导致报错 */
        sap_alert_log_h32(mod_node->mid.u32);
    }
#endif
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_free(sap_mod_pool_handle pool, sap_mod_handle module)
{
    td_s32 ret;
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = module;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    sys_check_null_ptr_return(module);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
#if (SAP_CHIP_TYPE != socmn1)
    sap_alert_log_h32(mod_node->mid.u32);
#endif
    if (mod_pool->free_num >= mod_pool->node_count) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        sap_err_log_u32(mod_pool->free_num);
        sap_err_log_u32(mod_pool->node_count);
        return ERR_SYS_INVALID_ID;
    }

    ret = core_mod_check_mod_id(mod_node->mid, mod_pool);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ret;
    }

    pool_rmv_mode(&mod_pool->used_list, mod_node);
    pool_add_tail(&mod_pool->free_list, mod_node);

    mod_pool->free_num += 1;
    mod_pool->used_num -= 1;

    mod_node->mid.bits.type = SAP_MOD_NULL;

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_alloc_by_id(sap_mod_pool_handle pool, td_u32 mod_id, sap_mod_handle *module)
{
    td_s32 ret;
    td_u32 head_size;
    td_u32 body_size;
    td_u32 offset;
    sap_mod_id mid = { .u32 = mod_id };
    td_void *mod_body = TD_NULL;
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = TD_NULL;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    sys_check_null_ptr_return(module);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    ret = core_mod_check_mod_id(mid, mod_pool);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ret;
    }

    offset = (mid.bits.index * mod_pool->node_size);
    mod_node = (sap_mod_head *)((td_void *)&mod_pool->pool_start[offset]);
    if (mod_node == TD_NULL) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ERR_SYS_NULL_PTR;
    }

    if (mod_node->mid.bits.type == SAP_MOD_NULL) {
        pool_rmv_mode(&mod_pool->free_list, mod_node);
        pool_add_tail(&mod_pool->used_list, mod_node);
        mod_pool->free_num -= 1;
        mod_pool->used_num += 1;

        head_size = (td_u32)sizeof(sap_mod_head);
        body_size = mod_pool->node_size - head_size;
        mod_body = (td_void *)((td_uintptr_t)mod_node + head_size);
        (td_void)memset_s(mod_body, body_size, 0, body_size);
        mod_node->mid.bits.type = mod_pool->mod_type;
        mod_node->mid.bits.reserved = SAP_MOD_PREFIX;
    }

    *module = mod_node;
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_free_by_id(sap_mod_pool_handle pool, td_u32 mod_id)
{
    td_s32 ret;
    td_u32 offset;
    sap_mod_id mid = { .u32 = mod_id };
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = TD_NULL;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    if (mod_pool->free_num >= mod_pool->node_count) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        sap_err_log_u32(mod_pool->free_num);
        sap_err_log_u32(mod_pool->node_count);
        return ERR_SYS_INVALID_ID;
    }

    ret = core_mod_check_mod_id(mid, mod_pool);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ret;
    }

    offset = (mid.bits.index * mod_pool->node_size);
    mod_node = (sap_mod_head *)((td_void *)&mod_pool->pool_start[offset]);
    if (mod_node == TD_NULL) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
        return ERR_SYS_NULL_PTR;
    }

    pool_rmv_mode(&mod_pool->used_list, mod_node);
    pool_add_tail(&mod_pool->free_list, mod_node);

    mod_pool->free_num += 1;
    mod_pool->used_num -= 1;

    mod_node->mid.bits.type = SAP_MOD_NULL;

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    audio_func_exit();

    return AUDIO_SUCCESS;
}

td_void core_mod_register_event_proc(sap_mod_handle module, core_mod_event_proc event_proc, td_void *context)
{
    audio_func_enter();
    if (module != TD_NULL) {
        module->mod_event_proc = event_proc;
        module->context = context;
    }
    audio_func_exit();
}

td_s32 core_mod_report_event(sap_mod_handle module, td_s32 event, td_void *param)
{
    sys_check_null_ptr_return(module);
    sys_check_null_ptr_return(module->mod_event_proc);
    return module->mod_event_proc(module, event, param, module->context);
}

td_s32 core_mod_set_eid(sap_mod_handle module, td_u32 ext_id)
{
    audio_func_enter();
    sys_check_null_ptr_return(module);
    module->ext_id = ext_id;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_mod_get_module_by_eid(sap_mod_pool_handle pool, td_u32 ext_id, sap_mod_handle *module)
{
    td_u8 i;
    sap_mod_pool *mod_pool = pool;
    sap_mod_head *mod_node = TD_NULL;

    audio_func_enter();
    sys_check_null_ptr_return(pool);
    sys_check_null_ptr_return(module);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    mod_node = mod_pool->used_list.next;
    for (i = 0; i < mod_pool->used_num; i++) {
        if (mod_node->ext_id == ext_id) {
            *module = mod_node;
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
            audio_func_exit();
            return AUDIO_SUCCESS;
        }
        mod_node = mod_node->next;
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    audio_func_exit();
    return ERR_SYS_INVALID_ID;
}

td_s32 core_mod_broadcast_event(sap_mod_pool_handle pool, td_s32 event, td_void *param)
{
    td_u8 i;
    sap_mod_head *mod_node = TD_NULL;
    sap_mod_pool *mod_pool = pool;

    sys_check_null_ptr_return(pool);
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&mod_pool->lock);

    mod_node = mod_pool->used_list.next;
    for (i = 0; i < mod_pool->used_num; i++) {
        mod_node->mod_event_proc((td_void *)mod_node, event, param, mod_node->context);
        mod_node = mod_node->next;
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&mod_pool->lock);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
