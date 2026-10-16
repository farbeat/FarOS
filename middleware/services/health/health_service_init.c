/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: FarOS Health Service initialization and task management. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_health_common.h"
#include "far_sensor.h"
#include "far_activity.h"
#include "far_physio.h"
#include "far_sleep.h"
#include "far_health_data.h"
#include "debug_print.h"
#include "securec.h"
#include "cmsis_os2.h"
#include "soc_osal.h"

/*
 * ============================================================
 * 运动健康服务初始化与任务管理
 * Health Service Init and Task Management
 * ============================================================
 *
 * 本文件负责运动健康服务的整体初始化和后台任务管理。
 * 包含一个 health_task 后台任务，负责：
 * 1. 传感器数据采集调度
 * 2. 算法周期性处理
 * 3. 数据存储和同步
 * 4. 事件分发
 *
 * 任务优先级: 18 (高于普通应用任务，低于蓝牙SDK)
 * 任务栈大小: 0x1000 (4KB)
 * 调度周期: 100ms
 */

#define HEALTH_TASK_STACK_SIZE      0x1000
#define HEALTH_TASK_PRIORITY        18
#define HEALTH_TASK_INTERVAL_MS     100
#define HEALTH_SENSOR_BATCH_SIZE    16

/* 健康服务全局状态 */
typedef struct {
    bool initialized;
    bool task_running;
    osal_task *task_handle;
    far_sensor_imu_config_t imu_config;
    far_sensor_ppg_config_t ppg_config;
    far_activity_config_t activity_config;
    far_sleep_config_t sleep_config;
} far_health_service_t;

static far_health_service_t g_health_service = {0};

/* IMU数据缓冲区 */
static far_sensor_imu_data_t g_imu_batch_buf[HEALTH_SENSOR_BATCH_SIZE];
static uint16_t g_imu_batch_count = 0;

/*
 * 传感器数据回调（IMU）
 * 异步接收传感器数据，批量处理
 */
static void health_sensor_imu_callback(const void *data, uint32_t len, void *user_data)
{
    (void)user_data;
    const far_sensor_imu_data_t *imu_data = (const far_sensor_imu_data_t *)data;

    /* 将数据传递给运动识别算法 */
    (void)uapi_activity_feed_imu_data(imu_data, len);

    /* 将数据传递给睡眠分析算法 */
    (void)uapi_sleep_feed_imu_data(imu_data, len);
}

/*
 * 传感器数据回调（PPG）
 */
static void health_sensor_ppg_callback(const void *data, uint32_t len, void *user_data)
{
    (void)user_data;
    (void)data;
    (void)len;

    /* TODO: PPG数据处理
     * 1. 从PPG原始数据提取心率
     * 2. 将心率数据传递给生理指标算法
     * 3. 将心率数据传递给睡眠分析算法
     */
}

/*
 * 健康服务后台任务
 */
static void health_task(void const *arg)
{
    (void)arg;

    PRINT("[health_service] Health task started\r\n");

    while (g_health_service.task_running) {
        /* 周期性读取传感器数据 */
        if (g_health_service.imu_config.accel_rate > 0) {
            uint32_t read_len = 0;
            (void)uapi_sensor_imu_read(g_imu_batch_buf, HEALTH_SENSOR_BATCH_SIZE, &read_len);
            if (read_len > 0) {
                (void)uapi_activity_feed_imu_data(g_imu_batch_buf, read_len);
                (void)uapi_sleep_feed_imu_data(g_imu_batch_buf, read_len);
            }
        }

        /* TODO: PPG数据读取和处理 */

        /* TODO: 周期性算法处理
         * 1. 步频计算（每10秒）
         * 2. 活动类型更新（每3秒）
         * 3. 卡路里累积计算
         * 4. HRV分析触发（每5分钟）
         * 5. 压力指数更新（每3分钟）
         * 6. 身体电量更新（每5分钟）
         * 7. 睡眠分期决策（每30秒）
         */

        /* TODO: 数据持久化
         * 1. 每小时保存一次每日汇总到NV
         * 2. 运动结束后保存完整记录
         * 3. 睡眠结束后保存睡眠记录
         */

        (void)osDelay(HEALTH_TASK_INTERVAL_MS);
    }

    PRINT("[health_service] Health task stopped\r\n");
}

/* ============================================================
 * Public APIs
 * ============================================================ */

errcode_t uapi_health_service_init(void)
{
    if (g_health_service.initialized) {
        return ERRCODE_SUCC;
    }

    PRINT("[health_service] Initializing FarOS Health Service v%d.%d.%d\r\n",
          FAR_HEALTH_API_VERSION_MAJOR,
          FAR_HEALTH_API_VERSION_MINOR,
          FAR_HEALTH_API_VERSION_PATCH);

    /* 1. 初始化数据管理模块 */
    (void)uapi_health_data_init(FAR_STORAGE_MODE_DUAL);

    /* 2. 初始化传感器 */
    g_health_service.imu_config.accel_rate = FAR_SENSOR_RATE_50_HZ;
    g_health_service.imu_config.gyro_rate = FAR_SENSOR_RATE_50_HZ;
    g_health_service.imu_config.mode = FAR_SENSOR_MODE_NORMAL;
    g_health_service.imu_config.accel_range_g = 8;
    g_health_service.imu_config.gyro_range_dps = 2000;
    (void)uapi_sensor_imu_init(&g_health_service.imu_config);

    g_health_service.ppg_config.sample_rate = FAR_SENSOR_RATE_25_HZ;
    g_health_service.ppg_config.mode = FAR_SENSOR_MODE_NORMAL;
    g_health_service.ppg_config.led_current_ma = 25;
    g_health_service.ppg_config.enable_green = true;
    g_health_service.ppg_config.enable_ir = false;
    g_health_service.ppg_config.enable_red = false;
    (void)uapi_sensor_ppg_init(&g_health_service.ppg_config);

    /* 3. 注册传感器回调 */
    (void)uapi_sensor_imu_register_callback(health_sensor_imu_callback, NULL);
    (void)uapi_sensor_ppg_register_callback(health_sensor_ppg_callback, NULL);

    /* 4. 初始化算法模块 */
    g_health_service.activity_config.enable_step_count = true;
    g_health_service.activity_config.enable_activity_type = true;
    g_health_service.activity_config.enable_cadence = true;
    g_health_service.activity_config.enable_distance = true;
    g_health_service.activity_config.step_threshold = 150;
    g_health_service.activity_config.confidence_threshold = 70;
    (void)uapi_activity_init(&g_health_service.activity_config);

    (void)uapi_physio_init();
    (void)uapi_physio_hr_init();

    g_health_service.sleep_config.enable_auto_detect = true;
    g_health_service.sleep_config.bedtime_hour = 255; /* auto */
    g_health_service.sleep_config.wake_time_hour = 255; /* auto */
    g_health_service.sleep_config.min_sleep_duration_min = 30;
    g_health_service.sleep_config.enable_hrv_analysis = true;
    (void)uapi_sleep_init(&g_health_service.sleep_config);

    /* 5. 创建后台任务 */
    g_health_service.task_running = true;
    g_health_service.task_handle = osal_kthread_create(
        (osal_kthread_handler)health_task, NULL,
        "health_task", HEALTH_TASK_STACK_SIZE);
    if (g_health_service.task_handle == NULL) {
        PRINT("[health_service] Failed to create health task\r\n");
        return ERRCODE_FAIL;
    }
    osal_kthread_set_priority(g_health_service.task_handle, HEALTH_TASK_PRIORITY);

    /* 6. 使能各模块 */
    (void)uapi_sensor_enable(0, true); /* IMU */
    (void)uapi_sensor_enable(1, true); /* PPG */
    (void)uapi_activity_enable();
    (void)uapi_sleep_enable();
    (void)uapi_physio_stress_enable(true);
    (void)uapi_physio_body_battery_enable(true);

    g_health_service.initialized = true;
    PRINT("[health_service] FarOS Health Service initialized successfully\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_health_service_deinit(void)
{
    if (!g_health_service.initialized) {
        return ERRCODE_SUCC;
    }

    /* 停止后台任务 */
    g_health_service.task_running = false;
    if (g_health_service.task_handle != NULL) {
        /* 等待任务结束或强制删除 */
        (void)osDelay(HEALTH_TASK_INTERVAL_MS * 2);
    }

    /* 反初始化各模块 */
    (void)uapi_sleep_deinit();
    (void)uapi_physio_hr_deinit();
    (void)uapi_physio_deinit();
    (void)uapi_activity_deinit();
    (void)uapi_sensor_imu_unregister_callback();
    (void)uapi_sensor_ppg_unregister_callback();
    (void)uapi_sensor_imu_deinit();
    (void)uapi_sensor_ppg_deinit();
    (void)uapi_health_data_deinit();

    (void)memset_s(&g_health_service, sizeof(g_health_service), 0, sizeof(g_health_service));

    PRINT("[health_service] FarOS Health Service deinitialized\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_health_get_version(uint8_t *major, uint8_t *minor, uint8_t *patch)
{
    if ((major == NULL) || (minor == NULL) || (patch == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *major = FAR_HEALTH_API_VERSION_MAJOR;
    *minor = FAR_HEALTH_API_VERSION_MINOR;
    *patch = FAR_HEALTH_API_VERSION_PATCH;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_set_user_profile(const far_user_profile_t *profile)
{
    if (profile == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 保存用户档案到NV存储
     * 同时更新各算法模块的内部参数
     */

    (void)profile;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_get_user_profile(far_user_profile_t *profile)
{
    if (profile == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 从NV存储读取用户档案 */

    (void)memset_s(profile, sizeof(far_user_profile_t), 0, sizeof(far_user_profile_t));
    return ERRCODE_SUCC;
}

errcode_t uapi_health_register_callback(far_health_callback_t callback)
{
    if (callback == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 注册全局健康数据更新回调
     * 将回调注册到各子模块
     */

    (void)callback;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_unregister_callback(void)
{
    /* TODO: 注销全局回调 */
    return ERRCODE_SUCC;
}
