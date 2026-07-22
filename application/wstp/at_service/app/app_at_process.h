/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd register func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef APP_AT_SERVICE_H
#define APP_AT_SERVICE_H

#include <stdint.h>
#include <stdio.h>
#include "errcode.h"
#include "soc_osal.h"
#include "errcode.h"
#include "diag_service.h"
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef uint32_t (*app_process_func)(uint8_t *para, uint32_t para_len);

typedef struct {
    uint8_t id;
    app_process_func func;
} app_process_table;

typedef enum aes_crypto {
    AES_DECRYPT_TEST,
    AES_ENCRYPT_TEST,
} aes_cypto_t;

#define FT_PAYLOAD_LEN_MAX 64
#define FT_RETURN_SUCC 0
#define FT_RETURN_FAIL 1

typedef struct {
    uint8_t return_code;
    uint8_t payload_len;
    uint8_t payload[FT_PAYLOAD_LEN_MAX];
} factory_print_table;

void app_at_process(uint8_t *data, uint32_t data_len);
#ifdef CFG_DRIVERS_NANDFLASH
void erase_all(struct nand_driver_struct  *driver);
#endif

uint32_t mcu_diag_cmd_process(diag_ser_data_t *data);
uint32_t send_spp_diag_msg(bool send, diag_ser_header_t *data, uint8_t* payload, uint32_t pay_len);
uint32_t parse_tlv_2value(uint8_t* data, uint16_t data_len, uint8_t** out, uint32_t *out_len);
void wstp_print(const char *str, ...);
void factory_test_print(const uint8_t return_code, const char *str, ...);
void print_at_cmd_ret(int ret);

/* 外部声明 */
extern void NotifyScreenOn(void);
extern void NotifyScreenOff(void);
extern void low_power_control1_power_on_display(void);
extern void touch_screen_resume(void);
extern uint32_t mcu_pm_lcd_on(uint8_t *para, uint32_t para_len);
extern uint32_t mcu_pm_lcd_off(uint8_t *para, uint32_t para_len);
extern errcode_t upg_flash_write(const uint32_t flash_offset, uint32_t size, const uint8_t *ram_data, bool do_erase);
extern uint32_t mcu_pm_lcd_on(uint8_t *para, uint32_t para_len);
extern uint32_t mcu_pm_lcd_off(uint8_t *para, uint32_t para_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif