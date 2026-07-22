/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: hifi3 sys command
 * Author: audio
 */

#include "sys_cmd.h"
#include "chip_type.h"
#include "mem_addr.h"
#include "audio_osal.h"
#include "audio_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SYS_CMD_WAIT_TIME (10 * CNT_100US_PS)

#define sys_cmd_read_reg_u32(val, reg) (val) = (reg)
#define sys_cmd_write_reg_u32(reg, val) (reg) = (val)

typedef union {
    /* define the struct bits */
    struct {
        td_u32 state : 2;
        td_u32 reserved_0 : 1;
        td_u32 sys_cmd_done : 1;
        td_u32 sys_cmd_ret : 4;
        td_u32 reserved_1 : 24;
    } bits;

    /* define an unsigned member */
    td_u32 u32;
} sys_cmd_state_reg;

typedef struct {
    sys_cmd_state_reg cmd_reg[DSP_CORE_NUM];
} sys_cmd_state;

static volatile sys_cmd_state *g_sys_state_reg = TD_NULL;

td_s32 sys_cmd_init(td_void)
{
#if (SAP_OS_TYPE != os_type_xtensa)
    volatile sys_cmd_state_reg val = {
        .u32 = 0,
    };
#endif

    if (g_sys_state_reg == TD_NULL) {
        g_sys_state_reg = (volatile sys_cmd_state *)audio_osal_address_map(
            SOC_IPC_SYS_STATE_ADDR, sizeof(sys_cmd_state));
        if (g_sys_state_reg == TD_NULL) {
            return AUDIO_FAILURE;
        }
    }

#if (SAP_OS_TYPE != os_type_xtensa)
    val.bits.state = AUDIO_SYS_DEEP_STANDBY;
    val.bits.sys_cmd_done = TD_TRUE;
    sys_cmd_write_reg_u32(g_sys_state_reg->cmd_reg[DSP_CORE0].u32, val.u32);
    sys_cmd_write_reg_u32(g_sys_state_reg->cmd_reg[DSP_CORE1].u32, val.u32);
#endif

    return AUDIO_SUCCESS;
}

td_void sys_cmd_deinit(td_void)
{
    if (g_sys_state_reg != TD_NULL) {
        audio_osal_address_unmap(SOC_IPC_SYS_STATE_ADDR, (td_void *)g_sys_state_reg);
        g_sys_state_reg = TD_NULL;
    }
}

static volatile sys_cmd_state_reg *sys_cmd_get_state_reg(audio_core_id core_id)
{
    if (core_id == AUDIO_CORE_DSP0) {
        return &g_sys_state_reg->cmd_reg[DSP_CORE0];
    } else {
        return &g_sys_state_reg->cmd_reg[DSP_CORE1];
    }
}

td_void sys_cmd_set_state(audio_core_id core_id, audio_sys_state state)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    val.bits.state = state;
    val.bits.sys_cmd_done = TD_FALSE;
    sys_cmd_write_reg_u32(reg->u32, val.u32);
}

td_void sys_cmd_get_state(audio_core_id core_id, audio_sys_state *state)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    *state = val.bits.state;
}

td_void sys_cmd_set_cmd_done(audio_core_id core_id, td_bool cmd_done)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    val.bits.sys_cmd_done = cmd_done;
    sys_cmd_write_reg_u32(reg->u32, val.u32);
}

td_void sys_cmd_get_cmd_done(audio_core_id core_id, td_bool *cmd_done)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    *cmd_done = val.bits.sys_cmd_done;
}

td_void sys_cmd_set_cmd_ret(audio_core_id core_id, td_u8 cmd_ret)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    val.bits.sys_cmd_ret = (cmd_ret & 0xF);
    sys_cmd_write_reg_u32(reg->u32, val.u32);
}

td_void sys_cmd_get_cmd_ret(audio_core_id core_id, td_u8 *cmd_ret)
{
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    *cmd_ret = val.bits.sys_cmd_ret;
}

td_s32 sys_cmd_check_cmd_done(audio_core_id core_id)
{
    td_u32 wait_cnt = 0;
    volatile sys_cmd_state_reg val;
    volatile sys_cmd_state_reg *reg = sys_cmd_get_state_reg(core_id);

    sys_cmd_read_reg_u32(val.u32, reg->u32);
    while ((wait_cnt++ < SYS_CMD_WAIT_TIME) && (val.bits.sys_cmd_done == 0)) {
        audio_osal_usleep(DELAY_100US);
        if ((wait_cnt % CNT_100US_PS) == 0) {
            sap_trace_log_u32(wait_cnt);
        }
        sys_cmd_read_reg_u32(val.u32, reg->u32);
    }

#if (SAP_CHIP_TYPE != socmn1)
    {
        td_u32 wait_time_us;
        wait_time_us = wait_cnt * DELAY_100US;
        sap_alert_log_u32(wait_time_us);
    }
#endif

    if (val.bits.sys_cmd_done == TD_TRUE) {
        return (td_s32)val.bits.sys_cmd_ret;
    }

    return AUDIO_FAILURE;
}

typedef union {
    /* define the struct bits */
    struct {
        td_u32 product_form : 8; /* see audio_product_form */
        td_u32 mono_mode    : 8; /* bool */
        td_u32 tws_mode     : 8; /* see audio_tws_mode */
        td_u32 tws_role     : 8; /* see audio_tws_role */
    } bits;

    /* define an unsigned member */
    td_u32 u32;
} sys_tws_config_reg;

static volatile sys_tws_config_reg *g_sys_tws_config_reg = TD_NULL;

td_void sys_audio_tws_config_reg_init(td_void)
{
    volatile sys_tws_config_reg val = {
        .u32 = 0,
    };

    if (g_sys_tws_config_reg == TD_NULL) {
        return;
    }

    val.bits.product_form = AUDIO_PRODUCT_FORM_NON_TWS;
    val.bits.mono_mode = TD_FALSE;
    val.bits.tws_mode = AUDIO_TWS_MODE_LISTEN;
    val.bits.tws_role = AUDIO_TWS_ROLE_FREEMAN;

    g_sys_tws_config_reg->u32 = val.u32;
}

td_void sys_audio_tws_config_deinit(td_void)
{
    if (g_sys_tws_config_reg == TD_NULL) {
        return;
    }

    audio_osal_address_unmap(SOC_TWS_CONFIG_ADDR, (td_void *)g_sys_tws_config_reg);
    g_sys_tws_config_reg = TD_NULL;
}

td_void sys_audio_tws_config_init(td_void)
{
    if (g_sys_tws_config_reg != TD_NULL) {
        return;
    }

    g_sys_tws_config_reg = (volatile sys_tws_config_reg *)audio_osal_address_map(
        SOC_TWS_CONFIG_ADDR, sizeof(*g_sys_tws_config_reg));
    if (g_sys_tws_config_reg == TD_NULL) {
        return;
    }

#if (SAP_OS_TYPE != os_type_xtensa)
    sys_audio_tws_config_reg_init();
#endif
}

td_s32 sys_audio_get_product_form(audio_product_form *product_form)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(product_form);
    sys_check_null_ptr_return(g_sys_tws_config_reg);

    val.u32 = g_sys_tws_config_reg->u32;
    *product_form = val.bits.product_form;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_set_product_form(audio_product_form product_form)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(g_sys_tws_config_reg);

    if (product_form >= AUDIO_PRODUCT_FORM_MAX) {
        sap_err_log_u32(product_form);
        return ERR_SYS_INVALID_PARAM;
    }

    val.u32 = g_sys_tws_config_reg->u32;
    val.bits.product_form = product_form;
    g_sys_tws_config_reg->u32 = val.u32;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_get_tws_mono_mode(td_bool *mono)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(mono);
    sys_check_null_ptr_return(g_sys_tws_config_reg);

    val.u32 = g_sys_tws_config_reg->u32;
    *mono = val.bits.mono_mode;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_set_tws_mono_mode(td_bool mono)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(g_sys_tws_config_reg);

    if (mono != TD_TRUE && mono != TD_FALSE) {
        return ERR_SYS_INVALID_PARAM;
    }

    val.u32 = g_sys_tws_config_reg->u32;
    val.bits.mono_mode = mono;
    g_sys_tws_config_reg->u32 = val.u32;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_get_tws_mode(audio_tws_mode *tws_mode)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(tws_mode);
    sys_check_null_ptr_return(g_sys_tws_config_reg);

    val.u32 = g_sys_tws_config_reg->u32;
    *tws_mode = val.bits.tws_mode;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_set_tws_mode(audio_tws_mode tws_mode)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(g_sys_tws_config_reg);

    if (tws_mode >= AUDIO_TWS_MODE_MAX) {
        sap_err_log_u32(tws_mode);
        return ERR_SYS_INVALID_PARAM;
    }

    val.u32 = g_sys_tws_config_reg->u32;
    val.bits.tws_mode = tws_mode;
    g_sys_tws_config_reg->u32 = val.u32;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_get_tws_role(audio_tws_role *tws_role)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(tws_role);
    sys_check_null_ptr_return(g_sys_tws_config_reg);

    val.u32 = g_sys_tws_config_reg->u32;
    *tws_role = val.bits.tws_role;

    return AUDIO_SUCCESS;
}

td_s32 sys_audio_set_tws_role(audio_tws_role tws_role)
{
    volatile sys_tws_config_reg val;

    sys_check_null_ptr_return(g_sys_tws_config_reg);

    if (tws_role >= AUDIO_TWS_ROLE_MAX) {
        sap_err_log_u32(tws_role);
        return ERR_SYS_INVALID_PARAM;
    }

    val.u32 = g_sys_tws_config_reg->u32;
    val.bits.tws_role = tws_role;
    g_sys_tws_config_reg->u32 = val.u32;

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
