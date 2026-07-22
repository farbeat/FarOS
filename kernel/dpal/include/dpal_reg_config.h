/*----------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#ifndef DPAL_REG_CONFIG_H
#define DPAL_REG_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MMC_REG_BASE            0x70021000
#define MMC_CLK_REG_BASE        0x5000A000
#define MMC_CMU_CLK             (MMC_CLK_REG_BASE + 0x0042C)
#define MMC_CMU_CLK_INIT        0x0
#define MMC_CMU_CLK_RDWR        0x1
#define MMC_CMU_CLK_MASK        0x7
#define  EMMC_FREQ_MASK         0x7
#define  EMMC_FREQ_SHIFT        0

#define FMC_REG_BASE  0x70022000
#define FMC_MEM_BASE  0x38000000

#define DPAL_CRG_REG_BASE             0x70001000

#ifndef MMC_CRG_REG_BASE
#define MMC_CRG_REG_BASE         (DPAL_CRG_REG_BASE + 0x0150)
#endif

#define IORESOURCE_IO       0x00000100  /* PCI/ISA I/O ports */
#define IORESOURCE_MEM      0x00000200
#define IORESOURCE_REG      0x00000300  /* Register offsets */
#define IORESOURCE_IRQ      0x00000400
#define IORESOURCE_DMA      0x00000800
#define IORESOURCE_BUS      0x00001000

#define OS_RISCV_SYS_VECTOR_CNT        26
#ifdef EMMC_BOOT
#define NUM_HAL_INTERRUPT_EMMC        (21 + OS_RISCV_SYS_VECTOR_CNT)
#else
#define NUM_HAL_INTERRUPT_EMMC        (15 + OS_RISCV_SYS_VECTOR_CNT)
#define NUM_HAL_INTERRUPT_SDIO        (18 + OS_RISCV_SYS_VECTOR_CNT)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* DPAL_REG_CONFIG_H */
