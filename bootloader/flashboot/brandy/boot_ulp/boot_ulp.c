/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides button port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */

#include "boot_ulp.h"
#include "errcode.h"
#include "chip_io.h"
#include "clock_calibration.h"


bool  g_startup_finish_flag = false;
uint16_t g_ulp_wkup_flag = 0;
static uint32_t g_clock_ulp_ssb = 3276800;

#define HAL_CLC_H_OFFSET 32
#define CLK_32K_32M_EN_SEL_REG  0x57000068
#define PMU_RC_512K_XO_SEL_MASK 3
#define RCXO_32K_SEL_MASK       12
#define CALIBRATION_CYCLE_MUL   0x200
#define CALIBRATION_CLOCK_MUL   100

calendar_regs_t *g_calendar_reg = SSB_CALENDAR_BASE_ADDR;

static uint64_t uapi_ulp_systick_get_count(void)
{
    uint64_t count = 0;

    count = (uint64_t)readw(ULP_SYSTICK_VAL3) << 48U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL2) << 32U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL1) << 16U;
    count |= (uint64_t)readw(ULP_SYSTICK_VAL0);

    return count;
}

static uint32_t uapi_ulp_systick_soc_calibration(void)
{
    unsigned int irq_sts = osal_irq_lock();
    uint16_t val = readw(CLK_32K_32M_EN_SEL_REG);
    reg16_clrbit(CLK_32K_32M_EN_SEL_REG, PMU_RC_512K_XO_SEL_MASK);
    reg16_clrbit(CLK_32K_32M_EN_SEL_REG, RCXO_32K_SEL_MASK);
    g_clock_ulp_ssb = calibration_get_clock_frq(CALIBRATION_CYCLE_MUL);
    writew(CLK_32K_32M_EN_SEL_REG, val);

    osal_irq_restore(irq_sts);
    return g_clock_ulp_ssb;
}

static errcode_t uapi_enable_alarm(uint64_t timeout_ms)
{
    uint64_t count = 0;
    count = uapi_ulp_systick_get_count();
    count += (timeout_ms * g_clock_ulp_ssb / (MS_PER_S * CALIBRATION_CLOCK_MUL));

    // alarm enable
    reg16_setbit(ULP_WKUP_EVT_EN, POS_2);
    reg16_setbit(ULP_WKUP_INT_EN, POS_2);

    writew(ULP_SYSTICK_SET3, (uint16_t)(count >> 48U));
    writew(ULP_SYSTICK_SET2, (uint16_t)(count >> 32U));
    writew(ULP_SYSTICK_SET1, (uint16_t)(count >> 16U));
    writew(ULP_SYSTICK_SET0, (uint16_t)count);

    return ERRCODE_SUCC;
}

static inline uint32_t get_high_32bit(uint64_t val)
{
    return (uint32_t)(val >> HAL_CLC_H_OFFSET);
}

static inline uint32_t get_low_32bit(uint64_t val)
{
    return (uint32_t)(val & 0xFFFFFFFFLL);
}

errcode_t hal_calendar_set_timestamp(uint64_t timestamp)
{
    uint64_t counter = uapi_ulp_systick_get_count();
    hal_calendar_set_cnt_h(get_high_32bit(counter));
    hal_calendar_set_cnt_l(get_low_32bit(counter));

    hal_calendar_set_data_h(get_high_32bit(timestamp));
    hal_calendar_set_data_l(get_low_32bit(timestamp));
    return ERRCODE_SUCC;
}

static void chip_enable_ulp_ram(void)
{
    if (readw(0x5702c50c) == 0) {
        writew(0x5702c50c, 0x1);
    }
    writew(0x5700450c, 0x101);
    writew(0x5700450c, 0x1);

    if (reg16_getbit(0x5702c508, 14) == 0) {
        reg16_setbit(0x5702c508, 14);
    }

    writew(0x5702c1d0, 0x0); // 恢复ulp寄存器ULP_SLP_EN
    writew(0x5702C038, 0x0); // 恢复ulp寄存器VBUS_SENSE_DIS
    /* To ULP Systick is not cleared configuration */
    reg16_setbit(0x5702C110, 0);
}

static void uapi_sys_shipmode(uint32_t arg)
{
    UNUSED(arg);

    /* clr ulp int and event */
    writel(0x5702C1E4, 0x3F);
    writel(0x5702C1EC, 0x3F);
    uapi_tcxo_delay_us(100);

    reg16_clrbit(0x5702c508, POS_14);
    writew(0x5700450c, 0x100);

    // gpio
    writew(0x5702C200, 0x0);
    writew(0x5702C204, 0x0);
    writew(0x5702C208, 0x0);
    writew(0x5702C20c, 0x0);
    writew(0x5702C210, 0x0);

    writew(0x5702c200, 0x7);
    writew(0x57030004, 0x0);
    writew(0x57030038, 0x1);
    writew(0x5703003c, 0x1);
    writew(0x57030034, 0x0);
    writew(0x57030030, 0x1);

    writew(0x570040d8, 0xc000);
    writew(0x5702c028, 0x0);
    writew(0x5702c024, 0x3);
    writew(0x5702c1d0, 0x1);
    writew(0x5702C038, 0x0);
    writew(0x5702C1F0, 0x1C);
    writew(0x5702C110, 0x101);
    writew(0x570040d0, 0x1);
    writew(0x570040e0, 0x1);
}

static void hal_calendar_timestamp_update(void)
{
    uint64_t timestamp;

    uint64_t alarm_l = hal_calendar_get_alarm_l();
    uint64_t alarm_h = hal_calendar_get_alarm_h();
    uint64_t data_l = hal_calendar_get_data_l();
    uint64_t data_h = hal_calendar_get_data_h();
    uint64_t cnt_l = hal_calendar_get_cnt_l();
    uint64_t cnt_h = hal_calendar_get_cnt_h();
    uint64_t counter = (cnt_h << HAL_CLC_H_OFFSET) | cnt_l;
    uint64_t alarm_times = (alarm_h << HAL_CLC_H_OFFSET) | alarm_l;

    uint64_t counter_cur = uapi_ulp_systick_get_count();
    timestamp = ((data_h << HAL_CLC_H_OFFSET) | data_l) +
                ((counter_cur - counter) * MS_PER_S * CALIBRATION_CLOCK_MUL) / g_clock_ulp_ssb;
    hal_calendar_set_cnt_h(get_high_32bit(counter_cur));
    hal_calendar_set_cnt_l(get_low_32bit(counter_cur));
    hal_calendar_set_data_h(get_high_32bit(timestamp));
    hal_calendar_set_data_l(get_low_32bit(timestamp));

    if ((timestamp < alarm_times) && (alarm_times != 0)) {
        if ((timestamp + ULP_CALIBRATION_INTERVAL) < alarm_times) {
            uapi_enable_alarm(ULP_CALIBRATION_INTERVAL);
            uapi_sys_shipmode(0);
        } else if ((alarm_times - timestamp >= 1) && (alarm_times - timestamp <= ULP_CALIBRATION_INTERVAL)) {
            uapi_enable_alarm(alarm_times - timestamp);
            uapi_sys_shipmode(0);
        }
    }
}

void uapi_ulp_systick_self_verification(void)
{
    chip_enable_ulp_ram();
    if ((reg16_getbit(ULP_WKUP_INT_EN, POS_3) == 1) && (reg16_getbit(ULP_WKUP_EVT_EN, POS_3) == 1) &&
        (reg16_getbit(ULP_WKUP_INT_STS, POS_3) == 1) && (reg16_getbit(ULP_WKUP_EVT_STS, POS_3) == 1)) {
        hal_calendar_set_alarm_h(0);
        hal_calendar_set_alarm_l(0);
        reg16_setbit(ULP_WKUP_INT_CLR, POS_2);
        reg16_setbit(ULP_WKUP_EVT_CLR, POS_2);
        reg16_clrbit(ULP_WKUP_INT_EN, POS_2);
        reg16_clrbit(ULP_WKUP_EVT_EN, POS_2);
    }

    if ((reg16_getbit(ULP_WKUP_INT_EN, POS_2) == 1) && (reg16_getbit(ULP_WKUP_EVT_EN, POS_2) == 1) &&
        (reg16_getbit(ULP_WKUP_INT_STS, POS_2) == 1) && (reg16_getbit(ULP_WKUP_EVT_STS, POS_2) == 1)) {
        uapi_ulp_systick_soc_calibration();
        hal_calendar_timestamp_update();
    }
}
