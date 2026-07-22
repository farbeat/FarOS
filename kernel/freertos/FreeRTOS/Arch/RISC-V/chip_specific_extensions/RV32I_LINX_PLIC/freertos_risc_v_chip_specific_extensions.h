/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code that tailors the port to a specific RISC-V chip:
 *
 * + FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S contains the code that
 *   is common to all currently supported RISC-V chips.  There is only one
 *   portASM.S file because the same file is built for all RISC-V target chips.
 *
 * + Header files called freertos_risc_v_chip_specific_extensions.h contain the
 *   code that tailors the FreeRTOS kernel's RISC-V port to a specific RISC-V
 *   chip.  There are multiple freertos_risc_v_chip_specific_extensions.h files
 *   as there are multiple RISC-V chip implementations.
 *
 * !!!NOTE!!!
 * TAKE CARE TO INCLUDE THE CORRECT freertos_risc_v_chip_specific_extensions.h
 * HEADER FILE FOR THE CHIP IN USE.  This is done using the assembler's (not the
 * compiler's!) include path.  For example, if the chip in use includes a core
 * local interrupter (CLINT) and does not include any chip specific register
 * extensions then add the path below to the assembler's include path:
 * FreeRTOS\Source\portable\GCC\RISC-V-RV32\chip_specific_extensions\RV32I_CLINT_no_extensions
 *
 */


#ifndef __FREERTOS_RISC_V_EXTENSIONS_H__
#define __FREERTOS_RISC_V_EXTENSIONS_H__

#if __riscv_xlen == 64
    #define portWORD_SIZE 8
    #define store_x sd
    #define load_x ld
    #define fstore_x fsd
    #define fload_x fld
#elif __riscv_xlen == 32
    #define store_x sw
    #define load_x lw
    #define fstore_x fsw
    #define fload_x flw
    #define portWORD_SIZE 4
#else
#endif

#define portasmHAS_SIFIVE_CLINT 1
#if (defined __riscv_flen) && (__riscv_flen > 0)
#define configFPU_ENABLE 1
#else
#define configFPU_ENABLE 0
#endif
#define portasmFPU_ENABLE configFPU_ENABLE
#define portasmHAS_MTIME 0
#if portasmFPU_ENABLE
#define portasmADDITIONAL_CONTEXT_SIZE 33 /* Must be even number on 32-bit cores. */
#else
#define portasmADDITIONAL_CONTEXT_SIZE 0
#endif

#define portasmHANDLE_INTERRUPT vHwiEntry

#if portasmFPU_ENABLE
.macro portasmSAVE_ADDITIONAL_REGISTERS
    /* FPU */
    addi sp, sp, -(portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE)
    fstore_x f0, 1 * portWORD_SIZE( sp )
    fstore_x f1, 2 * portWORD_SIZE( sp )
    fstore_x f2, 3 * portWORD_SIZE( sp )
    fstore_x f3, 4 * portWORD_SIZE( sp )
    fstore_x f4, 5 * portWORD_SIZE( sp )
    fstore_x f5, 6 * portWORD_SIZE( sp )
    fstore_x f6, 7 * portWORD_SIZE( sp )
    fstore_x f7, 8 * portWORD_SIZE( sp )
    fstore_x f8, 9 * portWORD_SIZE( sp )
    fstore_x f9, 10 * portWORD_SIZE( sp )
    fstore_x f10, 11 * portWORD_SIZE( sp )
    fstore_x f11, 12 * portWORD_SIZE( sp )
    fstore_x f12, 13 * portWORD_SIZE( sp )
    fstore_x f13, 14 * portWORD_SIZE( sp )
    fstore_x f14, 15 * portWORD_SIZE( sp )
    fstore_x f15, 16 * portWORD_SIZE( sp )
    fstore_x f16, 17 * portWORD_SIZE( sp )
    fstore_x f17, 18 * portWORD_SIZE( sp )
    fstore_x f18, 19 * portWORD_SIZE( sp )
    fstore_x f19, 20 * portWORD_SIZE( sp )
    fstore_x f20, 21 * portWORD_SIZE( sp )
    fstore_x f21, 22 * portWORD_SIZE( sp )
    fstore_x f22, 23 * portWORD_SIZE( sp )
    fstore_x f23, 24 * portWORD_SIZE( sp )
    fstore_x f24, 25 * portWORD_SIZE( sp )
    fstore_x f25, 26 * portWORD_SIZE( sp )
    fstore_x f26, 27 * portWORD_SIZE( sp )
    fstore_x f27, 28 * portWORD_SIZE( sp )
    fstore_x f28, 29 * portWORD_SIZE( sp )
    fstore_x f29, 30 * portWORD_SIZE( sp )
    fstore_x f30, 31 * portWORD_SIZE( sp )
    fstore_x f31, 32 * portWORD_SIZE( sp )
    frcsr t0
    store_x t0, 33 * portWORD_SIZE( sp )
    .endm

.macro portasmRESTORE_ADDITIONAL_REGISTERS
    /* FPU */
    load_x t0, 33 * portWORD_SIZE(sp)
    fscsr  t0
    fload_x f0, 1 * portWORD_SIZE( sp )
    fload_x f1, 2 * portWORD_SIZE( sp )
    fload_x f2, 3 * portWORD_SIZE( sp )
    fload_x f3, 4 * portWORD_SIZE( sp )
    fload_x f4, 5 * portWORD_SIZE( sp )
    fload_x f5, 6 * portWORD_SIZE( sp )
    fload_x f6, 7 * portWORD_SIZE( sp )
    fload_x f7, 8 * portWORD_SIZE( sp )
    fload_x f8, 9 * portWORD_SIZE( sp )
    fload_x f9, 10 * portWORD_SIZE( sp )
    fload_x f10, 11 * portWORD_SIZE( sp )
    fload_x f11, 12 * portWORD_SIZE( sp )
    fload_x f12, 13 * portWORD_SIZE( sp )
    fload_x f13, 14 * portWORD_SIZE( sp )
    fload_x f14, 15 * portWORD_SIZE( sp )
    fload_x f15, 16 * portWORD_SIZE( sp )
    fload_x f16, 17 * portWORD_SIZE( sp )
    fload_x f17, 18 * portWORD_SIZE( sp )
    fload_x f18, 19 * portWORD_SIZE( sp )
    fload_x f19, 20 * portWORD_SIZE( sp )
    fload_x f20, 21 * portWORD_SIZE( sp )
    fload_x f21, 22 * portWORD_SIZE( sp )
    fload_x f22, 23 * portWORD_SIZE( sp )
    fload_x f23, 24 * portWORD_SIZE( sp )
    fload_x f24, 25 * portWORD_SIZE( sp )
    fload_x f25, 26 * portWORD_SIZE( sp )
    fload_x f26, 27 * portWORD_SIZE( sp )
    fload_x f27, 28 * portWORD_SIZE( sp )
    fload_x f28, 29 * portWORD_SIZE( sp )
    fload_x f29, 30 * portWORD_SIZE( sp )
    fload_x f30, 31 * portWORD_SIZE( sp )
    fload_x f31, 32 * portWORD_SIZE( sp )
    addi sp, sp, (portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE) /* Remove space added */
    .endm
#else
.macro portasmSAVE_ADDITIONAL_REGISTERS
    /* No additional registers to save, so this macro does nothing. */
    .endm

.macro portasmRESTORE_ADDITIONAL_REGISTERS
    /* No additional registers to restore, so this macro does nothing. */
    .endm

#endif
#endif /* __FREERTOS_RISC_V_EXTENSIONS_H__ */
