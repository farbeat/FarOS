/*
 * Copyright (c) @CompanyNameMagicTag 2024. All rights reserved.
 * Description: BLE Apple Media Service Client module.
 */

/**
 * @defgroup bluetooth_bts_ams_client AMS CLIENT API
 * @ingroup  bluetooth
 * @{
 */
#ifndef BLE_AMS_CLIENT_H
#define BLE_AMS_CLIENT_H
#include "errcode.h"
#include "osal_list.h"
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
#define BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT 0x0000

/**
 * @if Eng
 * @brief  Client Characteristic Configuration value is notification.
 * @else
 * @brief  客户端特征配置描述符的值为通知。
 * @endif
 */
#define BLE_AMS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION 0x0001

/**
 * @if Eng
 * @brief  Enum of ams notify switch.
 * @else
 * @brief  通知开关
 * @endif
 */
typedef enum {
    AMS_CLIENT_NOTIFY_DISABLE, /*!< @if Eng disable notify @else 关闭通知 @endif */
    AMS_CLIENT_NOTIFY_ENABLE,  /*!< @if Eng enable notify @else 开启通知 @endif */
} ams_client_notify_enable_t;

/**
 * @if Eng
 * @brief  Enum of remote command
 * @else
 * @brief  远端命令ID
 * @endif
 */
typedef enum {
    AMS_RMT_CMD_ID_PLAY = 0,                  /*!< @if Eng play  @else 播放 @endif */
    AMS_RMT_CMD_ID_PAUSE = 1,                 /*!< @if Eng pause  @else 暂停 @endif */
    AMS_RMT_CMD_ID_TOGGLE_PLAY_PAUSE = 2,     /*!< @if Eng toggle play or pause @else 切换播放/暂停 @endif */
    AMS_RMT_CMD_ID_NEXT_TRACK = 3,            /*!< @if Eng next track  @else 下一首 @endif */
    AMS_RMT_CMD_ID_PREVIOUS_TRACK = 4,        /*!< @if Eng previous track  @else 上一首 @endif */
    AMS_RMT_CMD_ID_VOLUME_UP = 5,             /*!< @if Eng volume up  @else 调高音量 @endif */
    AMS_RMT_CMD_ID_VOLUME_DOWN = 6,           /*!< @if Eng volume down  @else 调低音量 @endif */
    AMS_RMT_CMD_ID_ADVANCE_REPEAT_MODE = 7,   /*!< @if Eng advance repeat mode  @else 高级重复模式 @endif */
    AMS_RMT_CMD_ID_ADVANCE_SHUFFLE_MODE = 8,  /*!< @if Eng advance shuffle mode  @else 高级随机模式 @endif */
    AMS_RMT_CMD_ID_SKIP_FORWARD = 9,          /*!< @if Eng skip forward  @else 跳过前一个 @endif */
    AMS_RMT_CMD_ID_SKIP_BACKWARD = 10,        /*!< @if Eng skip backward  @else 跳过后一个 @endif */
    AMS_RMT_CMD_ID_LIKE_TRACK = 11,           /*!< @if Eng like track  @else 喜欢 @endif */
    AMS_RMT_CMD_ID_DISLIKE_TRACK = 12,        /*!< @if Eng dislike track  @else 取消喜欢 @endif */
    AMS_RMT_CMD_ID_BOOKMARK_TRACK = 13,       /*!< @if Eng bookmark track @else 标记喜欢或取消喜欢 @endif */
    AMS_RMT_CMD_ID_RESERVED                   /*!< @if Eng reserved  @else 预留命令ID @endif */
} ams_remote_cmd_id_t;

/**
 * @if Eng
 * @brief  Enum of entity id
 * @else
 * @brief  实体ID
 * @endif
 */
typedef enum {
    AMS_ENTITY_ID_PLAYER = 0,  /*!< @if Eng player  @else 播放器 @endif */
    AMS_ENTITY_ID_QUEUE = 1,   /*!< @if Eng queue   @else 播放列表 @endif */
    AMS_ENTITY_ID_TRACK = 2,   /*!< @if Eng track   @else 曲目 @endif */
    AMS_ENTITY_ID_RESERVED     /*!< @if Eng reserved  @else 预留实体ID @endif */
} ams_entity_id_t;

/**
 * @if Eng
 * @brief  Enum of entity update flags
 * @else
 * @brief  实体更新标记
 * @endif
 */
typedef enum {
    AMS_ENTITY_UPDATE_FLAG_TRUNCATED = (1 << 0),  /*!< @if Eng truncated  @else 销毁 @endif */
    AMS_ENTITY_UPDATE_FLAG_RESERVED               /*!< @if Eng reserved  @else 预留标记 @endif */
} ams_entity_updata_flag_t;

/**
 * @if Eng
 * @brief  Enum of player attribute id
 * @else
 * @brief  播放器属性ID
 * @endif
 */
typedef enum {
    AMS_PALYER_ATT_ID_NAME = 0,                /*!< @if Eng name  @else 播放器名称 @endif */
    AMS_PALYER_ATT_ID_PLAYBACK_INFO = 1,       /*!< @if Eng playback info  @else 播放信息 @endif */
    AMS_PALYER_ATT_ID_VOLUME = 2,              /*!< @if Eng volume  @else 音量,0-1的浮点数 @endif */
    AMS_PALYER_ATT_ID_RESERVED                 /*!< @if Eng reserved  @else 预留属性ID @endif */
} ams_player_att_id_t;

/**
 * @if Eng
 * @brief  Enum of queue attribute id
 * @else
 * @brief  队列属性ID
 * @endif
 */
typedef enum {
    AMS_QUEUE_ATT_ID_INDEX = 0,          /*!< @if Eng index  @else 序号，从0开始 @endif */
    AMS_QUEUE_ATT_ID_COUNT = 1,          /*!< @if Eng count  @else 列表个数 @endif */
    AMS_QUEUE_ATT_ID_SHUFFLE_MODE = 2,   /*!< @if Eng shuffle mode  @else 随机模式 @endif */
    AMS_QUEUE_ATT_ID_REPEAT_MODE = 3,    /*!< @if Eng repeat mode  @else 重复模式 @endif */
    AMS_QUEUE_ATT_ID_RESERVED            /*!< @if Eng reserved  @else 预留属性ID @endif */
} ams_queue_att_id_t;


/**
 * @if Eng
 * @brief  Enum of track attribute id
 * @else
 * @brief  曲目属性ID
 * @endif
 */
typedef enum {
    AMS_TRACK_ATT_ID_ARTIST = 0,        /*!< @if Eng artist  @else 艺术家 @endif */
    AMS_TRACK_ATT_ID_ALBUM = 1,         /*!< @if Eng album  @else 专辑 @endif */
    AMS_TRACK_ATT_ID_TITLE = 2,         /*!< @if Eng title  @else 标题 @endif */
    AMS_TRACK_ATT_ID_DURATION = 3,      /*!< @if Eng duration  @else 音乐时长，秒级，浮点数 @endif */
    AMS_TRACK_ATT_ID_RESERVED           /*!< @if Eng reserved  @else 预留属性ID @endif */
} ams_track_att_id_t;

/**
 * @if Eng
 * @brief  Enum of shuffle mode
 * @else
 * @brief  随机模式
 * @endif
 */
typedef enum {
    AMS_SHUFFLE_MODE_OFF = 0,        /*!< @if Eng off  @else 关闭 @endif */
    AMS_SHUFFLE_MODE_ONE = 1,        /*!< @if Eng one  @else 随机模式1 @endif */
    AMS_SHUFFLE_MODE_ALL = 2,        /*!< @if Eng all  @else 随机模式2 @endif */
    AMS_SHUFFLE_MODE_RESERVED        /*!< @if Eng reserved  @else 预留随机模式 @endif */
} ams_shuffle_mode_t;

/**
 * @if Eng
 * @brief  Enum of repeat mode
 * @else
 * @brief  重复模式
 * @endif
 */
typedef enum {
    AMS_REPEAT_MODE_OFF = 0,        /*!< @if Eng off  @else 关闭 @endif */
    AMS_REPEAT_MODE_ONE = 1,        /*!< @if Eng one  @else 重复模式1 @endif */
    AMS_REPEAT_MODE_ALL = 2,        /*!< @if Eng all  @else 重复模式2 @endif */
    AMS_REPEAT_MODE_RESERVED        /*!< @if Eng reserved  @else 预留重复模式 @endif */
} ams_repeat_mode_t;

/**
 * @if Eng
 * @brief  Enum of subscribe type
 * @else
 * @brief  订阅类型
 * @endif
 */
typedef enum {
    AMS_CLIENT_SUBSCRIBE_REMOTE_CMD = 0,          /*!< @if Eng subscribe remote command @else 订阅远端命令 @endif */
    AMS_CLIENT_SUBSCRIBE_ENTITY_UPDATE = 1,      /*!< @if Eng subscribe entity update @else 订阅实体更新 @endif */
} ams_client_subscribe_type_t;


/**
 * @if Eng
 * @brief  Enum of write character
 * @else
 * @brief  写特征类型
 * @endif
 */
typedef enum {
    AMS_CLIENT_WRITE_REMOTE_CMD = 0,          /*!< @if Eng write remote command   @else 写远端命令 @endif */
    AMS_CLIENT_WRITE_ENTITY_UPDATE = 1,      /*!< @if Eng write entity update    @else 写实体更新 @endif */
    AMS_CLIENT_WRITE_ENTITY_ATTRIBUTE = 2,   /*!< @if Eng write entity attribute @else 写实体属性 @endif */
} ams_client_write_character_type_t;

/**
 * @if Eng
 * @brief  Enum of ams character.
 * @else
 * @brief  AMS服务特征。
 * @endif
 */
typedef enum {
    AMS_CHAR_REMOTE_CMD,         /*!< @if Eng remote command @else 远端命令 @endif */
    AMS_CHAR_ENTITY_UPDATE,      /*!< @if Eng entity update @else 实体更新 @endif */
    AMS_CHAR_ENTITY_ATTR,        /*!< @if Eng entity update @else 实体更新 @endif */
    AMS_CHAR_INVALID,            /*!< @if Eng invalid @else 无效特征 @endif */
} ams_character_t;

/**
 * @if Eng
 * @brief  Enum of ams error codes.
 * @else
 * @brief  AMS客户端写和读特征响应错误码。
 * @endif
 */
typedef enum {
    AMS_ERROR_CODE_INVALID_STATE = 0xA0,        /*!< @if Eng Invalid State
                                                     @else 无效状态，客户端未发起服务建立 @endif */
    AMS_ERROR_CODE_INVALID_CMD = 0xA1,          /*!< @if Eng Invalid command @else 无效命令 @endif */
    AMS_ERROR_CODE_ABSENT_ATTRIBUTE = 0xA2     /*!< @if Eng Absent attribute @else 属性为空 @endif */
} ams_write_error_code_t;

/**
 * @if Eng
 * @brief  Struct of ams service.
 * @else
 * @brief  AMS服务。
 * @endif
 */
typedef struct {
    struct osal_list_head node;
    uint8_t client_id;                                   /*!< @if Eng client id @else 客户端ID @endif */
    uint16_t conn_id;                                    /*!< @if Eng connect id @else 连接ID @endif */
    uint16_t start_handle;                               /*!< @if Eng start handle @else 服务开始handle @endif */
    uint16_t end_handle;                                 /*!< @if Eng end handle @else   服务结束handle @endif */
    gattc_discovery_character_result_t remote_cmd;       /*!< @if Eng remote command @else 远端命令特征 @endif */
    gattc_discovery_character_result_t entity_update;    /*!< @if Eng entity update @else 实体更新特征 @endif */
    gattc_discovery_character_result_t entity_attr;      /*!< @if Eng entity attribute @else 实体属性特征 @endif */
    uint16_t remote_cmd_ccc_handle;                      /*!< @if Eng remote command ccc
                                                              @else 远端命令特征ccc描述符handle @endif */
    uint16_t entity_update_ccc_handle;                   /*!< @if Eng entity update ccc
                                                              @else 实体更新特征ccc描述符handle @endif */
} ble_ams_service_t;

/**
 * @if Eng
 * @brief  Struct of interested entity attribute request.
 * @else
 * @brief  感兴趣实体属性请求。
 * @endif
 */
typedef struct {
    uint8_t entity_id;                   /*!< @if Eng entity id @else 实体ID @endif */
    uint16_t att_num;                    /*!< @if Eng the count of attribute @else 属性个数 @endif */
    uint8_t att_id[0];                   /*!< @if Eng the array of attribute @else 属性ID数组 @endif */
} ble_ams_interested_entity_attr_req_t;

/**
 * @if Eng
 * @brief  Struct of full entity attribute request.
 * @else
 * @brief  完整实体属性请求。
 * @endif
 */
typedef struct {
    uint8_t entity_id;    /*!< @if Eng entity id @else 实体ID @endif */
    uint8_t att_id;       /*!< @if Eng attribute id @else 属性ID @endif */
} ble_ams_full_entity_attr_cmd_t;

/**
 * @if Eng
 * @brief Struct of supported remote command.
 * @else
 * @brief  支持的远端命令。
 * @endif
 */
typedef struct {
    uint16_t cmd_num;      /*!< @if Eng the count of command @else 支持的远端命令个数 @endif */
    uint8_t remote_cmd[0]; /*!< @if Eng the array of remote command @else 支持的远端命令数组 @endif */
} ble_ams_remote_cmd_rpt_t;

/**
 * @if Eng
 * @brief Struct of entity update info.
 * @else
 * @brief  实体更新信息。
 * @endif
 */
typedef struct {
    uint8_t entity_id;               /*!< @if Eng entity id @else 实体ID @endif */
    uint8_t att_id;                  /*!< @if Eng attribute id @else 属性ID @endif */
    uint8_t entity_update_flags;     /*!< @if Eng entity update flags @else 实体更新标记 @endif */
    uint16_t value_length;           /*!< @if Eng the length of entity value @else 属性值长度 @endif */
    uint8_t value[0];                /*!< @if Eng entity value @else 属性值字节序（字符串，以0结尾），上层应用根据协议解析 @endif */
} ble_ams_entity_update_rpt_t;

/**
 * @if Eng
 * @brief Struct of full entity attribute.
 * @else
 * @brief  完整实体属性信息。
 * @endif
 */
typedef struct {
    uint8_t entity_id;      /*!< @if Eng entity id @else 实体ID @endif */
    uint8_t att_id;         /*!< @if Eng attribute id @else 属性ID @endif */
    uint16_t value_len;     /*!< @if Eng the length of entity value @else 属性值长度 @endif */
    uint8_t value[0];       /*!< @if Eng entity value @else 属性值字节序（字符串，以0结尾） @endif */
} ble_ams_full_attr_value_t;


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
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS客户端服务建立完成回调函数。
 * @par    AMS客户端服务建立完成回调函数。
 * @attention
 * @param [in] client_id 客户端ID。
 * @param [in] conn_id   连接ID。
 * @param [in] status    执行结果错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_setup_result_cbk)(uint8_t client_id, uint16_t conn_id, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when client subscribe character notify complete.
 * @par Callback invoked when client subscribe character notify complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] type       subscribe type{ @ref ams_client_subscribe_type_t }.
 * @param [in] status    gatt error code{ @ref gatt_status_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS客户端订阅特征通知结果回调函数。
 * @par    AMS客户端订阅特征通知结果回调函数。
 * @attention
 * @param [in] client_id 客户端ID。
 * @param [in] conn_id   连接ID。
 * @param [in] type      订阅类型{ @ref ams_client_subscribe_type_t }.
 * @param [in] status    gatt操作错误码{ @ref gatt_status_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_subscribe_result_cbk)(uint8_t client_id, uint16_t conn_id,
    ams_client_subscribe_type_t type, gatt_status_t status);

/**
 * @if Eng
 * @brief Callback invoked when remote service notify supported remote command.
 * @par Callback invoked when remote service notify supported remote command.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] remote_cmd supported remote command{ @ref ble_ams_remote_cmd_rpt_t }.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS远端服务通知支持的远端命令回调函数。
 * @par    AMS远端服务通知支持的远端命令回调函数。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] remote_cmd      支持的远端命令{ @ref ble_ams_remote_cmd_rpt_t }.
 * @param [in] status    操作错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_remote_command_rpt_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ams_remote_cmd_rpt_t *remote_cmd, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when remote service notify entity update info.
 * @par Callback invoked when remote service notify entity update info.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] entity_update entity update info{ @ref ble_ams_entity_update_rpt_t }.
 * @param [in] status    error code{ @ref errcode_bt_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_def.h
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS远端服务通知实体更新信息回调函数。
 * @par    AMS远端服务通知实体更新信息回调函数。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] entity_update   实体更新信息{ @ref ble_ams_entity_update_rpt_t }.
 * @param [in] status    操作错误码{ @ref errcode_bt_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_def.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_entity_update_rpt_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ams_entity_update_rpt_t *entity_update, errcode_t status);

/**
 * @if Eng
 * @brief Callback invoked when client write remote character complete.
 * @par Callback invoked when client write remote character complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] type       character type{ @ref ams_client_write_character_type_t }.
 * @param [in] status    gatt error code{ @ref gatt_status_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS客户端写对端特征响应回调函数。
 * @par    AMS客户端写对端特征响应回调函数。
 * @attention
 * @param [in] client_id      客户端ID。
 * @param [in] conn_id        连接ID。
 * @param [in] type           特征类型{ @ref ams_client_write_character_type_t }.
 * @param [in] status         gatt操作错误码{ @ref gatt_status_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_write_character_rsp_cbk)(uint8_t client_id, uint16_t conn_id,
    ams_client_write_character_type_t type,
    gatt_status_t status);

/**
 * @if Eng
 * @brief Callback invoked when client read remote character complete.
 * @par Callback invoked when client read remote character complete.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] read_result full attribute value{ @ref ble_ams_full_attr_value_t }.
 * @param [in] status    gatt error code{ @ref gatt_status_t }.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @else
 * @brief  AMS客户端读对端特征响应回调函数。
 * @par    AMS客户端读对端特征响应回调函数。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] read_result     特征值{ @ref ble_ams_full_attr_value_t }.
 * @param [in] status    gatt操作错误码{ @ref gatt_status_t }。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @see ble_ams_client_callbacks_t
 * @endif
 */
typedef void (*ams_client_read_full_attr_result_cbk)(uint8_t client_id, uint16_t conn_id,
    ble_ams_full_attr_value_t *read_result,
    gatt_status_t status);

/**
 * @if Eng
 * @brief Struct of AMS client callback function.
 * @else
 * @brief AMS 客户端回调函数接口定义。
 * @endif
 */
typedef struct {
    ams_client_setup_result_cbk               setup_cb;          /*!< @if Eng setup callback @else 客户端服务建立完成回调 @endif */
    ams_client_subscribe_result_cbk           subscribe_cb;      /*!< @if Eng subscribe callback
                                                                      @else 订阅特征通知结果回调 @endif */
    ams_client_remote_command_rpt_cbk         remote_cmd_cb;     /*!< @if Eng suppported remote command callback
                                                                      @else 远端服务通知支持的远端命令回调 @endif */
    ams_client_entity_update_rpt_cbk          entity_update_cb;  /*!< @if Eng entity update callback
                                                                      @else 远端服务通知实体更新信息回调 @endif */
    ams_client_write_character_rsp_cbk        write_rsp_cb;      /*!< @if Eng write response callback
                                                                      @else 客户端写请求响应回调 @endif */
    ams_client_read_full_attr_result_cbk      read_full_attr_result_cb; /*!< @if Eng read response callback
                                                                             @else 客户端读请求响应回调 @endif */
} ble_ams_client_callbacks_t;

/**
 * @if Eng
 * @brief The interface of AMS client setup service.
 * @par The interface of AMS client setup service.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端发起服务建立。
 * @par    AMS客户端发起服务建立。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_setup_service(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief The interface of AMS client discovery service result callback.
 * @par The interface of AMS client discovery service result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] service    service info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  AMS客户端发现服务结果处理接口。
 * @par    AMS客户端发现服务结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] service    AMS 服务信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ams_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service);

/**
 * @if Eng
 * @brief The interface of AMS client discovery character result callback.
 * @par The interface of AMS client discovery character result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] character    character info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  AMS客户端发现特征结果处理接口。
 * @par    AMS客户端发现特征结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] character    AMS 服务特征信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ams_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character);

/**
 * @if Eng
 * @brief The interface of AMS client discovery descriptor result callback.
 * @par The interface of AMS client discovery descriptor result callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] descriptor    descriptor info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  AMS客户端发现描述符结果处理接口。
 * @par    AMS客户端发现描述符结果处理接口。
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] descriptor    AMS 服务特征描述符信息。
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ams_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor);

/**
 * @if Eng
 * @brief The interface of AMS client receive notify info callback.
 * @par The interface of AMS client receive notify info callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] data       notify info.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  AMS客户端接收对端通知信息处理接口。
 * @par    AMS客户端接收对端通知信息处理接口。
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
void ble_ams_client_notify_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data);

/**
 * @if Eng
 * @brief The interface of AMS client write response callback.
 * @par The interface of AMS client write response callback.
 * @attention
 * @param [in] client_id  client ID.
 * @param [in] conn_id    connect id.
 * @param [in] status     gatt status {@ref gatt_status_t}.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @see
 * @else
 * @brief  AMS客户端写请求响应结果处理接口。
 * @par    AMS客户端写请求响应结果处理接口。
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
void ble_ams_client_write_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status);

/**
 * @if Eng
 * @brief The interface of AMS client read response callback.
 * @par The interface of AMS client read response callback.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] read_result    read response result.
 * @param [in] status         gatt status {@ref gatt_status_t}.
 * @retval #void no return value.
 * @par Dependency:
 * @li  bts_gatt_stru.h
 * @li  bts_gatt_client.h
 * @see
 * @else
 * @brief  AMS客户端读请求响应结果处理接口。
 * @par    AMS客户端读请求响应结果处理接口。
 * @attention
 * @param [in] client_id      客户端ID。
 * @param [in] conn_id        连接ID。
 * @param [in] read_result    读请求响应结果.
 * @param [in] status         gatt 操作结果码 {@ref gatt_status_t}.
 * @retval 无返回值。
 * @par 依赖:
 * @li  bts_gatt_stru.h
 * @li  bts_gatt_client.h
 * @see
 * @endif
 */
void ble_ams_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status);

/**
 * @if Eng
 * @brief The interface of AMS client subscribe remote command character notify.
 * @par The interface of AMS client subscribe remote command character notify.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] enable_flag    enable/disable flag {@ref ams_client_notify_enable_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端订阅远端命令特征通知接口。
 * @par    AMS客户端订阅远端命令特征通知接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] enable_flag     开启/关闭标记。 {@ref ams_client_notify_enable_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_subscribe_remote_cmd(uint8_t client_id, uint16_t conn_id,
    ams_client_notify_enable_t enable_flag);

/**
 * @if Eng
 * @brief The interface of AMS client subscribe entity update character notify.
 * @par The interface of AMS client subscribe entity update character notify.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] enable_flag    enable/disable flag {@ref ams_client_notify_enable_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端订阅实体更新特征通知接口。
 * @par    AMS客户端订阅实体更新特征通知接口。
 * @attention
 * @param [in] client_id       客户端ID。
 * @param [in] conn_id         连接ID。
 * @param [in] enable_flag     开启/关闭标记。 {@ref ams_client_notify_enable_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_subscribe_entity_update(uint8_t client_id, uint16_t conn_id,
    ams_client_notify_enable_t enable_flag);

/**
 * @if Eng
 * @brief The interface of AMS client send remote command.
 * @par The interface of AMS client send remote command, it is used for operating remote media action.
 * @attention
 * @param [in] client_id      client ID.
 * @param [in] conn_id        connect id.
 * @param [in] cmd_id         command id {@ref ams_remote_cmd_id_t}.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端发送远端命令接口。
 * @par    AMS客户端发送远端命令接口，该接口用来控制对端媒体行为，例如播放/暂停，调节音量等
 * @attention
 * @param [in] client_id  客户端ID。
 * @param [in] conn_id    连接ID。
 * @param [in] cmd_id     命令ID。 {@ref ams_remote_cmd_id_t}.
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_send_remote_cmd(uint8_t client_id, uint16_t conn_id,
    ams_remote_cmd_id_t cmd_id);

/**
 * @if Eng
 * @brief The interface of AMS client register interested entity attribute.
 * @par The interface of AMS client register interested entity attribute.
 * @attention
 * @param [in] client_id         client ID.
 * @param [in] conn_id           connect id.
 * @param [in] entity_attr_req   interested entity attribute list.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端注册感兴趣的实体属性接口。
 * @par    AMS客户端注册感兴趣的实体属性，当该实体属性更新时，对端会发送通知到本端。
 * @attention
 * @param [in] client_id           客户端ID。
 * @param [in] conn_id             连接ID。
 * @param [in] entity_attr_req     感兴趣的实体属性列表。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_register_interested_entity_attribute(uint8_t client_id, uint16_t conn_id,
    ble_ams_interested_entity_attr_req_t *entity_attr_req);

/**
 * @if Eng
 * @brief The interface of AMS client retrieve full entity attribute.
 * @par The interface of AMS client retrieve full entity attribute.
 * @attention
 * @param [in] client_id         client ID.
 * @param [in] conn_id           connect id.
 * @param [in] full_attr_req     entity attribute.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端请求对端加载完整的实体属性信息接口。
 * @par    AMS客户端请求对端加载完整的实体属性信息接口，稍后AMS客户端再发起读请求，读取完整的实体属性信息。
 * @attention
 * @param [in] client_id         客户端ID。
 * @param [in] conn_id           连接ID。
 * @param [in] full_attr_req     实体属性。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_retrieve_full_entity_attribute(uint8_t client_id, uint16_t conn_id,
    ble_ams_full_entity_attr_cmd_t *full_attr_req);

/**
 * @if Eng
 * @brief The interface of AMS client read full entity attribute.
 * @par The interface of AMS client read full entity attribute.
 * @attention
 * @param [in] client_id         client ID.
 * @param [in] conn_id           connect id.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端读取完整实体属性信息接口。
 * @par    AMS客户端读取完整完整实体属性信息接口。
 * @attention
 * @param [in] client_id         客户端ID。
 * @param [in] conn_id           连接ID。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_read_full_entity_attribute(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief The interface of AMS client register callback.
 * @par The interface of AMS client register callback.
 * @attention
 * @param [in] func        the pointer of callback struct.
 * @retval errcode_t {@ref errcode_bt_t}.
 * @par Dependency:
 * @li  bts_def.h
 * @see
 * @else
 * @brief  AMS客户端注册回调接口。
 * @par    AMS客户端注册回调接口。
 * @attention
 * @param [in] func         回调函数结构体指针。
 * @retval 操作错误码 {@ref errcode_bt_t}.
 * @par 依赖:
 * @li  bts_def.h
 * @see
 * @endif
 */
errcode_t ble_ams_client_register_callbacks(ble_ams_client_callbacks_t *func);

/**
 * @if Eng
 * @brief The interface of check service uuid whether or not AMS service.
 * @par The interface of check service uuid whether or not AMS service.
 * @attention
 * @param [in] uuid         service uuid.
 * @retval true or false    true：Yes, false:No.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  AMS客户端注册回调接口。
 * @par    AMS客户端注册回调接口。
 * @attention
 * @param [in] uuid         服务uuid。
 * @retval 是或否 是：是AMS服务，否：不是AMS服务.
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
bool is_ble_ams_service(bt_uuid_t *uuid);

/**
 * @if Eng
 * @brief The interface of AMS client clear stored remote ams service info by connect id.
 * @par The interface of AMS client clear stored remote ams service info by connect id.
 * @attention
 * @param [in] client_id         client ID.
 * @param [in] conn_id           connect id.
 * @retval #void no return value.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  AMS客户端根据连接ID清理存储的对端AMS服务信息。
 * @par    AMS客户端根据连接ID清理存储的对端AMS服务信息。
 * @attention
 * @param [in] client_id         客户端ID。
 * @param [in] conn_id           连接ID。
 * @retval 无返回值。
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
void ble_ams_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief The interface of AMS client clear all stored remote ams service info.
 * @par The interface of AMS client clear all stored remote ams service info.
 * @attention
 * @retval #void no return value.
 * @par Dependency:
 * @li
 * @see
 * @else
 * @brief  AMS客户端清理所有存储的对端AMS服务信息。
 * @par    AMS客户端清理所有存储的对端AMS服务信息。
 * @attention
 * @retval 无返回值。
 * @par 依赖:
 * @li
 * @see
 * @endif
 */
void ble_ams_client_clear_remote_all(void);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif