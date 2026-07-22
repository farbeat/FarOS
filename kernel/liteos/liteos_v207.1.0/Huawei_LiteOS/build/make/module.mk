ifeq ($(OS), Linux)
OBJOUT := $(BUILD)$(dir $(subst $(LITEOSTOPDIR),,$(shell pwd)))$(MODULE_NAME)
LOCAL_PWD := $(shell pwd)
else
TEMPLITEOSTOPDIR:=$(shell cygpath -u $(LITEOSTOPDIR))
OBJOUT := $(BUILD)$(dir $(subst $(TEMPLITEOSTOPDIR),,$(shell pwd)))$(MODULE_NAME)
LOCAL_PWD := $(shell cygpath -m $(shell pwd))
endif

ifeq ($(LOCAL_SO), y)
LIBSO := $(OUT)/lib/lib$(MODULE_NAME).so
LIBA := $(OUT)/lib/lib$(MODULE_NAME).a
else
LIBSO :=
LIBA := $(OUT)/lib/lib$(MODULE_NAME).a
endif
LIB := $(LIBA) $(LIBSO)

ifeq ($(LOSCFG_SAFETY_KERNEL), y)
LIBO := $(OUT)/lib/lib$(MODULE_NAME).O
LIB  += $(LIBO)
endif
# create a separate list of objects per source type
LOCAL_SRCS_FILETR := $(filter %.c %.cpp %.s %.S,$(LOCAL_SRCS))
LOCAL_OBJS     := $(patsubst %.c,%.o,$(LOCAL_SRCS_FILETR))
LOCAL_OBJS     := $(patsubst %.cpp,%.o,$(LOCAL_OBJS))
LOCAL_OBJS     := $(patsubst %.s,%.o,$(LOCAL_OBJS))
LOCAL_OBJS     := $(patsubst %.S,%.o,$(LOCAL_OBJS))

ifeq ($(LOCAL_SRCS_NEED_SORT), y)
    LOCAL_OBJS := $(sort $(LOCAL_OBJS))
endif

LOCAL_OBJS     := $(patsubst %.o,$(OBJOUT)/%.o,$(LOCAL_OBJS))

LOCAL_CGCH := $(patsubst %.h,%.h.gch,$(LOCAL_CHS))
LOCAL_CPPGCH := $(patsubst %.h,%.h.gch,$(LOCAL_CPPHS))

all : $(LIB)

SUB_MODULE_BUILD: $(MODULE_y)
	$(HIDE) for dir in $(MODULE_y); 		\
		do $(MAKE) -C $$dir all || exit 1; 	\
	done

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LOCAL_FLAGS += -Wno-builtin-macro-redefined
endif

$(OBJOUT)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) $(LOCAL_CFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) $(LOCAL_CFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(OBJOUT)/%.o: %.S
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) $(LOCAL_ASFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(OBJOUT)/%.o: %.s
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(AS) $(LITEOS_ASFLAGS) $(LOCAL_FLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
else
	$(HIDE)$(AS) $(LITEOS_ASFLAGS) $(LOCAL_FLAGS) $(LOCAL_ASFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(OBJOUT)/%.o: %.cpp
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(GPP) $(LITEOS_CXXFLAGS) $(LOCAL_FLAGS) $(LOCAL_CPPFLAGS) -c $< -o $@
else
	$(HIDE)$(GPP) $(LITEOS_CXXFLAGS) $(LOCAL_FLAGS) $(LOCAL_CPPFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(OBJOUT)/%.o: %.cc
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(GPP) $(LITEOS_CXXFLAGS) $(LOCAL_FLAGS) $(LOCAL_CPPFLAGS) -c $< -o $@
else
	$(HIDE)$(GPP) $(LITEOS_CXXFLAGS) $(LOCAL_FLAGS) $(LOCAL_CPPFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(LOCAL_CGCH): %.h.gch : %.h
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) $(LOCAL_CFLAGS) $> $^

$(LOCAL_CPPGCH): %.h.gch : %.h
	$(HIDE)$(GPP) $(LITEOS_CXXFLAGS) $(LOCAL_FLAGS) $(LOCAL_CPPFLAGS) -x c++-header $> $^

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LOCAL_GCH := $(LOCAL_CGCH) $(LOCAL_CPPGCH)
endif

$(LOCAL_OBJS): $(LOCAL_GCH)

$(LIBA): $(LOCAL_OBJS) SUB_MODULE_BUILD
	$(HIDE)$(OBJ_MKDIR)
	$(call add_lib_to_baselib_file,$(MODULE_NAME))
ifeq ($(OS), Linux)
	$(HIDE)$(AR) $(ARFLAGS) $@ $(LOCAL_OBJS)
else
ifeq ($(LOCAL_MODULES),)
	$(HIDE)$(AR) $(ARFLAGS) $@ $(LOCAL_OBJS)
else
	$(HIDE)for i in $(LOCAL_MODULES); do \
		pushd $(OBJOUT)/$$i 1>/dev/null; \
		$(AR) $(ARFLAGS) $@ *.o;\
		popd 1>/dev/null;\
	done
endif
endif

ifeq ($(LOCAL_SO), y)
$(LIBSO): $(LOCAL_OBJS)
	$(HIDE)$(CC) $(LITEOS_CFLAGS) -fPIC -shared $^ -o $@
endif

$(LIBO): $(LOCAL_OBJS)
ifneq ($(strip $(LOCAL_OBJS)), )
	$(HIDE)$(LD) -r -o $@ $(sort $^)
endif

clean:
	$(HIDE)$(RM) $(LIB) $(OBJOUT) $(LOCAL_GCH) *.bak *~

.PHONY: all clean

# clear some variables we set here
LOCAL_SRCS_NEED_SORT :=
# LOCAL_OBJS is passed back
#LOCAL_OBJS :=

