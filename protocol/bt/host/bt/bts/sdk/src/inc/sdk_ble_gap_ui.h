/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: advertesting
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */
#ifndef BLE_ADVERTISING_H__
#define BLE_ADVERTISING_H__
#include <stdint.h>
#include "bth_stack_gap.h"
#include "sdk_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_ADV_PARAM_NOT_READY    0
#define BLE_ADV_PARAM_READY        1

#define BLE_RET_CODE_SUCCESS       0
#define BLE_RET_CODE_FAIL          1

/**@brief   Advertising events.
 *
 * @details These events are propagated to the main application if a handler was provided during
 *          initialization of the Advertising Module. Events for modes that are not used can be
 *          ignored. Similarly, BLE_ADV_EVT_WHITELIST_REQUEST and BLE_ADV_EVT_PEER_ADDR_REQUEST
 *          can be ignored if whitelist and direct advertising is not used.
 */
typedef enum {
    BLE_ADV_EVT_IDLE,               /**< Idle; no connectable advertising is ongoing.*/
    BLE_ADV_EVT_DIRECTED_HIGH_DUTY, /**< Direct advertising mode has started. */
    BLE_ADV_EVT_DIRECTED,           /**< Directed advertising (low duty cycle) has started. */
    BLE_ADV_EVT_FAST,               /**< Fast advertising mode has started. */
    BLE_ADV_EVT_SLOW,               /**< Slow advertising mode has started. */
    BLE_ADV_EVT_FAST_WHITELIST,     /**< Fast advertising mode using the whitelist has started. */
    BLE_ADV_EVT_SLOW_WHITELIST,     /**< Slow advertising mode using the whitelist has started. */
    BLE_ADV_EVT_WHITELIST_REQUEST,  /**< Request a whitelist from the main application. For whitelist advertising to
                                       work, the whitelist must be set when this event occurs. */
    BLE_ADV_EVT_PEER_ADDR_REQUEST   /**< Request a peer address from the main application. For directed advertising to
                                       work, the peer address must be set when this event occurs. */
} ble_adv_evt_t;

/**@brief   connect paramter update struct from app.
 *
 * @details These struct is used to update connect paramters,user like to use conn_handle
 */
typedef struct {
    uint16_t conn_handle; /*!< for info whitch connect shall be update */
    uint16_t interval_min; /*!< user set min interval */
    uint16_t interval_max; /*!< user set max interval */
    uint16_t slave_latency; /*!< slave reply min latency */
    uint16_t timeout_multiplier; /*!< Interval for disconnection due to timeout */
} bth_gap_connection_param_app;

/**
 * @if Eng
 * @brief Enum of IO capability.
 * @else
 * @brief IO能力。
 * @endif
 */
typedef enum {
    GAP_BLE_IO_CAP_OUT = 0x00, /*!< @if Eng Display only
                                    @else 只有输出显示的能力 @endif */
    GAP_BLE_IO_CAP_IO,         /*!< @if Eng Display, input yes or no
                                    @else 只有输入YES和NO的能力，有显示能力 @endif */
    GAP_BLE_IO_CAP_IN,         /*!< @if Eng Keyboard only
                                    @else 可以输入0-9，YES/NO和确认的能力，没有显示的能力 @endif */
    GAP_BLE_IO_CAP_NONE,       /*!< @if Eng No input, no output
                                    @else 没有输入和输出能力 @endif */
    GAP_BLE_IO_CAP_KBDISP      /*!< @if Eng Keyboard, display
                                    @else 可以输入0-9，YES/NO和确认的能力，有显示的能力 @endif */
} gap_ble_io_cap_mode_t;

uint8_t uapi_advertising_init(bth_gap_set_extern_adv *adv_param, uint32_t adv_param_len);

/* start adv use default paramters */
uint8_t uapi_start_adv(void);

/* disable adv */
uint8_t uapi_stop_adv(void);

/* start adv use user paramters */
uint8_t uapi_set_conn_param(bth_gap_set_extern_adv *adv_param, uint32_t adv_param_len);

/* set addr */
uint8_t uapi_set_local_addr(uint8_t *addr, uint8_t len);

/* set local name */
uint8_t uapi_set_local_name(uint8_t *name, uint8_t len);

/* set local apperance */
uint8_t uapi_set_local_apperance(uint16_t appr);

/**@brief   update connect paramters.
 * @param   conn_handle:connect handle
 */
uint8_t uapi_connect_param_update(bth_gap_connection_param_app *params);

/**@brief   disconnect ble link.
 * @param   conn_handle: for info whitch connect shall be update
 *          interval_min: user set min interval
 *          interval_max: user set max interval
 *          slave_latency: slave reply min latency
 *          timeout_multiplier: Interval for disconnection due to timeout
 */
uint8_t uapi_disconnect_ble_link(uint16_t conn_handle);

/**
 * @brief  注册数据透传回调函数
 * @param  [in]  <func > 回调函数
 * @return SUCCESS: 成功, OTHER: 失败
 */
int8_t uapi_reg_data_bypass_cbk(sdk_ble_cbk func);

/**
 * @brief  发送BTH透传数据
 * @param  [in]  <data > 待透传数据
 * @param  [in]  <len > 待透传数据长度
 * @return SUCCESS: 成功, OTHER: 失败
 */
int8_t uapi_send_data_bypass(uint8_t *data, uint16_t len);

uint8_t uapi_set_sec_param_req(bth_smp_sec_param *sec_param);

#ifdef __cplusplus
}
#endif

#endif // BLE_ADVERTISING_H__

/** @} */
