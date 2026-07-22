include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(shell pwd))

LOCAL_SRCS :=  $(wildcard src/*.c)

LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/fs/jffs2/include \
	-I $(LITEOSTOPDIR)/fs/yaffs2/yaffs/include \
	-I $(LITEOSTOPDIR)/fs/yaffs2/yaffs/direct/include \
        -I $(LITEOSTOPDIR)/drivers/mtd/common/include

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
