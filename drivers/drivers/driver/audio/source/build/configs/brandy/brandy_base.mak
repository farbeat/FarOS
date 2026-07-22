#===================================================================================
SAP_SDK_DIR := $(realpath $(SAP_TOP_DIR)/../../../../..)
#===================================================================================
AUDIO_ROOT_DIR := ${SAP_SDK_DIR}/drivers/drivers/driver/audio/source
INTERIM_BINARY_PATH := ${SAP_SDK_DIR}/interim_binary/$(CFG_SAP_CHIP_TYPE)
LIBRARY_OUTPUT_PATH := ${INTERIM_BINARY_PATH}/libs/audio/${chip}_${core}/${target_application}
#===================================================================================
OSAL_DIR := $(SAP_SDK_DIR)/kernel/osal
SECUREC_DIR := $(SAP_SDK_DIR)/open_source/libboundscheck
SECUREC_INCLUDE := ${SECUREC_DIR}/include
SAP_FIRMWARE_DIR  := $(INTERIM_BINARY_PATH)/audio/$(CFG_SAP_CHIP_TYPE)_$(CFG_SAP_BOARD_TYPE)
SAP_UAPI_DIR := ${SAP_SDK_DIR}/drivers/drivers/driver/audio/include
SAP_OUT_DIR  := $(SAP_SDK_DIR)/output/$(CFG_SAP_CHIP_TYPE)/audio/$(CFG_SAP_CHIP_TYPE)_$(CFG_SAP_BOARD_TYPE)
SAP_OPEN_SOURCE_DIR := $(SAP_SDK_DIR)/open_source
SAP_XT_TOOLCHAINS_DIR := $(SAP_SDK_DIR)/tools/bin/compiler/xtensa/RG-2017.5-linux
#===================================================================================
SAP_FEATURE_CONFIGS := \
	-I$(OSAL_DIR)/include \
	-I$(SECUREC_INCLUDE) \
	-I$(SAP_UAPI_DIR)
#===================================================================================
