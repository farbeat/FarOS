/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Interrupt DRIVER
 * Author: @CompanyNameTag
 * Create: 2022-03-22
 */
#include "chip_io.h"
#include "arch_encoding.h"
#include "vectors.h"
#include "arch_barrier.h"
#include "interrupt_handler.h"

/*lint -e40 -e571 -e718 -e737 -e746*/
uint32_t int_set_irq_func(int32_t irq_id, isr_function func)
{
    ((isr_function *)(isr_get_ramexceptiontable_addr()))[irq_id] = func;

    return 0;
}

void int_set_priority_grouping(uint32_t priority_group)
{
    UNUSED(priority_group);
}

uint32_t int_get_priority_grouping(void)
{
    return 0;
}

void int_enable_irq(int32_t irq_id)
{
    dsb();
    volatile uint32_t clicinte = CLIC_BASE_REG + sys_clic_intie(irq_id);
    writeb(clicinte, SYS_CLICINTEN_ENABLE);
    dsb();
}

void int_disable_irq(int32_t irq_id)
{
    dsb();
    volatile uint32_t clicinte = CLIC_BASE_REG + sys_clic_intie(irq_id);
    writeb(clicinte, SYS_CLICINTEN_DISABLE);
    dsb();
}

uint32_t int_get_enable_irq(int32_t irq_id)
{
    volatile uint32_t clicinte = CLIC_BASE_REG + sys_clic_intie(irq_id);
    return readb(clicinte);
}

uint32_t int_get_pending_irq(int32_t irq_id)
{
    UNUSED(irq_id);
    return 0;
}

void int_clear_pending_irq(int32_t irq_id)
{
    dsb();
    volatile uint32_t clicintip = CLIC_BASE_REG + sys_clic_intip(irq_id);
    writeb(clicintip, SYS_CLICINTIP_DISABLE);
    dsb();
}

void int_set_pendind_irq(int32_t irq_id)
{
    dsb();
    volatile uint32_t clicintip = CLIC_BASE_REG + sys_clic_intip(irq_id);
    writeb(clicintip, SYS_CLICINTIP_ENABLE);
    dsb();
}

uint32_t int_get_active(int32_t irq_id)
{
    UNUSED(irq_id);
    return 0;
}

void int_set_priority(int32_t irq_id, uint32_t priority)
{
    uint8_t local_level = priority;
    local_level = local_level  << (POS_8 - SYS_CLICFGLEVELBITS);
    uint8_t mask = ((uint8_t)(-1)) >> (SYS_CLICFGLEVELBITS);
    uint8_t clicintctl = readb(CLIC_BASE_REG + sys_clic_intctl(irq_id));
    clicintctl = clicintctl & mask;
    clicintctl = clicintctl | local_level;
    writeb(CLIC_BASE_REG + sys_clic_intctl(irq_id), clicintctl);
}

uint32_t int_get_priority(int32_t irq_id)
{
    uint8_t clicintctl = readb(CLIC_BASE_REG + sys_clic_intctl(irq_id));
    uint8_t mask = ((uint8_t)(-1)) >> (SYS_CLICFGLEVELBITS);
    clicintctl = clicintctl & (~mask);
    clicintctl = clicintctl >> (POS_8 - SYS_CLICFGLEVELBITS);
    return clicintctl;
}

void int_system_reset(void)
{
}

void int_setup(void)
{
    uint8_t cliccfg = 0;
    cliccfg |= BIT_0 | (SYS_CLICNLBITS << POS_1);
    writeb(CLIC_BASE_REG + SYS_CLIC_CFG, cliccfg);

    uint8_t clicintattr = 0;
    /* clicintattr[0] = 1 vector; clicintattr[2:1] = 0 positive level-triggered; clicintattr[7:6] = 3 M-mode */
    clicintattr |= ((BIT_0) | (SYS_CLICINTATTR_MODE << POS_6));

    (void)set_csr(mstatus, MSTATUS_MIE);
    for (uint32_t irq_id = ISR_VECTOR_IRQ_0; irq_id < BUTT_IRQN; irq_id++) {
        writeb(CLIC_BASE_REG + sys_clic_intip(irq_id), SYS_CLICINTIP_DISABLE);      /* clear pending */
        writeb(CLIC_BASE_REG + sys_clic_intie(irq_id), SYS_CLICINTEN_DISABLE);      /* disabled */
        writeb(CLIC_BASE_REG + sys_clic_intattr(irq_id), clicintattr);
        writeb(CLIC_BASE_REG + sys_clic_intctl(irq_id), SYS_CLICINTCTL_DEF);
    }
}

bool int_is_interrupt_context(void)
{
    return (interrupt_number_get() > 0);
}

int32_t int_get_current_irqn(void)
{
    return (int32_t)(interrupt_number_get());
}

int32_t int_get_current_priority(void)
{
    return (int32_t)(int_get_priority(int_get_current_irqn()));
}
#ifndef __GNUC__
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif