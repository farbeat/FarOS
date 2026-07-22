ifeq ($(OS), Linux)
OBJOUT := $(BUILD)$(dir $(subst $(LITEOSTOPDIR),,$(shell pwd)))$(MODULE_NAME)
LOCAL_PWD := $(shell pwd)
else
TEMPLITEOSTOPDIR:=$(shell cygpath -u $(LITEOSTOPDIR))
OBJOUT := $(BUILD)$(dir $(subst $(TEMPLITEOSTOPDIR),,$(shell pwd)))$(MODULE_NAME)
LOCAL_PWD := $(shell cygpath -m $(shell pwd))
endif

LIBA := $(OUT)/lib/lib$(MODULE_NAME).a
LIB := $(LIBA)

ifeq ($(LOSCFG_SAFETY_KERNEL), y)
LIBO := $(OUT)/lib/lib$(MODULE_NAME).O
LIB  += $(LIBO)
endif
LITEOS_ASFLAGS += $(LITEOS_U_INCLUDES)
LITEOS_CFLAGS  += $(LITEOS_U_INCLUDES)

# create a separate list of objects per source type
LOCAL_SRCS_FILETR := $(filter %.c %.cpp %.s %.S,$(LOCAL_SRCS))

LOCAL_OBJS     := $(patsubst %.c,%.o,$(LOCAL_SRCS_FILETR))
LOCAL_OBJS     := $(patsubst %.cpp,%.o,$(LOCAL_OBJS))
LOCAL_OBJS     := $(patsubst %.s,%.o,$(LOCAL_OBJS))
LOCAL_OBJS     := $(patsubst %.S,%.o,$(LOCAL_OBJS))
LOCAL_OBJS     := $(sort $(LOCAL_OBJS))
LOCAL_OBJS     := $(patsubst %.o,$(OBJOUT)/%.o,$(LOCAL_OBJS))

LOCAL_CSRCS     := $(filter %.c,$(LOCAL_SRCS))
LOCAL_CPPSRCS   := $(filter %.cpp,$(LOCAL_SRCS))
LOCAL_ASMSRCS   := $(filter %.s,$(LOCAL_SRCS))
LOCAL_ASMSRCS2  := $(filter %.S,$(LOCAL_SRCS))

LOCAL_COBJS    := $(patsubst %.c,$(OBJOUT)/%.o,$(LOCAL_CSRCS))
LOCAL_CPPOBJS  := $(patsubst %.cpp,$(OBJOUT)/%.o,$(LOCAL_CPPSRCS))
LOCAL_ASMOBJS  := $(patsubst %.s,$(OBJOUT)/%.o,$(LOCAL_ASMSRCS))
LOCAL_ASMOBJS2 := $(patsubst %.S,$(OBJOUT)/%.o,$(LOCAL_ASMSRCS2))

LOCAL_CGCH   := $(patsubst %.h,%.h.gch,$(LOCAL_CHS))
LOCAL_CPPGCH := $(patsubst %.h,%.h.gch,$(LOCAL_CPPHS))

MODULE_CFLAGS := $(filter-out $(LIBC_FILTER_OPTS), $(LITEOS_CFLAGS))
MODULE_CXXFLAGS := $(filter-out $(LIBC_FILTER_OPTS), $(LITEOS_CXXFLAGS))
MODULE_ASFLAGS := $(filter-out $(LIBC_FILTER_OPTS), $(LITEOS_ASFLAGS))

all : $(LIB)

SUB_MODULE_BUILD: $(MODULE_y)
	$(HIDE) for dir in $(MODULE_y); 		\
		do $(MAKE) -C $$dir all || exit 1; 	\
	done

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LOCAL_FLAGS += -Wno-builtin-macro-redefined
endif

$(LOCAL_COBJS): $(OBJOUT)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_CFLAGS) $(LOCAL_CFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_CFLAGS) $(LOCAL_CFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(LOCAL_CPPOBJS): $(OBJOUT)/%.o: %.cpp
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(GPP) $(LOCAL_FLAGS) $(MODULE_CXXFLAGS) $(LOCAL_CPPFLAGS) -c $< -o $@
else
	$(HIDE)$(GPP) $(LOCAL_FLAGS) $(MODULE_CXXFLAGS) $(LOCAL_CPPFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(LOCAL_ASMOBJS2): $(OBJOUT)/%.o: %.S
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_CFLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_CFLAGS) $(LOCAL_ASFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(LOCAL_ASMOBJS): $(OBJOUT)/%.o: %.s
	$(HIDE)$(OBJ_MKDIR)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_ASFLAGS) $(LOCAL_ASFLAGS) -c $< -o $@
else
	$(HIDE)$(CC) $(LOCAL_FLAGS) $(MODULE_ASFLAGS) $(LOCAL_ASFLAGS) -D__FILE__='"$(notdir $(abspath $<))"' -c $< -o $@
endif

$(LOCAL_CGCH): %.h.gch : %.h
	$(HIDE)$(CC) $(LOCAL_FLAGS)$(MODULE_CFLAGS) $(LOCAL_CFLAGS) $> $^

$(LOCAL_CPPGCH): %.h.gch : %.h
	$(HIDE)$(GPP) $(LOCAL_FLAGS) $(MODULE_CXXFLAGS) $(LOCAL_CPPFLAGS) -x c++-header $> $^

LOCAL_GCH := $(LOCAL_CGCH) $(LOCAL_CPPGCH)

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

$(LIBO): $(LOCAL_OBJS)
ifneq ($(strip $(LOCAL_OBJS)), )
	$(HIDE)$(LD) -r -o $@ $(sort $^)
endif

clean:
	$(HIDE)$(RM) $(LIB) $(OBJOUT) $(LOCAL_GCH) *.bak *~

.PHONY: all clean
