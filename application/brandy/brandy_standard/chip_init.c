/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Application core chip init function for standard
 * Author:
 * Create:
 */
#include "chip_init.h"
#include "chip_io.h"
#include "tcxo.h"
#include "systick.h"
#include "timer.h"
#include "rtc.h"
#include "gpio.h"
#include "ipc.h"
#include "uart_porting.h"
#include "log_memory_region.h"
#include "clocks_core_common.h"
#include "clocks_switch.h"
#include "dma.h"
#include "pm_clocks.h"
#include "hal_xip.h"
#include "pinctrl.h"
#include "arch_encoding.h"
#include "debug_print.h"
#if !defined(BRANDY_MCU_BT)
#include "button_porting.h"
#else
uint32_t uapi_ulp_systick_soc_calibration(void);
#endif
#ifdef CONFIG_CLDO1PD_SUPPORT
#include "pmu_ldo.h"
#endif

#define DELAY_TIME_120US                        120
#define POS_TWO                                 2
#define CSR_WST_VAL                             0x4
#define CSR_WST_POS                             0xb
#define XIP_CACHE_INTR_MASK                     0xa3006128
#define XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK    0x8
#define XIP_CACHE_ENABLE_WRITE_INTR_MASK        0x1
#define XIP_CACHE_2AHBM_INTR_MASK               0x2

#if ENABLE_LOW_POWER == YES
extern uint32_t __sys_pm_start__;
extern uint32_t __sys_pm_load__;
extern uint32_t __sys_pm_end__;
#endif

static void clocks_set_xip_div(void)
{
    // xip cache div = 2
    writew(0xa3006014, 0x1);  // sync mode = 1, high freq
    regw_clrbits(0xa3006000, 0x24, 0x0, 0x1);       // close xip cache clk en
    regw_clrbits(0xa3006000, 0x24, 0x3, 0x1);       // close xip cache div en
    regw_setbits(0xa3006000, 0x24, 0x4, 0x2, 0x2);  // xip cache div = 2 from xip_opi
    regw_setbits(0xa3006000, 0x24, 0x3, 0x1, 0x1);  // open xip cache div en
    regw_setbits(0xa3006000, 0x24, 0x0, 0x1, 0x1);  // open xip cache clk en
}

#if ENABLE_LOW_POWER == YES
static void copy_from_flash_to_pm_text(void)
{
    uint32_t size = (uint32_t)(&__sys_pm_end__) - (uint32_t)(&__sys_pm_start__);
    if (memcpy_s((void *)((uint32_t)(&__sys_pm_start__)), size, (void *)(uint32_t)(&__sys_pm_load__), size) != 0) {
        PRINT("Copy from flash to pm fail."NEWLINE);
    }
}
#endif
static void chip_xip_init(void)
{
    hal_xip_init();
    hal_xip_config_interrupt_type(XIP_0, XIP_INTR_NMI);
    write_csr(0x7d0, read_csr(0x7d0) | (CSR_WST_VAL << CSR_WST_POS));
    // Mask interrupts caused by CACHE2AHBM module state machine timeout
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK);
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_ENABLE_WRITE_INTR_MASK);
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_2AHBM_INTR_MASK);
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

    /* To ULP Systick is not cleared configuration */
    reg16_setbit(0x5702C110, 0);
}

static void chip_get_tempe(void)
{
    uint32_t irq_sts = (uint32_t)osal_irq_lock();
    float real_temp = 0;
    uint32_t tmp = 0;
    writel(0x570040BC, 0x50);
    uapi_tcxo_delay_us(120); // wait 120us
    writel(0x570040BC, 0x70);
    uapi_tcxo_delay_us(30); // wait 30us
    writel(0x570040BC, 0x30);
    writel(0x570000B8, 0x0);
    writel(0x57004020, 0x101);
    uapi_tcxo_delay_us(30); // wait 30us
    writel(0x57004020, 0x1);
    writel(0x57004020, 0x11);
    writel(0x570000B8, 0x7);
    writel(0x57004244, 0x10);
    writel(0x59000820, 0x1A0);
    reg16_setbit(0x57000068, 0x0);
    writel(0x59000A34, 0x0);
    writel(0x59000A04, 0x3);
    writel(0x59000A10, 0x0);
    writel(0x59000A00, 0x1);
    uapi_tcxo_delay_ms(10); // wait 10ms
    tmp = (uint32_t)readl(0x59000A0C);
    real_temp = (((float)tmp - 0x706) / 0x14) + 26.4; // base tempe 26.4
    osal_irq_restore(irq_sts);
    printf("chip temperature=%f(tmp=%d)\r\n", real_temp, tmp);
    writew(0x57004244, 0x0); // close Tsensor switch
    writew(0x57004020, 0x100);
    uapi_tcxo_delay_us(30); // wait 30us
    writew(0x570000B8, 0x0);
    writew(0x570040BC, 0xc0);
}

void chip_init(void)
{
    chip_enable_ulp_ram();
    clocks_pll_init_vote();
    uapi_pin_init();
    uapi_gpio_init();
    uapi_efuse_init();

    /* uart init */
    uart_config_init();
    PRINT("App uart init succ."NEWLINE);

    uapi_tcxo_init();
    uapi_systick_init();
    uapi_timer_init();
    uapi_timer_adapter(0, TIMER_0_IRQN, irq_prio(TIMER_0_IRQN));

    chip_xip_init();
#if TCXO_CLK_DYN_ADJUST == YES
    init_mcu_core_clk();
#endif

    uapi_rtc_init();
    uapi_rtc_adapter(RTC_0, RTC_0_IRQN, g_aucIntPri[RTC_0_IRQN]);

    uapi_dma_init();
    uapi_dma_open();

    ipc_init();

    log_memory_region_init();

#if ENABLE_LOW_POWER == YES
    copy_from_flash_to_pm_text();
#endif
    clocks_set_xip_div();
    uapi_pmu_init();
    uapi_clocks_init();
    uapi_lpc_init();
#ifdef CONFIG_CLDO1PD_SUPPORT
    pmu_control_cldo1_sleep_bypass(TURN_OFF);
#endif
    chip_get_tempe();

    uapi_ulp_systick_soc_calibration();
}