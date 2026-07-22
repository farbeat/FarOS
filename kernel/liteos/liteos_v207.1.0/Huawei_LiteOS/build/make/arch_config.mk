# aarch64
LIBC_ARCH :=arm
# arm64
KERNEL_ARCH :=arm
KERNEL_SUB_ARCH :=cortex_a_r

ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_a7_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a17.cortex-a7
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_a7_soft)
	MCPU := -mcpu=cortex-a17.cortex-a7
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_a7_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a17.cortex-a7
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_a7_softfp_vfpv4)
	MCPU := -mcpu=cortex-a17.cortex-a7
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a17
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_soft)
	MCPU := -mcpu=cortex-a17
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a17
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a17_softfp_vfpv4)
	MCPU := -mcpu=cortex-a17
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a53_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a53
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a53_soft)
	MCPU := -mcpu=cortex-a53
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a53_soft_vfpv4)
	MCPU := -mcpu=cortex-a53
	FABI := -mfloat-abi=soft
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a53_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a53
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_a53_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a73.cortex-a53
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_a53_soft)
	MCPU := -mcpu=cortex-a73.cortex-a53
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_a53_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a73.cortex-a53
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_a53_softfp_vfpv4)
	MCPU := -mcpu=cortex-a73.cortex-a53
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a73
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_soft)
	MCPU := -mcpu=cortex-a73
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a73
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a73_softfp_vfpv4)
	MCPU := -mcpu=cortex-a73
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a7_hard_neon-vfpv4)
	MCPU := -mcpu=cortex-a7
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a7_soft)
	MCPU := -mcpu=cortex-a7
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a7_softfp_neon-vfpv4)
	MCPU := -mcpu=cortex-a7
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=neon-vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a7_softfp_vfpv4)
	MCPU := -mcpu=cortex-a7
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv4
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_hard_neon)
	MCPU := -mcpu=cortex-a9
	FABI := -mfloat-abi=hard
	FPU  := -mfpu=neon
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_soft)
	MCPU := -mcpu=cortex-a9
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_softfp_vfp)
	MCPU := -mcpu=cortex-a9
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfp
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_softfp_vfpv3-d16)
	MCPU := -mcpu=cortex-a9
	FABI := -mfloat-abi=softfp
	FPU  := -mfpu=vfpv3-d16
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_vfpv3)
	MCPU := -mcpu=cortex-a9
	FABI :=
	FPU  := -mfpu=vfpv3
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), a9_vfpv3_neon)
	MCPU := -mcpu=cortex-a9
	FABI :=
	FPU  := -mfpu=neon-vfpv3
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), armv5te_arm9_soft)
	MCPU := -mcpu=arm926ej-s -march=armv5te
	FABI := -mfloat-abi=soft
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), armv5te_arm9_vfp)
	MCPU := -mcpu=arm926ej-s -march=armv5te
	FABI :=
	FPU  := -mfpu=vfp
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), arm_generic) # arm generic
	MCPU :=
	FABI :=
	FPU  :=
else ifeq ($(strip $(CPU_CONFIG_OPTIONS)), aarch64_generic) # arm64
	MCPU := -mcpu=cortex-a53
	FABI :=
	FPU  :=
	LIBC_ARCH := aarch64
	KERNEL_ARCH :=arm64
	KERNEL_SUB_ARCH :=
	#TARGET_COMPILER := aarch64-himix210-linux-
else
$(error "invaild para $(strip $(CPU_CONFIG_OPTIONS))")
endif

LITEOS_MENUCONFIG_FLAGS := $(MCPU) $(FABI) $(FPU) -fPIC -DLOSCFG_MAKE_COMPILER_LIBS
