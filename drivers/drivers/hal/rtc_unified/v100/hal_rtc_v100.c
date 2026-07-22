/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * Description: Provides V100 HAL rtc \n
 *
 * History: \n
 * 2023-03-02, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "hal_rtc_v100_regs_op.h"
#include "hal_rtc.h"
#include "rtc_porting.h"
#include "hal_rtc_v100.h"

#if defined(CONFIG_RTC_START_DELAY_SUPPORT)
#if defined(CONFIG_RTC_START_DELAY_USING_OSAL)
#include "soc_osal.h"
#endif
#if defined(CONFIG_RTC_START_DELAY_USING_TCXO)
#include "tcxo.h"
#endif
#if defined(CONFIG_RTC_START_DELAY_USING_SYSTICK)
#include "systick.h"
#endif
#endif

#if defined(CONFIG_RTC_START_DELAY_SUPPORT)
#define RTC_START_DELAY_US 65
#endif

static hal_rtc_callback_t g_hal_rtc_callback[RTC_MAX_NUM] = {NULL};
static uint32_t g_rtc_int_cnt_record = 0;

#pragma weak hal_rtc_init = hal_rtc_v100_init
errcode_t hal_rtc_v100_init(rtc_index_t index, hal_rtc_callback_t callback)
{
    if ((hal_rtc_regs_init(index)) != ERRCODE_SUCC) {
        return ERRCODE_RTC_REG_ADDR_INVALID;
    }
    g_hal_rtc_callback[index] = callback;
    return ERRCODE_SUCC;
}

#pragma weak hal_rtc_deinit = hal_rtc_v100_deinit
void hal_rtc_v100_deinit(rtc_index_t index)
{
    g_hal_rtc_callback[index] = NULL;
    hal_rtc_regs_deinit(index);
}

#pragma weak hal_rtc_start = hal_rtc_v100_start
void hal_rtc_v100_start(rtc_index_t index)
{
    hal_rtc_control_reg_set_enable(index, 1);
#if defined(CONFIG_RTC_START_DELAY_SUPPORT)
    /* RTC_V100要求开始后的start信号同步到32Khz，经历了65us的时间 */
#if defined(CONFIG_RTC_START_DELAY_USING_OSAL)
    osal_udelay(RTC_START_DELAY_US);
#endif
#if defined(CONFIG_RTC_START_DELAY_USING_TCXO)
    uapi_tcxo_delay_us(RTC_START_DELAY_US);
#endif
#if defined(CONFIG_RTC_START_DELAY_USING_SYSTICK)
    uapi_systick_delay_us(RTC_START_DELAY_US);
#endif
#endif
}

#pragma weak hal_rtc_stop = hal_rtc_v100_stop
void hal_rtc_v100_stop(rtc_index_t index)
{
    hal_rtc_get_eoi(index);
    hal_rtc_control_reg_set_enable(index, 0);
}

#pragma weak hal_rtc_config_load = hal_rtc_v100_config_load
void hal_rtc_v100_config_load(rtc_index_t index, uint64_t delay_count)
{
    hal_rtc_control_reg_set_mode(index, (uint32_t)RTC_MODE_FREE_RUN);
    hal_rtc_load_count_set(index, (uint32_t)delay_count);
}

#pragma weak hal_rtc_get_current_value = hal_rtc_v100_get_current_value
uint64_t hal_rtc_v100_get_current_value(rtc_index_t index, bool *is_enable)
{
    /* 判断enable不为1时认为时钟已到期，返回0。 */
    if (hal_rtc_control_reg_get_enable(index) != 1) {
        *is_enable = false;
        return 0;
    }

    return (uint64_t)hal_rtc_get_current_load(index);
}

#if defined(CONFIG_RTC_SUPPORT_LPM)
#pragma weak hal_rtc_get_int_status = hal_rtc_v100_get_int_status
uint32_t hal_rtc_v100_get_int_status(rtc_index_t index)
{
    return hal_rtc_v100_get_int_sts(index);
}
#endif /* CONFIG_RTC_SUPPORT_LPM */

void hal_rtc_v100_irq_handler(rtc_index_t index)
{
    g_rtc_int_cnt_record++;

    /* Read the register to clear the interrupt */
    uint32_t int_clear = hal_rtc_get_eoi(index);
    unused(int_clear);
    if (g_hal_rtc_callback[index]) {
        g_hal_rtc_callback[index](index);
    }
}

#pragma weak hal_rtc_get_int_cnt_record = hal_rtc_v100_get_int_cnt_record
uint32_t hal_rtc_v100_get_int_cnt_record(void)
{
    uint32_t rtc_int_cnt = g_rtc_int_cnt_record;
    return rtc_int_cnt;
}