/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if defined(__FREERTOS__) && defined(FREERTOS_DEBUG)
#include "os_handler.h"
#endif
#if defined(__FREERTOS__) && defined(FREERTOS_CPUP)
#include "os_cpup.h"
#endif
#include "debug_print.h"
#include "securec.h"
#include "soc_osal.h"
#include "common_def.h"
#include "app_common_api.h"
#include "app_msg_manager.h"
#include "app_tx_process.h"
#include "app_at_process.h"
#include "app_test_fs.h"
#include "log_file.h"
#include "diag_msg.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

static uint32_t app_test_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);
    wstp_print("[app_test]test!!!!");
    uint8_t test0[] = {0xAF,0x03,0x05,0x00,0x24,0x0A,0x04,0x83,0x80,0x9A,0xEA};
    app_pkg_diag_msg_send(test0, sizeof(test0));
    uint8_t test1[] = {0xAF,0x03,0x0A,0x00,0x24,0x0A,0x04,0x88,0x02,0x03,0x00,0x11,0x22,0x33,0xEC,0x3B};
    app_pkg_diag_msg_send(test1, sizeof(test1));
    uint8_t test2[] = {0xAF,0x03,0x09,0x00,0x24,0x0A,0x08,0x88,0x01,0x03,0x11,0x22,0x33,0xCA,0xC3};
    app_pkg_diag_msg_send(test2, sizeof(test2));
    uint8_t test3[] = {0xAF,0x03,0x0C,0x00,0x24,0x0A,0x08,0x88,0x04,0x03,0x00,0x00,0x00,0x11,0x22,0x33,0x47,0x78};
    app_pkg_diag_msg_send(test3, sizeof(test3));
    return ERRCODE_SUCC;
}

static uint32_t app_fs_test_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    app_pkg_msg_send(FSTEST, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_reset_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    app_pkg_msg_send(RESET, DIAG_CONTROL_FALSE, NULL, 0);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_rw_test_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    app_pkg_msg_send(FSRWTEST, DIAG_CONTROL_FALSE, NULL, 0);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_format_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    app_pkg_msg_send(FSFORMAT, DIAG_CONTROL_FALSE, NULL, 0);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_set_clk_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FSSETCLK, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_read_clk_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    unused(para_len);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FSREADCLK, DIAG_CONTROL_FALSE, NULL, 0);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_write_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FSWRITE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_check_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FSCHECK, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_fs_rm_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FSRM, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_get_wstp_version_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETWSTPVER, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t parse_log_file_write(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(LOGWRITE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_switch_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_get_ext_csd_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_get_cid_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_send_cmd56_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_send_cmd60_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_emmc_set_driver_cap_handle(uint8_t *para, uint32_t para_len, char* str_cmd)
{
    unused(para);
    unused(para_len);
    unused(str_cmd);

    return ERRCODE_SUCC;
}

static uint32_t parse_heap_info_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);

    app_pkg_msg_send(HEAPINFO, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t parse_stack_info_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);

    app_pkg_msg_send(STACKINFO, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_dtcm_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTDTCMBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_l2m_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTL2MBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_mmc_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTMMCBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_psram_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTPSRAMBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_norflash_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTNORFLASHBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_nandflash_baudwidth_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TESTNANDFLASHBW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_get_fs_info_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETFSINFO, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_time_set_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TIMESET, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_time_get_handle(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(TIMEGET, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_into_ship_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SHIPMODE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_into_force_deep_sleep_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FORCEDEEPSLEEP, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_req_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(PMMODESET, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_set_pm_req_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(PMMODEUSR, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_dfx_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(PMMODEGET, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_lcd_off(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(LCDOFF, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_lcd_on(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    app_pkg_msg_send(LCDON, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_lcd_fill(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(LCDFILL, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_get_ids(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    app_pkg_msg_send(GETIDS, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}


static uint32_t app_test_lcd_light(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(LCDLIGHT, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}


static uint32_t app_test_pm_add_vote(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(PMADDVOTE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_pm_remove_vote(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(PMRMVOTE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_get_cur_second(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETCURSEC, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_touch_resume_flag(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(RESUMETOUCH, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_dynamic_power_flag(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(MCUDYMPW, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_cldo1_power_down(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(PMCLDO1PD, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_aes_decrypt(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(AESDECRYPT, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_aes_encrypt(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(AESENCRYPT, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_test_aes_setgid(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(AESSETGID, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_write(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(NANDWRITE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_read(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDREAD, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_erase(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDERASE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_mark(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDMARK, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_check(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDCHECK, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_set_mode(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDSETMODE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
static uint32_t app_adc_show_data(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ADCSHOWDATA, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_adc_autosample(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ADCAUTOSAMPLE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_adc_disableautosample(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(ADCCLOSEAUTOSAMPLE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

static uint32_t app_button_sample(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(BUTTON, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_powerkey_sample(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(POWERKEY, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_calendar_set_date(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SETCALENDAR, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_calendar_get_date(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETCALENDAR, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

#ifdef SUPPROT_EXECUTE_CORMARK
static uint32_t app_test_coremark(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(COREMARKTEST, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

static uint32_t app_set_delay_time(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(SETAPPDELAY, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_cldo_voltage(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(SETCLDO1VOL, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_testsuit_cmd(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    testsuit_msg_handle((char*)para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_nand_aging_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(NANDAGINGTEST, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_norflash_aging_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(FLASHAGINGTEST, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_pin_pull(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SETPINPULL, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_get_pin_pull(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETPINPULL, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

#ifdef SUPPORT_POWER_MANAGER
static uint32_t app_pms_set_screentime_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SETSCREENOFFTIME, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_pms_set_screenfunc_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SETSCREENOFFFUNC, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_pms_set_screenstate_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(SETSCREENSTATE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_pms_get_screenstate_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    app_pkg_msg_send(GETSCREENSTATE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef FT_DONGLE
static uint32_t app_check_dongle_connected(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    app_pkg_msg_send(DONGLECONNECTED, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

static uint32_t app_i2c_write(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(I2CWRITE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_i2c_read(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(I2CREAD, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

#ifdef SUPPORT_ALIPAY_SEC
static uint32_t app_alipay_sec_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ALIPAYSECTEST, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_alipay_pl_verify(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ALIPAYVERIFY, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_alipay_debug(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ALIPAYDEBUG, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_alipay_reset(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(ALIPAYRESET, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef SUPPORT_FILE_SPLIT_COMBINE
static uint32_t app_file_split_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(FILESPLIT, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_file_combine_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(FILECOMBINE, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef JS_ENABLE
static uint32_t app_get_trans_channel_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(GETTRANSCHAN, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_set_trans_channel_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(SETTRANSCHAN, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef SUPPORT_AUTO_OTA
static uint32_t app_auto_ota_block(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(AUTOOTABLOCK, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_auto_ota_run(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(AUTOOTARUN, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_auto_ota_del_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(AUTOOTADELMAC, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_auto_ota_add_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(AUTOOTAADDMAC, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_auto_ota_show_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(AUTOOTASHOWMAC, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef LOSCFG_KERNEL_PERF
static uint32_t app_perf_start(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(PERFSTART, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_perf_stop(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(PERFSTOP, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
static uint32_t app_sched_statistis_start(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(SCHEDSTART, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}

static uint32_t app_sched_statistis_stop(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(str_cmd);
    if (para == NULL) {
        return ERRCODE_FAIL;
    }
    app_pkg_msg_send(SCHEDSTOP, DIAG_CONTROL_FALSE, para, para_len);
    return ERRCODE_SUCC;
}
#endif

static const at_cmd_table_t g_app_cmd[] = {
/* 无参数无特殊处理AT命令 */
    { "AT^HEAPINFO",     parse_heap_info_handle,            "" },
    { "AT^STACKINFO",    parse_stack_info_handle,           "" },
/* 直接发A核的命令 */
    { "AT^APPTEST",      app_test_handle,                   "" },
    { "AT^RESET",        app_reset_handle,                  "" },
    { "AT^FSTEST",       app_fs_test_handle,                "" },
    { "AT^FSRWTEST",     app_fs_rw_test_handle,             "" },
    { "AT^FSFORMAT",     app_fs_format_handle,              "" },
    { "AT^FSSETCLK",     app_fs_set_clk_handle,             "" },
    { "AT^FSREADCLK",    app_fs_read_clk_handle,            "" },
    { "AT^FSWRITE",      app_fs_write_handle,               "" }, // AT^FSWRITE=0,"test",1024
    { "AT^FSCHECK",      app_fs_check_handle,               "" }, // AT^FSCHECK=0,"test",1024
    { "AT^FSRM",         app_fs_rm_handle,                  "" }, // AT^FSRM=3,"test"
    { "AT^GETWSTPVER",   app_get_wstp_version_handle,       "" },
    { "AT^DTCMBW",       app_test_dtcm_baudwidth_handle,    "" },
    { "AT^L2MBW",        app_test_l2m_baudwidth_handle,     "" },
    { "AT^MMCBW",        app_test_mmc_baudwidth_handle,     "" },
    { "AT^PSRAMBW",      app_test_psram_baudwidth_handle,   "" },
    { "AT^NORFLASHBW",   app_test_norflash_baudwidth_handle, "" },
    { "AT^NANDFLASHBW",  app_test_nandflash_baudwidth_handle, "" },
    { "AT^FSINFO",       app_get_fs_info_handle,            "" },
    { "AT^TIMESET",      app_time_set_handle,               "" },
    { "AT^TIMEGET",      app_time_get_handle,               "" },

    { "AT^MMCSWITCHCMD", parse_emmc_switch_handle,          "" }, // AT^MMCSWITCHCMD=0x031E0100
    { "AT^MMCGETECSD",   parse_emmc_get_ext_csd_handle,     "" }, // AT^MMCGETECSD
    { "AT^MMCGETCID",    parse_emmc_get_cid_handle,         "" }, // AT^MMCGETCID
    { "AT^MMCCMD60",     parse_emmc_send_cmd60_handle,      "" }, // AT^MMCCMD60
    { "AT^MMCCMD56",     parse_emmc_send_cmd56_handle,      "" }, // AT^MMCCMD56==0xF0000061
    { "AT^MMCDRIVERCAP", parse_emmc_set_driver_cap_handle,  "" }, // AT^MMCDRIVERCAP==5  (CLASS 0--7)
    { "AT^LOGWRITE",     parse_log_file_write,              "" },
    { "AT^PMADDVOTE",    app_test_pm_add_vote,              "" },
    { "AT^PMRMVOTE",     app_test_pm_remove_vote,           "" },
    { "AT^LCDON",        app_test_pm_lcd_on,                "" },
    { "AT^LCDOFF",       app_test_pm_lcd_off,               "" },
    { "AT^SYSSHIPMODE",  app_test_pm_into_ship_mode,        "" },
    { "AT^SYSFORCEDEEP", app_test_pm_into_force_deep_sleep_mode, "" },
    { "AT^PMMODE",       app_test_pm_req_mode,              "" },
    { "AT^PMMODESETUSR", app_test_set_pm_req_mode,          "" },
    { "AT^PMMODEGET",    app_test_pm_dfx_mode,              "" },
    { "AT^GETCURSEC",    app_get_cur_second,                "" },
    { "AT^LCDFILL",      app_test_lcd_fill,                 "" },
    { "AT^LCDLIGHT",     app_test_lcd_light,                "" },
    { "AT^GETIDS",       app_get_ids,                       "" },
    { "AT^STOUCHRESUME", app_set_touch_resume_flag,         "" },
    { "AT^SMCUDYMPW",    app_set_dynamic_power_flag,        "" },
    { "AT^PMCLDO1PD",    app_set_cldo1_power_down,          "" },
    { "AT^AESDECRYPT",   app_test_aes_decrypt,              "" },
    { "AT^AESENCRYPT",   app_test_aes_encrypt,              "" },
    { "AT^AESSETGID",    app_test_aes_setgid,               "" },
    { "AT^SETAPPDELAY",  app_set_delay_time,                "" },
    { "AT^SETCLDO1VOL",  app_set_cldo_voltage,              "" },
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    { "AT^ADCAUTOSCAN",  app_adc_autosample,                "" },
    { "AT^CLOSEADCSCAN", app_adc_disableautosample,         "" },
    { "AT^SHOWADCDATA",  app_adc_show_data,                 "" },
#endif
    { "AT^NANDW",        app_nand_write,                    "" },
    { "AT^NANDR",        app_nand_read,                     "" },
    { "AT^NANDE",        app_nand_erase,                    "" },
    { "AT^NANDM",        app_nand_mark,                     "" },
    { "AT^NANDC",        app_nand_check,                    "" },
    { "AT^NANDMODE",     app_nand_set_mode,                 "" },
    { "AT^BUTTON",       app_button_sample,                 "" },
    { "AT^PWRKEYCFG",    app_powerkey_sample,               "" },
    { "AT^SETCALENDAR",  app_calendar_set_date,             "" },
    { "AT^GETCALENDAR",  app_calendar_get_date,             "" },
    { "AT^TESTSUIT",     app_testsuit_cmd,                  "" },
    { "AT^NANDAGINGTEST", app_nand_aging_test,              "" },
    { "AT^FLASHAGINGTEST", app_norflash_aging_test,         "" },
    { "AT^SETPINPULL",   app_set_pin_pull,                  "" },
    { "AT^GETPINPULL",   app_get_pin_pull,                  "" },
#ifdef SUPPROT_EXECUTE_CORMARK
    { "AT^COREMARKTEST", app_test_coremark,                 "" },
#endif
#if defined(__FREERTOS__) && defined(FREERTOS_DEBUG)
    { "AT^FREE",         os_mem_handler,                    "" },
    { "AT^FREENODES",    os_mem_nodes_handler,              "" },
    { "AT^TASK",         os_task_handler,                   "" },
    { "AT^QUEUE",        os_queue_handler,                  "" },
    { "AT^SWTMR",        os_swtmr_handler,                  "" },
    { "AT^SYSTEMINFO",   os_sys_handler,                    "" },
    { "AT^SEM",          os_semphr_handler,                 "" },
    { "AT^MUTEX",        os_mutex_handler,                  "" },
    { "AT^HWI",          os_hwi_handler,                    "" },
#endif
#if defined(__FREERTOS__) && defined(FREERTOS_CPUP)
    { "AT^CPUP",         os_cpup_handler,                    "" },
#endif
#ifdef SUPPORT_POWER_MANAGER
    { "AT^SETSCREENTIME", app_pms_set_screentime_test,       "" },
    { "AT^SETSCREENFUNC", app_pms_set_screenfunc_test,       "" },
    { "AT^SETSCREENSTATE", app_pms_set_screenstate_test,     "" },
    { "AT^GETSCREENSTATE", app_pms_get_screenstate_test,     "" },
#endif
#ifdef FT_DONGLE
    { "AT^ISDONGLECONN", app_check_dongle_connected,         "" },
#endif
    { "AT^I2CWRITE", app_i2c_write,                 "" },
    { "AT^I2CREAD",  app_i2c_read,                  "" },
#ifdef SUPPORT_ALIPAY_SEC
    { "AT^ALIPAYSECTEST",  app_alipay_sec_test,              "" },
    { "AT^ALIPAYDEBUG",    app_alipay_debug,                 "" },
    { "AT^ALIPAYVERIFY",   app_alipay_pl_verify,             "" },
    { "AT^ALIPAYRESET",    app_alipay_reset,                 "" },
#endif
#ifdef SUPPORT_FILE_SPLIT_COMBINE
    { "AT^FILESPLIT",    app_file_split_test,              "" }, /* 注意，执行此操作前提是传入待分隔文件 */
    { "AT^FILECOMBINE",  app_file_combine_test,            "" },
#endif
#ifdef JS_ENABLE
    { "AT^GETTRANSCHAN",  app_get_trans_channel_test,      "" },
    { "AT^SETTRANSCHAN",  app_set_trans_channel_test,      "" },
#endif
#ifdef SUPPORT_AUTO_OTA
    { "AT^AUTOOTABLOCK",   app_auto_ota_block,      "" },
    { "AT^AUTOOTARUN",     app_auto_ota_run,        "" },
    { "AT^AUTOOTADELMAC",  app_auto_ota_del_mac,    "" },
    { "AT^AUTOOTAADDMAC",  app_auto_ota_add_mac,    "" },
    { "AT^AUTOOTASHOWMAC", app_auto_ota_show_mac,   "" },
#endif
#ifdef LOSCFG_KERNEL_PERF
    { "AT^PERFSTART", app_perf_start,   "" },
    { "AT^PERFSTOP", app_perf_stop,   "" },
#endif
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    { "AT^SCHEDSTART", app_sched_statistis_start,   "" },
    { "AT^SCHEDSTOP",  app_sched_statistis_stop,   "" },
#endif
};

at_cmd_table_t *get_app_cmd_table(uint32_t *cmd_count)
{
    *cmd_count = ARRAY_COUNT(g_app_cmd);

    return (at_cmd_table_t *)g_app_cmd;
}
