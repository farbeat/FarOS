/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides app common api func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef APP_COMMON_API_H
#define APP_COMMON_API_H

#include "stdint.h"

#define TIME_CONVERT            60
#define USEC_TO_MILLI           1000
#define FLASH_RESERVED_ADDR     (0x706E0000)
#define FLASH_RESERVED_OFFSET   (FLASH_RESERVED_ADDR - 0x70000000)
#define FLASH_RESERVED_LEN      (0x119000)

#ifndef SUPPORT_TEST_COMMON
typedef struct {
    char *func_name;
    char *func_description;
    int (*func)(int argc, char *argv[]);
} test_func;

int testsuit_msg_handle(char *buf, int buf_size);
int uapi_test_suite_add_function(char *name, char *description, int (*func)(int argc, char *argv[]));
int add_function(char *name, char *description, int (*func)(int argc, char *argv[]));
#endif
uint32_t get_sec_time(char* time);
uint32_t set_system_time(char* time);
uint32_t get_system_time(void);
uint32_t nandflash_aging_test(char* para);
uint32_t flash_aging_test(char* para);
uint32_t freertos_heap_print(void);
#endif