/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm pmu driver adapter code
 */

#include "pm_pmu.h"
#include "pm_bsp.h"
#include "pm_config.h"
#include "chip_io.h"
#include "pmu.h"
#include "pmu_ldo.h"
#include "pmu_pg.h"

#if CORE == MASTER_BY_ALL
static pm_pwr_cfg_t g_ldo_power_level[PM_PWR_MAX] = {0};

static uint32_t pmu_power_state_check(pmu_id_t pwr_id, switch_type_t config)
{
    // 如果已经打开/关闭，则返回

    // 获取sub下外设使用状态, 不允许关闭正在使用的子系统
    UNUSED(pwr_id);
    UNUSED(config);
    return 0;
}
#endif

void uapi_pmu_init(void)
{
    pmu_init();
}

void uapi_pmu_switch(pmu_id_t pwr_id, switch_type_t config)
{
    UNUSED(pwr_id);
    UNUSED(config);
    if (pwr_id >= PMU_ID_MAX) {
        return;
    }
#if CORE == MASTER_BY_ALL
    uint32_t ret = pmu_power_state_check(pwr_id, config);
    if (ret != 0) {
        return;
    }

    if (pwr_id < PMU_ID_LDO_CLDO1) {
        pmu1_dig_pg_sub_power_on((pmu1_dig_pg_sub_sys_t)(pwr_id - PMU_ID_SUB_B_SOC), config);
    } else if (pwr_id < PMU_ID_SIMO_VDD0P8) {
        uint8_t sub = (pwr_id - PMU_ID_LDO_CLDO1);
        pmu_ldo_power_on((pmu_ldo_t)sub, config);
        g_ldo_power_level[sub].pwr_en = config;
    } else {
        // SIMO BUCK
    }
#endif
}

void uapi_pmu_vset(pmu_id_t pwr_id, uint8_t level)
{
    UNUSED(level);
    if (pwr_id < PMU_ID_LDO_CLDO1 || pwr_id >= PMU_ID_MAX) {
        return;
    }

#if CORE == MASTER_BY_ALL
    uint8_t sub;
    if (pwr_id < PMU_ID_SIMO_VDD0P8) {
        sub = (pwr_id - PMU_ID_LDO_CLDO1);
        pmu_ldo_power_vset((pmu_ldo_t)sub, level);
    } else {
        // SIMO BUCK
        sub = (pwr_id - PMU_ID_SIMO_VDD0P8);
        pmu_buck_vset((pmu_simo_t)sub, level);
    }
    g_ldo_power_level[sub].pwr_vset = level;
#endif
}

void uapi_pmu_vget(pmu_id_t pwr_id, uint8_t *level)
{
    if (pwr_id < PMU_ID_LDO_CLDO1 || pwr_id >= PMU_ID_MAX) {
        return;
    }
    if (pwr_id < PMU_ID_SIMO_VDD0P8) {
        pmu_ldo_t sub = (pmu_ldo_t)(pwr_id - PMU_ID_LDO_CLDO1);
        *level = g_ldo_power_level[sub].pwr_vset;
    }
}
