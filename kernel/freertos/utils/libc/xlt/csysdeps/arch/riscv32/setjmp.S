  .globl  setjmp
  .type   setjmp, @function
setjmp:
#ifdef CONFIG_LIBC_COMPILE_LDM
    stmia {ra, sp, s0-s11}, (a0) /* store value in a0 address to registers. */
#else
    sw s0,    0(a0)
    sw s1,    4(a0)
    sw s2,    8(a0)
    sw s3,    12(a0)
    sw s4,    16(a0)
    sw s5,    20(a0)
    sw s6,    24(a0)
    sw s7,    28(a0)
    sw s8,    32(a0)
    sw s9,    36(a0)
    sw s10,   40(a0)
    sw s11,   44(a0)
    sw sp,    48(a0)
    sw ra,    52(a0)
#endif

    li      a0, 0
    ret
    .size   setjmp, .-setjmp

  .globl  longjmp
  .type   longjmp, @function
longjmp:
#ifdef CONFIG_LIBC_COMPILE_LDM
    ldmia {ra, sp, s0-s11}, (a0)
#else
    lw s0,    0(a0)
    lw s1,    4(a0)
    lw s2,    8(a0)
    lw s3,    12(a0)
    lw s4,    16(a0)
    lw s5,    20(a0)
    lw s6,    24(a0)
    lw s7,    28(a0)
    lw s8,    32(a0)
    lw s9,    36(a0)
    lw s10,   40(a0)
    lw s11,   44(a0)
    lw sp,    48(a0)
    lw ra,    52(a0)
#endif

    seqz a0, a1 /* if a1 = 0, a0 = 1; if a1 != 0, a0 = 0 */
    add  a0, a0, a1
    ret
    .size   longjmp, .-longjmp
