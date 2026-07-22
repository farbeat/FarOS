
WOW_SRC := $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/os_adapt/os_adapt.c
SCATTER_SRC := $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/os_adapt/os_adapt.c

############## make wow, when only runstop used #####################
wow_image: $(__LIBS)
	@$(CC) -E $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/board.ld.S -I$(LITEOSTOPDIR)/kernel/base/include -o $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/board.ld -P
	@touch $(WOW_SRC)

	@for dir in $(LITEOS_SUBDIRS); \
		do $(MAKE) -C $$dir all LITEOS_IMAGE_MACRO=-DMAKE_WOW_IMAGE || exit 1; \
	done

wow_link: wow_image
	$(call update_from_baselib_file)
	@$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
	@$(LD) $(LITEOS_LDFLAGS) $(LITEOS_TABLES_WOW_LDFLAGS) -Map=$(BUILD)/$(LITEOS_TARGET).map -o $(BUILD)/$(LITEOS_TARGET) --start-group $(LITEOS_BASELIB) --end-group
	@touch $(WOW_SRC)

wow: wow_image wow_link
	@$(SCRIPTS_PATH)/scatter_sr/liblist.sh wow $(BUILD)/$(LITEOS_TARGET) $(BUILD)/$(LITEOS_TARGET).map $(SCRIPTS_PATH)/scatter_sr $(LITEOS_COMPILER_GCCLIB_PATH) $(LITEOS_COMPILER_CXXLIB_PATH) $(OUT)/lib

################ make scatter, when only scatter used ##############
scatter_image: $(__LIBS)
	@$(CC) -E $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/board.ld.S -I$(LITEOSTOPDIR)/kernel/base/include -o $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/board.ld -P
	@touch $(SCATTER_SRC)

	@for dir in $(LITEOS_SUBDIRS); \
		do $(MAKE) -C $$dir all LITEOS_IMAGE_MACRO=-DMAKE_SCATTER_IMAGE || exit 1; \
	done

scatter_link: scatter_image
	$(call update_from_baselib_file)
	@$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
	@$(LD) $(LITEOS_LDFLAGS) $(LITEOS_TABLES_WOW_LDFLAGS) $(LITEOS_TABLES_SCATTER_LDFLAGS) -Map=$(BUILD)/$(LITEOS_TARGET).map -o $(BUILD)/$(LITEOS_TARGET) --start-group $(LITEOS_BASELIB) --end-group
	@touch $(SCATTER_SRC)

scatter: scatter_image scatter_link
	@$(SCRIPTS_PATH)/scatter_sr/liblist.sh scatter $(BUILD)/$(LITEOS_TARGET) $(BUILD)/$(LITEOS_TARGET).map $(SCRIPTS_PATH)/scatter_sr $(LITEOS_COMPILER_GCCLIB_PATH) $(LITEOS_COMPILER_CXXLIB_PATH) $(OUT)/lib

################## make wow_scatter, when runstop and scatter both used ###################
wow_scatter:
	@+make wow
	@+make scatter

wow_scatter_clean:
	@-$(RM) $(SCRIPTS_PATH)/scatter_sr/lib_list.*
	@-$(RM) $(SCRIPTS_PATH)/scatter_sr/symbol_list.*
	@$(SCRIPTS_PATH)/scatter_sr/clear_ld.sh $(SCRIPTS_PATH)/ld
	@touch -c $(WOW_SRC)
	@touch -c $(SCATTER_SRC)

.PHONY: image wow_image scatter_image wow scatter wow_scatter wow_scatter_clean
