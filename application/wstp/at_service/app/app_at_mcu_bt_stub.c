/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Minimal AT helpers for brandy-mcu-bt (no GUI/LCD)
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "securec.h"
#include "errcode.h"
#include "uart.h"
#include "hal_uart.h"
#include "app_at_process.h"
#include "diag_service.h"
#include "app_cmd_manager.h"
#include "app_rx_process.h"
#include "soc_osal.h"
#include "debug_print.h"
#include "at_cmd_api.h"

#ifndef BIT7
#define BIT7 0x80
#endif

#ifndef WORD_WIDTH
#define WORD_WIDTH 4
#endif

void wstp_print(const char *str, ...)
{
    static uint8_t s[256];
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }

    if (hal_uart_get_funcs(SW_DEBUG_UART_BUS) == NULL) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, (uint32_t)str_len, 0);
}

void factory_test_print(const uint8_t return_code, const char *str, ...)
{
    unused(return_code);
    static uint8_t s[256];
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len > 0) {
        wstp_print("[Factory]|%s.\r\n", s);
    }
}

void print_at_cmd_ret(int ret)
{
    uint32_t irq_sts = osal_irq_lock();
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\nOK~\r\n");
    } else {
        wstp_print("\r\nERR~\r\n");
    }
    osal_irq_restore(irq_sts);
}

uint32_t parse_tlv_2value(uint8_t *data, uint16_t data_len, uint8_t **out, uint32_t *out_len)
{
    unused(data_len);
    uint8_t num[WORD_WIDTH] = {0};
    uint8_t *ptr = data;
    uint8_t len;
    uint32_t length = 0;
    int i;

    ptr++;
    len = *(uint8_t *)ptr++;
    if (((len & BIT7) >> 7) == 1) {
        length = len & (~BIT7);
    } else {
        for (i = 0; i < len; i++) {
            num[i] = *ptr++;
        }
        length = *(uint32_t *)num;
    }
    *out = ptr;
    *out_len = length;
    return ERRCODE_SUCC;
}

uint32_t mcu_diag_cmd_process(diag_ser_data_t *data)
{
    unused(data);
    return ERRCODE_FAIL;
}

void app_at_process(uint8_t *data, uint32_t data_len)
{
    unused(data);
    unused(data_len);
}

at_cmd_table_t *get_app_cmd_table(uint32_t *cmd_count)
{
    if (cmd_count != NULL) {
        *cmd_count = 0;
    }
    return NULL;
}

void app_at_task_process_handler(void *param)
{
    unused(param);
}

uint32_t send_spp_diag_msg(bool send, diag_ser_header_t *data, uint8_t *payload, uint32_t pay_len)
{
    unused(send);
    unused(data);
    unused(payload);
    unused(pay_len);
    return ERRCODE_FAIL;
}

void uapi_self_heal_timer_enable(void)
{
}
