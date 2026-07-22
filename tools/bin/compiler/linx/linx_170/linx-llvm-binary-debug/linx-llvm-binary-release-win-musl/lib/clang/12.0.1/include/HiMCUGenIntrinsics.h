#define RISCV_GET_TASK_STACK()  __builtin_get_context_base()
#define RISCV_CHG_TASK_STACK(a) __builtin_chg_context_base(a)
#define RISCV_ADD  __builtin_linx_add
#define RISCV_ADDI  __builtin_linx_addi
#define RISCV_ADDI_FROM_GP  __builtin_linx_addi_from_gp
#define RISCV_ADDI_FROM_RA  __builtin_linx_addi_from_ra
#define RISCV_ADDI_FROM_SP  __builtin_linx_addi_from_sp
#define RISCV_ADDI_TO_GP  __builtin_linx_addi_to_gp
#define RISCV_ADDI_TO_RA  __builtin_linx_addi_to_ra
#define RISCV_ADDI_TO_SP  __builtin_linx_addi_to_sp
#define RISCV_ADDSHF  __builtin_linx_addshf
#define RISCV_AND  __builtin_linx_and
#define RISCV_ANDI  __builtin_linx_andi
#define RISCV_ANDN  __builtin_linx_andn
#define RISCV_ANDSHF  __builtin_linx_andshf
#define RISCV_AUIPC  __builtin_linx_auipc
#define RISCV_C_ADD  __builtin_linx_c_add
#define RISCV_C_ADDI  __builtin_linx_c_addi
#define RISCV_C_AND  __builtin_linx_c_and
#define RISCV_C_ANDI  __builtin_linx_c_andi
#define RISCV_C_FLW  __builtin_linx_c_flw
#define RISCV_C_FSW  __builtin_linx_c_fsw
#define RISCV_C_LBU  __builtin_linx_c_lbu
#define RISCV_C_LHU  __builtin_linx_c_lhu
#define RISCV_C_LI  __builtin_linx_c_li
#define RISCV_C_LUI  __builtin_linx_c_lui
#define RISCV_C_LW  __builtin_linx_c_lw
#define RISCV_C_MARKER  __builtin_linx_c_marker
#define RISCV_C_MUL  __builtin_linx_c_mul
#define RISCV_C_MV  __builtin_linx_c_mv
#define RISCV_C_NEG  __builtin_linx_c_neg
#define RISCV_C_NOP  __builtin_linx_c_nop
#define RISCV_C_OR  __builtin_linx_c_or
#define RISCV_C_SB  __builtin_linx_c_sb
#define RISCV_C_SBZ  __builtin_linx_c_sbz
#define RISCV_C_SEXT_B  __builtin_linx_c_sext_b
#define RISCV_C_SEXT_H  __builtin_linx_c_sext_h
#define RISCV_C_SH  __builtin_linx_c_sh
#define RISCV_C_SHZ  __builtin_linx_c_shz
#define RISCV_C_SLLI  __builtin_linx_c_slli
#define RISCV_C_SRAI  __builtin_linx_c_srai
#define RISCV_C_SRLI  __builtin_linx_c_srli
#define RISCV_C_SUB  __builtin_linx_c_sub
#define RISCV_C_SW  __builtin_linx_c_sw
#define RISCV_C_SWZ  __builtin_linx_c_swz
#define RISCV_C_XOR  __builtin_linx_c_xor
#define RISCV_C_XORI  __builtin_linx_c_xori
#define RISCV_C_ZEXT_B  __builtin_linx_c_zext_b
#define RISCV_C_ZEXT_H  __builtin_linx_c_zext_h
#define RISCV_CLZ  __builtin_linx_clz
#define RISCV_CRC16_CCITT_B  __builtin_linx_crc16_ccitt_b
#define RISCV_CSRRC  __builtin_linx_csrrc
#define RISCV_CSRRC_DCSR(a)  __builtin_linx_csrrc(0b011110110000, a)
#define RISCV_CSRRC_DPC(a)  __builtin_linx_csrrc(0b011110110001, a)
#define RISCV_CSRRC_DSCRATCH0(a)  __builtin_linx_csrrc(0b011110110010, a)
#define RISCV_CSRRC_MVENDORID(a)  __builtin_linx_csrrc(0b111100010001, a)
#define RISCV_CSRRC_MARCHID(a)  __builtin_linx_csrrc(0b111100010010, a)
#define RISCV_CSRRC_MIMPID(a)  __builtin_linx_csrrc(0b111100010011, a)
#define RISCV_CSRRC_MHARTID(a)  __builtin_linx_csrrc(0b111100010100, a)
#define RISCV_CSRRC_MSTATUS(a)  __builtin_linx_csrrc(0b001100000000, a)
#define RISCV_CSRRC_MISA(a)  __builtin_linx_csrrc(0b001100000001, a)
#define RISCV_CSRRC_MIE(a)  __builtin_linx_csrrc(0b001100000100, a)
#define RISCV_CSRRC_MTVEC(a)  __builtin_linx_csrrc(0b001100000101, a)
#define RISCV_CSRRC_MIP(a)  __builtin_linx_csrrc(0b001101000100, a)
#define RISCV_CSRRC_MSECCFG(a)  __builtin_linx_csrrc(0b001110010000, a)
#define RISCV_CSRRC_MCOUNTEREN(a)  __builtin_linx_csrrc(0b001100000110, a)
#define RISCV_CSRRC_MCOUNTINHIBIT(a)  __builtin_linx_csrrc(0b001100100000, a)
#define RISCV_CSRRC_MSCRATCH(a)  __builtin_linx_csrrc(0b001101000000, a)
#define RISCV_CSRRC_MEPC(a)  __builtin_linx_csrrc(0b001101000001, a)
#define RISCV_CSRRC_MCAUSE(a)  __builtin_linx_csrrc(0b001101000010, a)
#define RISCV_CSRRC_MTVAL(a)  __builtin_linx_csrrc(0b001101000011, a)
#define RISCV_CSRRC_PMPCFG0(a)  __builtin_linx_csrrc(0b001110100000, a)
#define RISCV_CSRRC_PMPCFG1(a)  __builtin_linx_csrrc(0b001110100001, a)
#define RISCV_CSRRC_PMPCFG2(a)  __builtin_linx_csrrc(0b001110100010, a)
#define RISCV_CSRRC_PMPCFG3(a)  __builtin_linx_csrrc(0b001110100011, a)
#define RISCV_CSRRC_PMPCFG4(a)  __builtin_linx_csrrc(0b001110100100, a)
#define RISCV_CSRRC_PMPCFG5(a)  __builtin_linx_csrrc(0b001110100101, a)
#define RISCV_CSRRC_PMPCFG6(a)  __builtin_linx_csrrc(0b001110100110, a)
#define RISCV_CSRRC_PMPCFG7(a)  __builtin_linx_csrrc(0b001110100111, a)
#define RISCV_CSRRC_PMPCFG8(a)  __builtin_linx_csrrc(0b001110101000, a)
#define RISCV_CSRRC_PMPCFG9(a)  __builtin_linx_csrrc(0b001110101001, a)
#define RISCV_CSRRC_PMPCFG10(a)  __builtin_linx_csrrc(0b001110101010, a)
#define RISCV_CSRRC_PMPCFG11(a)  __builtin_linx_csrrc(0b001110101011, a)
#define RISCV_CSRRC_PMPCFG12(a)  __builtin_linx_csrrc(0b001110101100, a)
#define RISCV_CSRRC_PMPCFG13(a)  __builtin_linx_csrrc(0b001110101101, a)
#define RISCV_CSRRC_PMPCFG14(a)  __builtin_linx_csrrc(0b001110101110, a)
#define RISCV_CSRRC_PMPCFG15(a)  __builtin_linx_csrrc(0b001110101111, a)
#define RISCV_CSRRC_PMPADDR0(a)  __builtin_linx_csrrc(0b001110110000, a)
#define RISCV_CSRRC_PMPADDR1(a)  __builtin_linx_csrrc(0b001110110001, a)
#define RISCV_CSRRC_PMPADDR2(a)  __builtin_linx_csrrc(0b001110110010, a)
#define RISCV_CSRRC_PMPADDR3(a)  __builtin_linx_csrrc(0b001110110011, a)
#define RISCV_CSRRC_PMPADDR4(a)  __builtin_linx_csrrc(0b001110110100, a)
#define RISCV_CSRRC_PMPADDR5(a)  __builtin_linx_csrrc(0b001110110101, a)
#define RISCV_CSRRC_PMPADDR6(a)  __builtin_linx_csrrc(0b001110110110, a)
#define RISCV_CSRRC_PMPADDR7(a)  __builtin_linx_csrrc(0b001110110111, a)
#define RISCV_CSRRC_PMPADDR8(a)  __builtin_linx_csrrc(0b001110111000, a)
#define RISCV_CSRRC_PMPADDR9(a)  __builtin_linx_csrrc(0b001110111001, a)
#define RISCV_CSRRC_PMPADDR10(a)  __builtin_linx_csrrc(0b001110111010, a)
#define RISCV_CSRRC_PMPADDR11(a)  __builtin_linx_csrrc(0b001110111011, a)
#define RISCV_CSRRC_PMPADDR12(a)  __builtin_linx_csrrc(0b001110111100, a)
#define RISCV_CSRRC_PMPADDR13(a)  __builtin_linx_csrrc(0b001110111101, a)
#define RISCV_CSRRC_PMPADDR14(a)  __builtin_linx_csrrc(0b001110111110, a)
#define RISCV_CSRRC_PMPADDR15(a)  __builtin_linx_csrrc(0b001110111111, a)
#define RISCV_CSRRC_PMPADDR16(a)  __builtin_linx_csrrc(0b001111000000, a)
#define RISCV_CSRRC_PMPADDR17(a)  __builtin_linx_csrrc(0b001111000001, a)
#define RISCV_CSRRC_PMPADDR18(a)  __builtin_linx_csrrc(0b001111000010, a)
#define RISCV_CSRRC_PMPADDR19(a)  __builtin_linx_csrrc(0b001111000011, a)
#define RISCV_CSRRC_PMPADDR20(a)  __builtin_linx_csrrc(0b001111000100, a)
#define RISCV_CSRRC_PMPADDR21(a)  __builtin_linx_csrrc(0b001111000101, a)
#define RISCV_CSRRC_PMPADDR22(a)  __builtin_linx_csrrc(0b001111000110, a)
#define RISCV_CSRRC_PMPADDR23(a)  __builtin_linx_csrrc(0b001111000111, a)
#define RISCV_CSRRC_PMPADDR24(a)  __builtin_linx_csrrc(0b001111001000, a)
#define RISCV_CSRRC_PMPADDR25(a)  __builtin_linx_csrrc(0b001111001001, a)
#define RISCV_CSRRC_PMPADDR26(a)  __builtin_linx_csrrc(0b001111001010, a)
#define RISCV_CSRRC_PMPADDR27(a)  __builtin_linx_csrrc(0b001111001011, a)
#define RISCV_CSRRC_PMPADDR28(a)  __builtin_linx_csrrc(0b001111001100, a)
#define RISCV_CSRRC_PMPADDR29(a)  __builtin_linx_csrrc(0b001111001101, a)
#define RISCV_CSRRC_PMPADDR30(a)  __builtin_linx_csrrc(0b001111001110, a)
#define RISCV_CSRRC_PMPADDR31(a)  __builtin_linx_csrrc(0b001111001111, a)
#define RISCV_CSRRC_PMPADDR32(a)  __builtin_linx_csrrc(0b001111010000, a)
#define RISCV_CSRRC_PMPADDR33(a)  __builtin_linx_csrrc(0b001111010001, a)
#define RISCV_CSRRC_PMPADDR34(a)  __builtin_linx_csrrc(0b001111010010, a)
#define RISCV_CSRRC_PMPADDR35(a)  __builtin_linx_csrrc(0b001111010011, a)
#define RISCV_CSRRC_PMPADDR36(a)  __builtin_linx_csrrc(0b001111010100, a)
#define RISCV_CSRRC_PMPADDR37(a)  __builtin_linx_csrrc(0b001111010101, a)
#define RISCV_CSRRC_PMPADDR38(a)  __builtin_linx_csrrc(0b001111010110, a)
#define RISCV_CSRRC_PMPADDR39(a)  __builtin_linx_csrrc(0b001111010111, a)
#define RISCV_CSRRC_PMPADDR40(a)  __builtin_linx_csrrc(0b001111011000, a)
#define RISCV_CSRRC_PMPADDR41(a)  __builtin_linx_csrrc(0b001111011001, a)
#define RISCV_CSRRC_PMPADDR42(a)  __builtin_linx_csrrc(0b001111011010, a)
#define RISCV_CSRRC_PMPADDR43(a)  __builtin_linx_csrrc(0b001111011011, a)
#define RISCV_CSRRC_PMPADDR44(a)  __builtin_linx_csrrc(0b001111011100, a)
#define RISCV_CSRRC_PMPADDR45(a)  __builtin_linx_csrrc(0b001111011101, a)
#define RISCV_CSRRC_PMPADDR46(a)  __builtin_linx_csrrc(0b001111011110, a)
#define RISCV_CSRRC_PMPADDR47(a)  __builtin_linx_csrrc(0b001111011111, a)
#define RISCV_CSRRC_PMPADDR48(a)  __builtin_linx_csrrc(0b001111100000, a)
#define RISCV_CSRRC_PMPADDR49(a)  __builtin_linx_csrrc(0b001111100001, a)
#define RISCV_CSRRC_PMPADDR50(a)  __builtin_linx_csrrc(0b001111100010, a)
#define RISCV_CSRRC_PMPADDR51(a)  __builtin_linx_csrrc(0b001111100011, a)
#define RISCV_CSRRC_PMPADDR52(a)  __builtin_linx_csrrc(0b001111100100, a)
#define RISCV_CSRRC_PMPADDR53(a)  __builtin_linx_csrrc(0b001111100101, a)
#define RISCV_CSRRC_PMPADDR54(a)  __builtin_linx_csrrc(0b001111100110, a)
#define RISCV_CSRRC_PMPADDR55(a)  __builtin_linx_csrrc(0b001111100111, a)
#define RISCV_CSRRC_PMPADDR56(a)  __builtin_linx_csrrc(0b001111101000, a)
#define RISCV_CSRRC_PMPADDR57(a)  __builtin_linx_csrrc(0b001111101001, a)
#define RISCV_CSRRC_PMPADDR58(a)  __builtin_linx_csrrc(0b001111101010, a)
#define RISCV_CSRRC_PMPADDR59(a)  __builtin_linx_csrrc(0b001111101011, a)
#define RISCV_CSRRC_PMPADDR60(a)  __builtin_linx_csrrc(0b001111101100, a)
#define RISCV_CSRRC_PMPADDR61(a)  __builtin_linx_csrrc(0b001111101101, a)
#define RISCV_CSRRC_PMPADDR62(a)  __builtin_linx_csrrc(0b001111101110, a)
#define RISCV_CSRRC_PMPADDR63(a)  __builtin_linx_csrrc(0b001111101111, a)
#define RISCV_CSRRC_MTVT(a)  __builtin_linx_csrrc(0b001100000111, a)
#define RISCV_CSRRC_MINTSTATUS(a)  __builtin_linx_csrrc(0b001101000110, a)
#define RISCV_CSRRC_MINTTHRESH(a)  __builtin_linx_csrrc(0b001101000111, a)
#define RISCV_CSRRC_MSCRATCHCSW(a)  __builtin_linx_csrrc(0b001101001000, a)
#define RISCV_CSRRC_MSCRATCHCSWL(a)  __builtin_linx_csrrc(0b001101001001, a)
#define RISCV_CSRRC_MCLICBASE(a)  __builtin_linx_csrrc(0b111110111111, a)
#define RISCV_CSRRC_MHPMEVENT3(a)  __builtin_linx_csrrc(0b001100100011, a)
#define RISCV_CSRRC_MHPMEVENT4(a)  __builtin_linx_csrrc(0b001100100100, a)
#define RISCV_CSRRC_MHPMEVENT5(a)  __builtin_linx_csrrc(0b001100100101, a)
#define RISCV_CSRRC_MHPMEVENT6(a)  __builtin_linx_csrrc(0b001100100110, a)
#define RISCV_CSRRC_MHPMEVENT7(a)  __builtin_linx_csrrc(0b001100100111, a)
#define RISCV_CSRRC_MHPMEVENT8(a)  __builtin_linx_csrrc(0b001100101000, a)
#define RISCV_CSRRC_MHPMEVENT9(a)  __builtin_linx_csrrc(0b001100101001, a)
#define RISCV_CSRRC_MHPMEVENT10(a)  __builtin_linx_csrrc(0b001100101010, a)
#define RISCV_CSRRC_MHPMEVENT11(a)  __builtin_linx_csrrc(0b001100101011, a)
#define RISCV_CSRRC_MHPMEVENT12(a)  __builtin_linx_csrrc(0b001100101100, a)
#define RISCV_CSRRC_MHPMEVENT13(a)  __builtin_linx_csrrc(0b001100101101, a)
#define RISCV_CSRRC_MHPMEVENT14(a)  __builtin_linx_csrrc(0b001100101110, a)
#define RISCV_CSRRC_MHPMEVENT15(a)  __builtin_linx_csrrc(0b001100101111, a)
#define RISCV_CSRRC_MHPMEVENT16(a)  __builtin_linx_csrrc(0b001100110000, a)
#define RISCV_CSRRC_MHPMEVENT17(a)  __builtin_linx_csrrc(0b001100110001, a)
#define RISCV_CSRRC_MHPMEVENT18(a)  __builtin_linx_csrrc(0b001100110010, a)
#define RISCV_CSRRC_MHPMEVENT19(a)  __builtin_linx_csrrc(0b001100110011, a)
#define RISCV_CSRRC_MHPMEVENT20(a)  __builtin_linx_csrrc(0b001100110100, a)
#define RISCV_CSRRC_MHPMEVENT21(a)  __builtin_linx_csrrc(0b001100110101, a)
#define RISCV_CSRRC_MHPMEVENT22(a)  __builtin_linx_csrrc(0b001100110110, a)
#define RISCV_CSRRC_MHPMEVENT23(a)  __builtin_linx_csrrc(0b001100110111, a)
#define RISCV_CSRRC_MHPMEVENT24(a)  __builtin_linx_csrrc(0b001100111000, a)
#define RISCV_CSRRC_MHPMEVENT25(a)  __builtin_linx_csrrc(0b001100111001, a)
#define RISCV_CSRRC_MHPMEVENT26(a)  __builtin_linx_csrrc(0b001100111010, a)
#define RISCV_CSRRC_MHPMEVENT27(a)  __builtin_linx_csrrc(0b001100111011, a)
#define RISCV_CSRRC_MHPMEVENT28(a)  __builtin_linx_csrrc(0b001100111100, a)
#define RISCV_CSRRC_MHPMEVENT29(a)  __builtin_linx_csrrc(0b001100111101, a)
#define RISCV_CSRRC_MHPMEVENT30(a)  __builtin_linx_csrrc(0b001100111110, a)
#define RISCV_CSRRC_MHPMEVENT31(a)  __builtin_linx_csrrc(0b001100111111, a)
#define RISCV_CSRRC_MCYCLE(a)  __builtin_linx_csrrc(0b101100000000, a)
#define RISCV_CSRRC_MINSTRET(a)  __builtin_linx_csrrc(0b101100000010, a)
#define RISCV_CSRRC_MHPMCOUNTER3(a)  __builtin_linx_csrrc(0b101100000011, a)
#define RISCV_CSRRC_MHPMCOUNTER4(a)  __builtin_linx_csrrc(0b101100000100, a)
#define RISCV_CSRRC_MHPMCOUNTER5(a)  __builtin_linx_csrrc(0b101100000101, a)
#define RISCV_CSRRC_MHPMCOUNTER6(a)  __builtin_linx_csrrc(0b101100000110, a)
#define RISCV_CSRRC_MHPMCOUNTER7(a)  __builtin_linx_csrrc(0b101100000111, a)
#define RISCV_CSRRC_MHPMCOUNTER8(a)  __builtin_linx_csrrc(0b101100001000, a)
#define RISCV_CSRRC_MHPMCOUNTER9(a)  __builtin_linx_csrrc(0b101100001001, a)
#define RISCV_CSRRC_MHPMCOUNTER10(a)  __builtin_linx_csrrc(0b101100001010, a)
#define RISCV_CSRRC_MHPMCOUNTER11(a)  __builtin_linx_csrrc(0b101100001011, a)
#define RISCV_CSRRC_MHPMCOUNTER12(a)  __builtin_linx_csrrc(0b101100001100, a)
#define RISCV_CSRRC_MHPMCOUNTER13(a)  __builtin_linx_csrrc(0b101100001101, a)
#define RISCV_CSRRC_MHPMCOUNTER14(a)  __builtin_linx_csrrc(0b101100001110, a)
#define RISCV_CSRRC_MHPMCOUNTER15(a)  __builtin_linx_csrrc(0b101100001111, a)
#define RISCV_CSRRC_MHPMCOUNTER16(a)  __builtin_linx_csrrc(0b101100010000, a)
#define RISCV_CSRRC_MHPMCOUNTER17(a)  __builtin_linx_csrrc(0b101100010001, a)
#define RISCV_CSRRC_MHPMCOUNTER18(a)  __builtin_linx_csrrc(0b101100010010, a)
#define RISCV_CSRRC_MHPMCOUNTER19(a)  __builtin_linx_csrrc(0b101100010011, a)
#define RISCV_CSRRC_MHPMCOUNTER20(a)  __builtin_linx_csrrc(0b101100010100, a)
#define RISCV_CSRRC_MHPMCOUNTER21(a)  __builtin_linx_csrrc(0b101100010101, a)
#define RISCV_CSRRC_MHPMCOUNTER22(a)  __builtin_linx_csrrc(0b101100010110, a)
#define RISCV_CSRRC_MHPMCOUNTER23(a)  __builtin_linx_csrrc(0b101100010111, a)
#define RISCV_CSRRC_MHPMCOUNTER24(a)  __builtin_linx_csrrc(0b101100011000, a)
#define RISCV_CSRRC_MHPMCOUNTER25(a)  __builtin_linx_csrrc(0b101100011001, a)
#define RISCV_CSRRC_MHPMCOUNTER26(a)  __builtin_linx_csrrc(0b101100011010, a)
#define RISCV_CSRRC_MHPMCOUNTER27(a)  __builtin_linx_csrrc(0b101100011011, a)
#define RISCV_CSRRC_MHPMCOUNTER28(a)  __builtin_linx_csrrc(0b101100011100, a)
#define RISCV_CSRRC_MHPMCOUNTER29(a)  __builtin_linx_csrrc(0b101100011101, a)
#define RISCV_CSRRC_MHPMCOUNTER30(a)  __builtin_linx_csrrc(0b101100011110, a)
#define RISCV_CSRRC_MHPMCOUNTER31(a)  __builtin_linx_csrrc(0b101100011111, a)
#define RISCV_CSRRC_MCYCLEH(a)  __builtin_linx_csrrc(0b101110000000, a)
#define RISCV_CSRRC_MINSTRETH(a)  __builtin_linx_csrrc(0b101110000010, a)
#define RISCV_CSRRC_MHPMCOUNTER3H(a)  __builtin_linx_csrrc(0b101110000011, a)
#define RISCV_CSRRC_MHPMCOUNTER4H(a)  __builtin_linx_csrrc(0b101110000100, a)
#define RISCV_CSRRC_MHPMCOUNTER5H(a)  __builtin_linx_csrrc(0b101110000101, a)
#define RISCV_CSRRC_MHPMCOUNTER6H(a)  __builtin_linx_csrrc(0b101110000110, a)
#define RISCV_CSRRC_MHPMCOUNTER7H(a)  __builtin_linx_csrrc(0b101110000111, a)
#define RISCV_CSRRC_MHPMCOUNTER8H(a)  __builtin_linx_csrrc(0b101110001000, a)
#define RISCV_CSRRC_MHPMCOUNTER9H(a)  __builtin_linx_csrrc(0b101110001001, a)
#define RISCV_CSRRC_MHPMCOUNTER10H(a)  __builtin_linx_csrrc(0b101110001010, a)
#define RISCV_CSRRC_MHPMCOUNTER11H(a)  __builtin_linx_csrrc(0b101110001011, a)
#define RISCV_CSRRC_MHPMCOUNTER12H(a)  __builtin_linx_csrrc(0b101110001100, a)
#define RISCV_CSRRC_MHPMCOUNTER13H(a)  __builtin_linx_csrrc(0b101110001101, a)
#define RISCV_CSRRC_MHPMCOUNTER14H(a)  __builtin_linx_csrrc(0b101110001110, a)
#define RISCV_CSRRC_MHPMCOUNTER15H(a)  __builtin_linx_csrrc(0b101110001111, a)
#define RISCV_CSRRC_MHPMCOUNTER16H(a)  __builtin_linx_csrrc(0b101110010000, a)
#define RISCV_CSRRC_MHPMCOUNTER17H(a)  __builtin_linx_csrrc(0b101110010001, a)
#define RISCV_CSRRC_MHPMCOUNTER18H(a)  __builtin_linx_csrrc(0b101110010010, a)
#define RISCV_CSRRC_MHPMCOUNTER19H(a)  __builtin_linx_csrrc(0b101110010011, a)
#define RISCV_CSRRC_MHPMCOUNTER20H(a)  __builtin_linx_csrrc(0b101110010100, a)
#define RISCV_CSRRC_MHPMCOUNTER21H(a)  __builtin_linx_csrrc(0b101110010101, a)
#define RISCV_CSRRC_MHPMCOUNTER22H(a)  __builtin_linx_csrrc(0b101110010110, a)
#define RISCV_CSRRC_MHPMCOUNTER23H(a)  __builtin_linx_csrrc(0b101110010111, a)
#define RISCV_CSRRC_MHPMCOUNTER24H(a)  __builtin_linx_csrrc(0b101110011000, a)
#define RISCV_CSRRC_MHPMCOUNTER25H(a)  __builtin_linx_csrrc(0b101110011001, a)
#define RISCV_CSRRC_MHPMCOUNTER26H(a)  __builtin_linx_csrrc(0b101110011010, a)
#define RISCV_CSRRC_MHPMCOUNTER27H(a)  __builtin_linx_csrrc(0b101110011011, a)
#define RISCV_CSRRC_MHPMCOUNTER28H(a)  __builtin_linx_csrrc(0b101110011100, a)
#define RISCV_CSRRC_MHPMCOUNTER29H(a)  __builtin_linx_csrrc(0b101110011101, a)
#define RISCV_CSRRC_MHPMCOUNTER30H(a)  __builtin_linx_csrrc(0b101110011110, a)
#define RISCV_CSRRC_MHPMCOUNTER31H(a)  __builtin_linx_csrrc(0b101110011111, a)
#define RISCV_CSRRC_ICCTL(a)  __builtin_linx_csrrc(0b011111000000, a)
#define RISCV_CSRRC_DCCTL(a)  __builtin_linx_csrrc(0b011111000001, a)
#define RISCV_CSRRC_ICMAINT(a)  __builtin_linx_csrrc(0b011111000010, a)
#define RISCV_CSRRC_DCMAINT(a)  __builtin_linx_csrrc(0b011111000011, a)
#define RISCV_CSRRC_ICINVA(a)  __builtin_linx_csrrc(0b011111000100, a)
#define RISCV_CSRRC_DCINCVA(a)  __builtin_linx_csrrc(0b011111000101, a)
#define RISCV_CSRRC_APREFI(a)  __builtin_linx_csrrc(0b011111000110, a)
#define RISCV_CSRRC_APREFD(a)  __builtin_linx_csrrc(0b011111000111, a)
#define RISCV_CSRRC_WST(a)  __builtin_linx_csrrc(0b011111010000, a)
#define RISCV_CSRRC_TCMARB(a)  __builtin_linx_csrrc(0b011111010010, a)
#define RISCV_CSRRC_MEMATTR0(a)  __builtin_linx_csrrc(0b101111000000, a)
#define RISCV_CSRRC_MEMATTR1(a)  __builtin_linx_csrrc(0b101111000001, a)
#define RISCV_CSRRC_MEMATTR2(a)  __builtin_linx_csrrc(0b101111000010, a)
#define RISCV_CSRRC_MEMATTR3(a)  __builtin_linx_csrrc(0b101111000011, a)
#define RISCV_CSRRC_MEMATTR4(a)  __builtin_linx_csrrc(0b101111000100, a)
#define RISCV_CSRRC_MEMATTR5(a)  __builtin_linx_csrrc(0b101111000101, a)
#define RISCV_CSRRC_MEMATTR6(a)  __builtin_linx_csrrc(0b101111000110, a)
#define RISCV_CSRRC_MEMATTR7(a)  __builtin_linx_csrrc(0b101111000111, a)
#define RISCV_CSRRC_MEMATTR8(a)  __builtin_linx_csrrc(0b101111001000, a)
#define RISCV_CSRRC_MEMATTR9(a)  __builtin_linx_csrrc(0b101111001001, a)
#define RISCV_CSRRC_MEMATTR10(a)  __builtin_linx_csrrc(0b101111001010, a)
#define RISCV_CSRRC_MEMATTR11(a)  __builtin_linx_csrrc(0b101111001011, a)
#define RISCV_CSRRC_MEMATTR12(a)  __builtin_linx_csrrc(0b101111001100, a)
#define RISCV_CSRRC_MEMATTR13(a)  __builtin_linx_csrrc(0b101111001101, a)
#define RISCV_CSRRC_MEMATTR14(a)  __builtin_linx_csrrc(0b101111001110, a)
#define RISCV_CSRRC_MEMATTR15(a)  __builtin_linx_csrrc(0b101111001111, a)
#define RISCV_CSRRC_MEMATTR16(a)  __builtin_linx_csrrc(0b101111010000, a)
#define RISCV_CSRRC_MEMATTR17(a)  __builtin_linx_csrrc(0b101111010001, a)
#define RISCV_CSRRC_MEMATTR18(a)  __builtin_linx_csrrc(0b101111010010, a)
#define RISCV_CSRRC_MEMATTR19(a)  __builtin_linx_csrrc(0b101111010011, a)
#define RISCV_CSRRC_MEMATTR20(a)  __builtin_linx_csrrc(0b101111010100, a)
#define RISCV_CSRRC_MEMATTR21(a)  __builtin_linx_csrrc(0b101111010101, a)
#define RISCV_CSRRC_MEMATTR22(a)  __builtin_linx_csrrc(0b101111010110, a)
#define RISCV_CSRRC_MEMATTR23(a)  __builtin_linx_csrrc(0b101111010111, a)
#define RISCV_CSRRC_MEMATTR24(a)  __builtin_linx_csrrc(0b101111011000, a)
#define RISCV_CSRRC_MEMATTR25(a)  __builtin_linx_csrrc(0b101111011001, a)
#define RISCV_CSRRC_MEMATTR26(a)  __builtin_linx_csrrc(0b101111011010, a)
#define RISCV_CSRRC_MEMATTR27(a)  __builtin_linx_csrrc(0b101111011011, a)
#define RISCV_CSRRC_MEMATTR28(a)  __builtin_linx_csrrc(0b101111011100, a)
#define RISCV_CSRRC_MEMATTR29(a)  __builtin_linx_csrrc(0b101111011101, a)
#define RISCV_CSRRC_MEMATTR30(a)  __builtin_linx_csrrc(0b101111011110, a)
#define RISCV_CSRRC_MEMATTR31(a)  __builtin_linx_csrrc(0b101111011111, a)
#define RISCV_CSRRC_MEMATTR32(a)  __builtin_linx_csrrc(0b101111100000, a)
#define RISCV_CSRRC_MEMATTR33(a)  __builtin_linx_csrrc(0b101111100001, a)
#define RISCV_CSRRC_MEMATTR34(a)  __builtin_linx_csrrc(0b101111100010, a)
#define RISCV_CSRRC_MEMATTR35(a)  __builtin_linx_csrrc(0b101111100011, a)
#define RISCV_CSRRC_MEMATTR36(a)  __builtin_linx_csrrc(0b101111100100, a)
#define RISCV_CSRRC_MEMATTR37(a)  __builtin_linx_csrrc(0b101111100101, a)
#define RISCV_CSRRC_MEMATTR38(a)  __builtin_linx_csrrc(0b101111100110, a)
#define RISCV_CSRRC_MEMATTR39(a)  __builtin_linx_csrrc(0b101111100111, a)
#define RISCV_CSRRC_MEMATTR40(a)  __builtin_linx_csrrc(0b101111101000, a)
#define RISCV_CSRRC_MEMATTR41(a)  __builtin_linx_csrrc(0b101111101001, a)
#define RISCV_CSRRC_MEMATTR42(a)  __builtin_linx_csrrc(0b101111101010, a)
#define RISCV_CSRRC_MEMATTR43(a)  __builtin_linx_csrrc(0b101111101011, a)
#define RISCV_CSRRC_MEMATTR44(a)  __builtin_linx_csrrc(0b101111101100, a)
#define RISCV_CSRRC_MEMATTR45(a)  __builtin_linx_csrrc(0b101111101101, a)
#define RISCV_CSRRC_MEMATTR46(a)  __builtin_linx_csrrc(0b101111101110, a)
#define RISCV_CSRRC_MEMATTR47(a)  __builtin_linx_csrrc(0b101111101111, a)
#define RISCV_CSRRC_MEMATTR48(a)  __builtin_linx_csrrc(0b101111110000, a)
#define RISCV_CSRRC_MEMATTR49(a)  __builtin_linx_csrrc(0b101111110001, a)
#define RISCV_CSRRC_MEMATTR50(a)  __builtin_linx_csrrc(0b101111110010, a)
#define RISCV_CSRRC_MEMATTR51(a)  __builtin_linx_csrrc(0b101111110011, a)
#define RISCV_CSRRC_MEMATTR52(a)  __builtin_linx_csrrc(0b101111110100, a)
#define RISCV_CSRRC_MEMATTR53(a)  __builtin_linx_csrrc(0b101111110101, a)
#define RISCV_CSRRC_MEMATTR54(a)  __builtin_linx_csrrc(0b101111110110, a)
#define RISCV_CSRRC_MEMATTR55(a)  __builtin_linx_csrrc(0b101111110111, a)
#define RISCV_CSRRC_MEMATTR56(a)  __builtin_linx_csrrc(0b101111111000, a)
#define RISCV_CSRRC_MEMATTR57(a)  __builtin_linx_csrrc(0b101111111001, a)
#define RISCV_CSRRC_MEMATTR58(a)  __builtin_linx_csrrc(0b101111111010, a)
#define RISCV_CSRRC_MEMATTR59(a)  __builtin_linx_csrrc(0b101111111011, a)
#define RISCV_CSRRC_MEMATTR60(a)  __builtin_linx_csrrc(0b101111111100, a)
#define RISCV_CSRRC_MEMATTR61(a)  __builtin_linx_csrrc(0b101111111101, a)
#define RISCV_CSRRC_MEMATTR62(a)  __builtin_linx_csrrc(0b101111111110, a)
#define RISCV_CSRRC_MEMATTR63(a)  __builtin_linx_csrrc(0b101111111111, a)
#define RISCV_CSRRC_PORPC(a)  __builtin_linx_csrrc(0b111111000000, a)
#define RISCV_CSRRC_WTOPC(a)  __builtin_linx_csrrc(0b111111000001, a)
#define RISCV_CSRRC_CXCPTSC(a)  __builtin_linx_csrrc(0b111111000010, a)
#define RISCV_CSRRC_DBGUOPCNT(a)  __builtin_linx_csrrc(0b011111111110, a)
#define RISCV_CSRRC_FFLAGS(a)  __builtin_linx_csrrc(0b000000000001, a)
#define RISCV_CSRRC_FRM(a)  __builtin_linx_csrrc(0b000000000010, a)
#define RISCV_CSRRC_FCSR(a)  __builtin_linx_csrrc(0b000000000011, a)
#define RISCV_CSRRC_HPMCOUNTER3(a)  __builtin_linx_csrrc(0b110000000011, a)
#define RISCV_CSRRC_HPMCOUNTER4(a)  __builtin_linx_csrrc(0b110000000100, a)
#define RISCV_CSRRC_HPMCOUNTER5(a)  __builtin_linx_csrrc(0b110000000101, a)
#define RISCV_CSRRC_HPMCOUNTER6(a)  __builtin_linx_csrrc(0b110000000110, a)
#define RISCV_CSRRC_HPMCOUNTER7(a)  __builtin_linx_csrrc(0b110000000111, a)
#define RISCV_CSRRC_HPMCOUNTER8(a)  __builtin_linx_csrrc(0b110000001000, a)
#define RISCV_CSRRC_HPMCOUNTER9(a)  __builtin_linx_csrrc(0b110000001001, a)
#define RISCV_CSRRC_HPMCOUNTER10(a)  __builtin_linx_csrrc(0b110000001010, a)
#define RISCV_CSRRC_HPMCOUNTER11(a)  __builtin_linx_csrrc(0b110000001011, a)
#define RISCV_CSRRC_HPMCOUNTER12(a)  __builtin_linx_csrrc(0b110000001100, a)
#define RISCV_CSRRC_HPMCOUNTER13(a)  __builtin_linx_csrrc(0b110000001101, a)
#define RISCV_CSRRC_HPMCOUNTER14(a)  __builtin_linx_csrrc(0b110000001110, a)
#define RISCV_CSRRC_HPMCOUNTER15(a)  __builtin_linx_csrrc(0b110000001111, a)
#define RISCV_CSRRC_HPMCOUNTER16(a)  __builtin_linx_csrrc(0b110000010000, a)
#define RISCV_CSRRC_HPMCOUNTER17(a)  __builtin_linx_csrrc(0b110000010001, a)
#define RISCV_CSRRC_HPMCOUNTER18(a)  __builtin_linx_csrrc(0b110000010010, a)
#define RISCV_CSRRC_HPMCOUNTER19(a)  __builtin_linx_csrrc(0b110000010011, a)
#define RISCV_CSRRC_HPMCOUNTER20(a)  __builtin_linx_csrrc(0b110000010100, a)
#define RISCV_CSRRC_HPMCOUNTER21(a)  __builtin_linx_csrrc(0b110000010101, a)
#define RISCV_CSRRC_HPMCOUNTER22(a)  __builtin_linx_csrrc(0b110000010110, a)
#define RISCV_CSRRC_HPMCOUNTER23(a)  __builtin_linx_csrrc(0b110000010111, a)
#define RISCV_CSRRC_HPMCOUNTER24(a)  __builtin_linx_csrrc(0b110000011000, a)
#define RISCV_CSRRC_HPMCOUNTER25(a)  __builtin_linx_csrrc(0b110000011001, a)
#define RISCV_CSRRC_HPMCOUNTER26(a)  __builtin_linx_csrrc(0b110000011010, a)
#define RISCV_CSRRC_HPMCOUNTER27(a)  __builtin_linx_csrrc(0b110000011011, a)
#define RISCV_CSRRC_HPMCOUNTER28(a)  __builtin_linx_csrrc(0b110000011100, a)
#define RISCV_CSRRC_HPMCOUNTER29(a)  __builtin_linx_csrrc(0b110000011101, a)
#define RISCV_CSRRC_HPMCOUNTER30(a)  __builtin_linx_csrrc(0b110000011110, a)
#define RISCV_CSRRC_HPMCOUNTER31(a)  __builtin_linx_csrrc(0b110000011111, a)
#define RISCV_CSRRC_CYCLE(a)  __builtin_linx_csrrc(0b110000000000, a)
#define RISCV_CSRRC_TIME(a)  __builtin_linx_csrrc(0b110000000001, a)
#define RISCV_CSRRC_INSTRET(a)  __builtin_linx_csrrc(0b110000000010, a)
#define RISCV_CSRRC_HPMCOUNTER3H(a)  __builtin_linx_csrrc(0b110010000011, a)
#define RISCV_CSRRC_HPMCOUNTER4H(a)  __builtin_linx_csrrc(0b110010000100, a)
#define RISCV_CSRRC_HPMCOUNTER5H(a)  __builtin_linx_csrrc(0b110010000101, a)
#define RISCV_CSRRC_HPMCOUNTER6H(a)  __builtin_linx_csrrc(0b110010000110, a)
#define RISCV_CSRRC_HPMCOUNTER7H(a)  __builtin_linx_csrrc(0b110010000111, a)
#define RISCV_CSRRC_HPMCOUNTER8H(a)  __builtin_linx_csrrc(0b110010001000, a)
#define RISCV_CSRRC_HPMCOUNTER9H(a)  __builtin_linx_csrrc(0b110010001001, a)
#define RISCV_CSRRC_HPMCOUNTER10H(a)  __builtin_linx_csrrc(0b110010001010, a)
#define RISCV_CSRRC_HPMCOUNTER11H(a)  __builtin_linx_csrrc(0b110010001011, a)
#define RISCV_CSRRC_HPMCOUNTER12H(a)  __builtin_linx_csrrc(0b110010001100, a)
#define RISCV_CSRRC_HPMCOUNTER13H(a)  __builtin_linx_csrrc(0b110010001101, a)
#define RISCV_CSRRC_HPMCOUNTER14H(a)  __builtin_linx_csrrc(0b110010001110, a)
#define RISCV_CSRRC_HPMCOUNTER15H(a)  __builtin_linx_csrrc(0b110010001111, a)
#define RISCV_CSRRC_HPMCOUNTER16H(a)  __builtin_linx_csrrc(0b110010010000, a)
#define RISCV_CSRRC_HPMCOUNTER17H(a)  __builtin_linx_csrrc(0b110010010001, a)
#define RISCV_CSRRC_HPMCOUNTER18H(a)  __builtin_linx_csrrc(0b110010010010, a)
#define RISCV_CSRRC_HPMCOUNTER19H(a)  __builtin_linx_csrrc(0b110010010011, a)
#define RISCV_CSRRC_HPMCOUNTER20H(a)  __builtin_linx_csrrc(0b110010010100, a)
#define RISCV_CSRRC_HPMCOUNTER21H(a)  __builtin_linx_csrrc(0b110010010101, a)
#define RISCV_CSRRC_HPMCOUNTER22H(a)  __builtin_linx_csrrc(0b110010010110, a)
#define RISCV_CSRRC_HPMCOUNTER23H(a)  __builtin_linx_csrrc(0b110010010111, a)
#define RISCV_CSRRC_HPMCOUNTER24H(a)  __builtin_linx_csrrc(0b110010011000, a)
#define RISCV_CSRRC_HPMCOUNTER25H(a)  __builtin_linx_csrrc(0b110010011001, a)
#define RISCV_CSRRC_HPMCOUNTER26H(a)  __builtin_linx_csrrc(0b110010011010, a)
#define RISCV_CSRRC_HPMCOUNTER27H(a)  __builtin_linx_csrrc(0b110010011011, a)
#define RISCV_CSRRC_HPMCOUNTER28H(a)  __builtin_linx_csrrc(0b110010011100, a)
#define RISCV_CSRRC_HPMCOUNTER29H(a)  __builtin_linx_csrrc(0b110010011101, a)
#define RISCV_CSRRC_HPMCOUNTER30H(a)  __builtin_linx_csrrc(0b110010011110, a)
#define RISCV_CSRRC_HPMCOUNTER31H(a)  __builtin_linx_csrrc(0b110010011111, a)
#define RISCV_CSRRC_CYCLEH(a)  __builtin_linx_csrrc(0b110010000000, a)
#define RISCV_CSRRC_TIMEH(a)  __builtin_linx_csrrc(0b110010000001, a)
#define RISCV_CSRRC_INSTRETH(a)  __builtin_linx_csrrc(0b110010000010, a)
#define RISCV_CSRRC_TMESCR(a)  __builtin_linx_csrrc(0b011111100000, a)
#define RISCV_CSRRC_TMESVEC(a)  __builtin_linx_csrrc(0b011111100001, a)
#define RISCV_CSRRC_TMESTOP(a)  __builtin_linx_csrrc(0b011111100010, a)
#define RISCV_CSRRC_TMEDELEG(a)  __builtin_linx_csrrc(0b011111100011, a)
#define RISCV_CSRRC_TMTVEC(a)  __builtin_linx_csrrc(0b011111100100, a)
#define RISCV_CSRRC_TMTVT(a)  __builtin_linx_csrrc(0b011111100101, a)
#define RISCV_CSRRC_TMINTTHRESH(a)  __builtin_linx_csrrc(0b011111100110, a)
#define RISCV_CSRRC_TMSTATUS(a)  __builtin_linx_csrrc(0b011111100111, a)
#define RISCV_CSRRC_TMEPC(a)  __builtin_linx_csrrc(0b011111101000, a)
#define RISCV_CSRRC_TMCAUSE(a)  __builtin_linx_csrrc(0b011111101001, a)
#define RISCV_CSRRC_TCXCPTSC(a)  __builtin_linx_csrrc(0b111111000011, a)
#define RISCV_CSRRC_TMTVAL(a)  __builtin_linx_csrrc(0b011111101011, a)
#define RISCV_CSRRC_TMSCRATCH(a)  __builtin_linx_csrrc(0b011111101100, a)
#define RISCV_CSRRC_TMESEPR(a)  __builtin_linx_csrrc(0b011111101101, a)
#define RISCV_CSRRC_TMESEPS(a)  __builtin_linx_csrrc(0b011111101110, a)
#define RISCV_CSRRC_MSECCTL(a)  __builtin_linx_csrrc(0b011111101111, a)
#define RISCV_CSRRC_PMPTE0(a)  __builtin_linx_csrrc(0b011111110000, a)
#define RISCV_CSRRC_PMPTE1(a)  __builtin_linx_csrrc(0b011111110001, a)
#define RISCV_CSRRC_MSPLIMIT(a)  __builtin_linx_csrrc(0b011111110010, a)
#define RISCV_CSRRC_TMSPLIMIT(a)  __builtin_linx_csrrc(0b011111110011, a)
#define RISCV_CSRRC_TUSP(a)  __builtin_linx_csrrc(0b100000000000, a)
#define RISCV_CSRRC_TUGP(a)  __builtin_linx_csrrc(0b100000000001, a)
#define RISCV_CSRRC_TUTP(a)  __builtin_linx_csrrc(0b100000000010, a)
#define RISCV_CSRRC_TSP(a)  __builtin_linx_csrrc(0b100000000011, a)
#define RISCV_CSRRC_TGP(a)  __builtin_linx_csrrc(0b100000000100, a)
#define RISCV_CSRRC_TTP(a)  __builtin_linx_csrrc(0b100000000101, a)
#define RISCV_CSRRC_TESEPR(a)  __builtin_linx_csrrc(0b110011000000, a)
#define RISCV_CSRRC_TESEPS(a)  __builtin_linx_csrrc(0b110011000001, a)
#define RISCV_CSRRC_VXSAT(a)  __builtin_linx_csrrc(0b000000001001, a)
#define RISCV_CSRRC_VXRM(a)  __builtin_linx_csrrc(0b000000001010, a)
#define RISCV_CSRRC_VCSR(a)  __builtin_linx_csrrc(0b000000001111, a)
#define RISCV_CSRRC_VSTART(a)  __builtin_linx_csrrc(0b000000001000, a)
#define RISCV_CSRRC_VL(a)  __builtin_linx_csrrc(0b110000100000, a)
#define RISCV_CSRRC_VTYPE(a)  __builtin_linx_csrrc(0b110000100001, a)
#define RISCV_CSRRC_VLENB(a)  __builtin_linx_csrrc(0b110000100010, a)
#define RISCV_CSRRC_JBC(a)  __builtin_linx_csrrc(0b100111110000, a)
#define RISCV_CSRRC_JPC_UPPER(a)  __builtin_linx_csrrc(0b100111110001, a)
#define RISCV_CSRRC_JPC_LOWER(a)  __builtin_linx_csrrc(0b100111110010, a)
#define RISCV_CSRRC_JBCFT(a)  __builtin_linx_csrrc(0b100111110011, a)
#define RISCV_CSRRC_JBC_UPPER(a)  __builtin_linx_csrrc(0b100111110100, a)
#define RISCV_CSRRC_JBC_SHADOW(a)  __builtin_linx_csrrc(0b110011110000, a)
#define RISCV_CSRRC_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrc(0b100111111111, a)
#define RISCV_CSRRC_TSELECT(a)  __builtin_linx_csrrc(0b011110100000, a)
#define RISCV_CSRRC_TDATA1(a)  __builtin_linx_csrrc(0b011110100001, a)
#define RISCV_CSRRC_TDATA2(a)  __builtin_linx_csrrc(0b011110100010, a)
#define RISCV_CSRRC_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRC_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRC_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRC_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRC_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRC_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRC_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRC_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRC_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRC_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRC_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRC_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRC_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRC_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRC_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRC_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRC_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRC_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRC_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRC_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRC_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRC_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRC_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRC_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRC_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRC_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRC_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRC_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRC_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRC_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRC_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRC_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRC_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRC_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRC_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRC_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRC_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRC_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRC_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRC_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRC_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRC_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRC_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRC_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRC_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRC_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRC_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRC_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRC_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRC_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRC_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRC_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRC_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRC_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRC_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRC_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRC_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRC_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRC_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRC_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRC_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRC_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRC_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRC_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRC_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRC_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRC_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRC_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRC_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRC_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRC_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRC_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRC_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRC_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRC_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRC_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRC_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRC_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRC_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRC_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRC_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRC_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRCI  __builtin_linx_csrrci
#define RISCV_CSRRCI_DCSR(a)  __builtin_linx_csrrci(0b011110110000, a)
#define RISCV_CSRRCI_DPC(a)  __builtin_linx_csrrci(0b011110110001, a)
#define RISCV_CSRRCI_DSCRATCH0(a)  __builtin_linx_csrrci(0b011110110010, a)
#define RISCV_CSRRCI_MVENDORID(a)  __builtin_linx_csrrci(0b111100010001, a)
#define RISCV_CSRRCI_MARCHID(a)  __builtin_linx_csrrci(0b111100010010, a)
#define RISCV_CSRRCI_MIMPID(a)  __builtin_linx_csrrci(0b111100010011, a)
#define RISCV_CSRRCI_MHARTID(a)  __builtin_linx_csrrci(0b111100010100, a)
#define RISCV_CSRRCI_MSTATUS(a)  __builtin_linx_csrrci(0b001100000000, a)
#define RISCV_CSRRCI_MISA(a)  __builtin_linx_csrrci(0b001100000001, a)
#define RISCV_CSRRCI_MIE(a)  __builtin_linx_csrrci(0b001100000100, a)
#define RISCV_CSRRCI_MTVEC(a)  __builtin_linx_csrrci(0b001100000101, a)
#define RISCV_CSRRCI_MIP(a)  __builtin_linx_csrrci(0b001101000100, a)
#define RISCV_CSRRCI_MSECCFG(a)  __builtin_linx_csrrci(0b001110010000, a)
#define RISCV_CSRRCI_MCOUNTEREN(a)  __builtin_linx_csrrci(0b001100000110, a)
#define RISCV_CSRRCI_MCOUNTINHIBIT(a)  __builtin_linx_csrrci(0b001100100000, a)
#define RISCV_CSRRCI_MSCRATCH(a)  __builtin_linx_csrrci(0b001101000000, a)
#define RISCV_CSRRCI_MEPC(a)  __builtin_linx_csrrci(0b001101000001, a)
#define RISCV_CSRRCI_MCAUSE(a)  __builtin_linx_csrrci(0b001101000010, a)
#define RISCV_CSRRCI_MTVAL(a)  __builtin_linx_csrrci(0b001101000011, a)
#define RISCV_CSRRCI_PMPCFG0(a)  __builtin_linx_csrrci(0b001110100000, a)
#define RISCV_CSRRCI_PMPCFG1(a)  __builtin_linx_csrrci(0b001110100001, a)
#define RISCV_CSRRCI_PMPCFG2(a)  __builtin_linx_csrrci(0b001110100010, a)
#define RISCV_CSRRCI_PMPCFG3(a)  __builtin_linx_csrrci(0b001110100011, a)
#define RISCV_CSRRCI_PMPCFG4(a)  __builtin_linx_csrrci(0b001110100100, a)
#define RISCV_CSRRCI_PMPCFG5(a)  __builtin_linx_csrrci(0b001110100101, a)
#define RISCV_CSRRCI_PMPCFG6(a)  __builtin_linx_csrrci(0b001110100110, a)
#define RISCV_CSRRCI_PMPCFG7(a)  __builtin_linx_csrrci(0b001110100111, a)
#define RISCV_CSRRCI_PMPCFG8(a)  __builtin_linx_csrrci(0b001110101000, a)
#define RISCV_CSRRCI_PMPCFG9(a)  __builtin_linx_csrrci(0b001110101001, a)
#define RISCV_CSRRCI_PMPCFG10(a)  __builtin_linx_csrrci(0b001110101010, a)
#define RISCV_CSRRCI_PMPCFG11(a)  __builtin_linx_csrrci(0b001110101011, a)
#define RISCV_CSRRCI_PMPCFG12(a)  __builtin_linx_csrrci(0b001110101100, a)
#define RISCV_CSRRCI_PMPCFG13(a)  __builtin_linx_csrrci(0b001110101101, a)
#define RISCV_CSRRCI_PMPCFG14(a)  __builtin_linx_csrrci(0b001110101110, a)
#define RISCV_CSRRCI_PMPCFG15(a)  __builtin_linx_csrrci(0b001110101111, a)
#define RISCV_CSRRCI_PMPADDR0(a)  __builtin_linx_csrrci(0b001110110000, a)
#define RISCV_CSRRCI_PMPADDR1(a)  __builtin_linx_csrrci(0b001110110001, a)
#define RISCV_CSRRCI_PMPADDR2(a)  __builtin_linx_csrrci(0b001110110010, a)
#define RISCV_CSRRCI_PMPADDR3(a)  __builtin_linx_csrrci(0b001110110011, a)
#define RISCV_CSRRCI_PMPADDR4(a)  __builtin_linx_csrrci(0b001110110100, a)
#define RISCV_CSRRCI_PMPADDR5(a)  __builtin_linx_csrrci(0b001110110101, a)
#define RISCV_CSRRCI_PMPADDR6(a)  __builtin_linx_csrrci(0b001110110110, a)
#define RISCV_CSRRCI_PMPADDR7(a)  __builtin_linx_csrrci(0b001110110111, a)
#define RISCV_CSRRCI_PMPADDR8(a)  __builtin_linx_csrrci(0b001110111000, a)
#define RISCV_CSRRCI_PMPADDR9(a)  __builtin_linx_csrrci(0b001110111001, a)
#define RISCV_CSRRCI_PMPADDR10(a)  __builtin_linx_csrrci(0b001110111010, a)
#define RISCV_CSRRCI_PMPADDR11(a)  __builtin_linx_csrrci(0b001110111011, a)
#define RISCV_CSRRCI_PMPADDR12(a)  __builtin_linx_csrrci(0b001110111100, a)
#define RISCV_CSRRCI_PMPADDR13(a)  __builtin_linx_csrrci(0b001110111101, a)
#define RISCV_CSRRCI_PMPADDR14(a)  __builtin_linx_csrrci(0b001110111110, a)
#define RISCV_CSRRCI_PMPADDR15(a)  __builtin_linx_csrrci(0b001110111111, a)
#define RISCV_CSRRCI_PMPADDR16(a)  __builtin_linx_csrrci(0b001111000000, a)
#define RISCV_CSRRCI_PMPADDR17(a)  __builtin_linx_csrrci(0b001111000001, a)
#define RISCV_CSRRCI_PMPADDR18(a)  __builtin_linx_csrrci(0b001111000010, a)
#define RISCV_CSRRCI_PMPADDR19(a)  __builtin_linx_csrrci(0b001111000011, a)
#define RISCV_CSRRCI_PMPADDR20(a)  __builtin_linx_csrrci(0b001111000100, a)
#define RISCV_CSRRCI_PMPADDR21(a)  __builtin_linx_csrrci(0b001111000101, a)
#define RISCV_CSRRCI_PMPADDR22(a)  __builtin_linx_csrrci(0b001111000110, a)
#define RISCV_CSRRCI_PMPADDR23(a)  __builtin_linx_csrrci(0b001111000111, a)
#define RISCV_CSRRCI_PMPADDR24(a)  __builtin_linx_csrrci(0b001111001000, a)
#define RISCV_CSRRCI_PMPADDR25(a)  __builtin_linx_csrrci(0b001111001001, a)
#define RISCV_CSRRCI_PMPADDR26(a)  __builtin_linx_csrrci(0b001111001010, a)
#define RISCV_CSRRCI_PMPADDR27(a)  __builtin_linx_csrrci(0b001111001011, a)
#define RISCV_CSRRCI_PMPADDR28(a)  __builtin_linx_csrrci(0b001111001100, a)
#define RISCV_CSRRCI_PMPADDR29(a)  __builtin_linx_csrrci(0b001111001101, a)
#define RISCV_CSRRCI_PMPADDR30(a)  __builtin_linx_csrrci(0b001111001110, a)
#define RISCV_CSRRCI_PMPADDR31(a)  __builtin_linx_csrrci(0b001111001111, a)
#define RISCV_CSRRCI_PMPADDR32(a)  __builtin_linx_csrrci(0b001111010000, a)
#define RISCV_CSRRCI_PMPADDR33(a)  __builtin_linx_csrrci(0b001111010001, a)
#define RISCV_CSRRCI_PMPADDR34(a)  __builtin_linx_csrrci(0b001111010010, a)
#define RISCV_CSRRCI_PMPADDR35(a)  __builtin_linx_csrrci(0b001111010011, a)
#define RISCV_CSRRCI_PMPADDR36(a)  __builtin_linx_csrrci(0b001111010100, a)
#define RISCV_CSRRCI_PMPADDR37(a)  __builtin_linx_csrrci(0b001111010101, a)
#define RISCV_CSRRCI_PMPADDR38(a)  __builtin_linx_csrrci(0b001111010110, a)
#define RISCV_CSRRCI_PMPADDR39(a)  __builtin_linx_csrrci(0b001111010111, a)
#define RISCV_CSRRCI_PMPADDR40(a)  __builtin_linx_csrrci(0b001111011000, a)
#define RISCV_CSRRCI_PMPADDR41(a)  __builtin_linx_csrrci(0b001111011001, a)
#define RISCV_CSRRCI_PMPADDR42(a)  __builtin_linx_csrrci(0b001111011010, a)
#define RISCV_CSRRCI_PMPADDR43(a)  __builtin_linx_csrrci(0b001111011011, a)
#define RISCV_CSRRCI_PMPADDR44(a)  __builtin_linx_csrrci(0b001111011100, a)
#define RISCV_CSRRCI_PMPADDR45(a)  __builtin_linx_csrrci(0b001111011101, a)
#define RISCV_CSRRCI_PMPADDR46(a)  __builtin_linx_csrrci(0b001111011110, a)
#define RISCV_CSRRCI_PMPADDR47(a)  __builtin_linx_csrrci(0b001111011111, a)
#define RISCV_CSRRCI_PMPADDR48(a)  __builtin_linx_csrrci(0b001111100000, a)
#define RISCV_CSRRCI_PMPADDR49(a)  __builtin_linx_csrrci(0b001111100001, a)
#define RISCV_CSRRCI_PMPADDR50(a)  __builtin_linx_csrrci(0b001111100010, a)
#define RISCV_CSRRCI_PMPADDR51(a)  __builtin_linx_csrrci(0b001111100011, a)
#define RISCV_CSRRCI_PMPADDR52(a)  __builtin_linx_csrrci(0b001111100100, a)
#define RISCV_CSRRCI_PMPADDR53(a)  __builtin_linx_csrrci(0b001111100101, a)
#define RISCV_CSRRCI_PMPADDR54(a)  __builtin_linx_csrrci(0b001111100110, a)
#define RISCV_CSRRCI_PMPADDR55(a)  __builtin_linx_csrrci(0b001111100111, a)
#define RISCV_CSRRCI_PMPADDR56(a)  __builtin_linx_csrrci(0b001111101000, a)
#define RISCV_CSRRCI_PMPADDR57(a)  __builtin_linx_csrrci(0b001111101001, a)
#define RISCV_CSRRCI_PMPADDR58(a)  __builtin_linx_csrrci(0b001111101010, a)
#define RISCV_CSRRCI_PMPADDR59(a)  __builtin_linx_csrrci(0b001111101011, a)
#define RISCV_CSRRCI_PMPADDR60(a)  __builtin_linx_csrrci(0b001111101100, a)
#define RISCV_CSRRCI_PMPADDR61(a)  __builtin_linx_csrrci(0b001111101101, a)
#define RISCV_CSRRCI_PMPADDR62(a)  __builtin_linx_csrrci(0b001111101110, a)
#define RISCV_CSRRCI_PMPADDR63(a)  __builtin_linx_csrrci(0b001111101111, a)
#define RISCV_CSRRCI_MTVT(a)  __builtin_linx_csrrci(0b001100000111, a)
#define RISCV_CSRRCI_MNXTI(a)  __builtin_linx_csrrci(0b001101000101, a)
#define RISCV_CSRRCI_MINTSTATUS(a)  __builtin_linx_csrrci(0b001101000110, a)
#define RISCV_CSRRCI_MINTTHRESH(a)  __builtin_linx_csrrci(0b001101000111, a)
#define RISCV_CSRRCI_MSCRATCHCSW(a)  __builtin_linx_csrrci(0b001101001000, a)
#define RISCV_CSRRCI_MSCRATCHCSWL(a)  __builtin_linx_csrrci(0b001101001001, a)
#define RISCV_CSRRCI_MCLICBASE(a)  __builtin_linx_csrrci(0b111110111111, a)
#define RISCV_CSRRCI_MHPMEVENT3(a)  __builtin_linx_csrrci(0b001100100011, a)
#define RISCV_CSRRCI_MHPMEVENT4(a)  __builtin_linx_csrrci(0b001100100100, a)
#define RISCV_CSRRCI_MHPMEVENT5(a)  __builtin_linx_csrrci(0b001100100101, a)
#define RISCV_CSRRCI_MHPMEVENT6(a)  __builtin_linx_csrrci(0b001100100110, a)
#define RISCV_CSRRCI_MHPMEVENT7(a)  __builtin_linx_csrrci(0b001100100111, a)
#define RISCV_CSRRCI_MHPMEVENT8(a)  __builtin_linx_csrrci(0b001100101000, a)
#define RISCV_CSRRCI_MHPMEVENT9(a)  __builtin_linx_csrrci(0b001100101001, a)
#define RISCV_CSRRCI_MHPMEVENT10(a)  __builtin_linx_csrrci(0b001100101010, a)
#define RISCV_CSRRCI_MHPMEVENT11(a)  __builtin_linx_csrrci(0b001100101011, a)
#define RISCV_CSRRCI_MHPMEVENT12(a)  __builtin_linx_csrrci(0b001100101100, a)
#define RISCV_CSRRCI_MHPMEVENT13(a)  __builtin_linx_csrrci(0b001100101101, a)
#define RISCV_CSRRCI_MHPMEVENT14(a)  __builtin_linx_csrrci(0b001100101110, a)
#define RISCV_CSRRCI_MHPMEVENT15(a)  __builtin_linx_csrrci(0b001100101111, a)
#define RISCV_CSRRCI_MHPMEVENT16(a)  __builtin_linx_csrrci(0b001100110000, a)
#define RISCV_CSRRCI_MHPMEVENT17(a)  __builtin_linx_csrrci(0b001100110001, a)
#define RISCV_CSRRCI_MHPMEVENT18(a)  __builtin_linx_csrrci(0b001100110010, a)
#define RISCV_CSRRCI_MHPMEVENT19(a)  __builtin_linx_csrrci(0b001100110011, a)
#define RISCV_CSRRCI_MHPMEVENT20(a)  __builtin_linx_csrrci(0b001100110100, a)
#define RISCV_CSRRCI_MHPMEVENT21(a)  __builtin_linx_csrrci(0b001100110101, a)
#define RISCV_CSRRCI_MHPMEVENT22(a)  __builtin_linx_csrrci(0b001100110110, a)
#define RISCV_CSRRCI_MHPMEVENT23(a)  __builtin_linx_csrrci(0b001100110111, a)
#define RISCV_CSRRCI_MHPMEVENT24(a)  __builtin_linx_csrrci(0b001100111000, a)
#define RISCV_CSRRCI_MHPMEVENT25(a)  __builtin_linx_csrrci(0b001100111001, a)
#define RISCV_CSRRCI_MHPMEVENT26(a)  __builtin_linx_csrrci(0b001100111010, a)
#define RISCV_CSRRCI_MHPMEVENT27(a)  __builtin_linx_csrrci(0b001100111011, a)
#define RISCV_CSRRCI_MHPMEVENT28(a)  __builtin_linx_csrrci(0b001100111100, a)
#define RISCV_CSRRCI_MHPMEVENT29(a)  __builtin_linx_csrrci(0b001100111101, a)
#define RISCV_CSRRCI_MHPMEVENT30(a)  __builtin_linx_csrrci(0b001100111110, a)
#define RISCV_CSRRCI_MHPMEVENT31(a)  __builtin_linx_csrrci(0b001100111111, a)
#define RISCV_CSRRCI_MCYCLE(a)  __builtin_linx_csrrci(0b101100000000, a)
#define RISCV_CSRRCI_MINSTRET(a)  __builtin_linx_csrrci(0b101100000010, a)
#define RISCV_CSRRCI_MHPMCOUNTER3(a)  __builtin_linx_csrrci(0b101100000011, a)
#define RISCV_CSRRCI_MHPMCOUNTER4(a)  __builtin_linx_csrrci(0b101100000100, a)
#define RISCV_CSRRCI_MHPMCOUNTER5(a)  __builtin_linx_csrrci(0b101100000101, a)
#define RISCV_CSRRCI_MHPMCOUNTER6(a)  __builtin_linx_csrrci(0b101100000110, a)
#define RISCV_CSRRCI_MHPMCOUNTER7(a)  __builtin_linx_csrrci(0b101100000111, a)
#define RISCV_CSRRCI_MHPMCOUNTER8(a)  __builtin_linx_csrrci(0b101100001000, a)
#define RISCV_CSRRCI_MHPMCOUNTER9(a)  __builtin_linx_csrrci(0b101100001001, a)
#define RISCV_CSRRCI_MHPMCOUNTER10(a)  __builtin_linx_csrrci(0b101100001010, a)
#define RISCV_CSRRCI_MHPMCOUNTER11(a)  __builtin_linx_csrrci(0b101100001011, a)
#define RISCV_CSRRCI_MHPMCOUNTER12(a)  __builtin_linx_csrrci(0b101100001100, a)
#define RISCV_CSRRCI_MHPMCOUNTER13(a)  __builtin_linx_csrrci(0b101100001101, a)
#define RISCV_CSRRCI_MHPMCOUNTER14(a)  __builtin_linx_csrrci(0b101100001110, a)
#define RISCV_CSRRCI_MHPMCOUNTER15(a)  __builtin_linx_csrrci(0b101100001111, a)
#define RISCV_CSRRCI_MHPMCOUNTER16(a)  __builtin_linx_csrrci(0b101100010000, a)
#define RISCV_CSRRCI_MHPMCOUNTER17(a)  __builtin_linx_csrrci(0b101100010001, a)
#define RISCV_CSRRCI_MHPMCOUNTER18(a)  __builtin_linx_csrrci(0b101100010010, a)
#define RISCV_CSRRCI_MHPMCOUNTER19(a)  __builtin_linx_csrrci(0b101100010011, a)
#define RISCV_CSRRCI_MHPMCOUNTER20(a)  __builtin_linx_csrrci(0b101100010100, a)
#define RISCV_CSRRCI_MHPMCOUNTER21(a)  __builtin_linx_csrrci(0b101100010101, a)
#define RISCV_CSRRCI_MHPMCOUNTER22(a)  __builtin_linx_csrrci(0b101100010110, a)
#define RISCV_CSRRCI_MHPMCOUNTER23(a)  __builtin_linx_csrrci(0b101100010111, a)
#define RISCV_CSRRCI_MHPMCOUNTER24(a)  __builtin_linx_csrrci(0b101100011000, a)
#define RISCV_CSRRCI_MHPMCOUNTER25(a)  __builtin_linx_csrrci(0b101100011001, a)
#define RISCV_CSRRCI_MHPMCOUNTER26(a)  __builtin_linx_csrrci(0b101100011010, a)
#define RISCV_CSRRCI_MHPMCOUNTER27(a)  __builtin_linx_csrrci(0b101100011011, a)
#define RISCV_CSRRCI_MHPMCOUNTER28(a)  __builtin_linx_csrrci(0b101100011100, a)
#define RISCV_CSRRCI_MHPMCOUNTER29(a)  __builtin_linx_csrrci(0b101100011101, a)
#define RISCV_CSRRCI_MHPMCOUNTER30(a)  __builtin_linx_csrrci(0b101100011110, a)
#define RISCV_CSRRCI_MHPMCOUNTER31(a)  __builtin_linx_csrrci(0b101100011111, a)
#define RISCV_CSRRCI_MCYCLEH(a)  __builtin_linx_csrrci(0b101110000000, a)
#define RISCV_CSRRCI_MINSTRETH(a)  __builtin_linx_csrrci(0b101110000010, a)
#define RISCV_CSRRCI_MHPMCOUNTER3H(a)  __builtin_linx_csrrci(0b101110000011, a)
#define RISCV_CSRRCI_MHPMCOUNTER4H(a)  __builtin_linx_csrrci(0b101110000100, a)
#define RISCV_CSRRCI_MHPMCOUNTER5H(a)  __builtin_linx_csrrci(0b101110000101, a)
#define RISCV_CSRRCI_MHPMCOUNTER6H(a)  __builtin_linx_csrrci(0b101110000110, a)
#define RISCV_CSRRCI_MHPMCOUNTER7H(a)  __builtin_linx_csrrci(0b101110000111, a)
#define RISCV_CSRRCI_MHPMCOUNTER8H(a)  __builtin_linx_csrrci(0b101110001000, a)
#define RISCV_CSRRCI_MHPMCOUNTER9H(a)  __builtin_linx_csrrci(0b101110001001, a)
#define RISCV_CSRRCI_MHPMCOUNTER10H(a)  __builtin_linx_csrrci(0b101110001010, a)
#define RISCV_CSRRCI_MHPMCOUNTER11H(a)  __builtin_linx_csrrci(0b101110001011, a)
#define RISCV_CSRRCI_MHPMCOUNTER12H(a)  __builtin_linx_csrrci(0b101110001100, a)
#define RISCV_CSRRCI_MHPMCOUNTER13H(a)  __builtin_linx_csrrci(0b101110001101, a)
#define RISCV_CSRRCI_MHPMCOUNTER14H(a)  __builtin_linx_csrrci(0b101110001110, a)
#define RISCV_CSRRCI_MHPMCOUNTER15H(a)  __builtin_linx_csrrci(0b101110001111, a)
#define RISCV_CSRRCI_MHPMCOUNTER16H(a)  __builtin_linx_csrrci(0b101110010000, a)
#define RISCV_CSRRCI_MHPMCOUNTER17H(a)  __builtin_linx_csrrci(0b101110010001, a)
#define RISCV_CSRRCI_MHPMCOUNTER18H(a)  __builtin_linx_csrrci(0b101110010010, a)
#define RISCV_CSRRCI_MHPMCOUNTER19H(a)  __builtin_linx_csrrci(0b101110010011, a)
#define RISCV_CSRRCI_MHPMCOUNTER20H(a)  __builtin_linx_csrrci(0b101110010100, a)
#define RISCV_CSRRCI_MHPMCOUNTER21H(a)  __builtin_linx_csrrci(0b101110010101, a)
#define RISCV_CSRRCI_MHPMCOUNTER22H(a)  __builtin_linx_csrrci(0b101110010110, a)
#define RISCV_CSRRCI_MHPMCOUNTER23H(a)  __builtin_linx_csrrci(0b101110010111, a)
#define RISCV_CSRRCI_MHPMCOUNTER24H(a)  __builtin_linx_csrrci(0b101110011000, a)
#define RISCV_CSRRCI_MHPMCOUNTER25H(a)  __builtin_linx_csrrci(0b101110011001, a)
#define RISCV_CSRRCI_MHPMCOUNTER26H(a)  __builtin_linx_csrrci(0b101110011010, a)
#define RISCV_CSRRCI_MHPMCOUNTER27H(a)  __builtin_linx_csrrci(0b101110011011, a)
#define RISCV_CSRRCI_MHPMCOUNTER28H(a)  __builtin_linx_csrrci(0b101110011100, a)
#define RISCV_CSRRCI_MHPMCOUNTER29H(a)  __builtin_linx_csrrci(0b101110011101, a)
#define RISCV_CSRRCI_MHPMCOUNTER30H(a)  __builtin_linx_csrrci(0b101110011110, a)
#define RISCV_CSRRCI_MHPMCOUNTER31H(a)  __builtin_linx_csrrci(0b101110011111, a)
#define RISCV_CSRRCI_ICCTL(a)  __builtin_linx_csrrci(0b011111000000, a)
#define RISCV_CSRRCI_DCCTL(a)  __builtin_linx_csrrci(0b011111000001, a)
#define RISCV_CSRRCI_ICMAINT(a)  __builtin_linx_csrrci(0b011111000010, a)
#define RISCV_CSRRCI_DCMAINT(a)  __builtin_linx_csrrci(0b011111000011, a)
#define RISCV_CSRRCI_ICINVA(a)  __builtin_linx_csrrci(0b011111000100, a)
#define RISCV_CSRRCI_DCINCVA(a)  __builtin_linx_csrrci(0b011111000101, a)
#define RISCV_CSRRCI_APREFI(a)  __builtin_linx_csrrci(0b011111000110, a)
#define RISCV_CSRRCI_APREFD(a)  __builtin_linx_csrrci(0b011111000111, a)
#define RISCV_CSRRCI_WST(a)  __builtin_linx_csrrci(0b011111010000, a)
#define RISCV_CSRRCI_TCMARB(a)  __builtin_linx_csrrci(0b011111010010, a)
#define RISCV_CSRRCI_MEMATTR0(a)  __builtin_linx_csrrci(0b101111000000, a)
#define RISCV_CSRRCI_MEMATTR1(a)  __builtin_linx_csrrci(0b101111000001, a)
#define RISCV_CSRRCI_MEMATTR2(a)  __builtin_linx_csrrci(0b101111000010, a)
#define RISCV_CSRRCI_MEMATTR3(a)  __builtin_linx_csrrci(0b101111000011, a)
#define RISCV_CSRRCI_MEMATTR4(a)  __builtin_linx_csrrci(0b101111000100, a)
#define RISCV_CSRRCI_MEMATTR5(a)  __builtin_linx_csrrci(0b101111000101, a)
#define RISCV_CSRRCI_MEMATTR6(a)  __builtin_linx_csrrci(0b101111000110, a)
#define RISCV_CSRRCI_MEMATTR7(a)  __builtin_linx_csrrci(0b101111000111, a)
#define RISCV_CSRRCI_MEMATTR8(a)  __builtin_linx_csrrci(0b101111001000, a)
#define RISCV_CSRRCI_MEMATTR9(a)  __builtin_linx_csrrci(0b101111001001, a)
#define RISCV_CSRRCI_MEMATTR10(a)  __builtin_linx_csrrci(0b101111001010, a)
#define RISCV_CSRRCI_MEMATTR11(a)  __builtin_linx_csrrci(0b101111001011, a)
#define RISCV_CSRRCI_MEMATTR12(a)  __builtin_linx_csrrci(0b101111001100, a)
#define RISCV_CSRRCI_MEMATTR13(a)  __builtin_linx_csrrci(0b101111001101, a)
#define RISCV_CSRRCI_MEMATTR14(a)  __builtin_linx_csrrci(0b101111001110, a)
#define RISCV_CSRRCI_MEMATTR15(a)  __builtin_linx_csrrci(0b101111001111, a)
#define RISCV_CSRRCI_MEMATTR16(a)  __builtin_linx_csrrci(0b101111010000, a)
#define RISCV_CSRRCI_MEMATTR17(a)  __builtin_linx_csrrci(0b101111010001, a)
#define RISCV_CSRRCI_MEMATTR18(a)  __builtin_linx_csrrci(0b101111010010, a)
#define RISCV_CSRRCI_MEMATTR19(a)  __builtin_linx_csrrci(0b101111010011, a)
#define RISCV_CSRRCI_MEMATTR20(a)  __builtin_linx_csrrci(0b101111010100, a)
#define RISCV_CSRRCI_MEMATTR21(a)  __builtin_linx_csrrci(0b101111010101, a)
#define RISCV_CSRRCI_MEMATTR22(a)  __builtin_linx_csrrci(0b101111010110, a)
#define RISCV_CSRRCI_MEMATTR23(a)  __builtin_linx_csrrci(0b101111010111, a)
#define RISCV_CSRRCI_MEMATTR24(a)  __builtin_linx_csrrci(0b101111011000, a)
#define RISCV_CSRRCI_MEMATTR25(a)  __builtin_linx_csrrci(0b101111011001, a)
#define RISCV_CSRRCI_MEMATTR26(a)  __builtin_linx_csrrci(0b101111011010, a)
#define RISCV_CSRRCI_MEMATTR27(a)  __builtin_linx_csrrci(0b101111011011, a)
#define RISCV_CSRRCI_MEMATTR28(a)  __builtin_linx_csrrci(0b101111011100, a)
#define RISCV_CSRRCI_MEMATTR29(a)  __builtin_linx_csrrci(0b101111011101, a)
#define RISCV_CSRRCI_MEMATTR30(a)  __builtin_linx_csrrci(0b101111011110, a)
#define RISCV_CSRRCI_MEMATTR31(a)  __builtin_linx_csrrci(0b101111011111, a)
#define RISCV_CSRRCI_MEMATTR32(a)  __builtin_linx_csrrci(0b101111100000, a)
#define RISCV_CSRRCI_MEMATTR33(a)  __builtin_linx_csrrci(0b101111100001, a)
#define RISCV_CSRRCI_MEMATTR34(a)  __builtin_linx_csrrci(0b101111100010, a)
#define RISCV_CSRRCI_MEMATTR35(a)  __builtin_linx_csrrci(0b101111100011, a)
#define RISCV_CSRRCI_MEMATTR36(a)  __builtin_linx_csrrci(0b101111100100, a)
#define RISCV_CSRRCI_MEMATTR37(a)  __builtin_linx_csrrci(0b101111100101, a)
#define RISCV_CSRRCI_MEMATTR38(a)  __builtin_linx_csrrci(0b101111100110, a)
#define RISCV_CSRRCI_MEMATTR39(a)  __builtin_linx_csrrci(0b101111100111, a)
#define RISCV_CSRRCI_MEMATTR40(a)  __builtin_linx_csrrci(0b101111101000, a)
#define RISCV_CSRRCI_MEMATTR41(a)  __builtin_linx_csrrci(0b101111101001, a)
#define RISCV_CSRRCI_MEMATTR42(a)  __builtin_linx_csrrci(0b101111101010, a)
#define RISCV_CSRRCI_MEMATTR43(a)  __builtin_linx_csrrci(0b101111101011, a)
#define RISCV_CSRRCI_MEMATTR44(a)  __builtin_linx_csrrci(0b101111101100, a)
#define RISCV_CSRRCI_MEMATTR45(a)  __builtin_linx_csrrci(0b101111101101, a)
#define RISCV_CSRRCI_MEMATTR46(a)  __builtin_linx_csrrci(0b101111101110, a)
#define RISCV_CSRRCI_MEMATTR47(a)  __builtin_linx_csrrci(0b101111101111, a)
#define RISCV_CSRRCI_MEMATTR48(a)  __builtin_linx_csrrci(0b101111110000, a)
#define RISCV_CSRRCI_MEMATTR49(a)  __builtin_linx_csrrci(0b101111110001, a)
#define RISCV_CSRRCI_MEMATTR50(a)  __builtin_linx_csrrci(0b101111110010, a)
#define RISCV_CSRRCI_MEMATTR51(a)  __builtin_linx_csrrci(0b101111110011, a)
#define RISCV_CSRRCI_MEMATTR52(a)  __builtin_linx_csrrci(0b101111110100, a)
#define RISCV_CSRRCI_MEMATTR53(a)  __builtin_linx_csrrci(0b101111110101, a)
#define RISCV_CSRRCI_MEMATTR54(a)  __builtin_linx_csrrci(0b101111110110, a)
#define RISCV_CSRRCI_MEMATTR55(a)  __builtin_linx_csrrci(0b101111110111, a)
#define RISCV_CSRRCI_MEMATTR56(a)  __builtin_linx_csrrci(0b101111111000, a)
#define RISCV_CSRRCI_MEMATTR57(a)  __builtin_linx_csrrci(0b101111111001, a)
#define RISCV_CSRRCI_MEMATTR58(a)  __builtin_linx_csrrci(0b101111111010, a)
#define RISCV_CSRRCI_MEMATTR59(a)  __builtin_linx_csrrci(0b101111111011, a)
#define RISCV_CSRRCI_MEMATTR60(a)  __builtin_linx_csrrci(0b101111111100, a)
#define RISCV_CSRRCI_MEMATTR61(a)  __builtin_linx_csrrci(0b101111111101, a)
#define RISCV_CSRRCI_MEMATTR62(a)  __builtin_linx_csrrci(0b101111111110, a)
#define RISCV_CSRRCI_MEMATTR63(a)  __builtin_linx_csrrci(0b101111111111, a)
#define RISCV_CSRRCI_PORPC(a)  __builtin_linx_csrrci(0b111111000000, a)
#define RISCV_CSRRCI_WTOPC(a)  __builtin_linx_csrrci(0b111111000001, a)
#define RISCV_CSRRCI_CXCPTSC(a)  __builtin_linx_csrrci(0b111111000010, a)
#define RISCV_CSRRCI_DBGUOPCNT(a)  __builtin_linx_csrrci(0b011111111110, a)
#define RISCV_CSRRCI_FFLAGS(a)  __builtin_linx_csrrci(0b000000000001, a)
#define RISCV_CSRRCI_FRM(a)  __builtin_linx_csrrci(0b000000000010, a)
#define RISCV_CSRRCI_FCSR(a)  __builtin_linx_csrrci(0b000000000011, a)
#define RISCV_CSRRCI_HPMCOUNTER3(a)  __builtin_linx_csrrci(0b110000000011, a)
#define RISCV_CSRRCI_HPMCOUNTER4(a)  __builtin_linx_csrrci(0b110000000100, a)
#define RISCV_CSRRCI_HPMCOUNTER5(a)  __builtin_linx_csrrci(0b110000000101, a)
#define RISCV_CSRRCI_HPMCOUNTER6(a)  __builtin_linx_csrrci(0b110000000110, a)
#define RISCV_CSRRCI_HPMCOUNTER7(a)  __builtin_linx_csrrci(0b110000000111, a)
#define RISCV_CSRRCI_HPMCOUNTER8(a)  __builtin_linx_csrrci(0b110000001000, a)
#define RISCV_CSRRCI_HPMCOUNTER9(a)  __builtin_linx_csrrci(0b110000001001, a)
#define RISCV_CSRRCI_HPMCOUNTER10(a)  __builtin_linx_csrrci(0b110000001010, a)
#define RISCV_CSRRCI_HPMCOUNTER11(a)  __builtin_linx_csrrci(0b110000001011, a)
#define RISCV_CSRRCI_HPMCOUNTER12(a)  __builtin_linx_csrrci(0b110000001100, a)
#define RISCV_CSRRCI_HPMCOUNTER13(a)  __builtin_linx_csrrci(0b110000001101, a)
#define RISCV_CSRRCI_HPMCOUNTER14(a)  __builtin_linx_csrrci(0b110000001110, a)
#define RISCV_CSRRCI_HPMCOUNTER15(a)  __builtin_linx_csrrci(0b110000001111, a)
#define RISCV_CSRRCI_HPMCOUNTER16(a)  __builtin_linx_csrrci(0b110000010000, a)
#define RISCV_CSRRCI_HPMCOUNTER17(a)  __builtin_linx_csrrci(0b110000010001, a)
#define RISCV_CSRRCI_HPMCOUNTER18(a)  __builtin_linx_csrrci(0b110000010010, a)
#define RISCV_CSRRCI_HPMCOUNTER19(a)  __builtin_linx_csrrci(0b110000010011, a)
#define RISCV_CSRRCI_HPMCOUNTER20(a)  __builtin_linx_csrrci(0b110000010100, a)
#define RISCV_CSRRCI_HPMCOUNTER21(a)  __builtin_linx_csrrci(0b110000010101, a)
#define RISCV_CSRRCI_HPMCOUNTER22(a)  __builtin_linx_csrrci(0b110000010110, a)
#define RISCV_CSRRCI_HPMCOUNTER23(a)  __builtin_linx_csrrci(0b110000010111, a)
#define RISCV_CSRRCI_HPMCOUNTER24(a)  __builtin_linx_csrrci(0b110000011000, a)
#define RISCV_CSRRCI_HPMCOUNTER25(a)  __builtin_linx_csrrci(0b110000011001, a)
#define RISCV_CSRRCI_HPMCOUNTER26(a)  __builtin_linx_csrrci(0b110000011010, a)
#define RISCV_CSRRCI_HPMCOUNTER27(a)  __builtin_linx_csrrci(0b110000011011, a)
#define RISCV_CSRRCI_HPMCOUNTER28(a)  __builtin_linx_csrrci(0b110000011100, a)
#define RISCV_CSRRCI_HPMCOUNTER29(a)  __builtin_linx_csrrci(0b110000011101, a)
#define RISCV_CSRRCI_HPMCOUNTER30(a)  __builtin_linx_csrrci(0b110000011110, a)
#define RISCV_CSRRCI_HPMCOUNTER31(a)  __builtin_linx_csrrci(0b110000011111, a)
#define RISCV_CSRRCI_CYCLE(a)  __builtin_linx_csrrci(0b110000000000, a)
#define RISCV_CSRRCI_TIME(a)  __builtin_linx_csrrci(0b110000000001, a)
#define RISCV_CSRRCI_INSTRET(a)  __builtin_linx_csrrci(0b110000000010, a)
#define RISCV_CSRRCI_HPMCOUNTER3H(a)  __builtin_linx_csrrci(0b110010000011, a)
#define RISCV_CSRRCI_HPMCOUNTER4H(a)  __builtin_linx_csrrci(0b110010000100, a)
#define RISCV_CSRRCI_HPMCOUNTER5H(a)  __builtin_linx_csrrci(0b110010000101, a)
#define RISCV_CSRRCI_HPMCOUNTER6H(a)  __builtin_linx_csrrci(0b110010000110, a)
#define RISCV_CSRRCI_HPMCOUNTER7H(a)  __builtin_linx_csrrci(0b110010000111, a)
#define RISCV_CSRRCI_HPMCOUNTER8H(a)  __builtin_linx_csrrci(0b110010001000, a)
#define RISCV_CSRRCI_HPMCOUNTER9H(a)  __builtin_linx_csrrci(0b110010001001, a)
#define RISCV_CSRRCI_HPMCOUNTER10H(a)  __builtin_linx_csrrci(0b110010001010, a)
#define RISCV_CSRRCI_HPMCOUNTER11H(a)  __builtin_linx_csrrci(0b110010001011, a)
#define RISCV_CSRRCI_HPMCOUNTER12H(a)  __builtin_linx_csrrci(0b110010001100, a)
#define RISCV_CSRRCI_HPMCOUNTER13H(a)  __builtin_linx_csrrci(0b110010001101, a)
#define RISCV_CSRRCI_HPMCOUNTER14H(a)  __builtin_linx_csrrci(0b110010001110, a)
#define RISCV_CSRRCI_HPMCOUNTER15H(a)  __builtin_linx_csrrci(0b110010001111, a)
#define RISCV_CSRRCI_HPMCOUNTER16H(a)  __builtin_linx_csrrci(0b110010010000, a)
#define RISCV_CSRRCI_HPMCOUNTER17H(a)  __builtin_linx_csrrci(0b110010010001, a)
#define RISCV_CSRRCI_HPMCOUNTER18H(a)  __builtin_linx_csrrci(0b110010010010, a)
#define RISCV_CSRRCI_HPMCOUNTER19H(a)  __builtin_linx_csrrci(0b110010010011, a)
#define RISCV_CSRRCI_HPMCOUNTER20H(a)  __builtin_linx_csrrci(0b110010010100, a)
#define RISCV_CSRRCI_HPMCOUNTER21H(a)  __builtin_linx_csrrci(0b110010010101, a)
#define RISCV_CSRRCI_HPMCOUNTER22H(a)  __builtin_linx_csrrci(0b110010010110, a)
#define RISCV_CSRRCI_HPMCOUNTER23H(a)  __builtin_linx_csrrci(0b110010010111, a)
#define RISCV_CSRRCI_HPMCOUNTER24H(a)  __builtin_linx_csrrci(0b110010011000, a)
#define RISCV_CSRRCI_HPMCOUNTER25H(a)  __builtin_linx_csrrci(0b110010011001, a)
#define RISCV_CSRRCI_HPMCOUNTER26H(a)  __builtin_linx_csrrci(0b110010011010, a)
#define RISCV_CSRRCI_HPMCOUNTER27H(a)  __builtin_linx_csrrci(0b110010011011, a)
#define RISCV_CSRRCI_HPMCOUNTER28H(a)  __builtin_linx_csrrci(0b110010011100, a)
#define RISCV_CSRRCI_HPMCOUNTER29H(a)  __builtin_linx_csrrci(0b110010011101, a)
#define RISCV_CSRRCI_HPMCOUNTER30H(a)  __builtin_linx_csrrci(0b110010011110, a)
#define RISCV_CSRRCI_HPMCOUNTER31H(a)  __builtin_linx_csrrci(0b110010011111, a)
#define RISCV_CSRRCI_CYCLEH(a)  __builtin_linx_csrrci(0b110010000000, a)
#define RISCV_CSRRCI_TIMEH(a)  __builtin_linx_csrrci(0b110010000001, a)
#define RISCV_CSRRCI_INSTRETH(a)  __builtin_linx_csrrci(0b110010000010, a)
#define RISCV_CSRRCI_TMESCR(a)  __builtin_linx_csrrci(0b011111100000, a)
#define RISCV_CSRRCI_TMESVEC(a)  __builtin_linx_csrrci(0b011111100001, a)
#define RISCV_CSRRCI_TMESTOP(a)  __builtin_linx_csrrci(0b011111100010, a)
#define RISCV_CSRRCI_TMEDELEG(a)  __builtin_linx_csrrci(0b011111100011, a)
#define RISCV_CSRRCI_TMTVEC(a)  __builtin_linx_csrrci(0b011111100100, a)
#define RISCV_CSRRCI_TMTVT(a)  __builtin_linx_csrrci(0b011111100101, a)
#define RISCV_CSRRCI_TMINTTHRESH(a)  __builtin_linx_csrrci(0b011111100110, a)
#define RISCV_CSRRCI_TMSTATUS(a)  __builtin_linx_csrrci(0b011111100111, a)
#define RISCV_CSRRCI_TMEPC(a)  __builtin_linx_csrrci(0b011111101000, a)
#define RISCV_CSRRCI_TMCAUSE(a)  __builtin_linx_csrrci(0b011111101001, a)
#define RISCV_CSRRCI_TCXCPTSC(a)  __builtin_linx_csrrci(0b111111000011, a)
#define RISCV_CSRRCI_TMTVAL(a)  __builtin_linx_csrrci(0b011111101011, a)
#define RISCV_CSRRCI_TMSCRATCH(a)  __builtin_linx_csrrci(0b011111101100, a)
#define RISCV_CSRRCI_TMESEPR(a)  __builtin_linx_csrrci(0b011111101101, a)
#define RISCV_CSRRCI_TMESEPS(a)  __builtin_linx_csrrci(0b011111101110, a)
#define RISCV_CSRRCI_MSECCTL(a)  __builtin_linx_csrrci(0b011111101111, a)
#define RISCV_CSRRCI_PMPTE0(a)  __builtin_linx_csrrci(0b011111110000, a)
#define RISCV_CSRRCI_PMPTE1(a)  __builtin_linx_csrrci(0b011111110001, a)
#define RISCV_CSRRCI_MSPLIMIT(a)  __builtin_linx_csrrci(0b011111110010, a)
#define RISCV_CSRRCI_TMSPLIMIT(a)  __builtin_linx_csrrci(0b011111110011, a)
#define RISCV_CSRRCI_TUSP(a)  __builtin_linx_csrrci(0b100000000000, a)
#define RISCV_CSRRCI_TUGP(a)  __builtin_linx_csrrci(0b100000000001, a)
#define RISCV_CSRRCI_TUTP(a)  __builtin_linx_csrrci(0b100000000010, a)
#define RISCV_CSRRCI_TSP(a)  __builtin_linx_csrrci(0b100000000011, a)
#define RISCV_CSRRCI_TGP(a)  __builtin_linx_csrrci(0b100000000100, a)
#define RISCV_CSRRCI_TTP(a)  __builtin_linx_csrrci(0b100000000101, a)
#define RISCV_CSRRCI_TESEPR(a)  __builtin_linx_csrrci(0b110011000000, a)
#define RISCV_CSRRCI_TESEPS(a)  __builtin_linx_csrrci(0b110011000001, a)
#define RISCV_CSRRCI_VXSAT(a)  __builtin_linx_csrrci(0b000000001001, a)
#define RISCV_CSRRCI_VXRM(a)  __builtin_linx_csrrci(0b000000001010, a)
#define RISCV_CSRRCI_VCSR(a)  __builtin_linx_csrrci(0b000000001111, a)
#define RISCV_CSRRCI_VSTART(a)  __builtin_linx_csrrci(0b000000001000, a)
#define RISCV_CSRRCI_VL(a)  __builtin_linx_csrrci(0b110000100000, a)
#define RISCV_CSRRCI_VTYPE(a)  __builtin_linx_csrrci(0b110000100001, a)
#define RISCV_CSRRCI_VLENB(a)  __builtin_linx_csrrci(0b110000100010, a)
#define RISCV_CSRRCI_JBC(a)  __builtin_linx_csrrci(0b100111110000, a)
#define RISCV_CSRRCI_JPC_UPPER(a)  __builtin_linx_csrrci(0b100111110001, a)
#define RISCV_CSRRCI_JPC_LOWER(a)  __builtin_linx_csrrci(0b100111110010, a)
#define RISCV_CSRRCI_JBCFT(a)  __builtin_linx_csrrci(0b100111110011, a)
#define RISCV_CSRRCI_JBC_UPPER(a)  __builtin_linx_csrrci(0b100111110100, a)
#define RISCV_CSRRCI_JBC_SHADOW(a)  __builtin_linx_csrrci(0b110011110000, a)
#define RISCV_CSRRCI_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrci(0b100111111111, a)
#define RISCV_CSRRCI_TSELECT(a)  __builtin_linx_csrrci(0b011110100000, a)
#define RISCV_CSRRCI_TDATA1(a)  __builtin_linx_csrrci(0b011110100001, a)
#define RISCV_CSRRCI_TDATA2(a)  __builtin_linx_csrrci(0b011110100010, a)
#define RISCV_CSRRCI_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRCI_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRCI_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRCI_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRCI_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRCI_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRCI_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRCI_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRCI_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRCI_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRCI_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRCI_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRCI_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRCI_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRCI_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRCI_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRCI_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRCI_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRCI_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRCI_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRCI_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRCI_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRCI_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRCI_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRCI_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRCI_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRCI_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRCI_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRCI_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRCI_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRCI_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRCI_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRCI_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRCI_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRCI_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRCI_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRCI_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRCI_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRCI_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRCI_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRCI_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRCI_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRCI_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRCI_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRCI_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRCI_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRCI_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRCI_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRCI_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRCI_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRCI_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRCI_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRCI_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRCI_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRCI_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRCI_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRCI_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRCI_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRCI_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRCI_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRCI_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRCI_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRCI_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRCI_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRCI_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRCI_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRCI_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRCI_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRCI_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRCI_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRCI_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRCI_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRCI_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRCI_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRCI_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRCI_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRCI_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRCI_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRCI_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRCI_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRCI_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRCI_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRS  __builtin_linx_csrrs
#define RISCV_CSRRS_DCSR(a)  __builtin_linx_csrrs(0b011110110000, a)
#define RISCV_CSRRS_DPC(a)  __builtin_linx_csrrs(0b011110110001, a)
#define RISCV_CSRRS_DSCRATCH0(a)  __builtin_linx_csrrs(0b011110110010, a)
#define RISCV_CSRRS_MVENDORID(a)  __builtin_linx_csrrs(0b111100010001, a)
#define RISCV_CSRRS_MARCHID(a)  __builtin_linx_csrrs(0b111100010010, a)
#define RISCV_CSRRS_MIMPID(a)  __builtin_linx_csrrs(0b111100010011, a)
#define RISCV_CSRRS_MHARTID(a)  __builtin_linx_csrrs(0b111100010100, a)
#define RISCV_CSRRS_MSTATUS(a)  __builtin_linx_csrrs(0b001100000000, a)
#define RISCV_CSRRS_MISA(a)  __builtin_linx_csrrs(0b001100000001, a)
#define RISCV_CSRRS_MIE(a)  __builtin_linx_csrrs(0b001100000100, a)
#define RISCV_CSRRS_MTVEC(a)  __builtin_linx_csrrs(0b001100000101, a)
#define RISCV_CSRRS_MIP(a)  __builtin_linx_csrrs(0b001101000100, a)
#define RISCV_CSRRS_MSECCFG(a)  __builtin_linx_csrrs(0b001110010000, a)
#define RISCV_CSRRS_MCOUNTEREN(a)  __builtin_linx_csrrs(0b001100000110, a)
#define RISCV_CSRRS_MCOUNTINHIBIT(a)  __builtin_linx_csrrs(0b001100100000, a)
#define RISCV_CSRRS_MSCRATCH(a)  __builtin_linx_csrrs(0b001101000000, a)
#define RISCV_CSRRS_MEPC(a)  __builtin_linx_csrrs(0b001101000001, a)
#define RISCV_CSRRS_MCAUSE(a)  __builtin_linx_csrrs(0b001101000010, a)
#define RISCV_CSRRS_MTVAL(a)  __builtin_linx_csrrs(0b001101000011, a)
#define RISCV_CSRRS_PMPCFG0(a)  __builtin_linx_csrrs(0b001110100000, a)
#define RISCV_CSRRS_PMPCFG1(a)  __builtin_linx_csrrs(0b001110100001, a)
#define RISCV_CSRRS_PMPCFG2(a)  __builtin_linx_csrrs(0b001110100010, a)
#define RISCV_CSRRS_PMPCFG3(a)  __builtin_linx_csrrs(0b001110100011, a)
#define RISCV_CSRRS_PMPCFG4(a)  __builtin_linx_csrrs(0b001110100100, a)
#define RISCV_CSRRS_PMPCFG5(a)  __builtin_linx_csrrs(0b001110100101, a)
#define RISCV_CSRRS_PMPCFG6(a)  __builtin_linx_csrrs(0b001110100110, a)
#define RISCV_CSRRS_PMPCFG7(a)  __builtin_linx_csrrs(0b001110100111, a)
#define RISCV_CSRRS_PMPCFG8(a)  __builtin_linx_csrrs(0b001110101000, a)
#define RISCV_CSRRS_PMPCFG9(a)  __builtin_linx_csrrs(0b001110101001, a)
#define RISCV_CSRRS_PMPCFG10(a)  __builtin_linx_csrrs(0b001110101010, a)
#define RISCV_CSRRS_PMPCFG11(a)  __builtin_linx_csrrs(0b001110101011, a)
#define RISCV_CSRRS_PMPCFG12(a)  __builtin_linx_csrrs(0b001110101100, a)
#define RISCV_CSRRS_PMPCFG13(a)  __builtin_linx_csrrs(0b001110101101, a)
#define RISCV_CSRRS_PMPCFG14(a)  __builtin_linx_csrrs(0b001110101110, a)
#define RISCV_CSRRS_PMPCFG15(a)  __builtin_linx_csrrs(0b001110101111, a)
#define RISCV_CSRRS_PMPADDR0(a)  __builtin_linx_csrrs(0b001110110000, a)
#define RISCV_CSRRS_PMPADDR1(a)  __builtin_linx_csrrs(0b001110110001, a)
#define RISCV_CSRRS_PMPADDR2(a)  __builtin_linx_csrrs(0b001110110010, a)
#define RISCV_CSRRS_PMPADDR3(a)  __builtin_linx_csrrs(0b001110110011, a)
#define RISCV_CSRRS_PMPADDR4(a)  __builtin_linx_csrrs(0b001110110100, a)
#define RISCV_CSRRS_PMPADDR5(a)  __builtin_linx_csrrs(0b001110110101, a)
#define RISCV_CSRRS_PMPADDR6(a)  __builtin_linx_csrrs(0b001110110110, a)
#define RISCV_CSRRS_PMPADDR7(a)  __builtin_linx_csrrs(0b001110110111, a)
#define RISCV_CSRRS_PMPADDR8(a)  __builtin_linx_csrrs(0b001110111000, a)
#define RISCV_CSRRS_PMPADDR9(a)  __builtin_linx_csrrs(0b001110111001, a)
#define RISCV_CSRRS_PMPADDR10(a)  __builtin_linx_csrrs(0b001110111010, a)
#define RISCV_CSRRS_PMPADDR11(a)  __builtin_linx_csrrs(0b001110111011, a)
#define RISCV_CSRRS_PMPADDR12(a)  __builtin_linx_csrrs(0b001110111100, a)
#define RISCV_CSRRS_PMPADDR13(a)  __builtin_linx_csrrs(0b001110111101, a)
#define RISCV_CSRRS_PMPADDR14(a)  __builtin_linx_csrrs(0b001110111110, a)
#define RISCV_CSRRS_PMPADDR15(a)  __builtin_linx_csrrs(0b001110111111, a)
#define RISCV_CSRRS_PMPADDR16(a)  __builtin_linx_csrrs(0b001111000000, a)
#define RISCV_CSRRS_PMPADDR17(a)  __builtin_linx_csrrs(0b001111000001, a)
#define RISCV_CSRRS_PMPADDR18(a)  __builtin_linx_csrrs(0b001111000010, a)
#define RISCV_CSRRS_PMPADDR19(a)  __builtin_linx_csrrs(0b001111000011, a)
#define RISCV_CSRRS_PMPADDR20(a)  __builtin_linx_csrrs(0b001111000100, a)
#define RISCV_CSRRS_PMPADDR21(a)  __builtin_linx_csrrs(0b001111000101, a)
#define RISCV_CSRRS_PMPADDR22(a)  __builtin_linx_csrrs(0b001111000110, a)
#define RISCV_CSRRS_PMPADDR23(a)  __builtin_linx_csrrs(0b001111000111, a)
#define RISCV_CSRRS_PMPADDR24(a)  __builtin_linx_csrrs(0b001111001000, a)
#define RISCV_CSRRS_PMPADDR25(a)  __builtin_linx_csrrs(0b001111001001, a)
#define RISCV_CSRRS_PMPADDR26(a)  __builtin_linx_csrrs(0b001111001010, a)
#define RISCV_CSRRS_PMPADDR27(a)  __builtin_linx_csrrs(0b001111001011, a)
#define RISCV_CSRRS_PMPADDR28(a)  __builtin_linx_csrrs(0b001111001100, a)
#define RISCV_CSRRS_PMPADDR29(a)  __builtin_linx_csrrs(0b001111001101, a)
#define RISCV_CSRRS_PMPADDR30(a)  __builtin_linx_csrrs(0b001111001110, a)
#define RISCV_CSRRS_PMPADDR31(a)  __builtin_linx_csrrs(0b001111001111, a)
#define RISCV_CSRRS_PMPADDR32(a)  __builtin_linx_csrrs(0b001111010000, a)
#define RISCV_CSRRS_PMPADDR33(a)  __builtin_linx_csrrs(0b001111010001, a)
#define RISCV_CSRRS_PMPADDR34(a)  __builtin_linx_csrrs(0b001111010010, a)
#define RISCV_CSRRS_PMPADDR35(a)  __builtin_linx_csrrs(0b001111010011, a)
#define RISCV_CSRRS_PMPADDR36(a)  __builtin_linx_csrrs(0b001111010100, a)
#define RISCV_CSRRS_PMPADDR37(a)  __builtin_linx_csrrs(0b001111010101, a)
#define RISCV_CSRRS_PMPADDR38(a)  __builtin_linx_csrrs(0b001111010110, a)
#define RISCV_CSRRS_PMPADDR39(a)  __builtin_linx_csrrs(0b001111010111, a)
#define RISCV_CSRRS_PMPADDR40(a)  __builtin_linx_csrrs(0b001111011000, a)
#define RISCV_CSRRS_PMPADDR41(a)  __builtin_linx_csrrs(0b001111011001, a)
#define RISCV_CSRRS_PMPADDR42(a)  __builtin_linx_csrrs(0b001111011010, a)
#define RISCV_CSRRS_PMPADDR43(a)  __builtin_linx_csrrs(0b001111011011, a)
#define RISCV_CSRRS_PMPADDR44(a)  __builtin_linx_csrrs(0b001111011100, a)
#define RISCV_CSRRS_PMPADDR45(a)  __builtin_linx_csrrs(0b001111011101, a)
#define RISCV_CSRRS_PMPADDR46(a)  __builtin_linx_csrrs(0b001111011110, a)
#define RISCV_CSRRS_PMPADDR47(a)  __builtin_linx_csrrs(0b001111011111, a)
#define RISCV_CSRRS_PMPADDR48(a)  __builtin_linx_csrrs(0b001111100000, a)
#define RISCV_CSRRS_PMPADDR49(a)  __builtin_linx_csrrs(0b001111100001, a)
#define RISCV_CSRRS_PMPADDR50(a)  __builtin_linx_csrrs(0b001111100010, a)
#define RISCV_CSRRS_PMPADDR51(a)  __builtin_linx_csrrs(0b001111100011, a)
#define RISCV_CSRRS_PMPADDR52(a)  __builtin_linx_csrrs(0b001111100100, a)
#define RISCV_CSRRS_PMPADDR53(a)  __builtin_linx_csrrs(0b001111100101, a)
#define RISCV_CSRRS_PMPADDR54(a)  __builtin_linx_csrrs(0b001111100110, a)
#define RISCV_CSRRS_PMPADDR55(a)  __builtin_linx_csrrs(0b001111100111, a)
#define RISCV_CSRRS_PMPADDR56(a)  __builtin_linx_csrrs(0b001111101000, a)
#define RISCV_CSRRS_PMPADDR57(a)  __builtin_linx_csrrs(0b001111101001, a)
#define RISCV_CSRRS_PMPADDR58(a)  __builtin_linx_csrrs(0b001111101010, a)
#define RISCV_CSRRS_PMPADDR59(a)  __builtin_linx_csrrs(0b001111101011, a)
#define RISCV_CSRRS_PMPADDR60(a)  __builtin_linx_csrrs(0b001111101100, a)
#define RISCV_CSRRS_PMPADDR61(a)  __builtin_linx_csrrs(0b001111101101, a)
#define RISCV_CSRRS_PMPADDR62(a)  __builtin_linx_csrrs(0b001111101110, a)
#define RISCV_CSRRS_PMPADDR63(a)  __builtin_linx_csrrs(0b001111101111, a)
#define RISCV_CSRRS_MTVT(a)  __builtin_linx_csrrs(0b001100000111, a)
#define RISCV_CSRRS_MINTSTATUS(a)  __builtin_linx_csrrs(0b001101000110, a)
#define RISCV_CSRRS_MINTTHRESH(a)  __builtin_linx_csrrs(0b001101000111, a)
#define RISCV_CSRRS_MSCRATCHCSW(a)  __builtin_linx_csrrs(0b001101001000, a)
#define RISCV_CSRRS_MSCRATCHCSWL(a)  __builtin_linx_csrrs(0b001101001001, a)
#define RISCV_CSRRS_MCLICBASE(a)  __builtin_linx_csrrs(0b111110111111, a)
#define RISCV_CSRRS_MHPMEVENT3(a)  __builtin_linx_csrrs(0b001100100011, a)
#define RISCV_CSRRS_MHPMEVENT4(a)  __builtin_linx_csrrs(0b001100100100, a)
#define RISCV_CSRRS_MHPMEVENT5(a)  __builtin_linx_csrrs(0b001100100101, a)
#define RISCV_CSRRS_MHPMEVENT6(a)  __builtin_linx_csrrs(0b001100100110, a)
#define RISCV_CSRRS_MHPMEVENT7(a)  __builtin_linx_csrrs(0b001100100111, a)
#define RISCV_CSRRS_MHPMEVENT8(a)  __builtin_linx_csrrs(0b001100101000, a)
#define RISCV_CSRRS_MHPMEVENT9(a)  __builtin_linx_csrrs(0b001100101001, a)
#define RISCV_CSRRS_MHPMEVENT10(a)  __builtin_linx_csrrs(0b001100101010, a)
#define RISCV_CSRRS_MHPMEVENT11(a)  __builtin_linx_csrrs(0b001100101011, a)
#define RISCV_CSRRS_MHPMEVENT12(a)  __builtin_linx_csrrs(0b001100101100, a)
#define RISCV_CSRRS_MHPMEVENT13(a)  __builtin_linx_csrrs(0b001100101101, a)
#define RISCV_CSRRS_MHPMEVENT14(a)  __builtin_linx_csrrs(0b001100101110, a)
#define RISCV_CSRRS_MHPMEVENT15(a)  __builtin_linx_csrrs(0b001100101111, a)
#define RISCV_CSRRS_MHPMEVENT16(a)  __builtin_linx_csrrs(0b001100110000, a)
#define RISCV_CSRRS_MHPMEVENT17(a)  __builtin_linx_csrrs(0b001100110001, a)
#define RISCV_CSRRS_MHPMEVENT18(a)  __builtin_linx_csrrs(0b001100110010, a)
#define RISCV_CSRRS_MHPMEVENT19(a)  __builtin_linx_csrrs(0b001100110011, a)
#define RISCV_CSRRS_MHPMEVENT20(a)  __builtin_linx_csrrs(0b001100110100, a)
#define RISCV_CSRRS_MHPMEVENT21(a)  __builtin_linx_csrrs(0b001100110101, a)
#define RISCV_CSRRS_MHPMEVENT22(a)  __builtin_linx_csrrs(0b001100110110, a)
#define RISCV_CSRRS_MHPMEVENT23(a)  __builtin_linx_csrrs(0b001100110111, a)
#define RISCV_CSRRS_MHPMEVENT24(a)  __builtin_linx_csrrs(0b001100111000, a)
#define RISCV_CSRRS_MHPMEVENT25(a)  __builtin_linx_csrrs(0b001100111001, a)
#define RISCV_CSRRS_MHPMEVENT26(a)  __builtin_linx_csrrs(0b001100111010, a)
#define RISCV_CSRRS_MHPMEVENT27(a)  __builtin_linx_csrrs(0b001100111011, a)
#define RISCV_CSRRS_MHPMEVENT28(a)  __builtin_linx_csrrs(0b001100111100, a)
#define RISCV_CSRRS_MHPMEVENT29(a)  __builtin_linx_csrrs(0b001100111101, a)
#define RISCV_CSRRS_MHPMEVENT30(a)  __builtin_linx_csrrs(0b001100111110, a)
#define RISCV_CSRRS_MHPMEVENT31(a)  __builtin_linx_csrrs(0b001100111111, a)
#define RISCV_CSRRS_MCYCLE(a)  __builtin_linx_csrrs(0b101100000000, a)
#define RISCV_CSRRS_MINSTRET(a)  __builtin_linx_csrrs(0b101100000010, a)
#define RISCV_CSRRS_MHPMCOUNTER3(a)  __builtin_linx_csrrs(0b101100000011, a)
#define RISCV_CSRRS_MHPMCOUNTER4(a)  __builtin_linx_csrrs(0b101100000100, a)
#define RISCV_CSRRS_MHPMCOUNTER5(a)  __builtin_linx_csrrs(0b101100000101, a)
#define RISCV_CSRRS_MHPMCOUNTER6(a)  __builtin_linx_csrrs(0b101100000110, a)
#define RISCV_CSRRS_MHPMCOUNTER7(a)  __builtin_linx_csrrs(0b101100000111, a)
#define RISCV_CSRRS_MHPMCOUNTER8(a)  __builtin_linx_csrrs(0b101100001000, a)
#define RISCV_CSRRS_MHPMCOUNTER9(a)  __builtin_linx_csrrs(0b101100001001, a)
#define RISCV_CSRRS_MHPMCOUNTER10(a)  __builtin_linx_csrrs(0b101100001010, a)
#define RISCV_CSRRS_MHPMCOUNTER11(a)  __builtin_linx_csrrs(0b101100001011, a)
#define RISCV_CSRRS_MHPMCOUNTER12(a)  __builtin_linx_csrrs(0b101100001100, a)
#define RISCV_CSRRS_MHPMCOUNTER13(a)  __builtin_linx_csrrs(0b101100001101, a)
#define RISCV_CSRRS_MHPMCOUNTER14(a)  __builtin_linx_csrrs(0b101100001110, a)
#define RISCV_CSRRS_MHPMCOUNTER15(a)  __builtin_linx_csrrs(0b101100001111, a)
#define RISCV_CSRRS_MHPMCOUNTER16(a)  __builtin_linx_csrrs(0b101100010000, a)
#define RISCV_CSRRS_MHPMCOUNTER17(a)  __builtin_linx_csrrs(0b101100010001, a)
#define RISCV_CSRRS_MHPMCOUNTER18(a)  __builtin_linx_csrrs(0b101100010010, a)
#define RISCV_CSRRS_MHPMCOUNTER19(a)  __builtin_linx_csrrs(0b101100010011, a)
#define RISCV_CSRRS_MHPMCOUNTER20(a)  __builtin_linx_csrrs(0b101100010100, a)
#define RISCV_CSRRS_MHPMCOUNTER21(a)  __builtin_linx_csrrs(0b101100010101, a)
#define RISCV_CSRRS_MHPMCOUNTER22(a)  __builtin_linx_csrrs(0b101100010110, a)
#define RISCV_CSRRS_MHPMCOUNTER23(a)  __builtin_linx_csrrs(0b101100010111, a)
#define RISCV_CSRRS_MHPMCOUNTER24(a)  __builtin_linx_csrrs(0b101100011000, a)
#define RISCV_CSRRS_MHPMCOUNTER25(a)  __builtin_linx_csrrs(0b101100011001, a)
#define RISCV_CSRRS_MHPMCOUNTER26(a)  __builtin_linx_csrrs(0b101100011010, a)
#define RISCV_CSRRS_MHPMCOUNTER27(a)  __builtin_linx_csrrs(0b101100011011, a)
#define RISCV_CSRRS_MHPMCOUNTER28(a)  __builtin_linx_csrrs(0b101100011100, a)
#define RISCV_CSRRS_MHPMCOUNTER29(a)  __builtin_linx_csrrs(0b101100011101, a)
#define RISCV_CSRRS_MHPMCOUNTER30(a)  __builtin_linx_csrrs(0b101100011110, a)
#define RISCV_CSRRS_MHPMCOUNTER31(a)  __builtin_linx_csrrs(0b101100011111, a)
#define RISCV_CSRRS_MCYCLEH(a)  __builtin_linx_csrrs(0b101110000000, a)
#define RISCV_CSRRS_MINSTRETH(a)  __builtin_linx_csrrs(0b101110000010, a)
#define RISCV_CSRRS_MHPMCOUNTER3H(a)  __builtin_linx_csrrs(0b101110000011, a)
#define RISCV_CSRRS_MHPMCOUNTER4H(a)  __builtin_linx_csrrs(0b101110000100, a)
#define RISCV_CSRRS_MHPMCOUNTER5H(a)  __builtin_linx_csrrs(0b101110000101, a)
#define RISCV_CSRRS_MHPMCOUNTER6H(a)  __builtin_linx_csrrs(0b101110000110, a)
#define RISCV_CSRRS_MHPMCOUNTER7H(a)  __builtin_linx_csrrs(0b101110000111, a)
#define RISCV_CSRRS_MHPMCOUNTER8H(a)  __builtin_linx_csrrs(0b101110001000, a)
#define RISCV_CSRRS_MHPMCOUNTER9H(a)  __builtin_linx_csrrs(0b101110001001, a)
#define RISCV_CSRRS_MHPMCOUNTER10H(a)  __builtin_linx_csrrs(0b101110001010, a)
#define RISCV_CSRRS_MHPMCOUNTER11H(a)  __builtin_linx_csrrs(0b101110001011, a)
#define RISCV_CSRRS_MHPMCOUNTER12H(a)  __builtin_linx_csrrs(0b101110001100, a)
#define RISCV_CSRRS_MHPMCOUNTER13H(a)  __builtin_linx_csrrs(0b101110001101, a)
#define RISCV_CSRRS_MHPMCOUNTER14H(a)  __builtin_linx_csrrs(0b101110001110, a)
#define RISCV_CSRRS_MHPMCOUNTER15H(a)  __builtin_linx_csrrs(0b101110001111, a)
#define RISCV_CSRRS_MHPMCOUNTER16H(a)  __builtin_linx_csrrs(0b101110010000, a)
#define RISCV_CSRRS_MHPMCOUNTER17H(a)  __builtin_linx_csrrs(0b101110010001, a)
#define RISCV_CSRRS_MHPMCOUNTER18H(a)  __builtin_linx_csrrs(0b101110010010, a)
#define RISCV_CSRRS_MHPMCOUNTER19H(a)  __builtin_linx_csrrs(0b101110010011, a)
#define RISCV_CSRRS_MHPMCOUNTER20H(a)  __builtin_linx_csrrs(0b101110010100, a)
#define RISCV_CSRRS_MHPMCOUNTER21H(a)  __builtin_linx_csrrs(0b101110010101, a)
#define RISCV_CSRRS_MHPMCOUNTER22H(a)  __builtin_linx_csrrs(0b101110010110, a)
#define RISCV_CSRRS_MHPMCOUNTER23H(a)  __builtin_linx_csrrs(0b101110010111, a)
#define RISCV_CSRRS_MHPMCOUNTER24H(a)  __builtin_linx_csrrs(0b101110011000, a)
#define RISCV_CSRRS_MHPMCOUNTER25H(a)  __builtin_linx_csrrs(0b101110011001, a)
#define RISCV_CSRRS_MHPMCOUNTER26H(a)  __builtin_linx_csrrs(0b101110011010, a)
#define RISCV_CSRRS_MHPMCOUNTER27H(a)  __builtin_linx_csrrs(0b101110011011, a)
#define RISCV_CSRRS_MHPMCOUNTER28H(a)  __builtin_linx_csrrs(0b101110011100, a)
#define RISCV_CSRRS_MHPMCOUNTER29H(a)  __builtin_linx_csrrs(0b101110011101, a)
#define RISCV_CSRRS_MHPMCOUNTER30H(a)  __builtin_linx_csrrs(0b101110011110, a)
#define RISCV_CSRRS_MHPMCOUNTER31H(a)  __builtin_linx_csrrs(0b101110011111, a)
#define RISCV_CSRRS_ICCTL(a)  __builtin_linx_csrrs(0b011111000000, a)
#define RISCV_CSRRS_DCCTL(a)  __builtin_linx_csrrs(0b011111000001, a)
#define RISCV_CSRRS_ICMAINT(a)  __builtin_linx_csrrs(0b011111000010, a)
#define RISCV_CSRRS_DCMAINT(a)  __builtin_linx_csrrs(0b011111000011, a)
#define RISCV_CSRRS_ICINVA(a)  __builtin_linx_csrrs(0b011111000100, a)
#define RISCV_CSRRS_DCINCVA(a)  __builtin_linx_csrrs(0b011111000101, a)
#define RISCV_CSRRS_APREFI(a)  __builtin_linx_csrrs(0b011111000110, a)
#define RISCV_CSRRS_APREFD(a)  __builtin_linx_csrrs(0b011111000111, a)
#define RISCV_CSRRS_WST(a)  __builtin_linx_csrrs(0b011111010000, a)
#define RISCV_CSRRS_TCMARB(a)  __builtin_linx_csrrs(0b011111010010, a)
#define RISCV_CSRRS_MEMATTR0(a)  __builtin_linx_csrrs(0b101111000000, a)
#define RISCV_CSRRS_MEMATTR1(a)  __builtin_linx_csrrs(0b101111000001, a)
#define RISCV_CSRRS_MEMATTR2(a)  __builtin_linx_csrrs(0b101111000010, a)
#define RISCV_CSRRS_MEMATTR3(a)  __builtin_linx_csrrs(0b101111000011, a)
#define RISCV_CSRRS_MEMATTR4(a)  __builtin_linx_csrrs(0b101111000100, a)
#define RISCV_CSRRS_MEMATTR5(a)  __builtin_linx_csrrs(0b101111000101, a)
#define RISCV_CSRRS_MEMATTR6(a)  __builtin_linx_csrrs(0b101111000110, a)
#define RISCV_CSRRS_MEMATTR7(a)  __builtin_linx_csrrs(0b101111000111, a)
#define RISCV_CSRRS_MEMATTR8(a)  __builtin_linx_csrrs(0b101111001000, a)
#define RISCV_CSRRS_MEMATTR9(a)  __builtin_linx_csrrs(0b101111001001, a)
#define RISCV_CSRRS_MEMATTR10(a)  __builtin_linx_csrrs(0b101111001010, a)
#define RISCV_CSRRS_MEMATTR11(a)  __builtin_linx_csrrs(0b101111001011, a)
#define RISCV_CSRRS_MEMATTR12(a)  __builtin_linx_csrrs(0b101111001100, a)
#define RISCV_CSRRS_MEMATTR13(a)  __builtin_linx_csrrs(0b101111001101, a)
#define RISCV_CSRRS_MEMATTR14(a)  __builtin_linx_csrrs(0b101111001110, a)
#define RISCV_CSRRS_MEMATTR15(a)  __builtin_linx_csrrs(0b101111001111, a)
#define RISCV_CSRRS_MEMATTR16(a)  __builtin_linx_csrrs(0b101111010000, a)
#define RISCV_CSRRS_MEMATTR17(a)  __builtin_linx_csrrs(0b101111010001, a)
#define RISCV_CSRRS_MEMATTR18(a)  __builtin_linx_csrrs(0b101111010010, a)
#define RISCV_CSRRS_MEMATTR19(a)  __builtin_linx_csrrs(0b101111010011, a)
#define RISCV_CSRRS_MEMATTR20(a)  __builtin_linx_csrrs(0b101111010100, a)
#define RISCV_CSRRS_MEMATTR21(a)  __builtin_linx_csrrs(0b101111010101, a)
#define RISCV_CSRRS_MEMATTR22(a)  __builtin_linx_csrrs(0b101111010110, a)
#define RISCV_CSRRS_MEMATTR23(a)  __builtin_linx_csrrs(0b101111010111, a)
#define RISCV_CSRRS_MEMATTR24(a)  __builtin_linx_csrrs(0b101111011000, a)
#define RISCV_CSRRS_MEMATTR25(a)  __builtin_linx_csrrs(0b101111011001, a)
#define RISCV_CSRRS_MEMATTR26(a)  __builtin_linx_csrrs(0b101111011010, a)
#define RISCV_CSRRS_MEMATTR27(a)  __builtin_linx_csrrs(0b101111011011, a)
#define RISCV_CSRRS_MEMATTR28(a)  __builtin_linx_csrrs(0b101111011100, a)
#define RISCV_CSRRS_MEMATTR29(a)  __builtin_linx_csrrs(0b101111011101, a)
#define RISCV_CSRRS_MEMATTR30(a)  __builtin_linx_csrrs(0b101111011110, a)
#define RISCV_CSRRS_MEMATTR31(a)  __builtin_linx_csrrs(0b101111011111, a)
#define RISCV_CSRRS_MEMATTR32(a)  __builtin_linx_csrrs(0b101111100000, a)
#define RISCV_CSRRS_MEMATTR33(a)  __builtin_linx_csrrs(0b101111100001, a)
#define RISCV_CSRRS_MEMATTR34(a)  __builtin_linx_csrrs(0b101111100010, a)
#define RISCV_CSRRS_MEMATTR35(a)  __builtin_linx_csrrs(0b101111100011, a)
#define RISCV_CSRRS_MEMATTR36(a)  __builtin_linx_csrrs(0b101111100100, a)
#define RISCV_CSRRS_MEMATTR37(a)  __builtin_linx_csrrs(0b101111100101, a)
#define RISCV_CSRRS_MEMATTR38(a)  __builtin_linx_csrrs(0b101111100110, a)
#define RISCV_CSRRS_MEMATTR39(a)  __builtin_linx_csrrs(0b101111100111, a)
#define RISCV_CSRRS_MEMATTR40(a)  __builtin_linx_csrrs(0b101111101000, a)
#define RISCV_CSRRS_MEMATTR41(a)  __builtin_linx_csrrs(0b101111101001, a)
#define RISCV_CSRRS_MEMATTR42(a)  __builtin_linx_csrrs(0b101111101010, a)
#define RISCV_CSRRS_MEMATTR43(a)  __builtin_linx_csrrs(0b101111101011, a)
#define RISCV_CSRRS_MEMATTR44(a)  __builtin_linx_csrrs(0b101111101100, a)
#define RISCV_CSRRS_MEMATTR45(a)  __builtin_linx_csrrs(0b101111101101, a)
#define RISCV_CSRRS_MEMATTR46(a)  __builtin_linx_csrrs(0b101111101110, a)
#define RISCV_CSRRS_MEMATTR47(a)  __builtin_linx_csrrs(0b101111101111, a)
#define RISCV_CSRRS_MEMATTR48(a)  __builtin_linx_csrrs(0b101111110000, a)
#define RISCV_CSRRS_MEMATTR49(a)  __builtin_linx_csrrs(0b101111110001, a)
#define RISCV_CSRRS_MEMATTR50(a)  __builtin_linx_csrrs(0b101111110010, a)
#define RISCV_CSRRS_MEMATTR51(a)  __builtin_linx_csrrs(0b101111110011, a)
#define RISCV_CSRRS_MEMATTR52(a)  __builtin_linx_csrrs(0b101111110100, a)
#define RISCV_CSRRS_MEMATTR53(a)  __builtin_linx_csrrs(0b101111110101, a)
#define RISCV_CSRRS_MEMATTR54(a)  __builtin_linx_csrrs(0b101111110110, a)
#define RISCV_CSRRS_MEMATTR55(a)  __builtin_linx_csrrs(0b101111110111, a)
#define RISCV_CSRRS_MEMATTR56(a)  __builtin_linx_csrrs(0b101111111000, a)
#define RISCV_CSRRS_MEMATTR57(a)  __builtin_linx_csrrs(0b101111111001, a)
#define RISCV_CSRRS_MEMATTR58(a)  __builtin_linx_csrrs(0b101111111010, a)
#define RISCV_CSRRS_MEMATTR59(a)  __builtin_linx_csrrs(0b101111111011, a)
#define RISCV_CSRRS_MEMATTR60(a)  __builtin_linx_csrrs(0b101111111100, a)
#define RISCV_CSRRS_MEMATTR61(a)  __builtin_linx_csrrs(0b101111111101, a)
#define RISCV_CSRRS_MEMATTR62(a)  __builtin_linx_csrrs(0b101111111110, a)
#define RISCV_CSRRS_MEMATTR63(a)  __builtin_linx_csrrs(0b101111111111, a)
#define RISCV_CSRRS_PORPC(a)  __builtin_linx_csrrs(0b111111000000, a)
#define RISCV_CSRRS_WTOPC(a)  __builtin_linx_csrrs(0b111111000001, a)
#define RISCV_CSRRS_CXCPTSC(a)  __builtin_linx_csrrs(0b111111000010, a)
#define RISCV_CSRRS_DBGUOPCNT(a)  __builtin_linx_csrrs(0b011111111110, a)
#define RISCV_CSRRS_FFLAGS(a)  __builtin_linx_csrrs(0b000000000001, a)
#define RISCV_CSRRS_FRM(a)  __builtin_linx_csrrs(0b000000000010, a)
#define RISCV_CSRRS_FCSR(a)  __builtin_linx_csrrs(0b000000000011, a)
#define RISCV_CSRRS_HPMCOUNTER3(a)  __builtin_linx_csrrs(0b110000000011, a)
#define RISCV_CSRRS_HPMCOUNTER4(a)  __builtin_linx_csrrs(0b110000000100, a)
#define RISCV_CSRRS_HPMCOUNTER5(a)  __builtin_linx_csrrs(0b110000000101, a)
#define RISCV_CSRRS_HPMCOUNTER6(a)  __builtin_linx_csrrs(0b110000000110, a)
#define RISCV_CSRRS_HPMCOUNTER7(a)  __builtin_linx_csrrs(0b110000000111, a)
#define RISCV_CSRRS_HPMCOUNTER8(a)  __builtin_linx_csrrs(0b110000001000, a)
#define RISCV_CSRRS_HPMCOUNTER9(a)  __builtin_linx_csrrs(0b110000001001, a)
#define RISCV_CSRRS_HPMCOUNTER10(a)  __builtin_linx_csrrs(0b110000001010, a)
#define RISCV_CSRRS_HPMCOUNTER11(a)  __builtin_linx_csrrs(0b110000001011, a)
#define RISCV_CSRRS_HPMCOUNTER12(a)  __builtin_linx_csrrs(0b110000001100, a)
#define RISCV_CSRRS_HPMCOUNTER13(a)  __builtin_linx_csrrs(0b110000001101, a)
#define RISCV_CSRRS_HPMCOUNTER14(a)  __builtin_linx_csrrs(0b110000001110, a)
#define RISCV_CSRRS_HPMCOUNTER15(a)  __builtin_linx_csrrs(0b110000001111, a)
#define RISCV_CSRRS_HPMCOUNTER16(a)  __builtin_linx_csrrs(0b110000010000, a)
#define RISCV_CSRRS_HPMCOUNTER17(a)  __builtin_linx_csrrs(0b110000010001, a)
#define RISCV_CSRRS_HPMCOUNTER18(a)  __builtin_linx_csrrs(0b110000010010, a)
#define RISCV_CSRRS_HPMCOUNTER19(a)  __builtin_linx_csrrs(0b110000010011, a)
#define RISCV_CSRRS_HPMCOUNTER20(a)  __builtin_linx_csrrs(0b110000010100, a)
#define RISCV_CSRRS_HPMCOUNTER21(a)  __builtin_linx_csrrs(0b110000010101, a)
#define RISCV_CSRRS_HPMCOUNTER22(a)  __builtin_linx_csrrs(0b110000010110, a)
#define RISCV_CSRRS_HPMCOUNTER23(a)  __builtin_linx_csrrs(0b110000010111, a)
#define RISCV_CSRRS_HPMCOUNTER24(a)  __builtin_linx_csrrs(0b110000011000, a)
#define RISCV_CSRRS_HPMCOUNTER25(a)  __builtin_linx_csrrs(0b110000011001, a)
#define RISCV_CSRRS_HPMCOUNTER26(a)  __builtin_linx_csrrs(0b110000011010, a)
#define RISCV_CSRRS_HPMCOUNTER27(a)  __builtin_linx_csrrs(0b110000011011, a)
#define RISCV_CSRRS_HPMCOUNTER28(a)  __builtin_linx_csrrs(0b110000011100, a)
#define RISCV_CSRRS_HPMCOUNTER29(a)  __builtin_linx_csrrs(0b110000011101, a)
#define RISCV_CSRRS_HPMCOUNTER30(a)  __builtin_linx_csrrs(0b110000011110, a)
#define RISCV_CSRRS_HPMCOUNTER31(a)  __builtin_linx_csrrs(0b110000011111, a)
#define RISCV_CSRRS_CYCLE(a)  __builtin_linx_csrrs(0b110000000000, a)
#define RISCV_CSRRS_TIME(a)  __builtin_linx_csrrs(0b110000000001, a)
#define RISCV_CSRRS_INSTRET(a)  __builtin_linx_csrrs(0b110000000010, a)
#define RISCV_CSRRS_HPMCOUNTER3H(a)  __builtin_linx_csrrs(0b110010000011, a)
#define RISCV_CSRRS_HPMCOUNTER4H(a)  __builtin_linx_csrrs(0b110010000100, a)
#define RISCV_CSRRS_HPMCOUNTER5H(a)  __builtin_linx_csrrs(0b110010000101, a)
#define RISCV_CSRRS_HPMCOUNTER6H(a)  __builtin_linx_csrrs(0b110010000110, a)
#define RISCV_CSRRS_HPMCOUNTER7H(a)  __builtin_linx_csrrs(0b110010000111, a)
#define RISCV_CSRRS_HPMCOUNTER8H(a)  __builtin_linx_csrrs(0b110010001000, a)
#define RISCV_CSRRS_HPMCOUNTER9H(a)  __builtin_linx_csrrs(0b110010001001, a)
#define RISCV_CSRRS_HPMCOUNTER10H(a)  __builtin_linx_csrrs(0b110010001010, a)
#define RISCV_CSRRS_HPMCOUNTER11H(a)  __builtin_linx_csrrs(0b110010001011, a)
#define RISCV_CSRRS_HPMCOUNTER12H(a)  __builtin_linx_csrrs(0b110010001100, a)
#define RISCV_CSRRS_HPMCOUNTER13H(a)  __builtin_linx_csrrs(0b110010001101, a)
#define RISCV_CSRRS_HPMCOUNTER14H(a)  __builtin_linx_csrrs(0b110010001110, a)
#define RISCV_CSRRS_HPMCOUNTER15H(a)  __builtin_linx_csrrs(0b110010001111, a)
#define RISCV_CSRRS_HPMCOUNTER16H(a)  __builtin_linx_csrrs(0b110010010000, a)
#define RISCV_CSRRS_HPMCOUNTER17H(a)  __builtin_linx_csrrs(0b110010010001, a)
#define RISCV_CSRRS_HPMCOUNTER18H(a)  __builtin_linx_csrrs(0b110010010010, a)
#define RISCV_CSRRS_HPMCOUNTER19H(a)  __builtin_linx_csrrs(0b110010010011, a)
#define RISCV_CSRRS_HPMCOUNTER20H(a)  __builtin_linx_csrrs(0b110010010100, a)
#define RISCV_CSRRS_HPMCOUNTER21H(a)  __builtin_linx_csrrs(0b110010010101, a)
#define RISCV_CSRRS_HPMCOUNTER22H(a)  __builtin_linx_csrrs(0b110010010110, a)
#define RISCV_CSRRS_HPMCOUNTER23H(a)  __builtin_linx_csrrs(0b110010010111, a)
#define RISCV_CSRRS_HPMCOUNTER24H(a)  __builtin_linx_csrrs(0b110010011000, a)
#define RISCV_CSRRS_HPMCOUNTER25H(a)  __builtin_linx_csrrs(0b110010011001, a)
#define RISCV_CSRRS_HPMCOUNTER26H(a)  __builtin_linx_csrrs(0b110010011010, a)
#define RISCV_CSRRS_HPMCOUNTER27H(a)  __builtin_linx_csrrs(0b110010011011, a)
#define RISCV_CSRRS_HPMCOUNTER28H(a)  __builtin_linx_csrrs(0b110010011100, a)
#define RISCV_CSRRS_HPMCOUNTER29H(a)  __builtin_linx_csrrs(0b110010011101, a)
#define RISCV_CSRRS_HPMCOUNTER30H(a)  __builtin_linx_csrrs(0b110010011110, a)
#define RISCV_CSRRS_HPMCOUNTER31H(a)  __builtin_linx_csrrs(0b110010011111, a)
#define RISCV_CSRRS_CYCLEH(a)  __builtin_linx_csrrs(0b110010000000, a)
#define RISCV_CSRRS_TIMEH(a)  __builtin_linx_csrrs(0b110010000001, a)
#define RISCV_CSRRS_INSTRETH(a)  __builtin_linx_csrrs(0b110010000010, a)
#define RISCV_CSRRS_TMESCR(a)  __builtin_linx_csrrs(0b011111100000, a)
#define RISCV_CSRRS_TMESVEC(a)  __builtin_linx_csrrs(0b011111100001, a)
#define RISCV_CSRRS_TMESTOP(a)  __builtin_linx_csrrs(0b011111100010, a)
#define RISCV_CSRRS_TMEDELEG(a)  __builtin_linx_csrrs(0b011111100011, a)
#define RISCV_CSRRS_TMTVEC(a)  __builtin_linx_csrrs(0b011111100100, a)
#define RISCV_CSRRS_TMTVT(a)  __builtin_linx_csrrs(0b011111100101, a)
#define RISCV_CSRRS_TMINTTHRESH(a)  __builtin_linx_csrrs(0b011111100110, a)
#define RISCV_CSRRS_TMSTATUS(a)  __builtin_linx_csrrs(0b011111100111, a)
#define RISCV_CSRRS_TMEPC(a)  __builtin_linx_csrrs(0b011111101000, a)
#define RISCV_CSRRS_TMCAUSE(a)  __builtin_linx_csrrs(0b011111101001, a)
#define RISCV_CSRRS_TCXCPTSC(a)  __builtin_linx_csrrs(0b111111000011, a)
#define RISCV_CSRRS_TMTVAL(a)  __builtin_linx_csrrs(0b011111101011, a)
#define RISCV_CSRRS_TMSCRATCH(a)  __builtin_linx_csrrs(0b011111101100, a)
#define RISCV_CSRRS_TMESEPR(a)  __builtin_linx_csrrs(0b011111101101, a)
#define RISCV_CSRRS_TMESEPS(a)  __builtin_linx_csrrs(0b011111101110, a)
#define RISCV_CSRRS_MSECCTL(a)  __builtin_linx_csrrs(0b011111101111, a)
#define RISCV_CSRRS_PMPTE0(a)  __builtin_linx_csrrs(0b011111110000, a)
#define RISCV_CSRRS_PMPTE1(a)  __builtin_linx_csrrs(0b011111110001, a)
#define RISCV_CSRRS_MSPLIMIT(a)  __builtin_linx_csrrs(0b011111110010, a)
#define RISCV_CSRRS_TMSPLIMIT(a)  __builtin_linx_csrrs(0b011111110011, a)
#define RISCV_CSRRS_TUSP(a)  __builtin_linx_csrrs(0b100000000000, a)
#define RISCV_CSRRS_TUGP(a)  __builtin_linx_csrrs(0b100000000001, a)
#define RISCV_CSRRS_TUTP(a)  __builtin_linx_csrrs(0b100000000010, a)
#define RISCV_CSRRS_TSP(a)  __builtin_linx_csrrs(0b100000000011, a)
#define RISCV_CSRRS_TGP(a)  __builtin_linx_csrrs(0b100000000100, a)
#define RISCV_CSRRS_TTP(a)  __builtin_linx_csrrs(0b100000000101, a)
#define RISCV_CSRRS_TESEPR(a)  __builtin_linx_csrrs(0b110011000000, a)
#define RISCV_CSRRS_TESEPS(a)  __builtin_linx_csrrs(0b110011000001, a)
#define RISCV_CSRRS_VXSAT(a)  __builtin_linx_csrrs(0b000000001001, a)
#define RISCV_CSRRS_VXRM(a)  __builtin_linx_csrrs(0b000000001010, a)
#define RISCV_CSRRS_VCSR(a)  __builtin_linx_csrrs(0b000000001111, a)
#define RISCV_CSRRS_VSTART(a)  __builtin_linx_csrrs(0b000000001000, a)
#define RISCV_CSRRS_VL(a)  __builtin_linx_csrrs(0b110000100000, a)
#define RISCV_CSRRS_VTYPE(a)  __builtin_linx_csrrs(0b110000100001, a)
#define RISCV_CSRRS_VLENB(a)  __builtin_linx_csrrs(0b110000100010, a)
#define RISCV_CSRRS_JBC(a)  __builtin_linx_csrrs(0b100111110000, a)
#define RISCV_CSRRS_JPC_UPPER(a)  __builtin_linx_csrrs(0b100111110001, a)
#define RISCV_CSRRS_JPC_LOWER(a)  __builtin_linx_csrrs(0b100111110010, a)
#define RISCV_CSRRS_JBCFT(a)  __builtin_linx_csrrs(0b100111110011, a)
#define RISCV_CSRRS_JBC_UPPER(a)  __builtin_linx_csrrs(0b100111110100, a)
#define RISCV_CSRRS_JBC_SHADOW(a)  __builtin_linx_csrrs(0b110011110000, a)
#define RISCV_CSRRS_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrs(0b100111111111, a)
#define RISCV_CSRRS_TSELECT(a)  __builtin_linx_csrrs(0b011110100000, a)
#define RISCV_CSRRS_TDATA1(a)  __builtin_linx_csrrs(0b011110100001, a)
#define RISCV_CSRRS_TDATA2(a)  __builtin_linx_csrrs(0b011110100010, a)
#define RISCV_CSRRS_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRS_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRS_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRS_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRS_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRS_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRS_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRS_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRS_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRS_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRS_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRS_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRS_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRS_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRS_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRS_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRS_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRS_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRS_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRS_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRS_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRS_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRS_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRS_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRS_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRS_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRS_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRS_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRS_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRS_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRS_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRS_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRS_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRS_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRS_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRS_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRS_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRS_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRS_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRS_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRS_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRS_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRS_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRS_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRS_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRS_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRS_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRS_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRS_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRS_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRS_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRS_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRS_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRS_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRS_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRS_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRS_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRS_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRS_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRS_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRS_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRS_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRS_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRS_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRS_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRS_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRS_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRS_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRS_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRS_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRS_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRS_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRS_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRS_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRS_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRS_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRS_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRS_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRS_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRS_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRS_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRS_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRSI  __builtin_linx_csrrsi
#define RISCV_CSRRSI_DCSR(a)  __builtin_linx_csrrsi(0b011110110000, a)
#define RISCV_CSRRSI_DPC(a)  __builtin_linx_csrrsi(0b011110110001, a)
#define RISCV_CSRRSI_DSCRATCH0(a)  __builtin_linx_csrrsi(0b011110110010, a)
#define RISCV_CSRRSI_MVENDORID(a)  __builtin_linx_csrrsi(0b111100010001, a)
#define RISCV_CSRRSI_MARCHID(a)  __builtin_linx_csrrsi(0b111100010010, a)
#define RISCV_CSRRSI_MIMPID(a)  __builtin_linx_csrrsi(0b111100010011, a)
#define RISCV_CSRRSI_MHARTID(a)  __builtin_linx_csrrsi(0b111100010100, a)
#define RISCV_CSRRSI_MSTATUS(a)  __builtin_linx_csrrsi(0b001100000000, a)
#define RISCV_CSRRSI_MISA(a)  __builtin_linx_csrrsi(0b001100000001, a)
#define RISCV_CSRRSI_MIE(a)  __builtin_linx_csrrsi(0b001100000100, a)
#define RISCV_CSRRSI_MTVEC(a)  __builtin_linx_csrrsi(0b001100000101, a)
#define RISCV_CSRRSI_MIP(a)  __builtin_linx_csrrsi(0b001101000100, a)
#define RISCV_CSRRSI_MSECCFG(a)  __builtin_linx_csrrsi(0b001110010000, a)
#define RISCV_CSRRSI_MCOUNTEREN(a)  __builtin_linx_csrrsi(0b001100000110, a)
#define RISCV_CSRRSI_MCOUNTINHIBIT(a)  __builtin_linx_csrrsi(0b001100100000, a)
#define RISCV_CSRRSI_MSCRATCH(a)  __builtin_linx_csrrsi(0b001101000000, a)
#define RISCV_CSRRSI_MEPC(a)  __builtin_linx_csrrsi(0b001101000001, a)
#define RISCV_CSRRSI_MCAUSE(a)  __builtin_linx_csrrsi(0b001101000010, a)
#define RISCV_CSRRSI_MTVAL(a)  __builtin_linx_csrrsi(0b001101000011, a)
#define RISCV_CSRRSI_PMPCFG0(a)  __builtin_linx_csrrsi(0b001110100000, a)
#define RISCV_CSRRSI_PMPCFG1(a)  __builtin_linx_csrrsi(0b001110100001, a)
#define RISCV_CSRRSI_PMPCFG2(a)  __builtin_linx_csrrsi(0b001110100010, a)
#define RISCV_CSRRSI_PMPCFG3(a)  __builtin_linx_csrrsi(0b001110100011, a)
#define RISCV_CSRRSI_PMPCFG4(a)  __builtin_linx_csrrsi(0b001110100100, a)
#define RISCV_CSRRSI_PMPCFG5(a)  __builtin_linx_csrrsi(0b001110100101, a)
#define RISCV_CSRRSI_PMPCFG6(a)  __builtin_linx_csrrsi(0b001110100110, a)
#define RISCV_CSRRSI_PMPCFG7(a)  __builtin_linx_csrrsi(0b001110100111, a)
#define RISCV_CSRRSI_PMPCFG8(a)  __builtin_linx_csrrsi(0b001110101000, a)
#define RISCV_CSRRSI_PMPCFG9(a)  __builtin_linx_csrrsi(0b001110101001, a)
#define RISCV_CSRRSI_PMPCFG10(a)  __builtin_linx_csrrsi(0b001110101010, a)
#define RISCV_CSRRSI_PMPCFG11(a)  __builtin_linx_csrrsi(0b001110101011, a)
#define RISCV_CSRRSI_PMPCFG12(a)  __builtin_linx_csrrsi(0b001110101100, a)
#define RISCV_CSRRSI_PMPCFG13(a)  __builtin_linx_csrrsi(0b001110101101, a)
#define RISCV_CSRRSI_PMPCFG14(a)  __builtin_linx_csrrsi(0b001110101110, a)
#define RISCV_CSRRSI_PMPCFG15(a)  __builtin_linx_csrrsi(0b001110101111, a)
#define RISCV_CSRRSI_PMPADDR0(a)  __builtin_linx_csrrsi(0b001110110000, a)
#define RISCV_CSRRSI_PMPADDR1(a)  __builtin_linx_csrrsi(0b001110110001, a)
#define RISCV_CSRRSI_PMPADDR2(a)  __builtin_linx_csrrsi(0b001110110010, a)
#define RISCV_CSRRSI_PMPADDR3(a)  __builtin_linx_csrrsi(0b001110110011, a)
#define RISCV_CSRRSI_PMPADDR4(a)  __builtin_linx_csrrsi(0b001110110100, a)
#define RISCV_CSRRSI_PMPADDR5(a)  __builtin_linx_csrrsi(0b001110110101, a)
#define RISCV_CSRRSI_PMPADDR6(a)  __builtin_linx_csrrsi(0b001110110110, a)
#define RISCV_CSRRSI_PMPADDR7(a)  __builtin_linx_csrrsi(0b001110110111, a)
#define RISCV_CSRRSI_PMPADDR8(a)  __builtin_linx_csrrsi(0b001110111000, a)
#define RISCV_CSRRSI_PMPADDR9(a)  __builtin_linx_csrrsi(0b001110111001, a)
#define RISCV_CSRRSI_PMPADDR10(a)  __builtin_linx_csrrsi(0b001110111010, a)
#define RISCV_CSRRSI_PMPADDR11(a)  __builtin_linx_csrrsi(0b001110111011, a)
#define RISCV_CSRRSI_PMPADDR12(a)  __builtin_linx_csrrsi(0b001110111100, a)
#define RISCV_CSRRSI_PMPADDR13(a)  __builtin_linx_csrrsi(0b001110111101, a)
#define RISCV_CSRRSI_PMPADDR14(a)  __builtin_linx_csrrsi(0b001110111110, a)
#define RISCV_CSRRSI_PMPADDR15(a)  __builtin_linx_csrrsi(0b001110111111, a)
#define RISCV_CSRRSI_PMPADDR16(a)  __builtin_linx_csrrsi(0b001111000000, a)
#define RISCV_CSRRSI_PMPADDR17(a)  __builtin_linx_csrrsi(0b001111000001, a)
#define RISCV_CSRRSI_PMPADDR18(a)  __builtin_linx_csrrsi(0b001111000010, a)
#define RISCV_CSRRSI_PMPADDR19(a)  __builtin_linx_csrrsi(0b001111000011, a)
#define RISCV_CSRRSI_PMPADDR20(a)  __builtin_linx_csrrsi(0b001111000100, a)
#define RISCV_CSRRSI_PMPADDR21(a)  __builtin_linx_csrrsi(0b001111000101, a)
#define RISCV_CSRRSI_PMPADDR22(a)  __builtin_linx_csrrsi(0b001111000110, a)
#define RISCV_CSRRSI_PMPADDR23(a)  __builtin_linx_csrrsi(0b001111000111, a)
#define RISCV_CSRRSI_PMPADDR24(a)  __builtin_linx_csrrsi(0b001111001000, a)
#define RISCV_CSRRSI_PMPADDR25(a)  __builtin_linx_csrrsi(0b001111001001, a)
#define RISCV_CSRRSI_PMPADDR26(a)  __builtin_linx_csrrsi(0b001111001010, a)
#define RISCV_CSRRSI_PMPADDR27(a)  __builtin_linx_csrrsi(0b001111001011, a)
#define RISCV_CSRRSI_PMPADDR28(a)  __builtin_linx_csrrsi(0b001111001100, a)
#define RISCV_CSRRSI_PMPADDR29(a)  __builtin_linx_csrrsi(0b001111001101, a)
#define RISCV_CSRRSI_PMPADDR30(a)  __builtin_linx_csrrsi(0b001111001110, a)
#define RISCV_CSRRSI_PMPADDR31(a)  __builtin_linx_csrrsi(0b001111001111, a)
#define RISCV_CSRRSI_PMPADDR32(a)  __builtin_linx_csrrsi(0b001111010000, a)
#define RISCV_CSRRSI_PMPADDR33(a)  __builtin_linx_csrrsi(0b001111010001, a)
#define RISCV_CSRRSI_PMPADDR34(a)  __builtin_linx_csrrsi(0b001111010010, a)
#define RISCV_CSRRSI_PMPADDR35(a)  __builtin_linx_csrrsi(0b001111010011, a)
#define RISCV_CSRRSI_PMPADDR36(a)  __builtin_linx_csrrsi(0b001111010100, a)
#define RISCV_CSRRSI_PMPADDR37(a)  __builtin_linx_csrrsi(0b001111010101, a)
#define RISCV_CSRRSI_PMPADDR38(a)  __builtin_linx_csrrsi(0b001111010110, a)
#define RISCV_CSRRSI_PMPADDR39(a)  __builtin_linx_csrrsi(0b001111010111, a)
#define RISCV_CSRRSI_PMPADDR40(a)  __builtin_linx_csrrsi(0b001111011000, a)
#define RISCV_CSRRSI_PMPADDR41(a)  __builtin_linx_csrrsi(0b001111011001, a)
#define RISCV_CSRRSI_PMPADDR42(a)  __builtin_linx_csrrsi(0b001111011010, a)
#define RISCV_CSRRSI_PMPADDR43(a)  __builtin_linx_csrrsi(0b001111011011, a)
#define RISCV_CSRRSI_PMPADDR44(a)  __builtin_linx_csrrsi(0b001111011100, a)
#define RISCV_CSRRSI_PMPADDR45(a)  __builtin_linx_csrrsi(0b001111011101, a)
#define RISCV_CSRRSI_PMPADDR46(a)  __builtin_linx_csrrsi(0b001111011110, a)
#define RISCV_CSRRSI_PMPADDR47(a)  __builtin_linx_csrrsi(0b001111011111, a)
#define RISCV_CSRRSI_PMPADDR48(a)  __builtin_linx_csrrsi(0b001111100000, a)
#define RISCV_CSRRSI_PMPADDR49(a)  __builtin_linx_csrrsi(0b001111100001, a)
#define RISCV_CSRRSI_PMPADDR50(a)  __builtin_linx_csrrsi(0b001111100010, a)
#define RISCV_CSRRSI_PMPADDR51(a)  __builtin_linx_csrrsi(0b001111100011, a)
#define RISCV_CSRRSI_PMPADDR52(a)  __builtin_linx_csrrsi(0b001111100100, a)
#define RISCV_CSRRSI_PMPADDR53(a)  __builtin_linx_csrrsi(0b001111100101, a)
#define RISCV_CSRRSI_PMPADDR54(a)  __builtin_linx_csrrsi(0b001111100110, a)
#define RISCV_CSRRSI_PMPADDR55(a)  __builtin_linx_csrrsi(0b001111100111, a)
#define RISCV_CSRRSI_PMPADDR56(a)  __builtin_linx_csrrsi(0b001111101000, a)
#define RISCV_CSRRSI_PMPADDR57(a)  __builtin_linx_csrrsi(0b001111101001, a)
#define RISCV_CSRRSI_PMPADDR58(a)  __builtin_linx_csrrsi(0b001111101010, a)
#define RISCV_CSRRSI_PMPADDR59(a)  __builtin_linx_csrrsi(0b001111101011, a)
#define RISCV_CSRRSI_PMPADDR60(a)  __builtin_linx_csrrsi(0b001111101100, a)
#define RISCV_CSRRSI_PMPADDR61(a)  __builtin_linx_csrrsi(0b001111101101, a)
#define RISCV_CSRRSI_PMPADDR62(a)  __builtin_linx_csrrsi(0b001111101110, a)
#define RISCV_CSRRSI_PMPADDR63(a)  __builtin_linx_csrrsi(0b001111101111, a)
#define RISCV_CSRRSI_MTVT(a)  __builtin_linx_csrrsi(0b001100000111, a)
#define RISCV_CSRRSI_MNXTI(a)  __builtin_linx_csrrsi(0b001101000101, a)
#define RISCV_CSRRSI_MINTSTATUS(a)  __builtin_linx_csrrsi(0b001101000110, a)
#define RISCV_CSRRSI_MINTTHRESH(a)  __builtin_linx_csrrsi(0b001101000111, a)
#define RISCV_CSRRSI_MSCRATCHCSW(a)  __builtin_linx_csrrsi(0b001101001000, a)
#define RISCV_CSRRSI_MSCRATCHCSWL(a)  __builtin_linx_csrrsi(0b001101001001, a)
#define RISCV_CSRRSI_MCLICBASE(a)  __builtin_linx_csrrsi(0b111110111111, a)
#define RISCV_CSRRSI_MHPMEVENT3(a)  __builtin_linx_csrrsi(0b001100100011, a)
#define RISCV_CSRRSI_MHPMEVENT4(a)  __builtin_linx_csrrsi(0b001100100100, a)
#define RISCV_CSRRSI_MHPMEVENT5(a)  __builtin_linx_csrrsi(0b001100100101, a)
#define RISCV_CSRRSI_MHPMEVENT6(a)  __builtin_linx_csrrsi(0b001100100110, a)
#define RISCV_CSRRSI_MHPMEVENT7(a)  __builtin_linx_csrrsi(0b001100100111, a)
#define RISCV_CSRRSI_MHPMEVENT8(a)  __builtin_linx_csrrsi(0b001100101000, a)
#define RISCV_CSRRSI_MHPMEVENT9(a)  __builtin_linx_csrrsi(0b001100101001, a)
#define RISCV_CSRRSI_MHPMEVENT10(a)  __builtin_linx_csrrsi(0b001100101010, a)
#define RISCV_CSRRSI_MHPMEVENT11(a)  __builtin_linx_csrrsi(0b001100101011, a)
#define RISCV_CSRRSI_MHPMEVENT12(a)  __builtin_linx_csrrsi(0b001100101100, a)
#define RISCV_CSRRSI_MHPMEVENT13(a)  __builtin_linx_csrrsi(0b001100101101, a)
#define RISCV_CSRRSI_MHPMEVENT14(a)  __builtin_linx_csrrsi(0b001100101110, a)
#define RISCV_CSRRSI_MHPMEVENT15(a)  __builtin_linx_csrrsi(0b001100101111, a)
#define RISCV_CSRRSI_MHPMEVENT16(a)  __builtin_linx_csrrsi(0b001100110000, a)
#define RISCV_CSRRSI_MHPMEVENT17(a)  __builtin_linx_csrrsi(0b001100110001, a)
#define RISCV_CSRRSI_MHPMEVENT18(a)  __builtin_linx_csrrsi(0b001100110010, a)
#define RISCV_CSRRSI_MHPMEVENT19(a)  __builtin_linx_csrrsi(0b001100110011, a)
#define RISCV_CSRRSI_MHPMEVENT20(a)  __builtin_linx_csrrsi(0b001100110100, a)
#define RISCV_CSRRSI_MHPMEVENT21(a)  __builtin_linx_csrrsi(0b001100110101, a)
#define RISCV_CSRRSI_MHPMEVENT22(a)  __builtin_linx_csrrsi(0b001100110110, a)
#define RISCV_CSRRSI_MHPMEVENT23(a)  __builtin_linx_csrrsi(0b001100110111, a)
#define RISCV_CSRRSI_MHPMEVENT24(a)  __builtin_linx_csrrsi(0b001100111000, a)
#define RISCV_CSRRSI_MHPMEVENT25(a)  __builtin_linx_csrrsi(0b001100111001, a)
#define RISCV_CSRRSI_MHPMEVENT26(a)  __builtin_linx_csrrsi(0b001100111010, a)
#define RISCV_CSRRSI_MHPMEVENT27(a)  __builtin_linx_csrrsi(0b001100111011, a)
#define RISCV_CSRRSI_MHPMEVENT28(a)  __builtin_linx_csrrsi(0b001100111100, a)
#define RISCV_CSRRSI_MHPMEVENT29(a)  __builtin_linx_csrrsi(0b001100111101, a)
#define RISCV_CSRRSI_MHPMEVENT30(a)  __builtin_linx_csrrsi(0b001100111110, a)
#define RISCV_CSRRSI_MHPMEVENT31(a)  __builtin_linx_csrrsi(0b001100111111, a)
#define RISCV_CSRRSI_MCYCLE(a)  __builtin_linx_csrrsi(0b101100000000, a)
#define RISCV_CSRRSI_MINSTRET(a)  __builtin_linx_csrrsi(0b101100000010, a)
#define RISCV_CSRRSI_MHPMCOUNTER3(a)  __builtin_linx_csrrsi(0b101100000011, a)
#define RISCV_CSRRSI_MHPMCOUNTER4(a)  __builtin_linx_csrrsi(0b101100000100, a)
#define RISCV_CSRRSI_MHPMCOUNTER5(a)  __builtin_linx_csrrsi(0b101100000101, a)
#define RISCV_CSRRSI_MHPMCOUNTER6(a)  __builtin_linx_csrrsi(0b101100000110, a)
#define RISCV_CSRRSI_MHPMCOUNTER7(a)  __builtin_linx_csrrsi(0b101100000111, a)
#define RISCV_CSRRSI_MHPMCOUNTER8(a)  __builtin_linx_csrrsi(0b101100001000, a)
#define RISCV_CSRRSI_MHPMCOUNTER9(a)  __builtin_linx_csrrsi(0b101100001001, a)
#define RISCV_CSRRSI_MHPMCOUNTER10(a)  __builtin_linx_csrrsi(0b101100001010, a)
#define RISCV_CSRRSI_MHPMCOUNTER11(a)  __builtin_linx_csrrsi(0b101100001011, a)
#define RISCV_CSRRSI_MHPMCOUNTER12(a)  __builtin_linx_csrrsi(0b101100001100, a)
#define RISCV_CSRRSI_MHPMCOUNTER13(a)  __builtin_linx_csrrsi(0b101100001101, a)
#define RISCV_CSRRSI_MHPMCOUNTER14(a)  __builtin_linx_csrrsi(0b101100001110, a)
#define RISCV_CSRRSI_MHPMCOUNTER15(a)  __builtin_linx_csrrsi(0b101100001111, a)
#define RISCV_CSRRSI_MHPMCOUNTER16(a)  __builtin_linx_csrrsi(0b101100010000, a)
#define RISCV_CSRRSI_MHPMCOUNTER17(a)  __builtin_linx_csrrsi(0b101100010001, a)
#define RISCV_CSRRSI_MHPMCOUNTER18(a)  __builtin_linx_csrrsi(0b101100010010, a)
#define RISCV_CSRRSI_MHPMCOUNTER19(a)  __builtin_linx_csrrsi(0b101100010011, a)
#define RISCV_CSRRSI_MHPMCOUNTER20(a)  __builtin_linx_csrrsi(0b101100010100, a)
#define RISCV_CSRRSI_MHPMCOUNTER21(a)  __builtin_linx_csrrsi(0b101100010101, a)
#define RISCV_CSRRSI_MHPMCOUNTER22(a)  __builtin_linx_csrrsi(0b101100010110, a)
#define RISCV_CSRRSI_MHPMCOUNTER23(a)  __builtin_linx_csrrsi(0b101100010111, a)
#define RISCV_CSRRSI_MHPMCOUNTER24(a)  __builtin_linx_csrrsi(0b101100011000, a)
#define RISCV_CSRRSI_MHPMCOUNTER25(a)  __builtin_linx_csrrsi(0b101100011001, a)
#define RISCV_CSRRSI_MHPMCOUNTER26(a)  __builtin_linx_csrrsi(0b101100011010, a)
#define RISCV_CSRRSI_MHPMCOUNTER27(a)  __builtin_linx_csrrsi(0b101100011011, a)
#define RISCV_CSRRSI_MHPMCOUNTER28(a)  __builtin_linx_csrrsi(0b101100011100, a)
#define RISCV_CSRRSI_MHPMCOUNTER29(a)  __builtin_linx_csrrsi(0b101100011101, a)
#define RISCV_CSRRSI_MHPMCOUNTER30(a)  __builtin_linx_csrrsi(0b101100011110, a)
#define RISCV_CSRRSI_MHPMCOUNTER31(a)  __builtin_linx_csrrsi(0b101100011111, a)
#define RISCV_CSRRSI_MCYCLEH(a)  __builtin_linx_csrrsi(0b101110000000, a)
#define RISCV_CSRRSI_MINSTRETH(a)  __builtin_linx_csrrsi(0b101110000010, a)
#define RISCV_CSRRSI_MHPMCOUNTER3H(a)  __builtin_linx_csrrsi(0b101110000011, a)
#define RISCV_CSRRSI_MHPMCOUNTER4H(a)  __builtin_linx_csrrsi(0b101110000100, a)
#define RISCV_CSRRSI_MHPMCOUNTER5H(a)  __builtin_linx_csrrsi(0b101110000101, a)
#define RISCV_CSRRSI_MHPMCOUNTER6H(a)  __builtin_linx_csrrsi(0b101110000110, a)
#define RISCV_CSRRSI_MHPMCOUNTER7H(a)  __builtin_linx_csrrsi(0b101110000111, a)
#define RISCV_CSRRSI_MHPMCOUNTER8H(a)  __builtin_linx_csrrsi(0b101110001000, a)
#define RISCV_CSRRSI_MHPMCOUNTER9H(a)  __builtin_linx_csrrsi(0b101110001001, a)
#define RISCV_CSRRSI_MHPMCOUNTER10H(a)  __builtin_linx_csrrsi(0b101110001010, a)
#define RISCV_CSRRSI_MHPMCOUNTER11H(a)  __builtin_linx_csrrsi(0b101110001011, a)
#define RISCV_CSRRSI_MHPMCOUNTER12H(a)  __builtin_linx_csrrsi(0b101110001100, a)
#define RISCV_CSRRSI_MHPMCOUNTER13H(a)  __builtin_linx_csrrsi(0b101110001101, a)
#define RISCV_CSRRSI_MHPMCOUNTER14H(a)  __builtin_linx_csrrsi(0b101110001110, a)
#define RISCV_CSRRSI_MHPMCOUNTER15H(a)  __builtin_linx_csrrsi(0b101110001111, a)
#define RISCV_CSRRSI_MHPMCOUNTER16H(a)  __builtin_linx_csrrsi(0b101110010000, a)
#define RISCV_CSRRSI_MHPMCOUNTER17H(a)  __builtin_linx_csrrsi(0b101110010001, a)
#define RISCV_CSRRSI_MHPMCOUNTER18H(a)  __builtin_linx_csrrsi(0b101110010010, a)
#define RISCV_CSRRSI_MHPMCOUNTER19H(a)  __builtin_linx_csrrsi(0b101110010011, a)
#define RISCV_CSRRSI_MHPMCOUNTER20H(a)  __builtin_linx_csrrsi(0b101110010100, a)
#define RISCV_CSRRSI_MHPMCOUNTER21H(a)  __builtin_linx_csrrsi(0b101110010101, a)
#define RISCV_CSRRSI_MHPMCOUNTER22H(a)  __builtin_linx_csrrsi(0b101110010110, a)
#define RISCV_CSRRSI_MHPMCOUNTER23H(a)  __builtin_linx_csrrsi(0b101110010111, a)
#define RISCV_CSRRSI_MHPMCOUNTER24H(a)  __builtin_linx_csrrsi(0b101110011000, a)
#define RISCV_CSRRSI_MHPMCOUNTER25H(a)  __builtin_linx_csrrsi(0b101110011001, a)
#define RISCV_CSRRSI_MHPMCOUNTER26H(a)  __builtin_linx_csrrsi(0b101110011010, a)
#define RISCV_CSRRSI_MHPMCOUNTER27H(a)  __builtin_linx_csrrsi(0b101110011011, a)
#define RISCV_CSRRSI_MHPMCOUNTER28H(a)  __builtin_linx_csrrsi(0b101110011100, a)
#define RISCV_CSRRSI_MHPMCOUNTER29H(a)  __builtin_linx_csrrsi(0b101110011101, a)
#define RISCV_CSRRSI_MHPMCOUNTER30H(a)  __builtin_linx_csrrsi(0b101110011110, a)
#define RISCV_CSRRSI_MHPMCOUNTER31H(a)  __builtin_linx_csrrsi(0b101110011111, a)
#define RISCV_CSRRSI_ICCTL(a)  __builtin_linx_csrrsi(0b011111000000, a)
#define RISCV_CSRRSI_DCCTL(a)  __builtin_linx_csrrsi(0b011111000001, a)
#define RISCV_CSRRSI_ICMAINT(a)  __builtin_linx_csrrsi(0b011111000010, a)
#define RISCV_CSRRSI_DCMAINT(a)  __builtin_linx_csrrsi(0b011111000011, a)
#define RISCV_CSRRSI_ICINVA(a)  __builtin_linx_csrrsi(0b011111000100, a)
#define RISCV_CSRRSI_DCINCVA(a)  __builtin_linx_csrrsi(0b011111000101, a)
#define RISCV_CSRRSI_APREFI(a)  __builtin_linx_csrrsi(0b011111000110, a)
#define RISCV_CSRRSI_APREFD(a)  __builtin_linx_csrrsi(0b011111000111, a)
#define RISCV_CSRRSI_WST(a)  __builtin_linx_csrrsi(0b011111010000, a)
#define RISCV_CSRRSI_TCMARB(a)  __builtin_linx_csrrsi(0b011111010010, a)
#define RISCV_CSRRSI_MEMATTR0(a)  __builtin_linx_csrrsi(0b101111000000, a)
#define RISCV_CSRRSI_MEMATTR1(a)  __builtin_linx_csrrsi(0b101111000001, a)
#define RISCV_CSRRSI_MEMATTR2(a)  __builtin_linx_csrrsi(0b101111000010, a)
#define RISCV_CSRRSI_MEMATTR3(a)  __builtin_linx_csrrsi(0b101111000011, a)
#define RISCV_CSRRSI_MEMATTR4(a)  __builtin_linx_csrrsi(0b101111000100, a)
#define RISCV_CSRRSI_MEMATTR5(a)  __builtin_linx_csrrsi(0b101111000101, a)
#define RISCV_CSRRSI_MEMATTR6(a)  __builtin_linx_csrrsi(0b101111000110, a)
#define RISCV_CSRRSI_MEMATTR7(a)  __builtin_linx_csrrsi(0b101111000111, a)
#define RISCV_CSRRSI_MEMATTR8(a)  __builtin_linx_csrrsi(0b101111001000, a)
#define RISCV_CSRRSI_MEMATTR9(a)  __builtin_linx_csrrsi(0b101111001001, a)
#define RISCV_CSRRSI_MEMATTR10(a)  __builtin_linx_csrrsi(0b101111001010, a)
#define RISCV_CSRRSI_MEMATTR11(a)  __builtin_linx_csrrsi(0b101111001011, a)
#define RISCV_CSRRSI_MEMATTR12(a)  __builtin_linx_csrrsi(0b101111001100, a)
#define RISCV_CSRRSI_MEMATTR13(a)  __builtin_linx_csrrsi(0b101111001101, a)
#define RISCV_CSRRSI_MEMATTR14(a)  __builtin_linx_csrrsi(0b101111001110, a)
#define RISCV_CSRRSI_MEMATTR15(a)  __builtin_linx_csrrsi(0b101111001111, a)
#define RISCV_CSRRSI_MEMATTR16(a)  __builtin_linx_csrrsi(0b101111010000, a)
#define RISCV_CSRRSI_MEMATTR17(a)  __builtin_linx_csrrsi(0b101111010001, a)
#define RISCV_CSRRSI_MEMATTR18(a)  __builtin_linx_csrrsi(0b101111010010, a)
#define RISCV_CSRRSI_MEMATTR19(a)  __builtin_linx_csrrsi(0b101111010011, a)
#define RISCV_CSRRSI_MEMATTR20(a)  __builtin_linx_csrrsi(0b101111010100, a)
#define RISCV_CSRRSI_MEMATTR21(a)  __builtin_linx_csrrsi(0b101111010101, a)
#define RISCV_CSRRSI_MEMATTR22(a)  __builtin_linx_csrrsi(0b101111010110, a)
#define RISCV_CSRRSI_MEMATTR23(a)  __builtin_linx_csrrsi(0b101111010111, a)
#define RISCV_CSRRSI_MEMATTR24(a)  __builtin_linx_csrrsi(0b101111011000, a)
#define RISCV_CSRRSI_MEMATTR25(a)  __builtin_linx_csrrsi(0b101111011001, a)
#define RISCV_CSRRSI_MEMATTR26(a)  __builtin_linx_csrrsi(0b101111011010, a)
#define RISCV_CSRRSI_MEMATTR27(a)  __builtin_linx_csrrsi(0b101111011011, a)
#define RISCV_CSRRSI_MEMATTR28(a)  __builtin_linx_csrrsi(0b101111011100, a)
#define RISCV_CSRRSI_MEMATTR29(a)  __builtin_linx_csrrsi(0b101111011101, a)
#define RISCV_CSRRSI_MEMATTR30(a)  __builtin_linx_csrrsi(0b101111011110, a)
#define RISCV_CSRRSI_MEMATTR31(a)  __builtin_linx_csrrsi(0b101111011111, a)
#define RISCV_CSRRSI_MEMATTR32(a)  __builtin_linx_csrrsi(0b101111100000, a)
#define RISCV_CSRRSI_MEMATTR33(a)  __builtin_linx_csrrsi(0b101111100001, a)
#define RISCV_CSRRSI_MEMATTR34(a)  __builtin_linx_csrrsi(0b101111100010, a)
#define RISCV_CSRRSI_MEMATTR35(a)  __builtin_linx_csrrsi(0b101111100011, a)
#define RISCV_CSRRSI_MEMATTR36(a)  __builtin_linx_csrrsi(0b101111100100, a)
#define RISCV_CSRRSI_MEMATTR37(a)  __builtin_linx_csrrsi(0b101111100101, a)
#define RISCV_CSRRSI_MEMATTR38(a)  __builtin_linx_csrrsi(0b101111100110, a)
#define RISCV_CSRRSI_MEMATTR39(a)  __builtin_linx_csrrsi(0b101111100111, a)
#define RISCV_CSRRSI_MEMATTR40(a)  __builtin_linx_csrrsi(0b101111101000, a)
#define RISCV_CSRRSI_MEMATTR41(a)  __builtin_linx_csrrsi(0b101111101001, a)
#define RISCV_CSRRSI_MEMATTR42(a)  __builtin_linx_csrrsi(0b101111101010, a)
#define RISCV_CSRRSI_MEMATTR43(a)  __builtin_linx_csrrsi(0b101111101011, a)
#define RISCV_CSRRSI_MEMATTR44(a)  __builtin_linx_csrrsi(0b101111101100, a)
#define RISCV_CSRRSI_MEMATTR45(a)  __builtin_linx_csrrsi(0b101111101101, a)
#define RISCV_CSRRSI_MEMATTR46(a)  __builtin_linx_csrrsi(0b101111101110, a)
#define RISCV_CSRRSI_MEMATTR47(a)  __builtin_linx_csrrsi(0b101111101111, a)
#define RISCV_CSRRSI_MEMATTR48(a)  __builtin_linx_csrrsi(0b101111110000, a)
#define RISCV_CSRRSI_MEMATTR49(a)  __builtin_linx_csrrsi(0b101111110001, a)
#define RISCV_CSRRSI_MEMATTR50(a)  __builtin_linx_csrrsi(0b101111110010, a)
#define RISCV_CSRRSI_MEMATTR51(a)  __builtin_linx_csrrsi(0b101111110011, a)
#define RISCV_CSRRSI_MEMATTR52(a)  __builtin_linx_csrrsi(0b101111110100, a)
#define RISCV_CSRRSI_MEMATTR53(a)  __builtin_linx_csrrsi(0b101111110101, a)
#define RISCV_CSRRSI_MEMATTR54(a)  __builtin_linx_csrrsi(0b101111110110, a)
#define RISCV_CSRRSI_MEMATTR55(a)  __builtin_linx_csrrsi(0b101111110111, a)
#define RISCV_CSRRSI_MEMATTR56(a)  __builtin_linx_csrrsi(0b101111111000, a)
#define RISCV_CSRRSI_MEMATTR57(a)  __builtin_linx_csrrsi(0b101111111001, a)
#define RISCV_CSRRSI_MEMATTR58(a)  __builtin_linx_csrrsi(0b101111111010, a)
#define RISCV_CSRRSI_MEMATTR59(a)  __builtin_linx_csrrsi(0b101111111011, a)
#define RISCV_CSRRSI_MEMATTR60(a)  __builtin_linx_csrrsi(0b101111111100, a)
#define RISCV_CSRRSI_MEMATTR61(a)  __builtin_linx_csrrsi(0b101111111101, a)
#define RISCV_CSRRSI_MEMATTR62(a)  __builtin_linx_csrrsi(0b101111111110, a)
#define RISCV_CSRRSI_MEMATTR63(a)  __builtin_linx_csrrsi(0b101111111111, a)
#define RISCV_CSRRSI_PORPC(a)  __builtin_linx_csrrsi(0b111111000000, a)
#define RISCV_CSRRSI_WTOPC(a)  __builtin_linx_csrrsi(0b111111000001, a)
#define RISCV_CSRRSI_CXCPTSC(a)  __builtin_linx_csrrsi(0b111111000010, a)
#define RISCV_CSRRSI_DBGUOPCNT(a)  __builtin_linx_csrrsi(0b011111111110, a)
#define RISCV_CSRRSI_FFLAGS(a)  __builtin_linx_csrrsi(0b000000000001, a)
#define RISCV_CSRRSI_FRM(a)  __builtin_linx_csrrsi(0b000000000010, a)
#define RISCV_CSRRSI_FCSR(a)  __builtin_linx_csrrsi(0b000000000011, a)
#define RISCV_CSRRSI_HPMCOUNTER3(a)  __builtin_linx_csrrsi(0b110000000011, a)
#define RISCV_CSRRSI_HPMCOUNTER4(a)  __builtin_linx_csrrsi(0b110000000100, a)
#define RISCV_CSRRSI_HPMCOUNTER5(a)  __builtin_linx_csrrsi(0b110000000101, a)
#define RISCV_CSRRSI_HPMCOUNTER6(a)  __builtin_linx_csrrsi(0b110000000110, a)
#define RISCV_CSRRSI_HPMCOUNTER7(a)  __builtin_linx_csrrsi(0b110000000111, a)
#define RISCV_CSRRSI_HPMCOUNTER8(a)  __builtin_linx_csrrsi(0b110000001000, a)
#define RISCV_CSRRSI_HPMCOUNTER9(a)  __builtin_linx_csrrsi(0b110000001001, a)
#define RISCV_CSRRSI_HPMCOUNTER10(a)  __builtin_linx_csrrsi(0b110000001010, a)
#define RISCV_CSRRSI_HPMCOUNTER11(a)  __builtin_linx_csrrsi(0b110000001011, a)
#define RISCV_CSRRSI_HPMCOUNTER12(a)  __builtin_linx_csrrsi(0b110000001100, a)
#define RISCV_CSRRSI_HPMCOUNTER13(a)  __builtin_linx_csrrsi(0b110000001101, a)
#define RISCV_CSRRSI_HPMCOUNTER14(a)  __builtin_linx_csrrsi(0b110000001110, a)
#define RISCV_CSRRSI_HPMCOUNTER15(a)  __builtin_linx_csrrsi(0b110000001111, a)
#define RISCV_CSRRSI_HPMCOUNTER16(a)  __builtin_linx_csrrsi(0b110000010000, a)
#define RISCV_CSRRSI_HPMCOUNTER17(a)  __builtin_linx_csrrsi(0b110000010001, a)
#define RISCV_CSRRSI_HPMCOUNTER18(a)  __builtin_linx_csrrsi(0b110000010010, a)
#define RISCV_CSRRSI_HPMCOUNTER19(a)  __builtin_linx_csrrsi(0b110000010011, a)
#define RISCV_CSRRSI_HPMCOUNTER20(a)  __builtin_linx_csrrsi(0b110000010100, a)
#define RISCV_CSRRSI_HPMCOUNTER21(a)  __builtin_linx_csrrsi(0b110000010101, a)
#define RISCV_CSRRSI_HPMCOUNTER22(a)  __builtin_linx_csrrsi(0b110000010110, a)
#define RISCV_CSRRSI_HPMCOUNTER23(a)  __builtin_linx_csrrsi(0b110000010111, a)
#define RISCV_CSRRSI_HPMCOUNTER24(a)  __builtin_linx_csrrsi(0b110000011000, a)
#define RISCV_CSRRSI_HPMCOUNTER25(a)  __builtin_linx_csrrsi(0b110000011001, a)
#define RISCV_CSRRSI_HPMCOUNTER26(a)  __builtin_linx_csrrsi(0b110000011010, a)
#define RISCV_CSRRSI_HPMCOUNTER27(a)  __builtin_linx_csrrsi(0b110000011011, a)
#define RISCV_CSRRSI_HPMCOUNTER28(a)  __builtin_linx_csrrsi(0b110000011100, a)
#define RISCV_CSRRSI_HPMCOUNTER29(a)  __builtin_linx_csrrsi(0b110000011101, a)
#define RISCV_CSRRSI_HPMCOUNTER30(a)  __builtin_linx_csrrsi(0b110000011110, a)
#define RISCV_CSRRSI_HPMCOUNTER31(a)  __builtin_linx_csrrsi(0b110000011111, a)
#define RISCV_CSRRSI_CYCLE(a)  __builtin_linx_csrrsi(0b110000000000, a)
#define RISCV_CSRRSI_TIME(a)  __builtin_linx_csrrsi(0b110000000001, a)
#define RISCV_CSRRSI_INSTRET(a)  __builtin_linx_csrrsi(0b110000000010, a)
#define RISCV_CSRRSI_HPMCOUNTER3H(a)  __builtin_linx_csrrsi(0b110010000011, a)
#define RISCV_CSRRSI_HPMCOUNTER4H(a)  __builtin_linx_csrrsi(0b110010000100, a)
#define RISCV_CSRRSI_HPMCOUNTER5H(a)  __builtin_linx_csrrsi(0b110010000101, a)
#define RISCV_CSRRSI_HPMCOUNTER6H(a)  __builtin_linx_csrrsi(0b110010000110, a)
#define RISCV_CSRRSI_HPMCOUNTER7H(a)  __builtin_linx_csrrsi(0b110010000111, a)
#define RISCV_CSRRSI_HPMCOUNTER8H(a)  __builtin_linx_csrrsi(0b110010001000, a)
#define RISCV_CSRRSI_HPMCOUNTER9H(a)  __builtin_linx_csrrsi(0b110010001001, a)
#define RISCV_CSRRSI_HPMCOUNTER10H(a)  __builtin_linx_csrrsi(0b110010001010, a)
#define RISCV_CSRRSI_HPMCOUNTER11H(a)  __builtin_linx_csrrsi(0b110010001011, a)
#define RISCV_CSRRSI_HPMCOUNTER12H(a)  __builtin_linx_csrrsi(0b110010001100, a)
#define RISCV_CSRRSI_HPMCOUNTER13H(a)  __builtin_linx_csrrsi(0b110010001101, a)
#define RISCV_CSRRSI_HPMCOUNTER14H(a)  __builtin_linx_csrrsi(0b110010001110, a)
#define RISCV_CSRRSI_HPMCOUNTER15H(a)  __builtin_linx_csrrsi(0b110010001111, a)
#define RISCV_CSRRSI_HPMCOUNTER16H(a)  __builtin_linx_csrrsi(0b110010010000, a)
#define RISCV_CSRRSI_HPMCOUNTER17H(a)  __builtin_linx_csrrsi(0b110010010001, a)
#define RISCV_CSRRSI_HPMCOUNTER18H(a)  __builtin_linx_csrrsi(0b110010010010, a)
#define RISCV_CSRRSI_HPMCOUNTER19H(a)  __builtin_linx_csrrsi(0b110010010011, a)
#define RISCV_CSRRSI_HPMCOUNTER20H(a)  __builtin_linx_csrrsi(0b110010010100, a)
#define RISCV_CSRRSI_HPMCOUNTER21H(a)  __builtin_linx_csrrsi(0b110010010101, a)
#define RISCV_CSRRSI_HPMCOUNTER22H(a)  __builtin_linx_csrrsi(0b110010010110, a)
#define RISCV_CSRRSI_HPMCOUNTER23H(a)  __builtin_linx_csrrsi(0b110010010111, a)
#define RISCV_CSRRSI_HPMCOUNTER24H(a)  __builtin_linx_csrrsi(0b110010011000, a)
#define RISCV_CSRRSI_HPMCOUNTER25H(a)  __builtin_linx_csrrsi(0b110010011001, a)
#define RISCV_CSRRSI_HPMCOUNTER26H(a)  __builtin_linx_csrrsi(0b110010011010, a)
#define RISCV_CSRRSI_HPMCOUNTER27H(a)  __builtin_linx_csrrsi(0b110010011011, a)
#define RISCV_CSRRSI_HPMCOUNTER28H(a)  __builtin_linx_csrrsi(0b110010011100, a)
#define RISCV_CSRRSI_HPMCOUNTER29H(a)  __builtin_linx_csrrsi(0b110010011101, a)
#define RISCV_CSRRSI_HPMCOUNTER30H(a)  __builtin_linx_csrrsi(0b110010011110, a)
#define RISCV_CSRRSI_HPMCOUNTER31H(a)  __builtin_linx_csrrsi(0b110010011111, a)
#define RISCV_CSRRSI_CYCLEH(a)  __builtin_linx_csrrsi(0b110010000000, a)
#define RISCV_CSRRSI_TIMEH(a)  __builtin_linx_csrrsi(0b110010000001, a)
#define RISCV_CSRRSI_INSTRETH(a)  __builtin_linx_csrrsi(0b110010000010, a)
#define RISCV_CSRRSI_TMESCR(a)  __builtin_linx_csrrsi(0b011111100000, a)
#define RISCV_CSRRSI_TMESVEC(a)  __builtin_linx_csrrsi(0b011111100001, a)
#define RISCV_CSRRSI_TMESTOP(a)  __builtin_linx_csrrsi(0b011111100010, a)
#define RISCV_CSRRSI_TMEDELEG(a)  __builtin_linx_csrrsi(0b011111100011, a)
#define RISCV_CSRRSI_TMTVEC(a)  __builtin_linx_csrrsi(0b011111100100, a)
#define RISCV_CSRRSI_TMTVT(a)  __builtin_linx_csrrsi(0b011111100101, a)
#define RISCV_CSRRSI_TMINTTHRESH(a)  __builtin_linx_csrrsi(0b011111100110, a)
#define RISCV_CSRRSI_TMSTATUS(a)  __builtin_linx_csrrsi(0b011111100111, a)
#define RISCV_CSRRSI_TMEPC(a)  __builtin_linx_csrrsi(0b011111101000, a)
#define RISCV_CSRRSI_TMCAUSE(a)  __builtin_linx_csrrsi(0b011111101001, a)
#define RISCV_CSRRSI_TCXCPTSC(a)  __builtin_linx_csrrsi(0b111111000011, a)
#define RISCV_CSRRSI_TMTVAL(a)  __builtin_linx_csrrsi(0b011111101011, a)
#define RISCV_CSRRSI_TMSCRATCH(a)  __builtin_linx_csrrsi(0b011111101100, a)
#define RISCV_CSRRSI_TMESEPR(a)  __builtin_linx_csrrsi(0b011111101101, a)
#define RISCV_CSRRSI_TMESEPS(a)  __builtin_linx_csrrsi(0b011111101110, a)
#define RISCV_CSRRSI_MSECCTL(a)  __builtin_linx_csrrsi(0b011111101111, a)
#define RISCV_CSRRSI_PMPTE0(a)  __builtin_linx_csrrsi(0b011111110000, a)
#define RISCV_CSRRSI_PMPTE1(a)  __builtin_linx_csrrsi(0b011111110001, a)
#define RISCV_CSRRSI_MSPLIMIT(a)  __builtin_linx_csrrsi(0b011111110010, a)
#define RISCV_CSRRSI_TMSPLIMIT(a)  __builtin_linx_csrrsi(0b011111110011, a)
#define RISCV_CSRRSI_TUSP(a)  __builtin_linx_csrrsi(0b100000000000, a)
#define RISCV_CSRRSI_TUGP(a)  __builtin_linx_csrrsi(0b100000000001, a)
#define RISCV_CSRRSI_TUTP(a)  __builtin_linx_csrrsi(0b100000000010, a)
#define RISCV_CSRRSI_TSP(a)  __builtin_linx_csrrsi(0b100000000011, a)
#define RISCV_CSRRSI_TGP(a)  __builtin_linx_csrrsi(0b100000000100, a)
#define RISCV_CSRRSI_TTP(a)  __builtin_linx_csrrsi(0b100000000101, a)
#define RISCV_CSRRSI_TESEPR(a)  __builtin_linx_csrrsi(0b110011000000, a)
#define RISCV_CSRRSI_TESEPS(a)  __builtin_linx_csrrsi(0b110011000001, a)
#define RISCV_CSRRSI_VXSAT(a)  __builtin_linx_csrrsi(0b000000001001, a)
#define RISCV_CSRRSI_VXRM(a)  __builtin_linx_csrrsi(0b000000001010, a)
#define RISCV_CSRRSI_VCSR(a)  __builtin_linx_csrrsi(0b000000001111, a)
#define RISCV_CSRRSI_VSTART(a)  __builtin_linx_csrrsi(0b000000001000, a)
#define RISCV_CSRRSI_VL(a)  __builtin_linx_csrrsi(0b110000100000, a)
#define RISCV_CSRRSI_VTYPE(a)  __builtin_linx_csrrsi(0b110000100001, a)
#define RISCV_CSRRSI_VLENB(a)  __builtin_linx_csrrsi(0b110000100010, a)
#define RISCV_CSRRSI_JBC(a)  __builtin_linx_csrrsi(0b100111110000, a)
#define RISCV_CSRRSI_JPC_UPPER(a)  __builtin_linx_csrrsi(0b100111110001, a)
#define RISCV_CSRRSI_JPC_LOWER(a)  __builtin_linx_csrrsi(0b100111110010, a)
#define RISCV_CSRRSI_JBCFT(a)  __builtin_linx_csrrsi(0b100111110011, a)
#define RISCV_CSRRSI_JBC_UPPER(a)  __builtin_linx_csrrsi(0b100111110100, a)
#define RISCV_CSRRSI_JBC_SHADOW(a)  __builtin_linx_csrrsi(0b110011110000, a)
#define RISCV_CSRRSI_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrsi(0b100111111111, a)
#define RISCV_CSRRSI_TSELECT(a)  __builtin_linx_csrrsi(0b011110100000, a)
#define RISCV_CSRRSI_TDATA1(a)  __builtin_linx_csrrsi(0b011110100001, a)
#define RISCV_CSRRSI_TDATA2(a)  __builtin_linx_csrrsi(0b011110100010, a)
#define RISCV_CSRRSI_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRSI_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRSI_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRSI_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRSI_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRSI_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRSI_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRSI_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRSI_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRSI_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRSI_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRSI_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRSI_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRSI_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRSI_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRSI_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRSI_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRSI_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRSI_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRSI_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRSI_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRSI_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRSI_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRSI_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRSI_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRSI_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRSI_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRSI_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRSI_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRSI_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRSI_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRSI_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRSI_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRSI_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRSI_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRSI_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRSI_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRSI_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRSI_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRSI_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRSI_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRSI_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRSI_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRSI_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRSI_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRSI_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRSI_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRSI_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRSI_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRSI_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRSI_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRSI_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRSI_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRSI_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRSI_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRSI_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRSI_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRSI_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRSI_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRSI_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRSI_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRSI_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRSI_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRSI_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRSI_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRSI_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRSI_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRSI_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRSI_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRSI_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRSI_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRSI_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRSI_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRSI_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRSI_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRSI_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRSI_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRSI_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRSI_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRSI_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRSI_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRSI_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRW  __builtin_linx_csrrw
#define RISCV_CSRRW_DCSR(a)  __builtin_linx_csrrw(0b011110110000, a)
#define RISCV_CSRRW_DCSR_PRV(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~3) | (a << 0)) & 3) >> 0
#define RISCV_CSRRW_DCSR_STEP(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_DCSR_NMIP(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_DCSR_MPRVEN(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_DCSR_DCSR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_DCSR_CAUSE(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~448) | (a << 6)) & 448) >> 6
#define RISCV_CSRRW_DCSR_STOPTIME(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_DCSR_STOPCOUNT(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_DCSR_STEPIE(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_DCSR_EBREAKU(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~4096) | (a << 12)) & 4096) >> 12
#define RISCV_CSRRW_DCSR_DCSR_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_DCSR_EBREAKM(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_DCSR_DCSR_RESERVED2(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~268369920) | (a << 16)) & 268369920) >> 16
#define RISCV_CSRRW_DCSR_XDEBUGVER(a)  (unsigned int)(RISCV_CSRRW_DCSR((RISCV_CSRRS_DCSR(0) & ~4026531840) | (a << 28)) & 4026531840) >> 28
#define RISCV_CSRRW_DPC(a)  __builtin_linx_csrrw(0b011110110001, a)
#define RISCV_CSRRW_DSCRATCH0(a)  __builtin_linx_csrrw(0b011110110010, a)
#define RISCV_CSRRW_MVENDORID(a)  __builtin_linx_csrrw(0b111100010001, a)
#define RISCV_CSRRW_MVENDORID_MVENDORID_OFFSET(a)  (unsigned int)(RISCV_CSRRW_MVENDORID((RISCV_CSRRS_MVENDORID(0) & ~127) | (a << 0)) & 127) >> 0
#define RISCV_CSRRW_MVENDORID_MVENDORID_BANK(a)  (unsigned int)(RISCV_CSRRW_MVENDORID((RISCV_CSRRS_MVENDORID(0) & ~4294967168) | (a << 7)) & 4294967168) >> 7
#define RISCV_CSRRW_MARCHID(a)  __builtin_linx_csrrw(0b111100010010, a)
#define RISCV_CSRRW_MIMPID(a)  __builtin_linx_csrrw(0b111100010011, a)
#define RISCV_CSRRW_MHARTID(a)  __builtin_linx_csrrw(0b111100010100, a)
#define RISCV_CSRRW_MSTATUS(a)  __builtin_linx_csrrw(0b001100000000, a)
#define RISCV_CSRRW_MSTATUS_MSTATUS_UIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MSTATUS_MSTATUS_SIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MSTATUS_MSTATUS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MSTATUS_MSTATUS_MIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MSTATUS_MSTATUS_UPIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MSTATUS_MSTATUS_SPIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MSTATUS_MSTATUS_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MSTATUS_MSTATUS_MPIE(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MSTATUS_MSTATUS_SPP(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MSTATUS_MSTATUS_VS(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~1536) | (a << 9)) & 1536) >> 9
#define RISCV_CSRRW_MSTATUS_MSTATUS_MPP(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MSTATUS_MSTATUS_FS(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MSTATUS_MSTATUS_XS(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~98304) | (a << 15)) & 98304) >> 15
#define RISCV_CSRRW_MSTATUS_MSTATUS_MPRV(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MSTATUS_MSTATUS_SUM(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MSTATUS_MSTATUS_MXR(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MSTATUS_MSTATUS_TVM(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~1048576) | (a << 20)) & 1048576) >> 20
#define RISCV_CSRRW_MSTATUS_MSTATUS_TW(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~2097152) | (a << 21)) & 2097152) >> 21
#define RISCV_CSRRW_MSTATUS_MSTATUS_TSR(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~4194304) | (a << 22)) & 4194304) >> 22
#define RISCV_CSRRW_MSTATUS_MSTATUS_RESERVED2(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~2139095040) | (a << 23)) & 2139095040) >> 23
#define RISCV_CSRRW_MSTATUS_MSTATUS_SD(a)  (unsigned int)(RISCV_CSRRW_MSTATUS((RISCV_CSRRS_MSTATUS(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_MISA(a)  __builtin_linx_csrrw(0b001100000001, a)
#define RISCV_CSRRW_MISA_MISA_A(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MISA_MISA_B(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MISA_MISA_C(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MISA_MISA_D(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MISA_MISA_E(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MISA_MISA_F(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MISA_MISA_G(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MISA_MISA_H(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MISA_MISA_I(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MISA_MISA_J(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MISA_MISA_K(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MISA_MISA_L(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_MISA_MISA_M(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~4096) | (a << 12)) & 4096) >> 12
#define RISCV_CSRRW_MISA_MISA_N(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~8192) | (a << 13)) & 8192) >> 13
#define RISCV_CSRRW_MISA_MISA_O(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~16384) | (a << 14)) & 16384) >> 14
#define RISCV_CSRRW_MISA_MISA_P(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MISA_MISA_Q(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~65536) | (a << 16)) & 65536) >> 16
#define RISCV_CSRRW_MISA_MISA_R(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MISA_MISA_S(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MISA_MISA_T(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MISA_MISA_U(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~1048576) | (a << 20)) & 1048576) >> 20
#define RISCV_CSRRW_MISA_MISA_V(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~2097152) | (a << 21)) & 2097152) >> 21
#define RISCV_CSRRW_MISA_MISA_W(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~4194304) | (a << 22)) & 4194304) >> 22
#define RISCV_CSRRW_MISA_MISA_X(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~8388608) | (a << 23)) & 8388608) >> 23
#define RISCV_CSRRW_MISA_MISA_Y(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_MISA_MISA_Z(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~33554432) | (a << 25)) & 33554432) >> 25
#define RISCV_CSRRW_MISA_MISA_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~1006632960) | (a << 26)) & 1006632960) >> 26
#define RISCV_CSRRW_MISA_MISA_MXL(a)  (unsigned int)(RISCV_CSRRW_MISA((RISCV_CSRRS_MISA(0) & ~3221225472) | (a << 30)) & 3221225472) >> 30
#define RISCV_CSRRW_MIE(a)  __builtin_linx_csrrw(0b001100000100, a)
#define RISCV_CSRRW_MIE_MIE_USIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MIE_MIE_SSIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MIE_MIE_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MIE_MIE_MSIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MIE_MIE_UTIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MIE_MIE_STIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MIE_MIE_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MIE_MIE_MTIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MIE_MIE_UEIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MIE_MIE_SEIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MIE_MIE_RESERVED2(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MIE_MIE_MEIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_MIE_MIE_RESERVED3(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~61440) | (a << 12)) & 61440) >> 12
#define RISCV_CSRRW_MIE_MIE_ULIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~65536) | (a << 16)) & 65536) >> 16
#define RISCV_CSRRW_MIE_MIE_SLIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MIE_MIE_RESERVED4(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MIE_MIE_MLIE(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MIE_MIE_RESERVED5(a)  (unsigned int)(RISCV_CSRRW_MIE((RISCV_CSRRS_MIE(0) & ~4293918720) | (a << 20)) & 4293918720) >> 20
#define RISCV_CSRRW_MTVEC(a)  __builtin_linx_csrrw(0b001100000101, a)
#define RISCV_CSRRW_MTVEC_MTVEC_MODE(a)  (unsigned int)(RISCV_CSRRW_MTVEC((RISCV_CSRRS_MTVEC(0) & ~63) | (a << 0)) & 63) >> 0
#define RISCV_CSRRW_MTVEC_MTVEC_BASE(a)  (unsigned int)(RISCV_CSRRW_MTVEC((RISCV_CSRRS_MTVEC(0) & ~4294967232) | (a << 6)) & 4294967232) >> 6
#define RISCV_CSRRW_MIP(a)  __builtin_linx_csrrw(0b001101000100, a)
#define RISCV_CSRRW_MIP_MIP_USIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MIP_MIP_SSIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MIP_MIP_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MIP_MIP_MSIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MIP_MIP_UTIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MIP_MIP_STIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MIP_MIP_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MIP_MIP_MTIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MIP_MIP_UEIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MIP_MIP_SEIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MIP_MIP_RESERVED2(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MIP_MIP_MEIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_MIP_MIP_RESERVED3(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~61440) | (a << 12)) & 61440) >> 12
#define RISCV_CSRRW_MIP_MIP_ULIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~65536) | (a << 16)) & 65536) >> 16
#define RISCV_CSRRW_MIP_MIP_SLIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MIP_MIP_RESERVED4(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MIP_MIP_MLIP(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MIP_MIP_RESERVED5(a)  (unsigned int)(RISCV_CSRRW_MIP((RISCV_CSRRS_MIP(0) & ~4293918720) | (a << 20)) & 4293918720) >> 20
#define RISCV_CSRRW_MSECCFG(a)  __builtin_linx_csrrw(0b001110010000, a)
#define RISCV_CSRRW_MSECCFG_MSECCFG_MML(a)  (unsigned int)(RISCV_CSRRW_MSECCFG((RISCV_CSRRS_MSECCFG(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MSECCFG_MSECCFG_MMWP(a)  (unsigned int)(RISCV_CSRRW_MSECCFG((RISCV_CSRRS_MSECCFG(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MSECCFG_MSECCFG_RLB(a)  (unsigned int)(RISCV_CSRRW_MSECCFG((RISCV_CSRRS_MSECCFG(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MSECCFG_MSECCFG_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MSECCFG((RISCV_CSRRS_MSECCFG(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_MCOUNTEREN(a)  __builtin_linx_csrrw(0b001100000110, a)
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_CY(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_TM(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_IR(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM3(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM4(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM5(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM6(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM7(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM8(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM9(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM10(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM11(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM12(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~4096) | (a << 12)) & 4096) >> 12
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM13(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~8192) | (a << 13)) & 8192) >> 13
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM14(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~16384) | (a << 14)) & 16384) >> 14
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM15(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM16(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~65536) | (a << 16)) & 65536) >> 16
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM17(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM18(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM19(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM20(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~1048576) | (a << 20)) & 1048576) >> 20
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM21(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~2097152) | (a << 21)) & 2097152) >> 21
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM22(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~4194304) | (a << 22)) & 4194304) >> 22
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM23(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~8388608) | (a << 23)) & 8388608) >> 23
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM24(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM25(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~33554432) | (a << 25)) & 33554432) >> 25
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM26(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~67108864) | (a << 26)) & 67108864) >> 26
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM27(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~134217728) | (a << 27)) & 134217728) >> 27
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM28(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~268435456) | (a << 28)) & 268435456) >> 28
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM29(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~536870912) | (a << 29)) & 536870912) >> 29
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM30(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~1073741824) | (a << 30)) & 1073741824) >> 30
#define RISCV_CSRRW_MCOUNTEREN_MCOUNTEREN_HPM31(a)  (unsigned int)(RISCV_CSRRW_MCOUNTEREN((RISCV_CSRRS_MCOUNTEREN(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_MCOUNTINHIBIT(a)  __builtin_linx_csrrw(0b001100100000, a)
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_CY(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_IR(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM3(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM4(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM5(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM6(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM7(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM8(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM9(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM10(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM11(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM12(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~4096) | (a << 12)) & 4096) >> 12
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM13(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~8192) | (a << 13)) & 8192) >> 13
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM14(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~16384) | (a << 14)) & 16384) >> 14
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM15(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM16(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~65536) | (a << 16)) & 65536) >> 16
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM17(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM18(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM19(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM20(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~1048576) | (a << 20)) & 1048576) >> 20
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM21(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~2097152) | (a << 21)) & 2097152) >> 21
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM22(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~4194304) | (a << 22)) & 4194304) >> 22
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM23(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~8388608) | (a << 23)) & 8388608) >> 23
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM24(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM25(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~33554432) | (a << 25)) & 33554432) >> 25
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM26(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~67108864) | (a << 26)) & 67108864) >> 26
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM27(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~134217728) | (a << 27)) & 134217728) >> 27
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM28(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~268435456) | (a << 28)) & 268435456) >> 28
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM29(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~536870912) | (a << 29)) & 536870912) >> 29
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM30(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~1073741824) | (a << 30)) & 1073741824) >> 30
#define RISCV_CSRRW_MCOUNTINHIBIT_MCOUNTINHIBIT_HPM31(a)  (unsigned int)(RISCV_CSRRW_MCOUNTINHIBIT((RISCV_CSRRS_MCOUNTINHIBIT(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_MSCRATCH(a)  __builtin_linx_csrrw(0b001101000000, a)
#define RISCV_CSRRW_MEPC(a)  __builtin_linx_csrrw(0b001101000001, a)
#define RISCV_CSRRW_MEPC_MEPC_0(a)  (unsigned int)(RISCV_CSRRW_MEPC((RISCV_CSRRS_MEPC(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MEPC_MEPC_MEPC(a)  (unsigned int)(RISCV_CSRRW_MEPC((RISCV_CSRRS_MEPC(0) & ~4294967294) | (a << 1)) & 4294967294) >> 1
#define RISCV_CSRRW_MCAUSE(a)  __builtin_linx_csrrw(0b001101000010, a)
#define RISCV_CSRRW_MCAUSE_MCAUSE_EXCCODE(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~4095) | (a << 0)) & 4095) >> 0
#define RISCV_CSRRW_MCAUSE_MCAUSE_UOPCNT3TO0(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~61440) | (a << 12)) & 61440) >> 12
#define RISCV_CSRRW_MCAUSE_MCAUSE_MPIL(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_MCAUSE_MCAUSE_UOPCNT4(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_MCAUSE_MCAUSE_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~100663296) | (a << 25)) & 100663296) >> 25
#define RISCV_CSRRW_MCAUSE_MCAUSE_MPIE(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~134217728) | (a << 27)) & 134217728) >> 27
#define RISCV_CSRRW_MCAUSE_MCAUSE_MPP(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~805306368) | (a << 28)) & 805306368) >> 28
#define RISCV_CSRRW_MCAUSE_MCAUSE_MINHV(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~1073741824) | (a << 30)) & 1073741824) >> 30
#define RISCV_CSRRW_MCAUSE_MCAUSE_INTERRUPT(a)  (unsigned int)(RISCV_CSRRW_MCAUSE((RISCV_CSRRS_MCAUSE(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_MTVAL(a)  __builtin_linx_csrrw(0b001101000011, a)
#define RISCV_CSRRW_PMPCFG0(a)  __builtin_linx_csrrw(0b001110100000, a)
#define RISCV_CSRRW_PMPCFG0_PMP0CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG0((RISCV_CSRRS_PMPCFG0(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG0_PMP1CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG0((RISCV_CSRRS_PMPCFG0(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG0_PMP2CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG0((RISCV_CSRRS_PMPCFG0(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG0_PMP3CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG0((RISCV_CSRRS_PMPCFG0(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG1(a)  __builtin_linx_csrrw(0b001110100001, a)
#define RISCV_CSRRW_PMPCFG1_PMP4CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG1((RISCV_CSRRS_PMPCFG1(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG1_PMP5CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG1((RISCV_CSRRS_PMPCFG1(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG1_PMP6CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG1((RISCV_CSRRS_PMPCFG1(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG1_PMP7CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG1((RISCV_CSRRS_PMPCFG1(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG2(a)  __builtin_linx_csrrw(0b001110100010, a)
#define RISCV_CSRRW_PMPCFG2_PMP8CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG2((RISCV_CSRRS_PMPCFG2(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG2_PMP9CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG2((RISCV_CSRRS_PMPCFG2(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG2_PMP10CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG2((RISCV_CSRRS_PMPCFG2(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG2_PMP11CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG2((RISCV_CSRRS_PMPCFG2(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG3(a)  __builtin_linx_csrrw(0b001110100011, a)
#define RISCV_CSRRW_PMPCFG3_PMP12CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG3((RISCV_CSRRS_PMPCFG3(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG3_PMP13CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG3((RISCV_CSRRS_PMPCFG3(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG3_PMP14CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG3((RISCV_CSRRS_PMPCFG3(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG3_PMP15CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG3((RISCV_CSRRS_PMPCFG3(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG4(a)  __builtin_linx_csrrw(0b001110100100, a)
#define RISCV_CSRRW_PMPCFG4_PMP16CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG4((RISCV_CSRRS_PMPCFG4(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG4_PMP17CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG4((RISCV_CSRRS_PMPCFG4(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG4_PMP18CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG4((RISCV_CSRRS_PMPCFG4(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG4_PMP19CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG4((RISCV_CSRRS_PMPCFG4(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG5(a)  __builtin_linx_csrrw(0b001110100101, a)
#define RISCV_CSRRW_PMPCFG5_PMP20CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG5((RISCV_CSRRS_PMPCFG5(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG5_PMP21CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG5((RISCV_CSRRS_PMPCFG5(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG5_PMP22CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG5((RISCV_CSRRS_PMPCFG5(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG5_PMP23CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG5((RISCV_CSRRS_PMPCFG5(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG6(a)  __builtin_linx_csrrw(0b001110100110, a)
#define RISCV_CSRRW_PMPCFG6_PMP24CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG6((RISCV_CSRRS_PMPCFG6(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG6_PMP25CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG6((RISCV_CSRRS_PMPCFG6(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG6_PMP26CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG6((RISCV_CSRRS_PMPCFG6(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG6_PMP27CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG6((RISCV_CSRRS_PMPCFG6(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG7(a)  __builtin_linx_csrrw(0b001110100111, a)
#define RISCV_CSRRW_PMPCFG7_PMP28CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG7((RISCV_CSRRS_PMPCFG7(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG7_PMP29CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG7((RISCV_CSRRS_PMPCFG7(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG7_PMP30CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG7((RISCV_CSRRS_PMPCFG7(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG7_PMP31CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG7((RISCV_CSRRS_PMPCFG7(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG8(a)  __builtin_linx_csrrw(0b001110101000, a)
#define RISCV_CSRRW_PMPCFG8_PMP32CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG8((RISCV_CSRRS_PMPCFG8(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG8_PMP33CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG8((RISCV_CSRRS_PMPCFG8(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG8_PMP34CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG8((RISCV_CSRRS_PMPCFG8(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG8_PMP35CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG8((RISCV_CSRRS_PMPCFG8(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG9(a)  __builtin_linx_csrrw(0b001110101001, a)
#define RISCV_CSRRW_PMPCFG9_PMP36CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG9((RISCV_CSRRS_PMPCFG9(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG9_PMP37CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG9((RISCV_CSRRS_PMPCFG9(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG9_PMP38CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG9((RISCV_CSRRS_PMPCFG9(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG9_PMP39CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG9((RISCV_CSRRS_PMPCFG9(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG10(a)  __builtin_linx_csrrw(0b001110101010, a)
#define RISCV_CSRRW_PMPCFG10_PMP40CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG10((RISCV_CSRRS_PMPCFG10(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG10_PMP41CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG10((RISCV_CSRRS_PMPCFG10(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG10_PMP42CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG10((RISCV_CSRRS_PMPCFG10(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG10_PMP43CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG10((RISCV_CSRRS_PMPCFG10(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG11(a)  __builtin_linx_csrrw(0b001110101011, a)
#define RISCV_CSRRW_PMPCFG11_PMP44CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG11((RISCV_CSRRS_PMPCFG11(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG11_PMP45CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG11((RISCV_CSRRS_PMPCFG11(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG11_PMP46CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG11((RISCV_CSRRS_PMPCFG11(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG11_PMP47CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG11((RISCV_CSRRS_PMPCFG11(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG12(a)  __builtin_linx_csrrw(0b001110101100, a)
#define RISCV_CSRRW_PMPCFG12_PMP48CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG12((RISCV_CSRRS_PMPCFG12(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG12_PMP49CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG12((RISCV_CSRRS_PMPCFG12(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG12_PMP50CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG12((RISCV_CSRRS_PMPCFG12(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG12_PMP51CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG12((RISCV_CSRRS_PMPCFG12(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG13(a)  __builtin_linx_csrrw(0b001110101101, a)
#define RISCV_CSRRW_PMPCFG13_PMP52CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG13((RISCV_CSRRS_PMPCFG13(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG13_PMP53CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG13((RISCV_CSRRS_PMPCFG13(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG13_PMP54CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG13((RISCV_CSRRS_PMPCFG13(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG13_PMP55CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG13((RISCV_CSRRS_PMPCFG13(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG14(a)  __builtin_linx_csrrw(0b001110101110, a)
#define RISCV_CSRRW_PMPCFG14_PMP56CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG14((RISCV_CSRRS_PMPCFG14(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG14_PMP57CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG14((RISCV_CSRRS_PMPCFG14(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG14_PMP58CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG14((RISCV_CSRRS_PMPCFG14(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG14_PMP59CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG14((RISCV_CSRRS_PMPCFG14(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPCFG15(a)  __builtin_linx_csrrw(0b001110101111, a)
#define RISCV_CSRRW_PMPCFG15_PMP60CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG15((RISCV_CSRRS_PMPCFG15(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_PMPCFG15_PMP61CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG15((RISCV_CSRRS_PMPCFG15(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_PMPCFG15_PMP62CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG15((RISCV_CSRRS_PMPCFG15(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_PMPCFG15_PMP63CFG(a)  (unsigned int)(RISCV_CSRRW_PMPCFG15((RISCV_CSRRS_PMPCFG15(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_PMPADDR0(a)  __builtin_linx_csrrw(0b001110110000, a)
#define RISCV_CSRRW_PMPADDR1(a)  __builtin_linx_csrrw(0b001110110001, a)
#define RISCV_CSRRW_PMPADDR2(a)  __builtin_linx_csrrw(0b001110110010, a)
#define RISCV_CSRRW_PMPADDR3(a)  __builtin_linx_csrrw(0b001110110011, a)
#define RISCV_CSRRW_PMPADDR4(a)  __builtin_linx_csrrw(0b001110110100, a)
#define RISCV_CSRRW_PMPADDR5(a)  __builtin_linx_csrrw(0b001110110101, a)
#define RISCV_CSRRW_PMPADDR6(a)  __builtin_linx_csrrw(0b001110110110, a)
#define RISCV_CSRRW_PMPADDR7(a)  __builtin_linx_csrrw(0b001110110111, a)
#define RISCV_CSRRW_PMPADDR8(a)  __builtin_linx_csrrw(0b001110111000, a)
#define RISCV_CSRRW_PMPADDR9(a)  __builtin_linx_csrrw(0b001110111001, a)
#define RISCV_CSRRW_PMPADDR10(a)  __builtin_linx_csrrw(0b001110111010, a)
#define RISCV_CSRRW_PMPADDR11(a)  __builtin_linx_csrrw(0b001110111011, a)
#define RISCV_CSRRW_PMPADDR12(a)  __builtin_linx_csrrw(0b001110111100, a)
#define RISCV_CSRRW_PMPADDR13(a)  __builtin_linx_csrrw(0b001110111101, a)
#define RISCV_CSRRW_PMPADDR14(a)  __builtin_linx_csrrw(0b001110111110, a)
#define RISCV_CSRRW_PMPADDR15(a)  __builtin_linx_csrrw(0b001110111111, a)
#define RISCV_CSRRW_PMPADDR16(a)  __builtin_linx_csrrw(0b001111000000, a)
#define RISCV_CSRRW_PMPADDR17(a)  __builtin_linx_csrrw(0b001111000001, a)
#define RISCV_CSRRW_PMPADDR18(a)  __builtin_linx_csrrw(0b001111000010, a)
#define RISCV_CSRRW_PMPADDR19(a)  __builtin_linx_csrrw(0b001111000011, a)
#define RISCV_CSRRW_PMPADDR20(a)  __builtin_linx_csrrw(0b001111000100, a)
#define RISCV_CSRRW_PMPADDR21(a)  __builtin_linx_csrrw(0b001111000101, a)
#define RISCV_CSRRW_PMPADDR22(a)  __builtin_linx_csrrw(0b001111000110, a)
#define RISCV_CSRRW_PMPADDR23(a)  __builtin_linx_csrrw(0b001111000111, a)
#define RISCV_CSRRW_PMPADDR24(a)  __builtin_linx_csrrw(0b001111001000, a)
#define RISCV_CSRRW_PMPADDR25(a)  __builtin_linx_csrrw(0b001111001001, a)
#define RISCV_CSRRW_PMPADDR26(a)  __builtin_linx_csrrw(0b001111001010, a)
#define RISCV_CSRRW_PMPADDR27(a)  __builtin_linx_csrrw(0b001111001011, a)
#define RISCV_CSRRW_PMPADDR28(a)  __builtin_linx_csrrw(0b001111001100, a)
#define RISCV_CSRRW_PMPADDR29(a)  __builtin_linx_csrrw(0b001111001101, a)
#define RISCV_CSRRW_PMPADDR30(a)  __builtin_linx_csrrw(0b001111001110, a)
#define RISCV_CSRRW_PMPADDR31(a)  __builtin_linx_csrrw(0b001111001111, a)
#define RISCV_CSRRW_PMPADDR32(a)  __builtin_linx_csrrw(0b001111010000, a)
#define RISCV_CSRRW_PMPADDR33(a)  __builtin_linx_csrrw(0b001111010001, a)
#define RISCV_CSRRW_PMPADDR34(a)  __builtin_linx_csrrw(0b001111010010, a)
#define RISCV_CSRRW_PMPADDR35(a)  __builtin_linx_csrrw(0b001111010011, a)
#define RISCV_CSRRW_PMPADDR36(a)  __builtin_linx_csrrw(0b001111010100, a)
#define RISCV_CSRRW_PMPADDR37(a)  __builtin_linx_csrrw(0b001111010101, a)
#define RISCV_CSRRW_PMPADDR38(a)  __builtin_linx_csrrw(0b001111010110, a)
#define RISCV_CSRRW_PMPADDR39(a)  __builtin_linx_csrrw(0b001111010111, a)
#define RISCV_CSRRW_PMPADDR40(a)  __builtin_linx_csrrw(0b001111011000, a)
#define RISCV_CSRRW_PMPADDR41(a)  __builtin_linx_csrrw(0b001111011001, a)
#define RISCV_CSRRW_PMPADDR42(a)  __builtin_linx_csrrw(0b001111011010, a)
#define RISCV_CSRRW_PMPADDR43(a)  __builtin_linx_csrrw(0b001111011011, a)
#define RISCV_CSRRW_PMPADDR44(a)  __builtin_linx_csrrw(0b001111011100, a)
#define RISCV_CSRRW_PMPADDR45(a)  __builtin_linx_csrrw(0b001111011101, a)
#define RISCV_CSRRW_PMPADDR46(a)  __builtin_linx_csrrw(0b001111011110, a)
#define RISCV_CSRRW_PMPADDR47(a)  __builtin_linx_csrrw(0b001111011111, a)
#define RISCV_CSRRW_PMPADDR48(a)  __builtin_linx_csrrw(0b001111100000, a)
#define RISCV_CSRRW_PMPADDR49(a)  __builtin_linx_csrrw(0b001111100001, a)
#define RISCV_CSRRW_PMPADDR50(a)  __builtin_linx_csrrw(0b001111100010, a)
#define RISCV_CSRRW_PMPADDR51(a)  __builtin_linx_csrrw(0b001111100011, a)
#define RISCV_CSRRW_PMPADDR52(a)  __builtin_linx_csrrw(0b001111100100, a)
#define RISCV_CSRRW_PMPADDR53(a)  __builtin_linx_csrrw(0b001111100101, a)
#define RISCV_CSRRW_PMPADDR54(a)  __builtin_linx_csrrw(0b001111100110, a)
#define RISCV_CSRRW_PMPADDR55(a)  __builtin_linx_csrrw(0b001111100111, a)
#define RISCV_CSRRW_PMPADDR56(a)  __builtin_linx_csrrw(0b001111101000, a)
#define RISCV_CSRRW_PMPADDR57(a)  __builtin_linx_csrrw(0b001111101001, a)
#define RISCV_CSRRW_PMPADDR58(a)  __builtin_linx_csrrw(0b001111101010, a)
#define RISCV_CSRRW_PMPADDR59(a)  __builtin_linx_csrrw(0b001111101011, a)
#define RISCV_CSRRW_PMPADDR60(a)  __builtin_linx_csrrw(0b001111101100, a)
#define RISCV_CSRRW_PMPADDR61(a)  __builtin_linx_csrrw(0b001111101101, a)
#define RISCV_CSRRW_PMPADDR62(a)  __builtin_linx_csrrw(0b001111101110, a)
#define RISCV_CSRRW_PMPADDR63(a)  __builtin_linx_csrrw(0b001111101111, a)
#define RISCV_CSRRW_MTVT(a)  __builtin_linx_csrrw(0b001100000111, a)
#define RISCV_CSRRW_MTVT_MTVT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MTVT((RISCV_CSRRS_MTVT(0) & ~63) | (a << 0)) & 63) >> 0
#define RISCV_CSRRW_MTVT_MTVT_BASE(a)  (unsigned int)(RISCV_CSRRW_MTVT((RISCV_CSRRS_MTVT(0) & ~4294967232) | (a << 6)) & 4294967232) >> 6
#define RISCV_CSRRW_MINTSTATUS(a)  __builtin_linx_csrrw(0b001101000110, a)
#define RISCV_CSRRW_MINTSTATUS_MINTSTATUS_UIL(a)  (unsigned int)(RISCV_CSRRW_MINTSTATUS((RISCV_CSRRS_MINTSTATUS(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_MINTSTATUS_MINTSTATUS_SIL(a)  (unsigned int)(RISCV_CSRRW_MINTSTATUS((RISCV_CSRRS_MINTSTATUS(0) & ~65280) | (a << 8)) & 65280) >> 8
#define RISCV_CSRRW_MINTSTATUS_MINTSTATUS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MINTSTATUS((RISCV_CSRRS_MINTSTATUS(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_MINTSTATUS_MINTSTATUS_MIL(a)  (unsigned int)(RISCV_CSRRW_MINTSTATUS((RISCV_CSRRS_MINTSTATUS(0) & ~4278190080) | (a << 24)) & 4278190080) >> 24
#define RISCV_CSRRW_MINTTHRESH(a)  __builtin_linx_csrrw(0b001101000111, a)
#define RISCV_CSRRW_MINTTHRESH_MINTTHRESH_TH(a)  (unsigned int)(RISCV_CSRRW_MINTTHRESH((RISCV_CSRRS_MINTTHRESH(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_MINTTHRESH_MINTTHRESH_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MINTTHRESH((RISCV_CSRRS_MINTTHRESH(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_MSCRATCHCSW(a)  __builtin_linx_csrrw(0b001101001000, a)
#define RISCV_CSRRW_MSCRATCHCSWL(a)  __builtin_linx_csrrw(0b001101001001, a)
#define RISCV_CSRRW_MCLICBASE(a)  __builtin_linx_csrrw(0b111110111111, a)
#define RISCV_CSRRW_MCLICBASE_MCLICBASE_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MCLICBASE((RISCV_CSRRS_MCLICBASE(0) & ~4095) | (a << 0)) & 4095) >> 0
#define RISCV_CSRRW_MCLICBASE_MCLICBASE_BASE(a)  (unsigned int)(RISCV_CSRRW_MCLICBASE((RISCV_CSRRS_MCLICBASE(0) & ~4294963200) | (a << 12)) & 4294963200) >> 12
#define RISCV_CSRRW_MHPMEVENT3(a)  __builtin_linx_csrrw(0b001100100011, a)
#define RISCV_CSRRW_MHPMEVENT3_MHPMEVENT3_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT3((RISCV_CSRRS_MHPMEVENT3(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT3_MHPMEVENT3_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT3((RISCV_CSRRS_MHPMEVENT3(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT4(a)  __builtin_linx_csrrw(0b001100100100, a)
#define RISCV_CSRRW_MHPMEVENT4_MHPMEVENT4_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT4((RISCV_CSRRS_MHPMEVENT4(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT4_MHPMEVENT4_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT4((RISCV_CSRRS_MHPMEVENT4(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT5(a)  __builtin_linx_csrrw(0b001100100101, a)
#define RISCV_CSRRW_MHPMEVENT5_MHPMEVENT5_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT5((RISCV_CSRRS_MHPMEVENT5(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT5_MHPMEVENT5_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT5((RISCV_CSRRS_MHPMEVENT5(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT6(a)  __builtin_linx_csrrw(0b001100100110, a)
#define RISCV_CSRRW_MHPMEVENT6_MHPMEVENT6_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT6((RISCV_CSRRS_MHPMEVENT6(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT6_MHPMEVENT6_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT6((RISCV_CSRRS_MHPMEVENT6(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT7(a)  __builtin_linx_csrrw(0b001100100111, a)
#define RISCV_CSRRW_MHPMEVENT7_MHPMEVENT7_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT7((RISCV_CSRRS_MHPMEVENT7(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT7_MHPMEVENT7_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT7((RISCV_CSRRS_MHPMEVENT7(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT8(a)  __builtin_linx_csrrw(0b001100101000, a)
#define RISCV_CSRRW_MHPMEVENT8_MHPMEVENT8_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT8((RISCV_CSRRS_MHPMEVENT8(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT8_MHPMEVENT8_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT8((RISCV_CSRRS_MHPMEVENT8(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT9(a)  __builtin_linx_csrrw(0b001100101001, a)
#define RISCV_CSRRW_MHPMEVENT9_MHPMEVENT9_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT9((RISCV_CSRRS_MHPMEVENT9(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT9_MHPMEVENT9_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT9((RISCV_CSRRS_MHPMEVENT9(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT10(a)  __builtin_linx_csrrw(0b001100101010, a)
#define RISCV_CSRRW_MHPMEVENT10_MHPMEVENT10_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT10((RISCV_CSRRS_MHPMEVENT10(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT10_MHPMEVENT10_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT10((RISCV_CSRRS_MHPMEVENT10(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT11(a)  __builtin_linx_csrrw(0b001100101011, a)
#define RISCV_CSRRW_MHPMEVENT11_MHPMEVENT11_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT11((RISCV_CSRRS_MHPMEVENT11(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT11_MHPMEVENT11_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT11((RISCV_CSRRS_MHPMEVENT11(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT12(a)  __builtin_linx_csrrw(0b001100101100, a)
#define RISCV_CSRRW_MHPMEVENT12_MHPMEVENT12_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT12((RISCV_CSRRS_MHPMEVENT12(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT12_MHPMEVENT12_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT12((RISCV_CSRRS_MHPMEVENT12(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT13(a)  __builtin_linx_csrrw(0b001100101101, a)
#define RISCV_CSRRW_MHPMEVENT13_MHPMEVENT13_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT13((RISCV_CSRRS_MHPMEVENT13(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT13_MHPMEVENT13_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT13((RISCV_CSRRS_MHPMEVENT13(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT14(a)  __builtin_linx_csrrw(0b001100101110, a)
#define RISCV_CSRRW_MHPMEVENT14_MHPMEVENT14_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT14((RISCV_CSRRS_MHPMEVENT14(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT14_MHPMEVENT14_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT14((RISCV_CSRRS_MHPMEVENT14(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT15(a)  __builtin_linx_csrrw(0b001100101111, a)
#define RISCV_CSRRW_MHPMEVENT15_MHPMEVENT15_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT15((RISCV_CSRRS_MHPMEVENT15(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT15_MHPMEVENT15_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT15((RISCV_CSRRS_MHPMEVENT15(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT16(a)  __builtin_linx_csrrw(0b001100110000, a)
#define RISCV_CSRRW_MHPMEVENT16_MHPMEVENT16_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT16((RISCV_CSRRS_MHPMEVENT16(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT16_MHPMEVENT16_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT16((RISCV_CSRRS_MHPMEVENT16(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT17(a)  __builtin_linx_csrrw(0b001100110001, a)
#define RISCV_CSRRW_MHPMEVENT17_MHPMEVENT17_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT17((RISCV_CSRRS_MHPMEVENT17(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT17_MHPMEVENT17_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT17((RISCV_CSRRS_MHPMEVENT17(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT18(a)  __builtin_linx_csrrw(0b001100110010, a)
#define RISCV_CSRRW_MHPMEVENT18_MHPMEVENT18_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT18((RISCV_CSRRS_MHPMEVENT18(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT18_MHPMEVENT18_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT18((RISCV_CSRRS_MHPMEVENT18(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT19(a)  __builtin_linx_csrrw(0b001100110011, a)
#define RISCV_CSRRW_MHPMEVENT19_MHPMEVENT19_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT19((RISCV_CSRRS_MHPMEVENT19(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT19_MHPMEVENT19_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT19((RISCV_CSRRS_MHPMEVENT19(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT20(a)  __builtin_linx_csrrw(0b001100110100, a)
#define RISCV_CSRRW_MHPMEVENT20_MHPMEVENT20_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT20((RISCV_CSRRS_MHPMEVENT20(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT20_MHPMEVENT20_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT20((RISCV_CSRRS_MHPMEVENT20(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT21(a)  __builtin_linx_csrrw(0b001100110101, a)
#define RISCV_CSRRW_MHPMEVENT21_MHPMEVENT21_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT21((RISCV_CSRRS_MHPMEVENT21(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT21_MHPMEVENT21_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT21((RISCV_CSRRS_MHPMEVENT21(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT22(a)  __builtin_linx_csrrw(0b001100110110, a)
#define RISCV_CSRRW_MHPMEVENT22_MHPMEVENT22_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT22((RISCV_CSRRS_MHPMEVENT22(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT22_MHPMEVENT22_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT22((RISCV_CSRRS_MHPMEVENT22(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT23(a)  __builtin_linx_csrrw(0b001100110111, a)
#define RISCV_CSRRW_MHPMEVENT23_MHPMEVENT23_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT23((RISCV_CSRRS_MHPMEVENT23(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT23_MHPMEVENT23_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT23((RISCV_CSRRS_MHPMEVENT23(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT24(a)  __builtin_linx_csrrw(0b001100111000, a)
#define RISCV_CSRRW_MHPMEVENT24_MHPMEVENT24_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT24((RISCV_CSRRS_MHPMEVENT24(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT24_MHPMEVENT24_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT24((RISCV_CSRRS_MHPMEVENT24(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT25(a)  __builtin_linx_csrrw(0b001100111001, a)
#define RISCV_CSRRW_MHPMEVENT25_MHPMEVENT25_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT25((RISCV_CSRRS_MHPMEVENT25(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT25_MHPMEVENT25_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT25((RISCV_CSRRS_MHPMEVENT25(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT26(a)  __builtin_linx_csrrw(0b001100111010, a)
#define RISCV_CSRRW_MHPMEVENT26_MHPMEVENT26_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT26((RISCV_CSRRS_MHPMEVENT26(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT26_MHPMEVENT26_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT26((RISCV_CSRRS_MHPMEVENT26(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT27(a)  __builtin_linx_csrrw(0b001100111011, a)
#define RISCV_CSRRW_MHPMEVENT27_MHPMEVENT27_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT27((RISCV_CSRRS_MHPMEVENT27(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT27_MHPMEVENT27_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT27((RISCV_CSRRS_MHPMEVENT27(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT28(a)  __builtin_linx_csrrw(0b001100111100, a)
#define RISCV_CSRRW_MHPMEVENT28_MHPMEVENT28_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT28((RISCV_CSRRS_MHPMEVENT28(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT28_MHPMEVENT28_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT28((RISCV_CSRRS_MHPMEVENT28(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT29(a)  __builtin_linx_csrrw(0b001100111101, a)
#define RISCV_CSRRW_MHPMEVENT29_MHPMEVENT29_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT29((RISCV_CSRRS_MHPMEVENT29(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT29_MHPMEVENT29_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT29((RISCV_CSRRS_MHPMEVENT29(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT30(a)  __builtin_linx_csrrw(0b001100111110, a)
#define RISCV_CSRRW_MHPMEVENT30_MHPMEVENT30_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT30((RISCV_CSRRS_MHPMEVENT30(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT30_MHPMEVENT30_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT30((RISCV_CSRRS_MHPMEVENT30(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MHPMEVENT31(a)  __builtin_linx_csrrw(0b001100111111, a)
#define RISCV_CSRRW_MHPMEVENT31_MHPMEVENT31_EVENT(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT31((RISCV_CSRRS_MHPMEVENT31(0) & ~511) | (a << 0)) & 511) >> 0
#define RISCV_CSRRW_MHPMEVENT31_MHPMEVENT31_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MHPMEVENT31((RISCV_CSRRS_MHPMEVENT31(0) & ~4294966784) | (a << 9)) & 4294966784) >> 9
#define RISCV_CSRRW_MCYCLE(a)  __builtin_linx_csrrw(0b101100000000, a)
#define RISCV_CSRRW_MINSTRET(a)  __builtin_linx_csrrw(0b101100000010, a)
#define RISCV_CSRRW_MHPMCOUNTER3(a)  __builtin_linx_csrrw(0b101100000011, a)
#define RISCV_CSRRW_MHPMCOUNTER4(a)  __builtin_linx_csrrw(0b101100000100, a)
#define RISCV_CSRRW_MHPMCOUNTER5(a)  __builtin_linx_csrrw(0b101100000101, a)
#define RISCV_CSRRW_MHPMCOUNTER6(a)  __builtin_linx_csrrw(0b101100000110, a)
#define RISCV_CSRRW_MHPMCOUNTER7(a)  __builtin_linx_csrrw(0b101100000111, a)
#define RISCV_CSRRW_MHPMCOUNTER8(a)  __builtin_linx_csrrw(0b101100001000, a)
#define RISCV_CSRRW_MHPMCOUNTER9(a)  __builtin_linx_csrrw(0b101100001001, a)
#define RISCV_CSRRW_MHPMCOUNTER10(a)  __builtin_linx_csrrw(0b101100001010, a)
#define RISCV_CSRRW_MHPMCOUNTER11(a)  __builtin_linx_csrrw(0b101100001011, a)
#define RISCV_CSRRW_MHPMCOUNTER12(a)  __builtin_linx_csrrw(0b101100001100, a)
#define RISCV_CSRRW_MHPMCOUNTER13(a)  __builtin_linx_csrrw(0b101100001101, a)
#define RISCV_CSRRW_MHPMCOUNTER14(a)  __builtin_linx_csrrw(0b101100001110, a)
#define RISCV_CSRRW_MHPMCOUNTER15(a)  __builtin_linx_csrrw(0b101100001111, a)
#define RISCV_CSRRW_MHPMCOUNTER16(a)  __builtin_linx_csrrw(0b101100010000, a)
#define RISCV_CSRRW_MHPMCOUNTER17(a)  __builtin_linx_csrrw(0b101100010001, a)
#define RISCV_CSRRW_MHPMCOUNTER18(a)  __builtin_linx_csrrw(0b101100010010, a)
#define RISCV_CSRRW_MHPMCOUNTER19(a)  __builtin_linx_csrrw(0b101100010011, a)
#define RISCV_CSRRW_MHPMCOUNTER20(a)  __builtin_linx_csrrw(0b101100010100, a)
#define RISCV_CSRRW_MHPMCOUNTER21(a)  __builtin_linx_csrrw(0b101100010101, a)
#define RISCV_CSRRW_MHPMCOUNTER22(a)  __builtin_linx_csrrw(0b101100010110, a)
#define RISCV_CSRRW_MHPMCOUNTER23(a)  __builtin_linx_csrrw(0b101100010111, a)
#define RISCV_CSRRW_MHPMCOUNTER24(a)  __builtin_linx_csrrw(0b101100011000, a)
#define RISCV_CSRRW_MHPMCOUNTER25(a)  __builtin_linx_csrrw(0b101100011001, a)
#define RISCV_CSRRW_MHPMCOUNTER26(a)  __builtin_linx_csrrw(0b101100011010, a)
#define RISCV_CSRRW_MHPMCOUNTER27(a)  __builtin_linx_csrrw(0b101100011011, a)
#define RISCV_CSRRW_MHPMCOUNTER28(a)  __builtin_linx_csrrw(0b101100011100, a)
#define RISCV_CSRRW_MHPMCOUNTER29(a)  __builtin_linx_csrrw(0b101100011101, a)
#define RISCV_CSRRW_MHPMCOUNTER30(a)  __builtin_linx_csrrw(0b101100011110, a)
#define RISCV_CSRRW_MHPMCOUNTER31(a)  __builtin_linx_csrrw(0b101100011111, a)
#define RISCV_CSRRW_MCYCLEH(a)  __builtin_linx_csrrw(0b101110000000, a)
#define RISCV_CSRRW_MINSTRETH(a)  __builtin_linx_csrrw(0b101110000010, a)
#define RISCV_CSRRW_MHPMCOUNTER3H(a)  __builtin_linx_csrrw(0b101110000011, a)
#define RISCV_CSRRW_MHPMCOUNTER4H(a)  __builtin_linx_csrrw(0b101110000100, a)
#define RISCV_CSRRW_MHPMCOUNTER5H(a)  __builtin_linx_csrrw(0b101110000101, a)
#define RISCV_CSRRW_MHPMCOUNTER6H(a)  __builtin_linx_csrrw(0b101110000110, a)
#define RISCV_CSRRW_MHPMCOUNTER7H(a)  __builtin_linx_csrrw(0b101110000111, a)
#define RISCV_CSRRW_MHPMCOUNTER8H(a)  __builtin_linx_csrrw(0b101110001000, a)
#define RISCV_CSRRW_MHPMCOUNTER9H(a)  __builtin_linx_csrrw(0b101110001001, a)
#define RISCV_CSRRW_MHPMCOUNTER10H(a)  __builtin_linx_csrrw(0b101110001010, a)
#define RISCV_CSRRW_MHPMCOUNTER11H(a)  __builtin_linx_csrrw(0b101110001011, a)
#define RISCV_CSRRW_MHPMCOUNTER12H(a)  __builtin_linx_csrrw(0b101110001100, a)
#define RISCV_CSRRW_MHPMCOUNTER13H(a)  __builtin_linx_csrrw(0b101110001101, a)
#define RISCV_CSRRW_MHPMCOUNTER14H(a)  __builtin_linx_csrrw(0b101110001110, a)
#define RISCV_CSRRW_MHPMCOUNTER15H(a)  __builtin_linx_csrrw(0b101110001111, a)
#define RISCV_CSRRW_MHPMCOUNTER16H(a)  __builtin_linx_csrrw(0b101110010000, a)
#define RISCV_CSRRW_MHPMCOUNTER17H(a)  __builtin_linx_csrrw(0b101110010001, a)
#define RISCV_CSRRW_MHPMCOUNTER18H(a)  __builtin_linx_csrrw(0b101110010010, a)
#define RISCV_CSRRW_MHPMCOUNTER19H(a)  __builtin_linx_csrrw(0b101110010011, a)
#define RISCV_CSRRW_MHPMCOUNTER20H(a)  __builtin_linx_csrrw(0b101110010100, a)
#define RISCV_CSRRW_MHPMCOUNTER21H(a)  __builtin_linx_csrrw(0b101110010101, a)
#define RISCV_CSRRW_MHPMCOUNTER22H(a)  __builtin_linx_csrrw(0b101110010110, a)
#define RISCV_CSRRW_MHPMCOUNTER23H(a)  __builtin_linx_csrrw(0b101110010111, a)
#define RISCV_CSRRW_MHPMCOUNTER24H(a)  __builtin_linx_csrrw(0b101110011000, a)
#define RISCV_CSRRW_MHPMCOUNTER25H(a)  __builtin_linx_csrrw(0b101110011001, a)
#define RISCV_CSRRW_MHPMCOUNTER26H(a)  __builtin_linx_csrrw(0b101110011010, a)
#define RISCV_CSRRW_MHPMCOUNTER27H(a)  __builtin_linx_csrrw(0b101110011011, a)
#define RISCV_CSRRW_MHPMCOUNTER28H(a)  __builtin_linx_csrrw(0b101110011100, a)
#define RISCV_CSRRW_MHPMCOUNTER29H(a)  __builtin_linx_csrrw(0b101110011101, a)
#define RISCV_CSRRW_MHPMCOUNTER30H(a)  __builtin_linx_csrrw(0b101110011110, a)
#define RISCV_CSRRW_MHPMCOUNTER31H(a)  __builtin_linx_csrrw(0b101110011111, a)
#define RISCV_CSRRW_ICCTL(a)  __builtin_linx_csrrw(0b011111000000, a)
#define RISCV_CSRRW_ICCTL_ICCTL_ICEN(a)  (unsigned int)(RISCV_CSRRW_ICCTL((RISCV_CSRRS_ICCTL(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_ICCTL_ICCTL_ICS(a)  (unsigned int)(RISCV_CSRRW_ICCTL((RISCV_CSRRS_ICCTL(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_ICCTL_ICCTL_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_ICCTL((RISCV_CSRRS_ICCTL(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_DCCTL(a)  __builtin_linx_csrrw(0b011111000001, a)
#define RISCV_CSRRW_DCCTL_DCCTL_DCEN(a)  (unsigned int)(RISCV_CSRRW_DCCTL((RISCV_CSRRS_DCCTL(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_DCCTL_DCCTL_DCS(a)  (unsigned int)(RISCV_CSRRW_DCCTL((RISCV_CSRRS_DCCTL(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_DCCTL_DCCTL_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_DCCTL((RISCV_CSRRS_DCCTL(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_ICMAINT(a)  __builtin_linx_csrrw(0b011111000010, a)
#define RISCV_CSRRW_ICMAINT_ICMAINT_VA(a)  (unsigned int)(RISCV_CSRRW_ICMAINT((RISCV_CSRRS_ICMAINT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_ICMAINT_ICMAINT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_ICMAINT((RISCV_CSRRS_ICMAINT(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_ICMAINT_ICMAINT_ICIV(a)  (unsigned int)(RISCV_CSRRW_ICMAINT((RISCV_CSRRS_ICMAINT(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_ICMAINT_ICMAINT_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_ICMAINT((RISCV_CSRRS_ICMAINT(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_DCMAINT(a)  __builtin_linx_csrrw(0b011111000011, a)
#define RISCV_CSRRW_DCMAINT_DCMAINT_VA(a)  (unsigned int)(RISCV_CSRRW_DCMAINT((RISCV_CSRRS_DCMAINT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_DCMAINT_DCMAINT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_DCMAINT((RISCV_CSRRS_DCMAINT(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_DCMAINT_DCMAINT_DCIV(a)  (unsigned int)(RISCV_CSRRW_DCMAINT((RISCV_CSRRS_DCMAINT(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_DCMAINT_DCMAINT_DCC(a)  (unsigned int)(RISCV_CSRRW_DCMAINT((RISCV_CSRRS_DCMAINT(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_DCMAINT_DCMAINT_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_DCMAINT((RISCV_CSRRS_DCMAINT(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_ICINVA(a)  __builtin_linx_csrrw(0b011111000100, a)
#define RISCV_CSRRW_ICINVA_ICINVA_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_ICINVA((RISCV_CSRRS_ICINVA(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_ICINVA_ICINVA_ADDR(a)  (unsigned int)(RISCV_CSRRW_ICINVA((RISCV_CSRRS_ICINVA(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_DCINCVA(a)  __builtin_linx_csrrw(0b011111000101, a)
#define RISCV_CSRRW_DCINCVA_DCINCVA_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_DCINCVA((RISCV_CSRRS_DCINCVA(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_DCINCVA_DCINCVA_ADDR(a)  (unsigned int)(RISCV_CSRRW_DCINCVA((RISCV_CSRRS_DCINCVA(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_APREFI(a)  __builtin_linx_csrrw(0b011111000110, a)
#define RISCV_CSRRW_APREFI_APREFI_IAPEN(a)  (unsigned int)(RISCV_CSRRW_APREFI((RISCV_CSRRS_APREFI(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_APREFI_APREFI_ICL(a)  (unsigned int)(RISCV_CSRRW_APREFI((RISCV_CSRRS_APREFI(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_APREFI_APREFI_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_APREFI((RISCV_CSRRS_APREFI(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_APREFD(a)  __builtin_linx_csrrw(0b011111000111, a)
#define RISCV_CSRRW_APREFD_APREFD_LAPEN(a)  (unsigned int)(RISCV_CSRRW_APREFD((RISCV_CSRRS_APREFD(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_APREFD_APREFD_LCL(a)  (unsigned int)(RISCV_CSRRW_APREFD((RISCV_CSRRS_APREFD(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_APREFD_APREFD_SAPEN(a)  (unsigned int)(RISCV_CSRRW_APREFD((RISCV_CSRRS_APREFD(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_APREFD_APREFD_SCL(a)  (unsigned int)(RISCV_CSRRW_APREFD((RISCV_CSRRS_APREFD(0) & ~224) | (a << 5)) & 224) >> 5
#define RISCV_CSRRW_APREFD_APREFD_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_APREFD((RISCV_CSRRS_APREFD(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_WST(a)  __builtin_linx_csrrw(0b011111010000, a)
#define RISCV_CSRRW_WST_WST_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_WST((RISCV_CSRRS_WST(0) & ~2047) | (a << 0)) & 2047) >> 0
#define RISCV_CSRRW_WST_WST_WST(a)  (unsigned int)(RISCV_CSRRW_WST((RISCV_CSRRS_WST(0) & ~14336) | (a << 11)) & 14336) >> 11
#define RISCV_CSRRW_WST_WST_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_WST((RISCV_CSRRS_WST(0) & ~4294950912) | (a << 14)) & 4294950912) >> 14
#define RISCV_CSRRW_TCMARB(a)  __builtin_linx_csrrw(0b011111010010, a)
#define RISCV_CSRRW_TCMARB_TCMARB_ECNT(a)  (unsigned int)(RISCV_CSRRW_TCMARB((RISCV_CSRRS_TCMARB(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_TCMARB_TCMARB_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TCMARB((RISCV_CSRRS_TCMARB(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_MEMATTR0(a)  __builtin_linx_csrrw(0b101111000000, a)
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR0_MEMATTR0_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR0((RISCV_CSRRS_MEMATTR0(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR1(a)  __builtin_linx_csrrw(0b101111000001, a)
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR1_MEMATTR1_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR1((RISCV_CSRRS_MEMATTR1(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR2(a)  __builtin_linx_csrrw(0b101111000010, a)
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR2_MEMATTR2_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR2((RISCV_CSRRS_MEMATTR2(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR3(a)  __builtin_linx_csrrw(0b101111000011, a)
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR3_MEMATTR3_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR3((RISCV_CSRRS_MEMATTR3(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR4(a)  __builtin_linx_csrrw(0b101111000100, a)
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR4_MEMATTR4_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR4((RISCV_CSRRS_MEMATTR4(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR5(a)  __builtin_linx_csrrw(0b101111000101, a)
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR5_MEMATTR5_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR5((RISCV_CSRRS_MEMATTR5(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR6(a)  __builtin_linx_csrrw(0b101111000110, a)
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR6_MEMATTR6_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR6((RISCV_CSRRS_MEMATTR6(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR7(a)  __builtin_linx_csrrw(0b101111000111, a)
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR7_MEMATTR7_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR7((RISCV_CSRRS_MEMATTR7(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR8(a)  __builtin_linx_csrrw(0b101111001000, a)
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR8_MEMATTR8_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR8((RISCV_CSRRS_MEMATTR8(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR9(a)  __builtin_linx_csrrw(0b101111001001, a)
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR9_MEMATTR9_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR9((RISCV_CSRRS_MEMATTR9(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR10(a)  __builtin_linx_csrrw(0b101111001010, a)
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR10_MEMATTR10_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR10((RISCV_CSRRS_MEMATTR10(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR11(a)  __builtin_linx_csrrw(0b101111001011, a)
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR11_MEMATTR11_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR11((RISCV_CSRRS_MEMATTR11(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR12(a)  __builtin_linx_csrrw(0b101111001100, a)
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR12_MEMATTR12_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR12((RISCV_CSRRS_MEMATTR12(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR13(a)  __builtin_linx_csrrw(0b101111001101, a)
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR13_MEMATTR13_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR13((RISCV_CSRRS_MEMATTR13(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR14(a)  __builtin_linx_csrrw(0b101111001110, a)
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR14_MEMATTR14_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR14((RISCV_CSRRS_MEMATTR14(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR15(a)  __builtin_linx_csrrw(0b101111001111, a)
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR15_MEMATTR15_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR15((RISCV_CSRRS_MEMATTR15(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR16(a)  __builtin_linx_csrrw(0b101111010000, a)
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR16_MEMATTR16_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR16((RISCV_CSRRS_MEMATTR16(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR17(a)  __builtin_linx_csrrw(0b101111010001, a)
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR17_MEMATTR17_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR17((RISCV_CSRRS_MEMATTR17(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR18(a)  __builtin_linx_csrrw(0b101111010010, a)
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR18_MEMATTR18_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR18((RISCV_CSRRS_MEMATTR18(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR19(a)  __builtin_linx_csrrw(0b101111010011, a)
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR19_MEMATTR19_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR19((RISCV_CSRRS_MEMATTR19(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR20(a)  __builtin_linx_csrrw(0b101111010100, a)
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR20_MEMATTR20_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR20((RISCV_CSRRS_MEMATTR20(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR21(a)  __builtin_linx_csrrw(0b101111010101, a)
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR21_MEMATTR21_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR21((RISCV_CSRRS_MEMATTR21(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR22(a)  __builtin_linx_csrrw(0b101111010110, a)
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR22_MEMATTR22_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR22((RISCV_CSRRS_MEMATTR22(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR23(a)  __builtin_linx_csrrw(0b101111010111, a)
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR23_MEMATTR23_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR23((RISCV_CSRRS_MEMATTR23(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR24(a)  __builtin_linx_csrrw(0b101111011000, a)
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR24_MEMATTR24_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR24((RISCV_CSRRS_MEMATTR24(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR25(a)  __builtin_linx_csrrw(0b101111011001, a)
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR25_MEMATTR25_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR25((RISCV_CSRRS_MEMATTR25(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR26(a)  __builtin_linx_csrrw(0b101111011010, a)
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR26_MEMATTR26_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR26((RISCV_CSRRS_MEMATTR26(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR27(a)  __builtin_linx_csrrw(0b101111011011, a)
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR27_MEMATTR27_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR27((RISCV_CSRRS_MEMATTR27(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR28(a)  __builtin_linx_csrrw(0b101111011100, a)
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR28_MEMATTR28_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR28((RISCV_CSRRS_MEMATTR28(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR29(a)  __builtin_linx_csrrw(0b101111011101, a)
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR29_MEMATTR29_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR29((RISCV_CSRRS_MEMATTR29(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR30(a)  __builtin_linx_csrrw(0b101111011110, a)
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR30_MEMATTR30_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR30((RISCV_CSRRS_MEMATTR30(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR31(a)  __builtin_linx_csrrw(0b101111011111, a)
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR31_MEMATTR31_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR31((RISCV_CSRRS_MEMATTR31(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR32(a)  __builtin_linx_csrrw(0b101111100000, a)
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR32_MEMATTR32_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR32((RISCV_CSRRS_MEMATTR32(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR33(a)  __builtin_linx_csrrw(0b101111100001, a)
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR33_MEMATTR33_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR33((RISCV_CSRRS_MEMATTR33(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR34(a)  __builtin_linx_csrrw(0b101111100010, a)
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR34_MEMATTR34_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR34((RISCV_CSRRS_MEMATTR34(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR35(a)  __builtin_linx_csrrw(0b101111100011, a)
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR35_MEMATTR35_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR35((RISCV_CSRRS_MEMATTR35(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR36(a)  __builtin_linx_csrrw(0b101111100100, a)
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR36_MEMATTR36_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR36((RISCV_CSRRS_MEMATTR36(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR37(a)  __builtin_linx_csrrw(0b101111100101, a)
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR37_MEMATTR37_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR37((RISCV_CSRRS_MEMATTR37(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR38(a)  __builtin_linx_csrrw(0b101111100110, a)
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR38_MEMATTR38_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR38((RISCV_CSRRS_MEMATTR38(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR39(a)  __builtin_linx_csrrw(0b101111100111, a)
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR39_MEMATTR39_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR39((RISCV_CSRRS_MEMATTR39(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR40(a)  __builtin_linx_csrrw(0b101111101000, a)
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR40_MEMATTR40_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR40((RISCV_CSRRS_MEMATTR40(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR41(a)  __builtin_linx_csrrw(0b101111101001, a)
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR41_MEMATTR41_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR41((RISCV_CSRRS_MEMATTR41(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR42(a)  __builtin_linx_csrrw(0b101111101010, a)
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR42_MEMATTR42_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR42((RISCV_CSRRS_MEMATTR42(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR43(a)  __builtin_linx_csrrw(0b101111101011, a)
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR43_MEMATTR43_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR43((RISCV_CSRRS_MEMATTR43(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR44(a)  __builtin_linx_csrrw(0b101111101100, a)
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR44_MEMATTR44_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR44((RISCV_CSRRS_MEMATTR44(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR45(a)  __builtin_linx_csrrw(0b101111101101, a)
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR45_MEMATTR45_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR45((RISCV_CSRRS_MEMATTR45(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR46(a)  __builtin_linx_csrrw(0b101111101110, a)
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR46_MEMATTR46_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR46((RISCV_CSRRS_MEMATTR46(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR47(a)  __builtin_linx_csrrw(0b101111101111, a)
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR47_MEMATTR47_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR47((RISCV_CSRRS_MEMATTR47(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR48(a)  __builtin_linx_csrrw(0b101111110000, a)
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR48_MEMATTR48_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR48((RISCV_CSRRS_MEMATTR48(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR49(a)  __builtin_linx_csrrw(0b101111110001, a)
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR49_MEMATTR49_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR49((RISCV_CSRRS_MEMATTR49(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR50(a)  __builtin_linx_csrrw(0b101111110010, a)
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR50_MEMATTR50_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR50((RISCV_CSRRS_MEMATTR50(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR51(a)  __builtin_linx_csrrw(0b101111110011, a)
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR51_MEMATTR51_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR51((RISCV_CSRRS_MEMATTR51(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR52(a)  __builtin_linx_csrrw(0b101111110100, a)
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR52_MEMATTR52_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR52((RISCV_CSRRS_MEMATTR52(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR53(a)  __builtin_linx_csrrw(0b101111110101, a)
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR53_MEMATTR53_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR53((RISCV_CSRRS_MEMATTR53(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR54(a)  __builtin_linx_csrrw(0b101111110110, a)
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR54_MEMATTR54_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR54((RISCV_CSRRS_MEMATTR54(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR55(a)  __builtin_linx_csrrw(0b101111110111, a)
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR55_MEMATTR55_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR55((RISCV_CSRRS_MEMATTR55(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR56(a)  __builtin_linx_csrrw(0b101111111000, a)
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR56_MEMATTR56_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR56((RISCV_CSRRS_MEMATTR56(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR57(a)  __builtin_linx_csrrw(0b101111111001, a)
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR57_MEMATTR57_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR57((RISCV_CSRRS_MEMATTR57(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR58(a)  __builtin_linx_csrrw(0b101111111010, a)
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR58_MEMATTR58_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR58((RISCV_CSRRS_MEMATTR58(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR59(a)  __builtin_linx_csrrw(0b101111111011, a)
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR59_MEMATTR59_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR59((RISCV_CSRRS_MEMATTR59(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR60(a)  __builtin_linx_csrrw(0b101111111100, a)
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR60_MEMATTR60_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR60((RISCV_CSRRS_MEMATTR60(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR61(a)  __builtin_linx_csrrw(0b101111111101, a)
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR61_MEMATTR61_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR61((RISCV_CSRRS_MEMATTR61(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR62(a)  __builtin_linx_csrrw(0b101111111110, a)
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR62_MEMATTR62_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR62((RISCV_CSRRS_MEMATTR62(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_MEMATTR63(a)  __builtin_linx_csrrw(0b101111111111, a)
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_IATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~15) | (a << 0)) & 15) >> 0
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_OATTR(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~240) | (a << 4)) & 240) >> 4
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_OUTS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_NS(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_ME(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_AMO(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_RSRV(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~24576) | (a << 13)) & 24576) >> 13
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_OCHE(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_OCHN(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~4128768) | (a << 16)) & 4128768) >> 16
#define RISCV_CSRRW_MEMATTR63_MEMATTR63_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MEMATTR63((RISCV_CSRRS_MEMATTR63(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_PORPC(a)  __builtin_linx_csrrw(0b111111000000, a)
#define RISCV_CSRRW_PORPC_PORPC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_PORPC((RISCV_CSRRS_PORPC(0) & ~3) | (a << 0)) & 3) >> 0
#define RISCV_CSRRW_PORPC_PORPC_PC(a)  (unsigned int)(RISCV_CSRRW_PORPC((RISCV_CSRRS_PORPC(0) & ~4294967292) | (a << 2)) & 4294967292) >> 2
#define RISCV_CSRRW_WTOPC(a)  __builtin_linx_csrrw(0b111111000001, a)
#define RISCV_CSRRW_WTOPC_WTOPC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_WTOPC((RISCV_CSRRS_WTOPC(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_WTOPC_WTOPC_PC(a)  (unsigned int)(RISCV_CSRRW_WTOPC((RISCV_CSRRS_WTOPC(0) & ~4294967294) | (a << 1)) & 4294967294) >> 1
#define RISCV_CSRRW_CXCPTSC(a)  __builtin_linx_csrrw(0b111111000010, a)
#define RISCV_CSRRW_CXCPTSC_CXCPTSC_CCAUSE(a)  (unsigned int)(RISCV_CSRRW_CXCPTSC((RISCV_CSRRS_CXCPTSC(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_CXCPTSC_CXCPTSC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_CXCPTSC((RISCV_CSRRS_CXCPTSC(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_DBGUOPCNT(a)  __builtin_linx_csrrw(0b011111111110, a)
#define RISCV_CSRRW_DBGUOPCNT_DBGUOPCNT_UOPCNT(a)  (unsigned int)(RISCV_CSRRW_DBGUOPCNT((RISCV_CSRRS_DBGUOPCNT(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_DBGUOPCNT_DBGUOPCNT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_DBGUOPCNT((RISCV_CSRRS_DBGUOPCNT(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_FFLAGS(a)  __builtin_linx_csrrw(0b000000000001, a)
#define RISCV_CSRRW_FFLAGS_FFLAGS_NX(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_FFLAGS_FFLAGS_UF(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_FFLAGS_FFLAGS_OF(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_FFLAGS_FFLAGS_DZ(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_FFLAGS_FFLAGS_NV(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_FFLAGS_FFLAGS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_FFLAGS((RISCV_CSRRS_FFLAGS(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_FRM(a)  __builtin_linx_csrrw(0b000000000010, a)
#define RISCV_CSRRW_FRM_FRM(a)  (unsigned int)(RISCV_CSRRW_FRM((RISCV_CSRRS_FRM(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_FCSR(a)  __builtin_linx_csrrw(0b000000000011, a)
#define RISCV_CSRRW_FCSR_FFLAGS(a)  (unsigned int)(RISCV_CSRRW_FCSR((RISCV_CSRRS_FCSR(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_FCSR_FRM(a)  (unsigned int)(RISCV_CSRRW_FCSR((RISCV_CSRRS_FCSR(0) & ~224) | (a << 5)) & 224) >> 5
#define RISCV_CSRRW_FCSR_FCSR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_FCSR((RISCV_CSRRS_FCSR(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_HPMCOUNTER3(a)  __builtin_linx_csrrw(0b110000000011, a)
#define RISCV_CSRRW_HPMCOUNTER4(a)  __builtin_linx_csrrw(0b110000000100, a)
#define RISCV_CSRRW_HPMCOUNTER5(a)  __builtin_linx_csrrw(0b110000000101, a)
#define RISCV_CSRRW_HPMCOUNTER6(a)  __builtin_linx_csrrw(0b110000000110, a)
#define RISCV_CSRRW_HPMCOUNTER7(a)  __builtin_linx_csrrw(0b110000000111, a)
#define RISCV_CSRRW_HPMCOUNTER8(a)  __builtin_linx_csrrw(0b110000001000, a)
#define RISCV_CSRRW_HPMCOUNTER9(a)  __builtin_linx_csrrw(0b110000001001, a)
#define RISCV_CSRRW_HPMCOUNTER10(a)  __builtin_linx_csrrw(0b110000001010, a)
#define RISCV_CSRRW_HPMCOUNTER11(a)  __builtin_linx_csrrw(0b110000001011, a)
#define RISCV_CSRRW_HPMCOUNTER12(a)  __builtin_linx_csrrw(0b110000001100, a)
#define RISCV_CSRRW_HPMCOUNTER13(a)  __builtin_linx_csrrw(0b110000001101, a)
#define RISCV_CSRRW_HPMCOUNTER14(a)  __builtin_linx_csrrw(0b110000001110, a)
#define RISCV_CSRRW_HPMCOUNTER15(a)  __builtin_linx_csrrw(0b110000001111, a)
#define RISCV_CSRRW_HPMCOUNTER16(a)  __builtin_linx_csrrw(0b110000010000, a)
#define RISCV_CSRRW_HPMCOUNTER17(a)  __builtin_linx_csrrw(0b110000010001, a)
#define RISCV_CSRRW_HPMCOUNTER18(a)  __builtin_linx_csrrw(0b110000010010, a)
#define RISCV_CSRRW_HPMCOUNTER19(a)  __builtin_linx_csrrw(0b110000010011, a)
#define RISCV_CSRRW_HPMCOUNTER20(a)  __builtin_linx_csrrw(0b110000010100, a)
#define RISCV_CSRRW_HPMCOUNTER21(a)  __builtin_linx_csrrw(0b110000010101, a)
#define RISCV_CSRRW_HPMCOUNTER22(a)  __builtin_linx_csrrw(0b110000010110, a)
#define RISCV_CSRRW_HPMCOUNTER23(a)  __builtin_linx_csrrw(0b110000010111, a)
#define RISCV_CSRRW_HPMCOUNTER24(a)  __builtin_linx_csrrw(0b110000011000, a)
#define RISCV_CSRRW_HPMCOUNTER25(a)  __builtin_linx_csrrw(0b110000011001, a)
#define RISCV_CSRRW_HPMCOUNTER26(a)  __builtin_linx_csrrw(0b110000011010, a)
#define RISCV_CSRRW_HPMCOUNTER27(a)  __builtin_linx_csrrw(0b110000011011, a)
#define RISCV_CSRRW_HPMCOUNTER28(a)  __builtin_linx_csrrw(0b110000011100, a)
#define RISCV_CSRRW_HPMCOUNTER29(a)  __builtin_linx_csrrw(0b110000011101, a)
#define RISCV_CSRRW_HPMCOUNTER30(a)  __builtin_linx_csrrw(0b110000011110, a)
#define RISCV_CSRRW_HPMCOUNTER31(a)  __builtin_linx_csrrw(0b110000011111, a)
#define RISCV_CSRRW_CYCLE(a)  __builtin_linx_csrrw(0b110000000000, a)
#define RISCV_CSRRW_TIME(a)  __builtin_linx_csrrw(0b110000000001, a)
#define RISCV_CSRRW_INSTRET(a)  __builtin_linx_csrrw(0b110000000010, a)
#define RISCV_CSRRW_HPMCOUNTER3H(a)  __builtin_linx_csrrw(0b110010000011, a)
#define RISCV_CSRRW_HPMCOUNTER4H(a)  __builtin_linx_csrrw(0b110010000100, a)
#define RISCV_CSRRW_HPMCOUNTER5H(a)  __builtin_linx_csrrw(0b110010000101, a)
#define RISCV_CSRRW_HPMCOUNTER6H(a)  __builtin_linx_csrrw(0b110010000110, a)
#define RISCV_CSRRW_HPMCOUNTER7H(a)  __builtin_linx_csrrw(0b110010000111, a)
#define RISCV_CSRRW_HPMCOUNTER8H(a)  __builtin_linx_csrrw(0b110010001000, a)
#define RISCV_CSRRW_HPMCOUNTER9H(a)  __builtin_linx_csrrw(0b110010001001, a)
#define RISCV_CSRRW_HPMCOUNTER10H(a)  __builtin_linx_csrrw(0b110010001010, a)
#define RISCV_CSRRW_HPMCOUNTER11H(a)  __builtin_linx_csrrw(0b110010001011, a)
#define RISCV_CSRRW_HPMCOUNTER12H(a)  __builtin_linx_csrrw(0b110010001100, a)
#define RISCV_CSRRW_HPMCOUNTER13H(a)  __builtin_linx_csrrw(0b110010001101, a)
#define RISCV_CSRRW_HPMCOUNTER14H(a)  __builtin_linx_csrrw(0b110010001110, a)
#define RISCV_CSRRW_HPMCOUNTER15H(a)  __builtin_linx_csrrw(0b110010001111, a)
#define RISCV_CSRRW_HPMCOUNTER16H(a)  __builtin_linx_csrrw(0b110010010000, a)
#define RISCV_CSRRW_HPMCOUNTER17H(a)  __builtin_linx_csrrw(0b110010010001, a)
#define RISCV_CSRRW_HPMCOUNTER18H(a)  __builtin_linx_csrrw(0b110010010010, a)
#define RISCV_CSRRW_HPMCOUNTER19H(a)  __builtin_linx_csrrw(0b110010010011, a)
#define RISCV_CSRRW_HPMCOUNTER20H(a)  __builtin_linx_csrrw(0b110010010100, a)
#define RISCV_CSRRW_HPMCOUNTER21H(a)  __builtin_linx_csrrw(0b110010010101, a)
#define RISCV_CSRRW_HPMCOUNTER22H(a)  __builtin_linx_csrrw(0b110010010110, a)
#define RISCV_CSRRW_HPMCOUNTER23H(a)  __builtin_linx_csrrw(0b110010010111, a)
#define RISCV_CSRRW_HPMCOUNTER24H(a)  __builtin_linx_csrrw(0b110010011000, a)
#define RISCV_CSRRW_HPMCOUNTER25H(a)  __builtin_linx_csrrw(0b110010011001, a)
#define RISCV_CSRRW_HPMCOUNTER26H(a)  __builtin_linx_csrrw(0b110010011010, a)
#define RISCV_CSRRW_HPMCOUNTER27H(a)  __builtin_linx_csrrw(0b110010011011, a)
#define RISCV_CSRRW_HPMCOUNTER28H(a)  __builtin_linx_csrrw(0b110010011100, a)
#define RISCV_CSRRW_HPMCOUNTER29H(a)  __builtin_linx_csrrw(0b110010011101, a)
#define RISCV_CSRRW_HPMCOUNTER30H(a)  __builtin_linx_csrrw(0b110010011110, a)
#define RISCV_CSRRW_HPMCOUNTER31H(a)  __builtin_linx_csrrw(0b110010011111, a)
#define RISCV_CSRRW_CYCLEH(a)  __builtin_linx_csrrw(0b110010000000, a)
#define RISCV_CSRRW_TIMEH(a)  __builtin_linx_csrrw(0b110010000001, a)
#define RISCV_CSRRW_INSTRETH(a)  __builtin_linx_csrrw(0b110010000010, a)
#define RISCV_CSRRW_TMESCR(a)  __builtin_linx_csrrw(0b011111100000, a)
#define RISCV_CSRRW_TMESCR_TMESCR_EME(a)  (unsigned int)(RISCV_CSRRW_TMESCR((RISCV_CSRRS_TMESCR(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TMESCR_TMESCR_ETE(a)  (unsigned int)(RISCV_CSRRW_TMESCR((RISCV_CSRRS_TMESCR(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_TMESCR_TMESCR_TVL(a)  (unsigned int)(RISCV_CSRRW_TMESCR((RISCV_CSRRS_TMESCR(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_TMESCR_TMESCR_UFSRO(a)  (unsigned int)(RISCV_CSRRW_TMESCR((RISCV_CSRRS_TMESCR(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_TMESCR_TMESCR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMESCR((RISCV_CSRRS_TMESCR(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_TMESVEC(a)  __builtin_linx_csrrw(0b011111100001, a)
#define RISCV_CSRRW_TMESVEC_TMESVEC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMESVEC((RISCV_CSRRS_TMESVEC(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_TMESVEC_TMESVEC_TMESVEC(a)  (unsigned int)(RISCV_CSRRW_TMESVEC((RISCV_CSRRS_TMESVEC(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_TMESTOP(a)  __builtin_linx_csrrw(0b011111100010, a)
#define RISCV_CSRRW_TMESTOP_TMESTOP_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMESTOP((RISCV_CSRRS_TMESTOP(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_TMESTOP_TMESTOP_TMESTOP(a)  (unsigned int)(RISCV_CSRRW_TMESTOP((RISCV_CSRRS_TMESTOP(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_TMEDELEG(a)  __builtin_linx_csrrw(0b011111100011, a)
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION0(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION1(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION2(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION3(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION4(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION5(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION6(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION7(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION8(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION9(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION10(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION11(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION12(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~4096) | (a << 12)) & 4096) >> 12
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION13(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~8192) | (a << 13)) & 8192) >> 13
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION14(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~16384) | (a << 14)) & 16384) >> 14
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION15(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~32768) | (a << 15)) & 32768) >> 15
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_EXCEPTION24(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_TMEDELEG_TMEDELEG_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_TMEDELEG((RISCV_CSRRS_TMEDELEG(0) & ~4261412864) | (a << 25)) & 4261412864) >> 25
#define RISCV_CSRRW_TMTVEC(a)  __builtin_linx_csrrw(0b011111100100, a)
#define RISCV_CSRRW_TMTVEC_TMTVEC_MODE(a)  (unsigned int)(RISCV_CSRRW_TMTVEC((RISCV_CSRRS_TMTVEC(0) & ~63) | (a << 0)) & 63) >> 0
#define RISCV_CSRRW_TMTVEC_TMTVEC_BASE(a)  (unsigned int)(RISCV_CSRRW_TMTVEC((RISCV_CSRRS_TMTVEC(0) & ~4294967232) | (a << 6)) & 4294967232) >> 6
#define RISCV_CSRRW_TMTVT(a)  __builtin_linx_csrrw(0b011111100101, a)
#define RISCV_CSRRW_TMTVT_TMTVT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMTVT((RISCV_CSRRS_TMTVT(0) & ~63) | (a << 0)) & 63) >> 0
#define RISCV_CSRRW_TMTVT_TMTVT_BASE(a)  (unsigned int)(RISCV_CSRRW_TMTVT((RISCV_CSRRS_TMTVT(0) & ~4294967232) | (a << 6)) & 4294967232) >> 6
#define RISCV_CSRRW_TMINTTHRESH(a)  __builtin_linx_csrrw(0b011111100110, a)
#define RISCV_CSRRW_TMINTTHRESH_TMINTTHRESH_SH(a)  (unsigned int)(RISCV_CSRRW_TMINTTHRESH((RISCV_CSRRS_TMINTTHRESH(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_TMINTTHRESH_TMINTTHRESH_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMINTTHRESH((RISCV_CSRRS_TMINTTHRESH(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_TMSTATUS(a)  __builtin_linx_csrrw(0b011111100111, a)
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_MIE(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~112) | (a << 4)) & 112) >> 4
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_MPIE(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED2(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~1792) | (a << 8)) & 1792) >> 8
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_MPP(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~6144) | (a << 11)) & 6144) >> 11
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED3(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~122880) | (a << 13)) & 122880) >> 13
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_MPRV(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~131072) | (a << 17)) & 131072) >> 17
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED4(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~1835008) | (a << 18)) & 1835008) >> 18
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_TW(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~2097152) | (a << 21)) & 2097152) >> 21
#define RISCV_CSRRW_TMSTATUS_TMSTATUS_RESERVED5(a)  (unsigned int)(RISCV_CSRRW_TMSTATUS((RISCV_CSRRS_TMSTATUS(0) & ~4290772992) | (a << 22)) & 4290772992) >> 22
#define RISCV_CSRRW_TMEPC(a)  __builtin_linx_csrrw(0b011111101000, a)
#define RISCV_CSRRW_TMEPC_TMEPC_0(a)  (unsigned int)(RISCV_CSRRW_TMEPC((RISCV_CSRRS_TMEPC(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TMEPC_TMEPC_TMEPC(a)  (unsigned int)(RISCV_CSRRW_TMEPC((RISCV_CSRRS_TMEPC(0) & ~4294967294) | (a << 1)) & 4294967294) >> 1
#define RISCV_CSRRW_TMCAUSE(a)  __builtin_linx_csrrw(0b011111101001, a)
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_EXCCODE(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~4095) | (a << 0)) & 4095) >> 0
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_UOPCNT3TO0(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~61440) | (a << 12)) & 61440) >> 12
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_MPIL(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~16711680) | (a << 16)) & 16711680) >> 16
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_UOPCNT4(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~16777216) | (a << 24)) & 16777216) >> 24
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~100663296) | (a << 25)) & 100663296) >> 25
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_MPIE(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~134217728) | (a << 27)) & 134217728) >> 27
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_MPP(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~805306368) | (a << 28)) & 805306368) >> 28
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_MINHV(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~1073741824) | (a << 30)) & 1073741824) >> 30
#define RISCV_CSRRW_TMCAUSE_TMCAUSE_INTERRUPT(a)  (unsigned int)(RISCV_CSRRW_TMCAUSE((RISCV_CSRRS_TMCAUSE(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_TCXCPTSC(a)  __builtin_linx_csrrw(0b111111000011, a)
#define RISCV_CSRRW_TCXCPTSC_TCXCPTSC_CCAUSE(a)  (unsigned int)(RISCV_CSRRW_TCXCPTSC((RISCV_CSRRS_TCXCPTSC(0) & ~31) | (a << 0)) & 31) >> 0
#define RISCV_CSRRW_TCXCPTSC_TCXCPTSC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TCXCPTSC((RISCV_CSRRS_TCXCPTSC(0) & ~4294967264) | (a << 5)) & 4294967264) >> 5
#define RISCV_CSRRW_TMTVAL(a)  __builtin_linx_csrrw(0b011111101011, a)
#define RISCV_CSRRW_TMSCRATCH(a)  __builtin_linx_csrrw(0b011111101100, a)
#define RISCV_CSRRW_TMESEPR(a)  __builtin_linx_csrrw(0b011111101101, a)
#define RISCV_CSRRW_TMESEPR_TMESEPR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMESEPR((RISCV_CSRRS_TMESEPR(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_TMESEPR_TMESEPR_TRADDR(a)  (unsigned int)(RISCV_CSRRW_TMESEPR((RISCV_CSRRS_TMESEPR(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_TMESEPS(a)  __builtin_linx_csrrw(0b011111101110, a)
#define RISCV_CSRRW_TMESEPS_TMESEPS_UTIE(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TMESEPS_TMESEPS_CTES(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_TMESEPS_TMESEPS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~252) | (a << 2)) & 252) >> 2
#define RISCV_CSRRW_TMESEPS_TMESEPS_PTES(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_TMESEPS_TMESEPS_PRET(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_TMESEPS_TMESEPS_PJMP(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_TMESEPS_TMESEPS_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_TMESEPS((RISCV_CSRRS_TMESEPS(0) & ~4294965248) | (a << 11)) & 4294965248) >> 11
#define RISCV_CSRRW_MSECCTL(a)  __builtin_linx_csrrw(0b011111101111, a)
#define RISCV_CSRRW_MSECCTL_MSECCTL_TVL(a)  (unsigned int)(RISCV_CSRRW_MSECCTL((RISCV_CSRRS_MSECCTL(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MSECCTL_MSECCTL_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MSECCTL((RISCV_CSRRS_MSECCTL(0) & ~4294967294) | (a << 1)) & 4294967294) >> 1
#define RISCV_CSRRW_PMPTE0(a)  __builtin_linx_csrrw(0b011111110000, a)
#define RISCV_CSRRW_PMPTE1(a)  __builtin_linx_csrrw(0b011111110001, a)
#define RISCV_CSRRW_MSPLIMIT(a)  __builtin_linx_csrrw(0b011111110010, a)
#define RISCV_CSRRW_MSPLIMIT_MSPLIMIT_EN(a)  (unsigned int)(RISCV_CSRRW_MSPLIMIT((RISCV_CSRRS_MSPLIMIT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_MSPLIMIT_MSPLIMIT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_MSPLIMIT((RISCV_CSRRS_MSPLIMIT(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_MSPLIMIT_MSPLIMIT_SPLIMIT(a)  (unsigned int)(RISCV_CSRRW_MSPLIMIT((RISCV_CSRRS_MSPLIMIT(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_TMSPLIMIT(a)  __builtin_linx_csrrw(0b011111110011, a)
#define RISCV_CSRRW_TMSPLIMIT_TMSPLIMIT_EN(a)  (unsigned int)(RISCV_CSRRW_TMSPLIMIT((RISCV_CSRRS_TMSPLIMIT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TMSPLIMIT_TMSPLIMIT_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TMSPLIMIT((RISCV_CSRRS_TMSPLIMIT(0) & ~14) | (a << 1)) & 14) >> 1
#define RISCV_CSRRW_TMSPLIMIT_TMSPLIMIT_SPLIMIT(a)  (unsigned int)(RISCV_CSRRW_TMSPLIMIT((RISCV_CSRRS_TMSPLIMIT(0) & ~4294967280) | (a << 4)) & 4294967280) >> 4
#define RISCV_CSRRW_TUSP(a)  __builtin_linx_csrrw(0b100000000000, a)
#define RISCV_CSRRW_TUGP(a)  __builtin_linx_csrrw(0b100000000001, a)
#define RISCV_CSRRW_TUTP(a)  __builtin_linx_csrrw(0b100000000010, a)
#define RISCV_CSRRW_TSP(a)  __builtin_linx_csrrw(0b100000000011, a)
#define RISCV_CSRRW_TGP(a)  __builtin_linx_csrrw(0b100000000100, a)
#define RISCV_CSRRW_TTP(a)  __builtin_linx_csrrw(0b100000000101, a)
#define RISCV_CSRRW_TESEPR(a)  __builtin_linx_csrrw(0b110011000000, a)
#define RISCV_CSRRW_TESEPR_TESEPR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TESEPR((RISCV_CSRRS_TESEPR(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_TESEPR_TESEPR_TRADDR(a)  (unsigned int)(RISCV_CSRRW_TESEPR((RISCV_CSRRS_TESEPR(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_TESEPS(a)  __builtin_linx_csrrw(0b110011000001, a)
#define RISCV_CSRRW_TESEPS_TESEPS_UTIE(a)  (unsigned int)(RISCV_CSRRW_TESEPS((RISCV_CSRRS_TESEPS(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TESEPS_TESEPS_CTES(a)  (unsigned int)(RISCV_CSRRW_TESEPS((RISCV_CSRRS_TESEPS(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_TESEPS_TESEPS_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TESEPS((RISCV_CSRRS_TESEPS(0) & ~4294967292) | (a << 2)) & 4294967292) >> 2
#define RISCV_CSRRW_VXSAT(a)  __builtin_linx_csrrw(0b000000001001, a)
#define RISCV_CSRRW_VXSAT_VXSAT(a)  (unsigned int)(RISCV_CSRRW_VXSAT((RISCV_CSRRS_VXSAT(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_VXRM(a)  __builtin_linx_csrrw(0b000000001010, a)
#define RISCV_CSRRW_VXRM_VXRM(a)  (unsigned int)(RISCV_CSRRW_VXRM((RISCV_CSRRS_VXRM(0) & ~3) | (a << 0)) & 3) >> 0
#define RISCV_CSRRW_VCSR(a)  __builtin_linx_csrrw(0b000000001111, a)
#define RISCV_CSRRW_VCSR_VXSAT(a)  (unsigned int)(RISCV_CSRRW_VCSR((RISCV_CSRRS_VCSR(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_VCSR_VXRM(a)  (unsigned int)(RISCV_CSRRW_VCSR((RISCV_CSRRS_VCSR(0) & ~6) | (a << 1)) & 6) >> 1
#define RISCV_CSRRW_VCSR_VCSR_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_VCSR((RISCV_CSRRS_VCSR(0) & ~4294967288) | (a << 3)) & 4294967288) >> 3
#define RISCV_CSRRW_VSTART(a)  __builtin_linx_csrrw(0b000000001000, a)
#define RISCV_CSRRW_VL(a)  __builtin_linx_csrrw(0b110000100000, a)
#define RISCV_CSRRW_VTYPE(a)  __builtin_linx_csrrw(0b110000100001, a)
#define RISCV_CSRRW_VTYPE_VTYPE_VLMUL(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~7) | (a << 0)) & 7) >> 0
#define RISCV_CSRRW_VTYPE_VTYPE_VSEW(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~56) | (a << 3)) & 56) >> 3
#define RISCV_CSRRW_VTYPE_VTYPE_VTA(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_VTYPE_VTYPE_VMA(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_VTYPE_VTYPE_VEDIV(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~768) | (a << 8)) & 768) >> 8
#define RISCV_CSRRW_VTYPE_VTYPE_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~2147482624) | (a << 10)) & 2147482624) >> 10
#define RISCV_CSRRW_VTYPE_VTYPE_VILL(a)  (unsigned int)(RISCV_CSRRW_VTYPE((RISCV_CSRRS_VTYPE(0) & ~2147483648) | (a << 31)) & 2147483648) >> 31
#define RISCV_CSRRW_VLENB(a)  __builtin_linx_csrrw(0b110000100010, a)
#define RISCV_CSRRW_JBC(a)  __builtin_linx_csrrw(0b100111110000, a)
#define RISCV_CSRRW_JBC_JBC_BYTE(a)  (unsigned int)(RISCV_CSRRW_JBC((RISCV_CSRRS_JBC(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_JBC_JBC_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_JBC((RISCV_CSRRS_JBC(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_JPC_UPPER(a)  __builtin_linx_csrrw(0b100111110001, a)
#define RISCV_CSRRW_JPC_LOWER(a)  __builtin_linx_csrrw(0b100111110010, a)
#define RISCV_CSRRW_JBCFT(a)  __builtin_linx_csrrw(0b100111110011, a)
#define RISCV_CSRRW_JBC_UPPER(a)  __builtin_linx_csrrw(0b100111110100, a)
#define RISCV_CSRRW_JBC_UPPER_JBC_UPPER_BYTE(a)  (unsigned int)(RISCV_CSRRW_JBC_UPPER((RISCV_CSRRS_JBC_UPPER(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_JBC_UPPER_JBC_UPPER_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_JBC_UPPER((RISCV_CSRRS_JBC_UPPER(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_JBC_SHADOW(a)  __builtin_linx_csrrw(0b110011110000, a)
#define RISCV_CSRRW_JBC_SHADOW_JBC_SHADOW_BYTE(a)  (unsigned int)(RISCV_CSRRW_JBC_SHADOW((RISCV_CSRRS_JBC_SHADOW(0) & ~255) | (a << 0)) & 255) >> 0
#define RISCV_CSRRW_JBC_SHADOW_JBC_SHADOW_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_JBC_SHADOW((RISCV_CSRRS_JBC_SHADOW(0) & ~4294967040) | (a << 8)) & 4294967040) >> 8
#define RISCV_CSRRW_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrw(0b100111111111, a)
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_MTVEC_LOCK(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_MTVT_LOCK(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_DUMMY_REG(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RANDOM_OPERANDS(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RANDOM_INSTRUCTION(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_FIXED_BRANCH(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_FIXED_DIVREM(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_INST_CMT_TIMEOUT(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_FUNC_MARKER(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~256) | (a << 8)) & 256) >> 8
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RAS_CHECK(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~512) | (a << 9)) & 512) >> 9
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RAS_INVALID(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~1024) | (a << 10)) & 1024) >> 10
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RJUMP_MARKER(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_SEC_ENHANCE_CTRL_SEC_ENHANCE_CTRL_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_SEC_ENHANCE_CTRL((RISCV_CSRRS_SEC_ENHANCE_CTRL(0) & ~4294963200) | (a << 12)) & 4294963200) >> 12
#define RISCV_CSRRW_TSELECT(a)  __builtin_linx_csrrw(0b011110100000, a)
#define RISCV_CSRRW_TDATA1(a)  __builtin_linx_csrrw(0b011110100001, a)
#define RISCV_CSRRW_TDATA1_TDATA1_LOAD(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~1) | (a << 0)) & 1) >> 0
#define RISCV_CSRRW_TDATA1_TDATA1_STORE(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~2) | (a << 1)) & 2) >> 1
#define RISCV_CSRRW_TDATA1_TDATA1_EXECUTE(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~4) | (a << 2)) & 4) >> 2
#define RISCV_CSRRW_TDATA1_TDATA1_U(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~8) | (a << 3)) & 8) >> 3
#define RISCV_CSRRW_TDATA1_TDATA1_S(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~16) | (a << 4)) & 16) >> 4
#define RISCV_CSRRW_TDATA1_TDATA1_RESERVED0(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~32) | (a << 5)) & 32) >> 5
#define RISCV_CSRRW_TDATA1_TDATA1_M(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~64) | (a << 6)) & 64) >> 6
#define RISCV_CSRRW_TDATA1_TDATA1_MATCH(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~128) | (a << 7)) & 128) >> 7
#define RISCV_CSRRW_TDATA1_TDATA1_RESERVED1(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~1792) | (a << 8)) & 1792) >> 8
#define RISCV_CSRRW_TDATA1_TDATA1_CHAIN(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~2048) | (a << 11)) & 2048) >> 11
#define RISCV_CSRRW_TDATA1_TDATA1_ACTION(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~61440) | (a << 12)) & 61440) >> 12
#define RISCV_CSRRW_TDATA1_TDATA1_SIZELO(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~196608) | (a << 16)) & 196608) >> 16
#define RISCV_CSRRW_TDATA1_TDATA1_TIMING(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~262144) | (a << 18)) & 262144) >> 18
#define RISCV_CSRRW_TDATA1_TDATA1_SELECT(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~524288) | (a << 19)) & 524288) >> 19
#define RISCV_CSRRW_TDATA1_TDATA1_HIT(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~1048576) | (a << 20)) & 1048576) >> 20
#define RISCV_CSRRW_TDATA1_TDATA1_MASKMAX(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~132120576) | (a << 21)) & 132120576) >> 21
#define RISCV_CSRRW_TDATA1_TDATA1_DMODE(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~134217728) | (a << 27)) & 134217728) >> 27
#define RISCV_CSRRW_TDATA1_TDATA1_TYPE(a)  (unsigned int)(RISCV_CSRRW_TDATA1((RISCV_CSRRS_TDATA1(0) & ~4026531840) | (a << 28)) & 4026531840) >> 28
#define RISCV_CSRRW_TDATA2(a)  __builtin_linx_csrrw(0b011110100010, a)
#define RISCV_CSRRW_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRW_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRW_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRW_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRW_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRW_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRW_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRW_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRW_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRW_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRW_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRW_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRW_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRW_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRW_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRW_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRW_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRW_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRW_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRW_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRW_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRW_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRW_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRW_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRW_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRW_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRW_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRW_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRW_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRW_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRW_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRW_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRW_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRW_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRW_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRW_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRW_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRW_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRW_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRW_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRW_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRW_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRW_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRW_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRW_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRW_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRW_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRW_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRW_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRW_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRW_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRW_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRW_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRW_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRW_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRW_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRW_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRW_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRW_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRW_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRW_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRW_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRW_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRW_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRW_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRW_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRW_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRW_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRW_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRW_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRW_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRW_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRW_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRW_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRW_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRW_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRW_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRW_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRW_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRW_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRW_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRW_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRWI  __builtin_linx_csrrwi
#define RISCV_CSRRWI_DCSR(a)  __builtin_linx_csrrwi(0b011110110000, a)
#define RISCV_CSRRWI_DPC(a)  __builtin_linx_csrrwi(0b011110110001, a)
#define RISCV_CSRRWI_DSCRATCH0(a)  __builtin_linx_csrrwi(0b011110110010, a)
#define RISCV_CSRRWI_MVENDORID(a)  __builtin_linx_csrrwi(0b111100010001, a)
#define RISCV_CSRRWI_MARCHID(a)  __builtin_linx_csrrwi(0b111100010010, a)
#define RISCV_CSRRWI_MIMPID(a)  __builtin_linx_csrrwi(0b111100010011, a)
#define RISCV_CSRRWI_MHARTID(a)  __builtin_linx_csrrwi(0b111100010100, a)
#define RISCV_CSRRWI_MSTATUS(a)  __builtin_linx_csrrwi(0b001100000000, a)
#define RISCV_CSRRWI_MISA(a)  __builtin_linx_csrrwi(0b001100000001, a)
#define RISCV_CSRRWI_MIE(a)  __builtin_linx_csrrwi(0b001100000100, a)
#define RISCV_CSRRWI_MTVEC(a)  __builtin_linx_csrrwi(0b001100000101, a)
#define RISCV_CSRRWI_MIP(a)  __builtin_linx_csrrwi(0b001101000100, a)
#define RISCV_CSRRWI_MSECCFG(a)  __builtin_linx_csrrwi(0b001110010000, a)
#define RISCV_CSRRWI_MCOUNTEREN(a)  __builtin_linx_csrrwi(0b001100000110, a)
#define RISCV_CSRRWI_MCOUNTINHIBIT(a)  __builtin_linx_csrrwi(0b001100100000, a)
#define RISCV_CSRRWI_MSCRATCH(a)  __builtin_linx_csrrwi(0b001101000000, a)
#define RISCV_CSRRWI_MEPC(a)  __builtin_linx_csrrwi(0b001101000001, a)
#define RISCV_CSRRWI_MCAUSE(a)  __builtin_linx_csrrwi(0b001101000010, a)
#define RISCV_CSRRWI_MTVAL(a)  __builtin_linx_csrrwi(0b001101000011, a)
#define RISCV_CSRRWI_PMPCFG0(a)  __builtin_linx_csrrwi(0b001110100000, a)
#define RISCV_CSRRWI_PMPCFG1(a)  __builtin_linx_csrrwi(0b001110100001, a)
#define RISCV_CSRRWI_PMPCFG2(a)  __builtin_linx_csrrwi(0b001110100010, a)
#define RISCV_CSRRWI_PMPCFG3(a)  __builtin_linx_csrrwi(0b001110100011, a)
#define RISCV_CSRRWI_PMPCFG4(a)  __builtin_linx_csrrwi(0b001110100100, a)
#define RISCV_CSRRWI_PMPCFG5(a)  __builtin_linx_csrrwi(0b001110100101, a)
#define RISCV_CSRRWI_PMPCFG6(a)  __builtin_linx_csrrwi(0b001110100110, a)
#define RISCV_CSRRWI_PMPCFG7(a)  __builtin_linx_csrrwi(0b001110100111, a)
#define RISCV_CSRRWI_PMPCFG8(a)  __builtin_linx_csrrwi(0b001110101000, a)
#define RISCV_CSRRWI_PMPCFG9(a)  __builtin_linx_csrrwi(0b001110101001, a)
#define RISCV_CSRRWI_PMPCFG10(a)  __builtin_linx_csrrwi(0b001110101010, a)
#define RISCV_CSRRWI_PMPCFG11(a)  __builtin_linx_csrrwi(0b001110101011, a)
#define RISCV_CSRRWI_PMPCFG12(a)  __builtin_linx_csrrwi(0b001110101100, a)
#define RISCV_CSRRWI_PMPCFG13(a)  __builtin_linx_csrrwi(0b001110101101, a)
#define RISCV_CSRRWI_PMPCFG14(a)  __builtin_linx_csrrwi(0b001110101110, a)
#define RISCV_CSRRWI_PMPCFG15(a)  __builtin_linx_csrrwi(0b001110101111, a)
#define RISCV_CSRRWI_PMPADDR0(a)  __builtin_linx_csrrwi(0b001110110000, a)
#define RISCV_CSRRWI_PMPADDR1(a)  __builtin_linx_csrrwi(0b001110110001, a)
#define RISCV_CSRRWI_PMPADDR2(a)  __builtin_linx_csrrwi(0b001110110010, a)
#define RISCV_CSRRWI_PMPADDR3(a)  __builtin_linx_csrrwi(0b001110110011, a)
#define RISCV_CSRRWI_PMPADDR4(a)  __builtin_linx_csrrwi(0b001110110100, a)
#define RISCV_CSRRWI_PMPADDR5(a)  __builtin_linx_csrrwi(0b001110110101, a)
#define RISCV_CSRRWI_PMPADDR6(a)  __builtin_linx_csrrwi(0b001110110110, a)
#define RISCV_CSRRWI_PMPADDR7(a)  __builtin_linx_csrrwi(0b001110110111, a)
#define RISCV_CSRRWI_PMPADDR8(a)  __builtin_linx_csrrwi(0b001110111000, a)
#define RISCV_CSRRWI_PMPADDR9(a)  __builtin_linx_csrrwi(0b001110111001, a)
#define RISCV_CSRRWI_PMPADDR10(a)  __builtin_linx_csrrwi(0b001110111010, a)
#define RISCV_CSRRWI_PMPADDR11(a)  __builtin_linx_csrrwi(0b001110111011, a)
#define RISCV_CSRRWI_PMPADDR12(a)  __builtin_linx_csrrwi(0b001110111100, a)
#define RISCV_CSRRWI_PMPADDR13(a)  __builtin_linx_csrrwi(0b001110111101, a)
#define RISCV_CSRRWI_PMPADDR14(a)  __builtin_linx_csrrwi(0b001110111110, a)
#define RISCV_CSRRWI_PMPADDR15(a)  __builtin_linx_csrrwi(0b001110111111, a)
#define RISCV_CSRRWI_PMPADDR16(a)  __builtin_linx_csrrwi(0b001111000000, a)
#define RISCV_CSRRWI_PMPADDR17(a)  __builtin_linx_csrrwi(0b001111000001, a)
#define RISCV_CSRRWI_PMPADDR18(a)  __builtin_linx_csrrwi(0b001111000010, a)
#define RISCV_CSRRWI_PMPADDR19(a)  __builtin_linx_csrrwi(0b001111000011, a)
#define RISCV_CSRRWI_PMPADDR20(a)  __builtin_linx_csrrwi(0b001111000100, a)
#define RISCV_CSRRWI_PMPADDR21(a)  __builtin_linx_csrrwi(0b001111000101, a)
#define RISCV_CSRRWI_PMPADDR22(a)  __builtin_linx_csrrwi(0b001111000110, a)
#define RISCV_CSRRWI_PMPADDR23(a)  __builtin_linx_csrrwi(0b001111000111, a)
#define RISCV_CSRRWI_PMPADDR24(a)  __builtin_linx_csrrwi(0b001111001000, a)
#define RISCV_CSRRWI_PMPADDR25(a)  __builtin_linx_csrrwi(0b001111001001, a)
#define RISCV_CSRRWI_PMPADDR26(a)  __builtin_linx_csrrwi(0b001111001010, a)
#define RISCV_CSRRWI_PMPADDR27(a)  __builtin_linx_csrrwi(0b001111001011, a)
#define RISCV_CSRRWI_PMPADDR28(a)  __builtin_linx_csrrwi(0b001111001100, a)
#define RISCV_CSRRWI_PMPADDR29(a)  __builtin_linx_csrrwi(0b001111001101, a)
#define RISCV_CSRRWI_PMPADDR30(a)  __builtin_linx_csrrwi(0b001111001110, a)
#define RISCV_CSRRWI_PMPADDR31(a)  __builtin_linx_csrrwi(0b001111001111, a)
#define RISCV_CSRRWI_PMPADDR32(a)  __builtin_linx_csrrwi(0b001111010000, a)
#define RISCV_CSRRWI_PMPADDR33(a)  __builtin_linx_csrrwi(0b001111010001, a)
#define RISCV_CSRRWI_PMPADDR34(a)  __builtin_linx_csrrwi(0b001111010010, a)
#define RISCV_CSRRWI_PMPADDR35(a)  __builtin_linx_csrrwi(0b001111010011, a)
#define RISCV_CSRRWI_PMPADDR36(a)  __builtin_linx_csrrwi(0b001111010100, a)
#define RISCV_CSRRWI_PMPADDR37(a)  __builtin_linx_csrrwi(0b001111010101, a)
#define RISCV_CSRRWI_PMPADDR38(a)  __builtin_linx_csrrwi(0b001111010110, a)
#define RISCV_CSRRWI_PMPADDR39(a)  __builtin_linx_csrrwi(0b001111010111, a)
#define RISCV_CSRRWI_PMPADDR40(a)  __builtin_linx_csrrwi(0b001111011000, a)
#define RISCV_CSRRWI_PMPADDR41(a)  __builtin_linx_csrrwi(0b001111011001, a)
#define RISCV_CSRRWI_PMPADDR42(a)  __builtin_linx_csrrwi(0b001111011010, a)
#define RISCV_CSRRWI_PMPADDR43(a)  __builtin_linx_csrrwi(0b001111011011, a)
#define RISCV_CSRRWI_PMPADDR44(a)  __builtin_linx_csrrwi(0b001111011100, a)
#define RISCV_CSRRWI_PMPADDR45(a)  __builtin_linx_csrrwi(0b001111011101, a)
#define RISCV_CSRRWI_PMPADDR46(a)  __builtin_linx_csrrwi(0b001111011110, a)
#define RISCV_CSRRWI_PMPADDR47(a)  __builtin_linx_csrrwi(0b001111011111, a)
#define RISCV_CSRRWI_PMPADDR48(a)  __builtin_linx_csrrwi(0b001111100000, a)
#define RISCV_CSRRWI_PMPADDR49(a)  __builtin_linx_csrrwi(0b001111100001, a)
#define RISCV_CSRRWI_PMPADDR50(a)  __builtin_linx_csrrwi(0b001111100010, a)
#define RISCV_CSRRWI_PMPADDR51(a)  __builtin_linx_csrrwi(0b001111100011, a)
#define RISCV_CSRRWI_PMPADDR52(a)  __builtin_linx_csrrwi(0b001111100100, a)
#define RISCV_CSRRWI_PMPADDR53(a)  __builtin_linx_csrrwi(0b001111100101, a)
#define RISCV_CSRRWI_PMPADDR54(a)  __builtin_linx_csrrwi(0b001111100110, a)
#define RISCV_CSRRWI_PMPADDR55(a)  __builtin_linx_csrrwi(0b001111100111, a)
#define RISCV_CSRRWI_PMPADDR56(a)  __builtin_linx_csrrwi(0b001111101000, a)
#define RISCV_CSRRWI_PMPADDR57(a)  __builtin_linx_csrrwi(0b001111101001, a)
#define RISCV_CSRRWI_PMPADDR58(a)  __builtin_linx_csrrwi(0b001111101010, a)
#define RISCV_CSRRWI_PMPADDR59(a)  __builtin_linx_csrrwi(0b001111101011, a)
#define RISCV_CSRRWI_PMPADDR60(a)  __builtin_linx_csrrwi(0b001111101100, a)
#define RISCV_CSRRWI_PMPADDR61(a)  __builtin_linx_csrrwi(0b001111101101, a)
#define RISCV_CSRRWI_PMPADDR62(a)  __builtin_linx_csrrwi(0b001111101110, a)
#define RISCV_CSRRWI_PMPADDR63(a)  __builtin_linx_csrrwi(0b001111101111, a)
#define RISCV_CSRRWI_MTVT(a)  __builtin_linx_csrrwi(0b001100000111, a)
#define RISCV_CSRRWI_MINTSTATUS(a)  __builtin_linx_csrrwi(0b001101000110, a)
#define RISCV_CSRRWI_MINTTHRESH(a)  __builtin_linx_csrrwi(0b001101000111, a)
#define RISCV_CSRRWI_MSCRATCHCSW(a)  __builtin_linx_csrrwi(0b001101001000, a)
#define RISCV_CSRRWI_MSCRATCHCSWL(a)  __builtin_linx_csrrwi(0b001101001001, a)
#define RISCV_CSRRWI_MCLICBASE(a)  __builtin_linx_csrrwi(0b111110111111, a)
#define RISCV_CSRRWI_MHPMEVENT3(a)  __builtin_linx_csrrwi(0b001100100011, a)
#define RISCV_CSRRWI_MHPMEVENT4(a)  __builtin_linx_csrrwi(0b001100100100, a)
#define RISCV_CSRRWI_MHPMEVENT5(a)  __builtin_linx_csrrwi(0b001100100101, a)
#define RISCV_CSRRWI_MHPMEVENT6(a)  __builtin_linx_csrrwi(0b001100100110, a)
#define RISCV_CSRRWI_MHPMEVENT7(a)  __builtin_linx_csrrwi(0b001100100111, a)
#define RISCV_CSRRWI_MHPMEVENT8(a)  __builtin_linx_csrrwi(0b001100101000, a)
#define RISCV_CSRRWI_MHPMEVENT9(a)  __builtin_linx_csrrwi(0b001100101001, a)
#define RISCV_CSRRWI_MHPMEVENT10(a)  __builtin_linx_csrrwi(0b001100101010, a)
#define RISCV_CSRRWI_MHPMEVENT11(a)  __builtin_linx_csrrwi(0b001100101011, a)
#define RISCV_CSRRWI_MHPMEVENT12(a)  __builtin_linx_csrrwi(0b001100101100, a)
#define RISCV_CSRRWI_MHPMEVENT13(a)  __builtin_linx_csrrwi(0b001100101101, a)
#define RISCV_CSRRWI_MHPMEVENT14(a)  __builtin_linx_csrrwi(0b001100101110, a)
#define RISCV_CSRRWI_MHPMEVENT15(a)  __builtin_linx_csrrwi(0b001100101111, a)
#define RISCV_CSRRWI_MHPMEVENT16(a)  __builtin_linx_csrrwi(0b001100110000, a)
#define RISCV_CSRRWI_MHPMEVENT17(a)  __builtin_linx_csrrwi(0b001100110001, a)
#define RISCV_CSRRWI_MHPMEVENT18(a)  __builtin_linx_csrrwi(0b001100110010, a)
#define RISCV_CSRRWI_MHPMEVENT19(a)  __builtin_linx_csrrwi(0b001100110011, a)
#define RISCV_CSRRWI_MHPMEVENT20(a)  __builtin_linx_csrrwi(0b001100110100, a)
#define RISCV_CSRRWI_MHPMEVENT21(a)  __builtin_linx_csrrwi(0b001100110101, a)
#define RISCV_CSRRWI_MHPMEVENT22(a)  __builtin_linx_csrrwi(0b001100110110, a)
#define RISCV_CSRRWI_MHPMEVENT23(a)  __builtin_linx_csrrwi(0b001100110111, a)
#define RISCV_CSRRWI_MHPMEVENT24(a)  __builtin_linx_csrrwi(0b001100111000, a)
#define RISCV_CSRRWI_MHPMEVENT25(a)  __builtin_linx_csrrwi(0b001100111001, a)
#define RISCV_CSRRWI_MHPMEVENT26(a)  __builtin_linx_csrrwi(0b001100111010, a)
#define RISCV_CSRRWI_MHPMEVENT27(a)  __builtin_linx_csrrwi(0b001100111011, a)
#define RISCV_CSRRWI_MHPMEVENT28(a)  __builtin_linx_csrrwi(0b001100111100, a)
#define RISCV_CSRRWI_MHPMEVENT29(a)  __builtin_linx_csrrwi(0b001100111101, a)
#define RISCV_CSRRWI_MHPMEVENT30(a)  __builtin_linx_csrrwi(0b001100111110, a)
#define RISCV_CSRRWI_MHPMEVENT31(a)  __builtin_linx_csrrwi(0b001100111111, a)
#define RISCV_CSRRWI_MCYCLE(a)  __builtin_linx_csrrwi(0b101100000000, a)
#define RISCV_CSRRWI_MINSTRET(a)  __builtin_linx_csrrwi(0b101100000010, a)
#define RISCV_CSRRWI_MHPMCOUNTER3(a)  __builtin_linx_csrrwi(0b101100000011, a)
#define RISCV_CSRRWI_MHPMCOUNTER4(a)  __builtin_linx_csrrwi(0b101100000100, a)
#define RISCV_CSRRWI_MHPMCOUNTER5(a)  __builtin_linx_csrrwi(0b101100000101, a)
#define RISCV_CSRRWI_MHPMCOUNTER6(a)  __builtin_linx_csrrwi(0b101100000110, a)
#define RISCV_CSRRWI_MHPMCOUNTER7(a)  __builtin_linx_csrrwi(0b101100000111, a)
#define RISCV_CSRRWI_MHPMCOUNTER8(a)  __builtin_linx_csrrwi(0b101100001000, a)
#define RISCV_CSRRWI_MHPMCOUNTER9(a)  __builtin_linx_csrrwi(0b101100001001, a)
#define RISCV_CSRRWI_MHPMCOUNTER10(a)  __builtin_linx_csrrwi(0b101100001010, a)
#define RISCV_CSRRWI_MHPMCOUNTER11(a)  __builtin_linx_csrrwi(0b101100001011, a)
#define RISCV_CSRRWI_MHPMCOUNTER12(a)  __builtin_linx_csrrwi(0b101100001100, a)
#define RISCV_CSRRWI_MHPMCOUNTER13(a)  __builtin_linx_csrrwi(0b101100001101, a)
#define RISCV_CSRRWI_MHPMCOUNTER14(a)  __builtin_linx_csrrwi(0b101100001110, a)
#define RISCV_CSRRWI_MHPMCOUNTER15(a)  __builtin_linx_csrrwi(0b101100001111, a)
#define RISCV_CSRRWI_MHPMCOUNTER16(a)  __builtin_linx_csrrwi(0b101100010000, a)
#define RISCV_CSRRWI_MHPMCOUNTER17(a)  __builtin_linx_csrrwi(0b101100010001, a)
#define RISCV_CSRRWI_MHPMCOUNTER18(a)  __builtin_linx_csrrwi(0b101100010010, a)
#define RISCV_CSRRWI_MHPMCOUNTER19(a)  __builtin_linx_csrrwi(0b101100010011, a)
#define RISCV_CSRRWI_MHPMCOUNTER20(a)  __builtin_linx_csrrwi(0b101100010100, a)
#define RISCV_CSRRWI_MHPMCOUNTER21(a)  __builtin_linx_csrrwi(0b101100010101, a)
#define RISCV_CSRRWI_MHPMCOUNTER22(a)  __builtin_linx_csrrwi(0b101100010110, a)
#define RISCV_CSRRWI_MHPMCOUNTER23(a)  __builtin_linx_csrrwi(0b101100010111, a)
#define RISCV_CSRRWI_MHPMCOUNTER24(a)  __builtin_linx_csrrwi(0b101100011000, a)
#define RISCV_CSRRWI_MHPMCOUNTER25(a)  __builtin_linx_csrrwi(0b101100011001, a)
#define RISCV_CSRRWI_MHPMCOUNTER26(a)  __builtin_linx_csrrwi(0b101100011010, a)
#define RISCV_CSRRWI_MHPMCOUNTER27(a)  __builtin_linx_csrrwi(0b101100011011, a)
#define RISCV_CSRRWI_MHPMCOUNTER28(a)  __builtin_linx_csrrwi(0b101100011100, a)
#define RISCV_CSRRWI_MHPMCOUNTER29(a)  __builtin_linx_csrrwi(0b101100011101, a)
#define RISCV_CSRRWI_MHPMCOUNTER30(a)  __builtin_linx_csrrwi(0b101100011110, a)
#define RISCV_CSRRWI_MHPMCOUNTER31(a)  __builtin_linx_csrrwi(0b101100011111, a)
#define RISCV_CSRRWI_MCYCLEH(a)  __builtin_linx_csrrwi(0b101110000000, a)
#define RISCV_CSRRWI_MINSTRETH(a)  __builtin_linx_csrrwi(0b101110000010, a)
#define RISCV_CSRRWI_MHPMCOUNTER3H(a)  __builtin_linx_csrrwi(0b101110000011, a)
#define RISCV_CSRRWI_MHPMCOUNTER4H(a)  __builtin_linx_csrrwi(0b101110000100, a)
#define RISCV_CSRRWI_MHPMCOUNTER5H(a)  __builtin_linx_csrrwi(0b101110000101, a)
#define RISCV_CSRRWI_MHPMCOUNTER6H(a)  __builtin_linx_csrrwi(0b101110000110, a)
#define RISCV_CSRRWI_MHPMCOUNTER7H(a)  __builtin_linx_csrrwi(0b101110000111, a)
#define RISCV_CSRRWI_MHPMCOUNTER8H(a)  __builtin_linx_csrrwi(0b101110001000, a)
#define RISCV_CSRRWI_MHPMCOUNTER9H(a)  __builtin_linx_csrrwi(0b101110001001, a)
#define RISCV_CSRRWI_MHPMCOUNTER10H(a)  __builtin_linx_csrrwi(0b101110001010, a)
#define RISCV_CSRRWI_MHPMCOUNTER11H(a)  __builtin_linx_csrrwi(0b101110001011, a)
#define RISCV_CSRRWI_MHPMCOUNTER12H(a)  __builtin_linx_csrrwi(0b101110001100, a)
#define RISCV_CSRRWI_MHPMCOUNTER13H(a)  __builtin_linx_csrrwi(0b101110001101, a)
#define RISCV_CSRRWI_MHPMCOUNTER14H(a)  __builtin_linx_csrrwi(0b101110001110, a)
#define RISCV_CSRRWI_MHPMCOUNTER15H(a)  __builtin_linx_csrrwi(0b101110001111, a)
#define RISCV_CSRRWI_MHPMCOUNTER16H(a)  __builtin_linx_csrrwi(0b101110010000, a)
#define RISCV_CSRRWI_MHPMCOUNTER17H(a)  __builtin_linx_csrrwi(0b101110010001, a)
#define RISCV_CSRRWI_MHPMCOUNTER18H(a)  __builtin_linx_csrrwi(0b101110010010, a)
#define RISCV_CSRRWI_MHPMCOUNTER19H(a)  __builtin_linx_csrrwi(0b101110010011, a)
#define RISCV_CSRRWI_MHPMCOUNTER20H(a)  __builtin_linx_csrrwi(0b101110010100, a)
#define RISCV_CSRRWI_MHPMCOUNTER21H(a)  __builtin_linx_csrrwi(0b101110010101, a)
#define RISCV_CSRRWI_MHPMCOUNTER22H(a)  __builtin_linx_csrrwi(0b101110010110, a)
#define RISCV_CSRRWI_MHPMCOUNTER23H(a)  __builtin_linx_csrrwi(0b101110010111, a)
#define RISCV_CSRRWI_MHPMCOUNTER24H(a)  __builtin_linx_csrrwi(0b101110011000, a)
#define RISCV_CSRRWI_MHPMCOUNTER25H(a)  __builtin_linx_csrrwi(0b101110011001, a)
#define RISCV_CSRRWI_MHPMCOUNTER26H(a)  __builtin_linx_csrrwi(0b101110011010, a)
#define RISCV_CSRRWI_MHPMCOUNTER27H(a)  __builtin_linx_csrrwi(0b101110011011, a)
#define RISCV_CSRRWI_MHPMCOUNTER28H(a)  __builtin_linx_csrrwi(0b101110011100, a)
#define RISCV_CSRRWI_MHPMCOUNTER29H(a)  __builtin_linx_csrrwi(0b101110011101, a)
#define RISCV_CSRRWI_MHPMCOUNTER30H(a)  __builtin_linx_csrrwi(0b101110011110, a)
#define RISCV_CSRRWI_MHPMCOUNTER31H(a)  __builtin_linx_csrrwi(0b101110011111, a)
#define RISCV_CSRRWI_ICCTL(a)  __builtin_linx_csrrwi(0b011111000000, a)
#define RISCV_CSRRWI_DCCTL(a)  __builtin_linx_csrrwi(0b011111000001, a)
#define RISCV_CSRRWI_ICMAINT(a)  __builtin_linx_csrrwi(0b011111000010, a)
#define RISCV_CSRRWI_DCMAINT(a)  __builtin_linx_csrrwi(0b011111000011, a)
#define RISCV_CSRRWI_ICINVA(a)  __builtin_linx_csrrwi(0b011111000100, a)
#define RISCV_CSRRWI_DCINCVA(a)  __builtin_linx_csrrwi(0b011111000101, a)
#define RISCV_CSRRWI_APREFI(a)  __builtin_linx_csrrwi(0b011111000110, a)
#define RISCV_CSRRWI_APREFD(a)  __builtin_linx_csrrwi(0b011111000111, a)
#define RISCV_CSRRWI_WST(a)  __builtin_linx_csrrwi(0b011111010000, a)
#define RISCV_CSRRWI_TCMARB(a)  __builtin_linx_csrrwi(0b011111010010, a)
#define RISCV_CSRRWI_MEMATTR0(a)  __builtin_linx_csrrwi(0b101111000000, a)
#define RISCV_CSRRWI_MEMATTR1(a)  __builtin_linx_csrrwi(0b101111000001, a)
#define RISCV_CSRRWI_MEMATTR2(a)  __builtin_linx_csrrwi(0b101111000010, a)
#define RISCV_CSRRWI_MEMATTR3(a)  __builtin_linx_csrrwi(0b101111000011, a)
#define RISCV_CSRRWI_MEMATTR4(a)  __builtin_linx_csrrwi(0b101111000100, a)
#define RISCV_CSRRWI_MEMATTR5(a)  __builtin_linx_csrrwi(0b101111000101, a)
#define RISCV_CSRRWI_MEMATTR6(a)  __builtin_linx_csrrwi(0b101111000110, a)
#define RISCV_CSRRWI_MEMATTR7(a)  __builtin_linx_csrrwi(0b101111000111, a)
#define RISCV_CSRRWI_MEMATTR8(a)  __builtin_linx_csrrwi(0b101111001000, a)
#define RISCV_CSRRWI_MEMATTR9(a)  __builtin_linx_csrrwi(0b101111001001, a)
#define RISCV_CSRRWI_MEMATTR10(a)  __builtin_linx_csrrwi(0b101111001010, a)
#define RISCV_CSRRWI_MEMATTR11(a)  __builtin_linx_csrrwi(0b101111001011, a)
#define RISCV_CSRRWI_MEMATTR12(a)  __builtin_linx_csrrwi(0b101111001100, a)
#define RISCV_CSRRWI_MEMATTR13(a)  __builtin_linx_csrrwi(0b101111001101, a)
#define RISCV_CSRRWI_MEMATTR14(a)  __builtin_linx_csrrwi(0b101111001110, a)
#define RISCV_CSRRWI_MEMATTR15(a)  __builtin_linx_csrrwi(0b101111001111, a)
#define RISCV_CSRRWI_MEMATTR16(a)  __builtin_linx_csrrwi(0b101111010000, a)
#define RISCV_CSRRWI_MEMATTR17(a)  __builtin_linx_csrrwi(0b101111010001, a)
#define RISCV_CSRRWI_MEMATTR18(a)  __builtin_linx_csrrwi(0b101111010010, a)
#define RISCV_CSRRWI_MEMATTR19(a)  __builtin_linx_csrrwi(0b101111010011, a)
#define RISCV_CSRRWI_MEMATTR20(a)  __builtin_linx_csrrwi(0b101111010100, a)
#define RISCV_CSRRWI_MEMATTR21(a)  __builtin_linx_csrrwi(0b101111010101, a)
#define RISCV_CSRRWI_MEMATTR22(a)  __builtin_linx_csrrwi(0b101111010110, a)
#define RISCV_CSRRWI_MEMATTR23(a)  __builtin_linx_csrrwi(0b101111010111, a)
#define RISCV_CSRRWI_MEMATTR24(a)  __builtin_linx_csrrwi(0b101111011000, a)
#define RISCV_CSRRWI_MEMATTR25(a)  __builtin_linx_csrrwi(0b101111011001, a)
#define RISCV_CSRRWI_MEMATTR26(a)  __builtin_linx_csrrwi(0b101111011010, a)
#define RISCV_CSRRWI_MEMATTR27(a)  __builtin_linx_csrrwi(0b101111011011, a)
#define RISCV_CSRRWI_MEMATTR28(a)  __builtin_linx_csrrwi(0b101111011100, a)
#define RISCV_CSRRWI_MEMATTR29(a)  __builtin_linx_csrrwi(0b101111011101, a)
#define RISCV_CSRRWI_MEMATTR30(a)  __builtin_linx_csrrwi(0b101111011110, a)
#define RISCV_CSRRWI_MEMATTR31(a)  __builtin_linx_csrrwi(0b101111011111, a)
#define RISCV_CSRRWI_MEMATTR32(a)  __builtin_linx_csrrwi(0b101111100000, a)
#define RISCV_CSRRWI_MEMATTR33(a)  __builtin_linx_csrrwi(0b101111100001, a)
#define RISCV_CSRRWI_MEMATTR34(a)  __builtin_linx_csrrwi(0b101111100010, a)
#define RISCV_CSRRWI_MEMATTR35(a)  __builtin_linx_csrrwi(0b101111100011, a)
#define RISCV_CSRRWI_MEMATTR36(a)  __builtin_linx_csrrwi(0b101111100100, a)
#define RISCV_CSRRWI_MEMATTR37(a)  __builtin_linx_csrrwi(0b101111100101, a)
#define RISCV_CSRRWI_MEMATTR38(a)  __builtin_linx_csrrwi(0b101111100110, a)
#define RISCV_CSRRWI_MEMATTR39(a)  __builtin_linx_csrrwi(0b101111100111, a)
#define RISCV_CSRRWI_MEMATTR40(a)  __builtin_linx_csrrwi(0b101111101000, a)
#define RISCV_CSRRWI_MEMATTR41(a)  __builtin_linx_csrrwi(0b101111101001, a)
#define RISCV_CSRRWI_MEMATTR42(a)  __builtin_linx_csrrwi(0b101111101010, a)
#define RISCV_CSRRWI_MEMATTR43(a)  __builtin_linx_csrrwi(0b101111101011, a)
#define RISCV_CSRRWI_MEMATTR44(a)  __builtin_linx_csrrwi(0b101111101100, a)
#define RISCV_CSRRWI_MEMATTR45(a)  __builtin_linx_csrrwi(0b101111101101, a)
#define RISCV_CSRRWI_MEMATTR46(a)  __builtin_linx_csrrwi(0b101111101110, a)
#define RISCV_CSRRWI_MEMATTR47(a)  __builtin_linx_csrrwi(0b101111101111, a)
#define RISCV_CSRRWI_MEMATTR48(a)  __builtin_linx_csrrwi(0b101111110000, a)
#define RISCV_CSRRWI_MEMATTR49(a)  __builtin_linx_csrrwi(0b101111110001, a)
#define RISCV_CSRRWI_MEMATTR50(a)  __builtin_linx_csrrwi(0b101111110010, a)
#define RISCV_CSRRWI_MEMATTR51(a)  __builtin_linx_csrrwi(0b101111110011, a)
#define RISCV_CSRRWI_MEMATTR52(a)  __builtin_linx_csrrwi(0b101111110100, a)
#define RISCV_CSRRWI_MEMATTR53(a)  __builtin_linx_csrrwi(0b101111110101, a)
#define RISCV_CSRRWI_MEMATTR54(a)  __builtin_linx_csrrwi(0b101111110110, a)
#define RISCV_CSRRWI_MEMATTR55(a)  __builtin_linx_csrrwi(0b101111110111, a)
#define RISCV_CSRRWI_MEMATTR56(a)  __builtin_linx_csrrwi(0b101111111000, a)
#define RISCV_CSRRWI_MEMATTR57(a)  __builtin_linx_csrrwi(0b101111111001, a)
#define RISCV_CSRRWI_MEMATTR58(a)  __builtin_linx_csrrwi(0b101111111010, a)
#define RISCV_CSRRWI_MEMATTR59(a)  __builtin_linx_csrrwi(0b101111111011, a)
#define RISCV_CSRRWI_MEMATTR60(a)  __builtin_linx_csrrwi(0b101111111100, a)
#define RISCV_CSRRWI_MEMATTR61(a)  __builtin_linx_csrrwi(0b101111111101, a)
#define RISCV_CSRRWI_MEMATTR62(a)  __builtin_linx_csrrwi(0b101111111110, a)
#define RISCV_CSRRWI_MEMATTR63(a)  __builtin_linx_csrrwi(0b101111111111, a)
#define RISCV_CSRRWI_PORPC(a)  __builtin_linx_csrrwi(0b111111000000, a)
#define RISCV_CSRRWI_WTOPC(a)  __builtin_linx_csrrwi(0b111111000001, a)
#define RISCV_CSRRWI_CXCPTSC(a)  __builtin_linx_csrrwi(0b111111000010, a)
#define RISCV_CSRRWI_DBGUOPCNT(a)  __builtin_linx_csrrwi(0b011111111110, a)
#define RISCV_CSRRWI_FFLAGS(a)  __builtin_linx_csrrwi(0b000000000001, a)
#define RISCV_CSRRWI_FRM(a)  __builtin_linx_csrrwi(0b000000000010, a)
#define RISCV_CSRRWI_FCSR(a)  __builtin_linx_csrrwi(0b000000000011, a)
#define RISCV_CSRRWI_HPMCOUNTER3(a)  __builtin_linx_csrrwi(0b110000000011, a)
#define RISCV_CSRRWI_HPMCOUNTER4(a)  __builtin_linx_csrrwi(0b110000000100, a)
#define RISCV_CSRRWI_HPMCOUNTER5(a)  __builtin_linx_csrrwi(0b110000000101, a)
#define RISCV_CSRRWI_HPMCOUNTER6(a)  __builtin_linx_csrrwi(0b110000000110, a)
#define RISCV_CSRRWI_HPMCOUNTER7(a)  __builtin_linx_csrrwi(0b110000000111, a)
#define RISCV_CSRRWI_HPMCOUNTER8(a)  __builtin_linx_csrrwi(0b110000001000, a)
#define RISCV_CSRRWI_HPMCOUNTER9(a)  __builtin_linx_csrrwi(0b110000001001, a)
#define RISCV_CSRRWI_HPMCOUNTER10(a)  __builtin_linx_csrrwi(0b110000001010, a)
#define RISCV_CSRRWI_HPMCOUNTER11(a)  __builtin_linx_csrrwi(0b110000001011, a)
#define RISCV_CSRRWI_HPMCOUNTER12(a)  __builtin_linx_csrrwi(0b110000001100, a)
#define RISCV_CSRRWI_HPMCOUNTER13(a)  __builtin_linx_csrrwi(0b110000001101, a)
#define RISCV_CSRRWI_HPMCOUNTER14(a)  __builtin_linx_csrrwi(0b110000001110, a)
#define RISCV_CSRRWI_HPMCOUNTER15(a)  __builtin_linx_csrrwi(0b110000001111, a)
#define RISCV_CSRRWI_HPMCOUNTER16(a)  __builtin_linx_csrrwi(0b110000010000, a)
#define RISCV_CSRRWI_HPMCOUNTER17(a)  __builtin_linx_csrrwi(0b110000010001, a)
#define RISCV_CSRRWI_HPMCOUNTER18(a)  __builtin_linx_csrrwi(0b110000010010, a)
#define RISCV_CSRRWI_HPMCOUNTER19(a)  __builtin_linx_csrrwi(0b110000010011, a)
#define RISCV_CSRRWI_HPMCOUNTER20(a)  __builtin_linx_csrrwi(0b110000010100, a)
#define RISCV_CSRRWI_HPMCOUNTER21(a)  __builtin_linx_csrrwi(0b110000010101, a)
#define RISCV_CSRRWI_HPMCOUNTER22(a)  __builtin_linx_csrrwi(0b110000010110, a)
#define RISCV_CSRRWI_HPMCOUNTER23(a)  __builtin_linx_csrrwi(0b110000010111, a)
#define RISCV_CSRRWI_HPMCOUNTER24(a)  __builtin_linx_csrrwi(0b110000011000, a)
#define RISCV_CSRRWI_HPMCOUNTER25(a)  __builtin_linx_csrrwi(0b110000011001, a)
#define RISCV_CSRRWI_HPMCOUNTER26(a)  __builtin_linx_csrrwi(0b110000011010, a)
#define RISCV_CSRRWI_HPMCOUNTER27(a)  __builtin_linx_csrrwi(0b110000011011, a)
#define RISCV_CSRRWI_HPMCOUNTER28(a)  __builtin_linx_csrrwi(0b110000011100, a)
#define RISCV_CSRRWI_HPMCOUNTER29(a)  __builtin_linx_csrrwi(0b110000011101, a)
#define RISCV_CSRRWI_HPMCOUNTER30(a)  __builtin_linx_csrrwi(0b110000011110, a)
#define RISCV_CSRRWI_HPMCOUNTER31(a)  __builtin_linx_csrrwi(0b110000011111, a)
#define RISCV_CSRRWI_CYCLE(a)  __builtin_linx_csrrwi(0b110000000000, a)
#define RISCV_CSRRWI_TIME(a)  __builtin_linx_csrrwi(0b110000000001, a)
#define RISCV_CSRRWI_INSTRET(a)  __builtin_linx_csrrwi(0b110000000010, a)
#define RISCV_CSRRWI_HPMCOUNTER3H(a)  __builtin_linx_csrrwi(0b110010000011, a)
#define RISCV_CSRRWI_HPMCOUNTER4H(a)  __builtin_linx_csrrwi(0b110010000100, a)
#define RISCV_CSRRWI_HPMCOUNTER5H(a)  __builtin_linx_csrrwi(0b110010000101, a)
#define RISCV_CSRRWI_HPMCOUNTER6H(a)  __builtin_linx_csrrwi(0b110010000110, a)
#define RISCV_CSRRWI_HPMCOUNTER7H(a)  __builtin_linx_csrrwi(0b110010000111, a)
#define RISCV_CSRRWI_HPMCOUNTER8H(a)  __builtin_linx_csrrwi(0b110010001000, a)
#define RISCV_CSRRWI_HPMCOUNTER9H(a)  __builtin_linx_csrrwi(0b110010001001, a)
#define RISCV_CSRRWI_HPMCOUNTER10H(a)  __builtin_linx_csrrwi(0b110010001010, a)
#define RISCV_CSRRWI_HPMCOUNTER11H(a)  __builtin_linx_csrrwi(0b110010001011, a)
#define RISCV_CSRRWI_HPMCOUNTER12H(a)  __builtin_linx_csrrwi(0b110010001100, a)
#define RISCV_CSRRWI_HPMCOUNTER13H(a)  __builtin_linx_csrrwi(0b110010001101, a)
#define RISCV_CSRRWI_HPMCOUNTER14H(a)  __builtin_linx_csrrwi(0b110010001110, a)
#define RISCV_CSRRWI_HPMCOUNTER15H(a)  __builtin_linx_csrrwi(0b110010001111, a)
#define RISCV_CSRRWI_HPMCOUNTER16H(a)  __builtin_linx_csrrwi(0b110010010000, a)
#define RISCV_CSRRWI_HPMCOUNTER17H(a)  __builtin_linx_csrrwi(0b110010010001, a)
#define RISCV_CSRRWI_HPMCOUNTER18H(a)  __builtin_linx_csrrwi(0b110010010010, a)
#define RISCV_CSRRWI_HPMCOUNTER19H(a)  __builtin_linx_csrrwi(0b110010010011, a)
#define RISCV_CSRRWI_HPMCOUNTER20H(a)  __builtin_linx_csrrwi(0b110010010100, a)
#define RISCV_CSRRWI_HPMCOUNTER21H(a)  __builtin_linx_csrrwi(0b110010010101, a)
#define RISCV_CSRRWI_HPMCOUNTER22H(a)  __builtin_linx_csrrwi(0b110010010110, a)
#define RISCV_CSRRWI_HPMCOUNTER23H(a)  __builtin_linx_csrrwi(0b110010010111, a)
#define RISCV_CSRRWI_HPMCOUNTER24H(a)  __builtin_linx_csrrwi(0b110010011000, a)
#define RISCV_CSRRWI_HPMCOUNTER25H(a)  __builtin_linx_csrrwi(0b110010011001, a)
#define RISCV_CSRRWI_HPMCOUNTER26H(a)  __builtin_linx_csrrwi(0b110010011010, a)
#define RISCV_CSRRWI_HPMCOUNTER27H(a)  __builtin_linx_csrrwi(0b110010011011, a)
#define RISCV_CSRRWI_HPMCOUNTER28H(a)  __builtin_linx_csrrwi(0b110010011100, a)
#define RISCV_CSRRWI_HPMCOUNTER29H(a)  __builtin_linx_csrrwi(0b110010011101, a)
#define RISCV_CSRRWI_HPMCOUNTER30H(a)  __builtin_linx_csrrwi(0b110010011110, a)
#define RISCV_CSRRWI_HPMCOUNTER31H(a)  __builtin_linx_csrrwi(0b110010011111, a)
#define RISCV_CSRRWI_CYCLEH(a)  __builtin_linx_csrrwi(0b110010000000, a)
#define RISCV_CSRRWI_TIMEH(a)  __builtin_linx_csrrwi(0b110010000001, a)
#define RISCV_CSRRWI_INSTRETH(a)  __builtin_linx_csrrwi(0b110010000010, a)
#define RISCV_CSRRWI_TMESCR(a)  __builtin_linx_csrrwi(0b011111100000, a)
#define RISCV_CSRRWI_TMESVEC(a)  __builtin_linx_csrrwi(0b011111100001, a)
#define RISCV_CSRRWI_TMESTOP(a)  __builtin_linx_csrrwi(0b011111100010, a)
#define RISCV_CSRRWI_TMEDELEG(a)  __builtin_linx_csrrwi(0b011111100011, a)
#define RISCV_CSRRWI_TMTVEC(a)  __builtin_linx_csrrwi(0b011111100100, a)
#define RISCV_CSRRWI_TMTVT(a)  __builtin_linx_csrrwi(0b011111100101, a)
#define RISCV_CSRRWI_TMINTTHRESH(a)  __builtin_linx_csrrwi(0b011111100110, a)
#define RISCV_CSRRWI_TMSTATUS(a)  __builtin_linx_csrrwi(0b011111100111, a)
#define RISCV_CSRRWI_TMEPC(a)  __builtin_linx_csrrwi(0b011111101000, a)
#define RISCV_CSRRWI_TMCAUSE(a)  __builtin_linx_csrrwi(0b011111101001, a)
#define RISCV_CSRRWI_TCXCPTSC(a)  __builtin_linx_csrrwi(0b111111000011, a)
#define RISCV_CSRRWI_TMTVAL(a)  __builtin_linx_csrrwi(0b011111101011, a)
#define RISCV_CSRRWI_TMSCRATCH(a)  __builtin_linx_csrrwi(0b011111101100, a)
#define RISCV_CSRRWI_TMESEPR(a)  __builtin_linx_csrrwi(0b011111101101, a)
#define RISCV_CSRRWI_TMESEPS(a)  __builtin_linx_csrrwi(0b011111101110, a)
#define RISCV_CSRRWI_MSECCTL(a)  __builtin_linx_csrrwi(0b011111101111, a)
#define RISCV_CSRRWI_PMPTE0(a)  __builtin_linx_csrrwi(0b011111110000, a)
#define RISCV_CSRRWI_PMPTE1(a)  __builtin_linx_csrrwi(0b011111110001, a)
#define RISCV_CSRRWI_MSPLIMIT(a)  __builtin_linx_csrrwi(0b011111110010, a)
#define RISCV_CSRRWI_TMSPLIMIT(a)  __builtin_linx_csrrwi(0b011111110011, a)
#define RISCV_CSRRWI_TUSP(a)  __builtin_linx_csrrwi(0b100000000000, a)
#define RISCV_CSRRWI_TUGP(a)  __builtin_linx_csrrwi(0b100000000001, a)
#define RISCV_CSRRWI_TUTP(a)  __builtin_linx_csrrwi(0b100000000010, a)
#define RISCV_CSRRWI_TSP(a)  __builtin_linx_csrrwi(0b100000000011, a)
#define RISCV_CSRRWI_TGP(a)  __builtin_linx_csrrwi(0b100000000100, a)
#define RISCV_CSRRWI_TTP(a)  __builtin_linx_csrrwi(0b100000000101, a)
#define RISCV_CSRRWI_TESEPR(a)  __builtin_linx_csrrwi(0b110011000000, a)
#define RISCV_CSRRWI_TESEPS(a)  __builtin_linx_csrrwi(0b110011000001, a)
#define RISCV_CSRRWI_VXSAT(a)  __builtin_linx_csrrwi(0b000000001001, a)
#define RISCV_CSRRWI_VXRM(a)  __builtin_linx_csrrwi(0b000000001010, a)
#define RISCV_CSRRWI_VCSR(a)  __builtin_linx_csrrwi(0b000000001111, a)
#define RISCV_CSRRWI_VSTART(a)  __builtin_linx_csrrwi(0b000000001000, a)
#define RISCV_CSRRWI_VL(a)  __builtin_linx_csrrwi(0b110000100000, a)
#define RISCV_CSRRWI_VTYPE(a)  __builtin_linx_csrrwi(0b110000100001, a)
#define RISCV_CSRRWI_VLENB(a)  __builtin_linx_csrrwi(0b110000100010, a)
#define RISCV_CSRRWI_JBC(a)  __builtin_linx_csrrwi(0b100111110000, a)
#define RISCV_CSRRWI_JPC_UPPER(a)  __builtin_linx_csrrwi(0b100111110001, a)
#define RISCV_CSRRWI_JPC_LOWER(a)  __builtin_linx_csrrwi(0b100111110010, a)
#define RISCV_CSRRWI_JBCFT(a)  __builtin_linx_csrrwi(0b100111110011, a)
#define RISCV_CSRRWI_JBC_UPPER(a)  __builtin_linx_csrrwi(0b100111110100, a)
#define RISCV_CSRRWI_JBC_SHADOW(a)  __builtin_linx_csrrwi(0b110011110000, a)
#define RISCV_CSRRWI_SEC_ENHANCE_CTRL(a)  __builtin_linx_csrrwi(0b100111111111, a)
#define RISCV_CSRRWI_TSELECT(a)  __builtin_linx_csrrwi(0b011110100000, a)
#define RISCV_CSRRWI_TDATA1(a)  __builtin_linx_csrrwi(0b011110100001, a)
#define RISCV_CSRRWI_TDATA2(a)  __builtin_linx_csrrwi(0b011110100010, a)
#define RISCV_CSRRWI_PMPCFG(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRWI_PMPCFG0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRWI_PMPCFG1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRWI_PMPCFG2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRWI_PMPCFG3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRWI_PMPCFG4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRWI_PMPCFG5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRWI_PMPCFG6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRWI_PMPCFG7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRWI_PMPCFG8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRWI_PMPCFG9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRWI_PMPCFG10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRWI_PMPCFG11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRWI_PMPCFG12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRWI_PMPCFG13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRWI_PMPCFG14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRWI_PMPCFG15(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CSRRWI_PMPADDR(idx, a) \
  ({ \
    int _stats = -1; \
    switch(idx) \
    {\
    case 0:\
      _stats = RISCV_CSRRWI_PMPADDR0(a);\
      break;\
    case 1:\
      _stats = RISCV_CSRRWI_PMPADDR1(a);\
      break;\
    case 2:\
      _stats = RISCV_CSRRWI_PMPADDR2(a);\
      break;\
    case 3:\
      _stats = RISCV_CSRRWI_PMPADDR3(a);\
      break;\
    case 4:\
      _stats = RISCV_CSRRWI_PMPADDR4(a);\
      break;\
    case 5:\
      _stats = RISCV_CSRRWI_PMPADDR5(a);\
      break;\
    case 6:\
      _stats = RISCV_CSRRWI_PMPADDR6(a);\
      break;\
    case 7:\
      _stats = RISCV_CSRRWI_PMPADDR7(a);\
      break;\
    case 8:\
      _stats = RISCV_CSRRWI_PMPADDR8(a);\
      break;\
    case 9:\
      _stats = RISCV_CSRRWI_PMPADDR9(a);\
      break;\
    case 10:\
      _stats = RISCV_CSRRWI_PMPADDR10(a);\
      break;\
    case 11:\
      _stats = RISCV_CSRRWI_PMPADDR11(a);\
      break;\
    case 12:\
      _stats = RISCV_CSRRWI_PMPADDR12(a);\
      break;\
    case 13:\
      _stats = RISCV_CSRRWI_PMPADDR13(a);\
      break;\
    case 14:\
      _stats = RISCV_CSRRWI_PMPADDR14(a);\
      break;\
    case 15:\
      _stats = RISCV_CSRRWI_PMPADDR15(a);\
      break;\
    case 16:\
      _stats = RISCV_CSRRWI_PMPADDR16(a);\
      break;\
    case 17:\
      _stats = RISCV_CSRRWI_PMPADDR17(a);\
      break;\
    case 18:\
      _stats = RISCV_CSRRWI_PMPADDR18(a);\
      break;\
    case 19:\
      _stats = RISCV_CSRRWI_PMPADDR19(a);\
      break;\
    case 20:\
      _stats = RISCV_CSRRWI_PMPADDR20(a);\
      break;\
    case 21:\
      _stats = RISCV_CSRRWI_PMPADDR21(a);\
      break;\
    case 22:\
      _stats = RISCV_CSRRWI_PMPADDR22(a);\
      break;\
    case 23:\
      _stats = RISCV_CSRRWI_PMPADDR23(a);\
      break;\
    case 24:\
      _stats = RISCV_CSRRWI_PMPADDR24(a);\
      break;\
    case 25:\
      _stats = RISCV_CSRRWI_PMPADDR25(a);\
      break;\
    case 26:\
      _stats = RISCV_CSRRWI_PMPADDR26(a);\
      break;\
    case 27:\
      _stats = RISCV_CSRRWI_PMPADDR27(a);\
      break;\
    case 28:\
      _stats = RISCV_CSRRWI_PMPADDR28(a);\
      break;\
    case 29:\
      _stats = RISCV_CSRRWI_PMPADDR29(a);\
      break;\
    case 30:\
      _stats = RISCV_CSRRWI_PMPADDR30(a);\
      break;\
    case 31:\
      _stats = RISCV_CSRRWI_PMPADDR31(a);\
      break;\
    case 32:\
      _stats = RISCV_CSRRWI_PMPADDR32(a);\
      break;\
    case 33:\
      _stats = RISCV_CSRRWI_PMPADDR33(a);\
      break;\
    case 34:\
      _stats = RISCV_CSRRWI_PMPADDR34(a);\
      break;\
    case 35:\
      _stats = RISCV_CSRRWI_PMPADDR35(a);\
      break;\
    case 36:\
      _stats = RISCV_CSRRWI_PMPADDR36(a);\
      break;\
    case 37:\
      _stats = RISCV_CSRRWI_PMPADDR37(a);\
      break;\
    case 38:\
      _stats = RISCV_CSRRWI_PMPADDR38(a);\
      break;\
    case 39:\
      _stats = RISCV_CSRRWI_PMPADDR39(a);\
      break;\
    case 40:\
      _stats = RISCV_CSRRWI_PMPADDR40(a);\
      break;\
    case 41:\
      _stats = RISCV_CSRRWI_PMPADDR41(a);\
      break;\
    case 42:\
      _stats = RISCV_CSRRWI_PMPADDR42(a);\
      break;\
    case 43:\
      _stats = RISCV_CSRRWI_PMPADDR43(a);\
      break;\
    case 44:\
      _stats = RISCV_CSRRWI_PMPADDR44(a);\
      break;\
    case 45:\
      _stats = RISCV_CSRRWI_PMPADDR45(a);\
      break;\
    case 46:\
      _stats = RISCV_CSRRWI_PMPADDR46(a);\
      break;\
    case 47:\
      _stats = RISCV_CSRRWI_PMPADDR47(a);\
      break;\
    case 48:\
      _stats = RISCV_CSRRWI_PMPADDR48(a);\
      break;\
    case 49:\
      _stats = RISCV_CSRRWI_PMPADDR49(a);\
      break;\
    case 50:\
      _stats = RISCV_CSRRWI_PMPADDR50(a);\
      break;\
    case 51:\
      _stats = RISCV_CSRRWI_PMPADDR51(a);\
      break;\
    case 52:\
      _stats = RISCV_CSRRWI_PMPADDR52(a);\
      break;\
    case 53:\
      _stats = RISCV_CSRRWI_PMPADDR53(a);\
      break;\
    case 54:\
      _stats = RISCV_CSRRWI_PMPADDR54(a);\
      break;\
    case 55:\
      _stats = RISCV_CSRRWI_PMPADDR55(a);\
      break;\
    case 56:\
      _stats = RISCV_CSRRWI_PMPADDR56(a);\
      break;\
    case 57:\
      _stats = RISCV_CSRRWI_PMPADDR57(a);\
      break;\
    case 58:\
      _stats = RISCV_CSRRWI_PMPADDR58(a);\
      break;\
    case 59:\
      _stats = RISCV_CSRRWI_PMPADDR59(a);\
      break;\
    case 60:\
      _stats = RISCV_CSRRWI_PMPADDR60(a);\
      break;\
    case 61:\
      _stats = RISCV_CSRRWI_PMPADDR61(a);\
      break;\
    case 62:\
      _stats = RISCV_CSRRWI_PMPADDR62(a);\
      break;\
    case 63:\
      _stats = RISCV_CSRRWI_PMPADDR63(a);\
      break;\
    }\
    _stats; \
  })
#define RISCV_CTZ  __builtin_linx_ctz
#define RISCV_DIV  __builtin_linx_div
#define RISCV_DIVU  __builtin_linx_divu
#define RISCV_EBSET  __builtin_linx_ebset
#define RISCV_EXTUI  __builtin_linx_extui
#define RISCV_FADD_D  __builtin_linx_fadd_d
#define RISCV_FADD_S  __builtin_linx_fadd_s
#define RISCV_FCLASS_D  __builtin_linx_fclass_d
#define RISCV_FCLASS_S  __builtin_linx_fclass_s
#define RISCV_FCVT_D_S  __builtin_linx_fcvt_d_s
#define RISCV_FCVT_D_W  __builtin_linx_fcvt_d_w
#define RISCV_FCVT_D_WU  __builtin_linx_fcvt_d_wu
#define RISCV_FCVT_S_D  __builtin_linx_fcvt_s_d
#define RISCV_FCVT_S_W  __builtin_linx_fcvt_s_w
#define RISCV_FCVT_S_WU  __builtin_linx_fcvt_s_wu
#define RISCV_FCVT_W_D  __builtin_linx_fcvt_w_d
#define RISCV_FCVT_W_S  __builtin_linx_fcvt_w_s
#define RISCV_FCVT_WU_D  __builtin_linx_fcvt_wu_d
#define RISCV_FCVT_WU_S  __builtin_linx_fcvt_wu_s
#define RISCV_FDIV_D  __builtin_linx_fdiv_d
#define RISCV_FDIV_S  __builtin_linx_fdiv_s
#define RISCV_FENCE  __builtin_linx_fence
#define RISCV_FENCE_I  __builtin_linx_fence_i
#define RISCV_FEQ_D  __builtin_linx_feq_d
#define RISCV_FEQ_S  __builtin_linx_feq_s
#define RISCV_FLD  __builtin_linx_fld
#define RISCV_FLD_PU  __builtin_linx_fld_pu
#define RISCV_FLD_U  __builtin_linx_fld_u
#define RISCV_FLE_D  __builtin_linx_fle_d
#define RISCV_FLE_S  __builtin_linx_fle_s
#define RISCV_FLT_D  __builtin_linx_flt_d
#define RISCV_FLT_S  __builtin_linx_flt_s
#define RISCV_FLW  __builtin_linx_flw
#define RISCV_FLW_PU  __builtin_linx_flw_pu
#define RISCV_FLW_U  __builtin_linx_flw_u
#define RISCV_FMADD_D  __builtin_linx_fmadd_d
#define RISCV_FMADD_S  __builtin_linx_fmadd_s
#define RISCV_FMAX_D  __builtin_linx_fmax_d
#define RISCV_FMAX_S  __builtin_linx_fmax_s
#define RISCV_FMIN_D  __builtin_linx_fmin_d
#define RISCV_FMIN_S  __builtin_linx_fmin_s
#define RISCV_FMOVEQZ_D  __builtin_linx_fmoveqz_d
#define RISCV_FMOVEQZ_S  __builtin_linx_fmoveqz_s
#define RISCV_FMOVNEZ_D  __builtin_linx_fmovnez_d
#define RISCV_FMOVNEZ_S  __builtin_linx_fmovnez_s
#define RISCV_FMSUB_D  __builtin_linx_fmsub_d
#define RISCV_FMSUB_S  __builtin_linx_fmsub_s
#define RISCV_FMUL_D  __builtin_linx_fmul_d
#define RISCV_FMUL_S  __builtin_linx_fmul_s
#define RISCV_FMV_W_X  __builtin_linx_fmv_w_x
#define RISCV_FMV_X_W  __builtin_linx_fmv_x_w
#define RISCV_FNMADD_D  __builtin_linx_fnmadd_d
#define RISCV_FNMADD_S  __builtin_linx_fnmadd_s
#define RISCV_FNMSUB_D  __builtin_linx_fnmsub_d
#define RISCV_FNMSUB_S  __builtin_linx_fnmsub_s
#define RISCV_FRINT_D  __builtin_linx_frint_d
#define RISCV_FRINT_D_RDN  __builtin_linx_frint_d_rdn
#define RISCV_FRINT_D_RMM  __builtin_linx_frint_d_rmm
#define RISCV_FRINT_D_RNE  __builtin_linx_frint_d_rne
#define RISCV_FRINT_D_RTZ  __builtin_linx_frint_d_rtz
#define RISCV_FRINT_D_RUP  __builtin_linx_frint_d_rup
#define RISCV_FRINT_S  __builtin_linx_frint_s
#define RISCV_FRINT_S_RDN  __builtin_linx_frint_s_rdn
#define RISCV_FRINT_S_RMM  __builtin_linx_frint_s_rmm
#define RISCV_FRINT_S_RNE  __builtin_linx_frint_s_rne
#define RISCV_FRINT_S_RTZ  __builtin_linx_frint_s_rtz
#define RISCV_FRINT_S_RUP  __builtin_linx_frint_s_rup
#define RISCV_FSD  __builtin_linx_fsd
#define RISCV_FSD_PU  __builtin_linx_fsd_pu
#define RISCV_FSD_U  __builtin_linx_fsd_u
#define RISCV_FSGNJ_D  __builtin_linx_fsgnj_d
#define RISCV_FSGNJ_S  __builtin_linx_fsgnj_s
#define RISCV_FSGNJN_D  __builtin_linx_fsgnjn_d
#define RISCV_FSGNJN_S  __builtin_linx_fsgnjn_s
#define RISCV_FSGNJX_D  __builtin_linx_fsgnjx_d
#define RISCV_FSGNJX_S  __builtin_linx_fsgnjx_s
#define RISCV_FSQRT_D  __builtin_linx_fsqrt_d
#define RISCV_FSQRT_S  __builtin_linx_fsqrt_s
#define RISCV_FSUB_D  __builtin_linx_fsub_d
#define RISCV_FSUB_S  __builtin_linx_fsub_s
#define RISCV_FSW  __builtin_linx_fsw
#define RISCV_FSW_PU  __builtin_linx_fsw_pu
#define RISCV_FSW_U  __builtin_linx_fsw_u
#define RISCV_JBCFETCH  __builtin_linx_jbcfetch
#define RISCV_LA_IMM_TO_GP  __builtin_linx_la_imm_to_gp
#define RISCV_LA_REG_TO_GP  __builtin_linx_la_reg_to_gp
#define RISCV_LB  __builtin_linx_lb
#define RISCV_LBU  __builtin_linx_lbu
#define RISCV_LBUGP  __builtin_linx_lbugp
#define RISCV_LH  __builtin_linx_lh
#define RISCV_LHU  __builtin_linx_lhu
#define RISCV_LHUGP  __builtin_linx_lhugp
#define RISCV_LUI  __builtin_linx_lui
#define RISCV_LW  __builtin_linx_lw
#define RISCV_LWGP  __builtin_linx_lwgp
#define RISCV_MADDR32  __builtin_linx_maddr32
#define RISCV_MAX  __builtin_linx_max
#define RISCV_MAXU  __builtin_linx_maxu
#define RISCV_MIN  __builtin_linx_min
#define RISCV_MINU  __builtin_linx_minu
#define RISCV_MOVEQZ  __builtin_linx_moveqz
#define RISCV_MOVNEZ  __builtin_linx_movnez
#define RISCV_MSUBR32  __builtin_linx_msubr32
#define RISCV_MUL  __builtin_linx_mul
#define RISCV_MULH  __builtin_linx_mulh
#define RISCV_MULHSU  __builtin_linx_mulhsu
#define RISCV_MULHU  __builtin_linx_mulhu
#define RISCV_MULIADD  __builtin_linx_muliadd
#define RISCV_OR  __builtin_linx_or
#define RISCV_ORC_B  __builtin_linx_orc_b
#define RISCV_ORI  __builtin_linx_ori
#define RISCV_ORN  __builtin_linx_orn
#define RISCV_ORSHF  __builtin_linx_orshf
#define RISCV_PACK  __builtin_linx_pack
#define RISCV_PACKH  __builtin_linx_packh
#define RISCV_PACKU  __builtin_linx_packu
#define RISCV_PCNT  __builtin_linx_pcnt
#define RISCV_PREFD  __builtin_linx_prefd
#define RISCV_PREFI  __builtin_linx_prefi
#define RISCV_REM  __builtin_linx_rem
#define RISCV_REMU  __builtin_linx_remu
#define RISCV_REV  __builtin_linx_rev
#define RISCV_REV8  __builtin_linx_rev8
#define RISCV_ROL  __builtin_linx_rol
#define RISCV_ROR  __builtin_linx_ror
#define RISCV_RORI  __builtin_linx_rori
#define RISCV_SB  __builtin_linx_sb
#define RISCV_SB_PU  __builtin_linx_sb_pu
#define RISCV_SB_U  __builtin_linx_sb_u
#define RISCV_SBCLR  __builtin_linx_sbclr
#define RISCV_SBCLRI  __builtin_linx_sbclri
#define RISCV_SBEXT  __builtin_linx_sbext
#define RISCV_SBEXTI  __builtin_linx_sbexti
#define RISCV_SBGP  __builtin_linx_sbgp
#define RISCV_SBINV  __builtin_linx_sbinv
#define RISCV_SBINVI  __builtin_linx_sbinvi
#define RISCV_SBSET  __builtin_linx_sbset
#define RISCV_SBSETI  __builtin_linx_sbseti
#define RISCV_SEXT_B  __builtin_linx_sext_b
#define RISCV_SEXT_H  __builtin_linx_sext_h
#define RISCV_SH  __builtin_linx_sh
#define RISCV_SH_PU  __builtin_linx_sh_pu
#define RISCV_SH_U  __builtin_linx_sh_u
#define RISCV_SHGP  __builtin_linx_shgp
#define RISCV_SLL  __builtin_linx_sll
#define RISCV_SLLI  __builtin_linx_slli
#define RISCV_SLO  __builtin_linx_slo
#define RISCV_SLOI  __builtin_linx_sloi
#define RISCV_SLT  __builtin_linx_slt
#define RISCV_SLTI  __builtin_linx_slti
#define RISCV_SLTIU  __builtin_linx_sltiu
#define RISCV_SLTU  __builtin_linx_sltu
#define RISCV_SRA  __builtin_linx_sra
#define RISCV_SRAI  __builtin_linx_srai
#define RISCV_SRL  __builtin_linx_srl
#define RISCV_SRLI  __builtin_linx_srli
#define RISCV_SRO  __builtin_linx_sro
#define RISCV_SROI  __builtin_linx_sroi
#define RISCV_STOP  __builtin_linx_stop
#define RISCV_SUB  __builtin_linx_sub
#define RISCV_SUBSHF  __builtin_linx_subshf
#define RISCV_SW  __builtin_linx_sw
#define RISCV_SW_PU  __builtin_linx_sw_pu
#define RISCV_SW_U  __builtin_linx_sw_u
#define RISCV_SWGP  __builtin_linx_swgp
#define RISCV_TRET  __builtin_linx_tret
#define RISCV_WFI  __builtin_linx_wfi
#define RISCV_XNOR  __builtin_linx_xnor
#define RISCV_XOR  __builtin_linx_xor
#define RISCV_XORI  __builtin_linx_xori
#define RISCV_XORSHF  __builtin_linx_xorshf
#define RISCV_GET_MEPC()  RISCV_CSRRS_MEPC(0)
#define RISCV_SET_MEPC(a)  RISCV_CSRRW_MEPC(a)
#define RISCV_GET_MSCRATCH()  RISCV_CSRRS_MSCRATCH(0)
#define RISCV_SET_MSCRATCH(a)  RISCV_CSRRW_MSCRATCH(a)
#define RISCV_GET_MTVAL()  RISCV_CSRRS_MTVAL(0)
#define RISCV_SET_MTVAL(a)  RISCV_CSRRW_MTVAL(a)
#define RISCV_GET_MIE()  RISCV_CSRRS_MIE(0)
#define RISCV_SET_MIE(a)  RISCV_CSRRW_MIE(a)
#define RISCV_GET_PMPCFG(idx)  RISCV_CSRRS_PMPCFG(idx, 0)
#define RISCV_SET_PMPCFG(idx, a)  RISCV_CSRRW_PMPCFG(idx, a)
#define RISCV_GET_PMPADDR(idx)  RISCV_CSRRS_PMPADDR(idx, 0)
#define RISCV_SET_PMPADDR(idx, a)  RISCV_CSRRW_PMPADDR(idx, a)
#define RISCV_GET_PMPTE(idx)  RISCV_CSRRS_PMPTE(idx, 0)
#define RISCV_SET_PMPTE(idx, a)  RISCV_CSRRW_PMPTE(idx, a)
