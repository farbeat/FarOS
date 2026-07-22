/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Main boot for Second Stage Bootloader (SSB).
 * Author: @CompanyNameTag
 * Create: 2021-03-09
 */

#include <stdbool.h>
#include "codeloader.h"
#include "hal_cpu_core.h"
#include "hal_reboot.h"
#include "hal_sectors.h"
#include "pinctrl_porting.h"
#include "watchdog.h"
#include "watchdog_porting.h"
#include "clocks.h"
#include "bootloader_utils.h"
#include "codeloader_version.h"
#include "codeloader_configuration.h"
#include "bootloader_configuration.h"
#include "otp.h"
#include "application_version.h"
#include "memory_core.h"
#include "irmalloc.h"
#include "tcxo.h"
#include "vectors.h"
#include "non_os.h"
#include "chip_io.h"
#include "hal_qspi.h"
#include "securec.h"
#include "flash_common_config.h"
#include "flash.h"
#include "gpio.h"
#include "preserve.h"
#include "sha256/sha256.h"
#include "qspi.h"
#include "ulp_aon_control.h"
#include "hal_pmu_ldo.h"
#include "pmu.h"
#include "otp_map.h"
#include "flash_config_info.h"
#include "pmu_ldo.h"
#include "sec_boot_fota.h"
#include "cpu_utils.h"
#include "sec_boot_public.h"
#include "clocks_core_common.h"
#include "psram_drv.h"
#include "partition.h"
#include "soc_errno.h"
#include "upg_definitions.h"
#include "hal_xip.h"
#include "pinctrl.h"
#include "systick.h"
#include "boot_ulp.h"
#include "flash_porting.h"
#include "clocks_switch_priv.h"
#include "main.h"
#include "hal_uart_v120_regs_op.h"
#include "hal_pmu_vdd.h"

#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#endif

#ifdef CFG_DRIVERS_MMC
#include "drv_mmc_adapter.h"
#endif

#define CFG_PMU_PROTECT_WDT_RESET    1

#define FLASH_INIT_DATA              0xff
#define FLASH_CHECK_LEN              16
#define FLASH_FIRST_PAGE             0
#define COPY_ONE_WORD_LEN             4
#define FLASH_READ_QE_CMD            0x35
#define FLASH_READ_QE_CMD_BIT        0x1
#define FLASH_READ_QE_CMD_VALUE      0x2  // QSPI mode
#define FLASH_SET_QE_CMD_COUNT       0xa
#define READ_FLASH_ID_COUNT          0xa

#define TCXO_REBOOT_DELAY             500ULL
#define TCXO_UART_DELAY               200ULL
#define READ_FLASH_ID_DELAY           20ULL
#define STARTUP_FIRST_INSTRUCTION     0x40006f
#define XIP_READ_QSPI_SYNC_0_OFF      0x304
#define XIP_READ_OVER_TIME_L_0_OFF    0x31c
#define XIP_READ_OVER_TIME_L          (XIP_CACHE_CTL + XIP_READ_OVER_TIME_L_0_OFF)
#define XIP_READ_QSPI_SYNC            (XIP_CACHE_CTL + XIP_READ_QSPI_SYNC_0_OFF)
#define XIP_READ_OVER_TIME_PERIOD     0x900
#define XIP_READ_QSPI_SYN_EN          0x1
#define PSRAM_XIP_START               0x6c000000
#define PSRAM_XIP_VALUE               0xdeadbeef
#define PSRAM_OPI_DQ                  0x57008288
#define PSRAM_OPI_DQ_VAL              0x44
#define PSRAM_CMD_REG_MR01            0x0d8d   // cmd:c0c0 addr:0  reg:mr0+mr1
#define PSRAM_EXIST                   1U
#define PSRAM_NOT_EXIST               0U
#define UPG_FLAG_OFFSET_OF_FOTA_DATA (0x1000 * 2)
#define UPG_UNFINISHED_FLAG 0xFFFFFFFF
#define DELAY_TIME_30US               30
#define FLASH_PAGE_NUMS_2             2
#define FLASH_PAGE_NUMS_3             3

uint32_t g_psram_exist = PSRAM_NOT_EXIST;
uint8_t  g_flash_contents[FLASH_PAGE_SIZE];

void clocks_system_init(void);
/*
 * -- Private function definitions
 */
typedef void (*RESET_HANDLER_WITH_ARGS)(uint32_t *table_addr, uint32_t length, uint32_t params);

static bool code_is_valid(const uint32_t *buff)
{
    /* g_RamExceptionTable data fit this feature */
    /* The APP start address does not locate the interrupt vector table for linx170. */
    /* Adaptation is required for this */
    if (buff[0] == STARTUP_FIRST_INSTRUCTION) {
        return true;
    }
    return false;
}

/**
 * check flash is empty
 */
static bool flash_is_empty(void)
{
    if (non_os_is_aon_flag_initialised(AON_ROMLOADER_DOWNLOAD_KEY_MODE) ||
        non_os_is_aon_flag_initialised(AON_FLASH_BACKUP_MODE)) {
        return true;
    }

    uint8_t buff[FLASH_CHECK_LEN] = { 0 };

    if (uapi_flash_read_data(0, 0, buff, FLASH_CHECK_LEN) == FLASH_CHECK_LEN) {
        /* flash init state */
        if (buff[0] == FLASH_INIT_DATA) {
            return true;
        }

        if (code_is_valid((uint32_t *)buff)) {
            return false;
        } else {
            PRINT("code valid fail\r\n");
        }
    }

    return true;
}

static bool is_rom_codeloadered(void)
{
    hal_uart_regs_init(UART_BUS_1);
    uint32_t vali = hal_uart_uartibrd_get_baud_divint(UART_BUS_1);
    uint32_t valf = hal_uart_uartfbrd_get_baud_divfrac(UART_BUS_1);
    if (vali == 0 && valf == 0) {
        return false;
    }
    return true;
}

/**
 * save ssb image to ram for boot
 */
static bool ssb_save_image_in_flash(void)
{
    uint8_t *dsc = (uint8_t *)APP_ITCM_ORIGIN;
    dsc = dsc + SSB_FLASH_REGION_LENGTH;
    uint32_t i = SSB_FLASH_REGION_LENGTH;

    if (uapi_flash_block_erase(0, 0, SSB_FLASH_REGION_LENGTH, true) != ERRCODE_SUCC) {
        PRINT("SSB ERASE error" NEWLINE);
        return false;
    }
    PRINT("SSB ERASE OK" NEWLINE);
    do {
        i -= FLASH_PAGE_SIZE;
        dsc -= FLASH_PAGE_SIZE;

        if (i == FLASH_FIRST_PAGE) {
            if (uapi_flash_write_data(0, COPY_ONE_WORD_LEN, (uint8_t *)(dsc + COPY_ONE_WORD_LEN),
                (FLASH_PAGE_SIZE - COPY_ONE_WORD_LEN)) != (FLASH_PAGE_SIZE - COPY_ONE_WORD_LEN)) {
                PRINT("copy ssb into first word of flash error" NEWLINE);
                return false;
            }
            if (uapi_flash_write_data(0, 0, dsc, COPY_ONE_WORD_LEN) != COPY_ONE_WORD_LEN) {
                PRINT("copy ssb into first page of flash error" NEWLINE);
                return false;
            }
        } else {
            if (uapi_flash_write_data(0, i, dsc, FLASH_PAGE_SIZE) != FLASH_PAGE_SIZE) {
                PRINT("copy ssb into flash error, addr = %x\r\n", (FLASH_START + i));
                return false;
            }
        }
        // check flash contents
        if (uapi_flash_read_data(0, i, g_flash_contents, FLASH_PAGE_SIZE) != FLASH_PAGE_SIZE) {
            PRINT("read flash error, addr = %x\r\n", (FLASH_START + i));
            return false;
        }

        if (memcmp((void *)g_flash_contents, (void *)dsc, FLASH_PAGE_SIZE) != 0) {
            PRINT("flash check error. i:%d\n\r\n", i);
            return false;
        }
        uapi_watchdog_kick();
    } while (i > 0);

    uapi_tcxo_delay_ms(1000ULL);
    PRINT("Copy ssb into flash success" NEWLINE);
    return true;
}

/**
 * save ssb image to ram for boot
 */
static bool ssb_save_sign_in_flash(void)
{
    if (!non_os_is_aon_flag_initialised(AON_ROMLOADER_DOWNLOAD_KEY_MODE) &&
        !non_os_is_aon_flag_initialised(AON_FLASH_BACKUP_MODE)) {
        return true;
    }

    if (non_os_is_aon_flag_initialised(AON_FLASH_BACKUP_MODE)) {
        if (uapi_flash_read_data(0, (MCU_SUB_PUBKEY_BACKUP_START - FLASH_START),
            (uint8_t*)(uintptr_t)ROMLOADER_USE_CERT_ORIGIN, ROMLOADER_USE_CERT_LENGTH) != ROMLOADER_USE_CERT_LENGTH) {
            PRINT("copy backup subkey to subkey error" NEWLINE);
            return false;
        }

        if (uapi_flash_read_data(0, (MCU_SSB_IMAGE_SIGN_BACKUP_START - FLASH_START),
            (uint8_t *)(uintptr_t)ROMLOADER_USE_SSB_SIGN_ORIGIN, ROMLOADER_USE_SSB_SIGN_LENGTH) !=
            ROMLOADER_USE_SSB_SIGN_LENGTH) {
            PRINT("copy backup sign to ssb sign error" NEWLINE);
            return false;
        }

        // erase 2 flash pages(subpubkey, ssb image sign)
        if (uapi_flash_block_erase(0, MCU_SUB_PUBKEY_START - FLASH_START,
            (FLASH_PAGE_NUMS_2 * FLASH_PAGE_SIZE), true) != ERRCODE_SUCC) {
            PRINT("copy backup sign to ssb sign" NEWLINE);
            return false;
        }
        PRINT("backup: erase subpubkey and ssb image region!!!" NEWLINE);
    } else {
        // erase 3 flash pages(rootkey, subpubkey, ssb image sign)
        if (uapi_flash_block_erase(0, MCU_ROOT_PUBKEY_START - FLASH_START,
            (FLASH_PAGE_NUMS_3 * FLASH_PAGE_SIZE), true) != ERRCODE_SUCC) {
            PRINT("flash erase rootkey error" NEWLINE);
            return false;
        }

        if (uapi_flash_write_data(0, MCU_ROOT_PUBKEY_START - FLASH_START,
            (uint8_t *)(uintptr_t)ROMLOADER_USE_ROOTKEY_ORIGIN, ROMLOADER_USE_ROOTKEY_LENGTH) !=
            ROMLOADER_USE_ROOTKEY_LENGTH) {
            PRINT("copy rootkey into flash error" NEWLINE);
            return false;
        }
    }

    if (uapi_flash_write_data(0, MCU_SUB_PUBKEY_START - FLASH_START, (uint8_t *)(uintptr_t)ROMLOADER_USE_CERT_ORIGIN,
        ROMLOADER_USE_CERT_LENGTH) != ROMLOADER_USE_CERT_LENGTH) {
        PRINT("copy subkey certificate into flash error" NEWLINE);
        return false;
    }

    if (uapi_flash_write_data(0, MCU_SSB_IMAGE_SIGN_START - FLASH_START,
        (uint8_t *)(uintptr_t)ROMLOADER_USE_SSB_SIGN_ORIGIN, ROMLOADER_USE_SSB_SIGN_LENGTH) !=
        ROMLOADER_USE_SSB_SIGN_LENGTH) {
        PRINT("copy ssb image sign into flash error" NEWLINE);
        return false;
    }

    PRINT("copy sec boot sign info succ" NEWLINE);
    return true;
}

static void jump_to_cimage(core_images_e cimage)
{
    uint32_t *table_addr = NULL;
    uint32_t table_length;

    if ((cimage != CORE_IMAGES_APPS) && (cimage != CORE_IMAGES_RECOVERY)) {
        PRINT("cimage param error");
        return;
    }
    uint32_t jump_addr = (cimage == CORE_IMAGES_APPS) ? APP_VECTORS_LOAD_ORIGIN : RECOVERY_FLASH_REGION_START;
    // 暂时copy 64k到itcm
    errno_t sec_ret = memcpy_s((void *)(uintptr_t)APP_VECTORS_ORIGIN, APP_VECTORS_LENGTH,
                               (void *)(uintptr_t)jump_addr, APP_VECTORS_LENGTH);
    if (sec_ret != EOK) {
        PRINT("Memory process is error, jump_addr 0x%x, sec_ret %d", jump_addr, sec_ret);
        return;
    }

    hal_xip_clear_tag();
    get_ssb_application_table(&table_addr, &table_length);
    PRINT("jump_to_cimage :%d, addr:0x%x" NEWLINE, cimage, APP_ITCM_ORIGIN);

    if (code_is_valid((uint32_t *)APP_ITCM_ORIGIN) &&
        ((cimage == CORE_IMAGES_APPS) || (cimage == CORE_IMAGES_RECOVERY))) {
        ((RESET_HANDLER_WITH_ARGS)(APP_VECTORS_ORIGIN))(table_addr, table_length, g_psram_exist);
    } else {
        watchdog_turnoff_clk();
        while (1) {  //lint !e716
        }
    }
}

static void ssb_invoke_codeloader(void)
{
    PRINT("code start codeloader\r\n");
#if defined(SW_UART_DEBUG) && defined(CODELOADER_UART_BUS)
    if ((SW_UART_DEBUG == UART_BUS_0 && CODELOADER_UART_BUS == UART_BUS_0) ||
        (SW_UART_DEBUG == UART_BUS_1 && CODELOADER_UART_BUS == UART_BUS_1)) {
        while (uapi_uart_has_pending_transmissions(SW_DEBUG_UART_BUS)) { }
        sw_debug_uart_deinit();
    }
#endif

    codeloader_options_t cl_options;
    memset_s(&cl_options, sizeof(codeloader_options_t), 0, sizeof(codeloader_options_t));

    cl_options.info.codeloader_type = CODELOADER_TYPE_SSB;
    cl_options.info.rom_code_version = bootloader_utils_get_rom_version();
    cl_options.info.chip = CODELOADER_CHIP_CURRENT_CHIP;
    cl_options.info.codeloader_version = CODELOADER_VERSION;

    cl_options.use_long_timeout = false;
    codeloader_configuration_get_permissions(&(cl_options.security_core_allowed),
                                             &(cl_options.protocol_core_allowed), &(cl_options.apps_core_allowed));

    // If the code loader is allowed load it
    if (cl_options.security_core_allowed || cl_options.protocol_core_allowed || cl_options.apps_core_allowed) {
        codeloader_retp_t cl_retp;
        (void)codeloader_main_loop(&cl_options, &cl_retp);
    }

#if defined(SW_UART_DEBUG) && (SW_DEBUG_UART_BUS == CODELOADER_UART_BUS)
    sw_debug_uart_init(UART_L_BAUDRATE);
#endif
    PRINT("code finish codeloader" NEWLINE);
}

static void ssb_set_flash_qe(uint32_t flash_set_qe_index)
{
    errcode_t loop_flag;
    uint8_t i = 0;

    PRINT("Set FLASH:%x QE\r\n", g_flash_need_set_qe[flash_set_qe_index].manufacturer_id);
    do {
        i++;
        loop_flag = uapi_flash_send_cmd_exe(0, g_flash_need_set_qe[flash_set_qe_index].flash_set_qe_cmd);
        if (i > FLASH_SET_QE_CMD_COUNT) {
            break;
        }
    } while (loop_flag != ERRCODE_SUCC);

    if (i > FLASH_SET_QE_CMD_COUNT) {
        PRINT("SET QE FAIL!\r\n");
        cpu_utils_set_system_status_by_cause(REBOOT_CAUSE_BT_SYSRESETREQ);
        hal_reboot_chip();
    } else {
        PRINT("SET QE SUCESS!\r\n");
    }
}

static void ssb_enable_qe_mode_if_need(uint32_t manufacture_id)
{
    uint32_t ssb_need_set_qe_num = sizeof(g_flash_need_set_qe) / sizeof(g_flash_need_set_qe[0]);
    for (uint32_t i = 0; i < ssb_need_set_qe_num; i++) {
        if (manufacture_id == g_flash_need_set_qe[i].manufacturer_id) {
            ssb_set_flash_qe(i);
            break;
        }
    }
}

static void hw_init(void)
{
    panic_init();
    uapi_gpio_init();
    uapi_pin_set_mode(ULP_GPIO5, (pin_mode_t)HAL_PIO_FUNC_SWD);
    uapi_pin_set_mode(ULP_GPIO6, (pin_mode_t)HAL_PIO_FUNC_SWD);
    uapi_pin_set_ds(ULP_GPIO5, (pin_drive_strength_t)HAL_PIO_DRIVE_3);
    uapi_pin_set_ds(ULP_GPIO6, (pin_drive_strength_t)HAL_PIO_DRIVE_3);
    uapi_pin_set_ds(S_EGPIO0, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    uapi_pin_set_ds(S_EGPIO1, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    uapi_pin_set_ds(S_EGPIO2, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    uapi_pin_set_ds(S_EGPIO3, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    uapi_pin_set_ds(S_EGPIO4, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    uapi_pin_set_ds(S_EGPIO5, (pin_drive_strength_t)HAL_PIO_DRIVE_0);
}

static bool upg_need_upgrade(fota_upgrade_flag_area_t *upg_flag_info)
{
    bool ret = false;
    uint32_t fota_address = 0;
    partition_information_t info;
    flash_info_t flash_info;
    uint32_t real_read = 0;

    uapi_flash_get_info(0, &flash_info);
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC || info.type != PARTITION_BY_ADDRESS) {
        return false;
    }
    fota_address = info.part_info.addr_info.addr;
    fota_address += UPG_FLAG_OFFSET_OF_FOTA_DATA;
    non_os_enter_critical();
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto end;
    }
    if (flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) {
        real_read = uapi_flash_read_data_without_align(0, fota_address, (uint8_t *)(upg_flag_info),
        sizeof(fota_upgrade_flag_area_t));
    } else {
        real_read = uapi_flash_read_data(0, fota_address, (uint8_t *)(upg_flag_info),
        sizeof(fota_upgrade_flag_area_t));
    }
    if (real_read != sizeof(fota_upgrade_flag_area_t)) {
        goto end;
    }

    if (!(upg_flag_info->head_magic == UPG_HEAD_MAGIC &&
          upg_flag_info->head_end_magic == UPG_END_MAGIC &&
          upg_flag_info->complete_flag != 0)) {
        /* 不需要升级直接返回 */
        PRINT("No need to upgrade...\r\n");
        goto end;
    }
    ret = true;
end:
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    return ret;
}

static void ssb_erase_upg_flag(void)
{
    uint32_t flag_offsets = 0;
    partition_information_t info;
    errcode_t ret = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret != ERRCODE_SUCC || info.type != PARTITION_BY_ADDRESS) {
        return;
    }
    flag_offsets = info.part_info.addr_info.addr + UPG_FLAG_OFFSET_OF_FOTA_DATA;

    non_os_enter_critical();
    if ((errcode_t)uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
        goto erase_end;
    }
    uapi_flash_sector_erase(0, flag_offsets, true);
erase_end:
    uapi_flash_switch_to_xip_mode(0);
    non_os_exit_critical();
    return;
}

static core_images_e ssb_get_jump_image(void)
{
    fota_upgrade_flag_area_t upg_flag_info;
    // 不需要升级，跳转APP
    if (!upg_need_upgrade(&upg_flag_info)) {
        return CORE_IMAGES_APPS;
    }

    // APP升级未完成，跳转APP
    if (upg_flag_info.complete_flag == UPG_UNFINISHED_FLAG) {
        return CORE_IMAGES_APPS;
    }

    // recovery升级未完成，跳转Recovery
    return CORE_IMAGES_RECOVERY;
}

static void ssb_enter_app(void)
{
    core_images_e cimage = ssb_get_jump_image();
    if (cimage == CORE_IMAGES_RECOVERY) {
        if (!sec_boot_verify_recovery_image()) {
            cimage = CORE_IMAGES_APPS;
            ssb_erase_upg_flag();
        } else {
            jump_to_cimage(cimage);
        }
    }
    if (sec_boot_verify_standard_image()) {
        jump_to_cimage(cimage);
    }
}

static void watchdog_init(void)
{
    watchdog_turnon_clk();
#ifdef NO_TIMEOUT
    uapi_watchdog_disable();
    watchdog_turnoff_clk();
#endif
}

static void ssb_try_read_flash_id(uint32_t *manufacture_id)
{
    uint8_t i = 0;
    non_os_set_driver_initalised(DRIVER_INIT_FLASH, true); // flash has been initialized in ROM, so initial flag bit

    uapi_flash_read_id(0, manufacture_id);
    while (*manufacture_id == 0x0 && i < READ_FLASH_ID_COUNT) {
        flash_porting_power_off(0);
        flash_porting_power_on(0);
        uapi_tcxo_delay_ms(READ_FLASH_ID_DELAY);
        uapi_flash_read_id(0, manufacture_id);
        i++;
    }
    PRINT("manufacture_id = 0x%x, read id count = 0x%x\r\n", *manufacture_id, i);
}

/* The qspi speed is too slow.
 * The xip overtime is too fast, and timeout is easy. */
static void ssb_adjust_xip_over_time(void)
{
    writew(XIP_READ_OVER_TIME_L, XIP_READ_OVER_TIME_PERIOD);
    writew(XIP_READ_QSPI_SYNC, XIP_READ_QSPI_SYN_EN);
}

static bool psram_is_exist(void)
{
    uint32_t val = 0;
    (void)uapi_psram_read_reg(0, &val);
    if ((uint16_t)val != PSRAM_CMD_REG_MR01) {
        PRINT("No psram exists on the current board, skipping psram init."NEWLINE);
        return false;
    }
    return true;
}

static void get_psram_density(void)
{
    uint32_t val = 0;
    uint32_t density = 0;

    if (uapi_psram_read_reg(1, &val) != ERRCODE_SUCC) {
        PRINT("get_psram_density fail."NEWLINE);
    }

    /* The lowest three bits indicate the psram capacity. */
    density = val & 0x7;

    PRINT("get_psram_density succ, MR1MR2 is 0x%x, density is 0x%x."NEWLINE, val, density);

    if (density == PSRAM_DENSITY_16M) {
        g_psram_exist = PSRAM_DENSITY_16M;
    } else if (density == PSRAM_DENSITY_64M) {
        g_psram_exist = PSRAM_DENSITY_64M;
    } else if (density == PSRAM_DENSITY_32M) {
        g_psram_exist = PSRAM_DENSITY_32M;
    } else {
        PRINT("psram density unknows, density is 0x%x."NEWLINE, density);
    }
}

static void init_psram_section(void)
{
    writel(PSRAM_OPI_DQ, PSRAM_OPI_DQ_VAL);
    uapi_psram_reset();
    uapi_psram_init();
    bool ret = psram_is_exist();
    if (ret == true) {
        g_psram_exist = PSRAM_EXIST;
        get_psram_density();
        uapi_psram_enter_xip();
        writel(PSRAM_XIP_START, PSRAM_XIP_VALUE);
        PRINT("Enter PSRAM XIP with value: 0x%x is %s\r\n",\
            readl(PSRAM_XIP_START), (readl(PSRAM_XIP_START) == PSRAM_XIP_VALUE) ? "Succ" : "Fail");
        hal_xip_clear_tag();
    } else {
        PRINT("PSRAM is not exist.\r\n");
    }
}

static void xip_init(void)
{
    // Switch flash to qspi mode in ssb, to enable xip
    errcode_t ret;
    ret = uapi_flash_switch_to_qspi_init(0);
    if (ret != ERRCODE_SUCC) {
        PRINT("Switch to qspi init fail\r\n");
        return;
    }
    ret = uapi_flash_switch_to_cache_mode(0);
    if (ret != ERRCODE_SUCC) {
        PRINT("Switch to cache mode\r\n");
        return;
    }

    ssb_adjust_xip_over_time();
    PRINT("Enter XIP with value: 0x%x is %s\r\n", readl(FLASH_START), (readb(FLASH_START) == 0x6f) ? "Succ" : "Fail");
    init_psram_section();
}

static void ssb_hold_flash_pin(void)
{
    /* In standard mode, when the IO2 and IO3 pins are at low level,
     * the flash will ignores the input signals and is suspended.
     */
    uapi_pin_set_mode(QSPI0_D2, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    uapi_pin_set_mode(QSPI0_D3, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(QSPI0_D2, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_dir(QSPI0_D3, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(QSPI0_D2, GPIO_LEVEL_HIGH);
    uapi_gpio_set_val(QSPI0_D3, GPIO_LEVEL_HIGH);
}

static void ssb_resume_flash_pin(void)
{
    uapi_pin_set_mode(QSPI0_D2, (pin_mode_t)HAL_PIO_FUNC_QSPI0);
    uapi_pin_set_mode(QSPI0_D3, (pin_mode_t)HAL_PIO_FUNC_QSPI0);
}

#if CFG_PMU_PROTECT_WDT_RESET == 1
void software_delay_us(uint32_t delay)
{
#define SOFT_DELAY_COUNT    40
    volatile uint32_t val = delay;
    volatile uint32_t count;

    while (--val > 0) {
        count = SOFT_DELAY_COUNT;
        while (count-- > 0) {
            ;
        }
    }
}

static void clear_wdt_reset_cause(void)
{
    for (uint16_t i = 0; i < UINT16_MAX; i++) {
        writew(0x57004304, 0x1);
        if (reg16_getbit(0x57004300, 0) == 0) {
            break;
        }
    }
}

static void pmu_protect_config_and_reset(bool rom_codeloadered)
{
    // codeloader
    if (rom_codeloadered == true) {
        writew(0x5702c010, 0x0);
        return;
    }

    // wdt reset
    if (reg16_getbit(0x57004300, 0) == 1) {
        if (readw(0x5702c010) == 0x5a00) {
            clear_wdt_reset_cause();
            writew(0x5702c010, 0x0);
        }
        return;
    }

    if (readw(0x5702c010) == 0x5a00) {
        writew(0x5702c010, 0x0);
        return;
    }

    PRINT("first reset, 0x%x \r\n", readw(0x5702c010));

    writew(0x5702c010, 0x5a00);
    uapi_tcxo_delay_ms(10ULL);

    writew(0x52000700, 0x0);  // disable nmi int
    watchdog_turnon_clk();
    uapi_watchdog_init(CHIP_WDT_TIMEOUT_2S);
    uapi_watchdog_enable(WDT_MODE_RESET);

    pmu_vdd0p8_power_vset(HAL_PMU_VDD0P8_1P0_1P100V);
    writew(0x57028104, 0xF);
    writew(0x57028184, 0x1);

    hal_pmu_intldo1_set_voltage(HAL_PMU_INTLDO1_LEVEL_1P92V);

    writew(0x5700412c, 0x1e); // set short_pd and softtime_adj
    writew(0x57004138, 0x1e);

    // DBB CCRG 16M
    writew(0x5500060c, 0x103); // cpu 8 div
    writew(0x5500060c, 0x113);
    writew(0x55000614, 0x13);  // perp ls 16M
    writew(0x55000618, 0x13);  // perp uart 16M
    writew(0x5500061c, 0x13);  // perp spi 16M
    writew(0x55000620, 0x13);  // combus ch0 16M
    writew(0x55000624, 0x13);  // combus ch0 16M
    writew(0x5500062c, 0x13);  // membus 16M
    writew(0x55000630, 0x13);  // xip opi 16M
    writew(0x55000634, 0x13);  // xip qspi 16M
    hal_clocks_set_aon_bus_div(4);  // aon bus 8M

    writew(0x57004130, 0x1822);   // 1.9->2.0v
    reg16_setbit(0x57004124, POS_14); // vo_det 0->1
    writew(0x57004108, 0x14C0);  // detect threshodd 1.85->1.7v
    uapi_tcxo_delay_ms(100UL);

    // 32k -> 1M   2s -> 64ms
    writew(0x57000060, 0x1820);
    writew(0x57000064, 0x601);
    writew(0x57000068, 0x1601);

    writew(0x57004110, 0x3);
    software_delay_us(5UL);
    writew(0x57004110, 0x7);

    // wait wdt reset
    while (1) {
    }
}
#endif

static void chip_init(bool rom_codeloadered)
{
    int_setup();
    uapi_pin_init();
    uapi_tcxo_init();
    uapi_systick_init();
    watchdog_init();
    hal_xip_init();

#ifdef SW_UART_DEBUG
    sw_debug_uart_init(UART_L_BAUDRATE);
    PRINT("SSb Debug uart init succ\r\n");
#endif

#if CFG_PMU_PROTECT_WDT_RESET == 1
    pmu_protect_config_and_reset(rom_codeloadered);
#endif
    hw_init();
    uapi_ulp_systick_self_verification();
    show_reboot_info();
    otp_init();
    irmalloc_init_default();
}

static void flash_init(void)
{
    uint32_t manufacture_id = 0;
    uint8_t unique_id = 0;

    ssb_hold_flash_pin();
    uapi_flash_init(0);
    ssb_try_read_flash_id(&manufacture_id);
    flash_read_unique_id(0, &unique_id);
#ifdef SW_UART_DEBUG
    PRINT("flash unique_id = %d.\r\n", unique_id);
#endif
    uapi_flash_config_cmd_at_xip_mode(0, unique_id);
    ssb_enable_qe_mode_if_need(manufacture_id);
    ssb_resume_flash_pin();
}

static void xip_and_save_ssb(void)
{
    xip_init();
    bootloader_configuration_init();
    create_ssb_to_application_table(hal_reboot_get_reset_reason());
    // Save flash information
    flash_config_save_info();

#ifndef SSB_EDA_VERSION
    uapi_flash_exit_from_xip_mode(0);
    ssb_invoke_codeloader();
    uapi_flash_switch_to_xip_mode(0);
    while (uapi_uart_has_pending_transmissions(SW_DEBUG_UART_BUS)) { }
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MCU_PERP_UART, CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_1);
#ifdef SW_UART_DEBUG
    sw_debug_uart_reset_baund();
#endif
#endif
    set_ssb_to_application_image_vaild(true, true);
    uapi_partition_init();
}

static void fs_init(void)
{
#ifdef CFG_DRIVERS_NANDFLASH
    nandflash_qspi_init();
#endif
#ifdef CFG_DRIVERS_MMC
    ssb_emmc_init();
#endif
}

#ifdef SW_RTT_DEBUG
extern uint32_t __rtt_bss_end;
extern uint32_t __rtt_bss_start;
#endif

void main(void)
{
    errno_t ret = 0;
    bool rom_codeloadered = is_rom_codeloadered();
#ifdef SW_RTT_DEBUG
    uint32_t size = (uint32_t)(&__rtt_bss_end) - (uint32_t)(&__rtt_bss_start);
    ret = memset_s((void *)((uint32_t)(&__rtt_bss_start)), size, 0, size);
    if (ret != EOK) {
        PRINT("rtt bss memset fail."NEWLINE);
    }
#endif

    chip_init(rom_codeloadered);
    flash_init();

    while (uapi_uart_has_pending_transmissions(SW_DEBUG_UART_BUS)) { }
#ifdef PRE_ASIC
    pmu_init();
    clocks_system_init();
#endif
#ifdef SW_UART_DEBUG
    sw_debug_uart_reset_baund();
#endif

    if ((flash_is_empty() || (rom_codeloadered == true)) && ssb_save_sign_in_flash()) {
        PRINT("flash is empty, save image into flash" NEWLINE);
        ret = ssb_save_image_in_flash();
        if (ret == false) {
            PRINT("save ssb into flash err" NEWLINE);
            uapi_tcxo_delay_ms((uint64_t)TCXO_REBOOT_DELAY);
            cpu_utils_set_system_status_by_cause(REBOOT_CAUSE_BT_SSB_EXCEPTION_REBOOT);
            // This will reset the aliased regions, you will go to ROM
            hal_reboot_chip();
        }
    }
    fs_init();
    xip_and_save_ssb();

    if (non_os_is_driver_initialised(DRIVER_INIT_OTP) == true) { otp_deinit(); }
    ssb_enter_app();
    PRINT("No jump, reset now" NEWLINE);
    uapi_tcxo_delay_ms((uint64_t)TCXO_REBOOT_DELAY);
    cpu_utils_set_system_status_by_cause(REBOOT_CAUSE_BT_SSB_EXCEPTION_REBOOT);
    // This will reset the aliased regions, you will go to ROM
    hal_reboot_chip();
}