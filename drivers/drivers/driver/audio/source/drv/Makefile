LOCAL_TARGET := audio_drv
#=========================== variables ==================================
SAP_TOP_DIR ?= $(realpath ../)

include $(SAP_TOP_DIR)/sap_base.mak

ifeq ($(PARAM_FILE),)
PARAM_FILE := $(SAP_TOP_DIR)/build/Makefile.param
include $(PARAM_FILE)
endif

SAP_DRV_DIR := $(SAP_TOP_DIR)/drv
OBJOUT := $(OBJS_DIR)$(dir $(subst $(realpath $(SAP_SDK_DIR)),,$(PWD)))$(LOCAL_TARGET)
#======================== common CFLAGS =========================================
LOCAL_CFLAGS := $(SAP_FEATURE_CONFIGS)
LOCAL_CFLAGS += \
	-I$(SAP_CORE_DIR)/adp \
	-I$(SAP_CORE_DIR)/aie/ai \
	-I$(SAP_CORE_DIR)/hal/include \
	-I$(SAP_CORE_DIR)/osal/$(CFG_SAP_CPU_OS) \
	-I$(SAP_CORE_DIR)/arch/$(CFG_SAP_CHIP_TYPE)/$(CFG_SAP_CHIP_REVERSION)/include \
	-I$(SAP_DRV_DIR)/include \
	-I$(SAP_DRV_DIR)/common/include \
	-I$(SAP_DRV_DIR)/sys/include \
	-I$(SAP_DRV_DIR)/adp/include \
	-I$(SAP_DRV_DIR)/aenc/include \
	-I$(SAP_DRV_DIR)/aie/include \
	-I$(SAP_DRV_DIR)/aoe/include \
	-I$(SAP_DRV_DIR)/adec/include \
	-I$(SAP_DRV_DIR)/ab/include \
	-I$(SAP_DRV_DIR)/arch/include \
	-I$(SAP_DRV_DIR)/arch/$(CFG_SAP_CHIP_TYPE)/$(CFG_SAP_CHIP_REVERSION)/include

LOCAL_CFLAGS += \
	-I$(SAP_CORE_DIR)/overlay \
	-I$(SAP_CORE_DIR)/common \
	-I$(SAP_CORE_DIR)/common/sys_ctrl

LOCAL_CFLAGS += -I$(SAP_DRV_DIR)/arch/$(CFG_SAP_CHIP_TYPE)/tianlai
LOCAL_CFLAGS += -I$(SAP_SDK_DIR)/utils/lowpower/lowpower/include
LOCAL_CFLAGS += -I$(SAP_SDK_DIR)/include/utils/lowpower

LOCAL_CFLAGS += -I$(SAP_SDK_DIR)/include/native/osa

LOCAL_CFLAGS += -DSAP_OS_TYPE=os_type_$(CFG_SAP_CPU_OS)
LOCAL_CFLAGS += -DBOARD_$(BOARD_VERSION)
#======================== osal SRCS =========================================
LOCAL_SRCS :=
ifneq ($(findstring $(CFG_SAP_CPU_OS), "linux"),)
LOCAL_SRCS += \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_addr.c \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_debug.c \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_device.c \
	../core/osal/$(CFG_SAP_CPU_OS)/drv_audio_osal_proc.c

LOCAL_SRCS += \
	../../soc/$(CFG_SAP_CHIP_TYPE)/mmz/audio_osal_mmz.c
endif

ifneq ($(findstring $(CFG_SAP_CPU_OS), "liteos"),)
LOCAL_SRCS += \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_addr.c \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_debug.c \
	../core/osal/$(CFG_SAP_CPU_OS)/audio_osal_timer.c \
	../core/osal/$(CFG_SAP_CPU_OS)/drv_audio_osal_proc.c
endif
#======================== application SRCS ====================================
ifeq ($(PLATFORM), apps)
LOCAL_CFLAGS += -DSOC_PLF_TYPE_APPS
LOCAL_CFLAGS += -DSOC_SUPPORT_DSP_MSG_TO_APPS
#===================================================================================
LOCAL_SRCS += \
	../core/adp/core_adp.c \
	../core/aie/ai/core_ai_lib.c \
	../core/hal/afe/dallas_v100/hal_afe_sys.c \
	../core/common/core_util.c \
	../core/common/core_module.c \
	../core/common/share_mem/core_msg.c \
	../core/common/share_mem/core_shrb.c \
	../core/common/share_mem/core_rbuf.c \
	../core/common/sys_ctrl/sys_cmd.c
#===================================================================================
ifeq ($(CFG_SAP_AFE_AVAD), y)
LOCAL_CFLAGS += -DSAP_AFE_AVAD
LOCAL_SRCS += \
	../core/aie/ai/dev_afe_avad.c \
	../core/hal/afe/remix_v200/hal_afe_avad.c
endif
#===================================================================================
LOCAL_SRCS += \
	arch/drv_sap_msg.c \
	arch/drv_sap_proc.c \
	arch/drv_sap_intf.c \
	arch/$(CFG_SAP_CHIP_TYPE)/$(CFG_SAP_CHIP_REVERSION)/audio_crg_func.c \
	arch/$(CFG_SAP_CHIP_TYPE)/$(CFG_SAP_CHIP_REVERSION)/drv_hal.c
#===================================================================================
LOCAL_SRCS += \
	sys/drv_sys.c \
	sys/drv_msg_sys.c \
	sys/drv_proc_sys.c
#===================================================================================
LOCAL_SRCS += \
	adp/drv_adp.c \
	adp/drv_msg_adp.c
#===================================================================================
LOCAL_SRCS += \
	aie/drv_vad.c \
	aie/drv_ai.c \
	aie/drv_sea.c \
	aie/drv_msg_ai.c \
	aie/drv_msg_sea.c \
	aie/drv_proc_ai.c \
	aie/drv_proc_sea.c
#===================================================================================
LOCAL_SRCS += \
	aoe/drv_ao.c \
	aoe/drv_msg_ao.c \
	aoe/drv_track.c \
	aoe/drv_msg_track.c \
	aoe/drv_proc_ao.c
#===================================================================================
LOCAL_SRCS += \
	aenc/drv_aenc.c \
	aenc/drv_msg_aenc.c \
	aenc/drv_proc_aenc.c
#===================================================================================
LOCAL_SRCS += \
	adec/drv_adec.c \
	adec/drv_msg_adec.c \
	adec/drv_proc_adec.c
#===================================================================================
LOCAL_SRCS += \
	anc/drv_anc.c \
	anc/drv_msg_anc.c
#===================================================================================
ifeq ($(CFG_SAP_HAID_SUPPORT), y)
LOCAL_CFLAGS += -I$(SAP_DRV_DIR)/haid
LOCAL_SRCS += \
	haid/drv_haid.c \
	haid/drv_msg_haid.c \
	haid/drv_proc_haid.c
endif
#===================================================================================
ifeq ($(CFG_SAP_PROC_SUPPORT), y)
LOCAL_SRCS += \
	ab/drv_ab.c \
	ab/drv_msg_ab.c \
	ab/drv_proc_ab.c
endif
#===================================================================================
ifeq ($(CFG_SAP_TIME_CONSUME), y)
LOCAL_SRCS += ../core/common/time_consume.c
endif
#===================================================================================
LOCAL_SRCS += \
	common/drv_overlay_load.c \
	common/drv_overlay_load_from_flash.c \
	common/proc/drv_audio_proc.c
endif # PLATFORM is apps
#======================== protocol SRCS ==================================
ifeq ($(PLATFORM), prot)
LOCAL_CFLAGS += -DSOC_PLF_TYPE_PROT

LOCAL_SRCS += \
	../core/common/share_mem/core_shrb.c \
	../core/common/share_mem/core_rbuf.c \
	../core/common/sys_ctrl/sys_cmd.c \
	../core/common/sys_ctrl/work_on.c \
	../core/adp/core_adp.c \
	../core/common/core_util.c \
	../core/common/core_module.c \
	../core/common/share_mem/core_msg.c
#===================================================================================
LOCAL_SRCS += \
	adp/drv_adp.c \
	adp/drv_msg_adp.c
#===================================================================================
ifeq ($(CFG_SAP_TIME_CONSUME), y)
LOCAL_SRCS += ../core/common/time_consume.c
endif
endif # PLATFORM is prot
#===================== os diff ===========================================
include $(SAP_DRV_DIR)/$(CFG_SAP_CPU_OS).mak
