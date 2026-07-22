LITEOSTOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
export OS=$(shell uname -s)
ifneq ($(OS), Linux)
LITEOSTOPDIR := $(shell dirname $(subst \,/,$(LITEOSTOPDIR))/./)
endif

export LITEOSTOPDIR

##### check kconfiglib is install or not #####
KCONFIGLIB_IS_INSTALL = $(shell python3 build/scripts/check_kconfiglib.py)

-include $(LITEOSTOPDIR)/config.mk

RM = rm -rf
MAKE = make
__LIBS = libs

LITEOS_TARGET = vs_server
LITEOS_LIBS_TARGET = libs_target

.PHONY: all lib clean cleanall $(LITEOS_TARGET) debug release help

all: $(OUT) $(BUILD) $(LITEOS_TARGET)
lib: $(OUT) $(BUILD) $(LITEOS_LIBS_TARGET)

help:
	$(HIDE)echo "-------------------------------------------------------"
	$(HIDE)echo "1.====make help:    get help information of make"
	$(HIDE)echo "2.====make:         make a debug version based the .config"
	$(HIDE)echo "3.====make debug:   make a debug version based the .config"
	$(HIDE)echo "4.====make release: make a release version for all platform"
	$(HIDE)echo "5.====make release PLATFORM=xxx:  make a release version only for platform xxx"
	$(HIDE)echo "xxx should be one of (hi3516cv300 hi3516ev200 hi3556av100/cortex-a53_aarch32 hi3559av100/cortex-a53_aarch64)"
	$(HIDE)echo "-------------------------------------------------------"

debug:
	$(HIDE)echo "=============== make a debug version  ==============="
	$(HIDE) $(MAKE) all

release:
ifeq ($(PLATFORM),)
	$(HIDE)echo "=============== make a release version for platform $(PLATFORM) ==============="
	$(HIDE)$(SCRIPTS_PATH)/mklibversion.sh $(PLATFORM)
else
	$(HIDE)echo "================make a release version for all platform ==============="
	$(HIDE)$(SCRIPTS_PATH)/mklibversion.sh
endif
##### make wow && make scatter && make wow_scatter #####
-include $(LITEOS_MK_PATH)/wow_scatter.mk

##### make wow && make scatter && make wow_scatter #####
-include $(LITEOS_MK_PATH)/aslr.mk

##### make dynload #####
-include $(LITEOS_MK_PATH)/dynload.mk
GSYM_FILE = $(OUT)/dynload_dir/dynload_sym/los_dynload_gsymbol.c

##### make multi binaries #####
-include $(LITEOS_MK_PATH)/multi_binaries.mk

##### make yaffs2image tool #####
-include $(LITEOS_MK_PATH)/yaffs2tool.mk

##### make build_lib tool #####
-include $(LITEOS_MK_PATH)/compiler_lib.mk

#-----need move when make version-----#
##### make lib #####
$(__LIBS): $(OUT) $(CXX_INCLUDE)
$(LITEOS_TARGET): $(__LIBS) LITEOS_BUILD

$(OUT): $(LITEOS_MENUCONFIG_H)
	$(HIDE)mkdir -p $(OUT)/lib
ifneq ($(strip $(wildcard $(BOARD_LD_S_FILE))),)
	$(HIDE)$(CC) -E $(BOARD_LD_S_FILE) -I$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM) -I$(LITEOSTOPDIR)/kernel/base/include -o $(BOARD_LD_FILE) -P
endif

$(BUILD):
	$(HIDE)mkdir -p $(BUILD)

$(LITEOS_LIBS_TARGET): $(__LIBS)
	$(HIDE)for dir in $(LIB_SUBDIRS); \
		do $(MAKE) -C $$dir all || exit 1; \
	done
	$(HIDE)echo "=============== make lib done  ==============="

include build/menuconfig/Makefile.kconfig

$(LITEOS_MENUCONFIG_H):
ifeq ($(KCONFIGLIB_IS_INSTALL), True)
ifneq ($(LITEOS_PLATFORM_MENUCONFIG_H), $(wildcard $(LITEOS_PLATFORM_MENUCONFIG_H)))
	$(HIDE)+make savemenuconfig
endif
else
	$(HIDE)$(LITEOS_MK_PATH)/make_menuconfig_headfile.sh .config $(LITEOS_PLATFORM_MENUCONFIG_H)
endif

LITEOS_DEVICE_GENERATOR: $(LITEOS_MENUCONFIG_H)
ifneq (, $(wildcard $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/$(LITEOS_PLATFORM).yaml))
	$(HIDE)$(PYTHON3) $(LITEOSTOPDIR)/internal/tools/device_generator/scripts/generator.py $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/$(LITEOS_PLATFORM).yaml
endif

LITEOS_BUILD: $(LITEOS_MENUCONFIG_H) LITEOS_DEVICE_GENERATOR
	$(HIDE)touch $(LOSCFG_ENTRY_SRC)

ifeq ($(LOSCFG_PLATFORM_OSAPPINIT), y)
ifeq ($(LOSCFG_KERNEL_RUNSTOP)$(LOSCFG_KERNEL_SCATTER), yy)
	$(HIDE)+make wow_scatter
else ifeq ($(LOSCFG_KERNEL_RUNSTOP), y)
	$(HIDE)+make wow
else ifeq ($(LOSCFG_KERNEL_SCATTER), y)
	$(HIDE)+make scatter
endif
endif

	$(HIDE)for dir in $(LITEOS_SUBDIRS); \
	do $(MAKE) -C $$dir all || exit 1; \
	done

ifeq ($(GSYM_FILE), $(wildcard $(GSYM_FILE)))
	$(HIDE)$(CC) -c $(LITEOS_CFLAGS) $(GSYM_FILE) -I $(LITEOSTOPDIR)/kernel/extended/dynload/include -o $(OUT)/dynload_dir/dynload_sym/los_dynload_gsymbol.o
endif

ifeq ($(ASLR_TYPE), "GSPR_check")
	$(HIDE)$(CC) $(ASLR_REL_INCLUDE_PATH) $(LITEOS_KERNEL_INCLUDE) -c $(ASLR_REL_FILE)  -o $(ASLR_REL_OBJECT)
	$(HIDE)mv $(ASLR_REL_FILE) $(ASLR_REL_FILE).bak
endif

$(LITEOS_TARGET):

ifeq ($(LOSCFG_PLATFORM_OSAPPINIT), y)
ifeq ($(LOSCFG_KERNEL_RUNSTOP)$(LOSCFG_KERNEL_SCATTER), yy)
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/scatter.sh $(CROSS_COMPILE) y y $(SCRIPTS_PATH)/scatter_sr $(SCRIPTS_PATH)/ld $(LITEOSTOPDIR)/liteos_tmp $(LIB_BIGODIR)
else ifeq ($(LOSCFG_KERNEL_RUNSTOP), y)
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/scatter.sh $(CROSS_COMPILE) y n $(SCRIPTS_PATH)/scatter_sr $(SCRIPTS_PATH)/ld $(LITEOSTOPDIR)/liteos_tmp $(LIB_BIGODIR)
else ifeq ($(LOSCFG_KERNEL_SCATTER), y)
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/scatter.sh $(CROSS_COMPILE) n y $(SCRIPTS_PATH)/scatter_sr $(SCRIPTS_PATH)/ld $(LITEOSTOPDIR)/liteos_tmp $(LIB_BIGODIR)
else
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
endif
else
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
endif

ifeq ($(LOSCFG_MULTI_BINARIES), y)
	$(HIDE)$(MULTI_BINARIES_SCRIPT_PATH)/multi_binaries.sh $(OUT) $(CROSS_COMPILE) $(LITEOS_COMPILER_GCCLIB_PATH) $(LITEOSTOPDIR)
	make multi_binaries
else
	$(call update_from_baselib_file)

	$(LD) $(LITEOS_LDFLAGS) $(LITEOS_TABLES_LDFLAGS) $(LITEOS_DYNLDFLAGS) $(LITEOS_ASLRLDFLAGS) -Map=$(OUT)/$@.map -o $(OUT)/$@ --start-group $(LITEOS_BASELIB) --end-group
	#$(SIZE) -t --common $(OUT)/lib/*.a >$(OUT)/$@.objsize
	$(OBJCOPY) -O binary $(OUT)/$@ $(OUT)/$@.bin
	$(OBJDUMP) -t $(OUT)/$@ |sort >$(OUT)/$@.sym.sorted
	$(OBJDUMP) -d -h $(OUT)/$@ >$(OUT)/$@.asm
	$(READELF) -a $(OUT)/$@ >$(OUT)/$@.readelf

	#$(PYTHON3) $(LITEOSTOPDIR)/tools/stackusage/stackusage.py $(OUT)/$@.asm -sp -t OsIdleTask > $(OUT)/$@.stack
	#$(NM) -S --size-sort $(OUT)/$@ >$(OUT)/$@.size
	$(HIDE) $(PYTHON3) $(SCRIPTS_PATH)/analysis_liba_size.py $(SIZE) $(OUT)/ $(OUT)/lib/
endif
ifeq ($(LOSCFG_KERNEL_SCATTER), y)
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/scatter_size.sh $(OUT)/$(LITEOS_TARGET)
endif
ifeq ($(LOSCFG_LASLR), y)
ifeq ($(LOSCFG_LASLR_GSPR), y)
# generate image rel info
	$(HIDE) $(PYTHON3) $(SCRIPTS_PATH)/aslr/generate_gspr_rel_info.py $(ASLR_TYPE) $(CROSS_COMPILE) $(OUT)/$@
ifeq ($(ASLR_TYPE), "GSPR_check")
	$(HIDE)$(SCRIPTS_PATH)/diff_files.sh $(ASLR_REL_FILE).bak $(ASLR_REL_FILE).check
endif
endif
ifeq ($(LOSCFG_LASLR_GOT), y)
# check image relocate type
	$(HIDE) $(PYTHON3) $(SCRIPTS_PATH)/aslr/check_aslr_rel_type.py $(ASLR_TYPE) $(CROSS_COMPILE) $(OUT)/$@
endif
# check aslr image independence
	$(HIDE) $(PYTHON3) $(SCRIPTS_PATH)/aslr/check_aslr_image.py $(OUT)/$@ $(OUT)/$@.map $(CROSS_COMPILE) $(LITEOS_LDFLAGS)
endif

clean:
	$(HIDE)for dir in $(LITEOS_SUBDIRS); \
		do make -C $$dir clean|| exit 1; \
	done
	$(HIDE)$(RM) .config.cmd .config.old
	$(HIDE)make wow_scatter_clean
	$(HIDE)$(RM) $(__OBJS) $(LITEOS_TARGET) $(OUT) $(BUILD) $(LITEOS_MENUCONFIG_H) *.bak *~
	$(HIDE)$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
	$(HIDE)$(RM) -rf $(LITEOS_PLATFORM_MENUCONFIG_H)
ifneq (, $(wildcard $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/$(LITEOS_PLATFORM).yaml))
	$(HIDE)$(PYTHON3) $(LITEOSTOPDIR)/internal/tools/device_generator/scripts/cleanup.py $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/$(LITEOS_PLATFORM).yaml
endif
	$(HIDE)echo "clean $(LITEOS_PLATFORM) finish"

cleanall:
	$(HIDE)rm -rf $(LITEOSTOPDIR)/out
	$(HIDE)find targets/ -name board.ld -exec rm -rf {} \;
	$(HIDE)cd sample/sample_osdrv;make clean;cd ../..;
	$(HIDE)echo "clean all"
	$(HIDE)make wow_scatter_clean
