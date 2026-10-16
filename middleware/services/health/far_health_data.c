/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Health data management implementation skeleton. \n
 *              Includes exercise record storage, historical query, \n
 *              daily summary, and data synchronization. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#include "far_health_data.h"
#include "far_sleep.h"
#include "debug_print.h"
#include "securec.h"
#include "fs.h"
#include "nv.h"

/*
 * ============================================================
 * 健康数据管理骨架实现说明
 * Health Data Management Skeleton Implementation Notes
 * ============================================================
 *
 * 本文件包含运动记录存储、历史数据查询、每日汇总和数据同步的框架。
 * 支持NV存储和文件系统双模式，确保数据可靠性。
 *
 * 存储架构：
 * 1. NV存储模式：使用uapi_nv_write/uapi_nv_read存储关键摘要数据
 *    - 今日步数、距离、卡路里汇总
 *    - 最近7天每日摘要
 *    - 适合快速读取的小数据
 *
 * 2. 文件系统模式：使用文件系统存储完整记录
 *    - 详细运动记录（GPS轨迹、心率曲线等）
 *    - 完整睡眠分期数据
 *    - 适合大数据量长期存储
 *
 * 3. 双模式：关键数据同时存储到NV和文件系统
 *    - NV用于快速访问
 *    - 文件系统用于数据备份和导出
 *
 * 记录ID格式：
 *   workout: "W{YYYYMMDD}_{HHMMSS}_{TYPE}"
 *   sleep:   "S{YYYYMMDD}_{HHMMSS}"
 *   hrv:     "H{YYYYMMDD}_{HHMMSS}"
 *
 * 数据格式（JSON-like紧凑格式）：
 *   文件系统使用文本格式存储，便于导出和调试
 *   NV存储使用二进制结构体，节省空间
 */

#define HEALTH_DATA_DIR             "/user/health/"
#define HEALTH_WORKOUT_DIR          "/user/health/workout/"
#define HEALTH_SLEEP_DIR            "/user/health/sleep/"
#define HEALTH_DAILY_FILE           "/user/health/daily.bin"
#define HEALTH_RECORD_PREFIX_WORK   'W'
#define HEALTH_RECORD_PREFIX_SLEEP  'S'
#define HEALTH_NV_KEY_DAILY_BASE    0x5000  /* NV key起始值 */
#define HEALTH_NV_KEY_PROFILE       0x5FFF
#define HEALTH_MAX_DAILY_CACHE      7       /* 缓存最近7天数据 */

/* 每日数据缓存 */
typedef struct {
    far_daily_summary_t summaries[HEALTH_MAX_DAILY_CACHE];
    uint8_t count;
    uint8_t write_idx;
} far_daily_cache_t;

/* 数据管理状态 */
typedef struct {
    bool initialized;
    far_storage_mode_t storage_mode;
    far_daily_cache_t daily_cache;
    uint32_t total_records;
    uint32_t storage_used;
    uint32_t storage_total;
    far_data_sync_callback_t sync_callback;
    void *sync_user_data;
} far_health_data_state_t;

static far_health_data_state_t g_health_data = {0};

/*
 * 生成记录ID
 */
static void health_gen_record_id(char *buf, uint32_t buf_len, far_record_type_t type,
                                  const far_health_timestamp_t *time)
{
    char prefix = (type == FAR_RECORD_TYPE_SLEEP) ? HEALTH_RECORD_PREFIX_SLEEP :
                  (type == FAR_RECORD_TYPE_WORKOUT) ? HEALTH_RECORD_PREFIX_WORK : 'R';

    if (buf_len >= FAR_HEALTH_RECORD_ID_MAX_LEN) {
        (void)snprintf_s(buf, buf_len, FAR_HEALTH_RECORD_ID_MAX_LEN - 1,
                         "%c%04d%02d%02d_%02d%02d%02d",
                         prefix, time->year, time->month, time->day,
                         time->hour, time->minute, time->second);
    }
}

/*
 * 确保目录存在
 */
static errcode_t health_ensure_directories(void)
{
    /* TODO: 创建健康数据存储目录
     * mkdir(HEALTH_DATA_DIR);
     * mkdir(HEALTH_WORKOUT_DIR);
     * mkdir(HEALTH_SLEEP_DIR);
     */
    return ERRCODE_SUCC;
}

/*
 * 将运动记录序列化为文件
 */
static errcode_t health_save_workout_to_fs(const far_workout_record_t *record)
{
    /* TODO: 将workout记录序列化为JSON/二进制文件
     * 1. 生成文件名: /user/health/workout/{record_id}.json
     * 2. 序列化字段:
     *    {"id":"W20241017_120000_3","type":1,"activity":3,...}
     * 3. 写入文件系统
     */
    (void)record;
    return ERRCODE_SUCC;
}

/*
 * 从文件读取运动记录
 */
static errcode_t health_load_workout_from_fs(const char *record_id,
                                              far_workout_record_t *record)
{
    /* TODO: 从文件系统读取workout记录
     * 1. 打开文件 /user/health/workout/{record_id}.json
     * 2. 解析JSON字段
     * 3. 填充record结构体
     */
    (void)record_id;
    (void)record;
    return ERRCODE_SUCC;
}

/*
 * 更新每日汇总缓存
 */
static errcode_t health_update_daily_cache(const far_daily_summary_t *summary)
{
    /* TODO: 更新每日汇总缓存
     * 1. 检查是否已有该日期的记录
     * 2. 若有则更新，否则添加到缓存
     * 3. 缓存满时淘汰最旧的记录
     */

    uint8_t idx = g_health_data.daily_cache.write_idx;
    (void)memcpy_s(&g_health_data.daily_cache.summaries[idx],
                  sizeof(far_daily_summary_t), summary, sizeof(far_daily_summary_t));
    g_health_data.daily_cache.write_idx = (idx + 1) % HEALTH_MAX_DAILY_CACHE;
    if (g_health_data.daily_cache.count < HEALTH_MAX_DAILY_CACHE) {
        g_health_data.daily_cache.count++;
    }

    return ERRCODE_SUCC;
}

/* ============================================================
 * Public APIs
 * ============================================================ */

errcode_t uapi_health_data_init(far_storage_mode_t storage_mode)
{
    if (g_health_data.initialized) {
        return ERRCODE_SUCC;
    }

    g_health_data.storage_mode = storage_mode;
    g_health_data.initialized = true;
    g_health_data.total_records = 0;
    g_health_data.storage_used = 0;
    g_health_data.storage_total = 1024 * 1024; /* 1MB default */
    g_health_data.sync_callback = NULL;
    g_health_data.sync_user_data = NULL;
    g_health_data.daily_cache.count = 0;
    g_health_data.daily_cache.write_idx = 0;

    /* 创建存储目录 */
    if (storage_mode != FAR_STORAGE_MODE_NV) {
        (void)health_ensure_directories();
    }

    /* TODO: 从NV/文件系统加载缓存数据
     * 1. 读取最近7天每日汇总
     * 2. 统计总记录数
     * 3. 计算已用存储空间
     */

    PRINT("[far_health_data] Data management init, mode=%d\r\n", storage_mode);
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_deinit(void)
{
    (void)memset_s(&g_health_data, sizeof(g_health_data), 0, sizeof(g_health_data));
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_save_workout(const far_workout_record_t *record)
{
    if (record == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* 生成记录ID（如未提供） */
    far_workout_record_t rec = *record;
    if (rec.record_id[0] == '\0') {
        health_gen_record_id(rec.record_id, sizeof(rec.record_id),
                              FAR_RECORD_TYPE_WORKOUT, &rec.start_time);
    }

    /* NV存储 */
    if (g_health_data.storage_mode == FAR_STORAGE_MODE_NV ||
        g_health_data.storage_mode == FAR_STORAGE_MODE_DUAL) {
        /* TODO: 使用uapi_nv_write存储workout摘要 */
    }

    /* 文件系统存储 */
    if (g_health_data.storage_mode == FAR_STORAGE_MODE_FS ||
        g_health_data.storage_mode == FAR_STORAGE_MODE_DUAL) {
        (void)health_save_workout_to_fs(&rec);
    }

    g_health_data.total_records++;
    PRINT("[far_health_data] Workout saved: %s\r\n", rec.record_id);
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_save_sleep(const far_sleep_record_t *record)
{
    if (record == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 保存睡眠记录
     * 类似workout记录的保存逻辑
     * 1. 生成记录ID
     * 2. 存储到NV/文件系统
     */

    (void)record;
    g_health_data.total_records++;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_get_workouts(const far_data_query_filter_t *filter,
                                         far_workout_record_t *records,
                                         uint32_t max_records,
                                         uint32_t *actual_count)
{
    if ((filter == NULL) || (records == NULL) || (actual_count == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *actual_count = 0;

    /* TODO: 查询历史workout记录
     * 1. 根据filter条件遍历文件系统/NV
     * 2. 解析每条记录
     * 3. 匹配filter条件（类型、日期范围、最小时长）
     * 4. 填充records数组（最多max_records条）
     * 5. 返回actual_count
     */

    /* 骨架：返回空结果 */
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_get_sleep_records(const far_health_timestamp_t *start_date,
                                              const far_health_timestamp_t *end_date,
                                              far_sleep_record_t *records,
                                              uint32_t max_records,
                                              uint32_t *actual_count)
{
    if ((start_date == NULL) || (end_date == NULL) ||
        (records == NULL) || (actual_count == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *actual_count = 0;

    /* TODO: 查询历史睡眠记录
     * 类似workout查询逻辑
     */

    (void)start_date;
    (void)end_date;
    (void)max_records;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_get_daily_summary(const far_health_timestamp_t *date,
                                              far_daily_summary_t *summary)
{
    if ((date == NULL) || (summary == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    /* 先在缓存中查找 */
    for (uint8_t i = 0; i < g_health_data.daily_cache.count; i++) {
        if ((g_health_data.daily_cache.summaries[i].date.year == date->year) &&
            (g_health_data.daily_cache.summaries[i].date.month == date->month) &&
            (g_health_data.daily_cache.summaries[i].date.day == date->day)) {
            *summary = g_health_data.daily_cache.summaries[i];
            return ERRCODE_SUCC;
        }
    }

    /* TODO: 缓存未命中，从NV/文件系统加载
     * 1. 尝试从NV读取
     * 2. 若NV不存在，从文件系统读取
     * 3. 更新缓存
     */

    return ERRCODE_FAIL;
}

errcode_t uapi_health_data_delete_record(const char *record_id)
{
    if (record_id == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 删除记录
     * 1. 从文件系统删除对应文件
     * 2. 从NV删除对应条目
     * 3. 更新总记录数
     */

    PRINT("[far_health_data] Record deleted: %s\r\n", record_id);
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_delete_before(const far_health_timestamp_t *before_date)
{
    if (before_date == NULL) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 批量删除指定日期前的记录
     * 1. 遍历文件系统目录
     * 2. 解析文件名中的日期
     * 3. 删除早于before_date的文件
     * 4. 清理NV中对应的条目
     */

    (void)before_date;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_get_storage_usage(uint32_t *used_bytes, uint32_t *total_bytes)
{
    if ((used_bytes == NULL) || (total_bytes == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    *used_bytes = g_health_data.storage_used;
    *total_bytes = g_health_data.storage_total;
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_sync_start(far_data_sync_callback_t callback, void *user_data)
{
    g_health_data.sync_callback = callback;
    g_health_data.sync_user_data = user_data;

    /* TODO: 启动数据同步任务
     * 1. 创建后台同步任务
     * 2. 遍历所有记录
     * 3. 通过蓝牙/BLE发送到手机APP
     * 4. 报告同步进度
     */

    if (callback != NULL) {
        callback(0, 2, user_data); /* status=2: in_progress */
    }

    PRINT("[far_health_data] Data sync started\r\n");
    return ERRCODE_SUCC;
}

errcode_t uapi_health_data_export(const char *filename, const far_data_query_filter_t *filter)
{
    if ((filename == NULL) || (filter == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    /* TODO: 导出记录为JSON/CSV文件
     * 1. 查询符合条件的记录
     * 2. 序列化为JSON/CSV格式
     * 3. 写入到filename指定的文件
     * 4. 返回导出结果
     */

    (void)filename;
    return ERRCODE_SUCC;
}
