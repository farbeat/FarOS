#ifndef _ASM_REGISTER_CONFIG_H
#define _ASM_REGISTER_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* RISC-V NMI config */
#define REG_BASE_PERI_CTRL      0x20000
#define NMI_REG_BASE            (REG_BASE_PERI_CTRL + 0xC18)
#define REG_BASE_MTIMER         0x110D1000 /* RISC-V MTIMER */

#define UART0_REG_BASE           0x70010000 // ACPU uart
#define UART1_REG_BASE           0x70011000
#define UART2_REG_BASE           0x70012000
#define UART3_REG_BASE           0x90094000 // share uart

// for linx170 start
#define TIMER0_REG_BASE         0x5200203C
// for linx170 end

#define TIMER1_REG_BASE         0x70002100
#define TIMER2_REG_BASE         0x70002200
#define TIMER3_REG_BASE         0x70002300
#define TIMER4_REG_BASE         0x70002400
#define TIMER5_REG_BASE         0x70002500
#define TIMER6_REG_BASE         0x70002600
#define TIMER7_REG_BASE         0x70002700

#define FPB_BASE                0xe0000000

#define TIMER_TICK_REG_BASE     TIMER0_REG_BASE
#define TIMER_TICK_ENABLE       TIMER0_ENABLE
#define TIMER_TIME_REG_BASE     TIMER1_REG_BASE
#define TIMER_TIME_ENABLE       TIMER1_ENABLE

#define MMC_REG_BASE            0x70021000
#define MMC_CLK_REG_BASE        0x5000A000
#define MMC_CMU_CLK             (MMC_CLK_REG_BASE + 0x0042C)
#define MMC_CMU_CLK_INIT        0x0
#define MMC_CMU_CLK_RDWR        0x1
#define MMC_CMU_CLK_MASK        0x7

#define FMC_REG_BASE             0x70022000
#define FMC_MEM_BASE             0x38000000

#define IO_MUX_REG_BASE          0x50005000
#define MISC_REG_BASE            IO_MUX_REG_BASE

#define SYS_CTRL_REG_BASE        0x50000000
#define CRG_REG_BASE             0x50001000
#define CRG_SYSLDO_CTRL_BASE     0x50002000
#define CRG_CPU_RST_CTRL         (CRG_SYSLDO_CTRL_BASE + 0x100)

/* PLL Clock */
#ifdef LOSCFG_BOARD_ASIC
#define CMU_BBPLL_LOCK                  0x5000A66C
#define CMU_CLK_GT_EN                   0x5000A440
#define CMU_CLK_320M_ACORE_GT_BIT_SHIFT (18)
#define CMU_CLK_320M_ACORE_GT_MASK      (0x1 << CMU_CLK_320M_ACORE_GT_BIT_SHIFT)
#define CMU_CLK_320M_ACORE_EN_BIT_SHIFT (19)
#define CMU_CLK_320M_ACORE_EN_MASK      (0x1 << CMU_CLK_320M_ACORE_EN_BIT_SHIFT)
#define PLL_ON_VOTE_BY_ACPU             0x50003CAC
#define CLK_SEL                         0x70001124
#define CLK_SEL_STS                     0x70001128
#define ACPU_BUS_CLKA_STS_BIT_SHIFT     (0)
#define ACPU_BUS_CLKA_STS_MASK          (0x1 << ACPU_BUS_CLKA_STS_BIT_SHIFT)
#define ACPU_BUS_CLKB_STS_BIT_SHIFT     (1)
#define ACPU_BUS_CLKB_STS_MASK          (0x1 << ACPU_BUS_CLKB_STS_BIT_SHIFT)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ASM_REGISTER_CONFIG_H */
