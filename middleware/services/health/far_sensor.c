/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Sensor abstraction layer implementation skeleton. \n
 *              Provides IMU and PPG sensor access for health algorithms. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_sensor.h"
#include "debug_print.h"
#include "securec.h"
#include "i2c.h"
#include "gpio.h"

/*
 * ============================================================
 * 传感器抽象层骨架实现说明
 * Skeleton Implementation Notes
 * ============================================================
 *
 * 本文件为传感器抽象层的骨架实现，包含函数框架和算法逻辑注释。
 * 实际传感器驱动需根据具体硬件（如BMI270、MAX30102等）进行填充。
 *
 * IMU传感器典型驱动流程：
 * 1. 通过I2C/SPI接口初始化传感器芯片
 * 2. 配置采样率、量程、工作模式
 * 3. 设置数据就绪中断（GPIO中断）
 * 4. 在中断服务程序中读取FIFO数据
 * 5. 将原始数据转换为标准单位（mg, mdps）
 *
 * PPG传感器典型驱动流程：
 * 1. 通过I2C接口初始化PPG芯片
 * 2. 配置LED电流、采样率、ADC分辨率
 * 3. 设置SpO2/HR模式
 * 4. 读取FIFO中的原始ADC值
 * 5. 进行DC消除和滤波预处理
 */

/* IMU sensor state */
typedef struct {
    bool initialized;
    bool enabled;
    far_sensor_imu_config_t config;
    far_sensor_data_callback_t callback;
    void *user_data;
    uint32_t sample_count;
} far_imu_state_t;

/* PPG sensor state */
typedef struct {
    bool initialized;
    bool enabled;
    far_sensor_ppg_config_t config;
    far_sensor_data_callback_t callback;
    void *user_data;
    uint32_t sample_count;
} far_ppg_state_t;

static far_imu_state_t g_imu_state = {0};
static far_ppg_state_t g_ppg_state = {0};

/* ============================================================
 * IMU Sensor APIs
 * ============================================================ */

errcode_t uapi_sensor_imu_init(const far_sensor_imu_config_t *config)
{
    if (config == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_imu_state.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memcpy_s(&g_imu_state.config, sizeof(far_sensor_imu_config_t),
                   config, sizeof(far_sensor_imu_config_t));

    /* TODO: 硬件初始化
     * 1. 通过I2C/SPI向IMU芯片（如BMI270）发送初始化序列
     *    - 软复位芯片
     *    - 配置电源模式（低噪声模式）
     *    - 配置加速度计：ODR=config->accel_rate, Range=config->accel_range_g
     *    - 配置陀螺仪：ODR=config->gyro_rate, Range=config->gyro_range_dps
     * 2. 配置FIFO模式（stream模式，watermark中断）
     * 3. 注册GPIO中断回调，数据就绪时触发读取
     * 4. 校准偏移（读取静态数据计算零偏）
     */

    g_imu_state.initialized = true;
    g_imu_state.enabled = false;
    g_imu_state.sample_count = 0;
    g_imu_state.callback = NULL;
    g_imu_state.user_data = NULL;

    PRINT("[far_sensor] IMU init: rate=%d, range=%dg\r\n",
          config->accel_rate, config->accel_range_g);
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_imu_deinit(void)
{
    if (!g_imu_state.initialized) {
        return ERRCODE_SUCC;
    }

    /* TODO: 硬件反初始化
     * 1. 禁用IMU数据中断
     * 2. 将芯片设置为 suspend 模式
     * 3. 关闭I2C/SPI通信
     */

    (void)memset_s(&g_imu_state, sizeof(g_imu_state), 0, sizeof(g_imu_state));
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_imu_read(far_sensor_imu_data_t *data, uint32_t max_len, uint32_t *read_len)
{
    if ((data == NULL) || (read_len == NULL) || (max_len == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    if (!g_imu_state.initialized) {
        return ERRCODE_FAIL;
    }

    *read_len = 0;

    /* TODO: 从IMU FIFO读取数据
     * 1. 查询FIFO帧计数
     * 2. 批量读取FIFO数据（加速度+陀螺仪6轴）
     * 3. 将原始LSB数据转换为物理单位：
     *    - 加速度: raw * (range_g * 1000 / 32768) -> mg
     *    - 陀螺仪: raw * (range_dps * 1000 / 32768) -> mdps
     * 4. 填充时间戳
     * 5. 返回实际读取的样本数
     */

    /* 骨架：模拟读取一个样本用于测试 */
    if (max_len > 0) {
        data[0].ax = 0;
        data[0].ay = 1000;    /* ~1g on Y axis (gravity) */
        data[0].az = 0;
        data[0].gx = 0;
        data[0].gy = 0;
        data[0].gz = 0;
        data[0].timestamp = 0; /* TODO: get system tick */
        *read_len = 1;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_imu_register_callback(far_sensor_data_callback_t callback, void *user_data)
{
    if (callback == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_imu_state.callback = callback;
    g_imu_state.user_data = user_data;
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_imu_unregister_callback(void)
{
    g_imu_state.callback = NULL;
    g_imu_state.user_data = NULL;
    return ERRCODE_SUCC;
}

/* ============================================================
 * PPG Sensor APIs
 * ============================================================ */

errcode_t uapi_sensor_ppg_init(const far_sensor_ppg_config_t *config)
{
    if (config == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    if (g_ppg_state.initialized) {
        return ERRCODE_SUCC;
    }

    (void)memcpy_s(&g_ppg_state.config, sizeof(far_sensor_ppg_config_t),
                   config, sizeof(far_sensor_ppg_config_t));

    /* TODO: 硬件初始化
     * 1. 通过I2C向PPG芯片（如MAX30102/VC52）发送初始化序列
     *    - 复位芯片
     *    - 配置SpO2模式（HR+SpO2）或仅HR模式
     *    - 配置LED电流: IR=config->led_current_ma, Red=config->led_current_ma
     *    - 配置采样率: SPO2_ADC_RANGE=4096nA, SPO2_SR=config->sample_rate
     * 2. 配置FIFO：满阈值中断、溢出处理
     * 3. 注册GPIO中断（INT引脚）
     * 4. 启动温度补偿（用于DC漂移校正）
     */

    g_ppg_state.initialized = true;
    g_ppg_state.enabled = false;
    g_ppg_state.sample_count = 0;
    g_ppg_state.callback = NULL;
    g_ppg_state.user_data = NULL;

    PRINT("[far_sensor] PPG init: rate=%d, led=%dmA\r\n",
          config->sample_rate, config->led_current_ma);
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_ppg_deinit(void)
{
    if (!g_ppg_state.initialized) {
        return ERRCODE_SUCC;
    }

    /* TODO: 硬件反初始化
     * 1. 禁用PPG中断
     * 2. 关闭LED驱动
     * 3. 进入shutdown模式
     */

    (void)memset_s(&g_ppg_state, sizeof(g_ppg_state), 0, sizeof(g_ppg_state));
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_ppg_read(far_sensor_ppg_data_t *data, uint32_t max_len, uint32_t *read_len)
{
    if ((data == NULL) || (read_len == NULL) || (max_len == 0)) {
        return ERRCODE_INVALID_PARAM;
    }

    if (!g_ppg_state.initialized) {
        return ERRCODE_FAIL;
    }

    *read_len = 0;

    /* TODO: 从PPG FIFO读取数据
     * 1. 查询FIFO写指针、读指针
     * 2. 计算可用样本数
     * 3. 批量读取FIFO（每样本3字节x3LED=9字节）
     * 4. 重组18位ADC值
     * 5. 填充 green_raw / ir_raw / red_raw
     */

    /* 骨架：模拟读取一个样本 */
    if (max_len > 0) {
        data[0].green_raw = 50000;  /* Typical PPG amplitude */
        data[0].ir_raw = 80000;
        data[0].red_raw = 45000;
        data[0].timestamp = 0;
        *read_len = 1;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_ppg_register_callback(far_sensor_data_callback_t callback, void *user_data)
{
    if (callback == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    g_ppg_state.callback = callback;
    g_ppg_state.user_data = user_data;
    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_ppg_unregister_callback(void)
{
    g_ppg_state.callback = NULL;
    g_ppg_state.user_data = NULL;
    return ERRCODE_SUCC;
}

/* ============================================================
 * Common Sensor APIs
 * ============================================================ */

errcode_t uapi_sensor_set_sample_rate(uint8_t sensor_type, far_sensor_rate_t rate)
{
    if (rate > FAR_SENSOR_RATE_400_HZ) {
        return ERRCODE_INVALID_PARAM;
    }

    if (sensor_type == 0) { /* IMU */
        if (!g_imu_state.initialized) {
            return ERRCODE_FAIL;
        }
        /* TODO: 动态修改IMU ODR寄存器 */
        g_imu_state.config.accel_rate = rate;
        g_imu_state.config.gyro_rate = rate;
    } else if (sensor_type == 1) { /* PPG */
        if (!g_ppg_state.initialized) {
            return ERRCODE_FAIL;
        }
        /* TODO: 动态修改PPG采样率寄存器 */
        g_ppg_state.config.sample_rate = rate;
    } else {
        return ERRCODE_INVALID_PARAM;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_enable(uint8_t sensor_type, bool enable)
{
    if (sensor_type == 0) { /* IMU */
        if (!g_imu_state.initialized) {
            return ERRCODE_FAIL;
        }
        g_imu_state.enabled = enable;
        /* TODO: 写寄存器使能/禁用加速度计和陀螺仪 */
        PRINT("[far_sensor] IMU %s\r\n", enable ? "enabled" : "disabled");
    } else if (sensor_type == 1) { /* PPG */
        if (!g_ppg_state.initialized) {
            return ERRCODE_FAIL;
        }
        g_ppg_state.enabled = enable;
        /* TODO: 写寄存器使能/禁用PPG LED和ADC */
        PRINT("[far_sensor] PPG %s\r\n", enable ? "enabled" : "disabled");
    } else {
        return ERRCODE_INVALID_PARAM;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_sensor_get_power_estimate(uint8_t sensor_type, uint32_t *power_ua)
{
    if (power_ua == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* 基于典型传感器芯片功耗估算
     * BMI270: 加速度计 ~0.5-1.5mA, 陀螺仪 ~2-4mA
     * MAX30102: ~0.6-1.2mA (HR模式), ~1.2-3mA (SpO2模式)
     */
    if (sensor_type == 0) { /* IMU */
        switch (g_imu_state.config.mode) {
            case FAR_SENSOR_MODE_LOW_POWER:
                *power_ua = 500;   /* 500uA */
                break;
            case FAR_SENSOR_MODE_NORMAL:
                *power_ua = 1500;  /* 1.5mA */
                break;
            case FAR_SENSOR_MODE_HIGH_PERF:
                *power_ua = 4000;  /* 4mA */
                break;
            default:
                *power_ua = 1500;
                break;
        }
    } else if (sensor_type == 1) { /* PPG */
        switch (g_ppg_state.config.mode) {
            case FAR_SENSOR_MODE_LOW_POWER:
                *power_ua = 600;   /* 600uA */
                break;
            case FAR_SENSOR_MODE_NORMAL:
                *power_ua = 1200;  /* 1.2mA */
                break;
            case FAR_SENSOR_MODE_HIGH_PERF:
                *power_ua = 3000;  /* 3mA */
                break;
            default:
                *power_ua = 1200;
                break;
        }
    } else {
        return ERRCODE_INVALID_PARAM;
    }

    return ERRCODE_SUCC;
}
