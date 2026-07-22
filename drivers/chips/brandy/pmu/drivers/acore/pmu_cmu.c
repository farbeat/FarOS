/*
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description:  PMU cmu DRIVER
 */

#include "pmu_cmu.h"
#include "soc_osal.h"
#include "hal_clocks_glb.h"
#include "tcxo.h"
#include "otp.h"
#include "clocks.h"
#include "clocks_core.h"
#include "panic.h"
#include "hal_reg_config.h"
#include "ipc.h"
#include "hal_clocks_app.h"
#include "hal_cpu_core.h"
#include "hal_pmu_ldo.h"
#include "hal_pmu_cmu.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "log_uart.h"
#include "log_def.h"
#include "log_oam_logger.h"
#endif

#define PMU_REG_HIGH_16             16

#define I2S_BASE                    49152000
#define REF_BASE                    32000000

#define FIRST_SET_FRAC_STEP_H       0xFF
#define FIRST_SET_FRAC_STEP_L       0x0
#define PLL_SET_FRAC_STEP_H         0x0
#define PLL_SET_FRAC_STEP_L         0xC8

#define MAX_INT32_T                 0x7FFFFFFF
#define WAIT_PLL_CFG_VLD_DONE       5
#define PMU_CMU_TIMEOUT             1000
#define CHECK_FREQUENCY             1000
#define PMU_CMU_STEP_PANIC          1

#define PLL_FBDIV_NUM_DEFAULT       0x37
#define PLL_FRAC_H_NUM_DEFAULT      0x4B
#define PLL_FRAC_L_NUM_DEFAULT      0xC6AB
#define PLL_POST_DIV1_DEFAULT       0x2
#define PLL_POST_DIV2_DEFAULT       0x1

bool g_pll_used = false;

bool pmu_cmu_pll_used_get(void)
{
    return g_pll_used;
}

void pmu_cmu_analog_status_grm_interrupt_enable(pmu_ana_sts_grm_int_t ana_sts_grm_int,
                                                switch_type_t int_en)
{
    if (int_en == TURN_ON) {
        hal_pmu_cmu_analog_status_raw_interrupt_enable((uint16_t)ana_sts_grm_int);
    } else {
        hal_pmu_cmu_analog_status_raw_interrupt_disable((uint16_t)ana_sts_grm_int);
    }
}

void pmu_pll_unlock_interrupt_enable(void)
{
    hal_pmu_analog_status_grm_interrupt_enable(PMU_ANA_UNLOCK_GRM, TURN_ON);
}

void pmu_pll_unlock_interrupt_disable(void)
{
    hal_pmu_analog_status_grm_interrupt_enable(PMU_ANA_UNLOCK_GRM, TURN_OFF);
}

void pmu_cmu_init(void)
{
    // Enable CLK_TCXO2DBB_GT/CLK_TCXO2DBB_PD
    hal_cmu_dbg_config_control(CMU_XO_CORE_PD, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_TCXO2DBB_GT, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_TCXO2DBB_PD, TURN_OFF);

    // Enable all pmu manual signal
    hal_pmu_manual_select_control(EN_CMU_LDO, TURN_ON);
    hal_pmu_manual_select_control(EN_CMU_LDO_DELAY, TURN_ON);
    hal_pmu_manual_config_control(EN_CMU_LDO, TURN_ON);
    hal_pmu_manual_config_control(EN_CMU_LDO_DELAY, TURN_ON);
    uapi_tcxo_delay_us(120ULL);

    hal_cmu_dbg_config_control(CMU_CLK_TCXO2ANA_PD, TURN_OFF);

    hal_cmu_dbg_config_control(CMU_CLK_XO_GT, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_GT, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_ISO_EN, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_XO_PLL_GT, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_BT_ADC_GT, TURN_OFF);
    // Enable GLB clk
    hal_cmu_dbg_config_control(CMU_GLB_PD, TURN_OFF);
}

void pmu_cmu_pll_init(void)
{
    if (g_pll_used) {
        return;
    }

    uint32_t irq = osal_irq_lock();
// make pll reg to hardware control
    hal_cmu_manual_single_config(CMU_CLK_PLL2CMU_GT, TURN_OFF);
    hal_cmu_manual_single_config(CMU_CLK_PLL2CMU_ISO_EN, TURN_OFF);
    hal_cmu_manual_single_config(CMU_CLK_XO_PLL_GT, TURN_OFF);

    hal_clocks_pll_module_clken(TURN_OFF);
    hal_pmu2_cmu_pll_fbdiv_control_select(TURN_ON);
    hal_pmu2_cmu_pll_fbdiv_manual_set(PLL_FBDIV_NUM_DEFAULT);

    hal_pmu2_cmu_pll_frac_control_select(TURN_ON);
    hal_pmu2_cmu_pll_frac_manual_set(PLL_FRAC_H_NUM_DEFAULT, PLL_FRAC_L_NUM_DEFAULT);

    hal_pmu2_cmu_pll_post_div_set(PLL_POST_DIV2_DEFAULT, PLL_POST_DIV1_DEFAULT);

    hal_cmu_dbg_config_control(CMU_GLB_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTVCO_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTPOSTDIV_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUT4PHASE_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DAC_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DSM_PD, TURN_OFF);
    hal_clocks_pll_module_clken(TURN_ON);
    osal_irq_restore(irq);
    uapi_tcxo_delay_us(100ULL);
    hal_pmu2_cmu_pll_lock_check();
    g_pll_used = true;
}

void pmu_cmu_pll_reinit(pmu_cmu_core_t pll, cmu_pll_config_t pll_cfg)
{
    UNUSED(pll);
    if (g_pll_used) {
        return;
    }
    uint32_t irq = osal_irq_lock();
    uapi_tcxo_delay_us(120ULL);
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_GT, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_ISO_EN, TURN_OFF);
    hal_cmu_dbg_config_control(CMU_CLK_XO_PLL_GT, TURN_OFF);
    hal_clocks_pll_module_clken(TURN_OFF);
    hal_cmu_dbg_config_control(CMU_GLB_PD, TURN_ON); // PD_DBG 1 power down
    hal_pmu2_cmu_pll_post_div_set(pll_cfg.pll_postdiv2, pll_cfg.pll_postdiv1);
    hal_pmu2_cmu_pll_fbdiv_manual_set(pll_cfg.pll_fbdiv);
    hal_pmu2_cmu_pll_frac_manual_set(pll_cfg.pll_frac_h, pll_cfg.pll_frac_l);
    hal_cmu_dbg_config_control(CMU_GLB_PD, TURN_OFF); // PD_DBG 0 power on
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTVCO_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTPOSTDIV_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUT4PHASE_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DAC_PD, TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DSM_PD, TURN_OFF);
    hal_clocks_pll_module_clken(TURN_ON);
    hal_pmu2_cmu_pll_lock_check();
    g_pll_used = true;
    osal_irq_restore(irq);
    uapi_tcxo_delay_us(100ULL);
}

void pmu_cmu_pll_deinit(pmu_cmu_core_t pll)
{
    UNUSED(pll);
    if (!g_pll_used) {
        return;
    }

    uint32_t irq = osal_irq_lock();
    pmu_pll_unlock_interrupt_disable();
    hal_clocks_pll_module_clken(TURN_OFF);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTVCO_PD, TURN_ON);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUTPOSTDIV_PD, TURN_ON);
    hal_pmu_cmu_core_pll_cfg_control(CMU_FOUT4PHASE_PD, TURN_ON);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DAC_PD, TURN_ON);
    hal_pmu_cmu_core_pll_cfg_control(CMU_DSM_PD, TURN_ON);
// make pll reg power down
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_GT, TURN_ON);
    hal_cmu_dbg_config_control(CMU_CLK_PLL2CMU_ISO_EN, TURN_ON);
    hal_cmu_dbg_config_control(CMU_CLK_XO_PLL_GT, TURN_ON);
// make pll reg to manual
    hal_cmu_manual_single_config(CMU_CLK_PLL2CMU_GT, TURN_ON);
    hal_cmu_manual_single_config(CMU_CLK_PLL2CMU_ISO_EN, TURN_ON);
    hal_cmu_manual_single_config(CMU_CLK_XO_PLL_GT, TURN_ON);
    uapi_tcxo_delay_us(120ULL);

    g_pll_used = false;
    osal_irq_restore(irq);
}

uint32_t pmu_cmu_get_pll_clock_value(void)
{
    uint32_t temp;
    uint64_t frac, fbdiv, fvco;
    uint8_t div1, div2;

    fbdiv = hal_pmu2_cmu_pll_fbdiv_manual_get();
    frac = hal_pmu2_cmu_pll_frac_manual_get();
    hal_pmu2_cmu_pll_post_div_get(&div1, &div2);
    if ((div1 == 0) || (div2 == 0)) {
        return 0;
    }

    // Calculate the pll value
    // FVCO : FREF *(FBDIV+FRAC/(2^24))/REFDIV ;CLK_PLL:FVCO/POSTDIV1/POSTDIV2
    fvco = (uint64_t)((REF_BASE * fbdiv) + ((REF_BASE * frac) >> 24));
    temp = (uint32_t)(fvco / div1 / div2);
    return temp;
}

bool pmu_cmu_get_pll_status(void)
{
    return g_pll_used;
}

void pmu_cmu_init_pll_status(bool used)
{
    g_pll_used = used;
}

/* Cmu txco init. */
void pmu_cmu_tcxo_init(void)
{
}

void pmu_cmu_pll_step(int32_t step)
{
    int32_t temp;
    uint64_t frac, fbdiv, foutvco;
    uint8_t div1, div2;
    static bool first_cfg_pll_vld = true;

    temp = I2S_BASE + step;
    if (((I2S_BASE + step) >= MAX_INT32_T) || (temp <= 0)) {
        panic(PANIC_PMU_CMU, __LINE__);
        return;  //lint !e527  unreachable code
    }

    hal_pmu2_cmu_pll_post_div_get(&div1, &div2);

    // Calculate the target value
    foutvco = (uint64_t)(((uint64_t)(uint32_t)temp) * PLL_I2S_CODEC_CLKDIV * (div1 * div2));
    fbdiv = (foutvco / REF_BASE);
    frac = (foutvco % REF_BASE);
    // FVCO : FREF *(FBDIV+FRAC/(2^24))/REFDIV ;CLK_PLL:FVCO/POSTDIV1/POSTDIV2
    frac = ((frac << 24) + (REF_BASE >> 1)) / REF_BASE;

    hal_pmu2_cmu_pll_fbdiv_target_set((uint16_t)fbdiv);
    hal_pmu2_cmu_pll_frac_target_set((uint32_t)frac);

    if (first_cfg_pll_vld) {
        hal_pmu2_cmu_pll_set_frac_step(FIRST_SET_FRAC_STEP_H, FIRST_SET_FRAC_STEP_L);
        hal_pmu2_cmu_pll_cfg_valid((switch_type_t)true);
        uapi_tcxo_delay_us((uint64_t)WAIT_PLL_CFG_VLD_DONE);
        hal_pmu2_cmu_pll_fbdiv_control_select(TURN_OFF);
        hal_pmu2_cmu_pll_frac_control_select(TURN_OFF);
        first_cfg_pll_vld = false;
    } else {
        hal_pmu2_cmu_pll_set_frac_step(PLL_SET_FRAC_STEP_H, PLL_SET_FRAC_STEP_L);
    }

    uint64_t time = uapi_tcxo_get_ms();
    uint64_t check_frq = CHECK_FREQUENCY;
    while (1) { //lint !e716
        if (fbdiv == hal_pmu2_cmu_pll_fbdiv_status()) {
            if (frac == hal_pmu2_cmu_pll_frac_status()) {
                return;
            }
        }

        check_frq--;
        if (check_frq == 0) {
            if ((time + PMU_CMU_TIMEOUT) < uapi_tcxo_get_ms()) {
                panic(PANIC_PMU_CMU, PMU_CMU_STEP_PANIC);
                return;  //lint !e527  unreachable code
            } else {
                check_frq = CHECK_FREQUENCY;
            }
        }
    }
}

static bool lpc_pll_micro_adjust_req(ipc_action_t message,
                                     const volatile ipc_payload *payload_p,
                                     cores_t src,
                                     uint32_t id)
{
    UNUSED(message);
    UNUSED(src);
    UNUSED(id);

#if CHIP_ASIC
    uint32_t irq_sts = osal_irq_lock();
    int32_t *step = (int32_t *)payload_p;
    pmu_cmu_pll_step(*step);
    osal_irq_restore(irq_sts);

#ifdef BUILD_APPLICATION_STANDARD
    oml_pf_log_print1(LOG_BCORE_PLT_INFO_CPU_CLK, LOG_NUM_INFO_CPU_CLK, LOG_LEVEL_INFO,
                      "[PMU_CMU]pll adjust:%d", *step);
#endif
#else
    UNUSED(payload_p);
#endif
    return true;
}

void hal_lpc_init(void)
{
    ipc_register_handler(IPC_ACTION_PLL_MICRO_REQ, lpc_pll_micro_adjust_req);
}
