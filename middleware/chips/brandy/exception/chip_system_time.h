/**
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: time
 * Author:
 * Create:
 */
#ifndef CHIP_SYSTEM_TIME_H
#define CHIP_SYSTEM_TIME_H

#ifndef BUILD_APPLICATION_SSB
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "calendar.h"
#include "chip_io.h"
#include "errcode.h"
#include "time.h"
#include "sys/time.h"
/**
 * @brief  Print the time represented by the given calendar date
 * @return void.
 */
void print_time(calendar_t *date);

/**
 * @brief  Save the current system time to non-volatile memory
 * @return void.
 */
void save_time(void);

/**
 * @brief  Get the current system time as a Unix timestamp
 * @return void.
 */
uint32_t get_current_timestamp(void);

/**
 * @brief  Get the current system time as a timeval struct and a tm
 * @return void.
 */
void get_current_time(struct timeval *tv, struct tm *tm);

/**
 * @brief  Restore the system time from the saved non-volatile
 * @return void.
 */
void restore_time(void);

/**
 * @brief  Synchronize the system time with the given timestamp and timezone
 * @return void.
 */
uint32_t sync_time(uint32_t timestamp, uint32_t timezone);

/**
 * @brief  use for test
 * @return void.
 */
void test_time(void);

/**
 * @brief  Get the current system time as a local timestamp with timezone offset
 * @return void.
 */
uint32_t get_local_timestamp(void);
#endif
#endif