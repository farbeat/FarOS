/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: zdiag service
 */

#include "diag_service.h"
#include "diag_pkt_router.h"
#include "diag_channel.h"
#include "diag_channel_item.h"
#include "errcode.h"
#include "stdbool.h"
#include "securec.h"
#include "dfx_adapt_layer.h"
#include "zdiag_adapt_layer.h"
#include "diag_cmd_dispatch.h"
#ifdef CONFIG_DIAG_WITH_SECURE
#include "diag_secure.h"
#endif

static diag_merge_data_t g_merge_data[DIAG_SUPPORT_CHANNEL_CNT] = {0};
static diag_notify_f g_diag_ser_func[DIAG_SER_MAX] = {NULL};

static diag_router_inner_frame_t *diag_get_router_inner_frame(diag_router_frame_t *data)
{
    diag_router_inner_frame_t *inner = (diag_router_inner_frame_t *)data->inner;
    if (get_frame_ctrl_extend_en(data->ctrl) != 0) {
        inner = (diag_router_inner_frame_t *)(data->inner + DIAG_ROUTER_CTRL2_LEN);
    }
    return inner;
}

#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
static errcode_t diag_service_data_merge(diag_ser_frame_t *frame, diag_router_frame_t *data,
    uint16_t payload_len, uint8_t cur_sn)
{
    diag_router_inner_frame_t *inner = diag_get_router_inner_frame(data);
    uint8_t sn_ctrl = get_frame_ctrl_sn(data->ctrl);
    diag_channel_id_t id = diag_adapt_addr_2_channel_id((diag_addr)inner->len_lsb); /* inner->len_lsb : src_addr */
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    diag_merge_data_status merge_status = DIAG_DATA_NO_MERGE;
    bool need_process = false;

    if (item == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    uint8_t *src_buf = (uint8_t *)(uintptr_t)frame;

    if (sn_ctrl == DIAG_FRAME_SN_START && g_merge_data[id].status != DIAG_DATA_MERGING) {
        /* 收到第一包，且当前状态是初始状态或组包结束状态 */
        dfx_log_debug("diag_service_data_merge: SN start! cur_sn = %d\r\n", cur_sn);
        if (g_merge_data[id].buff == NULL) {
            g_merge_data[id].buff = dfx_malloc(0, item->max_serv_pkg_len + sizeof(diag_ser_data_t));
        }

        g_merge_data[id].module_id = frame->module_id;
        g_merge_data[id].cmd_id = frame->cmd_id;
        g_merge_data[id].offset = (uint16_t)sizeof(diag_ser_data_t);
        merge_status = DIAG_DATA_MERGING;
    } else if (sn_ctrl == DIAG_FRAME_SN_INSIDE && g_merge_data[id].status == DIAG_DATA_MERGING &&
        g_merge_data[id].last_sn + 1 == cur_sn) {
        /* 收到中间包，且当前状态是组包状态，且当前SN号是连续的 */
        dfx_log_debug("diag_service_data_merge: SN inside. cur_sn = %d\r\n", cur_sn);
        merge_status = DIAG_DATA_MERGING;
    } else if (sn_ctrl == DIAG_FRAME_SN_END && g_merge_data[id].status == DIAG_DATA_MERGING &&
        g_merge_data[id].last_sn + 1 == cur_sn) {
        /* 收到最后结尾包，且当前状态是组包状态，且当前SN号是连续的 */
        dfx_log_debug("diag_service_data_merge: SN end. cur_sn = %d\r\n", cur_sn);
        merge_status = DIAG_DATA_MERGED;
        need_process = true;
    } else {
        /* 错误场景，释放buff */
        dfx_log_err("diag_service_data_merge: SN err. cur_sn = %d\r\n", cur_sn);
        if (g_merge_data[id].buff != NULL) {
            dfx_free(0, g_merge_data[id].buff);
        }
        (void)memset_s(&g_merge_data[id], sizeof(diag_merge_data_t), 0, sizeof(diag_merge_data_t));
        return ERRCODE_FAIL;
    }

    if (g_merge_data[id].buff == NULL) {
        return ERRCODE_MALLOC;
    }

    uint8_t *dst_buf = g_merge_data[id].buff + g_merge_data[id].offset;
    (void)memcpy_s(dst_buf, payload_len, src_buf, payload_len);

    g_merge_data[id].offset += payload_len;
    g_merge_data[id].status = merge_status;
    g_merge_data[id].last_sn = cur_sn;

    return (need_process == true) ? ERRCODE_SUCC : ERRCODE_FAIL;
}
#endif /* #if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES) */

#ifdef CONFIG_DIAG_WITH_SECURE
static errcode_t diag_make_frame_secure(diag_ser_frame_t **frame, diag_router_frame_t *data, uint16_t secure_len,
                                        uint8_t *extra_len)
{
    if (get_frame_ctrl_secure_flag(data->ctrl) != 0) {
        diag_secure_ctx_t *secure_ctx = diag_get_secure_ctx();
        uint8_t key_check[AES128_KEY_LEN];
        memset_s(key_check, AES128_KEY_LEN, 0, AES128_KEY_LEN);
        if (memcmp(secure_ctx->srp_info.key, key_check, AES128_KEY_LEN) != 0) {
            return ERRCODE_FAIL;
        }
        uint8_t *iv = (uint8_t *)((uint8_t *)data + DIAG_ROUTER_HEADER_LEN + *extra_len);
        uint8_t *tag = (uint8_t *)((uint8_t *)data + DIAG_ROUTER_HEADER_LEN + *extra_len + AES_IV_LEN);
        *extra_len += DIAG_ROUTER_SECURE_LEN;
        *frame = (diag_ser_frame_t *)((uint8_t *)data + DIAG_ROUTER_HEADER_LEN + *extra_len);
        uint16_t decrypt_size = secure_len - DIAG_ROUTER_SECURE_LEN;
        errcode_t ret = diag_aes_gcm_decrypt_inplace(*frame, decrypt_size, iv, tag);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    } else {
        *frame = (diag_ser_frame_t *)((uint8_t *)data + DIAG_ROUTER_HEADER_LEN + *extra_len);
    }
    return ERRCODE_SUCC;
}
#endif

static errcode_t diag_service_build_notify_data(diag_ser_data_t **notify_data, diag_channel_id_t *channel_id,
    diag_ser_frame_t *frame, diag_router_frame_t *data, uint16_t payload_len)
{
    diag_ser_data_t *notify_buf = NULL;
    uint8_t module_id;
    uint8_t cmd_id;
    uint16_t data_len;

    diag_router_inner_frame_t *inner = diag_get_router_inner_frame(data);
    diag_channel_id_t id = diag_adapt_addr_2_channel_id((diag_addr)inner->len_lsb); /* inner->len_lsb : src_addr */
    if (id >= DIAG_SUPPORT_CHANNEL_CNT) {
        dfx_log_err("diag_service_build_notify_data: src addr is valid.src_addr = %d, id = %d\r\n", inner->len_lsb, id);
        return ERRCODE_INVALID_PARAM;
    }

    *channel_id = id;

    if (g_merge_data[id].status == DIAG_DATA_MERGED) {
        notify_buf = (diag_ser_data_t *)g_merge_data[id].buff;
        module_id = g_merge_data[id].module_id;
        cmd_id = g_merge_data[id].cmd_id;
        data_len = g_merge_data[id].offset - (uint16_t)sizeof(diag_ser_data_t);
    } else {
        data_len = payload_len;

        notify_buf = (diag_ser_data_t *)dfx_malloc(0, data_len + (uint16_t)sizeof(diag_ser_header_t));
        if (notify_buf == NULL) {
            return ERRCODE_MALLOC;
        }

        (void)memcpy_s((uint8_t *)notify_buf->payload, data_len, frame, data_len);
        module_id = frame->module_id;
        cmd_id = frame->cmd_id;
    }

    notify_buf->header.ser_id  = module_id;
    notify_buf->header.cmd_id  = cmd_id;
    notify_buf->header.src     = inner->len_lsb;
    notify_buf->header.dst     = inner->len_msb;
    notify_buf->header.crc_en  = 0;
    notify_buf->header.ack_en  = 0;
    notify_buf->header.length  = data_len;

    *notify_data = notify_buf;
    return ERRCODE_SUCC;
}

static errcode_t diag_service_frame_proc(diag_ser_frame_t *frame, diag_router_frame_t *data, uint16_t payload_len)
{
    diag_channel_id_t id = DIAG_CHANNEL_ID_0;
    diag_ser_data_t *notify_data = NULL;
    uint8_t module_id;
    uint8_t cmd_id;

    errcode_t ret = diag_service_build_notify_data(&notify_data, &id, frame, data, payload_len);
    if (ret != ERRCODE_SUCC || notify_data == NULL) {
        goto err;
    }

    module_id = notify_data->header.ser_id;
    cmd_id = notify_data->header.cmd_id;
    unused(cmd_id);
#ifdef CONFIG_DIAG_WITH_SECURE
    /* 如果该帧不加密，但该命令要求加密，返回失败 */
    if ((get_frame_ctrl_secure_flag(data->ctrl) == 0) && (diag_need_secure(makeu16(cmd_id, module_id)))) {
        ret = ERRCODE_FAIL;
        goto err;
    }
#endif

    ret = ERRCODE_DIAG_OBJ_NOT_FOUND;
    if (module_id < DIAG_SER_MAX) {
        if (g_diag_ser_func[module_id]) {
            ret = g_diag_ser_func[module_id](notify_data);
        }
    }

err:
    if (notify_data != NULL) {
        if ((uint8_t *)notify_data == g_merge_data[id].buff) {
            dfx_free(0, g_merge_data[id].buff);
            (void)memset_s(&g_merge_data[id], sizeof(diag_merge_data_t), 0, sizeof(diag_merge_data_t));
        } else {
            dfx_free(0, notify_data);
        }
    }
    return ret;
}

/* data为链路层包起始，data_len为包总长(从SOF到结尾) */
errcode_t diag_service_data_proc(diag_router_frame_t *data, uint16_t data_len)
{
    errcode_t ret = ERRCODE_FAIL;
    uint8_t extra_len = 0;
    uint8_t *buf = (uint8_t *)(uintptr_t)data;
    uint8_t cur_sn = 0;

    /* payload_len标记payload字段的长度，从module_id开始到payload结束（不含CRC） */
    uint16_t payload_len = data_len - DIAG_ROUTER_HEADER_LEN;

    if (data == NULL || data_len == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    /* ctrl2 en */
    extra_len += get_frame_ctrl_extend_en(data->ctrl);

    /* fid en */
    extra_len += get_frame_ctrl_fid_en(data->ctrl);

    /* 是否是分包数据 */
    if (get_frame_ctrl_sn(data->ctrl) != 0) {
        cur_sn = buf[DIAG_ROUTER_HEADER_LEN + extra_len];
        extra_len++;
    }

    /* echo en */
    if ((get_frame_ctrl_extend_en(data->ctrl) != 0) && (get_frame_ctrl2_ack_type(data->inner[0])
        == FRAME_ACK_TYPE_ECHO)) {
        extra_len++;
    }

    if (get_frame_ctrl_crc_en(data->ctrl) != 0) {
        payload_len -= DIAG_ROUTER_CRC_LEN;
    }

    payload_len -= get_frame_ctrl_eof_en(data->ctrl);

#ifdef CONFIG_DIAG_WITH_SECURE
    diag_ser_frame_t *frame = NULL;
    uint16_t secure_len = payload_len - extra_len;
    ret = diag_make_frame_secure(&frame, data, secure_len, &extra_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
#else
    diag_ser_frame_t *frame = (diag_ser_frame_t *)((uint8_t *)data + DIAG_ROUTER_HEADER_LEN + extra_len);
#endif

    payload_len -= extra_len;

    /* 是否是分包数据 */
    if (get_frame_ctrl_sn(data->ctrl) != 0) {
#if (CONFIG_DFX_SUPPORT_DIAG_MULTI_FRAME == DFX_YES)
        ret = diag_service_data_merge(frame, data, payload_len, cur_sn);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
#else
        unused(cur_sn);
        return ERRCODE_DIAG_NOT_SUPPORT;
#endif
    }
    ret = diag_service_frame_proc(frame, data, payload_len);
    return ret;
}

errcode_t uapi_diag_service_register(diag_ser_id_t module_id, diag_notify_f func)
{
    if (module_id >= DIAG_SER_MAX) {
        return ERRCODE_INVALID_PARAM;
    }
    g_diag_ser_func[module_id] = func;
    return ERRCODE_SUCC;
}

errcode_t uapi_diag_service_send_data(diag_ser_data_t *data)
{
    if (data == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    diag_router_data_t router_data = {0};
    diag_ser_header_t *header = &data->header;

    bool fid_en = false;
    if ((header->dst != 0) || (header->src != 0)) {
        fid_en = true;
    }

    uint8_t dst = (header->dst == 0) ? DIAG_FRAME_FID_PC : header->dst;
    diag_channel_id_t id = diag_adapt_addr_2_channel_id((diag_addr)dst);
    diag_channel_item_t *item = diag_chan_idx_2_item(id);
    if (item == NULL) {
        dfx_log_err("uapi_diag_service_send_data: dst addr is valid. dst_addr = %d, id = %d\r\n", dst, id);
        return ERRCODE_INVALID_PARAM;
    }

    router_data.ctrl.en_crc = header->crc_en;
    router_data.ctrl.en_fid = fid_en;
    router_data.ctrl.en_sn  = FRAME_SN_FLAG_NONE;
    if (header->ack_en == true) {
        router_data.ctrl.en_extend = 1;
        router_data.ctrl2.ack_type = FRAME_ACK_TYPE_ACK;
    }

    router_data.fid         = (uint8_t)(header->dst << DIAG_ROUTER_FID_DST_BIT | header->src);
    router_data.sn_count    = 0;
    router_data.echo        = 0;
    router_data.mfs         = item->mfs;

    router_data.data        = data->payload;
    router_data.data_len    = header->length;

    uint16_t total_len = router_data.data_len +
        diag_pkt_router_calc_prefix_len(&(router_data.ctrl), &(router_data.ctrl2)) +
        diag_pkt_router_calc_suffix_len(&(router_data.ctrl));

    /* 先按不分包计算整包长度，如大于mfs， 则需要分包发送 */
    if (total_len > item->mfs) {
        router_data.ctrl.en_sn = FRAME_SN_FLAG_START;
    }

    return diag_pkt_router_send(&router_data);
}

void uapi_diag_service_init(void)
{
    uapi_diag_service_register(DIAG_SER_MAINTENANCE, uapi_zdiag_cmd_process);
    return;
}