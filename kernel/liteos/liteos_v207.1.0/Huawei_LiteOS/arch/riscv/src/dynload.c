/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: RISCV Dynamic Load handle
 * Author: Huawei LiteOS Team
 * Create: 2022-12-20
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
 * --------------------------------------------------------------------------- */

#include "los_ld_reloc_pri.h"

/*
 * 0x800: The successive instruction has a signed 12-bit immediate
 * so the value of the preceding high 20-bit relocation may have 1 added to it.
 */
#define U_TYPE_OFFSET 0x800
#define HI20_MASK 0xFFFFF000
LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvRel32Reloc(const LDRelocParam *relocParam)
{
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR position = relocParam->position;
    UINTPTR symAddr = relocParam->symAdd;

    *(UINTPTR *)position = symAddr + addend;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvJumpSlotReloc(const LDRelocParam *relocParam)
{
    UINTPTR position = relocParam->position;
    UINTPTR symAddr = relocParam->symAdd;

    *(UINTPTR *)position = symAddr;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvRelativeReloc(const LDRelocParam *relocParam)
{
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR position = relocParam->position;

    *(UINTPTR *)position = relocParam->baseSegment + addend;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvJalReloc(const LDRelocParam *relocParam)
{
#define JAL_BIT 20U
    INT32 imm;
    INT32 val;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend - position);
    val = (INT32)OS_SIGN_EXTEND((UINT32)imm >> 1U, JAL_BIT);
    if ((val <= (-(INT32)(1U << JAL_BIT))) || ((val >= 0) && ((UINT32)val >= (1U << JAL_BIT)))) {
        return LOS_NOK;
    }

    OS_MEM_READ_32(position, instruction);
    /*
     * UJ-type:
     * |31      |30       21|20       |19       12 |11     |6      |0
     * |imm[20] |imm[10:1]  |imm[11]  |imm[19:12]  |rd     |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 20, 31, 1);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 1, 21, 10);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 11, 20, 1);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 12, 12, 8);
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvBranchReloc(const LDRelocParam *relocParam)
{
#define BRANCH_BIT 12U
    INT32 imm;
    INT32 val;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend - position);
    val = (INT32)OS_SIGN_EXTEND((UINT32)imm >> 1U, BRANCH_BIT);
    if ((val <= (-(INT32)(1U << BRANCH_BIT))) || ((val >= 0) && ((UINT32)val >= (1U << BRANCH_BIT)))) {
        return LOS_NOK;
    }

    OS_MEM_READ_32(position, instruction);
    /*
     * SB-type:
     * |31      |30      25|24    |19   |14      |11       8|7       |6      |0
     * |imm[12] |imm[10:5] |rs2   |rs1  |funct3  |imm[4:1]  |imm[11] |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 12, 31, 1);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 5, 25, 6);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 1, 8, 4);
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 11, 7, 1);
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvCallReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;
    INT32 offset;

    offset = (INT32)(symAddr + addend - position);
    imm = (INT32)((UINT32)(offset + U_TYPE_OFFSET) & HI20_MASK);
    OS_MEM_READ_32(position, instruction);
    /*
     * MACRO call,tail (auipc+jalr pair)
     * auipc(U-type):
     * |31                                 12|11                  |6      |0
     * |imm[31:12]                           |rd                  |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 12, 12, 20);
    OS_MEM_WRITE_32(position, instruction);

    imm = (INT32)((UINT32)(offset - imm) & 0xfff);  /* 0xfff: low 12 bit */
    OS_MEM_READ_32(position + 4, instruction); /* 4: 32bits */
    /*
     * jalr(I-type):
     * |31                  20|19    |14     |11      |6      |0
     * |imm[11:0]             |rs1   |funct3 |rd      |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 0, 20, 12);
    OS_MEM_WRITE_32(position + 4, instruction); /* 4: 32bits */
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvPcrelHigh20Reloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;
    INT32 offset;

    offset = (INT32)(symAddr + addend - position);
    imm = (INT32)((UINT32)(offset + U_TYPE_OFFSET) & HI20_MASK);
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_hi|hi(symbol) (U-Type):
     * |31                               12|11                  |6      |0
     * |imm[31:12]                         |rd                  |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 12, 12, 20); /* 12-31bit -> 12-31bit, total 20 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvHigh20Reloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend) + U_TYPE_OFFSET;
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_hi|hi(symbol) (U-Type):
     * |31                               12|11                  |6      |0
     * |imm[31:12]                         |rd                  |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 12, 12, 20);
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvPcrelLow12IReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;
    INT32 offset;

    offset = (INT32)(symAddr + addend - position);
    imm =(INT32)((UINT32)offset - ((UINT32)(offset + U_TYPE_OFFSET) & HI20_MASK));
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_lo|lo(pcrel_hi20_address) (I-Type):
     * |31                        20|19    |14     |11      |6      |0
     * |imm[11:0]                   |rs1   |funct3 |rd      |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 0, 20, 12);  /* 0-11bit -> 20-31bit, total 12 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvLow12IReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend) - (INT32)(((symAddr + addend) + U_TYPE_OFFSET) & HI20_MASK);
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_lo|lo(pcrel_hi20_address) (I-Type):
     * |31                        20|19    |14     |11      |6      |0
     * |imm[11:0]                   |rs1   |funct3 |rd      |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 0, 20, 12);  /* 0-11bit -> 20-31bit, total 12 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvPcrelLow12SReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;
    INT32 offset;

    offset = (INT32)(symAddr + addend - position);
    imm = (INT32)((UINT32)offset - ((UINT32)(offset + U_TYPE_OFFSET) & HI20_MASK));
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_lo|lo(pcrel_hi20_address) (S-Type):
     * |31     25|24      |19       |14     |11      7|6      |0
     * imm[11:5] |rs2     |rs1      |funct3 |imm[4:0] |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 5, 25, 7);  /* 5-11bit -> 25-31bit, total 7 bits */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 0, 7, 5);   /* 0-4bit -> 7-11bit, total 5 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvLow12SReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend) - (INT32)(((symAddr + addend) + U_TYPE_OFFSET) & HI20_MASK);
    OS_MEM_READ_32(position, instruction);
    /*
     * %pcrel_lo|lo(pcrel_hi20_address) (S-Type):
     * |31     25|24      |19       |14     |11      7|6      |0
     * imm[11:5] |rs2     |rs1      |funct3 |imm[4:0] |opcode
     */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 5, 25, 7);  /* 5-11bit -> 25-31bit, total 7 bits */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 0, 7, 5);   /* 0-4bit -> 7-11bit, total 5 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvRvcBranchReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend - position);
    OS_MEM_READ_32(position, instruction);

    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 8, 12, 1);  /* 8bit -> 12bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 6, 5, 2);   /* 6-7bit -> 5-6bit, total 2 bits */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 5, 2, 1);   /* 5bit -> 2bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 3, 10, 2);  /* 3-4bit -> 10-11bit, total 2 bits */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 1, 3, 2);   /* 1-2bit -> 3-4bit, total 2 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvRvcJumpReloc(const LDRelocParam *relocParam)
{
    INT32 imm;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    UINT32 instruction;

    imm = (INT32)(symAddr + addend - position);
    OS_MEM_READ_32(position, instruction);

    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 11, 12, 1);  /* 11bit -> 12bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 10, 8, 1);   /* 10bit -> 8bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 8, 9, 2);    /* 8-9bit -> 9-10bit, total 2 bits */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 7, 6, 1);    /* 7bit -> 6bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 6, 7, 1);    /* 6bit -> 7bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 5, 2, 1);    /* 5bit -> 2bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 4, 11, 1);   /* 4bit -> 11bit */
    LD_MoveBitsArea32((UINT32)imm, (UINT32 *)&instruction, 1, 3, 3);    /* 1-3bit -> 3-5bit, total 3 bits */
    OS_MEM_WRITE_32(position, instruction);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvAdd8Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    operation = (INT32)(symAddr + addend);
    *(UINT8 *)position += (UINT8)operation;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvAdd16Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;

    operation = (INT32)(symAddr + addend);

    *(UINT16 *)position += (UINT16)operation;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvAdd32Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;

    operation = (INT32)(symAddr + addend);

    *(UINT32 *)position += (UINT32)operation;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvSub8Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    operation = (INT32)(symAddr + addend);

    *(UINT8 *)position -= (UINT8)operation;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvSub16Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    operation = (INT32)(symAddr + addend);

    *(UINT16 *)position -= (UINT16)operation;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvSub32Reloc(const LDRelocParam *relocParam)
{
    INT32 operation;
    UINT32 addend = (UINT32)relocParam->addend;
    UINTPTR symAddr = relocParam->symAdd;
    UINTPTR position = relocParam->position;
    operation = (INT32)(symAddr + addend);

    *(UINT32 *)position -= (UINT32)operation;

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ELF_RiscvNoneReloc(const LDRelocParam *relocParam)
{
    (VOID)relocParam;
    return LOS_OK;
}

STATIC UINT32 (*gRelocHandlers[]) (const LDRelocParam *relocParam) = {
    [OS_R_RISCV_NONE]           = ELF_RiscvNoneReloc,
    [OS_R_RISCV_32]             = ELF_RiscvRel32Reloc,
    [OS_R_RISCV_RELATIVE]       = ELF_RiscvRelativeReloc,
    [OS_R_RISCV_JUMP_SLOT]      = ELF_RiscvJumpSlotReloc,
    [OS_R_RISCV_BRANCH]         = ELF_RiscvBranchReloc,
    [OS_R_RISCV_JAL]            = ELF_RiscvJalReloc,
    [OS_R_RISCV_CALL]           = ELF_RiscvCallReloc,
    [OS_R_RISCV_PCREL_HI20]     = ELF_RiscvPcrelHigh20Reloc,
    [OS_R_RISCV_PCREL_LO12_I]   = ELF_RiscvPcrelLow12IReloc,
    [OS_R_RISCV_PCREL_LO12_S]   = ELF_RiscvPcrelLow12SReloc,
    [OS_R_RISCV_HI20]           = ELF_RiscvHigh20Reloc,
    [OS_R_RISCV_LO12_I]         = ELF_RiscvLow12IReloc,
    [OS_R_RISCV_LO12_S]         = ELF_RiscvLow12SReloc,
    [OS_R_RISCV_ADD8]           = ELF_RiscvAdd8Reloc,
    [OS_R_RISCV_ADD16]          = ELF_RiscvAdd16Reloc,
    [OS_R_RISCV_ADD32]          = ELF_RiscvAdd32Reloc,
    [OS_R_RISCV_SUB8]           = ELF_RiscvSub8Reloc,
    [OS_R_RISCV_SUB16]          = ELF_RiscvSub16Reloc,
    [OS_R_RISCV_SUB32]          = ELF_RiscvSub32Reloc,
    [OS_R_RISCV_ALIGN]          = ELF_RiscvNoneReloc,
    [OS_R_RISCV_RVC_BRANCH]     = ELF_RiscvRvcBranchReloc,
    [OS_R_RISCV_RVC_JUMP]       = ELF_RiscvRvcJumpReloc,
    [OS_R_RISCV_RELAX]          = ELF_RiscvNoneReloc,
};

STATIC LITE_OS_SEC_TEXT_MINOR UINT32 ArchDoRelocImp(const LD_ELF_RELA *rela, const LDRelocParam *relocParam)
{
    UINT32 ret;
    UINT32 type = LD_ELF_R_TYPE(rela->info);
    UINT32 (*handler) (const LDRelocParam *relocParam) = NULL;

    if (type >= (UINT32)(sizeof(gRelocHandlers) / sizeof(gRelocHandlers[0]))) {
        PRINT_ERR("reloc error, no type %u\n", type);
        return LOS_NOK;
    }

    handler = gRelocHandlers[type];
    if (handler == NULL) {
        PRINT_ERR("Unknown relocation type %u\n", type);
        return LOS_NOK;
    }

    ret = handler(relocParam);
    return ret;
}

LITE_OS_SEC_TEXT_MINOR UINT32 ArchSegRelocImp(const LD_ELF_RELA *rela, const LDRelocParam *relocParam)
{
    return ArchDoRelocImp(rela, relocParam);
}

LITE_OS_SEC_TEXT_MINOR UINT32 ArchObjRelocImp(const LD_ELF_RELA *relocCmd, const LDRelocParam *relocParam)
{
    return ArchDoRelocImp(relocCmd, relocParam);
}

LITE_OS_SEC_TEXT_MINOR UINT32 ArchSegRelocTblCheck(const RelocationInfo *relocInfo)
{
    return (relocInfo->relTableSize != 0) ? LOS_OK : LOS_NOK;
}
