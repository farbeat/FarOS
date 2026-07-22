/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides button port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */
#ifndef BUTTON_PORTING_H
#define BUTTON_PORTING_H

#include <stdint.h>
#include "button.h"
#include "soc_osal.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_button BUTTON
 * @ingroup  drivers_port
 * @{
 */

#define ULP_WKUP_INT_STS    0x5702C1E0
#define ULP_WKUP_EVT_STS    0x5702C1E8
#define ULP_WKUP_INT_CLR    0x5702C1E4
#define ULP_WKUP_EVT_CLR    0x5702C1EC
#define ULP_WKUP_EVT_EN     0x5702C1F0
#define ULP_WKUP_INT_EN     0x5702C1F8
#define ANA_STS             0x5702C034
#define PWRON_PD_MODE_REG   0x5702C02C
#define ULP_SYSTICK_VAL0    0x5702c514
#define ULP_SYSTICK_VAL1    0x5702c518
#define ULP_SYSTICK_VAL2    0x5702c51C
#define ULP_SYSTICK_VAL3    0x5702c520
#define ULP_SYSTICK_SET0    0x5702c524
#define ULP_SYSTICK_SET1    0x5702c528
#define ULP_SYSTICK_SET2    0x5702c52C
#define ULP_SYSTICK_SET3    0x5702c530
#define CLK_32K_32M_EN_SEL_REG  0x57000068
#define PMU_RC_512K_XO_SEL_MASK 3
#define RCXO_32K_SEL_MASK       12
#define WKUP_SYSTICK_MASK       0x2
#define WKUP_PWRON_MASK         0x3
#define WKUP_VBUS_EN_MASK       0x4
#define ULP_WKUP_INT_EN_MASK    0x3C
#define ULP_WKUP_INT_CLR_MASK   0x3C
#define ULP_INT_CLEAR_DELAY     100
#define PWRON_PD_MODE_POS4      0x4
#define PWRON_TM_MODE_POS0      0x0
#define PWRON_TM_MODE_POS1      0x1
#define CALIBRATION_CYCLE_MUL   0x200
#define CALIBRATION_CLOCK_MUL   100
#define VBUS_INSERT_INTERRUPT_POS 0x4
#define VBUS_UNPLUG_INNTERRUPT_POS 0x5
#define MS_PER_S 1000

typedef enum {
    PDMODE_RE_UP,
    PDMODE_DOWN,
    PDMODE_MAX
} power_mode_t;

typedef enum {
    PDTIME_LEVEL0,
    PDTIME_LEVEL1,
    PDTIME_LEVEL2,
    PDTIME_LEVEL3,
    PDTIME_LEVELMAX
} power_time_t;

typedef enum {
    STARTUP_SYSTICK,
    STARTUP_PWRON,
    STARTUP_VBUS_EN,

    STARTUP_OTHER
} startup_reason_t;

button_peripheral_api *button_port_get_api(void);
errcode_t uapi_power_set_mode(power_mode_t pdmode);
errcode_t uapi_power_time_mode(power_time_t time_mode);
void uapi_ulp_wkup_type_record(void);
startup_reason_t uapi_get_startup_reason(void);
errcode_t uapi_enable_alarm(uint64_t timeout_ms);
errcode_t uapi_disable_alarm(void);
uint64_t uapi_ulp_systick_get_count(void);
uint64_t uapi_ulp_systick_get_ms(void);

/**
 * @brief  RC32K clock calibration. Do not call this API when the clock is in use.
 */
uint32_t uapi_ulp_systick_soc_calibration(void);

uint32_t uapi_ulp_systick_get_frequency(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif