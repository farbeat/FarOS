/*
 * Copyright (c) @CompanyNameMagicTag 2024. All rights reserved.
 * Description: BLE Apple Notification Center Service Client module.
 */

/**
 * @defgroup bluetooth_bts_ancs_client ANCS CLIENT API
 * @ingroup  bluetooth
 * @{
 */
#ifndef BLE_ANCS_CLIENT_H
#define BLE_ANCS_CLIENT_H
#include "errcode.h"
#include "osal_list.h"
#include "sample_common.h"
#include "bts_gatt_client.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Default value of Client Characteristic Configuration.
 * @else
 * @brief  客户端特征配置描述符的默认值。
 * @endif
 */
#define BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT 0x0000

/**
 * @if Eng
 * @brief  Client Characteristic Configuration value is notification.
 * @else
 * @brief  客户端特征配置描述符的值为通知。
 * @endif
 */
#define BLE_ANCS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION 0x0001

/**
 * @if Eng
 * @brief  Enum of ancs notify switch.
 * @else
 * @brief  通知开关枚举值。
 * @endif
 */
typedef enum {
    ANCS_CLIENT_NOTIFY_DISABLE, /*!< @if Eng disable notify @else 关闭通知 @endif */
    ANCS_CLIENT_NOTIFY_ENABLE,  /*!< @if Eng enable notify @else 开启通知 @endif */
} ancs_client_notify_enable_t;

/**
 * @if Eng
 * @brief  Enum of event id
 * @else
 * @brief  事件ID枚举值
 * @endif
 */
typedef enum {
    ANCS_EVENT_ADDED = 0,     /*!< @if Eng add @else 新增通知 @endif */
    ANCS_EVENT_MODIFIED = 1,  /*!< @if Eng modify @else  修改通知 @endif */
    ANCS_EVENT_REMOEVD = 2,   /*!< @if Eng remove @else  移除通知 @endif */
    ANCS_EVENT_RESERVED       /*!< @if Eng reserved  @else 预留事件ID @endif */
} ancs_event_id_t;

/**
 * @if Eng
 * @brief  Enum of event flags
 * @else
 * @brief  事件标记枚举值
 * @endif
 */
typedef enum {
    ANCS_EVENT_FLAG_SILENT = (1 << 0),            /*!< @if Eng silent          @else  安静  @endif */
    ANCS_EVENT_FLAG_IMPORTANT = (1 << 1),         /*!< @if Eng important       @else  重要 @endif */
    ANCS_EVENT_FLAG_PREEXISTING = (1 << 2),       /*!< @if Eng preexisting     @else   先存的   @endif */
    ANCS_EVENT_FLAG_POSITIVE_ACTION = (1 << 3),   /*!< @if Eng positive action @else  积极活动 @endif */
    ANCS_EVENT_FLAG_NEGATIVE_ACTION = (1 << 4),   /*!< @if Eng negative action @else  消极活动 @endif */
    ANCS_EVENT_FLAG_RESERVED                      /*!< @if Eng reserved        @else 预留事件标记 @endif */
} ancs_event_flags_t;

/**
 * @if Eng
 * @brief  Enum of command id
 * @else
 * @brief  命令ID枚举值
 * @endif
 */
typedef enum {
    ANCS_CMD_GET_NOTIFY_ATT = 0,          /*!< @if Eng Get notify attribute  @else 获取通知属性  @endif */
    ANCS_CMD_GET_APP_ATT = 1,             /*!< @if Eng Get app attribute     @else  获取应用属性 @endif */
    ANCS_CMD_PERFORM_NOTIFY_ACTION = 2,   /*!< @if Eng Perform notify action @else  处理通知     @endif */
    ANCS_CMD_RESERVED                     /*!< @if Eng reserved              @else 预留命令ID    @endif */
} ancs_command_id_t;

/**
 * @if Eng
 * @brief  Enum of category id
 * @else
 * @brief  分类ID枚举值
 * @endif
 */
typedef enum {
    ANCS_CATEGORY_OTHER = 0,            /*!< @if Eng Other            @else 其他 @endif */
    ANCS_CATEGORY_INCOMING_CALL = 1,    /*!< @if Eng Incoming call    @else  来电 @endif */
    ANCS_CATEGORY_MISSED_CALL = 2,      /*!< @if Eng Missed call      @else  未接来电 @endif */
    ANCS_CATEGORY_VOICE_MAIL = 3,       /*!< @if Eng Voice mail       @else  语音消息 @endif */
    ANCS_CATEGORY_SOCIAL = 4,           /*!< @if Eng Social           @else  社交 @endif */
    ANCS_CATEGORY_SCHEDULE = 5,         /*!< @if Eng Schedule         @else  计划 @endif */
    ANCS_CATEGORY_EMAIL = 6,            /*!< @if Eng Email            @else  邮件 @endif */
    ANCS_CATEGORY_NEWS = 7,             /*!< @if Eng News             @else  新闻 @endif */
    ANCS_CATEGORY_HEALTH_FITNESS = 8,   /*!< @if Eng Health fitness   @else  运动健康 @endif */
    ANCS_CATEGORY_BUSINESS_FINANCE = 9, /*!< @if Eng Business finance @else  商业金融 @endif */
    ANCS_CATEGORY_LOCATION = 10,        /*!< @if Eng Location         @else  位置 @endif */
    ANCS_CATEGORY_ENTERTAINMENT = 11,   /*!< @if Eng Entertainment    @else  娱乐 @endif */
    ANCS_CATEGORY_RESERVED              /*!< @if Eng reserved         @else 预留分类ID    @endif */
} ancs_category_id_t;


/**
 * @if Eng
 * @brief  Enum of notify attribute id
 * @else
 * @brief  通知属性ID枚举值
 * @endif
 */
typedef enum {
    ANCS_NOTIFY_ATT_APP_ID = 0,          /*!< @if Eng App id         @else 应用编号 @endif */
    ANCS_NOTIFY_ATT_TITLE = 1,           /*!< @if Eng Title          @else  标题 @endif */
    ANCS_NOTIFY_ATT_SUBTITLE = 2,        /*!< @if Eng Subtitle       @else 副标题 @endif */
    ANCS_NOTIFY_ATT_MESSAGE = 3,         /*!< @if Eng Message        @else 消息 @endif */
    ANCS_NOTIFY_ATT_MSG_SIZE = 4,        /*!< @if Eng Message size   @else 消息大小 @endif */
    ANCS_NOTIFY_ATT_DATE = 5,            /*!< @if Eng Date           @else 日期 @endif */
    ANCS_NOTIFY_ATT_POSITIVE_LABEL = 6,  /*!< @if Eng Positive lable @else  积极活动标签 @endif */
    ANCS_NOTIFY_ATT_NEGATIVE_LABEL = 7,  /*!< @if Eng Negative lable @else  消极活动标签 @endif */
    ANCS_NOTIFY_ATT_RESERVED             /*!< @if Eng reserved       @else 预留通知属性ID @endif */
} ancs_notification_att_id_t;

/**
 * @if Eng
 * @brief  Enum of app attribute id
 * @else
 * @brief  应用属性ID枚举值
 * @endif
 */
typedef enum {
    ANCS_APP_ATT_DISPLAY_NAME = 0,          /*!< @if Eng Display name @else 应用显示名称 @endif */
    ANCS_APP_ATT_RESERVED                   /*!< @if Eng reserved     @else 预留应用属性ID @endif */
} ancs_app_att_id_t;

/**
 * @if Eng
 * @brief  Enum of action id
 * @else
 * @brief  活动ID枚举值
 * @endif
 */
typedef enum {
    ANCS_ACTION_POSITIVE = 0,          /*!< @if Eng Positive   @else 积极活动 @endif */
    ANCS_ACTION_NEGATIVE = 1,          /*!< @if Eng Negative   @else 消极活动 @endif */
    ANCS_ACTION_RESERVED               /*!< @if Eng Reserved   @else 预留活动ID @endif */
} ancs_action_id_t;

/**
 * @if Eng
 * @brief  Enum of subscribe type
 * @else
 * @brief  订阅类型
 * @endif
 */
typedef enum {
    ANCS_CLIENT_SUBSCRIBE_NOTIFY = 0,        /*!< @if Eng Subscribe notify @else 订阅通知 @endif */
    ANCS_CLIENT_SUBSCRIBE_DATA = 1,          /*!< @if Eng Subscribe data @else 订阅数据 @endif */
} ancs_client_subscribe_type_t;

/**
 * @if Eng
 * @brief  Enum of ancs servie character.
 * @else
 * @brief  ANCS服务特征。
 * @endif
 */
typedef enum {
    ANCS_CHAR_NOTIFY_SRC = 0,   /*!< @if Eng Notify source @else 通知源 @endif */
    ANCS_CHAR_CTRL_POINT = 1,   /*!< @if Eng Control point @else 控制点 @endif */
    ANCS_CHAR_DATA_SRC = 2,     /*!< @if Eng Data Source   @else 数据源 @endif */
    ANCS_CHAR_INVALID,
} ancs_character_t;

/**
 * @if Eng
 * @brief  Enum of ancs error codes.
 * @else
 * @brief  ANCS客户端写特征响应错误码。
 * @endif
 */
typedef enum {
    ANCS_ERROR_CODE_UNKNOW_CMD = 0xA0,        /*!< @if Eng Unknown command @else 未知命令 @endif */
    ANCS_ERROR_CODE_INVALID_CMD = 0xA1,       /*!< @if Eng Invalid command @else 无效命令 @endif */
    ANCS_ERROR_CODE_INVALID_PARAM = 0xA2,     /*!< @if Eng Invalid parameter @else 无效参数 @endif */
    ANCS_ERROR_CODE_ACTION_FAILED = 0xA3      /*!< @if Eng The action was not performed @else 行为没有被执行 @endif */
} ancs_write_error_code_t;

/**
 * @if Eng
 * @brief  Struct of ancs servie.
 * @else
 * @brief  ANCS服务。
 * @endif
 */
typedef struct {
    struct osal_list_head node;
    uint8_t client_id;                                 /*!< @if Eng Client id @else    客户端ID @endif */
    uint16_t conn_id;                                  /*!< @if Eng Connect id @else   连接ID @endif */
    uint16_t start_handle;                             /*!< @if Eng Start handle @else 开始handle @endif */
    uint16_t end_handle;                               /*!< @if Eng End handle @else   结束handle @endif */
    gattc_discovery_character_result_t notify_src;     /*!< @if Eng Notify source @else 通知源 @endif */
    gattc_discovery_character_result_t ctl_point;      /*!< @if Eng Control point @else 控制点 @endif */
    gattc_discovery_character_result_t data_src;       /*!< @if Eng Data Source   @else 数据源 @endif */
    uint16_t notify_src_ccc_handle;                    /*!< @if Eng Notify source CCC handle
                                                            @else 通知源CCC描述符handle @endif */
    uint16_t data_src_ccc_handle;                      /*!< @if Eng Data source CCC handle
                                                            @else 数据源CCC描述符handle @endif */
} ble_ancs_service_t;


/**
 * @if Eng
 * @brief  Struct of notification info.
 * @else
 * @brief  通知消息结构体。
 * @endif
 */
typedef struct {
    uint8_t event_id;                   /*!< @if Eng Event id {@ref ancs_event_id_t}
                                             @else 事件ID {@ref ancs_event_id_t} @endif */
    uint8_t event_flags;                /*!< @if Eng Event flags {@ref ancs_event_flags_t}
                                             @else 事件标记 {@ref ancs_event_flags_t} @endif */
    uint8_t category_id;                /*!< @if Eng Category id {@ref ancs_category_id_t}
                                             @else 分类ID {@ref ancs_category_id_t} @endif */
    uint8_t category_count;             /*!< @if Eng Category count @else 分类消息个数 @endif */
    uint32_t notification_uid;          /*!< @if Eng Notify uuid @else 通知ID @endif */
} ble_ancs_notification_src_t;

/**
 * @if Eng
 * @brief Struct of attribute request
 * @else
 * @brief  属性请求结构体。
 * @endif
 */
typedef struct {
    uint8_t att_id;                 /*!< @if Eng Attribute id @else 属性ID @endif */
    uint16_t att_max_length;        /*!< @if Eng Attribute max length
                                         @else 属性值最大长度，仅用于通知属性【标题】【子标题】【消息】属性，其他属性传0 @endif */
} ble_ancs_attr_req_t;

/**
 * @if Eng
 * @brief  Struct of notify attribute request
 * @else
 * @brief  通知属性请求结构体。
 * @endif
 */
typedef struct {
    uint32_t notification_uid;          /*!< @if Eng Notify uuid @else 通知ID @endif */
    uint16_t att_num;                   /*!< @if Eng Attribute num @else 属性个数 @endif */
    ble_ancs_attr_req_t attr[0];        /*!< @if Eng Attribute array @else 属性ID数组 @endif */
} ble_ancs_notification_attr_req_t;

/**
 * @if Eng
 * @brief  Struct of attribute response
 * @else
 * @brief  属性响应结构体。
 * @endif
 */
typedef struct {
    uint8_t att_id;                 /*!< @if Eng Attribute id     @else 属性ID @endif */
    uint16_t att_length;            /*!< @if Eng Attribute length @else 属性长度 @endif */
    uint8_t *att;                   /*!< @if Eng Attribute value  @else 属性值 @endif */
} ble_ancs_attr_rsp_t;

/**
 * @if Eng
 * @brief  Struct of notify attribute response
 * @else
 * @brief  通知属性响应结构体。
 * @endif
 */
typedef struct {
    uint32_t notification_uid;     /*!< @if Eng Notify uuid @else 通知ID @endif */
    uint16_t att_num;              /*!< @if Eng Attribute num @else 属性个数 @endif */
    struct osal_list_head attr_rsp_lst; /*!< @if Eng Attribute list {@ref ble_ancs_attr_rsp_t}
                                               @else 属性列表 {@ref ble_ancs_attr_rsp_t} @endif */
} ble_ancs_notification_attr_rsp_t;

/**
 * @if Eng
 * @brief  Struct of APP id
 * @else
 * @brief  应用ID
 * @endif
 */
typedef struct {
    uint16_t len;         /*!< @if Eng Length @else 长度 @endif */
    uint8_t *id;          /*!< @if Eng Id @else ID @endif */
} ble_ancs_app_id_t;

/**
 * @if Eng
 * @brief  Struct of app attribute request
 * @else
 * @brief  应用属性请求结构体。
 * @endif
 */
typedef struct {
    ble_ancs_app_id_t app_id;      /*!< @if Eng App id @else 应用ID @endif */
    uint16_t att_num;              /*!< @if Eng Attribute num @else 属性个数 @endif */
    uint8_t attr_id[0];            /*!< @if Eng Attribute Array {@ref ancs_app_att_id_t}
                                        @else 属性ID数组 {@ref ancs_app_att_id_t} @endif */
} ble_ancs_app_attr_req_t;

/**
 * @if Eng
 * @brief Struct of app attribute response
 * @else
 * @brief  应用属性响应结构体。
 * @endif
 */
typedef struct {
    ble_ancs_app_id_t app_id;      /*!< @if Eng App id @else 应用ID @endif */
    uint16_t att_num;              /*!< @if Eng Attribute num @else 属性个数 @endif */
    struct osal_list_head attr_rsp_lst; /*!< @if Eng Attribute list {@ref ble_ancs_attr_rsp_t}
                                               @else 属性列表 {@ref ble_ancs_attr_rsp_t} @endif */
} ble_ancs_app_attr_rsp_t;


/**
 * @if Eng
 * @brief Struct of perform action request
 * @else
 * @brief  通知处理请求结构体。
 * @endif
 */
typedef struct {
    uint32_t notification_uid;        /*!< @if Eng Notify uuid @else 通知ID @endif */
    uint8_t action_id;                /*!< @if Eng Action id {@ref ancs_action_id_t}
                                           @else 行为ID {@ref ancs_action_id_t} @endif */
} ble_ancs_notification_action_req_t;

/**
 * @if Eng
 * @brief Callback invoked when client setup service complete.
 * @par Callback invoked when client setup service complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  ANCS客户端服务建立完成回调函数。
 * @par    ANCS客户端服务建立完成回调函数。
 * @attention
 * @param [in] client_id 客户端ID。
 * @param [in] conn_id   连接ID。
 * @param [in] status    执行结果错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_setup_result_cbk)(uint8_t client_id, uint16_t conn_id, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when client subscribe character notify complete.
 * @par Callback invoked when client subscribe character notify complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] type       subscribe type{ @ref ancs_client_subscribe_type_t }.
 * @param [in] status    gatt error code{ @ref gatt_status_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  ANCS客户端订阅特征通知结果回调函数。
 * @par    ANCS客户端订阅特征通知结果回调函数。
 * @attention
 * @param [in] client_id 客户端ID。
 * @param [in] conn_id   连接ID。
 * @param [in] type      订阅类型{ @ref ancs_client_subscribe_type_t }.
 * @param [in] status    gatt操作错误码{ @ref gatt_status_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_subscribe_result_cbk)(uint8_t client_id, uint16_t conn_id,
    ancs_client_subscribe_type_t type, gatt_status_t status);

/**
 * @if Eng
 * @brief Callback invoked when remote service push notify message.
 * @par Callback invoked when remote service push notify message.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] notify_src notify message { @ref ble_ancs_notification_src_t }.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  ANCS远端服务推送通知消息回调函数。
 * @par    ANCS远端服务推送通知消息回调函数。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] notify_src      通知消息 { @ref ble_ancs_notification_src_t }.
 * @param [in] status    操作错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_notify_source_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_src_t *notify_src, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when remote service push notify attribute info.
 * @par Callback invoked when remote service push notify attribute info.
 * @attention
 * @param [in] client_id       client ID.
 * @param [in] conn_id         connect id.
 * @param [in] notify_attr_rsp notify attribute info { @ref ble_ancs_notification_attr_rsp_t }.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  ANCS远端服务推送通知属性信息回调函数。
 * @par    ANCS远端服务推送通知属性信息回调函数。
 * @attention
 * @param [in] client_id            客户端ID。
 * @param [in] conn_id              连接ID。
 * @param [in] notify_attr_rsp      通知属性信息 { @ref ble_ancs_notification_attr_rsp_t }.
 * @param [in] status    操作错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_notification_attr_rsp_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_attr_rsp_t *notify_attr_rsp, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when remote service push app attribute info.
 * @par Callback invoked when remote service push app attribute info.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] app_attr_rsp app attribute info { @ref ble_ancs_app_attr_rsp_t }.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  ANCS远端服务推送应用属性信息回调函数。
 * @par    ANCS远端服务推送应用属性信息回调函数。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] app_attr_rsp    应用属性信息 { @ref ble_ancs_app_attr_rsp_t }.
 * @param [in] status    操作错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_app_attr_rsp_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ancs_app_attr_rsp_t *app_attr_rsp, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when client write contro point complete.
 * @par Callback invoked when client write contro point complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] status    gatt error code{ @ref gatt_status_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see ble_ancs_client_callbacks_t
 * @else
 * @brief  AMS客户端写控制点特征响应回调函数。
 * @par    AMS客户端写控制点特征响应回调函数。
 * @attention
 * @param [in] client_id      客户端ID。
 * @param [in] conn_id        连接ID。
 * @param [in] status         gatt操作错误码{ @ref gatt_status_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see ble_ancs_client_callbacks_t
 * @endif
 */
typedef void (*ancs_client_write_result_cbk)(uint8_t client_id, uint16_t conn_id, gatt_status_t status);

/**
 * @if Eng
 * @brief Struct of GATT client callback function.
 * @else
 * @brief GATT client回调函数接口定义。
 * @endif
 */
typedef struct {
    ancs_client_setup_result_cbk           setup_cb;          /*!< @if Eng setup callback @else 客户端服务建立完成回调 @endif */
    ancs_client_subscribe_result_cbk        subscribe_cb;     /*!< @if Eng subscribe callback @else 订阅特征通知结果回调 @endif */
    ancs_client_notify_source_cbk           notify_source_cb; /*!< @if Eng notify source callback
                                                                   @else 远端服务推送通知消息回调 @endif */
    ancs_client_notification_attr_rsp_cbk   notify_attr_cb;   /*!< @if Eng notify attribute callback
                                                                   @else 远端服务推送通知属性信息回调 @endif */
    ancs_client_app_attr_rsp_cbk            app_attr_cb;      /*!< @if Eng app attribute callback
                                                                   @else 远端服务推送应用属性信息回调 @endif */
    ancs_client_write_result_cbk            write_result_cb;  /*!< @if Eng write result callback
                                                                   @else 客户端写控制点特征请求响应回调 @endif */
} ble_ancs_client_callbacks_t;

/**
 * @if Eng
 * @brief The interface of ANCS client setup service.
 * @par The interface of ANCS client setup service.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端发起服务建立。
 * @par    ANCS客户端发起服务建立。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_setup_service(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief The interface of ANCS client discovery service result callback.
 * @par The interface of ANCS client discovery service result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] service    service info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  ANCS客户端发现服务结果处理接口。
 * @par    ANCS客户端发现服务结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] service    ANCS 服务信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ancs_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service);

/**
 * @if Eng
 * @brief The interface of ANCS client discovery character result callback.
 * @par The interface of ANCS client discovery character result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] character    character info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  ANCS客户端发现特征结果处理接口。
 * @par    ANCS客户端发现特征结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] character    ANCS 服务特征信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ancs_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character);

/**
 * @if Eng
 * @brief The interface of ANCS client discovery descriptor result callback.
 * @par The interface of ANCS client discovery descriptor result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] descriptor    descriptor info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  ANCS客户端发现描述符结果处理接口。
 * @par    ANCS客户端发现描述符结果处理接口。
 * @attention
 * @param [in] client_id     客户端ID。
 * @param [in] conn_id       连接ID。
 * @param [in] descriptor    ANCS服务特征描述符信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ancs_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor);

/**
 * @if Eng
 * @brief The interface of ANCS client receive notify info callback.
 * @par The interface of ANCS client receive notify info callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] data       notify info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  ANCS客户端接收对端通知信息处理接口。
 * @par    ANCS客户端接收对端通知信息处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] data       对端通知信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ancs_client_notify_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data);

/**
 * @if Eng
 * @brief The interface of ANCS client write response callback.
 * @par The interface of ANCS client write response callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] status     gatt status {@ref gatt_status_t}.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see
 * @else
 * @brief  ANCS客户端写请求响应结果处理接口。
 * @par    ANCS客户端写请求响应结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] status     gatt 操作结果码 {@ref gatt_status_t}.
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see
 * @endif
 */
void ble_ancs_client_write_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status);

/**
 * @if Eng
 * @brief The interface of ANCS client subscribe notify source character notify.
 * @par The interface of ANCS client subscribe notify source character notify.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] enable_flag    enable/disable flag {@ref ancs_client_notify_enable_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端订阅通知源特征通知接口。
 * @par    ANCS客户端订阅通知源特征通知接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] enable_flag     开启/关闭标记。 {@ref ancs_client_notify_enable_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_subscribe_notification_source(uint8_t client_id, uint16_t conn_id,
    ancs_client_notify_enable_t enable_flag);

/**
 * @if Eng
 * @brief The interface of ANCS client subscribe data source character notify.
 * @par The interface of ANCS client subscribe data source character notify.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] enable_flag    enable/disable flag {@ref ancs_client_notify_enable_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端订阅数据源特征通知接口。
 * @par    ANCS客户端订阅数据源特征通知接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] enable_flag     开启/关闭标记。 {@ref ancs_client_notify_enable_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_subscribe_data_source(uint8_t client_id, uint16_t conn_id,
    ancs_client_notify_enable_t enable_flag);

/**
 * @if Eng
 * @brief The interface of ANCS client send get notify attribute command.
 * @par The interface of ANCS client send get notify attribute command.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] notify_att_req   notify attribute request {@ref ble_ancs_notification_attr_req_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端发送获取通知属性命令接口。
 * @par    ANCS客户端发送获取通知属性命令接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] notify_att_req  通知属性请求。 {@ref ble_ancs_notification_attr_req_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_send_get_notify_att_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_attr_req_t *notify_att_req);

/**
 * @if Eng
 * @brief The interface of ANCS client send get app attribute command.
 * @par The interface of ANCS client send get app attribute command.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] app_att_req   app attribute request {@ref ble_ancs_app_attr_req_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端发送获取应用属性命令接口。
 * @par    ANCS客户端发送获取应用属性命令接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] app_att_req  应用属性请求。 {@ref ble_ancs_app_attr_req_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_send_get_app_att_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_app_attr_req_t *app_att_req);

/**
 * @if Eng
 * @brief The interface of ANCS client send perform notify action command.
 * @par The interface of ANCS client send perform notify action command.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] action_req     perform action request {@ref ble_ancs_notification_action_req_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端发送执行通知行为命令接口。
 * @par    ANCS客户端发送执行通知行为命令接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] action_req      执行行为请求。 {@ref ble_ancs_notification_action_req_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_send_perform_notify_action_cmd(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_action_req_t *action_req);

/**
 * @if Eng
 * @brief The interface of ANCS client register callback.
 * @par The interface of ANCS client register callback.
 * @attention
 * @param [in] func        the pointer of callback struct.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  ANCS客户端注册回调接口。
 * @par    ANCS客户端注册回调接口。
 * @attention
 * @param [in] func         回调函数结构体指针。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ancs_client_register_callbacks(ble_ancs_client_callbacks_t *func);

/**
 * @if Eng
 * @brief The interface of check service uuid whether or not ANCS service.
 * @par The interface of check service uuid whether or not ANCS service.
 * @attention
 * @param [in] uuid         service uuid.
 * @retval true or false    true：Yes, false:No.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  ANCS客户端注册回调接口。
 * @par    ANCS客户端注册回调接口。
 * @attention
 * @param [in] uuid         服务uuid。
 * @retval 是或否 是：是ANCS服务，否：不是ANCS服务.
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
bool is_ble_ancs_service(bt_uuid_t *uuid);

/**
 * @if Eng
 * @brief The interface of ANCS client clear stored remote ANCS service info by connect id.
 * @par The interface of ANCS client clear stored remote ANCS service info by connect id.
 * @attention
 * @param [in] client_id         client ID.
 * @param [in] conn_id           connect id.
 * @retval #void no return value.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  ANCS客户端根据连接ID清理存储的对端ANCS服务信息。
 * @par    ANCS客户端根据连接ID清理存储的对端ANCS服务信息。
 * @attention
 * @param [in] client_id         客户端ID。
 * @param [in] conn_id           连接ID。
 * @retval 无返回值。
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
void ble_ancs_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief The interface of ANCS client clear all stored remote ANCS service info.
 * @par The interface of ANCS client clear all stored remote ANCS service info.
 * @attention
 * @retval #void no return value.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  ANCS客户端清理所有存储的对端ANCS服务信息。
 * @par    ANCS客户端清理所有存储的对端ANCS服务信息。
 * @attention
 * @retval 无返回值。
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
void ble_ancs_client_clear_remote_all(void);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif