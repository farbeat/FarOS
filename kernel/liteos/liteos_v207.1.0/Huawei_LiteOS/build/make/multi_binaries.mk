# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

multi_image: $(LITEOS_LIBS_TARGET)
	$(CC) -E $(BOARD_LD_S_FILE) -I $(LITEOSTOPDIR)/kernel/base/include -o $(BOARD_LD_FILE) -P

multi_binaries: multi_image
	$(call update_from_baselib_file)
	$(LD) $(LITEOS_LD_OPTS) $(LITEOS_LD_PATH) $(LITEOS_LD_SCRIPT) $(LITEOS_TABLES_LDFLAGS) $(LITEOS_DYNLDFLAGS) -Map=$(OUT)/$@.map -o $(OUT)/$@ --start-group $(LITEOS_BASELIB) --end-group
	$(OBJCOPY) -O binary $(OUT)/$@ $(OUT)/$@.bin
	$(OBJDUMP) -t $(OUT)/$@ |sort >$(OUT)/$@.sym.sorted
	$(OBJDUMP) -d $(OUT)/$@ >$(OUT)/$@.asm
ifeq ($(LOSCFG_SAFETY_KERNEL), y)
	$(OBJCOPY) -O binary -j *.rom.text $(OUT)/$@ $(OUT)/$@_rom.bin
	$(OBJCOPY) -O binary -R *.rom.text $(OUT)/$@ $(OUT)/$@_non_rom.bin
else
	dd if=$(OUT)/$@.bin of=$(OUT)/$@_non_rom_boot.bin seek=0 count=1 bs=1k
	dd if=$(OUT)/$@.bin of=$(OUT)/$@_rom.bin skip=1 count=50 bs=1k
	dd if=$(OUT)/$@.bin of=$(OUT)/$@_non_rom.bin skip=51 bs=1k
endif

	@mkdir -p $(OUT)/bin_test
	$(CC) -DROM_BIN_TEST -E $(BOARD_LD_S_FILE)  -I $(LITEOSTOPDIR)/kernel/base/include -o $(BOARD_LD_FILE) -P
	$(LD) $(LITEOS_LD_OPTS) $(LITEOS_LD_PATH)  $(LITEOS_LD_SCRIPT)  $(LITEOS_TABLES_LDFLAGS) $(LITEOS_DYNLDFLAGS) -Map=$(OUT)/bin_test/$@2.map -o $(OUT)/bin_test/$@2 --start-group $(LITEOS_BASELIB) --end-group
	$(OBJCOPY) -O binary $(OUT)/bin_test/$@2 $(OUT)/bin_test/$@2.bin
	$(OBJDUMP) -d $(OUT)/bin_test/$@2 >$(OUT)/bin_test/$@2.asm

ifeq ($(LOSCFG_SAFETY_KERNEL), y)
	$(OBJCOPY) -O binary -j *.rom.text $(OUT)/bin_test/$@2 $(OUT)/bin_test/$@2_rom.bin
	$(OBJCOPY) -O binary -R *.rom.text $(OUT)/bin_test/$@2 $(OUT)/bin_test/$@2_non_rom.bin
else
	dd if=$(OUT)/bin_test/$@2.bin of=$(OUT)/bin_test/$@2_non_rom_boot.bin seek=0 count=1 bs=1k
	dd if=$(OUT)/bin_test/$@2.bin of=$(OUT)/bin_test/$@2_rom.bin skip=1 count=50 bs=1k
	dd if=$(OUT)/bin_test/$@2.bin of=$(OUT)/bin_test/$@2_non_rom.bin skip=51 bs=1k
endif
	@if cmp -s $(OUT)/bin_test/$@2_rom.bin $(OUT)/$@_rom.bin; \
	then echo "#########ROM BIN IS SAME WITH FIRST COMPILE!#########"; \
	else echo "#########ROM BIN IS DIFFERENT WITH FIRST COMPILE!#########";exit 1;fi
	$(HIDE) $(PYTHON3) $(SCRIPTS_PATH)/analysis_liba_size.py $(SIZE) $(OUT)/lib/multi_binaries/ $(OUT)/lib/multi_binaries/

.PHONY : multi_binaries
