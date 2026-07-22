/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides I2C driver api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "common_def.h"
#include "errcode.h"
#include "hal_i2c.h"
#include "i2c_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_driver_i2c I2C
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of I2C TX/RX data.
 * @else
 * @brief  I2C发送/接收数据结构定义。
 * @endif
 */
typedef struct i2c_data {
    uint8_t *send_buf;              /*!< @if Eng Send buffer pointer.
                                         @else   发送数据的buffer指针。  @endif */
    uint32_t send_len;              /*!< @if Eng Send buffer len.
                                         @else   发送数据的buffer长度。  @endif */
    uint8_t *receive_buf;           /*!< @if Eng Receive buffer pointer.
                                         @else   接收数据的buffer指针。  @endif */
    uint32_t receive_len;           /*!< @if Eng Receive buffer pointer.
                                         @else   接收数据的buffer长度。  @endif */
} i2c_data_t;

/**
 * @if Eng
 * @brief  Definition of I2C TX/RX wait condition.
 * @else
 * @brief  I2C发送/接收等待条件数据结构定义。
 * @endif
 */
typedef struct i2c_wait_condition {
    uint32_t evt_mask;              /*!< @if Eng Wait event condition, will trigger task schedule if blocked.
                                         @else   等待事件条件，如果不成立会引起任务调度，释放cpu占用。  @endif */
    uint32_t ctrl_mask;             /*!< @if Eng Wait control condition, will polling if blocked.
                                         @else   等待控制条件，如果不成立一直轮询，不释放cpu占用。  @endif */
} i2c_wait_condition_t;

#if defined(CONFIG_I2C_SUPPORT_MASTER) && (CONFIG_I2C_SUPPORT_MASTER == 1)
/**
 * @if Eng
 * @brief  Initialize the I2C as master according to the specified parameters.
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  baudrate The baudrate of I2C, high limit by ip:
 *                        - Standard speed mode baudrate high limit : 100KHz
 *                        - Fast speed mode baudrate high limit : 400KHz
 *                        - High speed mode baudrate high limit : 3.4MHz
 * @param  [in]  hscode The I2C high speed mode master code, each master has its unique master code, \n
 *                      valid values 0 ~ 7, only configured in high speed mode.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  根据指定的参数初始化该I2C为主机。
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  baudrate I2C波特率，不能超过IP的上限：
 *                        - 标准模式波特率上限：100KHz
 *                        - 快速模式波特率上限：400KHz
 *                        - 高速模式波特率上限：3.4MHz
 * @param  [in]  hscode I2C高速模式主机码, 每个主机有自己唯一的主机码，有效取值范围 0 ~ 7，仅在高速模式下需要配置。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_master_init(i2c_bus_t bus, uint32_t baudrate, uint8_t hscode);

/**
 * @if Eng
 * @brief  I2C Master writes a buffer to target slave. There are three transfer modes:
 *         - Polling mode
 *         - DMA mode
 *         - Interrupt mode
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  dev_addr The target slave address for master to write data.
 * @param  [in]  data The information pointer of send data, see @ref i2c_data_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  I2C主机将数据发送到目标从机上。一共有三种传输模式：
 *         - 轮询模式
 *         - DMA模式
 *         - 中断模式
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  dev_addr 主机发送数据的目标从机地址。
 * @param  [in]  data 发送数据的信息指针，参考 @ref i2c_data_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_master_write(i2c_bus_t bus, uint16_t dev_addr, i2c_data_t *data);

/**
 * @if Eng
 * @brief  I2C Master receives a buffer from target slave. There are three transfer modes:
 *         - Polling mode
 *         - DMA mode
 *         - Interrupt mode
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  dev_addr The target slave address for master to receive data.
 * @param  [in]  data The information pointer of receive data, see @ref i2c_data_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  主机接收来自目标I2C从机的数据。一共有三种传输模式：
 *         - 轮询模式
 *         - DMA模式
 *         - 中断模式
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  dev_addr 主机接收数据的目标从机地址。
 * @param  [in]  data 接收数据的信息指针，参考 @ref i2c_data_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_master_read(i2c_bus_t bus, uint16_t dev_addr, i2c_data_t *data);

/**
 * @if Eng
 * @brief  I2C Master writes a buffer to target slave and receives a buffer from it in polling mode.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t.
 * @param  [in]  dev_addr The target slave address for master to receive data.
 * @param  [in]  data The information pointer of receive data, see @ref i2c_data_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  主机发送数据到目标I2C从机，并接收来自此从机的数据，使用轮询模式。
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  dev_addr 主机接收数据的目标从机地址。
 * @param  [in]  data 接收数据的信息指针，参考 @ref i2c_data_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_master_writeread(i2c_bus_t bus, uint16_t dev_addr, i2c_data_t *data);
#endif  /* CONFIG_I2C_SUPPORT_MASTER */

#if defined(CONFIG_I2C_SUPPORT_SLAVE) && (CONFIG_I2C_SUPPORT_SLAVE == 1)
/**
 * @if Eng
 * @brief  Initialize the I2C as slave according to the specified parameters.
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  baudrate The baudrate of I2C, need to be consistent with the master.
 * @param  [in]  addr The slave address when the I2C is operating as a slave:
 *                    - 7-bits address range is [0x8, 0x77]
 *                    - 10-bits address range is [0x7800, 0x7BFF]
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  根据指定的参数初始化该I2C为从机。
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  baudrate I2C波特率，需要与主机保持一致：
 *                        - 标准模式波特率上限：100KHz
 *                        - 快速模式波特率上限：400KHz
 *                        - 高速模式波特率上限：3.4MHz
 * @param  [in]  addr I2C作为从机工作时的从机地址：
 *                    - 7比特地址范围 [0x8, 0x77]
 *                    - 10比特地址范围 [0x7800, 0x7BFF]
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */

errcode_t uapi_i2c_slave_init(i2c_bus_t bus, uint32_t baudrate, uint16_t addr);

/**
 * @if Eng
 * @brief  Slave writes a buffer to Master. There are three transfer modes:
 *         - Polling mode
 *         - DMA mode
 *         - Interrupt mode
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  data The information pointer of send data, see @ref i2c_data_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  从机将数据发送给主机。一共有三种传输模式：
 *         - 轮询模式
 *         - DMA模式
 *         - 中断模式
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  data 发送数据的信息指针，参考 @ref i2c_data_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_slave_write(i2c_bus_t bus, i2c_data_t *data);

/**
 * @if Eng
 * @brief  Slave receives a buffer form Master. There are three transfer modes:
 *         - Polling mode
 *         - DMA mode
 *         - Interrupt mode
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  data The information pointer of receive data, see @ref i2c_data_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  从机接收来自主机的数据。一共有三种传输模式：
 *         - 轮询模式
 *         - DMA模式
 *         - 中断模式
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @param  [in]  data 接收数据的信息指针，参考 @ref i2c_data_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_slave_read(i2c_bus_t bus, i2c_data_t *data);
#endif /* CONFIG_I2C_SUPPORT_SLAVE */

#if defined(CONFIG_I2C_SUPPORT_INT) && (CONFIG_I2C_SUPPORT_INT == 1)
/**
 * @if Eng
 * @brief  Set whether to use the interrupt mode to transfer data.
 * @param  [in]  bus The I2C bus. For details, see @ref i2c_bus_t.
 * @param  [in]  irq_en Whether to use the interrupt mode.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  设置是否使用中断模式传输数据。
 * @param  [in]  bus 指定的I2C接口，参考 @ref i2c_bus_t 。
 * @param  [in]  irq_en 是否使用中断模式。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_set_irq_mode(i2c_bus_t bus, bool irq_en);
#endif  /* CONFIG_I2C_SUPPORT_INT */

#if defined(CONFIG_I2C_SUPPORT_DMA) && (CONFIG_I2C_SUPPORT_DMA == 1)
/**
 * @if Eng
 * @brief  Enable/disable data transfer function in dma mode.
 * @param  [in]  bus The I2C bus. For details, see @ref i2c_bus_t.
 * @param  [in]  en Enable/disable data transfer.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  使能/去使能DMA模式下I2C传输。
 * @param  [in]  bus 指定的I2C接口。参考 @ref i2c_bus_t 。
 * @param  [in]  en 是否使能DMA传输。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_set_dma_mode(i2c_bus_t bus, bool en);
#endif  /* CONFIG_I2C_SUPPORT_DMA */

/**
 * @if Eng
 * @brief  Deinitialize the I2C peripheral, valid for both master and slave mode.
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  去初始化I2C，支持主从机。
 * @param  [in]  bus I2C总线，参考 @ref i2c_bus_t 。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_deinit(i2c_bus_t bus);

/**
 * @if Eng
 * @brief  Reset I2C baudrate for initialized I2C, valid for both master and slave mode.
 * @param  [in]  bus The I2C bus, see @ref i2c_bus_t.
 * @param  [in]  baudrate The baudrate of I2C, high limit by ip:
 *                        - Standard speed mode baudrate high limit : 100KHz
 *                        - Fast speed mode baudrate high limit : 400KHz
 *                        - High speed mode baudrate high limit : 3.4MHz
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  对已初始化的I2C重置波特率，支持主从机。
 * @param  [in]  bus I2C总线id，参考 @ref i2c_bus_t 。
 * @param  [in]  baudrate I2C波特率，不能超过IP的上限：
 *                        - 标准模式波特率上限：100KHz
 *                        - 快速模式波特率上限：400KHz
 *                        - 高速模式波特率上限：3.4MHz
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_set_baudrate(i2c_bus_t bus, uint32_t baudrate);

#if defined(CONFIG_I2C_SUPPORT_LPM)
/**
 * @if Eng
 * @brief  Suspend all of the I2C channels.
 * @param  [in]  arg Argument for suspend.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  挂起所有I2C通道。
 * @param  [in]  arg 挂起所需要的参数。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_suspend(uintptr_t arg);

/**
 * @if Eng
 * @brief  Resume all of the I2C channels.
 * @param  [in]  arg Argument for resume.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  恢复所有I2C通道。
 * @param  [in]  arg 恢复所需要的参数。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t uapi_i2c_resume(uintptr_t arg);
#endif

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif