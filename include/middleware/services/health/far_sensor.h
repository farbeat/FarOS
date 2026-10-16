/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides sensor abstraction layer API for FarOS Health Service. \n
 *              Supports IMU (accelerometer + gyroscope) and PPG sensors. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_SENSOR_H
#define FAR_SENSOR_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "far_health_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup health_sensor Sensor Abstraction Layer
 * @ingroup  health_service
 * @brief    传感器抽象层，提供IMU（加速度计+陀螺仪）和PPG（光电容积脉搏波）传感器的统一访问接口。
 *
 * @if Eng
 * Sensor abstraction layer providing unified access to IMU (accelerometer +
 * gyroscope) and PPG (photoplethysmography) sensors for health algorithms.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  IMU data sample structure (accelerometer + gyroscope).
 * @else
 * @brief  IMU数据采样结构体（加速度计+陀螺仪）。
 * @endif
 */
typedef struct {
    int16_t ax;         /*!< @if Eng X-axis acceleration, unit: mg (milli-g).
                             @else   X轴加速度，单位：mg（千分之一重力加速度）。 @endif */
    int16_t ay;         /*!< @if Eng Y-axis acceleration, unit: mg.
                             @else   Y轴加速度，单位：mg。 @endif */
    int16_t az;         /*!< @if Eng Z-axis acceleration, unit: mg.
                             @else   Z轴加速度，单位：mg。 @endif */
    int16_t gx;         /*!< @if Eng X-axis angular velocity, unit: mdps (milli-deg/s).
                             @else   X轴角速度，单位：mdps（毫度/秒）。 @endif */
    int16_t gy;         /*!< @if Eng Y-axis angular velocity, unit: mdps.
                             @else   Y轴角速度，单位：mdps。 @endif */
    int16_t gz;         /*!< @if Eng Z-axis angular velocity, unit: mdps.
                             @else   Z轴角速度，单位：mdps。 @endif */
    uint32_t timestamp; /*!< @if Eng Sample timestamp in milliseconds.
                             @else   采样时间戳，单位：毫秒。 @endif */
} far_sensor_imu_data_t;

/**
 * @if Eng
 * @brief  PPG raw data sample structure.
 * @else
 * @brief  PPG原始数据采样结构体。
 * @endif
 */
typedef struct {
    int32_t green_raw;      /*!< @if Eng Green LED raw ADC value (primary HR channel).
                                 @else   绿光LED原始ADC值（主心率通道）。 @endif */
    int32_t ir_raw;         /*!< @if Eng IR LED raw ADC value (SpO2/HR channel).
                                 @else   红外LED原始ADC值（血氧/心率通道）。 @endif */
    int32_t red_raw;        /*!< @if Eng Red LED raw ADC value (SpO2 channel).
                                 @else   红光LED原始ADC值（血氧通道）。 @endif */
    uint32_t timestamp;     /*!< @if Eng Sample timestamp in milliseconds.
                                 @else   采样时间戳，单位：毫秒。 @endif */
} far_sensor_ppg_data_t;

/**
 * @if Eng
 * @brief  Sensor sample rate enumeration.
 * @else
 * @brief  传感器采样率枚举。
 * @endif
 */
typedef enum {
    FAR_SENSOR_RATE_12_5_HZ  = 0,   /*!< @if Eng 12.5 Hz - Ultra low power.
                                         @else   12.5 Hz - 超低功耗模式。 @endif */
    FAR_SENSOR_RATE_25_HZ    = 1,   /*!< @if Eng 25 Hz - Low power.
                                         @else   25 Hz - 低功耗模式。 @endif */
    FAR_SENSOR_RATE_50_HZ    = 2,   /*!< @if Eng 50 Hz - Standard.
                                         @else   50 Hz - 标准模式。 @endif */
    FAR_SENSOR_RATE_100_HZ   = 3,   /*!< @if Eng 100 Hz - High precision.
                                         @else   100 Hz - 高精度模式。 @endif */
    FAR_SENSOR_RATE_200_HZ   = 4,   /*!< @if Eng 200 Hz - High frequency.
                                         @else   200 Hz - 高频模式。 @endif */
    FAR_SENSOR_RATE_400_HZ   = 5,   /*!< @if Eng 400 Hz - Maximum precision.
                                         @else   400 Hz - 最高精度模式。 @endif */
} far_sensor_rate_t;

/**
 * @if Eng
 * @brief  Sensor operating mode enumeration.
 * @else
 * @brief  传感器工作模式枚举。
 * @endif
 */
typedef enum {
    FAR_SENSOR_MODE_LOW_POWER   = 0,   /*!< @if Eng Low power mode, reduced accuracy.
                                            @else   低功耗模式，降低精度。 @endif */
    FAR_SENSOR_MODE_NORMAL      = 1,   /*!< @if Eng Normal mode, balanced power/accuracy.
                                            @else   标准模式，功耗与精度平衡。 @endif */
    FAR_SENSOR_MODE_HIGH_PERF   = 2,   /*!< @if Eng High performance mode, maximum accuracy.
                                            @else   高性能模式，最大精度。 @endif */
} far_sensor_mode_t;

/**
 * @if Eng
 * @brief  IMU sensor configuration structure.
 * @else
 * @brief  IMU传感器配置结构体。
 * @endif
 */
typedef struct {
    far_sensor_rate_t  accel_rate;     /*!< @if Eng Accelerometer sample rate.
                                            @else   加速度计采样率。 @endif */
    far_sensor_rate_t  gyro_rate;      /*!< @if Eng Gyroscope sample rate.
                                            @else   陀螺仪采样率。 @endif */
    far_sensor_mode_t  mode;           /*!< @if Eng Operating mode.
                                            @else   工作模式。 @endif */
    uint8_t            accel_range_g;  /*!< @if Eng Accelerometer full-scale range in g.
                                            @else   加速度计量程，单位：g。 @endif */
    uint16_t           gyro_range_dps; /*!< @if Eng Gyroscope full-scale range in dps.
                                            @else   陀螺仪量程，单位：dps。 @endif */
} far_sensor_imu_config_t;

/**
 * @if Eng
 * @brief  PPG sensor configuration structure.
 * @else
 * @brief  PPG传感器配置结构体。
 * @endif
 */
typedef struct {
    far_sensor_rate_t  sample_rate;    /*!< @if Eng PPG sample rate.
                                            @else   PPG采样率。 @endif */
    far_sensor_mode_t  mode;           /*!< @if Eng Operating mode.
                                            @else   工作模式。 @endif */
    uint8_t            led_current_ma; /*!< @if Eng LED drive current in mA.
                                            @else   LED驱动电流，单位：mA。 @endif */
    bool               enable_green;   /*!< @if Eng Enable green LED.
                                            @else   使能绿光LED。 @endif */
    bool               enable_ir;      /*!< @if Eng Enable IR LED.
                                            @else   使能红外LED。 @endif */
    bool               enable_red;     /*!< @if Eng Enable red LED.
                                            @else   使能红光LED。 @endif */
} far_sensor_ppg_config_t;

/**
 * @if Eng
 * @brief  Sensor data callback function type.
 * @param  [in] data      Pointer to sensor data (type depends on sensor).
 * @param  [in] len       Number of samples in the batch.
 * @param  [in] user_data User-defined pointer passed during registration.
 * @else
 * @brief  传感器数据回调函数类型。
 * @param  [in] data      传感器数据指针（类型取决于传感器）。
 * @param  [in] len       批次中的采样数量。
 * @param  [in] user_data 注册时传入的用户自定义指针。
 * @endif
 */
typedef void (*far_sensor_data_callback_t)(const void *data, uint32_t len, void *user_data);

/**
 * @if Eng
 * @brief  Initialize the IMU sensor subsystem.
 * @param  [in] config    Pointer to IMU configuration.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @retval ERRCODE_FAIL   Hardware initialization failed.
 * @else
 * @brief  初始化IMU传感器子系统。
 * @param  [in] config    IMU配置指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @retval ERRCODE_FAIL   硬件初始化失败。
 * @endif
 */
errcode_t uapi_sensor_imu_init(const far_sensor_imu_config_t *config);

/**
 * @if Eng
 * @brief  Deinitialize the IMU sensor subsystem.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化IMU传感器子系统。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sensor_imu_deinit(void);

/**
 * @if Eng
 * @brief  Read a batch of IMU data samples.
 * @param  [out] data     Buffer to store IMU samples.
 * @param  [in]  max_len  Maximum number of samples to read.
 * @param  [out] read_len Actual number of samples read.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  读取一批IMU数据采样。
 * @param  [out] data     存储IMU采样的缓冲区。
 * @param  [in]  max_len  最大读取采样数量。
 * @param  [out] read_len 实际读取的采样数量。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sensor_imu_read(far_sensor_imu_data_t *data, uint32_t max_len, uint32_t *read_len);

/**
 * @if Eng
 * @brief  Register IMU data callback for asynchronous data streaming.
 * @param  [in] callback  Callback function pointer.
 * @param  [in] user_data User-defined pointer.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  注册IMU数据回调，用于异步数据流。
 * @param  [in] callback  回调函数指针。
 * @param  [in] user_data 用户自定义指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sensor_imu_register_callback(far_sensor_data_callback_t callback, void *user_data);

/**
 * @if Eng
 * @brief  Unregister IMU data callback.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注销IMU数据回调。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sensor_imu_unregister_callback(void);

/**
 * @if Eng
 * @brief  Initialize the PPG sensor subsystem.
 * @param  [in] config    Pointer to PPG configuration.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @retval ERRCODE_FAIL   Hardware initialization failed.
 * @else
 * @brief  初始化PPG传感器子系统。
 * @param  [in] config    PPG配置指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @retval ERRCODE_FAIL   硬件初始化失败。
 * @endif
 */
errcode_t uapi_sensor_ppg_init(const far_sensor_ppg_config_t *config);

/**
 * @if Eng
 * @brief  Deinitialize the PPG sensor subsystem.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化PPG传感器子系统。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sensor_ppg_deinit(void);

/**
 * @if Eng
 * @brief  Read a batch of PPG raw data samples.
 * @param  [out] data     Buffer to store PPG samples.
 * @param  [in]  max_len  Maximum number of samples to read.
 * @param  [out] read_len Actual number of samples read.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  读取一批PPG原始数据采样。
 * @param  [out] data     存储PPG采样的缓冲区。
 * @param  [in]  max_len  最大读取采样数量。
 * @param  [out] read_len 实际读取的采样数量。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sensor_ppg_read(far_sensor_ppg_data_t *data, uint32_t max_len, uint32_t *read_len);

/**
 * @if Eng
 * @brief  Register PPG data callback for asynchronous data streaming.
 * @param  [in] callback  Callback function pointer.
 * @param  [in] user_data User-defined pointer.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  注册PPG数据回调，用于异步数据流。
 * @param  [in] callback  回调函数指针。
 * @param  [in] user_data 用户自定义指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sensor_ppg_register_callback(far_sensor_data_callback_t callback, void *user_data);

/**
 * @if Eng
 * @brief  Unregister PPG data callback.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注销PPG数据回调。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_sensor_ppg_unregister_callback(void);

/**
 * @if Eng
 * @brief  Set sensor sample rate dynamically.
 * @param  [in] sensor_type 0=IMU, 1=PPG.
 * @param  [in] rate        Target sample rate.
 * @retval ERRCODE_SUCC     Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  动态设置传感器采样率。
 * @param  [in] sensor_type 0=IMU，1=PPG。
 * @param  [in] rate        目标采样率。
 * @retval ERRCODE_SUCC     成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_sensor_set_sample_rate(uint8_t sensor_type, far_sensor_rate_t rate);

/**
 * @if Eng
 * @brief  Enable or disable a sensor.
 * @param  [in] sensor_type 0=IMU, 1=PPG.
 * @param  [in] enable      true=enable, false=disable.
 * @retval ERRCODE_SUCC     Success.
 * @else
 * @brief  使能或禁用传感器。
 * @param  [in] sensor_type 0=IMU，1=PPG。
 * @param  [in] enable      true=使能，false=禁用。
 * @retval ERRCODE_SUCC     成功。
 * @endif
 */
errcode_t uapi_sensor_enable(uint8_t sensor_type, bool enable);

/**
 * @if Eng
 * @brief  Get sensor power consumption estimate.
 * @param  [in] sensor_type 0=IMU, 1=PPG.
 * @param  [out] power_ua   Estimated power consumption in microamperes.
 * @retval ERRCODE_SUCC     Success.
 * @else
 * @brief  获取传感器功耗估算值。
 * @param  [in] sensor_type 0=IMU，1=PPG。
 * @param  [out] power_ua   估算功耗，单位：微安。
 * @retval ERRCODE_SUCC     成功。
 * @endif
 */
errcode_t uapi_sensor_get_power_estimate(uint8_t sensor_type, uint32_t *power_ua);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_SENSOR_H */
