/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: source file of ring buf
 * Author: audio
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_rbuf.h"
#include "core_module.h"
#include "core_shrb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef union {
    struct {
        td_u8 core_id; /* ::audio_core_id */
        td_u8 index;
        td_u8 reserved0;
        td_u8 reserved1;
    } u;
    td_u32 u32;
} core_shrb_id;

static inline td_u32 core_shrb_core_id(td_u32 shrb_id)
{
    core_shrb_id tmp = {
        .u32 = shrb_id,
    };

    return tmp.u.core_id;
}

static inline td_u32 core_shrb_index(td_u32 shrb_id)
{
    core_shrb_id tmp = {
        .u32 = shrb_id,
    };

    return tmp.u.index;
}

/*
 * struct core_shrb_inst is used for each rbuf instant. This struct is stored in ctrl block, and sharing by two cores.
 * For each rbuf instance, we need to alloc a mmz buf.
 * dma_buf & fd: Recording the mmz buf which is archieved by dma_buf after kernel 4.14.
 */
typedef struct sap_core_shrb_inst {
    td_u32 shrb_id; /* ::core_shrb_id */
    td_u32 mod_id;  /* ::sap_mod_id */

    td_u32 data_buf_size;
    td_u32 data_buf_tail_size;
    td_u32 data_buf_write;
    td_u32 data_buf_read;

    td_u32 info_buf_size;
    td_u32 info_buf_write;
    td_u32 info_buf_read;
    td_bool eos;

    td_u32 mmz_size;
    td_s32 mmz_fd;
    td_u64 dma_buf; /* kernel dma buffer for each shrb inst */
    td_u64 phys_addr;
    td_u64 virt_addr;
    td_u64 virt_addr_tab[AUDIO_CORE_MAX];
} core_shrb_inst;

/*
 * struct core_shrb_mgr is used for each core to manage the share ring buffer.
 * It's unique in different core with the same mem to store each shrb inst in ctrl block.
 * The mem for ctrl block is alloced with mmz buf. All cores use this same mem to manage shrb instance.
 * dma_buf & fd: Recording the mmz buf which is archieved by dma_buf after kernel 4.14.
 */
typedef struct {
    td_u8 core_id;
    td_u8 mem_type; /* ::shrb_mem_type */
    td_u8 shrb_cnt;

    td_bool cache;
    td_u32 mmz_size;
    td_u64 dma_buf;
    td_u64 phys_addr;
    td_u64 base_addr;

    shrb_handle shrb_list;
    rbuf_handle data_rbuf[CORE_RBUF_MAX_NUM];
    rbuf_handle info_rbuf[CORE_RBUF_MAX_NUM];
} core_shrb_mgr;

static td_u32 g_shrb_init_cnt = 0;
static core_shrb_mgr g_shrb_mgr = {
    .core_id = 0,
    .mem_type = 0,
    .shrb_cnt = 0,
    .base_addr = 0LL,
    .dma_buf = 0LL,
};

static inline td_s32 shrb_get_mgr(core_shrb_mgr **shrb_mgr)
{
    if (g_shrb_mgr.base_addr == 0LL) {
        return ERR_SYS_NOT_INIT;
    }

    *shrb_mgr = &g_shrb_mgr;
    return AUDIO_SUCCESS;
}

static td_s32 shrb_get_id(core_shrb_mgr *shrb_mgr, td_u32 mod_id, shrb_handle *shrb)
{
    td_s32 i;
    shrb_handle shrb_node = shrb_mgr->shrb_list;

    for (i = 0; i < shrb_mgr->shrb_cnt; i++) {
        if (shrb_node->mod_id == mod_id) {
            *shrb = shrb_node;
            return AUDIO_SUCCESS;
        }
        shrb_node++;
    }

    return ERR_SYS_INVALID_ID;
}

static td_void shrb_get_free_inst(core_shrb_mgr *shrb_mgr, shrb_handle *shrb, td_u8 *index)
{
    td_u8 i;
    shrb_handle shrb_node = shrb_mgr->shrb_list;

    *shrb = TD_NULL;

    for (i = 0; i < shrb_mgr->shrb_cnt; i++) {
        if (shrb_node->shrb_id == 0) {
            *index = i;
            *shrb = shrb_node;
            return;
        }
        shrb_node++;
    }
}

static inline td_void shrb_set_mmz_param(shrb_handle shrb, audio_buffer *mmz_buf)
{
    shrb->mmz_fd = mmz_buf->fd;
    shrb->mmz_size = mmz_buf->size;
    shrb->phys_addr = mmz_buf->phys_addr;               /* physic addr */
    shrb->virt_addr = (td_uintptr_t)mmz_buf->virt_addr; /* kernel virtual addr */
    shrb->dma_buf = (td_uintptr_t)mmz_buf->dma_buf;     /* kernel dma buffer */
}

static td_void shrb_get_init_cfg(td_ulong init_reg, shrb_init_cfg *init_cfg)
{
    td_u32 i;
    volatile td_u8 *to = (volatile td_u8 *)init_cfg;
    volatile td_u8 *from = (volatile td_u8 *)audio_osal_address_map(init_reg, sizeof(shrb_init_cfg));
    if (from == TD_NULL) {
        return;
    }

    for (i = 0; i < sizeof(shrb_init_cfg); i++) {
        *(to + i) = *(from + i);
    }

    audio_osal_address_unmap(init_reg, (td_void *)from);
}

static td_void shrb_set_init_cfg(td_ulong init_reg, const shrb_init_cfg *init_cfg)
{
    td_u32 i;
    volatile td_u8 *from = (volatile td_u8 *)init_cfg;
    volatile td_u8 *to = (volatile td_u8 *)audio_osal_address_map(init_reg, sizeof(shrb_init_cfg));
    if (to == TD_NULL) {
        return;
    }

    for (i = 0; i < sizeof(shrb_init_cfg); i++) {
        *(to + i) = *(from + i);
    }

    audio_osal_dcache_region_wb(init_reg, (td_void *)to, sizeof(shrb_init_cfg));
    audio_osal_address_unmap(init_reg, (td_void *)to);
}

td_void core_shrb_get_init_cfg(td_u32 init_reg, shrb_init_cfg *init_cfg)
{
    audio_buffer mmz_buf;

    if (init_cfg == TD_NULL) {
        return;
    }

    shrb_get_init_cfg(init_reg, init_cfg);

    if (init_cfg->fd > 0) {
        mmz_buf.size = init_cfg->size;
        mmz_buf.cache = init_cfg->cache;
        mmz_buf.phys_addr = init_cfg->phys_addr;
        mmz_buf.dma_buf = (td_void *)(td_uintptr_t)init_cfg->dma_buf;
        audio_osal_mmz_map(&mmz_buf);
        init_cfg->fd = mmz_buf.fd;
    }

    sap_trace_log_s32(init_cfg->mem_type);
    sap_trace_log_bool(init_cfg->cache);
    sap_trace_log_h32(init_cfg->size);
    sap_trace_log_h64(init_cfg->phys_addr);
    sap_trace_log_h64(init_cfg->dma_buf);
    sap_trace_log_s32(init_cfg->fd);
}

td_void core_shrb_set_init_cfg(td_u32 init_reg, const shrb_init_cfg *init_cfg)
{
    if (init_cfg == TD_NULL) {
        return;
    }

    sap_trace_log_s32(init_cfg->mem_type);
    sap_trace_log_bool(init_cfg->cache);
    sap_trace_log_h32(init_cfg->size);
    sap_trace_log_h64(init_cfg->phys_addr);
    sap_trace_log_h64(init_cfg->dma_buf);
    sap_trace_log_s32(init_cfg->fd);

    shrb_set_init_cfg(init_reg, init_cfg);
}

static td_s32 shrb_inst_list_alloc(audio_buffer *mmz_buf, shrb_init_cfg *init_cfg)
{
    td_s32 ret;
    td_u32 inst_list_size;
    core_shrb_mgr *shrb_mgr = &g_shrb_mgr;

    audio_func_enter();

    inst_list_size = (td_u32)(sizeof(core_shrb_inst) * shrb_mgr->shrb_cnt);
    ret = audio_osal_mmz_alloc(mmz_buf, audio_align_up8(inst_list_size));
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(audio_osal_mmz_alloc, ret);
        return ret;
    }
    (td_void)memset_s((td_void *)mmz_buf->virt_addr, mmz_buf->size, 0, mmz_buf->size);

    init_cfg->mem_type = SHRB_MEM_MMZ;
    init_cfg->size = mmz_buf->size;
    init_cfg->cache = mmz_buf->cache;
    init_cfg->phys_addr = mmz_buf->phys_addr;
    init_cfg->dma_buf = (td_u64)(td_uintptr_t)mmz_buf->dma_buf;
    init_cfg->fd = mmz_buf->fd;

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_void shrb_inst_list_map(audio_buffer *mmz_buf, shrb_init_cfg *init_cfg)
{
    mmz_buf->size = init_cfg->size;
    mmz_buf->cache = init_cfg->cache;
    mmz_buf->phys_addr = init_cfg->phys_addr;
    mmz_buf->dma_buf = (td_void *)(td_uintptr_t)init_cfg->dma_buf;
    mmz_buf->fd = init_cfg->fd;
    audio_osal_mmz_map(mmz_buf);
}

audio_core_id core_shrb_get_core_id(td_void)
{
    td_s32 ret;
    audio_core_id core_id = AUDIO_CORE_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    ret = shrb_get_mgr(&shrb_mgr);
    core_id = (ret == AUDIO_SUCCESS ? (audio_core_id)shrb_mgr->core_id : AUDIO_CORE_NULL);
    return core_id;
}

td_s32 core_shrb_init(audio_core_id core_id, shrb_init_cfg *init_cfg)
{
    td_s32 ret;
    audio_buffer mmz_buf;
    core_shrb_mgr *shrb_mgr = &g_shrb_mgr;

    audio_func_enter();

    if (init_cfg == TD_NULL) {
        return ERR_SYS_INVALID_PARAM;
    }

    if (g_shrb_init_cnt > 0) {
        if (shrb_mgr->core_id != (td_u8)core_id) {
            sap_fatal_log_h32(core_id);
            sap_fatal_log_h32(shrb_mgr->core_id);
            return ERR_SYS_INIT_FAILED;
        }
        g_shrb_init_cnt++;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    (td_void)memset_s((td_void *)shrb_mgr, sizeof(core_shrb_mgr), 0, sizeof(core_shrb_mgr));
    shrb_mgr->core_id = (td_u8)core_id;
    shrb_mgr->mem_type = (td_u8)init_cfg->mem_type;
    shrb_mgr->shrb_cnt = (td_u8)SHRB_BUFFER_MAX_NUM;

    if (init_cfg->mem_type == SHRB_MEM_NULL) {
        ret = shrb_inst_list_alloc(&mmz_buf, init_cfg);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(shrb_inst_list_alloc, ret);
            return ret;
        }
    } else {
        shrb_inst_list_map(&mmz_buf, init_cfg);
    }

    if (mmz_buf.virt_addr == TD_NULL) {
        sap_fatal_log_h32(init_cfg->mem_type);
        sap_fatal_log_s32(core_id);
        return ERR_SYS_MEM_ALLOC;
    }

    shrb_mgr->cache = mmz_buf.cache;
    shrb_mgr->mmz_size = mmz_buf.size;
    shrb_mgr->phys_addr = mmz_buf.phys_addr;
    shrb_mgr->dma_buf = (td_u64)(td_uintptr_t)mmz_buf.dma_buf;
    shrb_mgr->base_addr = (td_u64)(td_uintptr_t)mmz_buf.virt_addr;
    shrb_mgr->shrb_list = (shrb_handle)mmz_buf.virt_addr;

    sap_trace_log_h64(shrb_mgr->base_addr);

    g_shrb_init_cnt++;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_deinit(td_void)
{
    td_s32 ret;
    audio_buffer mmz_buf;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    audio_func_enter();
    if (g_shrb_init_cnt == 0) {
        audio_func_exit();
        return AUDIO_SUCCESS;
    }
    if (g_shrb_init_cnt > 1) {
        g_shrb_init_cnt--;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    mmz_buf.cache = shrb_mgr->cache;
    mmz_buf.size = shrb_mgr->mmz_size;
    mmz_buf.phys_addr = shrb_mgr->phys_addr;
    mmz_buf.dma_buf = (td_void *)(td_uintptr_t)shrb_mgr->dma_buf;
    mmz_buf.virt_addr = (td_u8 *)(td_uintptr_t)shrb_mgr->base_addr;
    if (shrb_mgr->mem_type == SHRB_MEM_NULL) {
        audio_osal_mmz_release(&mmz_buf);
    } else {
        audio_osal_mmz_unmap(&mmz_buf);
    }

    (td_void)memset_s((td_void *)shrb_mgr, sizeof(core_shrb_mgr), 0, sizeof(core_shrb_mgr));

    g_shrb_init_cnt--;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_get_id(td_u32 mod_id, shrb_handle *shrb)
{
    td_s32 ret;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    if (shrb == TD_NULL || mod_id == 0) {
        sap_err_log_h32(mod_id);
        return ERR_SYS_INVALID_PARAM;
    }

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    ret = shrb_get_id(shrb_mgr, mod_id, shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(mod_id);
        sap_err_log_fun(shrb_get_id, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_alloc(shrb_handle *handle, td_u32 mod_id, const shrb_attr *attr)
{
    td_s32 ret;
    td_u8 index;
    audio_buffer mmz_buf;
    core_shrb_id inst_id;
    shrb_handle shrb = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    audio_func_enter();
    if (handle == TD_NULL || attr == TD_NULL || mod_id == 0) {
        sap_err_log_h32(mod_id);
        return ERR_SYS_INVALID_PARAM;
    }

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    /* User should make sure user_id is unique */
    ret = shrb_get_id(shrb_mgr, mod_id, &shrb);
    if (ret == AUDIO_SUCCESS) {
        sap_err_log_h32(mod_id);
        return ERR_SYS_INST_BUSY;
    }

    shrb_get_free_inst(shrb_mgr, &shrb, &index);
    if (shrb == TD_NULL) {
        sap_err_log_info("call shrb_get_free_inst failed");
        return ERR_SYS_INST_FULL;
    }

    (td_void)memset_s(shrb, sizeof(*shrb), 0, sizeof(core_shrb_inst));

    inst_id.u32 = 0xffffffff;
    inst_id.u.index = index;
    inst_id.u.core_id = shrb_mgr->core_id;

    ret = audio_osal_mmz_alloc(&mmz_buf, attr->data_buf_size + attr->data_buf_tail_size + attr->info_buf_size);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(audio_osal_mmz_alloc, ret);
        return ret;
    }

    shrb_set_mmz_param(shrb, &mmz_buf);
    shrb->mod_id = mod_id;
    shrb->shrb_id = inst_id.u32;
    shrb->data_buf_size = attr->data_buf_size;
    shrb->data_buf_tail_size = attr->data_buf_tail_size;
    shrb->info_buf_size = attr->info_buf_size;

    *handle = shrb;

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_free(shrb_handle shrb)
{
    td_s32 i, ret;
    audio_buffer mmz_buf;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    audio_func_enter();
    sys_check_null_ptr_return(shrb);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    if (core_shrb_core_id(shrb->shrb_id) != shrb_mgr->core_id) {
        sap_err_log_h32(shrb->shrb_id);
        sap_err_log_h32(shrb_mgr->core_id);
        return ERR_SYS_INVALID_ID;
    }

    for (i = 0; i < AUDIO_CORE_MAX; i++) {
        if (shrb->virt_addr_tab[i] != 0LL) {
            sap_err_log_u32(i);
            sap_err_log_h64(shrb->virt_addr_tab[i]);
            return ERR_SYS_INST_BUSY;
        }
    }

    /* Owner unmapes share memory, and then realses it. */
    mmz_buf.cache = TD_FALSE;
    mmz_buf.size = shrb->mmz_size;
    mmz_buf.phys_addr = shrb->phys_addr;
    mmz_buf.virt_addr = (td_u8 *)(td_uintptr_t)shrb->virt_addr;
    mmz_buf.dma_buf = (td_void *)(td_uintptr_t)shrb->dma_buf;
    mmz_buf.fd = shrb->mmz_fd;
    audio_osal_mmz_release(&mmz_buf);

    (td_void)memset_s(shrb, sizeof(*shrb), 0, sizeof(core_shrb_inst));

    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 shrb_destroy_data_buf(shrb_handle shrb)
{
    td_s32 ret;
    rbuf_handle buf_handle = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    buf_handle = shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)];
    ret = ring_buf_destroy(buf_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_trace_log_h32(buf_handle);
        sap_err_log_fun(ring_buf_destroy, ret);
        return ret;
    }

    shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)] = TD_NULL;
    return AUDIO_SUCCESS;
}

static td_s32 shrb_create_data_buf(shrb_handle shrb, rbuf_copy_mode copy_mode)
{
    td_s32 ret;
    rbuf_handle buf_handle = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;
    td_u8 *buf = TD_NULL;
    ring_buf buf_attr = {
        .alloc_type = RING_BUF_ALLOC_EXTERNAL,
        .copy_mode = copy_mode,
        .data = TD_NULL,
        .size = shrb->data_buf_size,
        .tail_size = shrb->data_buf_tail_size,
        .read_pos = &shrb->data_buf_read,
        .write_pos = &shrb->data_buf_write,
    };

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    buf = (td_u8 *)(td_uintptr_t)shrb->virt_addr_tab[shrb_mgr->core_id];
    buf_attr.data = buf;

    ret = ring_buf_create(&buf_handle, &buf_attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_create, ret);
        return ret;
    }

    shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)] = buf_handle;
    return AUDIO_SUCCESS;
}

static td_s32 shrb_destroy_info_buf(shrb_handle shrb)
{
    td_s32 ret;
    rbuf_handle buf_handle = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    buf_handle = shrb_mgr->info_rbuf[core_shrb_index(shrb->shrb_id)];
    if (buf_handle == TD_NULL) { /* info buffer is not created */
        return AUDIO_SUCCESS;
    }

    ret = ring_buf_destroy(buf_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_trace_log_h32(buf_handle);
        sap_err_log_fun(ring_buf_destroy, ret);
        return ret;
    }

    shrb_mgr->info_rbuf[core_shrb_index(shrb->shrb_id)] = TD_NULL;
    return AUDIO_SUCCESS;
}

static td_s32 shrb_create_info_buf(shrb_handle shrb, rbuf_copy_mode copy_mode)
{
    td_s32 ret;
    rbuf_handle buf_handle = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;
    td_u8 *buf = TD_NULL;
    ring_buf buf_attr = {
        .alloc_type = RING_BUF_ALLOC_EXTERNAL,
        .copy_mode = copy_mode,
        .data = TD_NULL,
        .size = shrb->info_buf_size,
        .tail_size = 0,
        .read_pos = &shrb->info_buf_read,
        .write_pos = &shrb->info_buf_write,
    };

    /* do not need info buffer */
    if (shrb->info_buf_size == 0) {
        return AUDIO_SUCCESS;
    }

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    buf = (td_u8 *)(td_uintptr_t)shrb->virt_addr_tab[shrb_mgr->core_id];
    buf_attr.data = buf + shrb->data_buf_size + shrb->data_buf_tail_size;

    ret = ring_buf_create(&buf_handle, &buf_attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_create, ret);
        return ret;
    }

    shrb_mgr->info_rbuf[core_shrb_index(shrb->shrb_id)] = buf_handle;
    return AUDIO_SUCCESS;
}

static td_s32 shrb_destroy_buf(shrb_handle shrb)
{
    td_s32 ret;

    ret = shrb_destroy_info_buf(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_destroy_info_buf, ret);
        return ret;
    }

    ret = shrb_destroy_data_buf(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_destroy_data_buf, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

static td_s32 shrb_create_buf(shrb_handle shrb, rbuf_copy_mode copy_mode)
{
    td_s32 ret;

    ret = shrb_create_data_buf(shrb, copy_mode);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_create_data_buf, ret);
        return ret;
    }

    ret = shrb_create_info_buf(shrb, copy_mode);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_create_info_buf, ret);
        (td_void)shrb_destroy_data_buf(shrb);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_attach(shrb_handle shrb)
{
    td_s32 ret;
    rbuf_copy_mode copy_mode;
    audio_buffer mmz_buf;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    audio_func_enter();

    sys_check_null_ptr_return(shrb);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    if (shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)] != TD_NULL) {
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    if (core_shrb_core_id(shrb->shrb_id) == shrb_mgr->core_id) {
        shrb->virt_addr_tab[shrb_mgr->core_id] = shrb->virt_addr;
        copy_mode = (shrb_mgr->core_id == AUDIO_CORE_DSP0) ? RBUF_CPY_DSP_TODSP : RBUF_CPY_ARM_TOARM;
    } else {
        mmz_buf.fd = -1;
        mmz_buf.size = shrb->mmz_size;
        mmz_buf.cache = TD_FALSE;
        mmz_buf.virt_addr = TD_NULL;
        mmz_buf.phys_addr = shrb->phys_addr;
        mmz_buf.dma_buf = (td_void *)(td_uintptr_t)shrb->dma_buf;
        audio_osal_mmz_map(&mmz_buf);
        shrb->virt_addr_tab[shrb_mgr->core_id] = (td_u64)(td_uintptr_t)mmz_buf.virt_addr;
        copy_mode = (shrb_mgr->core_id == AUDIO_CORE_DSP0) ? RBUF_CPY_DSP_TOARM : RBUF_CPY_ARM_TODSP;
    }

    ret = shrb_create_buf(shrb, copy_mode);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_create_buf, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_detach(shrb_handle shrb)
{
    td_s32 ret;
    rbuf_handle data_buf = TD_NULL;
    audio_buffer mmz_buf;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    audio_func_enter();

    sys_check_null_ptr_return(shrb);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    data_buf = shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)];
    if (data_buf == TD_NULL) {
        sap_trace_log_h32(shrb);
        audio_log_info("shrb already detach.");
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = shrb_destroy_buf(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_destroy_buf, ret);
        return ret;
    }

    if (core_shrb_core_id(shrb->shrb_id) != shrb_mgr->core_id) {
        mmz_buf.fd = -1; /* linux must use fd created at mmz map */
        mmz_buf.size = shrb->mmz_size;
        mmz_buf.cache = TD_FALSE;
        mmz_buf.phys_addr = shrb->phys_addr;
        mmz_buf.virt_addr = (td_u8 *)(td_uintptr_t)shrb->virt_addr_tab[shrb_mgr->core_id];
        audio_osal_mmz_unmap(&mmz_buf);
    }
    shrb->virt_addr_tab[shrb_mgr->core_id] = 0LL;

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_get_rbuf(shrb_handle shrb, shrb_buffer *buf)
{
    td_s32 ret;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(shrb);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    buf->data_buf = shrb_mgr->data_rbuf[core_shrb_index(shrb->shrb_id)];
    buf->info_buf = shrb_mgr->info_rbuf[core_shrb_index(shrb->shrb_id)];
    return AUDIO_SUCCESS;
}

static inline td_bool shrb_check_info_buf_free(rbuf_handle info_buf)
{
    if (info_buf == TD_NULL) {
        /* return true when adp do not pass frame info */
        return TD_TRUE;
    }

    return ring_buf_query_free(info_buf) > sizeof(audio_frame);
}

static td_bool shrb_check_buf_free(const shrb_buffer *buf, td_u32 size)
{
    return shrb_check_info_buf_free(buf->info_buf) &&
        ring_buf_query_free(buf->data_buf) >= size;
}

static td_bool shrb_check_info_buf_data(rbuf_handle info_buf)
{
    if (info_buf == TD_NULL) {
        /* return true when adp do not pass frame info */
        return TD_TRUE;
    }

    return ring_buf_query_busy(info_buf) >= sizeof(audio_frame);
}

td_s32 core_shrb_query_size(shrb_handle shrb, td_u32 *data_size, td_u32 *info_size)
{
    td_s32 ret;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    sys_check_null_ptr_return(data_size);
    sys_check_null_ptr_return(info_size);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        return ret;
    }

    if (shrb == TD_NULL) {
        /* mmz buf */
        *info_size = 0;
        *data_size = shrb_mgr->mmz_size;
    } else {
        /* share ring buf */
        *info_size = shrb->info_buf_size;
        *data_size = shrb->data_buf_size + shrb->data_buf_tail_size;
    }
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_query_free(const shrb_buffer *buf, td_u32 *free_size)
{
    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(free_size);

    *free_size = 0;
    if (!shrb_check_info_buf_free(buf->info_buf)) {
        return AUDIO_SUCCESS;
    }

    *free_size = ring_buf_query_free(buf->data_buf);
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_query_busy(const shrb_buffer *buf, td_u32 *data_size)
{
    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(data_size);

    *data_size = 0;
    if (!shrb_check_info_buf_data(buf->info_buf)) {
        return AUDIO_SUCCESS;
    }

    *data_size = ring_buf_query_busy(buf->data_buf);
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_get_buffer(const shrb_buffer *buf, audio_stream_buf *stream)
{
    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(stream);

    if (!shrb_check_info_buf_free(buf->info_buf)) {
        return ERR_SYS_BUF_FULL;
    }

    stream->eos = TD_FALSE;
    stream->pkg_loss = TD_FALSE;

    return ring_buf_linear_get_buffer(buf->data_buf, (td_u32 *)&stream->data, &stream->size);
}

td_s32 core_shrb_put_buffer(const shrb_buffer *buf, const audio_stream_buf *stream)
{
    td_s32 ret;
    audio_frame frame = {
        .interleaved = TD_FALSE,
        .channels = 0,
        .bit_depth = 0,
        .sample_rate = 0,
        .pcm_buffer = TD_NULL,
        .pcm_samples = 0,
        .frame_index = 0,
    };

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(stream);

    if (stream->size == 0) {
        return AUDIO_SUCCESS;
    }

    if (!shrb_check_info_buf_free(buf->info_buf)) {
        return ERR_SYS_BUF_FULL;
    }

    ret = ring_buf_linear_put_buffer(buf->data_buf, stream->size);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_linear_put_buffer, ret);
        return ret;
    }

    if (buf->info_buf != TD_NULL) {
        frame.bits_buffer = (td_s32 *)stream->data;
        frame.bits_bytes = stream->size;
        frame.pts = stream->pts;
        frame.eos = stream->eos;
        frame.pkg_loss = stream->pkg_loss;

        ret = ring_buf_write(buf->info_buf, (const td_u8 *)&frame, sizeof(frame));
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(ring_buf_write, ret);
            return ret;
        }
    }

    return AUDIO_SUCCESS;
}

static td_s32 core_shrb_check_frame_attr(const audio_frame *frame)
{
    td_s32 ret;

    ret = core_check_sample_rate_range(frame->sample_rate);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = core_check_bit_depth(frame->bit_depth);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = core_check_channel(frame->channels);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_send_frame(const shrb_buffer *buf, const audio_frame *frame)
{
    td_s32 ret;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(frame);

    ret = core_shrb_check_frame_attr(frame);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_check_frame_attr, ret);
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_check_bool(frame->interleaved);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_check_bool(frame->eos);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_check_bool(frame->pkg_loss);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    if (frame->bits_bytes == 0) {
        sap_warn_log_u32(frame->bits_bytes);
        return AUDIO_SUCCESS;
    }

    if (!shrb_check_buf_free(buf, frame->bits_bytes)) {
        return ERR_SYS_BUF_FULL;
    }

    if (buf->info_buf != TD_NULL) {
        ret = ring_buf_write(buf->info_buf, (const td_u8 *)frame, sizeof(*frame));
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(ring_buf_write, ret);
            return ret;
        }
    }

    ret = ring_buf_write(buf->data_buf, (const td_u8 *)frame->bits_buffer, frame->bits_bytes);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_write, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_send_stream(const shrb_buffer *buf, const audio_stream_buf *stream)
{
    td_s32 ret;
    audio_frame frame = {
        .interleaved = TD_FALSE,
        .channels = 0,
        .bit_depth = 0,
        .sample_rate = 0,
        .pcm_buffer = TD_NULL,
        .pcm_samples = 0,
        .frame_index = 0,
    };

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(stream);

    if (stream->size == 0) {
        return AUDIO_SUCCESS;
    }

    ret = core_check_bool(stream->eos);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    ret = core_check_bool(stream->pkg_loss);
    if (ret != AUDIO_SUCCESS) {
        return ERR_ADP_INVALID_PARAM;
    }

    if (!shrb_check_buf_free(buf, stream->size)) {
        return ERR_SYS_BUF_FULL;
    }

    if (buf->info_buf != TD_NULL) {
        frame.bits_buffer = (td_s32 *)stream->data;
        frame.bits_bytes = stream->size;
        frame.pts = stream->pts;
        frame.eos = stream->eos;
        frame.pkg_loss = stream->pkg_loss;

        ret = ring_buf_write(buf->info_buf, (const td_u8 *)&frame, sizeof(frame));
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(ring_buf_write, ret);
            return ret;
        }
    }

    ret = ring_buf_write(buf->data_buf, stream->data, stream->size);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_write, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

static td_s32 shrb_acquire_frame_info(rbuf_handle info_buf, audio_frame *frame)
{
    if (info_buf == TD_NULL) {
        frame->interleaved = TD_TRUE;
        frame->channels = 0;
        frame->bit_depth = 0;
        frame->sample_rate = 0;
        frame->pts = 0LL;
        frame->pcm_samples = 0;
        frame->bits_buffer = TD_NULL;
        frame->bits_bytes = 0;
        frame->pcm_buffer = TD_NULL;
        frame->frame_index = 0;
        frame->eos = TD_FALSE;
        frame->pkg_loss = TD_FALSE;

        return AUDIO_SUCCESS;
    } else {
        return ring_buf_peek(info_buf, (td_u8 *)frame, sizeof(*frame));
    }
}

td_s32 core_shrb_acquire_frame(const shrb_buffer *buf, audio_frame *frame)
{
    td_s32 ret;
    td_u32 data_size = 0;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(frame);

    if (!shrb_check_info_buf_data(buf->info_buf)) {
        return ERR_SYS_BUF_EMPTY;
    }

    ret = shrb_acquire_frame_info(buf->info_buf, frame);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_acquire_frame_info, ret);
        return ret;
    }

    ret = ring_buf_linear_acquire_data(buf->data_buf, (td_u32 *)&frame->bits_buffer, &data_size);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    if (buf->info_buf == TD_NULL) {
        frame->bits_bytes = data_size;
        return AUDIO_SUCCESS;
    }

    if (data_size < frame->bits_bytes) {
        return ERR_SYS_BUF_EMPTY;
    }

    return AUDIO_SUCCESS;
}

static td_s32 shrb_release_frame_info(rbuf_handle info_buf, const audio_frame *frame)
{
    td_s32 ret;
    audio_frame peek_frame;

    if (info_buf == TD_NULL) {
        return AUDIO_SUCCESS;
    }

    ret = ring_buf_peek(info_buf, (td_u8 *)&peek_frame, sizeof(peek_frame));
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_peek, ret);
        return ret;
    }

    if (peek_frame.bits_bytes != frame->bits_bytes ||
        peek_frame.pcm_samples != frame->pcm_samples) {
        sap_err_log_h32(peek_frame.bits_bytes);
        sap_err_log_h32(peek_frame.pcm_samples);
        sap_err_log_h32(frame->bits_bytes);
        sap_err_log_h32(frame->pcm_samples);
        return ERR_SYS_MEM_OVERLOAD;
    }

    ret = ring_buf_poke(info_buf, sizeof(peek_frame));
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_poke, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_release_frame(const shrb_buffer *buf, const audio_frame *frame)
{
    td_s32 ret;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(frame);

    ret = shrb_release_frame_info(buf->info_buf, frame);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_release_frame_info, ret);
        return ret;
    }

    return ring_buf_linear_release_data(buf->data_buf, frame->bits_bytes);
}

static td_s32 shrb_acquire_stream_info(rbuf_handle info_buf, audio_stream_buf *stream)
{
    td_s32 ret;
    audio_frame peek_frame;

    if (info_buf == TD_NULL) {
        stream->data = TD_NULL;
        stream->size = 0;
        stream->pts = 0LL;
        stream->eos = TD_FALSE;
        stream->pkg_loss = TD_FALSE;

        return AUDIO_SUCCESS;
    } else {
        ret = ring_buf_peek(info_buf, (td_u8 *)&peek_frame, sizeof(peek_frame));
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(ring_buf_peek, ret);
            return ret;
        }

        stream->data = (td_u8 *)peek_frame.bits_buffer;
        stream->size = peek_frame.bits_bytes;
        stream->pts = peek_frame.pts;
        stream->eos = peek_frame.eos;
        stream->pkg_loss = peek_frame.pkg_loss;

        return AUDIO_SUCCESS;
    }
}

td_s32 core_shrb_acquire_stream(const shrb_buffer *buf, audio_stream_buf *stream)
{
    td_s32 ret;
    td_u32 data_size = 0;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(stream);

    if (!shrb_check_info_buf_data(buf->info_buf)) {
        return ERR_SYS_BUF_EMPTY;
    }

    ret = shrb_acquire_stream_info(buf->info_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_acquire_stream_info, ret);
        return ret;
    }

    ret = ring_buf_linear_acquire_data(buf->data_buf, (td_u32 *)&stream->data, &data_size);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    if (buf->info_buf == TD_NULL) {
        stream->size = data_size;
        return AUDIO_SUCCESS;
    }

    if (data_size < stream->size) {
        return ERR_SYS_BUF_EMPTY;
    }

    return AUDIO_SUCCESS;
}

static td_s32 shrb_release_stream_info(rbuf_handle info_buf, const audio_stream_buf *stream)
{
    td_s32 ret;
    audio_frame peek_frame;

    if (info_buf == TD_NULL) {
        return AUDIO_SUCCESS;
    }

    ret = ring_buf_peek(info_buf, (td_u8 *)&peek_frame, sizeof(peek_frame));
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_peek, ret);
        return ret;
    }

    if (peek_frame.bits_bytes != stream->size) {
        sap_err_log_h32(peek_frame.bits_bytes);
        sap_err_log_h32(stream->size);
        return ERR_SYS_MEM_OVERLOAD;
    }

    ret = ring_buf_poke(info_buf, sizeof(peek_frame));
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(ring_buf_poke, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_shrb_release_stream(const shrb_buffer *buf, const audio_stream_buf *stream)
{
    td_s32 ret;

    sys_check_null_ptr_return(buf);
    sys_check_null_ptr_return(stream);

    ret = shrb_release_stream_info(buf->info_buf, stream);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_release_stream_info, ret);
        return ret;
    }

    return ring_buf_linear_release_data(buf->data_buf, stream->size);
}

td_s32 core_shrb_get_eos(shrb_handle shrb, td_bool *eos)
{
    sys_check_null_ptr_return(shrb);
    sys_check_null_ptr_return(eos);

    *eos = shrb->eos;
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_set_eos(shrb_handle shrb, td_bool eos)
{
    sys_check_null_ptr_return(shrb);

    shrb->eos = eos;
    return AUDIO_SUCCESS;
}

td_s32 core_shrb_query_mod_ids(td_u32 *mod_ids, td_u32 *mod_cnt)
{
    td_s32 ret;
    td_u32 i, j, shrb_cnt;
    shrb_handle shrb_node = TD_NULL;
    core_shrb_mgr *shrb_mgr = TD_NULL;

    sys_check_null_ptr_return(mod_ids);
    sys_check_null_ptr_return(mod_cnt);

    ret = shrb_get_mgr(&shrb_mgr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(shrb_get_mgr, ret);
        *mod_cnt = 0;
        return ret;
    }

    shrb_node = shrb_mgr->shrb_list;
    shrb_cnt = sap_min(shrb_mgr->shrb_cnt, SHRB_BUFFER_MAX_NUM);
    for (i = 0, j = 0; i < shrb_cnt; i++) {
        shrb_node = &shrb_mgr->shrb_list[i];
        if (shrb_node->shrb_id != 0) {
            mod_ids[j++] = shrb_node->mod_id;
        }
    }

    *mod_cnt = j;
    return AUDIO_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
