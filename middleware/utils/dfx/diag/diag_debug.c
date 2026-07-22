/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: diag debug
 * This file should be changed only infrequently and with great care.
 */

#include "diag_debug.h"
#include "zdiag_adapt_layer.h"
#include "diag_common.h"
#include "soc_diag_util.h"
#include "diag_log.h"

uint32_t g_dfx_debug_level = DIAG_LOG_LEVEL_ERROR;

STATIC void printf_frame_header(const char *str, const diag_router_frame_t *frame)
{
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)frame->inner;
    if (get_frame_ctrl_extend_en(frame->ctrl) != 0) {
        inner = (diag_router_inner_frame_t *)(frame->inner + DIAG_ROUTER_CTRL2_LEN);
    }
    dfx_log_debug("[%s][frame][sof=0x%02x][ctrl=0x%02x]", str, frame->sof, frame->ctrl);
    if (get_frame_ctrl_extend_en(frame->ctrl) != 0) {
        dfx_log_debug("[ctrl2=0x%02x]", frame->inner[0]);
    }
    dfx_log_debug("[len=0x%02x]\r\n", get_frame_len(inner->len_msb, inner->len_lsb));
    unused(inner);
    unused(str);
}

uint32_t diag_get_debug_level(void)
{
    return g_dfx_debug_level;
}

void diag_set_debug_level(uint32_t level)
{
    g_dfx_debug_level = level;
}

void zdiag_debug_print_pkt_info(const char *str, uint8_t *pkt)
{
    diag_router_frame_t *frame = (diag_router_frame_t *)pkt;

    printf_frame_header(str, frame);
}

void zdiag_pkt_printf(const char *str, diag_pkt_handle_t *pkt)
{
    diag_router_frame_t *frame = diag_pkt_handle_get_frame(pkt);
    dfx_log_debug("%s cnt=%u data_len[0]=%u data_len[1]=%u data_len[2]=%u\r\n",
                  str, pkt->data_cnt,
                  pkt->data_len[DIAG_PKT_DATA_ID_0],
                  pkt->data_len[DIAG_PKT_DATA_ID_1],
                  pkt->data_len[DIAG_PKT_DATA_ID_2]);
    unused(frame);
    unused(str);
}
