/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for core msg process
 * Author: audio
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "audio_msg.h"
#include "core_module.h"
#include "core_util.h"
#include "core_rbuf.h"
#include "core_shrb.h"
#include "core_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MSG_FIFO_TAIL_SIZE 0x400
#define MSG_CHECK_ACK_PER_MS 1
#define MSG_CHECK_ACK_TIMEOUT_MS 10

#define msg_fifo_align_up(size) audio_align_up(size, 128)
#define msg_pkt_align_up(size) audio_align_up(size, 4)

typedef enum {
    MSG_ACK_NULL = 0,
    MSG_ACK_WAIT_PROCESS,
    MSG_ACK_WAIT_TIMEOUT,
    MSG_ACK_PROC_RESULT,
    MSG_ACK_MAX = 0xFF
} core_msg_ack;

typedef union {
    struct {
        td_u8 mod_type;  /* #sap_mod_type */
        td_u8 shm_cid;   /* #audio_core_id */
        td_u8 from_cid;  /* #audio_core_id */
        td_u8 to_cid;    /* #audio_core_id */
    } bits;
    td_u32 u32;
} core_msg_qid;

typedef struct {
    td_s32 exec_ret;
    td_u16 input_size;
    td_u16 output_size;
    td_u8  module;       /* #core_msg_module */
    td_u8  from_ack;
    td_u8  to_ack;
    td_u8  crc;
} core_msg_head;

typedef struct {
    td_u32 timeout;
    td_u32 start_time;
    core_msg_head *msg;
} core_msg_sync;

typedef struct {
    sap_mod_head head;
    core_msg_qid qid;
    td_handle lock;
    shrb_handle shrb;
    shrb_buffer shrb_buf;
    td_u32 fifo_size;
    td_bool fifo_alloc_flag;
    core_msg_sync sync;
    core_msg_shoot shoot;
    core_msg_sys_active sys_active;
    core_msg_module_func mod[MSG_CORE_MODULE_MAX];
} core_msg_inst;

static td_u32 g_msg_init_cnt = 0;
static sap_mod_pool_handle g_msg_pool = TD_NULL;
static audio_core_id g_msg_this_cid = AUDIO_CORE_NULL;
static audio_core_id g_msg_shrb_cid = AUDIO_CORE_CPU_K;
static td_handle g_msg_handle[AUDIO_CORE_MAX][AUDIO_CORE_MAX];

AUDIO_STATIC sap_mod_handle msg_get_handle(td_handle msg)
{
    td_s32 ret;
    sap_mod_handle msg_handle = TD_NULL;

    ret = core_mod_get_module(g_msg_pool, (td_u32)msg, &msg_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_trace_log_h32(msg);
        return TD_NULL;
    }

    return msg_handle;
}

#define msg_get_inst_and_check_null_return(msg_inst, msg) \
    do { \
        (msg_inst) = (core_msg_inst *)msg_get_handle((msg)); \
        if ((msg_inst) == TD_NULL) { \
            return ERR_SYS_INVALID_HANDLE; \
        } \
    } while (0)

static inline td_u32 msg_get_qid(sap_mod_type mod_type, audio_core_id shrb, audio_core_id from, audio_core_id to)
{
    core_msg_qid msg_qid;
    msg_qid.u32 = 0;
    msg_qid.bits.mod_type = (td_u8)mod_type;
    msg_qid.bits.shm_cid  = (td_u8)shrb;
    msg_qid.bits.from_cid = (td_u8)from;
    msg_qid.bits.to_cid   = (td_u8)to;
    return msg_qid.u32;
}

static td_s32 msg_sync_wait_resp(const core_msg_head *msg_head, td_u32 timeout)
{
    td_u32 i;
    td_u32 count = timeout / MSG_CHECK_ACK_PER_MS;

    for (i = 0; i < count; i++) {
        if (msg_head->to_ack != MSG_ACK_WAIT_PROCESS) {
            break;
        }
        audio_osal_msleep(MSG_CHECK_ACK_PER_MS);
    }

    if (i >= count) {
        return ERR_SYS_TIME_OUT;
    }

    return AUDIO_SUCCESS;
}

static inline td_bool msg_sync_in_process(const core_msg_sync *msg_sync)
{
    return (msg_sync->msg != TD_NULL ? TD_TRUE : TD_FALSE);
}

static inline td_void msg_sync_start_check(core_msg_sync *msg_sync, core_msg_head *msg_head, td_u32 timeout)
{
    msg_sync->msg = msg_head;
    msg_sync->timeout = timeout;
    msg_sync->start_time = audio_osal_gettime_us();
}

static inline td_void msg_sync_stop_check(core_msg_sync *msg_sync)
{
    msg_sync->msg = TD_NULL;
    msg_sync->timeout = 0;
    msg_sync->start_time = 0;
}

static td_s32 msg_sync_check_resp(core_msg_sync *msg_sync)
{
    td_u32 wait_time;
    td_u32 curr_time;

    if (msg_sync->msg->from_ack != MSG_ACK_WAIT_PROCESS) {
        return AUDIO_SUCCESS;
    }

    curr_time = audio_osal_gettime_us();
    wait_time = audio_osal_cali_time_diff_us(msg_sync->start_time, curr_time) / AUDIO_US_NUM_PER_MS;
    if (wait_time > msg_sync->timeout) {
        msg_sync->start_time = 0;
        return ERR_SYS_TIME_OUT;
    }

    return ERR_SYS_INST_BUSY;
}

static inline td_void msg_pkt_set_result(core_msg_head *msg_head, td_s32 exec_ret)
{
    if (msg_head->to_ack == MSG_ACK_WAIT_PROCESS) {
        msg_head->to_ack = MSG_ACK_PROC_RESULT;
        msg_head->exec_ret = exec_ret;
    }
}

static td_void msg_pkt_fill_head(core_msg_head *msg_head, const core_msg_data *msg_data,
                                 core_msg_module msg_mod, td_bool ack_ena)
{
    msg_head->module = (td_u8)msg_mod;
    msg_head->to_ack = (ack_ena ? MSG_ACK_WAIT_PROCESS : MSG_ACK_NULL);
    msg_head->from_ack = (ack_ena ? MSG_ACK_WAIT_PROCESS : MSG_ACK_NULL);
    msg_head->exec_ret = AUDIO_FAILURE;
    msg_head->input_size = (td_u16)msg_data->input_data_size;
    msg_head->output_size = (td_u16)msg_data->output_data_size;
    msg_head->crc = audio_crc8((td_u8 *)msg_head, sizeof(*msg_head) - sizeof(msg_head->crc));
}

static td_void msg_pkt_fill_body(const core_msg_head *msg_head, const core_msg_data *msg_data)
{
    td_s32 ret;
    td_u8 *buf_wptr = (td_u8 *)msg_head;

    /* Skip message head */
    buf_wptr += sizeof(*msg_head);

    /* Write message command */
    *((td_u32 *)buf_wptr) = msg_data->cmd;
    buf_wptr += sizeof(msg_data->cmd);

    /* Write receiver handle */
    *((td_u32 *)buf_wptr) = msg_data->handle;
    buf_wptr += sizeof(msg_data->handle);

    /* Write command input parameters */
    if (msg_data->input_data_size > 0) {
        ret = memcpy_s(buf_wptr, msg_data->input_data_size, msg_data->input_data, msg_data->input_data_size);
        if (ret != EOK) {
            sap_err_log_fun(memcpy_s, ret);
            return;
        }
    }
    buf_wptr += msg_data->input_data_size;

    /* Reserve command output buffer */
    buf_wptr += msg_data->output_data_size;
}

static td_void msg_pkt_get_output(const core_msg_head *msg_head, td_u8 *out_data)
{
    td_s32 ret;
    td_u32 offset;
    core_msg_data msg_data;
    td_u8 *buf_wptr = (td_u8 *)msg_head;

    offset = (td_u32)(sizeof(*msg_head) + sizeof(msg_data.cmd) +
        sizeof(msg_data.handle) + msg_pkt_align_up((td_u32)(msg_head->input_size)));
    buf_wptr += offset;
    if (msg_head->output_size > 0) {
        ret = memcpy_s(out_data, msg_head->output_size, buf_wptr, msg_head->output_size);
        if (ret != EOK) {
            sap_err_log_fun(memcpy_s, ret);
            return;
        }
    }
}

static td_void msg_pkt_parse_body(const core_msg_head *msg_head, core_msg_data *msg_data)
{
    td_u8 *buf_rptr = (td_u8 *)msg_head;

    msg_data->input_data_size = msg_head->input_size;
    msg_data->output_data_size = msg_head->output_size;

    /* Skip message head */
    buf_rptr += sizeof(*msg_head);

    /* Read message command */
    msg_data->cmd = *((td_u32 *)buf_rptr);
    buf_rptr += sizeof(msg_data->cmd);

    /* Read receiver handle */
    msg_data->handle = *((td_u32 *)buf_rptr);
    buf_rptr += sizeof(msg_data->handle);

    /* Read command input parameters */
    msg_data->input_data = (msg_data->input_data_size > 0 ? (td_void *)buf_rptr : TD_NULL);
    buf_rptr += msg_pkt_align_up(msg_data->input_data_size);

    msg_data->output_data = (msg_data->output_data_size > 0 ? (td_void *)buf_rptr : TD_NULL);
    buf_rptr += msg_data->output_data_size;
}

static td_s32 msg_fifo_put_pkt(const core_msg_inst *msg_inst, core_msg_module msg_mod,
                               core_msg_head **msg_head_p, core_msg_data *msg_data, td_bool ack_ena)
{
    td_u32 msg_size;
    td_u32 buf_size;
    td_u32 read_pos;
    td_u32 write_pos;
    td_u32 msg_buf = 0;
    td_u8 *head_p = TD_NULL;
    core_msg_head *msg_head = TD_NULL;

    msg_size = (td_u32)(sizeof(*msg_head) + sizeof(msg_data->cmd) + sizeof(msg_data->handle));
    msg_size += msg_data->input_data_size + msg_data->output_data_size;
    msg_size = msg_pkt_align_up(msg_size);
    if (msg_size > MSG_FIFO_TAIL_SIZE) {
        sap_err_log_u32(msg_size);
        return ERR_SYS_BUF_OVERFLOW;
    }
    if (ring_buf_linear_query_free(msg_inst->shrb_buf.data_buf) < msg_size) {
        ring_buf_get_pos(msg_inst->shrb_buf.data_buf, &read_pos, &write_pos);
        sap_err_log_u32(read_pos);
        sap_err_log_u32(write_pos);
        sap_err_log_u32(msg_size);
        return ERR_SYS_BUF_FULL;
    }

    ring_buf_linear_get_buffer(msg_inst->shrb_buf.data_buf, &msg_buf, &buf_size);
    head_p = (td_u8 *)(td_uintptr_t)msg_buf;
    msg_head = (core_msg_head *)head_p;
    msg_pkt_fill_head(msg_head, msg_data, msg_mod, ack_ena);
    msg_pkt_fill_body(msg_head, msg_data);
    ring_buf_linear_put_buffer(msg_inst->shrb_buf.data_buf, msg_size);

    *msg_head_p = msg_head;
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_acquire_pkt(const core_msg_inst *msg_inst, core_msg_head **msg_head_p)
{
    td_u32 data_buf;
    td_u32 buf_size;
    td_u8  msg_crc;
    td_u32 msg_size;
    td_u8 *msg_buf = TD_NULL;
    core_msg_head *msg_head = TD_NULL;
    core_msg_data msg_data;

    if (ring_buf_linear_query_busy(msg_inst->shrb_buf.data_buf) < sizeof(core_msg_head)) {
        return ERR_SYS_BUF_EMPTY;
    }

    (td_void)ring_buf_linear_acquire_data(msg_inst->shrb_buf.data_buf, &data_buf, &buf_size);
    msg_buf = (td_u8 *)(td_uintptr_t)data_buf;
    msg_head = (core_msg_head *)msg_buf;
    msg_size = (td_u32)(sizeof(*msg_head) + sizeof(msg_data.cmd) + sizeof(msg_data.handle));
    msg_size += msg_head->input_size + msg_head->output_size;
    msg_size = msg_pkt_align_up(msg_size);
    if (buf_size < msg_size) {
        sap_err_log_u32(msg_size);
        (td_void)ring_buf_flush(msg_inst->shrb_buf.data_buf);
        return ERR_SYS_BUF_UNDERFLOW;
    }

    msg_crc = audio_crc8((td_u8 *)msg_head, sizeof(*msg_head) - sizeof(msg_head->crc));
    if (msg_crc != msg_head->crc) {
        sap_err_log_u32(msg_crc);
        (td_void)ring_buf_linear_release_data(msg_inst->shrb_buf.data_buf, msg_size);
        return ERR_SYS_VERIFY_FAIL;
    }

    *msg_head_p = msg_head;
    return AUDIO_SUCCESS;
}

static td_void msg_fifo_release_pkt(const core_msg_inst *msg_inst, const core_msg_head *msg_head)
{
    td_u32 msg_size;
    core_msg_data msg_data;

    msg_size = (td_u32)(sizeof(*msg_head) + sizeof(msg_data.cmd) + sizeof(msg_data.handle));
    msg_size += msg_head->input_size + msg_head->output_size;
    msg_size = msg_pkt_align_up(msg_size);
    (td_void)ring_buf_linear_release_data(msg_inst->shrb_buf.data_buf, msg_size);
}

static td_s32 msg_fifo_proc_cmd(const core_msg_inst *msg_inst, core_msg_head *msg_head)
{
    td_s32 ret;
    core_msg_data msg_data;

    if (msg_head->module >= MSG_CORE_MODULE_MAX || msg_inst->mod[msg_head->module].func == TD_NULL) {
        msg_pkt_set_result(msg_head, ERR_SYS_NOT_SUPPORTED);
        return ERR_SYS_NOT_SUPPORTED;
    }

    msg_pkt_parse_body(msg_head, &msg_data);
    ret = msg_inst->mod[msg_head->module].func(&msg_data);
    msg_pkt_set_result(msg_head, ret);

    return ret;
}

static td_s32 msg_fifo_alloc_shrb(core_msg_inst *msg_inst)
{
    td_s32 ret;
    shrb_attr attr;
    shrb_handle shrb = TD_NULL;

    audio_func_enter();
    attr.data_buf_size = msg_inst->fifo_size;
    attr.data_buf_tail_size = MSG_FIFO_TAIL_SIZE;
    attr.info_buf_size = 0;
    ret = core_shrb_alloc(&shrb, msg_inst->qid.u32, &attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_alloc, ret);
        return ret;
    }

    ret = core_shrb_attach(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_attach, ret);
        (td_void)core_shrb_free(shrb);
        return ret;
    }

    core_shrb_get_rbuf(shrb, &msg_inst->shrb_buf);
    msg_inst->shrb = shrb;
    msg_inst->fifo_alloc_flag = TD_TRUE;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_free_shrb(core_msg_inst *msg_inst)
{
    td_s32 ret;
    shrb_handle shrb = TD_NULL;

    audio_func_enter();

    if (msg_inst->fifo_alloc_flag != TD_TRUE) {
        return ERR_ADP_NOT_SUPPORTED;
    }

    ret = core_shrb_get_id(msg_inst->qid.u32, &shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_id, ret);
        return ret;
    }

    ret = core_shrb_detach(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_detach, ret);
        return ret;
    }

    ret = core_shrb_free(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_free, ret);
        return ret;
    }

    msg_inst->shrb = TD_NULL;
    msg_inst->shrb_buf.data_buf = TD_NULL;
    msg_inst->shrb_buf.info_buf = TD_NULL;
    msg_inst->fifo_alloc_flag = TD_FALSE;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_attach_shrb(core_msg_inst *msg_inst)
{
    td_s32 ret;
    shrb_handle shrb = TD_NULL;

    audio_func_enter();

    ret = core_shrb_get_id(msg_inst->qid.u32, &shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_id, ret);
        return ret;
    }

    ret = core_shrb_attach(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_attach, ret);
        return ret;
    }

    core_shrb_get_rbuf(shrb, &msg_inst->shrb_buf);
    msg_inst->shrb = shrb;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_detach_shrb(core_msg_inst *msg_inst)
{
    td_s32 ret;
    shrb_handle shrb = TD_NULL;

    audio_func_enter();

    ret = core_shrb_get_id(msg_inst->qid.u32, &shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_get_id, ret);
        return ret;
    }

    ret = core_shrb_detach(shrb);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_shrb_detach, ret);
        return ret;
    }

    msg_inst->shrb = TD_NULL;
    msg_inst->shrb_buf.data_buf = TD_NULL;
    msg_inst->shrb_buf.info_buf = TD_NULL;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_init(td_void)
{
    td_s32 ret;

    audio_func_enter();
    if (g_msg_init_cnt > 0) {
        g_msg_init_cnt++;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = core_mod_init(&g_msg_pool, SAP_MOD_MSG, sizeof(core_msg_inst), MOD_MSG_MAX_NUM);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_init, ret);
        return ret;
    }

    g_msg_this_cid = core_shrb_get_core_id();
    memset_s(g_msg_handle, sizeof(g_msg_handle), 0, sizeof(g_msg_handle));
    g_msg_init_cnt++;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_deinit(td_void)
{
    td_s32 ret;

    audio_func_enter();
    if (g_msg_init_cnt == 0) {
        audio_func_exit();
        return AUDIO_SUCCESS;
    }
    if (g_msg_init_cnt > 1) {
        g_msg_init_cnt--;
        audio_func_exit();
        return AUDIO_SUCCESS;
    }

    ret = core_mod_deinit(g_msg_pool);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_deinit, ret);
        return ret;
    }
    g_msg_pool = TD_NULL;

    g_msg_init_cnt--;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_create(core_msg_inst **msg_inst_p, audio_core_id from, audio_core_id to, td_u32 fifo_size)
{
    td_s32 ret;
    sap_mod_handle msg_handle = TD_NULL;
    core_msg_inst *msg_inst = TD_NULL;

    audio_func_enter();

    if (msg_inst_p == TD_NULL) {
        return ERR_SYS_NULL_PTR;
    }
    if ((from >= AUDIO_CORE_MAX) || (to >= AUDIO_CORE_MAX)) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_mod_alloc(g_msg_pool, &msg_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_alloc, ret);
        return ret;
    }
    msg_inst = (core_msg_inst *)msg_handle;

    msg_inst->qid.u32 = msg_get_qid(SAP_MOD_MSG, g_msg_shrb_cid, from, to);
    msg_inst->fifo_size = msg_fifo_align_up(fifo_size);
    sap_trace_log_h32(msg_inst->qid.u32);

    if (g_msg_this_cid == g_msg_shrb_cid) {
        ret = msg_fifo_alloc_shrb(msg_inst);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(msg_fifo_alloc_shrb, ret);
            core_mod_free(g_msg_pool, &msg_inst->head);
            return ret;
        }
        sap_trace_log_h32(msg_inst->shrb_buf.data_buf);
    } else {
        ret = msg_fifo_attach_shrb(msg_inst);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(msg_fifo_attach_shrb, ret);
            core_mod_free(g_msg_pool, &msg_inst->head);
            return ret;
        }
        sap_trace_log_h32(msg_inst->shrb_buf.data_buf);
    }

    audio_osal_mutex_init((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    *msg_inst_p = msg_inst;
    audio_func_exit();
    return AUDIO_SUCCESS;
}

static td_s32 msg_fifo_destroy(core_msg_inst *msg_inst)
{
    td_s32 ret;

    audio_func_enter();
    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    if (msg_inst->fifo_alloc_flag == TD_TRUE) {
        ret = msg_fifo_free_shrb(msg_inst);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(msg_fifo_free_shrb, ret);
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return ret;
        }
    } else {
        ret = msg_fifo_detach_shrb(msg_inst);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(msg_fifo_detach_shrb, ret);
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return ret;
        }
    }
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    audio_osal_mutex_deinit((audio_osal_mutex *)(td_void *)&msg_inst->lock);

    ret = core_mod_free(g_msg_pool, &msg_inst->head);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_mod_free, ret);
        return ret;
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_s32 core_msg_register_module(audio_core_id from, core_msg_module msg_mod, core_msg_func msg_func)
{
    core_msg_inst *msg_inst = TD_NULL;
    msg_get_inst_and_check_null_return(msg_inst, g_msg_handle[from][g_msg_this_cid]);

    if (msg_mod >= MSG_CORE_MODULE_MAX) {
        return ERR_SYS_INVALID_PARAM;
    }

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    msg_inst->mod[msg_mod].func = msg_func;
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return AUDIO_SUCCESS;
}

td_s32 core_msg_register_sys_active(audio_core_id to, core_msg_sys_active sys_active)
{
    core_msg_inst *msg_inst = TD_NULL;
    msg_get_inst_and_check_null_return(msg_inst, g_msg_handle[g_msg_this_cid][to]);

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    msg_inst->sys_active = sys_active;
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return AUDIO_SUCCESS;
}

td_s32 core_msg_register_interrupt(audio_core_id to, core_msg_shoot msg_shoot)
{
    core_msg_inst *msg_inst = TD_NULL;
    msg_get_inst_and_check_null_return(msg_inst, g_msg_handle[g_msg_this_cid][to]);

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    msg_inst->shoot = msg_shoot;
    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return AUDIO_SUCCESS;
}

td_s32 core_msg_init(audio_core_id from, audio_core_id to)
{
    td_s32 ret;
    td_handle msg;
    core_msg_inst *msg_inst = TD_NULL;

    ret = msg_fifo_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(msg_fifo_init, ret);
        return ret;
    }

    msg = g_msg_handle[from][to];
    if (msg != 0) {
        return AUDIO_SUCCESS;
    }

    ret = msg_fifo_create(&msg_inst, from, to, MSG_FIFO_BUFFER_SIZE);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(msg_fifo_create, ret);
        msg_fifo_deinit();
        return ret;
    }

    msg = (td_handle)core_mod_get_handle(msg_inst);
    g_msg_handle[from][to] = msg;

    return AUDIO_SUCCESS;
}

td_s32 core_msg_deinit(audio_core_id from, audio_core_id to)
{
    td_s32 ret;
    td_handle msg;
    core_msg_inst *msg_inst = TD_NULL;

    msg = g_msg_handle[from][to];
    msg_get_inst_and_check_null_return(msg_inst, msg);
    ret = msg_fifo_destroy(msg_inst);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(msg_fifo_destroy, ret);
        return ret;
    }
    g_msg_handle[from][to] = 0;

    msg_fifo_deinit();
    return AUDIO_SUCCESS;
}

td_s32 core_msg_send(audio_core_id to, core_msg_module msg_mod, core_msg_data *msg_data, td_u32 timeout)
{
    td_s32 ret;
    td_bool ack_ena;
    core_msg_head *msg_head = TD_NULL;
    core_msg_inst *msg_inst = TD_NULL;
    msg_get_inst_and_check_null_return(msg_inst, g_msg_handle[g_msg_this_cid][to]);

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    if (msg_inst->sys_active != TD_NULL && msg_data->cmd != MSG_CORE_SYS_SET_CLOCK_ATTR) {
        /* 每次启动DSP均会发送时钟属性配置消息，无须再唤醒DSP，否则会出现死锁问题 */
        msg_inst->sys_active(to);
    }

    ack_ena = (timeout > 0 ? TD_TRUE : TD_FALSE);
    ret = msg_fifo_put_pkt(msg_inst, msg_mod, &msg_head, msg_data, ack_ena);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
        return ret;
    }
    if (msg_inst->shoot != TD_NULL) {
        msg_inst->shoot(g_msg_this_cid, to);
    }

    if (timeout > 0) {
        ret = msg_sync_wait_resp(msg_head, timeout);
        if (ret != AUDIO_SUCCESS) {
            msg_head->from_ack = MSG_ACK_WAIT_TIMEOUT;
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return ret;
        }

        if (msg_head->exec_ret != AUDIO_SUCCESS) {
            msg_head->from_ack = MSG_ACK_NULL;
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return msg_head->exec_ret;
        }

        msg_pkt_get_output(msg_head, (td_u8 *)msg_data->output_data);
        msg_head->from_ack = MSG_ACK_NULL;
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return AUDIO_SUCCESS;
}

td_s32 core_msg_process(audio_core_id from)
{
    td_s32 ret;
    core_msg_head *msg_head = TD_NULL;
    core_msg_inst *msg_inst = TD_NULL;
    msg_get_inst_and_check_null_return(msg_inst, g_msg_handle[from][g_msg_this_cid]);

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    if (msg_sync_in_process(&msg_inst->sync)) {
        ret = msg_sync_check_resp(&msg_inst->sync);
        if ((ret != AUDIO_SUCCESS) && ((td_u32)ret != ERR_SYS_TIME_OUT)) {
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return ERR_SYS_INST_BUSY;
        }

        msg_fifo_release_pkt(msg_inst, msg_inst->sync.msg);
        msg_sync_stop_check(&msg_inst->sync);

        if ((td_u32)ret == ERR_SYS_TIME_OUT) {
            audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
            return ret;
        }
    }

    ret = msg_fifo_acquire_pkt(msg_inst, &msg_head);
    if (ret != AUDIO_SUCCESS) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
        return ret;
    }

    ret = msg_fifo_proc_cmd(msg_inst, msg_head);
    if (ret != AUDIO_SUCCESS) {
        msg_fifo_release_pkt(msg_inst, msg_head);
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
        sap_err_log_h32(ret);
        return ret;
    }

    if (msg_head->from_ack == MSG_ACK_WAIT_PROCESS) {
        msg_sync_start_check(&msg_inst->sync, msg_head, MSG_CHECK_ACK_TIMEOUT_MS);
    } else {
        msg_fifo_release_pkt(msg_inst, msg_head);
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return AUDIO_SUCCESS;
}

td_bool core_msg_check_empty(audio_core_id from)
{
    core_msg_inst *msg_inst = (core_msg_inst *)msg_get_handle(g_msg_handle[from][g_msg_this_cid]);

    if (msg_inst == TD_NULL) {
        return TD_TRUE;
    }

    audio_osal_mutex_lock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    if (ring_buf_linear_query_busy(msg_inst->shrb_buf.data_buf) < sizeof(core_msg_head)) {
        audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
        return TD_TRUE;
    }

    audio_osal_mutex_unlock((audio_osal_mutex *)(td_void *)&msg_inst->lock);
    return TD_FALSE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
