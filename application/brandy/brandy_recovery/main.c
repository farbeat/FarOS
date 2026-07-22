/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core Main function for Standard.
 * Author:
 * Create: 2021-03-09
 */
#include "main.h"
#include "cmsis_os2.h"
#include "ipc.h"
#include "string.h"
#include "rtc.h"
#include "app_os_init.h"
#include "gpio.h"
#include "chip_io.h"
#include "panic.h"
#include "securec.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"
#include "tcxo.h"
#include "systick.h"
#include "cpu_trace.h"
#include "application_version.h"
#include "pmu.h"
#include "pm_control.h"
#include "watchdog.h"
#include "watchdog_porting.h"
#include "flash.h"
#include "otp.h"
#include "cpu_utils.h"
#include "exception.h"
#include "log_oml_exception.h"
#include "uart.h"
#include "uart_porting.h"
#include "pmp_config.h"
#include "hal_xip.h"
#include "partition.h"
#include "identity.h"
#include "soc_osal.h"
#include "arch_encoding.h"
#include "arch_port.h"
#include "flash_porting.h"
#include "clocks_switch.h"
#include "pm_clocks.h"
#include "pmu_pg.h"
#include "efuse.h"
#if defined(__LITEOS__)
#include "los_task_pri.h"
#include "los_printf_pri.h"

#elif defined(__FREERTOS__)
#include "FreeRTOS.h"
#include "task.h"
#include "cache.h"
#if defined(XLTCFG_SUPPORT_MEMMNG)
#include "xlt_memory.h"
#endif
#endif

#define WDT_TIMEOUT_S  30
#define IMG_NUM_MAX    2
#define IMG_SEC        0
#define IMG_HIFI       1
#define SW_PRINT_DELAY 200ULL

#define CSR_WST_VAL                             0x4
#define CSR_WST_POS                             0xb
#define XIP_CACHE_INTR_MASK                     0xa3006128
#define XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK    0x8
#define XIP_CACHE_ENABLE_WRITE_INTR_MASK        0x1

extern uint32_t __l2m_gpu_text_start;
extern uint32_t __l2m_gpu_text_load;
extern uint32_t __l2m_gpu_text_end;
extern uint32_t __l2m_gpu_bss_start;
extern uint32_t __l2m_gpu_bss_end;
extern uint32_t __l2m_gpu_data_start;
extern uint32_t __l2m_gpu_data_load;
extern uint32_t __l2m_gpu_data_end;
#ifndef CONFIG_PSRAM_SUPPORT
extern uint32_t __l2m_dsp_text_start;
extern uint32_t __l2m_dsp_text_load;
extern uint32_t __l2m_dsp_text_end;
extern uint32_t __l2m_dsp_bss_start;
extern uint32_t __l2m_dsp_bss_end;
extern uint32_t __l2m_dsp_data_start;
extern uint32_t __l2m_dsp_data_load;
extern uint32_t __l2m_dsp_data_end;
#endif

extern char __heap_start;
extern void *g_intheap_size;
extern char __extend_heap_start;
extern void *g_extend_heap_size;

#if (defined SUPPORT_CXX)
typedef void (*SystemInitFunc)(void);
extern char __init_array_start;
extern char __init_array_end;

#if defined(__FREERTOS__)
static void OsCacheInit(void);
#else
void OsCacheInit(void);
#endif
void OsSetMainTask(void);
void *OsGetMainTask(void);

static void system_init_array(uintptr_t start, uintptr_t end)
{
    if (start >= end) {
        return;
    }

    SystemInitFunc func;
    uintptr_t *it = (uintptr_t *)start;
    for (; it != (uintptr_t *)end; ++it) {
        func = (SystemInitFunc)(*it);
        func();
    }
}
#endif

#if defined(__FREERTOS__)
static void OsCacheInit(void)
{
    dsb();
    ArchICacheInvAll();
    ArchDCacheInvAll();
    dsb();

    (void)ArchICacheEnable(CACHE_32KB);
    (void)ArchICachePrefetchEnable(CACHE_PREF_1_LINES);

    (void)ArchDCacheEnable(CACHE_32KB);
    (void)ArchDCachePrefetchEnable(CACHE_PREF_1_LINES, CACHE_PREF_1_LINES);
}
#endif

static void copy_from_flash_to_l2ram(void)
{
    uint32_t size = (uint32_t)(&__l2m_gpu_text_end) - (uint32_t)(&__l2m_gpu_text_start);
    (void)memcpy_s((void *)((uint32_t)(&__l2m_gpu_text_start)), size, (void *)(uint32_t)(&__l2m_gpu_text_load), size);

    size = (uint32_t)(&__l2m_gpu_data_end) - (uint32_t)(&__l2m_gpu_data_start);
    (void)memcpy_s((void *)((uint32_t)(&__l2m_gpu_data_start)), size, (void *)(uint32_t)(&__l2m_gpu_data_load), size);

    size = (uint32_t)(&__l2m_gpu_bss_end) - (uint32_t)(&__l2m_gpu_bss_start);
    (void)memset_s((void *)((uint32_t)(&__l2m_gpu_bss_start)), size, 0, size);

#ifndef CONFIG_PSRAM_SUPPORT
#if DSP_L2RAM_AREA_SIZE < DSP_L2RAM_AREA_TOTAL
    size = (uint32_t)(&__l2m_dsp_text_end) - (uint32_t)(&__l2m_dsp_text_start);
    (void)memcpy_s((void *)((uint32_t)(&__l2m_dsp_text_start)), size, (void *)(uint32_t)(&__l2m_dsp_text_load), size);

    size = (uint32_t)(&__l2m_dsp_data_end) - (uint32_t)(&__l2m_dsp_data_start);
    (void)memcpy_s((void *)((uint32_t)(&__l2m_dsp_data_start)), size, (void *)(uint32_t)(&__l2m_dsp_data_load), size);

    size = (uint32_t)(&__l2m_dsp_bss_end) - (uint32_t)(&__l2m_dsp_bss_start);
    (void)memset_s((void *)((uint32_t)(&__l2m_dsp_bss_start)), size, 0, size);
#endif
#endif
}

static void service_init(void)
{
    cpu_utils_init();
    hal_register_exception_dump_callback(log_exception_dump);

    uapi_partition_init();
}

static void pio_func_init(void)
{
    size_t pin_num;
    hal_pio_config_t *pio_func = NULL;

    get_pio_func_config(&pin_num, &pio_func);

    for (pin_t i = S_EGPIO0; i < pin_num; i++) {
        if (pio_func[i].func != HAL_PIO_FUNC_INVALID) {
            if (pio_func[i].func == HAL_PIO_FUNC_DEFAULT_HIGH_Z) {
                uapi_pin_set_mode(i, (pin_mode_t)HAL_PIO_FUNC_GPIO);
                uapi_gpio_set_dir(i, GPIO_DIRECTION_INPUT);
            } else {
                uapi_pin_set_mode(i, (pin_mode_t)pio_func[i].func);
                uapi_pin_set_ds(i, (pin_drive_strength_t)pio_func[i].drive);
            }
            if (pio_func[i].pull != HAL_PIO_PULL_MAX) {
                uapi_pin_set_pull(i, (pin_pull_t)pio_func[i].pull);
            }
            if (pio_func[i].ie != HAL_PIO_IE_MAX) {
                uapi_pin_set_ie(i, (pin_input_enable_t)pio_func[i].ie);
            }
        }
    }
}

static void gpio_func_init(void)
{
    size_t pin_num = 0;
    hal_gpio_config_t *gpio_func = NULL;

    get_gpio_func_config(&pin_num, &gpio_func);

    for (pin_t i = 0; i < pin_num; i++) {
        uapi_gpio_set_dir(gpio_func[i].gpio, (gpio_direction_t)gpio_func[i].direction);
        if (gpio_func[i].direction == HAL_GPIO_DIRECTION_INPUT && gpio_func[i].callback != NULL) {
            uapi_gpio_register_isr_func(gpio_func[i].gpio, gpio_func[i].triger, gpio_func[i].callback);
        }
        if (gpio_func[i].direction == HAL_GPIO_DIRECTION_OUTPUT) {
            uapi_gpio_set_val(gpio_func[i].gpio, (gpio_level_t)gpio_func[i].level);
        }
    }
}

static void board_init(uint32_t flash_id, uint32_t flash_unique_id)
{
    /* flash init */
    flash_funcreg_adapt(0);
    flash_save_manufacturer(0, flash_id);
    uapi_flash_init(0);
    uapi_flash_config_cmd_at_xip_mode(0, (uint8_t)flash_unique_id);
    uapi_flash_switch_to_cache_mode(0);
    PRINT("Flash id is %x, init success"NEWLINE, flash_id);

    pio_func_init();
    gpio_func_init();
}

static void chip_xip_init(void)
{
    hal_xip_init();
    hal_xip_config_interrupt_type(XIP_0, XIP_INTR_NMI);
    write_csr(0x7d0, read_csr(0x7d0) | (CSR_WST_VAL << CSR_WST_POS));
    // Mask interrupts caused by CACHE2AHBM module state machine timeout
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK);
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_ENABLE_WRITE_INTR_MASK);
}

static void chip_enable_ulp_ram(void)
{
    writew(0x5702c50c, 0x1);
    writew(0x5700450c, 0x1);
}

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

static void chip_init(void)
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

    chip_xip_init();
#if TCXO_CLK_DYN_ADJUST == YES
    init_mcu_core_clk();
#endif

    uapi_rtc_init();
    uapi_rtc_adapter(RTC_0, RTC_0_IRQN, g_aucIntPri[RTC_0_IRQN]);

    uapi_dma_init();
    uapi_dma_open();

    clocks_set_xip_div();
    uapi_clocks_init();

    uapi_pm_control(PM_SDIO_POWER_UP_AND_RUN);
}

static void liteos_kernel_init(void)
{
#ifdef __LITEOS__
    /* enable icache and dcahce */
    pmp_init();
    OsCacheInit();

    ArchSetExcHook((EXC_PROC_FUNC)do_fault_handler);
    ArchSetNMIHook((NMI_PROC_FUNC)do_hard_fault_handler);
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    (void)osKernelInitialize();
#if defined SUPPORT_CXX
    system_init_array((uintptr_t)&__init_array_start, (uintptr_t)&__init_array_end);
#endif
#endif
}

static void freertos_kernel_init(void)
{
#ifdef __FREERTOS__
    /* os kernel adapt config */
    vPortHwiInit();

    /* enable icache and dcahce */
    pmp_init();
    OsCacheInit();

    /* memory pool init */
#if defined(XLTCFG_SUPPORT_MEMMNG)
    g_heap = (void *)&__heap_start;
    if (xlt_mem_init(g_heap, (uint32_t)&g_intheap_size)) {
        panic(PANIC_MEMORY, __LINE__);
    }

    g_extend_heap = (void *)&__extend_heap_start;
    if (xlt_mem_init(g_extend_heap, (uint32_t)&g_extend_heap_size)) {
        panic(PANIC_MEMORY, __LINE__);
    }
#endif

    osal_irq_request(NON_MASKABLE_INT_IRQN, (osal_irq_handler)do_fault_handler_freertos, NULL, NULL, NULL);
    osal_irq_enable(NON_MASKABLE_INT_IRQN);
    (void)osKernelInitialize();
    vTimerTickInit();

#if defined SUPPORT_CXX
    system_init_array((uintptr_t)&__init_array_start, (uintptr_t)&__init_array_end);
#endif
#endif
}

static void chip_watchdog_init(void)
{
    watchdog_turnon_clk();
    watchdog_func_adapt(CHIP_WDT_TIMEOUT_32S);
    uapi_watchdog_init(CHIP_WDT_TIMEOUT_32S);
    uapi_watchdog_enable(WDT_MODE_INTERRUPT);
}

static void kernel_init(void)
{
    chip_watchdog_init();
    pmu1_dsp_l2ram_config(0);
    copy_from_flash_to_l2ram();

    panic_init();

    freertos_kernel_init();
    liteos_kernel_init();
}

static startup_table ssb_to_app_table;

void sec_main(const void *startup_details_table)
{
    if (memcpy_s(&ssb_to_app_table, sizeof(ssb_to_app_table), startup_details_table, sizeof(ssb_to_app_table)) != EOK) {
        panic(PANIC_MEMORY, __LINE__);
        return;
    }

    kernel_init();

    chip_init();

    board_init(ssb_to_app_table.ssb_to_application_flash_id, ssb_to_app_table.ssb_to_application_flash_unique_id);

    service_init();

    app_os_init(NULL, 0);

    (void)osKernelStart();
    for (;;) {
    }
}
