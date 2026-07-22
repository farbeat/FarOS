/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: zdiag pkt
 * This file should be changed only infrequently and with great care.
 */
#include "diag_pkt.h"
#include "zdiag_adapt_layer.h"
#include "diag.h"
#include "diag_filter.h"
#include "errcode.h"
#include "common_def.h"

errcode_t diag_check_mux_pkt(const diag_router_frame_t *frame, uint16_t size)
{
    unused(size);
    if (frame->sof != DIAG_ROUTER_FRAME_START_FLAG) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

void diag_pkt_handle_init(diag_pkt_handle_t *pkt, uint8_t data_cnt)
{
    memset_s(pkt, sizeof(diag_pkt_handle_t), 0, sizeof(diag_pkt_handle_t));
    pkt->data_cnt = data_cnt;
}

void diag_pkt_set_critical(diag_pkt_handle_t *pkt)
{
    pkt->critical = 1;
}

void diag_pkt_set_output_type(diag_pkt_handle_t *pkt, uint8_t type)
{
    pkt->output_type = type;
}

void diag_pkt_handle_set_data(diag_pkt_handle_t *pkt, uint8_t idx, uint8_t *data, uint16_t data_len,
    diag_pkt_data_t attribute)
{
    unused(attribute);
    pkt->data[idx] = data;
    pkt->data_len[idx] = data_len;

    if (((uint32_t)attribute & DIAG_PKT_DATA_ATTRIBUTE_SINGLE_TASK) != 0) {
        pkt->single_task = true;
    }

    if (((uint32_t)attribute & DIAG_PKT_DATA_ATTRIBUTE_DYN_MEM) != 0) {
        pkt->need_free = true;
    }
    return;
}

void diag_mk_log_pkt(diag_cmd_log_layer_stru_t *log_pkt, uint32_t module_id, uint32_t msg_id)
{
    STATIC uint32_t msg_sn = 0;
    log_pkt->module = module_id;
    log_pkt->dest_mod = diag_adapt_get_local_addr();
    log_pkt->no = msg_sn++;
    log_pkt->id = msg_id;
    log_pkt->time = diag_adapt_get_msg_time();
}

void diag_mk_normal_log_pkt(diag_cmd_normal_log_layer_t *log_pkt, uint8_t module_id, uint32_t msg_id)
{
    STATIC uint16_t msg_sn = 0;
    uint64_t time_ms = diag_adapt_get_msg_time_ms();
    log_pkt->module = module_id;
    log_pkt->no_lo = (uint8_t)(msg_sn & 0xFF);          /* 0xFF： get lower 8 bits */
    log_pkt->no_hi = (uint8_t)((msg_sn >> 8) & 0x3F);   /* 8, 0x3F： get upper 6 bits */
    log_pkt->id = msg_id;
    log_pkt->time = (uint32_t)(time_ms / 1000);     /* 1 second = 1000 milliseconds */
    log_pkt->time_ms = (uint16_t)(time_ms % 1000);  /* 1 second = 1000 milliseconds */
    msg_sn++;
}

void diag_mk_extend_log_pkt(diag_cmd_extend_log_layer_t *log_pkt, uint8_t module_id, uint32_t msg_id)
{
    STATIC uint8_t msg_sn = 0;
    log_pkt->sub_mod = module_id;
    log_pkt->no = msg_sn++;
    log_pkt->id = msg_id;
    log_pkt->time = diag_adapt_get_msg_local_time();
}

void diag_mk_min_log_pkt(diag_cmd_minimal_log_layer_t *log_pkt, uint32_t msg_id)
{
    STATIC uint16_t msg_sn = 0;
    log_pkt->no = msg_sn++;
    log_pkt->id = msg_id;
}

void diag_mk_log_pkt_sn(diag_cmd_log_layer_stru_t *log_pkt, uint32_t module_id, uint32_t msg_id, uint32_t sn)
{
    log_pkt->module = module_id;
    log_pkt->dest_mod = diag_adapt_get_local_addr();
    log_pkt->no = sn;
    log_pkt->id = msg_id;
    log_pkt->time = diag_adapt_get_msg_time();
}

void diag_mk_normal_log_pkt_sn(diag_cmd_normal_log_layer_t *log_pkt, uint8_t module_id, uint32_t msg_id, uint16_t sn)
{
    uint64_t time_ms = diag_adapt_get_msg_time_ms();
    log_pkt->module = module_id;
    log_pkt->no_lo = (uint8_t)(sn & 0xFF);          /* 0xFF： Lower 8 bits */
    log_pkt->no_hi = (uint8_t)((sn >> 8) & 0x3F);   /* 0x3F： Upper 6 bits */
    log_pkt->id = msg_id;
    log_pkt->time = (uint32_t)(time_ms / 1000);     /* 1 second = 1000 milliseconds */
    log_pkt->time_ms = (uint16_t)(time_ms % 1000);  /* 1 second = 1000 milliseconds */
}

void diag_mk_extend_log_pkt_sn(diag_cmd_extend_log_layer_t *log_pkt, uint8_t module_id, uint32_t msg_id, uint8_t sn)
{
    log_pkt->sub_mod = module_id;
    log_pkt->no = sn;
    log_pkt->id = msg_id;
    log_pkt->time = diag_adapt_get_msg_local_time();
}

void diag_mk_min_log_pkt_sn(diag_cmd_minimal_log_layer_t *log_pkt, uint32_t msg_id, uint16_t sn)
{
    log_pkt->no = sn;
    log_pkt->id = msg_id;
}

void diag_mk_req_header(diag_head_req_stru_t *req, uint16_t cmd_id)
{
    req->mod_id = hiu8(cmd_id);
    req->cmd_id = lou8(cmd_id);
    return;
}

void diag_mk_ind_header(diag_head_ind_stru_t *ind, uint16_t cmd_id)
{
    ind->mod_id = hiu8(cmd_id);
    ind->cmd_id = lou8(cmd_id);
    return;
}

void diag_mk_frame_header_1(diag_router_frame_t *frame, uint16_t pkt_size)
{
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)frame->inner;
    frame->sof      = DIAG_ROUTER_FRAME_START_FLAG;
    frame->ctrl     = 0;
    inner->len_msb  = (uint8_t)(pkt_size >> DIAG_FRAME_SHIFT_8);
    inner->len_lsb  = (uint8_t)(pkt_size);
    return;
}
