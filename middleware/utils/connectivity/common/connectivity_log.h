/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: logging interface header
 * Author:
 * Create:
 */
#ifndef LIB_CONNECTIVITY_LOG_H
#define LIB_CONNECTIVITY_LOG_H

#include "error_types.h"
#include "chip_core_definition.h"
#include "platform_types.h"
#include "log_types.h"
#ifdef SUPPORT_IPC
#include "ipc_actions.h"
#endif
#include <stdint.h>
#include "platform_core.h"

/** @defgroup connectivity_libs_connectivity_log CONNECTIVITY Log
  * @ingroup  connectivity_libs_connectivity
  * @{
  */
#if CORE == APPS
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_APP_DEFAULT_CONFIG
#elif CORE == BT
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_BT_DEFAULT_CONFIG
#elif CORE == GNSS
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_GNSS_DEFAULT_CONFIG
#else
#define DEFAULT_LOG_LEVEL   LOG_LEVEL_DSP_DEFAULT_CONFIG
#endif

#define LOG_LEVEL_MAGIC     0xABABUL
/*
 * save 32-bit log level in sharemem, which is combined with 2 parts
 * +       16 Bits       +    16 Bits    +
 * +---------------------+---------------+
 * |   LOG_LEVEL_MAGIC   |   log_level_e   |
 * +---------------------+---------------+
 */
#define get_log_level_magic(mgc_log_lvl)   ((mgc_log_lvl) >> 16)
#define set_log_level_magic(log_lvl)       ((LOG_LEVEL_MAGIC << 16) | (log_lvl))
#define get_real_log_level(mgc_log_lvl)    ((mgc_log_lvl) & 0xFFFF)

typedef struct {
    volatile uint32_t app_core_level;
    volatile uint32_t bt_core_level;
#if CORE_NUMS > 2
    volatile uint32_t dsp_core_level;
#endif
#if CHIP_LIBRA
    volatile uint32_t gnss_core_level;
    volatile uint32_t sec_core_level;
#endif
} log_level_config_t;

typedef struct {
    volatile uint32_t app_core_mode;
    volatile uint32_t bt_core_mode;
#if CORE_NUMS > 2
    volatile uint32_t dsp_core_mode;
#endif
#if CHIP_LIBRA
    volatile uint32_t gnss_core_mode;
    volatile uint32_t sec_core_mode;
#endif
} log_mode_config_t;

#if CHIP_LIBRA
typedef struct {
    log_level_config_t log_level;       /* core level */
    log_mode_config_t log_mode;         /* core mode */
    volatile uint64_t log_time;
    volatile uint32_t dsp0_power_flag;
    volatile uint32_t gnss_power_flag;
    volatile uint32_t bt_power_flag;
    volatile uint32_t sec_power_flag;
} system_config_t;
#else
typedef struct {
    log_level_config_t log_level;
    log_mode_config_t log_mode;
    volatile uint64_t log_time;
#if CORE_NUMS > 2
    volatile uint32_t dsp0_power_flag;
#endif
#if DUAL_DSP_EXIST == YES
    volatile uint32_t dsp1_power_flag;
#endif
    volatile uint32_t bt_power_flag;
    volatile uint32_t reserved[5];  // The size of the reserved array n is 5, total 20 bytes.
} system_config_t;
#endif

typedef enum {
    LOG_MODE_SDT_LOG_BY_UART,
    LOG_MODE_COMPRESS_LOG_IN_FLASH,
    LOG_MODE_MAX_NUM,
} log_mode_t;

typedef enum {
    SLAVE_CPU_POWER_ON,
    SLAVE_CPU_POWER_OFF,
    SLAVE_CPU_POWER_MAX_NUM,
} slave_cpu_power_mode_t;

/**
 * @brief  Init the log system
 * @return None
 */
void init_log_level(void);

/**
 * @brief  Set Log Level For Print
 * @param  core  pointer to which core to set, security protocol ...
 * @param  level The level need to be set.
 * @return RET_TYPE_OK or error code
 */
ret_type_e set_log_level(cores_t core, log_level_e level);

/**
 * @brief  Get Log Level, this interface is only used by the App core to get the log level of 3 cores,
 * for the other cores, can only get the current core's log level
 * @param  core  set the core ,which can be security protocol ...
 * @param  level get the level
 * @return RET_TYPE_OK or error code
 */
ret_type_e get_log_level(cores_t core, log_level_e *level);

/**
 * @brief  Set Log Mode, SDT or Compress Log
 * @param  core  pointer to which core to set
 * @param  mode The mode need to be set.
 * @return RET_TYPE_OK or error code
 */
ret_type_e set_log_mode(cores_t core, log_mode_t mode);

/**
 * @brief  Get Log Mode, SDT or Compress Log
 * @param  core  set the core ,which can be security protocol ...
 * @param  mode get the mode
 * @return RET_TYPE_OK or error code
 */
ret_type_e get_log_mode(cores_t core, log_mode_t* const mode);

/**
 * @brief  Send the hook start request to BT
 * @param  hook_type The type of hook action
 * @param  command  The command of hook
 * @param  length The length of command
 * @return RET_TYPE_OK or error code
 */
#ifdef SUPPORT_IPC
ret_type_e send_bt_hook_data(ipc_bt_ap_hook_type_e hook_type, const uint8_t *command, uint32_t length);
#endif
/**
 * @brief  Set log time in Sharemem since dsp often power on and off
 * @param  rtc_time_ms   the ms count in a day when chip start
 */
void log_set_sharemem_timestamp(uint64_t rtc_time_ms);

/**
 * @brief  Get log time in Sharemem when system start
 * @return uint64_t   the ms count in a day when chip start
 */
uint64_t log_get_sharemem_timestamp(void);

/**
 * @brief  Set slave cpu power on or not.
 * @param  slave_cpu  Set the slave cpu ,which needs to power on or not
 * @param  slave_mode Power on or not
 */
void set_slave_cpu_power_mode(slave_cpu_t slave_cpu, slave_cpu_power_mode_t slave_mode);

/**
 * @brief  Get slave cpu power on or not.
 * @param  slave_cpu  Get the slave cpu status
 * @return slave cpu power on or not
 */
slave_cpu_power_mode_t get_slave_cpu_power_mode(slave_cpu_t slave_cpu);

/**
  * @}
  */
#endif
