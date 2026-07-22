LOCAL_TARGET := audio_api

#================================== Variables ==================================
ifeq ($(SAP_TOP_DIR),)
CUR_DIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
SAP_TOP_DIR := $(realpath $(CUR_DIR)/..)
endif

include $(SAP_TOP_DIR)/sap_base.mak

ifeq ($(PARAM_FILE),)
PARAM_FILE := $(SAP_TOP_DIR)/build/Makefile.param
include $(PARAM_FILE)
endif

ADP_HOST_FLAG ?= 1
OBJOUT := $(OBJS_DIR)$(dir $(subst $(realpath $(SAP_SDK_DIR)),,$(shell pwd)))$(LOCAL_TARGET)
LIB := $(LIBS_DIR)/lib$(LOCAL_TARGET).a
#================================== API FLAGS ==================================
LOCAL_CFLAGS := $(SAP_FEATURE_CONFIGS)
LOCAL_CFLAGS += \
	-I$(SAP_UAPI_DIR) \
	-I$(SAP_MPI_DIR)/include \
	-I$(SAP_DRV_DIR)/include \
	-I$(SAP_DRV_DIR)/common/include \
	-I$(SAP_DRV_DIR)/arch/include \
	-I$(SAP_DRV_DIR)/arch/$(CFG_SAP_CHIP_TYPE)/include \
	-I$(SAP_DRV_DIR)/osal/kernel \
	-I$(SAP_CORE_DIR)/include \
	-I$(SAP_CORE_DIR)/common/include \
	-I$(SAP_CORE_DIR)/common/securec/include \
	-I$(SAP_CORE_DIR)/osal/$(CFG_SAP_CPU_OS) \
	-I$(SAP_CORE_DIR)/adp

LOCAL_CFLAGS += -I$(SAP_CORE_DIR)/arch/$(CFG_SAP_CHIP_TYPE)/$(CFG_SAP_CHIP_REVERSION)/include

LOCAL_CFLAGS += \
    -DSAP_OS_TYPE=os_type_$(CFG_SAP_CPU_OS) \
    -DBOARD_$(BOARD_VERSION)
#================================== API SRCS ===================================
LOCAL_SRCS :=

ifeq ($(PLATFORM), apps)
ADP_HOST_FLAG = 1
LOCAL_SRCS := \
	$(wildcard sys/*.c) \
	$(wildcard adp/*.c) \
	$(wildcard adec/*.c) \
	$(wildcard aenc/*.c) \
	$(wildcard aie/*.c) \
	$(wildcard aoe/*.c)

LOCAL_SRCS += \
	anc/mpi_anc.c \
	anc/uapi_anc.c
LOCAL_CFLAGS += -DSOC_PLF_TYPE_APPS
endif

ifeq ($(PLATFORM), prot)
ADP_HOST_FLAG = 0
LOCAL_SRCS += $(wildcard adp/*.c)
LOCAL_CFLAGS += -DSOC_PLF_TYPE_PROT
endif

LOCAL_CFLAGS += -DADP_HOST_FLAG=$(ADP_HOST_FLAG)
#================================== HAID ===================================
ifeq ($(CFG_SAP_HAID_SUPPORT), y)
LOCAL_SRCS += \
	haid/mpi_haid.c \
	haid/uapi_haid.c
endif
#================================== API Diff ===================================
ifeq ($(CFG_SAP_CPU_OS), linux)
LOCAL_CFLAGS += \
	-I$(SAP_CORE_DIR)/osal/include

LOCAL_SRCS += \
	$(SAP_CORE_DIR)/adp/core_adp.c \
	$(SAP_CORE_DIR)/common/core_module.c \
	$(SAP_CORE_DIR)/common/core_util.c \
	$(SAP_CORE_DIR)/common/share_mem/core_shrb.c \
	$(SAP_CORE_DIR)/common/share_mem/core_rbuf.c
endif
#===============================================================================

CFLAGS += $(LOCAL_CFLAGS)
OBJS := $(patsubst %.c, $(OBJOUT)/%.o, $(LOCAL_SRCS))
#===============================================================================
.PHONY: all clean
#===============================================================================
all: $(LIB)
clean: $(LIB)_clean
#===============================================================================
$(OBJOUT)/%.o : %.c
	$(AT)$(OBJ_MKDIR)
	$(AT)$(CC) $(CFLAGS) -c $< -o $@
#===============================================================================
.PHONY: $(LIB) $(LIB)_clean
#===============================================================================
$(LIB) : $(OBJS)
	$(AT)$(OBJ_MKDIR)
	$(AT)$(AR) $(ARFLAGS) $@ $(OBJS)

$(LIB)_clean:
	$(AT)rm -rf $(LIB) $(OBJOUT)
#===============================================================================
