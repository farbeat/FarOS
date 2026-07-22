/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  HAL PMU DRIVER HEADER FILE
 */

#ifndef SRC_DRIVERS_BRANDY_HAL_PMU_LDO_H
#define SRC_DRIVERS_BRANDY_HAL_PMU_LDO_H

#include "chip_definitions.h"
#include "chip_io.h"

/** @defgroup connectivity_drivers_hal_pmu_ldo PMU LDO
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */
/**
 * @brief  Enum defining the pmu system status.
 */
typedef enum {
    HAL_PMU_SYSTEM_OFF = 0,             //!< The whole system is power off.
    HAL_PMU_SYSTEM_BOOTING = 1,         //!< The whole system is booting.
    HAL_PMU_SYSTEM_SLEEPING = 2,        //!< The whole system is sleeping.
    HAL_PMU_SYSTEM_WORK = 3,            //!< The whole system is work.
    HAL_PMU_SYSTEM_PROTECT = 4,         //!< The whole system is Power-off protection.
    HAL_PMU_SYSTEM_PROTECTING = 5,      //!< The whole system is Power-off protecting.
    HAL_PMU_SYSTEM_SLEEP = 6,           //!< The whole system is sleep.
    HAL_PMU_SYSTEM_WAKING = 7,          //!< The whole system is waking.
} hal_pmu_system_current_status_type_t;

typedef enum {
    HAL_PMU_WKUP_EN_UVLO,
    HAL_PMU_WKUP_EN_OVP,
    HAL_PMU_WKUP_EN_REF_BG,
    HAL_PMU_WKUP_EN_REFBUFFER,
    HAL_PMU_WKUP_EN_INTLDO2,
    HAL_PMU_WKUP_EN_IBG2,
    HAL_PMU_WKUP_EN_IPOLY,
    HAL_PMU_WKUP_EN_ITUNE,
    HAL_PMU_WKUP_EN_XLDO,
    HAL_PMU_WKUP_XO_CORE_PD,
    HAL_PMU_WKUP_RC_PD,
    HAL_PMU_WKUP_RC2_DBB_PD,
    HAL_PMU_WKUP_EN_CLDO1,
    HAL_PMU_WKUP_EN_CLDO1_ISO,
    HAL_PMU_WKUP_EN_CLDO3,
    HAL_PMU_WKUP_RST_AON_CRG,
    HAL_PMU_WKUP_GLB_CLK_FORCE_ON,
    HAL_PMU_WKUP_RST_AON_LGC,
    HAL_PMU_WKUP_RST_PWR_C1_CRG_N,
    HAL_PMU_WKUP_RST_PWR_C1_LGC_N,
    HAL_PMU_WKUP_RST_PWR_C1_CPU_N,
    HAL_PMU_WKUP_TCXO2ANA_PD,
    HAL_PMU_WKUP_TIME_MAX,
} hal_pmu_system_wakeup_time_type_t;

typedef enum {
    HAL_PMU_LOAD_SWITCH1,           //!< Load switch1 module
    HAL_PMU_LOAD_SWITCH2,           //!< Load switch2 module
    HAL_PMU_LOAD_SWITCH3,           //!< Load switch3 module
    HAL_PMU_LOAD_SWITCH4,           //!< Load switch4 module
} hal_load_switch_type_t;

typedef enum {
    EN_XLDO,
    EN_XLDO_DELAY,
    EN_INTLDO2,
    EN_INTLDO2_DELAY,
    EN_INTLDO1,
    EN_INTLDO1_DELAY,
    EN_REF_BG,
    EN_IBG2,
    EN_IBG1,
    EN_OVP,
    EN_IPOLY,
    EN_ITUNE,
    EN_UVLO,
    EN_REFBUFFER,
    EN_CMU_LDO,
    EN_CMU_LDO_DELAY,
    IOLDO_EN,
    IOLDO_EN_DELAY,
} hal_pmu_man_sel_t;

typedef enum {
    EN_CLDO1,
    EN_CLDO1_DELAY,
    LDO_C1_TO_SYS_ISO_EN,
    LDO_C1_TO_C2_ISO_EN,
    EN_CLDO2,
    EN_CLDO2_DELAY,
    LDO_C2_TO_SYS_ISO_EN_RB,
    LDO_C2_TO_C1_ISO_EN_RB,
    EN_CLDO3,
    EN_CLDO3_DELAY,
} hal_ldo_man_sel_t;

typedef enum {
    HAL_PMU_AUXLDO0,
    HAL_PMU_AUXLDO1,
    HAL_PMU_AUXLDO2,
    HAL_PMU_AUXLDO3,
} hal_auxldo_id_t;

/**
 * @brief  Defines the different CLDO1\CLDO2\CLDO3 voltage regulator levels
 */
typedef enum {
    HAL_PMU_CLDO_LEVEL_0P400V = 0,   //!< Set internal CLDO regulator to 0.400V
    HAL_PMU_CLDO_LEVEL_0P425V = 1,   //!< Set internal CLDO regulator to 0.425V
    HAL_PMU_CLDO_LEVEL_0P450V = 2,   //!< Set internal CLDO regulator to 0.450V
    HAL_PMU_CLDO_LEVEL_0P475V = 3,   //!< Set internal CLDO regulator to 0.475V
    HAL_PMU_CLDO_LEVEL_0P500V = 4,   //!< Set internal CLDO regulator to 0.500V
    HAL_PMU_CLDO_LEVEL_0P525V = 5,   //!< Set internal CLDO regulator to 0.525V
    HAL_PMU_CLDO_LEVEL_0P550V = 6,   //!< Set internal CLDO regulator to 0.550V
    HAL_PMU_CLDO_LEVEL_0P575V = 7,   //!< Set internal CLDO regulator to 0.575V
    HAL_PMU_CLDO_LEVEL_0P600V = 8,   //!< Set internal CLDO regulator to 0.600V
    HAL_PMU_CLDO_LEVEL_0P625V = 9,   //!< Set internal CLDO regulator to 0.625V
    HAL_PMU_CLDO_LEVEL_0P650V = 10,  //!< Set internal CLDO regulator to 0.650V
    HAL_PMU_CLDO_LEVEL_0P675V = 11,  //!< Set internal CLDO regulator to 0.675V
    HAL_PMU_CLDO_LEVEL_0P700V = 12,  //!< Set internal CLDO regulator to 0.700V
    HAL_PMU_CLDO_LEVEL_0P750V = 13,  //!< Set internal CLDO regulator to 0.750V
    HAL_PMU_CLDO_LEVEL_0P800V = 14,  //!< Set internal CLDO regulator to 0.800V
    HAL_PMU_CLDO_LEVEL_0P850V = 15,  //!< Set internal CLDO regulator to 0.850V
} hal_pmu_cldo_level_t;

/**
 * @brief  Defines the different AUXLDO voltage regulator levels
 */
typedef enum {
    HAL_PMU_AUXLDO_LEVEL_1P600V = 0,   //!< Set internal AUXLDO regulator to 1.600V
    HAL_PMU_AUXLDO_LEVEL_1P625V = 1,   //!< Set internal AUXLDO regulator to 1.625V
    HAL_PMU_AUXLDO_LEVEL_1P650V = 2,   //!< Set internal AUXLDO regulator to 1.650V
    HAL_PMU_AUXLDO_LEVEL_1P675V = 3,   //!< Set internal AUXLDO regulator to 1.675V
    HAL_PMU_AUXLDO_LEVEL_1P700V = 4,   //!< Set internal AUXLDO regulator to 1.700V
    HAL_PMU_AUXLDO_LEVEL_1P725V = 5,   //!< Set internal AUXLDO regulator to 1.725V
    HAL_PMU_AUXLDO_LEVEL_1P750V = 6,   //!< Set internal AUXLDO regulator to 1.750V
    HAL_PMU_AUXLDO_LEVEL_1P775V = 7,   //!< Set internal AUXLDO regulator to 1.775V
    HAL_PMU_AUXLDO_LEVEL_1P800V = 8,   //!< Set internal AUXLDO regulator to 1.800V
    HAL_PMU_AUXLDO_LEVEL_1P825V = 9,   //!< Set internal AUXLDO regulator to 1.825V
    HAL_PMU_AUXLDO_LEVEL_1P850V = 10,  //!< Set internal AUXLDO regulator to 1.850V
    HAL_PMU_AUXLDO_LEVEL_1P875V = 11,  //!< Set internal AUXLDO regulator to 1.875V
    HAL_PMU_AUXLDO_LEVEL_1P900V = 12,  //!< Set internal AUXLDO regulator to 1.900V
    HAL_PMU_AUXLDO_LEVEL_1P925V = 13,  //!< Set internal AUXLDO regulator to 1.925V
    HAL_PMU_AUXLDO_LEVEL_1P950V = 14,  //!< Set internal AUXLDO regulator to 1.950V
    HAL_PMU_AUXLDO_LEVEL_1P975V = 15,  //!< Set internal AUXLDO regulator to 1.975V
} hal_pmu_auxldo_level_t;

typedef enum {
    HAL_PMU_IOLDO_LEVEL_1P6V,
    HAL_PMU_IOLDO_LEVEL_1P625V,
    HAL_PMU_IOLDO_LEVEL_1P65V,
    HAL_PMU_IOLDO_LEVEL_1P675V,
    HAL_PMU_IOLDO_LEVEL_1P7V,
    HAL_PMU_IOLDO_LEVEL_1P725V,
    HAL_PMU_IOLDO_LEVEL_1P75V,
    HAL_PMU_IOLDO_LEVEL_1P775V,
    HAL_PMU_IOLDO_LEVEL_1P8V,
    HAL_PMU_IOLDO_LEVEL_1P825V,
    HAL_PMU_IOLDO_LEVEL_1P85V,
    HAL_PMU_IOLDO_LEVEL_1P875V,
    HAL_PMU_IOLDO_LEVEL_1P9V,
    HAL_PMU_IOLDO_LEVEL_1P925V,
    HAL_PMU_IOLDO_LEVEL_1P95V,
    HAL_PMU_IOLDO_LEVEL_1P975V,
} hal_pmu_ioldo_level_t;

typedef enum {
    HAL_PMU_INTLDO1_LEVEL_1P8V   = 0,
    HAL_PMU_INTLDO1_LEVEL_1P78V  = 1,
    HAL_PMU_INTLDO1_LEVEL_1P82V  = 2,
    HAL_PMU_INTLDO1_LEVEL_1P84V  = 3,
    HAL_PMU_INTLDO1_LEVEL_1P86V  = 4,
    HAL_PMU_INTLDO1_LEVEL_1P88V  = 5,
    HAL_PMU_INTLDO1_LEVEL_1P90V  = 6,
    HAL_PMU_INTLDO1_LEVEL_1P92V  = 7,
} hal_pmu_intldo1_level_type_t;

typedef enum {
    HAL_PMU_INTLDO2_LEVEL_1P78V  = 0,
    HAL_PMU_INTLDO2_LEVEL_1P8V   = 1,
    HAL_PMU_INTLDO2_LEVEL_1P82V  = 2,
    HAL_PMU_INTLDO2_LEVEL_1P84V  = 3,
    HAL_PMU_INTLDO2_LEVEL_1P86V  = 4,
    HAL_PMU_INTLDO2_LEVEL_1P88V  = 5,
    HAL_PMU_INTLDO2_LEVEL_1P90V  = 6,
    HAL_PMU_INTLDO2_LEVEL_1P92V  = 7,
} hal_pmu_intldo2_level_type_t;

typedef enum {
    HAL_PMU_XLDO_LEVEL_1P6V,
    HAL_PMU_XLDO_LEVEL_1P625V,
    HAL_PMU_XLDO_LEVEL_1P65V,
    HAL_PMU_XLDO_LEVEL_1P675V,
    HAL_PMU_XLDO_LEVEL_1P7V,
    HAL_PMU_XLDO_LEVEL_1P725V,
    HAL_PMU_XLDO_LEVEL_1P75V,
    HAL_PMU_XLDO_LEVEL_1P775V,
    HAL_PMU_XLDO_LEVEL_1P8V,
    HAL_PMU_XLDO_LEVEL_1P825V,
    HAL_PMU_XLDO_LEVEL_1P85V,
} hal_pmu_xldo_level_type_t;

typedef enum {
    HAL_PMU_CMU_LDO_LEVEL_0P72V  = 0,
    HAL_PMU_CMU_LDO_LEVEL_0P74V  = 1,
    HAL_PMU_CMU_LDO_LEVEL_0P76V  = 2,
    HAL_PMU_CMU_LDO_LEVEL_0P78V  = 3,
    HAL_PMU_CMU_LDO_LEVEL_0P80V  = 4,
    HAL_PMU_CMU_LDO_LEVEL_0P82V  = 5,
    HAL_PMU_CMU_LDO_LEVEL_0P84V  = 6,
    HAL_PMU_CMU_LDO_LEVEL_0P86V  = 7,
    HAL_PMU_CMU_LDO_LEVEL_0P88V  = 8,
    HAL_PMU_CMU_LDO_LEVEL_0P90V  = 9,
    HAL_PMU_CMU_LDO_LEVEL_0P92V  = 10,
    HAL_PMU_CMU_LDO_LEVEL_0P94V  = 11,
    HAL_PMU_CMU_LDO_LEVEL_0P96V  = 12,
    HAL_PMU_CMU_LDO_LEVEL_0P98V  = 13,
    HAL_PMU_CMU_LDO_LEVEL_1P00V  = 14,
    HAL_PMU_CMU_LDO_LEVEL_1P20V  = 15,
} hal_pmu_cmu_ldo_level_t;

typedef enum {
    HAL_PMU_CLDO1            = 0,
    HAL_PMU_CLDO2            = 1,
    HAL_PMU_CLDO3            = 2,
    HAL_PMU_MAX,
} hal_pmu_cldo_t;

typedef enum {
    HAL_PMU_XO_CORE_DS_1X,     // 1x drive capacity
    HAL_PMU_XO_CORE_DS_1_025X, // 1.25x drive capacity
    HAL_PMU_XO_CORE_DS_1_075X, // 1.75x drive capacity
    HAL_PMU_XO_CORE_DS_3_075X, // 3.75x drive capacity
} hal_pmu_xo_driver_capacity_t;

/**
 * @brief  Get System power on count.
 * @return System boot count.
 */
uint16_t hal_pmu_system_boot_count(void);

/**
 * @brief  GET system current status.
 * @return System current status.
 */
hal_pmu_system_current_status_type_t hal_pmu_system_current_status(void);

/**
 * @brief  Config AON sleep ready manual select value.
 * @param  manual_switch Set/Clear PMU manual select default value.
 */
void hal_aon_sleep_ready_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config AON sleep ready manual value.
 * @param  power_switch Set/Clear PMU manual default value.
 */
void hal_aon_sleep_ready_manual_config(switch_type_t power_switch);

/**
 * @brief  PMU1 reset manual control config.
 * @param  power_switch    TURN_ON/TURN_OFF.
 */
void hal_pmu_reset_manual_config(switch_type_t power_switch);

/**
 * @brief  PMU1 reset control select.
 * @param  manual_switch   Manual reg config/reset signal.
 */
void hal_pmu_reset_manual_select(switch_type_t manual_switch);

/**
 * @brief  Enable XLDO.
 * @param  en Enable/disable XLDO.
 */
void hal_pmu_xldo_enable(switch_type_t en);

/**
 * @brief  Config XLDO level voltage.
 * @param  level Config XLDO voltage.
 */
void hal_pmu_xldo_set_voltage(hal_pmu_xldo_level_type_t level);

/**
 * @brief  Control XLDO bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_xldo_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Control XLDO level high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_xldo_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Control XLDO ictr.
 * @param  ictr    Config XLDO ictr.
 */
void hal_pmu_xldo_set_ictr(uint8_t ictr);

/**
 * @brief  Enable INTLDO1.
 * @param  en Enable/disable INTLDO1.
 */
void hal_pmu_intldo1_enable(switch_type_t en);
/**
 * @brief  Config INTLDO1 level voltage.
 * @param  level Config INTLDO1 voltage.
 */
void hal_pmu_intldo1_set_voltage(hal_pmu_intldo1_level_type_t level);

/**
 * @brief  Control INTLDO1 bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_intldo1_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Control INTLDO1 level high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_intldo1_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Enable INTLDO2.
 * @param  en Enable/disable INTLDO2.
 */
void hal_pmu_intldo2_enable(switch_type_t en);

/**
 * @brief  Config INTLDO2 level voltage.
 * @param  level Config INTLDO2 voltage.
 */
void hal_pmu_intldo2_set_voltage(hal_pmu_intldo2_level_type_t level);

/**
 * @brief  Control INTLDO2 bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_intldo2_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Control INTLDO2 level high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_intldo2_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Config Auxldo voltage.
 * @param  auxldo  Auxldo ID.
 * @param  level Config Auxldo voltage.
 */
void hal_pmu_auxldo_set_voltage(hal_auxldo_id_t auxldo, hal_pmu_auxldo_level_t level);

/**
 * @brief  Control Auxldo bypass mode enable/disable.
 * @param  auxldo  Auxldo ID.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_auxldo_enable_bypass(hal_auxldo_id_t auxldo, switch_type_t enable_bypass);

/**
 * @brief  Control Auxldo high_z enable/disable.
 * @param  auxldo  Auxldo ID.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_auxldo_enable_high_resistance(hal_auxldo_id_t auxldo, switch_type_t enable_high_z);

/**
 * @brief  Control Auxldo ictr.
 * @param  auxldo  Auxldo ID.
 * @param  ictr    Config Auxldo ictr.
 */
void hal_pmu_auxldo_set_ictr(hal_auxldo_id_t auxldo, uint8_t ictr);

/**
 * @brief  Enable or disable AUXLDO.
 * @param  auxldo  Auxldo ID.
 * @param  en Enable or disable LDO.
 */
void hal_pmu_auxldo_enable(hal_auxldo_id_t auxldo, bool en);

/**
 * @brief  Get the power status of auxldo.
 * @param  auxldo  Auxldo ID.
 * @return Power status, true: power on; false: power off
 */
bool hal_pmu_auxldo_power_status(hal_auxldo_id_t auxldo);

/**
 * @brief  Enable or disable IOLDO.
 * @param  en Enable or disable IOLDO.
 */
void hal_pmu_ioldo_enable(switch_type_t en);

/**
 * @brief  Control Auxldo high_z enable/disable.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_ioldo_enable_high_resistance(switch_type_t enable_high_z);

/**
 * @brief  Control IOLDO bypass mode enable/disable.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_ioldo_enable_bypass(switch_type_t enable_bypass);

/**
 * @brief  Config IOLDO voltage.
 * @param  level Config IOLDO voltage.
 */
void hal_pmu_ioldo_set_voltage(hal_pmu_ioldo_level_t level);

/**
 * @brief  Enable CMU LDO.
 * @param  en Enable/disable CMU LDO.
 */
void hal_pmu_cmu_ldo_enable(switch_type_t en);

/**
 * @brief  Config cmu ldo voltage.
 * @param  level cmu ldo voltage level.
 */
void hal_pmu_cmu_ldo_set_voltage(hal_pmu_cmu_ldo_level_t level);

/**
 * @brief  Enable CLDO1.
 * @param  en Enable/disable CLDO1.
 */
void hal_pmu_cldo1_enable(switch_type_t en);

/**
 * @brief  Enable CLDO2.
 * @param  en Enable/disable CLDO2.
 */
void hal_pmu_cldo2_enable(switch_type_t en);

/**
 * @brief  Enable CLDO3.
 * @param  en Enable/disable CLDO3.
 */
void hal_pmu_cldo3_enable(switch_type_t en);
/**
 * @brief  enable or disable LDO vset auto cfg
 * @param  ldo       Which PMU moudle(CLDO1\CLDO2\CLDO3).
 * @param  select_en config value.
 */
void hal_pmu_cldo_vset_auto_cfg(hal_pmu_cldo_t ldo, switch_type_t select_en);

/**
 * @brief  Control CLDO bypass mode enable/disable.
 * @param  ldo               CLDO ID.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_cldo_enable_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass);

/**
 * @brief  Control CLDO high_z enable/disable.
 * @param  ldo              CLDO ID.
 * @param  enable_high_z    Enable/disable high_z.
 */
void hal_pmu_cldo_enable_high_resistance(hal_pmu_cldo_t ldo, switch_type_t enable_high_z);

/**
 * @brief  Control cldo ictr.
 * @param  ldo     CLDO ID.
 * @param  ictr    Config cldo ictr.
 */
void hal_pmu_cldo_set_ictr(hal_pmu_cldo_t ldo, uint8_t ictr);

/**
 * @brief  Config cldo voltage.
 * @param  ldo   Which PMU moudle(CLDO1\CLDO2\CLDO3).
 * @param  level Config cldo voltage.
 */
void hal_pmu_cldo_set_voltage(hal_pmu_cldo_t ldo, hal_pmu_cldo_level_t level);

/**
 * @brief  Requset cldo voltage adjustment.
 * @param  ldo   Which PMU moudle(CLDO1\CLDO2\CLDO3).
 */
void hal_pmu_cldo_vset_requset(hal_pmu_cldo_t ldo);

/**
 * @brief  Get cldo vset requst ack
 * @param  ldo   Which PMU moudle(CLDO1\CLDO2\CLDO3).
 */
uint8_t hal_pmu_cldo_vset_ack_get(hal_pmu_cldo_t ldo);

/**
 * @brief  Clear cldo vset requst ack.
 * @param  ldo   Which PMU moudle(CLDO1\CLDO2\CLDO3).
 */
void hal_pmu_cldo_vset_ack_clr(hal_pmu_cldo_t ldo);

/**
 * @brief  Config LDO manual select value.
 * @param  ldo_sel_control Config which LDO moudle.
 * @param  select_en       Manual value.
 */
void hal_ldo_manual_select_control(hal_ldo_man_sel_t ldo_sel_control, switch_type_t select_en);

/**
 * @brief  Config LDO manual select value.
 * @param  ldo_manual_set CMU manual select value.
 */
void hal_ldo_manual_select_set(uint8_t ldo_manual_set);

/**
 * @brief  Config LDO manual select value.
 * @param  manual_switch Set/Clear LDO manual select default value.
 */
void hal_ldo_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config LDO manual or auto.
 * @param  ldo_man_control Which LDO moudle.
 * @param  manual_en       Manual or auto.
 */
void hal_ldo_manual_config_control(hal_ldo_man_sel_t ldo_man_control, switch_type_t manual_en);

/**
 * @brief  Config LDO manual or auto.
 * @param  ldo_man_set Set LDO manual value.
 */
void hal_ldo_manual_config_set(uint8_t ldo_man_set);

/**
 * @brief  Config LDO manual value.
 * @param  power_switch Set/Clear CMU manual default value.
 */
void hal_ldo_manual_config(switch_type_t power_switch);

/**
 * @brief  Config PMU manual select value.
 * @param  pmu_sel_control Config which PMU moudle.
 * @param  select_en       Manual value.
 */
void hal_pmu_manual_select_control(hal_pmu_man_sel_t pmu_sel_control, switch_type_t select_en);

/**
 * @brief  Config PMU manual select value.
 * @param  pmu_manual_select PMU manual select value.
 */
void hal_pmu_manual_select_set(uint16_t pmu_manual_select);

/**
 * @brief  Config PMU manual select value.
 * @param  manual_switch Set/Clear PMU manual select default value.
 */
void hal_pmu_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config PMU manual or auto.
 * @param  pmu_man_control Which PMU moudle.
 * @param  manual_en       Manual or auto.
 */
void hal_pmu_manual_config_control(hal_pmu_man_sel_t pmu_man_control, switch_type_t manual_en);

/**
 * @brief  Config PMU manual value.
 * @param  pmu_manual_config PMU manual value.
 */
void hal_pmu_manual_config_set(uint16_t pmu_manual_config);

/**
 * @brief  Config PMU manual value.
 * @param  power_switch Set/Clear PMU manual default value.
 */
void hal_pmu_manual_config(switch_type_t power_switch);

/**
 * @brief  Get PMU sts0(vdd0p8/cldo1 vset).
 * @param  sts_0 vdd0p8/cldo1 vset value.
 */
void hal_pmu_sts_0_get(uint16_t *sts_0);

/**
 * @brief  Get PMU sts1(cldo2/cldo3 vset).
 * @param  sts_1 cldo2/cldo3 vset value.
 */
void hal_pmu_sts_1_get(uint16_t *sts_1);

/**
 * @brief  PMU system wakeup delay time config.
 * @param  pmu_module    Which wakeup time.
 * @param  delay_time   set wakeup delay time config.
 */
void hal_pmu_system_wakeup_time_set(hal_pmu_system_wakeup_time_type_t pmu_module, int16_t delay_time);

/**
 * @brief  Config INTLDO delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_en_intldo2_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config XLDO delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_en_xldo_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config CLDO1 delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_en_cldo1_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config CMU LDO delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_cmu_ldo_en_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config CLDO3 delay time.
 * @param  delay_time LDO power on delay time.
 */
void hal_pmu_cldo3_en_delay_time_config(uint16_t delay_time);

/**
 * @brief  Config xocore driver capccity.
 * @param  hal_pmu_xo_driver_capacity_t xocore driver capccity value.
 */
void hal_pmu_set_cmu_xo_trim(hal_pmu_xo_driver_capacity_t ds);

/**
 * @brief  Config xocore driver capccity.
 * @param  hal_pmu_xo_driver_capacity_t xocore driver capccity value.
 */
void hal_pmu_set_cmu_xo_trim(hal_pmu_xo_driver_capacity_t ds);

/**
 * @brief  Config cldo nor bypass.
 * @param  ldo               CLDO ID.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_cldo_enable_nor_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass);

/**
 * @brief  Config cldo slp bypass.
 * @param  ldo               CLDO ID.
 * @param  enable_bypass    Enable/disable bypass.
 */
void hal_pmu_cldo_enable_slp_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass);
/**
  * @}
  */
#endif
