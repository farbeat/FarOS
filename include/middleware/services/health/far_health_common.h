/**
 * Copyright (c) @国体智慧体育技术创新中心 2024-2025. All rights reserved. \n
 *
 * Description: Provides common definitions for FarOS Health Service. \n
 * Author: 国体智慧体育技术创新中心 \n
 * History: \n
 * 2024-10-17, Create file. \n
 */

#ifndef FAR_HEALTH_COMMON_H
#define FAR_HEALTH_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup health_service FarOS Health Service
 * @ingroup  middleware_services
 * @brief    FarOS运动健康服务模块，面向可穿戴设备提供运动识别、生理指标分析、
 *           睡眠监测及数据管理功能。参考Firstbeat核心概念框架设计。
 *
 * @if Eng
 * FarOS Health Service Module provides activity recognition, physiological
 * indicator analysis, sleep monitoring, and health data management for
 * wearable devices. Designed with reference to Firstbeat core concepts.
 * @endif
 * @{
 */

/**
 * @if Eng
 * @brief  Health Service API version number.
 * @else
 * @brief  运动健康服务API版本号。
 * @endif
 */
#define FAR_HEALTH_API_VERSION_MAJOR    1
#define FAR_HEALTH_API_VERSION_MINOR    0
#define FAR_HEALTH_API_VERSION_PATCH    0

/**
 * @if Eng
 * @brief  Maximum length of health record ID string.
 * @else
 * @brief  健康记录ID字符串最大长度。
 * @endif
 */
#define FAR_HEALTH_RECORD_ID_MAX_LEN    32

/**
 * @if Eng
 * @brief  Maximum number of historical records that can be queried at one time.
 * @else
 * @brief  单次查询历史记录最大数量。
 * @endif
 */
#define FAR_HEALTH_MAX_QUERY_RECORDS    50

/**
 * @if Eng
 * @brief  Maximum number of sleep stage segments.
 * @else
 * @brief  睡眠分期段最大数量。
 * @endif
 */
#define FAR_HEALTH_MAX_SLEEP_SEGMENTS   32

/**
 * @if Eng
 * @brief  Number of heart rate zones (based on Firstbeat/Karvonen model).
 * @else
 * @brief  心率区间数量（基于Firstbeat/Karvonen模型）。
 * @endif
 */
#define FAR_HEALTH_HR_ZONE_COUNT        5

/**
 * @if Eng
 * @brief  Callback function type for health data updates.
 * @param  [in] type    Data update type, see @ref far_health_data_type_t.
 * @param  [in] data    Pointer to update data.
 * @param  [in] len     Data length in bytes.
 * @else
 * @brief  健康数据更新回调函数类型。
 * @param  [in] type    数据更新类型，参考 @ref far_health_data_type_t 。
 * @param  [in] data    更新数据指针。
 * @param  [in] len     数据长度，单位：字节。
 * @endif
 */
typedef void (*far_health_callback_t)(uint32_t type, const void *data, uint32_t len);

/**
 * @if Eng
 * @brief  Enumeration of health data update types.
 * @else
 * @brief  健康数据更新类型枚举。
 * @endif
 */
typedef enum {
    FAR_HEALTH_DATA_TYPE_STEP_COUNT     = 0x01,  /*!< @if Eng Step count updated.
                                                      @else   步数更新。 @endif */
    FAR_HEALTH_DATA_TYPE_HEART_RATE     = 0x02,  /*!< @if Eng Heart rate updated.
                                                      @else   心率更新。 @endif */
    FAR_HEALTH_DATA_TYPE_ACTIVITY       = 0x03,  /*!< @if Eng Activity type changed.
                                                      @else   活动类型变化。 @endif */
    FAR_HEALTH_DATA_TYPE_SLEEP_STAGE    = 0x04,  /*!< @if Eng Sleep stage changed.
                                                      @else   睡眠分期变化。 @endif */
    FAR_HEALTH_DATA_TYPE_HRV            = 0x05,  /*!< @if Eng HRV analysis result updated.
                                                      @else   HRV分析结果更新。 @endif */
    FAR_HEALTH_DATA_TYPE_STRESS         = 0x06,  /*!< @if Eng Stress index updated.
                                                      @else   压力指数更新。 @endif */
    FAR_HEALTH_DATA_TYPE_BODY_BATTERY   = 0x07,  /*!< @if Eng Body battery updated.
                                                      @else   身体电量更新。 @endif */
    FAR_HEALTH_DATA_TYPE_CALORIE        = 0x08,  /*!< @if Eng Calorie consumption updated.
                                                      @else   卡路里消耗更新。 @endif */
    FAR_HEALTH_DATA_TYPE_DISTANCE       = 0x09,  /*!< @if Eng Distance updated.
                                                      @else   距离更新。 @endif */
    FAR_HEALTH_DATA_TYPE_VO2MAX         = 0x0A,  /*!< @if Eng VO2max estimate updated.
                                                      @else   VO2max估算值更新。 @endif */
    FAR_HEALTH_DATA_TYPE_TRAINING_LOAD  = 0x0B,  /*!< @if Eng Training load updated.
                                                      @else   训练负荷更新。 @endif */
    FAR_HEALTH_DATA_TYPE_RECOVERY_TIME  = 0x0C,  /*!< @if Eng Recovery time updated.
                                                      @else   恢复时间更新。 @endif */
} far_health_data_type_t;

/**
 * @if Eng
 * @brief  Enumeration of activity types.
 * @else
 * @brief  活动类型枚举。
 * @endif
 */
typedef enum {
    FAR_ACTIVITY_TYPE_UNKNOWN   = 0,   /*!< @if Eng Unknown activity.
                                            @else   未知活动。 @endif */
    FAR_ACTIVITY_TYPE_REST      = 1,   /*!< @if Eng Rest / Sedentary.
                                            @else   静止/久坐。 @endif */
    FAR_ACTIVITY_TYPE_WALK      = 2,   /*!< @if Eng Walking.
                                            @else   步行。 @endif */
    FAR_ACTIVITY_TYPE_RUN       = 3,   /*!< @if Eng Running.
                                            @else   跑步。 @endif */
    FAR_ACTIVITY_TYPE_CYCLE     = 4,   /*!< @if Eng Cycling.
                                            @else   骑行。 @endif */
    FAR_ACTIVITY_TYPE_SWIM      = 5,   /*!< @if Eng Swimming.
                                            @else   游泳。 @endif */
    FAR_ACTIVITY_TYPE_HIKE      = 6,   /*!< @if Eng Hiking.
                                            @else   徒步。 @endif */
    FAR_ACTIVITY_TYPE_ELLIPTICAL = 7,  /*!< @if Eng Elliptical training.
                                            @else   椭圆机。 @endif */
    FAR_ACTIVITY_TYPE_ROWING    = 8,   /*!< @if Eng Rowing.
                                            @else   划船。 @endif */
    FAR_ACTIVITY_TYPE_STAIR     = 9,   /*!< @if Eng Stair climbing.
                                            @else   爬楼梯。 @endif */
    FAR_ACTIVITY_TYPE_SPORT_OTHER = 10, /*!< @if Eng Other sports.
                                             @else   其他运动。 @endif */
} far_activity_type_t;

/**
 * @if Eng
 * @brief  Enumeration of sleep stages.
 * @else
 * @brief  睡眠分期枚举。
 * @endif
 */
typedef enum {
    FAR_SLEEP_STAGE_WAKE    = 0,   /*!< @if Eng Awake.
                                        @else   清醒。 @endif */
    FAR_SLEEP_STAGE_REM     = 1,   /*!< @if Eng REM sleep.
                                        @else   REM快速眼动期。 @endif */
    FAR_SLEEP_STAGE_LIGHT   = 2,   /*!< @if Eng Light sleep.
                                        @else   浅睡期。 @endif */
    FAR_SLEEP_STAGE_DEEP    = 3,   /*!< @if Eng Deep sleep.
                                        @else   深睡期。 @endif */
} far_sleep_stage_t;

/**
 * @if Eng
 * @brief  Enumeration of heart rate zones (Karvonen model).
 * @else
 * @brief  心率区间枚举（Karvonen模型）。
 * @endif
 */
typedef enum {
    FAR_HR_ZONE_WARMUP      = 0,   /*!< @if Eng Warm-up zone (< 60% HRR).
                                        @else   热身区（< 60% HRR）。 @endif */
    FAR_HR_ZONE_FAT_BURN    = 1,   /*!< @if Eng Fat burn zone (60-70% HRR).
                                        @else   燃脂区（60-70% HRR）。 @endif */
    FAR_HR_ZONE_AEROBIC     = 2,   /*!< @if Eng Aerobic zone (70-80% HRR).
                                        @else   有氧耐力区（70-80% HRR）。 @endif */
    FAR_HR_ZONE_THRESHOLD   = 3,   /*!< @if Eng Threshold zone (80-90% HRR).
                                        @else   乳酸阈值区（80-90% HRR）。 @endif */
    FAR_HR_ZONE_ANAEROBIC   = 4,   /*!< @if Eng Anaerobic zone (90-100% HRR).
                                        @else   无氧区（90-100% HRR）。 @endif */
} far_hr_zone_t;

/**
 * @if Eng
 * @brief  User profile structure for physiological calculations.
 * @else
 * @brief  用于生理计算的用户档案结构体。
 * @endif
 */
typedef struct {
    uint8_t  gender;           /*!< @if Eng Gender: 0=female, 1=male.
                                    @else   性别：0=女，1=男。 @endif */
    uint8_t  age;              /*!< @if Eng Age in years.
                                    @else   年龄，单位：岁。 @endif */
    uint16_t height_cm;        /*!< @if Eng Height in centimeters.
                                    @else   身高，单位：厘米。 @endif */
    uint16_t weight_kg;        /*!< @if Eng Weight in kilograms (x10 for 0.1kg precision).
                                    @else   体重，单位：千克x10（支持0.1kg精度）。 @endif */
    uint8_t  max_hr;           /*!< @if Eng User-defined maximum heart rate, 0=auto calc.
                                    @else   用户自定义最大心率，0=自动计算。 @endif */
    uint8_t  rest_hr;          /*!< @if Eng Measured resting heart rate.
                                    @else   实测静息心率。 @endif */
} far_user_profile_t;

/**
 * @if Eng
 * @brief  Timestamp structure for health records.
 * @else
 * @brief  健康记录时间戳结构体。
 * @endif
 */
typedef struct {
    uint16_t year;     /*!< @if Eng Year, e.g., 2024.
                            @else   年，例如2024。 @endif */
    uint8_t  month;    /*!< @if Eng Month, 1-12.
                            @else   月，1-12。 @endif */
    uint8_t  day;      /*!< @if Eng Day, 1-31.
                            @else   日，1-31。 @endif */
    uint8_t  hour;     /*!< @if Eng Hour, 0-23.
                            @else   时，0-23。 @endif */
    uint8_t  minute;   /*!< @if Eng Minute, 0-59.
                            @else   分，0-59。 @endif */
    uint8_t  second;   /*!< @if Eng Second, 0-59.
                            @else   秒，0-59。 @endif */
} far_health_timestamp_t;

/**
 * @if Eng
 * @brief  Get the version string of FarOS Health Service.
 * @param  [out] major    Major version number.
 * @param  [out] minor    Minor version number.
 * @param  [out] patch    Patch version number.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure.
 * @else
 * @brief  获取FarOS运动健康服务版本号。
 * @param  [out] major    主版本号。
 * @param  [out] minor    次版本号。
 * @param  [out] patch    补丁版本号。
 * @retval ERRCODE_SUCC   成功。
 * @retval Other          失败。
 * @endif
 */
errcode_t uapi_health_get_version(uint8_t *major, uint8_t *minor, uint8_t *patch);

/**
 * @if Eng
 * @brief  Set user profile for physiological calculations.
 * @param  [in] profile   Pointer to user profile data.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  设置用于生理计算的用户档案。
 * @param  [in] profile   用户档案数据指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_health_set_user_profile(const far_user_profile_t *profile);

/**
 * @if Eng
 * @brief  Get current user profile.
 * @param  [out] profile  Pointer to store user profile data.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  获取当前用户档案。
 * @param  [out] profile  存储用户档案数据的指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_health_get_user_profile(far_user_profile_t *profile);

/**
 * @if Eng
 * @brief  Register a global health data update callback.
 * @param  [in] callback  Callback function pointer.
 * @retval ERRCODE_SUCC   Success.
 * @retval ERRCODE_INVALID_PARAM Invalid parameter.
 * @else
 * @brief  注册全局健康数据更新回调。
 * @param  [in] callback  回调函数指针。
 * @retval ERRCODE_SUCC   成功。
 * @retval ERRCODE_INVALID_PARAM 参数无效。
 * @endif
 */
errcode_t uapi_health_register_callback(far_health_callback_t callback);

/**
 * @if Eng
 * @brief  Unregister the global health data update callback.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  注销全局健康数据更新回调。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_health_unregister_callback(void);

/**
 * @if Eng
 * @brief  Initialize the entire FarOS Health Service.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure.
 * @else
 * @brief  初始化FarOS运动健康服务总模块。
 * @retval ERRCODE_SUCC   成功。
 * @retval Other          失败。
 * @endif
 */
errcode_t uapi_health_service_init(void);

/**
 * @if Eng
 * @brief  Deinitialize the FarOS Health Service.
 * @retval ERRCODE_SUCC   Success.
 * @else
 * @brief  反初始化FarOS运动健康服务总模块。
 * @retval ERRCODE_SUCC   成功。
 * @endif
 */
errcode_t uapi_health_service_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* FAR_HEALTH_COMMON_H */
