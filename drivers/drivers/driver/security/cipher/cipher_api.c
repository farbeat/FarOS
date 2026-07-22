/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides cipher driver header \n
 * Author: @CompanyNameTag \n
 */

#include "cipher_api.h"
#include "common_def.h"
#include "symc.h"


errcode_t uapi_drv_cipher_symc_init(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_symc_deinit(void)
{
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_symc_create(uint32_t *symc_handle, const uapi_drv_cipher_symc_attr_t *symc_attr)
{
    if (symc_attr == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    drv_cipher_attr_t attr;
    attr.cipher_type = DRV_CIPHER_TYPE_NORMAL;
    attr.lock_mode = symc_attr->is_long_term ? DRV_CIPHER_LONG_TERM_LOCK : DRV_CIPHER_SHORT_TERM_LOCK;

    return uapi_drv_cipher_create((uintptr_t *)(uintptr_t)symc_handle, &attr);
}

errcode_t uapi_drv_cipher_symc_destroy(uint32_t symc_handle)
{
    return uapi_drv_cipher_destroy((uintptr_t)symc_handle);
}


typedef union drv_cipher_config_param {
    drv_cipher_config_aes_t     aes_param;
    drv_cipher_config_aes_gcm_t aes_gcm_param;
    drv_cipher_config_aes_ccm_t aes_ccm_param;
} drv_cipher_config_param_t;

typedef struct work_mode_pair {
    uapi_drv_cipher_symc_work_mode_t  symc_work_mode;
    drv_cipher_work_mode_t            work_mode;
} work_mode_pair_t;

static const work_mode_pair_t g_work_mode_pair[] = {
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_ECB,     DRV_CIPHER_WORK_MODE_ECB},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC,     DRV_CIPHER_WORK_MODE_CBC},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CTR,     DRV_CIPHER_WORK_MODE_CTR},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_OFB,     DRV_CIPHER_WORK_MODE_OFB},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CFB,     DRV_CIPHER_WORK_MODE_CFB},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CCM,     DRV_CIPHER_WORK_MODE_CCM},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_GCM,     DRV_CIPHER_WORK_MODE_GCM},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC_MAC, DRV_CIPHER_WORK_MODE_CBC_MAC},
    {UAPI_DRV_CIPHER_SYMC_WORK_MODE_CMAC,    DRV_CIPHER_WORK_MODE_CMAC},
};

static drv_cipher_work_mode_t convert_from_symc_work_mode(uapi_drv_cipher_symc_work_mode_t work_mode)
{
    for (uint32_t i = 0; i < array_size(g_work_mode_pair); i++) {
        if (g_work_mode_pair[i].symc_work_mode == work_mode) {
            return g_work_mode_pair[i].work_mode;
        }
    }

    return DRV_CIPHER_WORK_MODE_MAX;
}

static uapi_drv_cipher_symc_work_mode_t convert_to_symc_work_mode(drv_cipher_work_mode_t work_mode)
{
    for (uint32_t i = 0; i < array_size(g_work_mode_pair); i++) {
        if (g_work_mode_pair[i].work_mode == work_mode) {
            return g_work_mode_pair[i].symc_work_mode;
        }
    }

    return UAPI_DRV_CIPHER_SYMC_WORK_MODE_MAX;
}

static void convert_from_symc_param_aes(const uapi_drv_cipher_symc_ctrl_t *symc_ctrl, drv_cipher_config_param_t *param)
{
    drv_cipher_config_aes_t *aes_cfg = &(param->aes_param);

    aes_cfg->key_len = (drv_cipher_key_length_t)symc_ctrl->symc_key_length;
    aes_cfg->bit_width = (drv_cipher_bit_width_t)symc_ctrl->symc_bit_width;
    aes_cfg->key_parity = (drv_cipher_key_parity_t)symc_ctrl->key_parity;

    (void)memcpy_s(aes_cfg->iv, sizeof(aes_cfg->iv), symc_ctrl->iv, symc_ctrl->iv_length);

    switch (symc_ctrl->iv_change_flag) {
        case UAPI_DRV_CIPHER_SYMC_IV_DO_NOT_CHANGE:
            aes_cfg->iv_change_flag = DRV_CIPHER_IV_DO_NOT_CHANGE;
            break;

        case UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ONE_PKG:
            aes_cfg->iv_change_flag = DRV_CIPHER_IV_CHANGE_ONE_PKG;
            break;

        case UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ALL_PKG:
            aes_cfg->iv_change_flag = DRV_CIPHER_IV_CHANGE_ALL_PKG;
            break;

        default:
            aes_cfg->iv_change_flag = DRV_CIPHER_IV_CHANGE_MAX;
            break;
    }
}

static void convert_from_symc_param_aes_gcm(const uapi_drv_cipher_symc_ctrl_t *symc_ctrl,
    drv_cipher_config_param_t *param)
{
    drv_cipher_config_aes_gcm_t *aes_gcm = &(param->aes_gcm_param);
    uapi_drv_cipher_symc_config_aes_ccm_gcm_t *gcm = (uapi_drv_cipher_symc_config_aes_ccm_gcm_t *)symc_ctrl->param;

    aes_gcm->key_len = (drv_cipher_key_length_t)symc_ctrl->symc_key_length;
    aes_gcm->key_parity = (drv_cipher_key_parity_t)symc_ctrl->key_parity;
    aes_gcm->iv = (uint8_t *)symc_ctrl->iv;
    aes_gcm->iv_len = symc_ctrl->iv_length;
    (void)memcpy_s(aes_gcm->iv_ctr, sizeof(aes_gcm->iv_ctr), symc_ctrl->iv, symc_ctrl->iv_length);

    switch (symc_ctrl->iv_change_flag) {
        case UAPI_DRV_CIPHER_SYMC_GCM_IV_DO_NOT_CHANGE:
            aes_gcm->iv_change_flag = DRV_CIPHER_GCM_IV_DO_NOT_CHANGE;
            break;

        case UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_START:
            aes_gcm->iv_change_flag = DRV_CIPHER_GCM_IV_CHANGE_START;
            break;

        case UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_UPDATE:
            aes_gcm->iv_change_flag = DRV_CIPHER_GCM_IV_CHANGE_UPDATE;
            break;

        case UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_FINISH:
            aes_gcm->iv_change_flag = DRV_CIPHER_GCM_IV_CHANGE_FINISH;
            break;

        default:
            aes_gcm->iv_change_flag = DRV_CIPHER_GCM_IV_CHANGE_MAX;
            break;
    }

    if (gcm != NULL) {
        aes_gcm->aad = (uint8_t *)gcm->aad_buf.phys_addr;
        aes_gcm->aad_len = gcm->aad_len;
        aes_gcm->data_len = gcm->data_len;
    }
}

static void convert_from_symc_param_aes_ccm(const uapi_drv_cipher_symc_ctrl_t *symc_ctrl,
    drv_cipher_config_param_t *param)
{
    drv_cipher_config_aes_ccm_t *aes_ccm = &(param->aes_ccm_param);
    uapi_drv_cipher_symc_config_aes_ccm_gcm_t *ccm = (uapi_drv_cipher_symc_config_aes_ccm_gcm_t *)symc_ctrl->param;

    aes_ccm->key_len = (drv_cipher_key_length_t)symc_ctrl->symc_key_length;
    aes_ccm->key_parity = (drv_cipher_key_parity_t)symc_ctrl->key_parity;
    (void)memcpy_s(aes_ccm->iv_ctr, sizeof(aes_ccm->iv_ctr), symc_ctrl->iv, symc_ctrl->iv_length);
    aes_ccm->iv_len = symc_ctrl->iv_length;

    switch (symc_ctrl->iv_change_flag) {
        case UAPI_DRV_CIPHER_SYMC_CCM_IV_DO_NOT_CHANGE:
            aes_ccm->iv_change_flag = DRV_CIPHER_CCM_IV_DO_NOT_CHANGE;
            break;

        case UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_START:
            aes_ccm->iv_change_flag = DRV_CIPHER_CCM_IV_CHANGE_START;
            break;

        case UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_UPDATE:
            aes_ccm->iv_change_flag = DRV_CIPHER_CCM_IV_CHANGE_UPDATE;
            break;

        case UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_FINISH:
            aes_ccm->iv_change_flag = DRV_CIPHER_CCM_IV_CHANGE_FINISH;
            break;

        default:
            aes_ccm->iv_change_flag = DRV_CIPHER_CCM_IV_CHANGE_MAX;
            break;
    }

    if (ccm != NULL) {
        aes_ccm->aad = (uint8_t *)ccm->aad_buf.phys_addr;
        aes_ccm->aad_len = ccm->aad_len;
        aes_ccm->total_len = ccm->data_len;
        aes_ccm->tag_len = (uint8_t)ccm->tag_len;
    }
}

static void convert_from_symc_param(const uapi_drv_cipher_symc_ctrl_t *symc_ctrl,
    drv_cipher_work_mode_t mode, drv_cipher_config_param_t *param)
{
    if (mode == DRV_CIPHER_WORK_MODE_GCM) {
        convert_from_symc_param_aes_gcm(symc_ctrl, param);
        return;
    }

    if (mode == DRV_CIPHER_WORK_MODE_CCM) {
        convert_from_symc_param_aes_ccm(symc_ctrl, param);
        return;
    }

    convert_from_symc_param_aes(symc_ctrl, param);
}

static void convert_to_symc_param_aes(const drv_cipher_config_param_t *param, uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    const drv_cipher_config_aes_t *aes_cfg = &(param->aes_param);
    symc_ctrl->symc_key_length = (uapi_drv_cipher_symc_key_length_t)aes_cfg->key_len;
    symc_ctrl->symc_bit_width = (uapi_drv_cipher_symc_bit_width_t)aes_cfg->bit_width;
    symc_ctrl->key_parity = (uapi_drv_cipher_symc_key_parity_t)aes_cfg->key_parity;

    (void)memcpy_s(symc_ctrl->iv, sizeof(symc_ctrl->iv), aes_cfg->iv, sizeof(aes_cfg->iv));
    symc_ctrl->iv_length = (uint32_t)sizeof(aes_cfg->iv);

    switch (aes_cfg->iv_change_flag) {
        case DRV_CIPHER_IV_DO_NOT_CHANGE:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_DO_NOT_CHANGE;
            break;

        case DRV_CIPHER_IV_CHANGE_ONE_PKG:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ONE_PKG;
            break;

        case DRV_CIPHER_IV_CHANGE_ALL_PKG:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ALL_PKG;
            break;

        default:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_MAX;
            break;
    }

    symc_ctrl->param = NULL;
}

static void convert_to_symc_param_aes_gcm(const drv_cipher_config_param_t *param,
    uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    const drv_cipher_config_aes_gcm_t *aes_gcm = &(param->aes_gcm_param);
    uapi_drv_cipher_symc_config_aes_ccm_gcm_t *gcm = (uapi_drv_cipher_symc_config_aes_ccm_gcm_t *)symc_ctrl->param;

    symc_ctrl->symc_key_length = (uapi_drv_cipher_symc_key_length_t)aes_gcm->key_len;
    symc_ctrl->key_parity = (uapi_drv_cipher_symc_key_parity_t)aes_gcm->key_parity;

    (void)memcpy_s(symc_ctrl->iv, sizeof(symc_ctrl->iv), aes_gcm->iv, aes_gcm->iv_len);
    symc_ctrl->iv_length = aes_gcm->iv_len;

    switch (aes_gcm->iv_change_flag) {
        case DRV_CIPHER_GCM_IV_DO_NOT_CHANGE:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_GCM_IV_DO_NOT_CHANGE;
            break;

        case DRV_CIPHER_GCM_IV_CHANGE_START:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_START;
            break;

        case DRV_CIPHER_GCM_IV_CHANGE_UPDATE:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_UPDATE;
            break;

        case DRV_CIPHER_GCM_IV_CHANGE_FINISH:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_FINISH;
            break;

        default:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_MAX;
            break;
    }

    if (gcm != NULL) {
        (void)memset_s(gcm, sizeof(uapi_drv_cipher_symc_config_aes_ccm_gcm_t),
            0, sizeof(uapi_drv_cipher_symc_config_aes_ccm_gcm_t));
        gcm->aad_buf.phys_addr = (uintptr_t)aes_gcm->aad;
        gcm->aad_len = aes_gcm->aad_len;
        gcm->data_len = aes_gcm->data_len;
    }
}

static void convert_to_symc_param_aes_ccm(const drv_cipher_config_param_t *param,
    uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    const drv_cipher_config_aes_ccm_t *aes_ccm = &(param->aes_ccm_param);
    uapi_drv_cipher_symc_config_aes_ccm_gcm_t *ccm = (uapi_drv_cipher_symc_config_aes_ccm_gcm_t *)symc_ctrl->param;

    symc_ctrl->symc_key_length = (uapi_drv_cipher_symc_key_length_t)aes_ccm->key_len;
    symc_ctrl->key_parity = (uapi_drv_cipher_symc_key_parity_t)aes_ccm->key_parity;

    (void)memcpy_s(symc_ctrl->iv, sizeof(symc_ctrl->iv), aes_ccm->iv_ctr, aes_ccm->iv_len);
    symc_ctrl->iv_length = aes_ccm->iv_len;

    switch (aes_ccm->iv_change_flag) {
        case DRV_CIPHER_CCM_IV_DO_NOT_CHANGE:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_CCM_IV_DO_NOT_CHANGE;
            break;

        case DRV_CIPHER_CCM_IV_CHANGE_START:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_START;
            break;

        case DRV_CIPHER_CCM_IV_CHANGE_UPDATE:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_UPDATE;
            break;

        case DRV_CIPHER_CCM_IV_CHANGE_FINISH:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_CCM_IV_CHANGE_FINISH;
            break;

        default:
            symc_ctrl->iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_MAX;
            break;
    }

    if (ccm != NULL) {
        (void)memset_s(ccm, sizeof(uapi_drv_cipher_symc_config_aes_ccm_gcm_t),
            0, sizeof(uapi_drv_cipher_symc_config_aes_ccm_gcm_t));
        ccm->aad_buf.phys_addr = (uintptr_t)aes_ccm->aad;
        ccm->aad_len = aes_ccm->aad_len;
        ccm->data_len = aes_ccm->total_len;
        ccm->tag_len = aes_ccm->tag_len;
    }
}

static void convert_to_symc_param(const drv_cipher_config_param_t *param, drv_cipher_work_mode_t mode,
    uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    if (mode == DRV_CIPHER_WORK_MODE_GCM) {
        convert_to_symc_param_aes_gcm(param, symc_ctrl);
        return;
    }

    if (mode == DRV_CIPHER_WORK_MODE_CCM) {
        convert_to_symc_param_aes_ccm(param, symc_ctrl);
        return;
    }

    convert_to_symc_param_aes(param, symc_ctrl);
}

errcode_t uapi_drv_cipher_symc_set_config(uint32_t symc_handle, const uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    if (symc_ctrl == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    drv_cipher_config_param_t param;
    drv_cipher_config_t ctrl;
    ctrl.alg = (symc_ctrl->symc_alg == UAPI_DRV_CIPHER_SYMC_ALG_AES) ? DRV_CIPHER_ALG_AES : DRV_CIPHER_ALG_MAX;
    ctrl.dfa = DRV_CIPHER_DFA_DISABLE;
    ctrl.work_mode = convert_from_symc_work_mode(symc_ctrl->work_mode);
    convert_from_symc_param(symc_ctrl, ctrl.work_mode, &param);
    ctrl.param = (void *)&param;

    return uapi_drv_cipher_set_config((const uintptr_t)symc_handle, &ctrl);
}

errcode_t uapi_drv_cipher_symc_get_config(uint32_t symc_handle, uapi_drv_cipher_symc_ctrl_t *symc_ctrl)
{
    if (symc_ctrl == NULL) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    drv_cipher_config_t ctrl;
    drv_cipher_config_param_t param;
    ctrl.param = (void *)&param;
    errcode_t ret = uapi_drv_cipher_get_config((const uintptr_t)symc_handle, &ctrl);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    symc_ctrl->symc_alg = (ctrl.alg == DRV_CIPHER_ALG_AES) ?
        UAPI_DRV_CIPHER_SYMC_ALG_AES : UAPI_DRV_CIPHER_SYMC_ALG_MAX;
    symc_ctrl->work_mode = convert_to_symc_work_mode(ctrl.work_mode);

    convert_to_symc_param((const drv_cipher_config_param_t *)ctrl.param, ctrl.work_mode, symc_ctrl);

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_cipher_symc_attach(uint32_t symc_handle, uint32_t keyslot_handle)
{
    return uapi_drv_cipher_attach((const uintptr_t)symc_handle, (const uintptr_t)keyslot_handle);
}

errcode_t uapi_drv_cipher_symc_detach(uint32_t symc_handle, uint32_t keyslot_handle)
{
    return uapi_drv_cipher_detach((const uintptr_t)symc_handle, (const uintptr_t)keyslot_handle);
}

static inline void convert_to_drv_cipher_buff(const uapi_drv_cipher_buf_attr_t *in, drv_cipher_buff_attr_t *out)
{
    out->address = (uint8_t *)in->phys_addr;
    out->buf_sec = (in->buf_sec == UAPI_DRV_CIPHER_BUF_SECURE) ? DRV_CIPHER_BUFFER_SECURE : DRV_CIPHER_BUFFER_NONSECURE;
}

errcode_t uapi_drv_cipher_symc_encrypt(uint32_t symc_handle, const uapi_drv_cipher_buf_attr_t *src_buf,
    const uapi_drv_cipher_buf_attr_t *dst_buf, uint32_t length)
{
    if ((src_buf == NULL) || (dst_buf == NULL)) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    drv_cipher_buff_attr_t new_src_buf;
    drv_cipher_buff_attr_t new_dst_buf;

    convert_to_drv_cipher_buff(src_buf, &new_src_buf);
    convert_to_drv_cipher_buff(dst_buf, &new_dst_buf);

    return uapi_drv_cipher_encrypt((const uintptr_t)symc_handle, &new_src_buf, &new_dst_buf, length, 0);
}

errcode_t uapi_drv_cipher_symc_decrypt(uint32_t symc_handle, const uapi_drv_cipher_buf_attr_t *src_buf,
    const uapi_drv_cipher_buf_attr_t *dst_buf, uint32_t length)
{
    if ((src_buf == NULL) || (dst_buf == NULL)) {
        return ERRCODE_CIPHER_INVALID_PARAMETER;
    }

    drv_cipher_buff_attr_t new_src_buf;
    drv_cipher_buff_attr_t new_dst_buf;

    convert_to_drv_cipher_buff(src_buf, &new_src_buf);
    convert_to_drv_cipher_buff(dst_buf, &new_dst_buf);

    return uapi_drv_cipher_decrypt((const uintptr_t)symc_handle, &new_src_buf, &new_dst_buf, length, 0);
}

errcode_t uapi_drv_cipher_symc_get_tag(uint32_t symc_handle, uint8_t *tag, uint32_t tag_length)
{
    uint32_t tmp_len = tag_length;
    return uapi_drv_cipher_get_tag((uintptr_t)symc_handle, tag, &tmp_len);
}
