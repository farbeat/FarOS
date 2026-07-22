/******************************************************************************
 * @brief    CRT0 file used for starting up the rest of the code
 * Copyright (c) 2022 CompanyNameMagicTag
******************************************************************************/
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied sec_main() routine is called.
 * @param  None
 * @retval : None
*/
#if defined(__FREERTOS__)
        .extern freertos_risc_v_trap_handler
#elif defined(__LITEOS__)
        .extern TrapVector
#endif
        .extern g_vectorIntTable
        .section .text.entry
        .global reset_vector
        .option norvc
reset_vector:
        j _start

_start:
        mv t4, a0
        mv t5, a1
        mv t6, a2

#if defined(__FREERTOS__)
        la t0, freertos_risc_v_trap_handler
        andi t0,t0,0xfffffffc
        csrw mtvec, t0
#elif defined(__LITEOS__)
        la t0, TrapVector
        csrw    mtvec, t0
        la t0, g_vectorIntTable
        csrw    0x307, t0
#endif

        la      t0, __irq_stack_top__
        csrw    mscratch, t0
        csrwi   mstatus, 0
        csrwi   mie, 0
        la t3, g_intheap_begin

        /* Copy application core version table to heap begin */
set_app_table_loop:
        lw     t0, (t4)
        sw     t0, (t3)
        addi   t4, t4, 4
        addi   t3, t3, 4
        addi   t5, t5, -1               /* Decrease the number of bytes to do by a quad octet */
        blt    x0, t5, set_app_table_loop   /* Keep going until it is all done */
        la     t5, g_intheap_begin

        .option push
        .option norelax
        /* initialize global pointer */
        la      gp, _gp_

        # initialize stack pointer
        la sp, __stack_top__

        /* init stack */
        la      t0, g_system_stack_begin
        la      t1, g_system_stack_end
        beq     t0, t1, end_set_stack_loop
        li      t2, 0xefbeadde

set_stack_loop:
        sw      t2, (t0)
        addi    t0, t0, 4
        blt     t0, t1, set_stack_loop
end_set_stack_loop:

        /* set data section */
        la      t0, __data_begin__
        la      t1, __data_load__
        la      t2, __data_size__
        beq     t2, x0, end_set_data_loop

set_data_loop:
        lw      t3, (t1)
        sw      t3, (t0)
        addi    t0, t0, 4
        addi    t1, t1, 4
        addi    t2, t2, -4
        blt     x0, t2, set_data_loop
end_set_data_loop:

        /* setup .ramtext section */
        la      t0, __ramtext_begin__
        la      t1, __ramtext_load__
        la      t2, __ramtext_size__
        beq     t2, x0, end_set_ramtext_loop

set_ramtext_loop:
        lw      t3, (t1)
        sw      t3, (t0)
        addi    t0, t0, 4
        addi    t1, t1, 4
        addi    t2, t2, -4
        blt     x0, t2, set_ramtext_loop
end_set_ramtext_loop:
        /* t6 = 0 means no psram on borad */
        beq     t6, x0, end_set_psramtext_loop

        /* setup .psram_text section */
        la      t0, __psram_text_start
        la      t1, __psram_text_load
        la      t2, __psram_text_size
        beq     t2, x0, end_set_psramtext_loop

set_psramtext_loop:
        lw      t3, (t1)
        sw      t3, (t0)
        addi    t0, t0, 4
        addi    t1, t1, 4
        addi    t2, t2, -4
        blt     x0, t2, set_psramtext_loop
end_set_psramtext_loop:

        /* clear bss section */
        la      t0, __bss_begin__
        la      t1, __bss_end__
        beq     t0, t1, end_clear_bss_loop
        li      t2, 0x00000000

clear_bss_loop:
        sw      t2, (t0)
        addi    t0, t0, 4
        blt     t0, t1, clear_bss_loop
end_clear_bss_loop:
        .option pop

        /* set to initial state of FPU */
        li      t0, 0x00006000
        csrs    mstatus, t0
        fssr    x0

        mv a0, t5
        j sec_main
