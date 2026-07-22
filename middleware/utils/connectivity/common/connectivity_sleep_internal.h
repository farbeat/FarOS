/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: System sleep internal manager header.
 * Author:
 * Create:  2021-3-1
 */
#ifndef CONNECTIVITY_SLEEP_INTERNAL_H
#define CONNECTIVITY_SLEEP_INTERNAL_H

#include <stdint.h>
#include <stdbool.h>

/** @defgroup connectivity_libs_connectivity_sleep_internal CONNECTIVITY Sleep Internal
  * @ingroup  connectivity_libs_connectivity
  * @{
  */
/**
 * @brief  Get property expected sleep time.
 * @return Value of expected sleep time.
 */
uint32_t connectivity_expected_sleep_time_get(void);

/**
 * @brief  Set property expected sleep time.
 */
void connectivity_expected_sleep_time_set(void);

/**
 * @brief  Get property time of entry sleep.
 * @return Value of the time entry sleep.
 */
uint64_t connectivity_entry_sleep_time_get(void);

/**
 * @brief  Set property time of entry sleep.
 * @param  val Value to be set.
 */
void connectivity_entry_sleep_time_set(uint64_t val);

/**
 * @brief  Get property is system sleep.
 * @return Value of is system sleep.
 */
bool connectivity_sys_is_sleep_get(void);

/**
 * @brief  Set property is system sleep.
 * @param  val Value to be set.
 */
void connectivity_sys_is_sleep_set(bool val);

/**
 * @brief  Change systick count to rtc ms.
 * @param  systick_count Systick count.
 * @return Ms result.
 */
uint32_t connectivity_systick_count_to_rtc_ms(uint32_t systick_count);

/**
 * @brief  System wakeup callback.
 */
void connectivity_sys_wakeup(void);

/**
 * @brief OS systick time compensation
 * @param sleep_ticks Predicted sleep ticks.
 */
void connectivity_sys_sleep_time_compensation(uint32_t sleep_ticks);
/**
  * @}
  */
#endif