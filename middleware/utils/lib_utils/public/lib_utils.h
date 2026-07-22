/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Exported string functions
 * Author:
 * Create:
 */

#ifndef LIB_UTILS_STRING_TIMEOUT_H
#define LIB_UTILS_STRING_TIMEOUT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * @defgroup connectivity_libs_utils UTILS
 * @ingroup  connectivity_libs
 * @{
 */
/**
 * @brief  Specified the seed of rand.
 * @param  seed The seed.
 */
void srand(unsigned int seed);

/**
 * @brief  Get a rand number.
 * @return int The rand.
 */
int rand(void);

/**
 * @brief  Compare two string.
 * @param  s1 String1.
 * @param  s2 String2.
 * @return int The result of this comparing.
 */
int stricmp(const char *s1, const char *s2);

/**
 * @brief  Convert a string to int.
 * @param  nptr The string.
 * @return int The result of this converting.
 */
int atoi(const char *nptr);

/**
 * @brief  Check the timeout periodic.
 * @param  timeout The value of timeout.
 * @param  start_time The start time.
 * @param  check_times Will do the checking foreach this times.
 * @return true Timeout happens.
 * @return false Not timeout.
 */
bool lib_timeout_check_timeout_periodic(uint64_t timeout, uint64_t start_time, uint32_t *check_times);

/**
 * @}
 */
#endif
