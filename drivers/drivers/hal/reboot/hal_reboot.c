/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  HAL Reboot functionality.
 * Author: @CompanyNameTag
 * Create:  2018-10-15
 */
#include "hal_reboot.h"
#include "soc_osal.h"
#include "core.h"
#include "non_os_reboot.h"
#include "debug_print.h"
#include "chip_io.h"
#if (CORE == MASTER_BY_ALL) && (CORE != WIFI)
#ifdef SUPPORT_GPIO
#include "hal_gpio.h"
#endif
#include "preserve.h"
#endif

#define HAL_REBOOT_REASON_MCPU_WDT      0x2002
#define HAL_REBOOT_REASON_DSP_WDT       0x4002
#define HAL_REBOOT_REASON_BCPU_WDT      0x8002
#define HAL_REBOOT_REASON_BCPU_CHIP_WDT 0x8004

#define HAL_CHIP_RESET_REG                  (PMU1_CTL_RB_BASE + CHIP_RESET_OFF)
#define HAL_CHIP_RESET_REG_OFFSET           0x0
#define HAL_CHIP_RESET_REG_ENABLE_RESET_BIT 0
#define HAL_PMU_PROTECT_STATUS_REG     (PMU1_CTL_RB_BASE + 0x300)
#define HAL_PMU_PROTECT_STATUS_CLR_REG (PMU1_CTL_RB_BASE + 0x304)

// buck short circuit protection, if this case occur, cpu can't read, only ssi can read the status.
#define HAL_PMU_PROTECT_BUCK2_SCP_BIT 2
#define HAL_PMU_PROTECT_BUCK1_SCP_BIT 3
#define HAL_PMU_PROTECT_CHIP_WDG_BIT  0
#define HAL_PMU_PROTECT_CHIP_WDG_BIT_CLR  0
#define HAL_BCPU_RESET_STS_REG (GLB_CTL_M_RB_BASE + 0x30)
#define HAL_MCPU_RESET_STS_REG (GLB_CTL_M_RB_BASE + 0x34)

// This status is valid when CLDO2 power on.
#define HAL_DSP_RESET_STS_REG            (GLB_CTL_B_RB_BASE + 0x38)
#define HAL_RESET_STS_COMMON_RESET_BIT   0 // Control by RST_PULSE0 at GLB_CTL_B + 0x70
#define HAL_RESET_STS_CORE_RESET_BIT     1 // Control by RST_PULSE0 at GLB_CTL_B + 0x70
#define HAL_RESET_STS_CORE_WDT_RESET_BIT 2
#define HAL_RESET_STS_CLEAR_REG (GLB_CTL_M_RB_BASE + 0x3c)
#define HAL_RESET_STS_CLEAR_ALL 0xFF
// Software dedicated whether a hard power failure occurs
#define HAL_ULP_AON_GENERAL_REG         (ULP_AON_CTL_RB_ADDR + 0x14)
#define HAL_ULP_AON_NO_POWEROFF_FLAG    0xA5A5

#define PM_SLEEP_SOFT_RESET_FORCE_WKUP_REG 0x57000014
#define PM_SLEEP_SOFT_RESET_FORCE_WKUP_VAL 0xb66
#define PM_SLEEP_SOFT_RESET_FORCE_WKUP_ACK 0x499
#define PM_MCU_SEND_IPC_WKUP_BT_REG        0x52000034
#define PM_MCU_SEND_IPC_WKUP_BT_VAL        0x2
#define PM_WAIT_BT_WAKEUP_DELAY            100
#define HAL_REBOOT_WAIT_ACK_TRY_TIMES      60
#define BT_RESET_DELAY                     500
#define POS_NO_POWEROFF 15
#define POS_FIRST_POWERON 14

static uint16_t g_reset_cause = REBOOT_CAUSE_UNKNOWN;
void hal_reboot_set_ulp_aon_no_poweroff_flag(void)
{
    reg16_setbit(HAL_ULP_AON_GENERAL_REG, POS_NO_POWEROFF);
}

bool hal_reboot_get_ulp_aon_no_poweroff_flag(void)
{
    if (reg16_getbit(HAL_ULP_AON_GENERAL_REG, POS_NO_POWEROFF)) {
        return true;
    } else {
        return false;
    }
}

void hal_reboot_set_first_power_on_flag(void)
{
    reg16_setbit(HAL_ULP_AON_GENERAL_REG, POS_FIRST_POWERON);
}

void hal_reboot_clear_first_power_on_flag(void)
{
    reg16_clrbit(HAL_ULP_AON_GENERAL_REG, POS_FIRST_POWERON);
}

bool hal_reboot_get_first_power_on_flag(void)
{
    if (reg16_getbit(HAL_ULP_AON_GENERAL_REG, POS_FIRST_POWERON)) {
        return true;
    } else {
        return false;
    }
}

bool hal_reboot_hard_wdg_timeout(uint16_t cause)
{
    bool ret = false;
    uint16_t wdg_timeout_flag = 0;
    uint16_t i = 0;
    uint16_t stat = 0;
    wdg_timeout_flag = readw(HAL_PMU_PROTECT_STATUS_REG);
    if ((wdg_timeout_flag & BIT(HAL_PMU_PROTECT_CHIP_WDG_BIT)) != 0) {
        g_reset_cause = REBOOT_CAUSE_APPLICATION_STD_CHIP_WDT_FRST;
        do {
            reg16_setbit(HAL_PMU_PROTECT_STATUS_CLR_REG, HAL_PMU_PROTECT_CHIP_WDG_BIT_CLR);
            stat = reg16_getbit(HAL_PMU_PROTECT_STATUS_REG, HAL_PMU_PROTECT_CHIP_WDG_BIT);
            i++;
        } while ((stat == 1) && (i < UINT16_MAX));
        if (stat == 1) {
            PRINT("PMU_PROTECT_STATUS_REG clear fail = %x\r\n", readl(HAL_PMU_PROTECT_STATUS_REG));
        }
        ret = true;
    } else {
        g_reset_cause = cause;
    }
    if (wdg_timeout_flag != 0) {
        PRINT("Chip boot err,"NEWLINE);
    }
    return ret;
}

void hal_reboot_deinit(void)
{
}

void hal_reboot_chip(void)
{
#if CORE == MASTER_BY_ALL && (CORE != WIFI)
    uint32_t irq_sts = osal_irq_lock();

    set_cpu_utils_system_boot_magic();
#ifdef SUPPORT_GPIO
    gpio_ulp_int_en(false);  // when soft reset, ulp area is not cleared.
#endif
    // when bt at sleep, need wkup bt before soft reset
#if !defined(BUILD_APPLICATION_SSB) && !defined(BUILD_RECOVERY_IMAGE)
    uint16_t lock_cnt = 0;
    uint16_t max_lock_cnt = HAL_REBOOT_WAIT_ACK_TRY_TIMES;
    writew(PM_SLEEP_SOFT_RESET_FORCE_WKUP_REG, PM_SLEEP_SOFT_RESET_FORCE_WKUP_VAL);
    writew(PM_MCU_SEND_IPC_WKUP_BT_REG, PM_MCU_SEND_IPC_WKUP_BT_VAL);
    while((readw(PM_SLEEP_SOFT_RESET_FORCE_WKUP_REG) != PM_SLEEP_SOFT_RESET_FORCE_WKUP_ACK) && \
          (lock_cnt < max_lock_cnt)) {
        PRINT("bt not send ack[%x],don't to reset,try %d.\n", readw(PM_SLEEP_SOFT_RESET_FORCE_WKUP_REG), lock_cnt);
        uapi_tcxo_delay_ms(PM_WAIT_BT_WAKEUP_DELAY);
        lock_cnt++;
    }
#else
    writew(PM_SLEEP_SOFT_RESET_FORCE_WKUP_REG, PM_SLEEP_SOFT_RESET_FORCE_WKUP_VAL);
    writew(PM_MCU_SEND_IPC_WKUP_BT_REG, PM_MCU_SEND_IPC_WKUP_BT_VAL);
    uapi_tcxo_delay_ms(BT_RESET_DELAY);
#endif
    // Request the reset & wait for it...
    regw_clrbit(HAL_CHIP_RESET_REG, HAL_CHIP_RESET_REG_OFFSET, HAL_CHIP_RESET_REG_ENABLE_RESET_BIT);
    for (;;) {}
#if defined(__GNUC__)
    osal_irq_restore(irq_sts);
#endif
#endif
}

void hal_reboot_clear_history(void)
{
#if (CORE == MASTER_BY_ALL) && (CORE != WIFI)
    writew(HAL_RESET_STS_CLEAR_REG, HAL_RESET_STS_CLEAR_ALL);
    reg16_setbit(HAL_PMU_PROTECT_STATUS_CLR_REG, HAL_PMU_PROTECT_CHIP_WDG_BIT_CLR);
#endif
}

/**
 * Return reset code for the last time this core was rebooted
 */
uint16_t hal_reboot_get_reset_reason(void)
{
    return g_reset_cause;
}
