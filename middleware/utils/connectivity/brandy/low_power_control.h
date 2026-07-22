/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: low power control interface header
 * Author:
 * Create:
 */
#ifndef LIB_PUBLIC_LOW_POWER_CONTROL_H
#define LIB_PUBLIC_LOW_POWER_CONTROL_H

#include "connectivity_low_power.h"
#include "core.h"
#if (CORE == MASTER_BY_ALL)
#include "hal_pmu_ldo.h"
#endif

/**
 * @addtogroup connectivity_libs_connectivity_low_power
 * @{
 */
typedef void (*btc_wakeup_handler) (void);
typedef void (*btc_thread_back_handler) (void);

typedef enum {
    CODEC_UNUSED   = 0,
    CODEC_POWEROFF = 1,
    CODEC_POWERON  = 2,
} codec_state_e;

// used for smart wear run mode
typedef enum {
    PRODUCT_AW_MODE = 0,
    PRODUCT_GT_MODE,
    PRODUCT_INVALID_MODE,
} product_mode_e;

// used for smart wear diff board type
typedef enum {
    BOARD_TYPE_BEFORE_V3 = 0,
    BOARD_TYPE_V3,
    BOARD_TYPE_INVALID,
} board_type_e;

#if CORE == APPS
typedef struct {
    switch_type_t enable;
    switch_type_t bypass_singal;
} dmic_config_t;

/**
 * @brief  low power control interface
 * @param  type        operation types.
 * @return TURE        success
 */
bool low_power_control(low_power_control_e type);

/**
 * @brief M3 low power control interface.
 * @param type operation types.
 */
void low_power_m3_power_control_private(low_power_control_e type);

#if (BTH_WITH_SMART_WEAR == YES)
/**
  * @brief  low power set gpio before run m3
  * @return NULL
  */
void low_power_set_coex_gpio_power_on(void);

/**
  * @brief  low power control for bt and wifi coex
  * @param  type        operation types power on or power off.
  * @return NULL
  */
bool low_power_control_coex(low_power_control_e type);

/**
  * @brief  Get board type(V3 or befor V3) and set share mem
  * @return NULL
  */
void set_board_type(void);
#endif

/**
  * @brief  download hifi image
  * @param  type   operation types
  * @return NULL
  */
void low_power_hifi_download(low_power_control_e type);
#endif

#if (BTH_WITH_SMART_WEAR == YES)
/**
  * @brief  Get product AW&GT mode
  * @return aw or gt mode
  */
product_mode_e get_aw_gt_mode(void);

/**
  * @brief  Get board type v3 or before v3
  * @return boart type
  */
board_type_e get_board_type(void);
#endif


#if CORE == BT
/**
 * @brief  Control DSP power on.
 */
void low_power_dsp_power_on(void);

/**
 * @brief  Control DSP power on.
 * @param  btc_handler When 1p3 power on, will call back is func.
 */
void low_power_wakeup_btc_register(btc_wakeup_handler btc_handler);

/**
 * @brief  Return to btc thread after codec poweroff
 * @param  btc_thread_handler When 1p3 power off, will call back is func.
 */
void low_power_btc_thread_back_register(btc_thread_back_handler btc_thread_handler);

/**
 * @brief  Return codec is power on or off.
 * @return true is power on and false is power off.
 */
bool low_power_codec_is_poweron(void);
#endif

#if (CORE == MASTER_BY_ALL)
/**
 * @brief  Turn on/off dmic power
 * @param  enable TURN_ON/TURN_OFF
 */
void low_power_dmic_config(switch_type_t enable);

/**
 * @brief  Get dmic power status
 * @return dmic power(enable:1 disble:0) & bypass(bypass:1 not bypass:0)
 */
dmic_config_t low_power_get_dmic_config(void);

/**
 * @brief  Turn on/off ioldo
 * @param  enable TURN_ON/TURN_OFF
 */
void low_power_ioldo_config(switch_type_t enable);

/**
 * @brief  Get ioldo status
 * @param  control_id Which PMU moudle
 * @return ioldo on:1 ioldo off:0
 */
switch_type_t low_power_get_ioldo_config(hal_pmu_man_sel_t control_id);

/**
 * @brief  Turn on/off load switch
 * @param  load_switch Load switch types.
 * @param  enable TURN_ON/TURN_OFF
 */
void low_power_load_switch_config(hal_load_switch_type_t load_switch, switch_type_t enable);


/**
 * @brief  Turn on/off mipi power
 * @param  enable TURN_ON/TURN_OFF
 */
void low_power_mipi_config(switch_type_t enable);
#endif
/**
 * @}
 */
#endif
