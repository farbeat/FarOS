# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

CROSS_COMPILE := xt-

ifeq ($(LOSCFG_ARCH_XTENSA_XEA3), y)
VERSION_NUM   := RH-2018.7-linux
endif
ifeq ($(LOSCFG_ARCH_XTENSA_XEA2), y)
VERSION_NUM   := RG-2017.5-linux
endif

CC      = $(CROSS_COMPILE)xcc
GPP     = $(CROSS_COMPILE)xc++
AS      = $(CROSS_COMPILE)as
AR      = $(CROSS_COMPILE)ar
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF  = $(CROSS_COMPILE)readelf
SIZE    = $(CROSS_COMPILE)size
NM      = $(CROSS_COMPILE)nm

# xtensa use compiler libc impl
LITEOS_BASELIB += -lc -lgloss -lhal

LITEOS_COMPILER_PATH := $(shell $(LITEOSTOPDIR)/build/scripts/get_compiler_path.sh $(CC))

XTENSA_XTDEV_TOOLS_PATH = $(LITEOS_COMPILER_PATH)/../../../..
XTENSA_ELF_DIR  = $(XTENSA_XTDEV_TOOLS_PATH)/install/builds/$(VERSION_NUM)/$(XTENSA_CORE)/xtensa-elf
XTENSA_TOOL_DIR = $(XTENSA_XTDEV_TOOLS_PATH)/install/tools/$(VERSION_NUM)/XtensaTools
XTENSA_ARCH_DIR = $(XTENSA_ELF_DIR)/arch
LITEOS_COMPILER_GCC_INCLUDE = -I $(XTENSA_TOOL_DIR)/xtensa-elf/include \
                              -I $(XTENSA_TOOL_DIR)/lib/xcc/include \
                              -I $(XTENSA_ELF_DIR)/include \
                              -I $(XTENSA_ARCH_DIR)/include
