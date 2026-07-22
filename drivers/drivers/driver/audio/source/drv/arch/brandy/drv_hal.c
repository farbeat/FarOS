/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: brandy drv hal
 * Author: audio
 */

#include "securec.h"
#include "soc_osal.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "sys_struct.h"
#include "reg_addr.h"
#include "common_dsp.h"
#include "core_msg.h"
#include "pm_clocks.h"
#include "pm_definition.h"
#include "pm.h"
#include "drv_msg_sys.h"
#include "drv_hal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MSG_CHECK_ACK_PER_MS 1
#define MSG_CHECK_ACK_TIMEOUT_MS 10

#define HAL_DSP_CORE_NUM 1
#define HAL_SOC_PLL_HZ 884736000
#define HAL_INT_X2A_AUDIO (16 + 3)

#define REG_X2M_SOFT_INT_EN (SOC_M_CTL_RB + 0x020)
#define REG_X2M_SOFT_INT_STS (SOC_M_CTL_RB + 0x028)
#define REG_X2M_SOFT_INT_CLR (SOC_M_CTL_RB + 0x02C)

#define REG_DSP_DIV_NUM (SOC_AUD_DSP_CFG + 0x000)
#define REG_DSP_CLK_EN0 (SOC_AUD_DSP_CFG + 0x004)
#define REG_DSP_RST_EN0 (SOC_AUD_DSP_CFG + 0x008)
#define REG_AUDIO_CLK_CFG (SOC_AUD_DSP_CFG + 0x018)
#define REG_DSP_RUNSTALL (SOC_AUD_DSP_CFG + 0x028)

#define dsp_check_core_id_valid(core_id) \
    (((core_id) >= AUDIO_CORE_DSP0) && ((core_id) < (AUDIO_CORE_DSP0 + HAL_DSP_CORE_NUM)))

typedef enum {
    DSP_OCR_NORM_MODE = 0, /* Normal mode */
    DSP_OCR_RTT_MODE, /* Retention mode */
    DSP_OCR_MODE_MAX,
} dsp_ocr_mode; /* On-Chip RAM work mode */

td_void hal_dsp_get_int_cfg(audio_core_id core_id, dsp_int_cfg *int_cfg)
{
    if (int_cfg != TD_NULL) {
        int_cfg->irq_no = HAL_INT_X2A_AUDIO;
        int_cfg->reg_int = REG_X2M_SOFT_INT_STS;
        int_cfg->reg_int_clr = REG_X2M_SOFT_INT_CLR;
        int_cfg->reg_int_mask = REG_X2M_SOFT_INT_EN;
        int_cfg->int_msg_bit = 0x00000008; /* REG_X2M_SOFT_INT_EN msg bit */
        int_cfg->int_vad_bit = 0x00000000;
        int_cfg->int_clr_bit = 0x00000008; /* REG_X2M_SOFT_INT_CLR msg bit */
    }
    audio_unused(core_id);
}

td_u32 hal_dsp_get_core_num(td_void)
{
    return HAL_DSP_CORE_NUM;
}

td_s32 hal_dsp_init(audio_core_id core_id)
{
    if (!dsp_check_core_id_valid(core_id)) {
        return ERR_SYS_NOT_SUPPORTED;
    }

    return AUDIO_SUCCESS;
}

td_s32 hal_dsp_deinit(audio_core_id core_id)
{
    if (!dsp_check_core_id_valid(core_id)) {
        return ERR_SYS_NOT_SUPPORTED;
    }

    return AUDIO_SUCCESS;
}

static td_void init_clock_attr(audio_clock_attr *clock_attr, const pm_hifi_freq_t *hifi_freq)
{
    switch (hifi_freq->dsp_src) {
        case CLOCKS_SRC_TCXO_2X:
            clock_attr->clock_type = AUDIO_CLOCK_XO;
            break;

        case CLOCKS_SRC_PLL:
            clock_attr->clock_type = AUDIO_CLOCK_PLL;
            break;

        default:
            clock_attr->clock_type = AUDIO_CLOCK_MAX;
            break;
    }

    clock_attr->clock_freq = (td_u32)hifi_freq->dsp_freq_val;
    sap_trace_log_u32(clock_attr->clock_freq);
}

td_void hal_dsp_set_clock_attr(td_void)
{
    td_s32 ret;
    audio_clock_attr clock_attr;
    pm_hifi_mode_t hifi_mode = uapi_pm_get_hifi_mode();
    pm_hifi_freq_t hifi_freq = uapi_pm_get_hifi_freq(hifi_mode);

    init_clock_attr(&clock_attr, &hifi_freq);

    ret = drv_msg_sys_set_clock_attr(0, &clock_attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_sys_set_clock_attr, ret);
    }
}

td_void hal_dsp_set_power_on(audio_core_id core_id, td_bool enable)
{
    if (enable) {
        uapi_pm_control(PM_DSP_POWER_UP_AND_RUN);
    } else {
        uapi_pm_control(PM_DSP_POWER_OFF);
    }

    audio_unused(core_id);
}

td_s32 hal_dsp_boot_prepare(audio_core_id core_id)
{
    if (!dsp_check_core_id_valid(core_id)) {
        return ERR_SYS_NOT_SUPPORTED;
    }

    uapi_register_hifi_inform_callback(hal_dsp_set_clock_attr);

    /* DSP inner module clock enable && dereset */
    audio_reg_write(REG_DSP_DIV_NUM, 0x00000100); /* dsp_div_num=0,bus_div_num=1 DSP_BUS_CLK=1/2 DSP_CLK */
    audio_reg_write(REG_DSP_CLK_EN0, 0x07E0011D); /* AUDIO inner module clock enable */
    audio_reg_write(REG_DSP_RST_EN0, 0x0000011D); /* AUDIO inner module dereset */
    audio_reg_write(REG_AUDIO_CLK_CFG, 0x00000003); /* Set codec clock gating */

    return AUDIO_SUCCESS;
}

td_s32 hal_dsp_boot_up(audio_core_id core_id)
{
    if (!dsp_check_core_id_valid(core_id)) {
        return ERR_SYS_NOT_SUPPORTED;
    }

    /* dereset DSP DEBUG && set DSP in work mode, hifi_ocdhaltonreset = 1’b0;
     * addr: 0xE800_7028[31:0]
     * R_DSP_RUNSTALL[2]:hifi0_ocdhaltonreset;
     *      Ocdhaltonreset=1, set dsp into OCDHaltMode;
     *      Ocdhaltonreset=0, set dsp into work mode;
     */
    audio_reg_bit_wr(REG_DSP_RUNSTALL, AUDIO_BIT2, AUDIO_BIT2, 0x0); /* set dsp into work mode */

    /* Pull down HIFI RUNSTALL signal, let HIFI boot from TCM;
     *  addr: 0xE800_7028[31:0]
     *
     *  [1] hifi_statvectorsel dsp boot addr select:
     *      dsp_statvectorsel=0, dsp boot from ITCM;
     *      dsp_statvectorsel=1, dsp boot from DDR;
     *  [0] hifi_runstall
     *      runstall=1, pause dsp's pipeline to decrease power comsumption;
     *      runstall=0, resume dsp's pipeline.
     */
    audio_reg_bit_wr(REG_DSP_RUNSTALL, AUDIO_BIT0, AUDIO_BIT1, 0x0); /* resume dsp's pipeline */

    return AUDIO_SUCCESS;
}

td_s32 hal_dsp_reset(audio_core_id core_id)
{
    if (!dsp_check_core_id_valid(core_id)) {
        return ERR_SYS_NOT_SUPPORTED;
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
