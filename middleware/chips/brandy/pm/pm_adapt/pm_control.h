/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm control header
 */

#ifndef SRC_LIB_PM_CONTROL_H
#define SRC_LIB_PM_CONTROL_H

#include "chip_io.h"
#include "hal_pmu_ldo.h"
#include "errcode.h"

typedef void (*pm_control_proc_func)(switch_type_t);

typedef enum {
    // !< Power control
    PM_BT_POWER_UP_AND_RUN      = 0,   // !< Power up and run bt
    PM_BT_POWER_OFF             = 1,   // !< Power off bt.
    PM_DSP_POWER_UP_AND_RUN     = 2,   // !< Power up and run dsp
    PM_DSP_POWER_OFF            = 3,   // !< Power off off.
    PM_DISPLAY_POWER_UP_AND_RUN = 4,   // !< Power up and run display
    PM_DISPLAY_POWER_OFF        = 5,   // !< Power off off.
    PM_SDIO_POWER_UP_AND_RUN    = 6,   // !< Power up and run sdio
    PM_SDIO_POWER_OFF           = 7,   // !< Power off off.
    PM_CONTROL_MAX,
} pm_control_t;

/**
 * @brief  L2ram dsp power ctrl.
 */
typedef enum {
    U0_1_L2M_DSP_RAM,
    U2_3_L2M_DSP_RAM,
    U4_5_L2M_DSP_RAM,
    U6_7_L2M_DSP_RAM,
    U8_9_L2M_DSP_RAM,
    U10_11_L2M_DSP_RAM,
    U12_13_L2M_DSP_RAM,
    U14_15_L2M_DSP_RAM,
    U16_17_L2M_DSP_RAM,
    U18_19_L2M_DSP_RAM,
} l2m_dsp_power_on_t;

void uapi_pm_control(pm_control_t type);

errcode_t uapi_pm_dsp_l2ram_power_on(l2m_dsp_power_on_t start, uint8_t num, switch_type_t power_on);

errcode_t uapi_pm_register_cldo1_at_lp_handler(pm_control_proc_func cldo1_func);

void uapi_pm_unregister_cldo1_at_lp_handler(void);

void uapi_pm_run_cldo1_at_lp_handler(switch_type_t power_on);

#endif