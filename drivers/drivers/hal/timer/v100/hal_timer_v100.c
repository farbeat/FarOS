/**
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * Description: Provides V100 HAL timer \n
 *
 * History: \n
 * 2022-11-08, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "hal_timer_v100_regs_op.h"
#include "hal_drv_timer.h"
#include "timer_porting.h"
#include "hal_timer_v100.h"

static hal_timer_callback_t g_hal_timer_callback[TIMER_MAX_NUM] = {NULL};

#pragma weak hal_timer_init = hal_timer_v100_init
errcode_t hal_timer_v100_init(timer_index_t index, hal_timer_callback_t callback)
{
    if ((hal_timer_regs_init(index)) != ERRCODE_SUCC) {
        return ERRCODE_TIMER_INVALID_REG_ADDR;
    }
    g_hal_timer_callback[index] = callback;
    return ERRCODE_SUCC;
}

#pragma weak hal_timer_deinit = hal_timer_v100_deinit
void hal_timer_v100_deinit(timer_index_t index)
{
    g_hal_timer_callback[index] = NULL;
    hal_timer_regs_deinit(index);
}

#pragma weak hal_timer_start = hal_timer_v100_start
void hal_timer_v100_start(timer_index_t index)
{
    hal_timer_control_reg_set_enable(index, 1);
}

#pragma weak hal_timer_stop = hal_timer_v100_stop
void hal_timer_v100_stop(timer_index_t index)
{
    hal_timer_control_reg_set_enable(index, 0);
}

#pragma weak hal_timer_config_load = hal_timer_v100_config_load
void hal_timer_v100_config_load(timer_index_t index, uint64_t delay_count, uint32_t mode)
{
    hal_timer_control_reg_set_mode(index, mode);
    hal_timer_load_count_set(index, (uint32_t)delay_count);
}

#pragma weak hal_timer_get_current_value = hal_timer_v100_get_current_value
uint64_t hal_timer_v100_get_current_value(timer_index_t index, bool *is_enable)
{
    /* 判断enable不为1时认为时钟已到期，返回0。 */
    if (hal_timer_control_reg_get_enable(index) != 1) {
        *is_enable = false;
        return 0;
    }

    return (uint64_t)hal_timer_get_current_load(index);
}

void hal_timer_v100_irq_handler(timer_index_t index)
{
    /* Read the register to clear the interrupt */
    uint32_t int_clear = ((timer_regs_info_t *)g_timer_regs[index])->eoi;
    unused(int_clear);

    if (g_hal_timer_callback[index]) {
        g_hal_timer_callback[index](index);
    }
}