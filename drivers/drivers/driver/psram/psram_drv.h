/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides psram driver API \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-07, Create file. \n
 */
#ifndef PSRAM_DRV_H
#define PSRAM_DRV_H

#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_driver_psram PSRAM
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief  Support PSRAM device.
 * @else
 * @brief  支持的PSRAM
 * @endif
 */
typedef enum {
    PSRAM_DEVICE_AP_X16 = 0,
    PSRAM_DEVICE_WINBON_X8 = 1,
    PSRAM_DEVICE_AP_X8 = 2,
    PSRAM_DEVICE_MAX_NUM,
    PSRAM_DEVICE_NONE = PSRAM_DEVICE_MAX_NUM
} psram_device_e;

/**
 * @if Eng
 * @brief  Psram drvie strength.
 * @else
 * @brief  Psram 驱动能力表
 * @endif
 */
typedef enum {                  /*!< AP                      winbon */
    PSRAM_DRIVER_0 = 0,         /*!< Full (25 ohm)           34 ohms (default) */
    PSRAM_DRIVER_1 = 1,         /*!< Half (50ohm default)    115 ohms */
    PSRAM_DRIVER_2 = 2,         /*!< 1/4 (100ohm)            67 ohms */
    PSRAM_DRIVER_3 = 3,         /*!< 1/8 (200ohm)            46 ohms */
    PSRAM_DRIVER_4 = 4,         /*!<                         34 ohms */
    PSRAM_DRIVER_5 = 5,         /*!<                         27 ohms */
    PSRAM_DRIVER_6 = 6,         /*!<                         22 ohms */
    PSRAM_DRIVER_7 = 7,         /*!<                         19 ohms */
    PSRAM_DRIVER_MAX_NUM = 8,
    PSRAM_DRIVER_NONE = PSRAM_DRIVER_MAX_NUM,
} psram_drivers_e;

/**
 * @if Eng
 * @brief  Psram clock frequence.
 * @else
 * @brief  Psram 时钟频率
 * @endif
 */
#if OPI_USE_MCU_HS_CLK == NO
typedef enum {
    PSRAM_50MHZ = 0,
    PSRAM_75MHZ = 1,
    PSRAM_100MHZ = 2,
    PSRAM_133MHZ = 3,
    PSRAM_157MHZ = 4,
    PSRAM_159MHZ = 5,
    PSRAM_162MHZ = 6,
    PSRAM_166MHZ = 7,
    PSRAM_168MHZ = 8,
    PSRAM_169MHZ = 9,
    PSRAM_170MHZ = 10,
    PSRAM_171MHZ = 11,
    PSRAM_MAX,
} psram_clk_e;
#else
typedef enum {
    PSRAM_114MHZ,
    PSRAM_57MHZ,
    PSRAM_98MHZ,
    PSRAM_65MHZ,
    PSRAM_MAX,
} psram_clk_e;
#endif

/**
 * @if Eng
 * @brief  Init PSRAM, config psram parameter.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  重置psram, 配置初始参数
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_init(void);

/**
 * @if Eng
 * @brief  Reset psram.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  重置psram
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_reset(void);

/**
 * @if Eng
 * @brief  Write psram register parameter.
 * @param  [in]  addr Psram register number.
 * @param  [in]  value Parameter value.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  配置psram寄存器参数
 * @param  [in]  addr Psram寄存器编号
 * @param  [in]  value 参数值.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_write_reg(uint32_t addr, uint32_t value);

/**
 * @if Eng
 * @brief  Read psram register parameter.
 * @param  [in]  addr Psram register number.
 * @param  [in]  value Pointer to the parameter value.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  配置psram寄存器参数
 * @param  [in]  addr Psram寄存器编号
 * @param  [in]  value 存储参数值的Buffer.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_read_reg(uint32_t addr, uint32_t *value);

/**
 * @if Eng
 * @brief  Set Psram enter XIP mode.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  切Psram到XIP模式
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_enter_xip(void);

/**
 * @if Eng
 * @brief  Set Psram exit XIP mode.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  切Psram退出XIP模式
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_exit_xip(void);

/**
 * @if Eng
 * @brief  Write psram memory by apb.
 * @param  [in]  address Psram memory address.
 * @param  [in]  buffer Pointer to the buffer to write to psram.
 * @param  [in]  buffer_length Length of the buffer to write.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  写psram内存
 * @param  [in]  address Psram内存地址
 * @param  [in]  buffer 要写给psram的数据Buffer
 * @param  [in]  buffer_length 要写的数据的长度
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_write_memory(uint32_t address, uint32_t *buffer, uint32_t buffer_length);

/**
 * @if Eng
 * @brief  Read psram memory by apb.
 * @param  [in]  address Psram memory address.
 * @param  [in]  buffer Pointer to the buffer to read from psram.
 * @param  [in]  buffer_length Length of the buffer to read.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  写psram内存
 * @param  [in]  address Psram内存地址
 * @param  [in]  buffer 存取从psram读取数据的Buffer
 * @param  [in]  buffer_length 要读取的数据长度
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_read_memory(uint32_t address, uint32_t *buffer, uint32_t buffer_length);

/**
 * @if Eng
 * @brief  Return psram in XIP mode or not.
 * @return Return true if psram in XIP mode, or return false.
 * @else
 * @brief  返回psram是否处于XIP模式
 * @return 如果是XIP模式则返回true，否则返回false
 * @endif
 */
bool uapi_psram_xip_enabled(void);

/**
 * @if Eng
 * @brief  Config psram driver strength, psram after init, driver is default value.
 * @param  [in]  driver Psram driver strength.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  配置psram驱动能力，psram初始化后，驱动能力为默认值
 * @param  [in]  driver psram驱动能力
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_config_driver(psram_drivers_e driver);

/**
 * @if Eng
 * @brief  Set psram enter sleep.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  psram进入睡眠模式
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_enter_sleep(void);

/**
 * @if Eng
 * @brief  Set psram exit sleep.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  psram退出睡眠模式
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_exit_sleep(void);

/**
 * @if Eng
 * @brief  Suspend psram xip before core enter deepsleep.Then suspend opi.
 * @else
 * @brief  退出psram xip模式，然后进入深睡模式，并暂停opi接口
 * @endif
 */
void uapi_psram_xip_suspend(void);

/**
 * @if Eng
 * @brief  Suspend opi, psram exit deepsleep then enter xip.
 * @else
 * @brief  恢复opi接口，psram退出深睡模式并切入xip
 * @endif
 */
void uapi_psram_xip_resume(void);

/**
 * @if Eng
 * @brief  Swtitch psram to high fre.
 * @param  [in]  sel Psram clock frequence.
 * @else
 * @brief  切psram时钟频率
 * @param  [in]  sel Psram时钟频率
 * @endif
 */
void uapi_psram_switch_to_high_fre(psram_clk_e sel);

/**
 * @if Eng
 * @brief  Read psram EID.
 * @param  [in]  id Psram eid param, uint16_t param[4].
 * @param  [in]  length Eid length, it should be 8 bytes.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  读取psram的EID值
 * @param  [in]  id Psram eid, 格式为uint16_t psram[4]
 * @param  [in]  length Eid长度，应为8字节
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_psram_read_eid(uint16_t *id, uint8_t length);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* PSRAM_DRV_H */
