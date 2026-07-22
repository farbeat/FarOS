/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: PM low power control interface
 */

#include "pm_control.h"
#include "pmu.h"
#include "pmu_interrupt.h"
#include "hal_clocks_glb.h"
#include "hal_pmu_ldo.h"
#include "hal_pmu_vdd.h"
#include "hal_pmu_peripheral.h"
#include "panic.h"
#include "securec.h"
#include "tcxo.h"
#include "pmu_cmu.h"
#include "pm_clocks.h"
#if (CORE == MASTER_BY_ALL)
#include "pmu_pg.h"
#include "non_os.h"
#include "clocks_core.h"
#include "pmu_ldo.h"
#ifndef BUILD_APPLICATION_SSB
#include "hal_cpu_core.h"
#include "cpu_load.h"
#endif
#ifdef BUILD_APPLICATION_STANDARD
#include "log_oam_logger.h"
#include "log_def.h"
#endif
#include "pm_definition.h"

// flash addr: 0x70090000
#define B_CORE_LOAD_ADDR             (BT_FLASH_REGION_START)
#define B_SRAM_ORIGIN_APP_MAPPING    0xA6000000
#define BRAM_ALL_IN_BCPU_0K_FOR_DIAG 0x0
#define VIDEO_CLK_VAL                0xfe

#define DELAY_TIME_30US            30

#define SLAVE_RUN_DELAY            1ULL

static pm_control_proc_func g_cldo1_at_lp_func = NULL;

static void pm_sdio_power_on(void)
{
    uint32_t irq = osal_irq_lock();
    pmu_sdio_glb_common_reset(TURN_OFF);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_RESET);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_RESET);

    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_MCU_HS, TURN_ON);

    pmu_sdio_glb_common_reset(TURN_ON);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_DERESET);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_DERESET);
    osal_irq_restore(irq);
}

static void pm_sdio_power_off(void)
{
    uint32_t irq = osal_irq_lock();
    pmu_sdio_glb_common_reset(TURN_OFF);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_RESET);
    hal_pmu_mcpu_perip_config(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_RESET);

    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_MCU_HS, TURN_OFF);
    osal_irq_restore(irq);
}

static void pm_display_power_on(void)
{
    uint32_t irq = osal_irq_lock();
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_MCPU_CLK_TEST, TURN_ON);
    pmu_display_power_and_reset_cfg(TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_JPGD_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_MIPITX_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_TDE_APB_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_VDP_APB_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_VIDEO_PLL2CBB_160M_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_AON_CTRL_MIPITX_CLK_EN, TURN_ON);
    hal_clocks_video_clk_en(HAL_MCPU_AON_CTRL_VIDEO_CLK_EN, TURN_ON);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_MIPITX_REQ, TURN_ON);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_TDE_REQ, TURN_ON);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_VDP_REQ, TURN_ON);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_JPGD_REQ, TURN_ON);
#ifdef BUILD_APPLICATION_STANDARD
    if (hal_clocks_video_clk_en_get() != VIDEO_CLK_VAL) {
        oml_pf_log_print0(LOG_BCORE_PLT_INFO_SYS, LOG_NUM_LIB_LOG, LOG_LEVEL_INFO,
            "Video clk en fail when Display power On.\n");
    }
#endif
    osal_irq_restore(irq);
}

static void pm_display_power_off(void)
{
    uint32_t irq = osal_irq_lock();
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_JPGD_REQ, TURN_OFF);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_VDP_REQ, TURN_OFF);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_TDE_REQ, TURN_OFF);
    hal_clocks_video_srst_en(HAL_MCPU_VIDEO_SRST_MIPITX_REQ, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_AON_CTRL_VIDEO_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_AON_CTRL_MIPITX_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_VIDEO_PLL2CBB_160M_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_VDP_APB_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_TDE_APB_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_MIPITX_CLK_EN, TURN_OFF);
    hal_clocks_video_clk_en(HAL_MCPU_JPGD_CLK_EN, TURN_OFF);
    pmu_display_power_and_reset_cfg(TURN_OFF);
    hal_clocks_aon_config(HAL_CLOCK_AON_PERIP_MCPU_CLK_TEST, TURN_OFF);
#ifdef BUILD_APPLICATION_STANDARD
    oml_pf_log_print0(LOG_BCORE_PLT_INFO_SYS, LOG_NUM_LIB_LOG, LOG_LEVEL_INFO, "Display power OFF.\n");
#endif
    osal_irq_restore(irq);
}

static void pm_bt_core_load(void)
{
    uint32_t irq = osal_irq_lock();
    cpu_load_image((void *)((uintptr_t)(B_CORE_LOAD_ADDR)), CORE_IMAGES_BT);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CPU, HAL_SOFT_RST_DERESET);
    uapi_tcxo_delay_ms(SLAVE_RUN_DELAY);
    osal_irq_restore(irq);
}

static void pm_bt_power_on(void)
{
    /* CLDO2 Power ON, bypass en */
    pmu_ldo_power_on(PMU_LDO_CLDO2, TURN_ON);
    hal_pmu_cldo_enable_bypass(HAL_PMU_CLDO2, TURN_ON);

    pmu_lpm_system_bypass_bt_status(TURN_OFF);
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_B_MEM, TURN_ON);

    // Reset b sub
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CRG, HAL_SOFT_RST_RESET);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_LGC, HAL_SOFT_RST_RESET);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CPU, HAL_SOFT_RST_RESET);

#if CHIP_ASIC
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_B_SOC, TURN_ON);
#endif
    // dereset b sub, and run
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CRG, HAL_SOFT_RST_DERESET);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_LGC, HAL_SOFT_RST_DERESET);
    hal_pmu1_dig_pg_bram_pwr_en(true);

    pmu_bt_ram_config(BRAM_ALL_IN_BCPU_0K_FOR_DIAG);

    pm_bt_core_load();
    hal_clocks_bt_core_wait_release();
}

static void pm_bt_power_off(void)
{
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CRG, HAL_SOFT_RST_RESET);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_LGC, HAL_SOFT_RST_RESET);
    hal_clocks_glb_bt_soft_reset_control(HAL_SOFT_RST2_B_CPU, HAL_SOFT_RST_RESET);
#if CHIP_ASIC
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_B_SOC, TURN_OFF);
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_B_MEM, TURN_OFF);
    hal_pmu1_dig_pg_bram_pwr_en(false);
    pmu_lpm_system_bypass_bt_status(TURN_ON);
    hal_pmu_cldo_enable_bypass(HAL_PMU_CLDO2, TURN_OFF);
    pmu_ldo_power_on(PMU_LDO_CLDO2, TURN_OFF);
#endif
}

static void pm_audio_power_on(void)
{
    if (get_dsp_power_up_status(SLAVE_CPU_DSP0) == true) {
        return;
    }

    pm_set_hifi_mode(uapi_pm_get_hifi_mode());

    hal_clocks_glb_dsp_soft_reset_control(HAL_SOFT_RST_RESET);
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_ASUB, TURN_ON); // audio sub power on
    hal_clocks_glb_dsp_soft_reset_control(HAL_SOFT_RST_DERESET);
    for (uint8_t i = 0; i < DSP_L2RAM_AREA_SIZE; i++) { // power no the memory of l2ram that belongs to dsp
        reg16_clrbit(0x5700404c, i);
    }
    hal_clocks_codec_enable();
    uapi_tcxo_delay_us(500ULL);
    set_dsp_power_up_status(SLAVE_CPU_DSP0, true);
}

static void pm_audio_power_off(void)
{
    if (get_dsp_power_up_status(SLAVE_CPU_DSP0) == false) {
        return;
    }

    hal_clocks_codec_disable();
    hal_clocks_glb_dsp_soft_reset_control(HAL_SOFT_RST_RESET);
    pmu1_dig_pg_sub_power_on(PMU1_DIG_PG_ASUB, TURN_OFF);
    uapi_clocks_codec_config(CLOCKS_CLK_SRC_MAX, CLOCK_DIV_36); // I2s clk disable
    uapi_clocks_hifi_config(CLOCKS_CLK_SRC_MAX, CLOCK_DIV_6);
    for (uint8_t i = 0; i < DSP_L2RAM_AREA_SIZE; i++) { // power off the memory of l2ram that belongs to dsp
        reg16_setbit(0x5700404c, i);
    }
    set_dsp_power_up_status(SLAVE_CPU_DSP0, false);
#ifdef BUILD_APPLICATION_STANDARD
    oml_pf_log_print0(LOG_BCORE_PLT_INFO_SYS, LOG_NUM_LIB_LOG, LOG_LEVEL_INFO, "DSP0 power off\n");
#endif
}

static void pm_sdio_power_control(pm_control_t type)
{
    if (type == PM_SDIO_POWER_UP_AND_RUN) {
        pm_sdio_power_on();
    } else if (type == PM_SDIO_POWER_OFF) {
        pm_sdio_power_off();
    }
}

static void pm_display_power_control(pm_control_t type)
{
    if (type == PM_DISPLAY_POWER_UP_AND_RUN) {
        pm_display_power_on();
    } else if (type == PM_DISPLAY_POWER_OFF) {
        pm_display_power_off();
    }
}

static void pm_audio_power_control(pm_control_t type)
{
    if (type == PM_DSP_POWER_UP_AND_RUN) {
        uapi_pm_add_sleep_veto(PM_ID_AUDIO);
        pm_audio_power_on();
    } else if (type == PM_DSP_POWER_OFF) {
        pm_audio_power_off();
        uapi_pm_remove_sleep_veto(PM_ID_AUDIO);
    }
}

static void pm_bt_power_control(pm_control_t type)
{
    if (type == PM_BT_POWER_UP_AND_RUN) {
        pm_bt_power_on();
    } else if (type == PM_BT_POWER_OFF) {
        pm_bt_power_off();
    }
}

void uapi_pm_control(pm_control_t type)
{
    if (type >= PM_CONTROL_MAX) {
        return;
    }

    if (type <= PM_BT_POWER_OFF) {
#ifndef BUILD_RECOVERY_IMAGE
        pm_bt_power_control(type);
#endif
    } else if (type <= PM_DSP_POWER_OFF) {
#ifndef BUILD_RECOVERY_IMAGE
        pm_audio_power_control(type);
#endif
    } else if (type <= PM_DISPLAY_POWER_OFF) {
        pm_display_power_control(type);
    } else if (type <= PM_SDIO_POWER_OFF) {
        pm_sdio_power_control(type);
    }
}

static hal_l2m_dsp_power_on_t pmu1_dsp_l2ram_power_on_pos_translation(l2m_dsp_power_on_t pos)
{
    switch (pos) {
        case U0_1_L2M_DSP_RAM:
            return HAL_U0_1_L2M_DSP_RAM;
        case U2_3_L2M_DSP_RAM:
            return HAL_U2_3_L2M_DSP_RAM;
        case U4_5_L2M_DSP_RAM:
            return HAL_U4_5_L2M_DSP_RAM;
        case U6_7_L2M_DSP_RAM:
            return HAL_U6_7_L2M_DSP_RAM;
        case U8_9_L2M_DSP_RAM:
            return HAL_U8_9_L2M_DSP_RAM;
        case U10_11_L2M_DSP_RAM:
            return HAL_U10_11_L2M_DSP_RAM;
        case U12_13_L2M_DSP_RAM:
            return HAL_U12_13_L2M_DSP_RAM;
        case U14_15_L2M_DSP_RAM:
            return HAL_U14_15_L2M_DSP_RAM;
        case U16_17_L2M_DSP_RAM:
            return HAL_U16_17_L2M_DSP_RAM;
        default:
            return HAL_U18_19_L2M_DSP_RAM;
    }
}

errcode_t uapi_pm_dsp_l2ram_power_on(l2m_dsp_power_on_t start, uint8_t num, switch_type_t power_on)
{
    if (num == 0) {
        return ERRCODE_FAIL;
    }

    hal_l2m_dsp_power_on_t pos = pmu1_dsp_l2ram_power_on_pos_translation(start);

    for (uint8_t i = 0; i < num; i++) {
        pos = start + i;
        if (power_on != 0) {
            hal_pmu1_dig_pg_sub_l2m_dsp_pwr_en(pos, true);
            (void)hal_pmu1_dig_pg_sub_l2m_dsp_pwr_ack_get(pos);
            hal_pmu1_dig_pg_sub_l2m_dsp_ret1_en(pos, false);
        } else {
            hal_pmu1_dig_pg_sub_l2m_dsp_ret1_en(pos, true);
            hal_pmu1_dig_pg_sub_l2m_dsp_pwr_en(pos, false);
        }
    }
    return ERRCODE_SUCC;
}
#endif