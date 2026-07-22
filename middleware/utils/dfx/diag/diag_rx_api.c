/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: diag rx
 * This file should be changed only infrequently and with great care.
 */
#include "diag_rx_api.h"
#include "securec.h"
#include "diag_mem.h"
#include "diag_pkt.h"
#include "errcode.h"
#include "diag_channel_item.h"
#include "diag_pkt_router.h"
#include "zdiag_adapt_layer.h"
#include "diag_dfx.h"
#include "uapi_crc.h"

#define MUX_START_OFFSET 0
#define MAX_MUX_PARAM_SIZE 1024

#if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES)

STATIC int32_t diag_hso_find_start_flag(uint8_t *data, uint16_t size)
{
    int i;
    int pos = -1;
    for (i = 0; i < size; i++) {
        if (data[i] == DIAG_ROUTER_FRAME_START_FLAG) {
            pos = i;
            break;
        }
    }
    return pos;
}

STATIC uint16_t diag_get_frame_len(diag_router_frame_t *frame)
{
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)frame->inner;
    uint16_t frame_len = DIAG_ROUTER_HEADER_LEN;
    if (get_frame_ctrl_extend_en(frame->ctrl) != 0) {
        inner = (diag_router_inner_frame_t *)(frame->inner + DIAG_ROUTER_CTRL2_LEN);
        frame_len += DIAG_ROUTER_CTRL2_LEN;
    }
    frame_len += get_frame_len(inner->len_msb, inner->len_lsb);
    return frame_len;
}

STATIC int32_t soc_diag_chan_copy_data(diag_channel_item_t *item, uint8_t *data, uint16_t size)
{
    uint16_t free_size = item->rx_buf_len - item->rx_buf_pos;
    uint16_t copy_size = uapi_min(size, free_size);
    if (memcpy_s(&item->rx_buf[item->rx_buf_pos], free_size, data, copy_size) != EOK) {
        return 0;
    }
    item->rx_buf_pos += copy_size;
    return copy_size;
}

STATIC void diag_rest_rx_buf(diag_channel_item_t *item)
{
    item->rx_buf_pos = MUX_START_OFFSET;
    item->rx_buf_is_using = false;
}

STATIC uint16_t diag_get_rx_data_size(diag_channel_item_t *item)
{
    return item->rx_buf_pos - MUX_START_OFFSET;
}

STATIC errcode_t diag_chan_move_buf(diag_channel_item_t *item, uint32_t dst, uint32_t src, uint32_t len)
{
    if (memmove_s(&item->rx_buf[dst], item->rx_buf_len, &item->rx_buf[src], len) != EOK) {
        return ERRCODE_FAIL;
    }
    item->rx_buf_pos -= (uint16_t)(src - dst);
    return ERRCODE_SUCC;
}

STATIC errcode_t diag_check_frame_crc(diag_router_frame_t *frame, uint16_t frame_len)
{
    uint16_t cal_crc;
    uint16_t frame_crc;

    unused(frame_len);

    diag_router_ctrl_t *ctrl = (diag_router_ctrl_t *)&frame->ctrl;
    if ((ctrl->en_crc) == 0) {
        /* CRC校验未使能，直接返回成功 */
        return ERRCODE_SUCC;
    }

    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)frame->inner;
    /* 是否有扩展CTRL字段 */
    if ((ctrl->en_extend) != 0) {
        inner = (diag_router_inner_frame_t *)(frame->inner + DIAG_ROUTER_CTRL2_LEN);
    }

    uint16_t payload_len = get_frame_len(inner->len_msb, inner->len_lsb);
    /* 是否有结束标志 */
    if ((ctrl->en_eof) != 0) {
        /* 确保payload_len不发生翻转 */
        payload_len -= ((payload_len > DIAG_ROUTER_EOF_LEN) ? DIAG_ROUTER_EOF_LEN : payload_len);
    }

    /* 确保payload_len不发生翻转 */
    payload_len -= ((payload_len > DIAG_ROUTER_CRC_LEN) ? DIAG_ROUTER_CRC_LEN : payload_len);

    /* 校验crc */
    if ((ctrl->en_full_crc) != 0) {
        cal_crc = uapi_crc16(0, (uint8_t *)frame, DIAG_FRAME_HEADER_SIZE + ctrl->en_extend + payload_len);
    } else {
        cal_crc = uapi_crc16(0, (uint8_t *)inner->payload, payload_len);
    }
    frame_crc = *(uint16_t *)(&inner->payload[payload_len]);
    if (cal_crc != frame_crc) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

STATIC diag_router_frame_t *diag_chan_full_pkt_receive(diag_channel_item_t *item)
{
    if (diag_get_rx_data_size(item) < DIAG_ROUTER_HEADER_LEN) {
        return NULL;
    }
    diag_router_frame_t *frame = (diag_router_frame_t *)&item->rx_buf[MUX_START_OFFSET];

    uint16_t frame_len = diag_get_frame_len(frame);
    if (frame_len >= item->rx_buf_len) {
        /*
         * 帧长度大于buf长度，可能是找到了一个错误的帧头，取出一个字节，继续向后查找SOF。
         */
        diag_chan_move_buf(item, MUX_START_OFFSET, 1, (uint32_t)(item->rx_buf_pos - 1));
        return NULL;
    }

    if (diag_get_rx_data_size(item) < frame_len) {
        return NULL;
    }

    if (diag_check_frame_crc(frame, frame_len) != ERRCODE_SUCC) {
        /*
         * CRC校验失败，可能是找到了一个错误的帧头，取出一个字节，继续向后查找SOF。
         */
        diag_chan_move_buf(item, MUX_START_OFFSET, 1, (uint32_t)(item->rx_buf_pos - 1));
        return NULL;
    }

    return frame;
}

STATIC errcode_t diag_channel_process_frame(diag_channel_id_t id, diag_router_frame_t *frame)
{
    diag_pkt_handle_t pkt;
    diag_pkt_process_param_t process_param;

    uint16_t frame_len = diag_get_frame_len(frame);
    diag_pkt_handle_init(&pkt, 1);
    diag_pkt_handle_set_data(&pkt, DIAG_PKT_DATA_ID_0, (uint8_t*)frame, frame_len, DIAG_PKT_STACK_DATA);

    process_param.cur_proc = DIAG_PKT_PROC_ASYNC;
    process_param.src_addr = diag_adapt_channel_id_2_addr(id);
    process_param.dst_addr = DIAG_FRAME_FID_LOCAL; /* 接收到的数据dst_addr改为LOCAL */
    return diag_pkt_router(&pkt, &process_param);
}

STATIC int32_t diag_chan_rx_frame_uart_data(diag_channel_id_t id, uint8_t *data, uint16_t size)
{
    int32_t copied_size = 0;
    int32_t start_flag_pos;
    int32_t frame_count = 0;
    diag_router_frame_t *frame = NULL;

    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL) {
        return 0;
    }

    while (copied_size != size && item->rx_buf_pos != item->rx_buf_len) { /* 数据未处理完成 并且 接收缓存有空间 */
        copied_size += soc_diag_chan_copy_data(item, data + copied_size, (uint16_t)(size - copied_size));
        if (item->rx_buf_is_using) {
            return frame_count;
        }

        if (diag_get_rx_data_size(item) < DIAG_ROUTER_HEADER_LEN) {
            continue;
        }

        start_flag_pos = diag_hso_find_start_flag(&item->rx_buf[MUX_START_OFFSET], item->rx_buf_pos - MUX_START_OFFSET);
        if (start_flag_pos < 0) {
            diag_rest_rx_buf(item);
            continue;
        } else if (start_flag_pos > 0) {
            diag_chan_move_buf(item, MUX_START_OFFSET, (uint32_t)start_flag_pos,
                               (uint32_t)(item->rx_buf_pos - start_flag_pos));
        }

        do {
            frame = diag_chan_full_pkt_receive(item);
            if (frame == NULL) {
                break;
            }
            uint16_t frame_len = diag_get_frame_len(frame);
            (void)diag_channel_process_frame(id, frame);

            frame_count++;
            diag_chan_move_buf(item, MUX_START_OFFSET, frame_len, item->rx_buf_pos - frame_len);
        } while (true);
    }
    return frame_count;
}

STATIC errcode_t diag_channel_rx_data(diag_channel_id_t id, uint8_t *data, uint16_t size)
{
    errcode_t ret;

    if (data == NULL || size == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    diag_router_frame_t *frame = (diag_router_frame_t *)data;

    ret = diag_check_mux_pkt(frame, size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return diag_channel_process_frame(id, frame);
}

#endif /* #if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES) */

/* 字符串数据接收函数，data中是字符数据，可能需要组包(形成完整Diag帧数据)，也可能包含多个帧 */
int32_t uapi_diag_channel_rx_mux_char_data(diag_channel_id_t id, uint8_t *data, uint16_t size)
{
#if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES)
    int32_t frame_cnt;

    diag_dfx_channel_receive_data(id, size);
    frame_cnt = diag_chan_rx_frame_uart_data(id, data, size);
    diag_dfx_channel_receive_frame(id, (uint32_t)frame_cnt);
    return frame_cnt;
#else
    return 0;
#endif
}

/* Diag帧数据接收函数，data中是一个完整的Diag帧数据，无需组包 */
errcode_t uapi_diag_channel_rx_frame_data(diag_channel_id_t id, uint8_t *data, uint16_t size)
{
#if (CONFIG_DFX_SUPPORT_DIAG_RX_PKT == DFX_YES)
    diag_dfx_channel_receive_data(id, size);
    errcode_t ret = diag_channel_rx_data(id, data, size);
    if (ret == ERRCODE_SUCC) {
        diag_dfx_channel_receive_frame(id, 1);
    }
    return ret;
#else
    return ERRCODE_DIAG_NOT_SUPPORT;
#endif
}
