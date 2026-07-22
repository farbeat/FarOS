/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core OS Initialize Interface Header for Standard
 */

#ifndef THREAD_INIT_H
#define THREAD_INIT_H
#include "cmsis_os2.h"

/**
 * @defgroup MAIN_ENTRY CHIP Main Entry
 * @{
 */
typedef void (*CreateQueueFunction_t)(void);

typedef enum {
    USE_DTCM_MEM,
    USE_EXTEND_MEM,
} use_mem_type;

typedef struct {
    osThreadAttr_t attr;
    osThreadFunc_t func;
    uint32_t *task_handle;
    CreateQueueFunction_t create_queue_handle;
    uint32_t use_mem;
} app_task_definition_t;

/**
 * @brief  Thread task creat.
 */
void thread_init(void);

/**
 * @brief  dsp power on init
 */
void low_power_control1_power_on_asub(void);

/**
 * @brief  bt power on init
 */
void low_power_on_bt_control1(void);

/**
 * @brief  display power on init
 */
void low_power_control1_power_on_display(void);

/**
 * @brief  set app delay time
 * @param  time the delay time
 */
void set_app_delay_time(uint32_t time);

extern uint32_t g_app_delay_ms;

/**
 * @}
 */

#endif
