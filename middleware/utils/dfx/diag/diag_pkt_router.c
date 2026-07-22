/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: pkt router
 * This file should be changed only infrequently and with great care.
 */

#include "diag_pkt_router.h"
#include "securec.h"
#include "diag_msg.h"
#include "diag_mem.h"
#include "diag_cmd_dst.h"
#include "diag_ind_dst.h"
#include "diag_tx.h"
#include "diag_debug.h"
#include "zdiag_adapt_layer.h"
#include "diag_dfx.h"
#include "dfx_adapt_layer.h"
#include "errcode.h"
#include "debug_print.h"
#include "diag_service.h"
#include "uapi_crc.h"

#define DIAG_ROUTER_EXTRAS_MAX_LEN  (DIAG_ROUTER_HEADER_MAX_LEN + DIAG_ROUTER_CRC_LEN + DIAG_ROUTER_EOF_LEN)

typedef struct {
    diag_merge_data_status  status;
    uint8_t                 module_id;
} diag_merge_flag_t;

#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
static diag_merge_flag_t g_diag_merge_flag[DIAG_SUPPORT_CHANNEL_CNT] = {0};
#endif

static void diag_router_build_frame(diag_router_frame_t *frame, uint16_t send_len, diag_router_data_t *data)
{
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)frame->inner;
    uint16_t offset = 0;
    uint16_t payload_len = send_len - DIAG_ROUTER_HEADER_LEN;

    frame->sof  = DIAG_ROUTER_FRAME_START_FLAG;
    frame->ctrl = frame_ctrl_build(data->ctrl.en_crc, data->ctrl.en_fid, data->ctrl.en_sn, data->ctrl.en_eof,
                                   data->ctrl.en_full_crc, data->ctrl.en_crypto, data->ctrl.en_extend);

    if ((data->ctrl.en_extend) != 0) {
        inner = (diag_router_inner_frame_t *)(frame->inner + DIAG_ROUTER_CTRL2_LEN);
        frame->inner[0] = frame_ctrl2_build(data->ctrl2.ack_type);
        payload_len -= DIAG_ROUTER_CTRL2_LEN;
    }

    inner->len_msb = (uint8_t)(payload_len >> DIAG_FRAME_SHIFT_8);
    inner->len_lsb = (uint8_t)payload_len;

    if ((data->ctrl.en_fid) != 0) {
        inner->payload[offset] = data->fid;
        offset++;
    }

    if ((data->ctrl.en_sn) != 0) {
        inner->payload[offset] = data->sn_count;
        offset++;
    }

    if ((data->echo) != 0) {
        inner->payload[offset] = data->echo;
        offset++;
    }

    memcpy_s((void *)(inner->payload + offset), data->data_len, data->data, data->data_len);
    offset += data->data_len;
    if ((data->ctrl.en_crc) != 0) {
        uint16_t crc16;
        if ((data->ctrl.en_full_crc) != 0) {
            crc16 = uapi_crc16(0, (uint8_t *)frame, DIAG_ROUTER_HEADER_LEN + data->ctrl.en_extend + offset);
        } else {
            crc16 = uapi_crc16(0, (uint8_t *)inner->payload, offset);
        }
        memcpy_s((void *)(inner->payload + offset), sizeof(uint16_t), &crc16, sizeof(uint16_t));
        offset += (uint16_t)sizeof(uint16_t);
    }

    if ((data->ctrl.en_eof) != 0) {
        inner->payload[offset] = DIAG_ROUTER_FRAME_END_FLAG;
    }
}

/* 回复ack */
static void diag_router_check_and_send_ack(diag_router_frame_t *frame, diag_pkt_process_param_t *process_param,
    uint8_t echo)
{
    /* 是否有扩展CTRL字段 */
    if (get_frame_ctrl_extend_en(frame->ctrl) == 0) {
        return;
    }

    diag_router_ctrl2_t *ctrl2 = (diag_router_ctrl2_t *)&frame->inner[0];
    /* 是否需要ACK */
    if (ctrl2->ack_type != FRAME_ACK_TYPE_ACK) {
        return;
    }

    dfx_log_debug("diag pkt send ack\r\n");
    diag_router_data_t router_data = {0};

    router_data.ctrl.en_crc      = 1;
    router_data.ctrl.en_fid      = 0;
    router_data.ctrl.en_sn       = FRAME_SN_FLAG_NONE;
    router_data.ctrl.en_eof      = 0;
    router_data.ctrl.en_full_crc = 0;
    router_data.ctrl.en_crypto   = 0;
    router_data.ctrl.en_extend   = 1;
    router_data.ctrl2.ack_type   = FRAME_ACK_TYPE_ECHO;

    router_data.sn_count         = 0;
    router_data.echo             = echo;

    router_data.data             = NULL;
    router_data.data_len         = 0;

    uint16_t send_len = DIAG_ROUTER_HEADER_LEN + DIAG_ROUTER_CTRL2_LEN + DIAG_ROUTER_ECHO_LEN + DIAG_ROUTER_CRC_LEN;
    uint8_t *ack_buffer = dfx_malloc(0, send_len);
    if (ack_buffer != NULL) {
        diag_router_build_frame((diag_router_frame_t *)ack_buffer, send_len, &router_data);

        diag_pkt_handle_t pkt;
        diag_pkt_handle_init(&pkt, 1);
        diag_pkt_handle_set_data(&pkt, DIAG_PKT_DATA_ID_0, ack_buffer, send_len, DIAG_PKT_STACK_DATA);

        process_param->dst_addr = process_param->src_addr;
        diag_pkt_router_output(&pkt, process_param);
        dfx_free(0, ack_buffer);
    }
}

#if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES)
/* 接收处理 */
STATIC errcode_t diag_pkt_router_notify(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param)
{
    if ((pkt == NULL) || (pkt->data[0] == NULL) || (pkt->data_len[0] == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    diag_router_frame_t *frame = (diag_router_frame_t *)pkt->data[0];
    if (frame->sof != DIAG_ROUTER_FRAME_START_FLAG) {
        return ERRCODE_FAIL;
    }

    diag_router_ctrl_t *ctrl = (diag_router_ctrl_t *)&frame->ctrl;
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)(frame->inner + ctrl->en_extend);
    uint16_t payload_len = get_frame_len(inner->len_msb, inner->len_lsb);
    uint16_t calc_crc_len = payload_len;

    /* data_len使用uint32_t，防止翻转 */
    uint32_t data_len = (uint32_t)DIAG_ROUTER_HEADER_LEN + (uint32_t)ctrl->en_extend + (uint32_t)payload_len;

    /* 如果payload_len大于pkt数据长度，payload_len错误 */
    if (data_len > (uint32_t)pkt->data_len[0]) {
        return ERRCODE_FAIL;
    }

    /* 是否有结束标志 */
    if ((ctrl->en_eof) != 0) {
        calc_crc_len -= ((calc_crc_len > DIAG_ROUTER_EOF_LEN) ? DIAG_ROUTER_EOF_LEN : calc_crc_len);
    }

    /* 校验crc */
    if ((ctrl->en_crc) != 0) {
        uint16_t crc_val;
        calc_crc_len -= ((calc_crc_len > DIAG_ROUTER_CRC_LEN) ? DIAG_ROUTER_CRC_LEN : calc_crc_len);
        if ((ctrl->en_full_crc) != 0) {
            crc_val = uapi_crc16(0, (uint8_t *)frame, DIAG_FRAME_HEADER_SIZE + ctrl->en_extend + calc_crc_len);
        } else {
            crc_val = uapi_crc16(0, (uint8_t *)inner->payload, calc_crc_len);
        }

        uint16_t frame_crc = *(uint16_t *)(&inner->payload[calc_crc_len]);
        uint8_t frame_echo = (crc_val != frame_crc) ? 1 : 0;

        diag_router_check_and_send_ack(frame, process_param, frame_echo);

        /* 校验失败 */
        if (frame_echo != 0) {
            dfx_log_err("diag pkt check crc error! frame_crc = 0x%x crc_val = 0x%x\r\n", frame_crc, crc_val);
            return ERRCODE_DIAG_CRC_ERROR;
        }
    } else {
        /* 如果有效数据长度为0（去掉头尾），payload_len错误 */
        if (calc_crc_len == 0) {
            return ERRCODE_FAIL;
        }
    }

    /* frame len is no longer used and is used to record addr */
    inner->len_msb = process_param->dst_addr;
    inner->len_lsb = process_param->src_addr;
    return diag_service_data_proc(frame, (uint16_t)data_len);
}
#endif /* #if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES) */

static uint8_t diag_get_frame_module_id(diag_router_frame_t *frame, uint8_t extra_len, uint8_t src_addr)
{
    diag_ser_frame_t *ser_frame = (diag_ser_frame_t *)((uint8_t *)frame + extra_len);
    uint8_t module_id = ser_frame->module_id;
#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
    diag_channel_id_t id = diag_adapt_addr_2_channel_id((diag_addr)src_addr);
    if (id >= DIAG_SUPPORT_CHANNEL_CNT) {
        return module_id;
    }

    uint8_t sn = get_frame_ctrl_sn(frame->ctrl);
    /* 不是分包数据 */
    if (sn == 0) {
        g_diag_merge_flag[id].status = DIAG_DATA_NO_MERGE;
        g_diag_merge_flag[id].module_id = 0;
        return module_id;
    }

    /* 分包数据，只有第一包中包含module_id和cmd id */
    if (sn == DIAG_FRAME_SN_START) {
        /* 第一包，保存module_id */
        g_diag_merge_flag[id].module_id = module_id;
        g_diag_merge_flag[id].status = DIAG_DATA_MERGING;
        return module_id;
    } else if ((sn == DIAG_FRAME_SN_INSIDE) && (g_diag_merge_flag[id].status == DIAG_DATA_MERGING)) {
        return g_diag_merge_flag[id].module_id;
    } else if ((sn == DIAG_FRAME_SN_END) && (g_diag_merge_flag[id].status == DIAG_DATA_MERGING)) {
        g_diag_merge_flag[id].status = DIAG_DATA_MERGED;
        return g_diag_merge_flag[id].module_id;
    } else {
        dfx_log_err("the SN of package is incorrect.\r\n");
        return module_id;
    }
#else
    return module_id;
#endif /* #if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES) */
}

static bool diag_check_transmit_pkt(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param)
{
    uint8_t extra_len = 0;
    diag_router_frame_t *frame = (diag_router_frame_t *)pkt->data[0];
    uint8_t module_id;

    /* 数据保存至Flash的类型（如离线日志） */
    if (pkt->output_type == 1) {
        return false;
    }

    /* 加密的数据 */
#ifdef CONFIG_DIAG_WITH_SECURE
    if (get_frame_ctrl_secure_flag(frame->ctrl) != 0) {
        return false;
    }
#endif /* CONFIG_DIAG_WITH_SECURE */

    if (frame->sof != DIAG_ROUTER_FRAME_START_FLAG) {
        return false;
    }

    extra_len = diag_pkt_router_calc_prefix_len((diag_router_ctrl_t *)&(frame->ctrl),
        (diag_router_ctrl2_t *)&(frame->inner[0]));

    module_id = diag_get_frame_module_id(frame, extra_len, process_param->src_addr);
    if (module_id == DIAG_SER_FILE_TRANFER || module_id == DIAG_SER_OTA) {
        return true;
    }

    return false;
}

STATIC errcode_t diag_pkt_router_enqueue(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param)
{
    errcode_t ret;
    diag_pkt_msg_t msg;

    ret = diag_pkt_need_cross_task(pkt);
    if (ret != ERRCODE_SUCC) {
        dfx_log_err("[ERROR][diag_pkt_router_enqueue][%d][errcode=%u]\r\n", __LINE__, ret);
        return ret;
    }

    msg.pkt = *pkt;
    msg.param.cur_proc = DIAG_PKT_PROC_SYNC;
    msg.param.src_addr = process_param->src_addr;
    msg.param.dst_addr = process_param->dst_addr;

    if (diag_check_transmit_pkt(pkt, process_param)) {
        ret = transmit_msg_write(DFX_MSG_ID_DIAG_PKT, (uint8_t *)&msg, sizeof(diag_pkt_msg_t), false);
    } else {
        ret = dfx_msg_write(DFX_MSG_ID_DIAG_PKT, (uint8_t *)&msg, sizeof(diag_pkt_msg_t), false);
    }

    if (ret == ERRCODE_SUCC) {
        zdiag_dfx_send_local_q_success();
        diag_pkt_handle_clean(pkt);
    } else {
        zdiag_dfx_send_local_q_fail();
    }
    return ret;
}

STATIC errcode_t diag_pkt_router_process(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param)
{
    diag_frame_fid_t dst_addr = process_param->dst_addr;
    if (dst_addr != DIAG_FRAME_FID_LOCAL) {
        return diag_pkt_router_output(pkt, process_param);
    } else {
#if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES)
        return diag_pkt_router_notify(pkt, process_param);
#else
        return ERRCODE_DIAG_NOT_SUPPORT;
#endif
    }
}

errcode_t diag_pkt_router(diag_pkt_handle_t *pkt, diag_pkt_process_param_t *process_param)
{
    errcode_t ret = ERRCODE_FAIL;
    if (process_param->cur_proc == DIAG_PKT_PROC_ASYNC) {
        ret = diag_pkt_router_enqueue(pkt, process_param);
    } else {
        ret = diag_pkt_router_process(pkt, process_param);
    }

    diag_pkt_free(pkt);
    return ret;
}

/* 发送流程 */
STATIC errcode_t diag_pkt_router_send_single(diag_router_data_t *data)
{
    diag_pkt_handle_t pkt;
    diag_pkt_process_param_t process_param;

    uint16_t send_len = data->data_len + diag_pkt_router_calc_prefix_len(&(data->ctrl), &(data->ctrl2)) +
        diag_pkt_router_calc_suffix_len(&(data->ctrl));

    uint8_t *buffer = dfx_malloc(0, send_len);
    if (buffer == NULL) {
        return ERRCODE_MALLOC;
    }
    diag_router_build_frame((diag_router_frame_t *)buffer, send_len, data);

    diag_pkt_handle_init(&pkt, 1);
    diag_pkt_handle_set_data(&pkt, DIAG_PKT_DATA_ID_0, buffer, send_len, DIAG_PKT_STACK_DATA);

    process_param.cur_proc = DIAG_PKT_PROC_SYNC;
    process_param.dst_addr = data->fid >> DIAG_ROUTER_FID_DST_BIT;
    process_param.src_addr = data->fid & DIAG_ROUTER_FID_MASK;
    errcode_t ret = diag_pkt_router(&pkt, &process_param);
    dfx_free(0, buffer);
    return ret;
}

#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
/* 拆包 */
STATIC errcode_t diag_pkt_router_send_loop(diag_router_data_t *router_data)
{
    errcode_t ret;
    uint16_t offset    = 0;
    uint16_t valid_mfs = 0;
    uint16_t size      = router_data->data_len;
    uint16_t non_payload_len = diag_pkt_router_calc_prefix_len(&(router_data->ctrl), &(router_data->ctrl2)) +
        diag_pkt_router_calc_suffix_len(&(router_data->ctrl));

    if (router_data->mfs <= non_payload_len) {
        dfx_log_err("diag pkt mfs error! valid_mfs = 0x%x\r\n", valid_mfs);
        return ERRCODE_DIAG_BAD_DATA;
    }

    valid_mfs = router_data->mfs - non_payload_len;

    diag_router_data_t *send_data = router_data;
    while (offset < size) {
        /* first frame data */
        if (offset == 0) {
            send_data->data_len = valid_mfs;
            send_data->ctrl.en_sn = FRAME_SN_FLAG_START;
        } else if (offset + valid_mfs > size) {
            send_data->data_len = size - offset;
            send_data->ctrl.en_sn = FRAME_SN_FLAG_END;
        } else {
            send_data->data_len = valid_mfs;
            send_data->ctrl.en_sn = FRAME_SN_FLAG_ONGOING;
        }

        ret = diag_pkt_router_send_single(send_data);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
        offset += valid_mfs;
        send_data->data += valid_mfs;
        send_data->sn_count++;
    }
    return ERRCODE_SUCC;
}
#endif /* #if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES) */

errcode_t diag_pkt_router_send(diag_router_data_t *router_data)
{
    if (router_data == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if ((router_data->ctrl.en_sn) != 0) {
#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
        return diag_pkt_router_send_loop(router_data);
#else
        return ERRCODE_DIAG_NOT_SUPPORT;
#endif
    } else {
        return diag_pkt_router_send_single(router_data);
    }
}

void diag_pkt_msg_proc(uint32_t msg_id, uint8_t *msg, uint32_t msg_len)
{
    diag_pkt_msg_t *pkt_msg = (diag_pkt_msg_t *)msg;
    diag_pkt_process_param_t process_param;
    process_param.cur_proc = pkt_msg->param.cur_proc;
    process_param.src_addr = pkt_msg->param.src_addr;
    process_param.dst_addr = pkt_msg->param.dst_addr;
    diag_pkt_router(&pkt_msg->pkt, &process_param);
    unused(msg_id);
    unused(msg_len);
}
