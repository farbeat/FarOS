# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.

ifeq ($(LOSCFG_COMPILER_LLVM_BINUTILS), y)
    CROSS_COMPILE := llvm-
    CROSS_LINKER := ld.lld
else
    ifeq ($(LOSCFG_COMPILER_TOOLCHAIN_MUSL), y)
        CROSS_COMPILE := riscv32-linux-musl-
    else ifeq ($(LOSCFG_COMPILER_TOOLCHAIN_UNKNOWN), y)
        CROSS_COMPILE := riscv32-unknown-elf-
    else ifeq ($(LOSCFG_COMPILER_LINGLONG), y)
        CROSS_COMPILE := linglong-elf-
    endif
    CROSS_LINKER := $(CROSS_COMPILE)ld
endif
VERSION_NUM    = $(shell $(CC) -dumpversion)

CC      = clang
GPP     = clang++
AS      = $(CROSS_COMPILE)as
AR      = $(CROSS_COMPILE)ar
LD      = $(CROSS_LINKER)
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf
SIZE    = $(CROSS_COMPILE)size
NM      = $(CROSS_COMPILE)nm

LITEOS_COMPILER_PATH := $(shell $(LITEOSTOPDIR)/build/scripts/get_compiler_path.sh $(CC))

# Note that the path to the library may vary depending on the compiler version.
# The latest compiler version is 1.2.0.B038, the path to the library is as follows:
ifeq ($(LOSCFG_COMPILER_LINGLONG), y)
    LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/linglong-elf/lib
    LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/linglong-elf/lib
    CLANG_CXXINCLUDE = $(LITEOS_COMPILER_PATH)/linglong-elf/include
else ifeq ($(LOSCFG_ARCH_ARM_AARCH32), y)
    LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/newlib/arm-none-eabi/lib
    LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/newlib/arm-none-eabi/lib
    CLANG_CXXINCLUDE = $(LITEOS_COMPILER_PATH)/newlib/arm-none-eabi/include
    ifeq ($(LOSCFG_COMPILER_LLVM_BINUTILS), y)
        # Set target emulation
        LITEOS_LD_OPTS += -marmelf
    endif
else
    ifeq ($(LOSCFG_ARCH_FPU_ENABLE), y)
        ifeq ($(LOSCFG_ARCH_FPU_DOUBLE), y)
            ifeq ($(LOSCFG_LCMP_CUSTOM_INST16_EXTENSIONS)$(LOSCFG_LCMP_CUSTOM_INST16_SUB_EXTENSIONS)$(LOSCFG_LCMP_CUSTOM_INST32_EXTENSIONS), yyy)
                LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/riscv32-elf/lib/rv32imafdcbxlinxma_xlinxmb_xlinxmc_xlinxmd_ilp32d
                LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/lib/rv32imafdcbxlinxma_xlinxmb_xlinxmc_xlinxmd_ilp32d
            else
                LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/riscv32-elf/lib/rv32imfdc_ilp32d
                LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/lib/rv32imfdc_ilp32d
            endif
        else
            ifeq ($(LOSCFG_LCMP_CUSTOM_INST16_EXTENSIONS)$(LOSCFG_LCMP_CUSTOM_INST16_SUB_EXTENSIONS)$(LOSCFG_LCMP_CUSTOM_INST32_EXTENSIONS), yyy)
                LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/riscv32-elf/lib/rv32imafcbxlinxma_xlinxmb_xlinxmc_xlinxmd_ilp32f
                LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/lib/rv32imafcbxlinxma_xlinxmb_xlinxmc_xlinxmd_ilp32f
            else
                LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/riscv32-elf/lib/rv32imfc_ilp32f
                LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/lib/rv32imfc_ilp32f
            endif
        endif
    else
        LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/riscv32-elf/lib/rv32imc_ilp32
        LITEOS_COMPILER_GCCLIB_PATH = $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/lib/rv32imc_ilp32
    endif
    CLANG_CXXINCLUDE = $(LITEOS_COMPILER_PATH)/riscv32-elf/include
endif

LITEOS_COMPILER_GCC_INCLUDE = -I $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/include

LITEOS_CXXINCLUDE += \
    -I $(CLANG_CXXINCLUDE)\
    -I $(LITEOS_COMPILER_PATH)/lib/clang/$(VERSION_NUM)/include

$(call path_checker,$(LITEOS_COMPILER_GCCLIB_PATH))
