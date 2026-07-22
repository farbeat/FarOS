ifeq ($(LOSCFG_LASLR), y)
ASLR_REL_FILE := $(OUT)/aslr/rel/aslr_gspr_info.c
ASLR_REL_OBJECT := $(OUT)/aslr/rel/aslr_gspr_info.o
ASLR_REL_INCLUDE_PATH := -I $(LITEOSTOPDIR)/kernel/extended/dynload/include \
	-I $(LITEOSTOPDIR)/kernel/include
ASLR_TYPE :=
LITEOS_ASLRLDFLAGS :=

ifeq ($(LOSCFG_LASLR_GOT), y)
	ASLR_TYPE := "GOT"
else ifeq ($(LOSCFG_LASLR_GSPR), y)
	ifeq ($(ASLR_REL_FILE), $(wildcard $(ASLR_REL_FILE)))
		ASLR_TYPE := "GSPR_check"
		LITEOS_ASLRLDFLAGS = $(ASLR_REL_OBJECT)
	else
		ASLR_TYPE := "GSPR"
	endif
endif

endif