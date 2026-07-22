CPU_CONFIG_OPTIONS :=arm_generic
LIBC_RELATIVE_PATH := ./open_source/musl

include ./build/make/arch_config.mk
include ./lib/libc/libc_src.mk

# arm64 in libc is aarch64, in kernel is arm64
KERNEL_ARCH :=$(strip $(KERNEL_ARCH))
KERNEL_SUB_ARCH :=$(strip $(KERNEL_SUB_ARCH))
LIBC_ARCH := $(strip $(LIBC_ARCH))
BUILD_LIB_CC = $(TARGET_COMPILER)gcc
BUILD_LIB_AR = $(TARGET_COMPILER)ar
BUILD_LIB_RANLIB = $(TARGET_COMPILER)ranlib
OUT_OBJ :=$(TARGET_OUT)/obj/$(CPU_CONFIG_OPTIONS)

TARGET_OUT_EXIST = $(shell if [ -d $(TARGET_OUT) ]; then echo "exist"; else echo "noexist"; fi)
INSTALL_DIR_EXIST = $(shell if [ -d $(TARGET_INSTALL)/usr ]; then echo "exist"; else echo "noexist"; fi)
OUT_OBJ_EXIST = $(shell if [ -d $(OUT_OBJ) ]; then echo "exist"; else echo "noexist"; fi)
OUT_GENERIC_EXIST = $(shell if [ -d $(TARGET_OUT)/usr/lib/$(LIBC_ARCH)_generic ]; then echo "exist"; else echo "noexist"; fi)

STATIC_LIBC_LIBS = $(TARGET_OUT)/usr/lib/$(CPU_CONFIG_OPTIONS)/libc.a
STATIC_LIBM_LIBS = $(TARGET_OUT)/usr/lib/$(CPU_CONFIG_OPTIONS)/libm.a

LITEOS_LIBC_CUSTOM_PATH = ./lib/liteos_libc
LIBC_PATH = $(LIBC_RELATIVE_PATH)/src
LIBM_PATH = $(LIBC_RELATIVE_PATH)/src/math
CRT_PATH = $(LIBC_RELATIVE_PATH)/crt

LOCAL_INCLUDE = -I $(LITEOS_LIBC_CUSTOM_PATH)/pthread \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/errno \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/tzdst \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/time \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/lock \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/init \
				-I $(LITEOS_LIBC_CUSTOM_PATH)/locale \
				-I $(LIBC_RELATIVE_PATH)/src/include \
				-I $(LIBC_RELATIVE_PATH)/src/internal \
				-I $(LIBC_RELATIVE_PATH)/src/crypt \
				-I $(LIBC_RELATIVE_PATH)/src/ctype \
				-I $(LIBC_RELATIVE_PATH)/src/dirent \
				-I $(LIBC_RELATIVE_PATH)/src/errno \
				-I $(LIBC_RELATIVE_PATH)/src/ipc \
				-I $(LIBC_RELATIVE_PATH)/src/locale \
				-I $(LIBC_RELATIVE_PATH)/src/math \
				-I $(LIBC_RELATIVE_PATH)/src/multibyte \
				-I $(LIBC_RELATIVE_PATH)/src/networknetwork \
				-I $(LIBC_RELATIVE_PATH)/src/passwd \
				-I $(LIBC_RELATIVE_PATH)/src/prng \
				-I $(LIBC_RELATIVE_PATH)/src/process \
				-I $(LIBC_RELATIVE_PATH)/src/regex \
				-I $(LIBC_RELATIVE_PATH)/src/search \
				-I $(LIBC_RELATIVE_PATH)/src/time
EXTERN_LOCAL_INCLUDE  = -I ./kernel/base/include \
					    -I ./kernel/extended/include
EXTERN_LOCAL_INCLUDE += -include $(LITEOS_PLATFORM_MENUCONFIG_H)

# libc/arch/ must be the first dir
global_include_dir = $(LIBC_RELATIVE_PATH)/arch/$(LIBC_ARCH) \
					 $(LIBC_RELATIVE_PATH)/include \
				 	 $(LIBC_RELATIVE_PATH)/arch/generic \
				     $(LITEOS_LIBC_CUSTOM_PATH)/include \
				     ./arch/$(KERNEL_ARCH)/$(KERNEL_SUB_ARCH)/include \
				     ./kernel/include \
					 ./platform/libsec/include \
					 ./drivers/interrupt/include \
					 ./drivers/timer/include

GLOBAL_INCLUDE = $(addprefix -I , $(global_include_dir))

MUSL_LIB_EXTRA_FLAGS = -Wno-unused-variable \
    		           -Wno-unused-but-set-variable \
		       	       -Wno-implicit-function-declaration \
                       -Wno-sign-compare \
                       -Wno-parentheses \
    		           -Wno-implicit-fallthrough \
    		           -Wno-int-conversion \
    		           -Wno-missing-declarations \
    		           -Wno-missing-prototypes \
				       -Wno-maybe-uninitialized \
					   -Wno-strict-prototypes \
					   -Wno-unused-value \
					   -Wno-unknown-pragmas \
					   -Wno-float-equal

BUILD_LIB_CFLAGS = $(LITEOS_COPTS) $(LITEOS_CMACRO) $(LITEOS_MENUCONFIG_FLAGS) \
				   $(MUSL_LIB_EXTRA_FLAGS) $(LOCAL_INCLUDE) $(GLOBAL_INCLUDE) \
				   $(EXTERN_GLOBAL_INCLUDE) $(EXTERN_LOCAL_INCLUDE)

LIBC_FILE = $(LIBC_ERRNO_SRC) $(LIBC_STRING_SRC) $(LIBC_STDLIB_SRC) \
			$(LIBC_INTERNAL_SRC) $(LIBC_TIME_SRC) $(LIBC_MISC_SRC) \
			$(LIBC_LOCALE_SRC) $(LIBC_CTYPE_SRC) $(LIBC_TEMP_SRC) \
			$(LIBC_FCNTL_SRC) $(LIBC_SIGNAL_SRC) $(LIBC_NETWORK_SRC) \
			$(LIBC_UNISTD_SRC) $(LIBC_COMPLEX_SRC) $(LIBC_STDIO_SRC) \
			$(LIBC_FORMAT_SRC) $(LIBC_STDIO_BASE) $(LIBC_EXIT_SRC) \
			$(LIBC_PRNG_SRC) $(LIBC_MULTIBYTE_SRC) $(LIBC_LINUX_SRC) \
			$(LIBC_LEGACY_SRC) $(LIBC_THREAD_SRC)

LIBC_FILE += $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/env/*.c) \
			 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/errno/*.c) \
			 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/exit/*.c) \
			 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/init/*.c) \
			 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/ldso/*.c) \
			 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/locale/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/lock/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/misc/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/mq/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/pthread/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/sched/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/signal/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/stdlib/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/string/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/time/*.c) \
           	 $(wildcard $(LITEOS_LIBC_CUSTOM_PATH)/tzdst/*.c)

# some files depend on LiteOS's other modules, filter them out.
FILTER_FILE = $(LITEOS_LIBC_CUSTOM_PATH)/misc/getentropy.c $(LIBC_RELATIVE_PATH)/src/misc/getopt.c
LIBC_FILE_TEM = $(filter-out $(FILTER_FILE), $(LIBC_FILE))
LIBC_OBJ = $(patsubst %.c,$(OUT_OBJ)/%.o,$(LIBC_FILE_TEM))

LIBM_FILE = $(wildcard $(LIBM_PATH)/*.c)
LIBM_OBJ = $(patsubst %.c,$(OUT_OBJ)/%.o,$(LIBM_FILE))

CRT_FILE = $(wildcard $(CRT_PATH)/*.c)
CRT_OBJ = $(patsubst %.c,$(OUT_OBJ)/%.o,$(CRT_FILE))


build_lib: copy_headers $(STATIC_LIBC_LIBS) $(STATIC_LIBM_LIBS) $(CRT_OBJ)

creat_outdir:
	$(HIDE)mkdir -p $(TARGET_OUT)/usr/lib/$(CPU_CONFIG_OPTIONS);
	$(HIDE)mkdir -p $(TARGET_OUT)/usr/include;
	$(HIDE)mkdir -p $(OUT_OBJ)
copy_headers: creat_outdir
	$(foreach dir, $(global_include_dir), $(shell cp -rn ${dir}/* ${TARGET_OUT}/usr/include/;))


$(LIBC_OBJ): $(OUT_OBJ)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(BUILD_LIB_CC) -c $(BUILD_LIB_CFLAGS) $< -o $@

$(LIBM_OBJ): $(OUT_OBJ)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(BUILD_LIB_CC) -c $(BUILD_LIB_CFLAGS) $< -o $@

$(CRT_OBJ): $(OUT_OBJ)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(BUILD_LIB_CC) -c $(BUILD_LIB_CFLAGS) $< -o $@

$(STATIC_LIBC_LIBS): $(LIBC_OBJ) $(CRT_OBJ)
	$(HIDE)$(BUILD_LIB_AR) rc $@ $(LIBC_OBJ)
	$(HIDE)$(BUILD_LIB_RANLIB) $@
	$(HIDE)cp $(CRT_OBJ) $(TARGET_OUT)/usr/lib/$(CPU_CONFIG_OPTIONS)
	$(HIDE)echo "generate $(STATIC_LIBC_LIBS) done"

$(STATIC_LIBM_LIBS): $(LIBM_OBJ)
	$(HIDE)$(BUILD_LIB_AR) rc $@ $(LIBM_OBJ)
	$(HIDE)$(BUILD_LIB_RANLIB) $@
	$(HIDE)echo "generate $(STATIC_LIBM_LIBS) done"


install_lib:
ifeq ($(INSTALL_DIR_EXIST), exist)
	$(HIDE)rm -r $(TARGET_INSTALL)/usr
endif
ifeq ($(OUT_GENERIC_EXIST), exist)
	$(HIDE)mv $(TARGET_OUT)/usr/lib/$(LIBC_ARCH)_generic/* $(TARGET_OUT)/usr/lib
	$(HIDE)rm -r $(TARGET_OUT)/usr/lib/$(LIBC_ARCH)_generic
endif
	$(HIDE)mkdir -p $(TARGET_INSTALL)/usr/include
	$(HIDE)cp -r $(TARGET_OUT)/usr/include/* $(TARGET_INSTALL)/usr/include/
	$(HIDE)cp -r $(TARGET_OUT)/usr/lib $(TARGET_INSTALL)/usr/


clean_lib:
ifeq ($(TARGET_OUT_EXIST), exist)
	$(HIDE)rm -r $(TARGET_OUT)
else
	$(HIDE)#echo "dir $(TARGET_OUT) not exist, not need clean"
endif
ifeq ($(INSTALL_DIR_EXIST), exist)
	$(HIDE)rm -r $(TARGET_INSTALL)
else
	$(HIDE)#echo "dir $(TARGET_INSTALL) not exist, not need clean"
endif


clean_obj:
ifeq ($(OUT_OBJ_EXIST), exist)
	$(HIDE)rm -r $(OUT_OBJ)
else
	$(HIDE)#echo "dir $(OUT_OBJ) not exist, not need clean"
endif

.PHONY : clean_lib install_lib build_lib clean_obj
