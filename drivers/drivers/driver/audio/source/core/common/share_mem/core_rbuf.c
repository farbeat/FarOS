/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: source file of ring buf
 * Author: audio
 */

#include "securec.h"
#include "osal_list.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_util.h"
#include "core_rbuf.h"

#if (SAP_OS_TYPE == os_type_xtensa)
#include <xtensa/hal.h>
#endif /* SAP_OS_TYPE */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define RBUF_MAX_INST_NUM CORE_RBUF_MAX_NUM

/* "xhal_dcache_region_invalidate" function need buf addr alignment to 64 */
#define RBUF_ALIGN_BYTES 128
#define rbuf_mem_align_int(size) audio_mem_align(size, RBUF_ALIGN_BYTES)
#define rbuf_mem_align_ext(size) ((size) & ~(RBUF_ALIGN_BYTES - 1))

struct core_ring_buffer {
    rbuf_alloc_type alloc_type;
    rbuf_copy_mode copy_mode;
    td_u8 *buf;
    td_u8 *data;
    td_u32 size;
    td_u32 *read_pos;
    td_u32 *write_pos;
    td_u32 tail_size;
    td_u32 tail_busy;
    struct osal_list_head node;
};

static OSAL_LIST_HEAD(g_rbuf_used_list);
static OSAL_LIST_HEAD(g_rbuf_free_list);
static rbuf_handle g_rbuf_insts = TD_NULL;

static inline td_u32 circ_buf_min(td_u32 x, td_u32 y)
{
    return (x <= y) ? x : y;
}

static inline td_u32 circ_buf_data_size(td_u32 write, td_u32 read, td_u32 size)
{
    return (write >= read) ? (write - read) : (size - read + write);
}

static inline td_u32 circ_buf_free_size(td_u32 write, td_u32 read, td_u32 size)
{
    return size - circ_buf_data_size(write, read, size);
}

static inline td_u8 *ring_buf_map_uncache(td_u8 *buf, rbuf_copy_mode copy_mode)
{
    audio_unused(copy_mode);
    return buf;
}

static td_void ring_buf_inst_init(rbuf_handle rbuf, const ring_buf *attr)
{
    rbuf->copy_mode = attr->copy_mode;
    rbuf->alloc_type = attr->alloc_type;
    rbuf->buf = TD_NULL;
    rbuf->data = TD_NULL;
    rbuf->size = attr->size;
    rbuf->read_pos = attr->read_pos;
    rbuf->write_pos = attr->write_pos;
    rbuf->tail_size = attr->tail_size;
    rbuf->tail_busy = 0;

    if (rbuf->copy_mode != RBUF_CPY_DSP_TOARM) {
        *rbuf->read_pos = 0;
        *rbuf->write_pos = 0;
    }
}

static td_s32 ring_buf_check_attr(const ring_buf *attr)
{
    if (attr == TD_NULL) {
        return ERR_SYS_INVALID_PARAM;
    }

    if (attr->read_pos == TD_NULL || attr->write_pos == TD_NULL) {
        return ERR_SYS_INVALID_PARAM;
    }

    return AUDIO_SUCCESS;
}

static td_void rbuf_init(td_void)
{
    td_u32 i;
    size_t mem_size;
    rbuf_handle inst = TD_NULL;

    if (g_rbuf_insts != TD_NULL) {
        return;
    }

    mem_size = RBUF_MAX_INST_NUM * sizeof(*inst);
    g_rbuf_insts = (rbuf_handle)audio_osal_calloc(mem_size);
    if (g_rbuf_insts == TD_NULL) {
        return;
    }

    for (i = 0; i < RBUF_MAX_INST_NUM; i++) {
        inst = g_rbuf_insts + i;
        osal_list_add_tail(&inst->node, &g_rbuf_free_list);
    }
}

static td_void rbuf_deinit(td_void)
{
    if (g_rbuf_insts == TD_NULL) {
        return;
    }

    if (osal_list_empty(&g_rbuf_used_list) == 0) {
        return;
    }

    osal_list_del_init(&g_rbuf_used_list);
    osal_list_del_init(&g_rbuf_free_list);
    audio_osal_free(g_rbuf_insts);
    g_rbuf_insts = TD_NULL;

    return;
}

static rbuf_handle rbuf_alloc_inst(td_void)
{
    rbuf_handle inst = TD_NULL;

    if (osal_list_empty(&g_rbuf_free_list) != 0) {
        return TD_NULL;
    }

    inst = osal_list_first_entry(&g_rbuf_free_list, struct core_ring_buffer, node);
    osal_list_del(&inst->node);
    osal_list_add_tail(&inst->node, &g_rbuf_used_list);
    return inst;
}

static td_void rbuf_free_inst(const struct core_ring_buffer *rbuf)
{
    rbuf_handle inst = TD_NULL;
    osal_list_for_each_entry(inst, &g_rbuf_used_list, node) {
        if (inst == rbuf) {
            osal_list_del(&inst->node);
            osal_list_add_tail(&inst->node, &g_rbuf_free_list);
            return;
        }
    }
}

td_s32 ring_buf_create(rbuf_handle *handle, const ring_buf *attr)
{
    td_u32 rbuf_size;
    rbuf_handle rbuf = TD_NULL;

    audio_func_enter();

    if (handle == TD_NULL || ring_buf_check_attr(attr) != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    rbuf_init();
    rbuf = (rbuf_handle)rbuf_alloc_inst();
    if (rbuf == TD_NULL) {
        return ERR_SYS_MEM_ALLOC;
    }
    ring_buf_inst_init(rbuf, attr);

    if (attr->alloc_type == RING_BUF_ALLOC_INTERNAL) {
        rbuf_size = rbuf_mem_align_int(attr->size + attr->tail_size);
        rbuf->buf = (td_u8 *)audio_osal_malloc_aligned(rbuf_size, RBUF_ALIGN_BYTES);
        if (rbuf->buf == TD_NULL) {
            sap_err_log_u32(rbuf_size);
            rbuf_free_inst(rbuf);
            return ERR_SYS_MEM_ALLOC;
        }

        rbuf->data = ring_buf_map_uncache(rbuf->buf, rbuf->copy_mode);
    } else { /* memory mallocated external */
        rbuf_size = rbuf_mem_align_ext(attr->size + attr->tail_size);
        rbuf->data = (td_u8 *)attr->data;
    }

    rbuf->size = rbuf_size - attr->tail_size;

    sap_trace_log_h32(attr->size);
    sap_trace_log_h32(attr->tail_size);

    *handle = rbuf;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 ring_buf_destroy(rbuf_handle rbuf)
{
    audio_func_enter();

    sys_check_null_ptr_return(rbuf);

    if (rbuf->buf != TD_NULL) {
        audio_osal_free_aligned(rbuf->buf);
        rbuf->buf = TD_NULL;
    }

    rbuf->data = TD_NULL;
    rbuf_free_inst(rbuf);
    rbuf_deinit();

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 ring_buf_set_read_pos(rbuf_handle rbuf, td_u32 read_pos)
{
    sys_check_null_ptr_return(rbuf);
    sys_check_null_ptr_return(rbuf->read_pos);

    if (read_pos < rbuf->size) {
        *(rbuf->read_pos) = read_pos;
    }

    return AUDIO_SUCCESS;
}

static inline td_u32 saturate_add(td_u32 x, td_u32 y, td_u32 saturation)
{
    /* add x and y, saturate to saturation */
    td_u32 sum = x + y;

    return (sum >= saturation) ? (sum - saturation) : sum;
}

static td_u32 cb_read_data(const rbuf_handle rbuf, td_u8 *to, td_u32 len, td_u32 off)
{
    td_s32 ret;
    if (len == 0) {
        return 0;
    }

    ret = memcpy_s(to, len * sizeof(td_u8), rbuf->data + off, len);
    if (ret != EOK) {
        return 0;
    }

    return len;
}

/* read data from circle buffer */
static inline td_u32 circ_buf_read_data(const rbuf_handle rbuf, td_u8 *to, td_u32 len, td_u32 off)
{
    td_u32 l = circ_buf_min(len, rbuf->size - off);

    if (to == TD_NULL) {
        /* dump data in circle buffer */
        return len;
    }

    return cb_read_data(rbuf, to, l, off) + cb_read_data(rbuf, to + l, len - l, 0);
}

/* dump data in circle buffer when buf is NULL */
td_s32 ring_buf_read(rbuf_handle rbuf, td_u8 *buf, td_u32 size)
{
    td_u32 r_pos;
    td_u32 w_pos;

    sys_check_null_ptr_return(rbuf);

    if (size == 0) {
        sap_err_log_u32(size);
        return ERR_SYS_INVALID_PARAM;
    }

    r_pos = *(rbuf->read_pos);
    w_pos = *(rbuf->write_pos);

    if (circ_buf_data_size(w_pos, r_pos, rbuf->size) < size) {
        return ERR_SYS_BUF_UNDERFLOW;
    }

    if (circ_buf_read_data(rbuf, buf, size, r_pos) != size) {
        return ERR_SYS_MEM_COPY;
    }

    *rbuf->read_pos = saturate_add(r_pos, size, rbuf->size);
    return AUDIO_SUCCESS;
}

td_s32 ring_buf_freeze_read(rbuf_handle rbuf, td_u8 *buf, td_u32 size)
{
    td_u32 r_pos;
    td_u32 w_pos;

    if (buf == TD_NULL || size == 0) {
        return ERR_SYS_INVALID_PARAM;
    }

    r_pos = *(rbuf->read_pos);
    w_pos = *(rbuf->write_pos);

    if (circ_buf_data_size(w_pos, r_pos, rbuf->size) < size) {
        return ERR_SYS_BUF_UNDERFLOW;
    }

    if (circ_buf_read_data(rbuf, buf, size, r_pos) != size) {
        return ERR_SYS_MEM_COPY;
    }

    return AUDIO_SUCCESS;
}

static inline td_u32 cb_write_zero(rbuf_handle rbuf, const td_u8 *from, td_u32 len, td_u32 off)
{
    if ((len == 0) || (memset_s(rbuf->data + off, len, 0, len) != EOK)) {
        return 0;
    }

    audio_unused(from);

    return len;
}

static inline td_u32 cb_write_data(rbuf_handle rbuf, const td_u8 *from, td_u32 len, td_u32 off)
{
    if ((len == 0) || (memcpy_s(rbuf->data + off, rbuf->size - off, from, len) != EOK)) {
        return 0;
    }

    return len;
}

/*
 * circ_buf_write_data - puts some data into the circle buffer
 * @cb: addr of the circle buffer to be used
 * @from: pointer to the data to be added
 * @len: the length of the data to be added
 * @off: data write offset in circle buffer
 * Note: Check free space in circle buffer before calling this function
 */
static inline td_u32 circ_buf_write_data(rbuf_handle rbuf, const td_u8 *from, td_u32 len, td_u32 off)
{
    td_u32 l = circ_buf_min(len, rbuf->size - off);

    /* generate mute data if input buffer is TD_NULL */
    td_u32 (*action)(rbuf_handle, const td_u8 *, td_u32, td_u32) =
        (from == TD_NULL) ? cb_write_zero : cb_write_data;

    return action(rbuf, from, l, off) + action(rbuf, from + l, len - l, 0);
}

/* generate mute data if input buffer is TD_NULL */
td_s32 ring_buf_write(rbuf_handle rbuf, const td_u8 *data, td_u32 size)
{
    td_u32 read;
    td_u32 write;

    sys_check_null_ptr_return(rbuf);

    if (size == 0) {
        sap_err_log_u32(size);
        return ERR_SYS_INVALID_PARAM;
    }

    read = *(rbuf->read_pos);
    write = *(rbuf->write_pos);

    if (circ_buf_free_size(write, read, rbuf->size) <= size) {
        return ERR_SYS_BUF_OVERFLOW;
    }

    if (circ_buf_write_data(rbuf, data, size, write) != size) {
        return ERR_SYS_MEM_COPY;
    }

    *(rbuf->write_pos) = saturate_add(write, size, rbuf->size);
    return AUDIO_SUCCESS;
}

// how many bytes is filled in the buffer, these bytes can be read
td_u32 ring_buf_query_busy(rbuf_handle rbuf)
{
    if (rbuf == TD_NULL) {
        return 0;
    }

    return circ_buf_data_size(*rbuf->write_pos, *rbuf->read_pos, rbuf->size);
}

td_u32 ring_buf_get_delay(const rbuf_handle rbuf, const audio_pcm_format *pcm_format)
{
    td_u32 data_size;

    if (rbuf == TD_NULL) {
        return 0;
    }

    data_size = circ_buf_data_size(*rbuf->write_pos, *rbuf->read_pos, rbuf->size);
    if (data_size == 0) {
        return 0;
    }

    return calc_pcm_frame_ms(pcm_format, data_size);
}

// how many bytes is free in the buffer, these bytes can be write
td_u32 ring_buf_query_free(rbuf_handle rbuf)
{
    td_u32 free_size;
    if (rbuf == TD_NULL) {
        return 0;
    }

    free_size = circ_buf_free_size(*rbuf->write_pos, *rbuf->read_pos, rbuf->size);
    return free_size > sizeof(td_s64) ? (free_size - sizeof(td_s64)) : 0;
}

td_s32 ring_buf_get_info(rbuf_handle rbuf, td_u32 *buf_addr, td_u32 *buf_len, td_u32 *tail_len)
{
    sys_check_null_ptr_return(rbuf);
    sys_check_null_ptr_return(buf_addr);
    sys_check_null_ptr_return(buf_len);

    *buf_addr = (td_u32)(td_uintptr_t)(rbuf->data);
    *buf_len = rbuf->size;
    if (tail_len != TD_NULL) {
        /* 必要时才获取tail_size */
        *tail_len = rbuf->tail_size;
    }

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_get_pos(const struct core_ring_buffer *rbuf, td_u32 *read_pos, td_u32 *write_pos)
{
    sys_check_null_ptr_return(rbuf);
    sys_check_null_ptr_return(read_pos);
    sys_check_null_ptr_return(write_pos);

    *read_pos = *rbuf->read_pos;
    *write_pos = *rbuf->write_pos;

    return AUDIO_SUCCESS;
}

static inline td_u32 circ_buf_linear_tail_left_size(td_u32 start_pos, td_u32 size, td_u32 tail_size)
{
    return size + tail_size - start_pos;
}

/* how many bytes is filled in the buffer, these bytes can be read linearly */
static inline td_u32 circ_buf_linear_query_busy(td_u32 w_pos, td_u32 r_pos, td_u32 size, td_u32 tail_size)
{
    td_u32 busy_size;
    td_u32 left_size;

    left_size = circ_buf_linear_tail_left_size(r_pos, size, tail_size);
    busy_size = circ_buf_data_size(w_pos, r_pos, size);
    return circ_buf_min(busy_size, left_size);
}

/* how many bytes is free in the buffer, these bytes can be write linearly */
static inline td_u32 circ_buf_linear_query_free(td_u32 w_pos, td_u32 r_pos, td_u32 size, td_u32 tail_size)
{
    td_u32 free_size;
    td_u32 left_size;

    left_size = circ_buf_linear_tail_left_size(w_pos, size, tail_size);
    free_size = circ_buf_free_size(w_pos, r_pos, size);
    return circ_buf_min(free_size, left_size);
}

/* how many bytes is filled in the buffer, these bytes can be read linearly */
td_u32 ring_buf_linear_query_busy(rbuf_handle rbuf)
{
    td_u32 r_pos;
    td_u32 w_pos;

    if (rbuf == TD_NULL) {
        return 0;
    }

    r_pos = *rbuf->read_pos;
    w_pos = *rbuf->write_pos;

    return circ_buf_linear_query_busy(w_pos, r_pos, rbuf->size, rbuf->tail_size);
}

/* how many bytes is free in the buffer, these bytes can be write linearly */
td_u32 ring_buf_linear_query_free(rbuf_handle rbuf)
{
    td_u32 r_pos;
    td_u32 w_pos;
    if (rbuf == TD_NULL) {
        return 0;
    }

    r_pos = *rbuf->read_pos;
    w_pos = *rbuf->write_pos;

    return circ_buf_linear_query_free(w_pos, r_pos, rbuf->size, rbuf->tail_size);
}

td_s32 ring_buf_linear_get_buffer(rbuf_handle rbuf, td_u32 *data_buf, td_u32 *buf_size)
{
    td_u32 r_pos;
    td_u32 w_pos;
    td_u32 free_size;
    sys_check_null_ptr_return(rbuf);

    r_pos = *rbuf->read_pos;
    w_pos = *rbuf->write_pos;

    free_size = circ_buf_linear_query_free(w_pos, r_pos, rbuf->size, rbuf->tail_size);
    free_size = (free_size > 0) ? (free_size - 1) : 0;

    *data_buf = ((td_u32)(td_uintptr_t)rbuf->data) + w_pos;
    *buf_size = free_size;

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_linear_put_buffer(rbuf_handle rbuf, td_u32 data_size)
{
    td_u32 r_pos;
    td_u32 w_pos;
    td_u32 free_size;
    td_u32 copy_size;

    sys_check_null_ptr_return(rbuf);

    r_pos = *rbuf->read_pos;
    w_pos = *rbuf->write_pos;

    free_size = circ_buf_linear_query_free(w_pos, r_pos, rbuf->size, rbuf->tail_size);
    free_size = (free_size > 0) ? (free_size - 1) : 0;
    if (data_size > free_size) {
        return ERR_SYS_INVALID_PARAM;
    }

    copy_size = w_pos + data_size;
    copy_size = (copy_size >= rbuf->size) ? (copy_size - rbuf->size) : 0; /* 头尾数据拷贝大小 */

    if (cb_write_data(rbuf, rbuf->data + rbuf->size, copy_size, 0) != copy_size) {
        return ERR_SYS_MEM_COPY;
    }

    *(rbuf->write_pos) = saturate_add(w_pos, data_size, rbuf->size);

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_linear_acquire_data(rbuf_handle rbuf, td_u32 *data_buf, td_u32 *buf_size)
{
    td_u32 r_pos;
    td_u32 w_pos;
    td_u32 copy_size;

    sys_check_null_ptr_return(rbuf);
    sys_check_null_ptr_return(data_buf);
    sys_check_null_ptr_return(buf_size);

    r_pos = *rbuf->read_pos;
    w_pos = *rbuf->write_pos;

    *buf_size = circ_buf_linear_query_busy(w_pos, r_pos, rbuf->size, rbuf->tail_size);

    copy_size = r_pos + *buf_size;
    copy_size = (copy_size > rbuf->size) ? (copy_size - rbuf->size) : 0;           /* 回绕判断 */
    copy_size = (copy_size > rbuf->tail_busy) ? (copy_size - rbuf->tail_busy) : 0; /* 头尾数据拷贝大小 */

    if (cb_read_data(rbuf, rbuf->data + rbuf->size + rbuf->tail_busy, copy_size,
        rbuf->tail_busy) != copy_size) {
        sap_trace_log_h32(ERR_SYS_MEM_COPY);
        return ERR_SYS_MEM_COPY;
    }

    rbuf->tail_busy += copy_size; /* 记录rbuf尾部临时利用size */

    *data_buf = ((td_u32)(td_uintptr_t)rbuf->data) + r_pos;

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_linear_release_data(rbuf_handle rbuf, td_u32 data_size)
{
    td_u32 read;
    td_u32 pos;

    sys_check_null_ptr_return(rbuf);

    if (data_size == 0) {
        return AUDIO_SUCCESS;
    }

    read = *rbuf->read_pos;
    pos = saturate_add(read, data_size, rbuf->size);
    if (pos < read) {
        rbuf->tail_busy = 0;
    }

    *rbuf->read_pos = pos;

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_flush(rbuf_handle rbuf)
{
    td_u32 write;

    sys_check_null_ptr_return(rbuf);

    write = *rbuf->write_pos;
    *rbuf->read_pos = write;

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_reset(rbuf_handle rbuf)
{
    sys_check_null_ptr_return(rbuf);

    *rbuf->write_pos  = 0;
    *rbuf->read_pos = 0;

    return AUDIO_SUCCESS;
}

td_u32 ring_buf_read_with_action(rbuf_handle rbuf,
    td_u32 (*action)(td_void *to, const td_void *from, td_u32 size),
    td_u8 *to, td_u32 len)
{
    td_u32 l;
    td_u32 l1;
    td_u32 l2;
    td_u32 read;
    td_u32 write;

    if (rbuf == TD_NULL) {
        return 0;
    }

    read = *rbuf->read_pos;
    write = *rbuf->write_pos;

    if (circ_buf_data_size(write, read, rbuf->size) < len) {
        return 0;
    }

    l = circ_buf_min(len, rbuf->size - read);

    l1 = action(to, rbuf->data + read, l);
    l2 = action(to + l, rbuf->data, len - l);

    *rbuf->read_pos = saturate_add(read, len, rbuf->size);

    return l1 + l2;
}

td_s32 ring_buf_peek(rbuf_handle rbuf, td_u8 *buf, td_u32 size)
{
    td_u32 read;
    td_u32 write;

    sys_check_null_ptr_return(rbuf);

    if (buf == TD_NULL || size == 0) {
        return ERR_SYS_INVALID_PARAM;
    }

    read = *rbuf->read_pos;
    write = *rbuf->write_pos;

    if (circ_buf_data_size(write, read, rbuf->size) < size) {
        return ERR_SYS_BUF_UNDERFLOW;
    }

    if (circ_buf_read_data(rbuf, buf, size, read) != size) {
        return ERR_SYS_MEM_COPY;
    }

    return AUDIO_SUCCESS;
}

td_s32 ring_buf_poke(rbuf_handle rbuf, td_u32 size)
{
    td_u32 read;
    td_u32 write;

    sys_check_null_ptr_return(rbuf);

    if (size == 0) {
        return AUDIO_SUCCESS;
    }

    read = *rbuf->read_pos;
    write = *rbuf->write_pos;

    if (circ_buf_data_size(write, read, rbuf->size) < size) {
        return ERR_SYS_BUF_UNDERFLOW;
    }

    *rbuf->read_pos = saturate_add(read, size, rbuf->size);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
