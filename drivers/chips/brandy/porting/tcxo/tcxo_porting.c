/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides tcxo port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "tcxo_porting.h"
#include "hal_tcxo.h"
#include "hal_tcxo_v150.h"
#include "chip_io.h"

#define DELAY_COUNT_US            27 // 1us count at 0.9v
#define TCXO_CNT_EN               2

static uintptr_t g_tcxo_base_addr =  (uintptr_t)TCXO_COUNT_BASE_ADDR;

uintptr_t tcxo_porting_base_addr_get(void)
{
    return g_tcxo_base_addr;
}

void tcxo_port_register_hal_funcs(void)

{
    hal_tcxo_register_funcs(hal_tcxo_v150_funcs_get());
}

void tcxo_port_unregister_hal_funcs(void)
{
    hal_tcxo_unregister_funcs();
}

static uint32_t g_tcxo_ticks_per_usec = TCXO_TICKS_PER_U_SECOND;

uint32_t tcxo_porting_ticks_per_usec_get(void)
{
    return g_tcxo_ticks_per_usec;
}

void tcxo_porting_ticks_per_usec_set(uint32_t ticks)
{
    g_tcxo_ticks_per_usec = ticks;
}

void tcxo_port_delay_us(uint32_t delay)
{
    volatile uint32_t val = delay;
    volatile uint32_t count;

    while (--val > 0) {
        count = DELAY_COUNT_US;
        while (count-- > 0) {
            ;
        }
    }
}

void tcxo_port_enable(uint8_t en)
{
    if (en != 0) {
        reg16_setbit(TCXO_COUNT_BASE_ADDR, TCXO_CNT_EN);
    } else {
        reg16_clrbit(TCXO_COUNT_BASE_ADDR, TCXO_CNT_EN);
    }
}