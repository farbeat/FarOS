/*
 * Copyright (c) CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: header of IPC common interface
 * Author: audio
 * Create: 2022-10-15
 */

#ifndef IPC_H
#define IPC_H

#include <stdint.h>
#include <stdbool.h>

#include "td_type.h"

#define IPC_SEND_DEFAULT_TIMEOUT 10

#define IPC_RPC_PAYLOAD_SIZE 768
#define IPC_SHIFT_BIT        28
#define IPC_ID_MASK          0x0FFFFFFF

typedef enum {
    CORES_BT_CORE       = 0,  /* bt Core. */
    CORES_HIFI_CORE     = 1,  /* Hifi Core. */
    CORES_PROTOCOL_CORE = CORES_HIFI_CORE,
    CORES_APPS_CORE     = 2,  /* Applications Core. */
    CORES_HIFI1_CORE    = 3,  /* Hifi1 Core. */
    CORES_MAX_NUMBER_PHYSICAL,

    CORES_NONE = CORES_MAX_NUMBER_PHYSICAL,         /* Used to return a NONE value where needed. */
    CORES_ASSET_CORE = CORES_MAX_NUMBER_PHYSICAL,   /* used for asset store */
    CORES_UNKNOWN = CORES_MAX_NUMBER_PHYSICAL + 1,  /* Used to return a unknown value. */
} cores_t;

/**
 * @brief  This enum describes all the supported IPC messages. Each message needs to have an associated payload type
 *         defined in the ipc_payload union.
 * Note: Gaps in numbering lead to wasted memory, as also used to iterate over an array of callbacks.
 */
typedef enum {
    IPC_ACTION_CORE_STARTED = 0,          // !< Sent to the security core if another core has started
    IPC_ACTION_PROTOCOL_REBOOT_APPS = 1,  // !< Protocol Send Reboot Apps To security Implement
    IPC_ACTION_EXCEPTION_TEST = 2,        // !< Sent to security core to test exception handle
    IPC_ACTION_UNUSED_3 = 3,              // !<
    IPC_ACTION_FLASH_OP_CNF = 4,          // !<
    IPC_ACTION_FLASH_OP_IND = 5,          // !<
    IPC_ACTION_SYS_REBOOT_REQ = 6,        // !< Request a complete system reboot.
    IPC_ACTION_LOG_INFO = 7,              // !< Inform there are logs available.
    IPC_ACTION_RPC_COMMAND = 8,           // !< Used to send rpc style commands between the cores
    IPC_ACTION_DATA = 9,                  // !< Used to send data
    IPC_ACTION_WAKUP_INFO = 10,           // !< Apps ipc wakeup interrupt.
    IPC_ACTION_PLL_MICRO_REQ = 11,        // !< DSP ipc request PLL micro adjust interrupt.
    IPC_ACTION_SET_LOG_LEVEL = 12,        // !< Used by App core to set the log level of BT core or DSP core.
    IPC_ACTION_EXCEPTION_IND = 13,        // !< Used by Any core to inform the current exception.
    IPC_ACTION_AP_BT_HOOK_DATA = 14,      // !< Used by App core to hook the data.
    IPC_ACTION_BT_AP_HOOK_DATA = 15,      // !< Used by BT core to send the result of hook data.
    IPC_ACTION_BT_HIFI_MEM_REQ = 16,      // !< Used by BT core to request the memory.
    IPC_ACTION_HIFI_BT_MEM_CNF = 17,      // !< Used by DSP core to response the memory request.
    IPC_ACTION_AP_BT_CALIBRATION_PARAM_REQ = 18, // !< Used by App core to send the calibration params to BT core.
    IPC_ACTION_SET_SYSTEM_CLOCKS = 19,    // !< Used to switch system clocks.
    IPC_ACTION_MASS_DATA_INFORM = 20,     // !< Used to inform there are coming mass data from bt&dsp core.
    IPC_ACTION_SET_LOG_TIME = 21,         // !< Used to set the rtc time of compress log.
    IPC_ACTION_REDUCE_VOLTAGE_AFTER_WAKEUP = 22, // !< Used to reduce voltage from 0.9v to 0.8v after wakeup.
    IPC_ACTION_AUDIO_HOOK_DATA = 23,      // !< Used to hook audio data
    IPC_ACTION_GET_CAL_FREQ_REQ = 24,     // !< Used by App core to get the freq of clock calibration.
    IPC_ACTION_GET_CAL_FREQ_CNF = 25,     // !< Used by BT core to report the freq of clock calibration.
    IPC_ACTION_HCI_INFORM = 26,           // !< Used to inform there are coming hci data from app core.
    IPC_ACTION_TS_MESSAGE_REQ = 27,       // !< Used by the test suite to send a command.
    IPC_ACTION_TS_MESSAGE_CNF = 28,       // !< Used by the test suite to send the response to a previous command.
    IPC_ACTION_SET_LOG_SWITCH = 29,       // !< Used to config log switch by AT cmd.
    IPC_ACTION_BT_POWER_ON = 30,                 // !< Used by BT core to inform App core no xip access
    IPC_ACTION_UPDATA_TRNG = 31,          // !< Used to BT core to inform App core updata trng share memory
    IPC_ACTION_SLAVE_ADJUST_FREQ = 32,    // !< Used by Master core to inform slave core updata freq
    IPC_ACTION_TEMP_CHANGE = 33,          // !< Used by BT core to inform App core temperature change
    IPC_ACTION_DSP_CURRENT_FREQ = 34,     // !< Used by Master core to inform dsp core current freq
    IPC_ACTION_BT_POWER_OFF = 35,         // !< Used by Master core to inform bt core power off.
    IPC_ACTION_BT_HIGH_POWER = 36,        // !< Used by BT core to inform app core open or close high power.
    IPC_ACTION_MASS_DATA_INFORM_WITH_EXTEND = 37,   // !< Used to inform mass data by slave core in wear.
    IPC_ACTION_GNSS_START = 50,           // !< [50:] is used by gnss.
    IPC_ACTION_GNSS_LOG_INFO = IPC_ACTION_GNSS_START + 0,
    IPC_ACTION_GNSS_MSG = IPC_ACTION_GNSS_START + 1,
    IPC_ACTION_GNSS_POWER_OFF = IPC_ACTION_GNSS_START + 2,
    IPC_ACTION_GNSS_END = 70,

    IPC_ACTION_NUM_ACTIONS,               // !< Used to size arrays.
    IPC_ACTION_FORCE_SIZE = 0x7FFF,       // !< Used to force enum to 2 bytes
} ipc_action_t;

typedef struct {
    td_u32 data[IPC_RPC_PAYLOAD_SIZE];
} ipc_payload_rpc;

typedef union {
    ipc_payload_rpc rpc_payload; /* Not actually used.  Just to ensure ipc_payload union is a reasonable size. */
} ipc_payload;

typedef enum {
    IPC_PRIORITY_HIGHEST = 0, /* Highest priority (lowest numeric value). */
    IPC_PRIORITY_LOWEST,      /* Lowest priority (highest numeric value). */
    IPC_PRIORITY_MAX_NUM,     /* Used to size arrays. */
} ipc_priority_e;

typedef enum {
    IPC_ACTION_STATUS_CLEAR = 0, /* Message available for use in the message pool. */
    IPC_ACTION_STATUS_PROCESS,   /* Message is 'en-route' and awaiting further processing. */
} ipc_action_status;

/**
 * @brief  Enum holding the status codes returned by the IPC mechanism. Note that this is different
 * to the ipc_action_status_e enum which documents the IPC message status.
 */
typedef enum {
    IPC_STATUS_OK = 0,  // !< Everything went OK. Default return code.

    IPC_STATUS_INVALID_ACTION = 1,    // !< The requested operation referred to an unknown / invalid ipc_action_t type.
    IPC_STATUS_INVALID_HANDLER = 2,   // !< The requested operation failed due to an error with the provided callback.
    IPC_STATUS_INVALID_CORE = 3,      // !< The requested core doesn't exist.
    IPC_STATUS_INVALID_PRIORITY = 4,  // !< The requested priority doesn't exist.
    IPC_STATUS_INVALID_PARAM = 5,     // !< input invalid.

    IPC_STATUS_ALREADY_REGISTERED = 6,  // !< The requested operation failed as already have callback registered.
    IPC_STATUS_MAILBOX_BUSY = 7,        // !< Can't send a message as the oubox is in use.
    IPC_STATUS_MEMORY_ERROR = 8,        // !< Memory process error.
    IPC_STATUS_SEND_TIMEOUT = 9,        // maybe poweroff.
    IPC_STATUS_POWER_OFF = 10,          // maybe poweroff.
} ipc_status_t;

typedef struct {
    uint32_t id;                  /* Incrementing message id. Numbering is unique to sending core only. */
    ipc_priority_e priority;    /* Priority to use when processing the message internally. */

    cores_t src;                /* Sending core. */
    cores_t dst;                /* Destination core. */

    /* User fields */
    ipc_action_t action;        /* Type of IPC message being sent. Used to route message on the destination. */
    ipc_payload payload;        /* Union of user data types. The action field determines which union member to use. */

    bool request_ack;        /* Request that a delivery confirmation be sent to the sender. UNUSED! */
    uint8_t check_sum;            /* Simple integrity checksum. UNUSED! */

    /* Internal 'state' of the message as it is processed.  Must be the last element written to! */
    ipc_action_status status;
} ipc_mailbox_entry;

typedef struct {
    ipc_priority_e priority;    /* Priority to use when processing the message internally. */
    bool request_ack;           /* Request that a delivery confirmation be sent to the sender. UNUSED! */

    const ipc_payload *payload; /* user data. The action field determines which union member to use. */
    uint16_t payload_length;
} ipc_mailbox_payload;

/**
 * @brief  Send an IPC message containing the provided action and payload to the specified core. If the message
 * can't be sent immediately the call fails and IPC_STATUS_MAILBOX_BUSY is returned, it is up to the
 * caller to retry. Expected usage is that the caller will yield/busy-wait if the needed mailslot is
 * not available. Note that IPC messages between the APPS and PROTOCOL cores are sent via the security core,
 * which also handles the priorities.
 * @param  dst Destination core for the message.
 * @param  action Type of message being sent.
 * @param  payload Pointer to the payload to be sent to the remote core. Can be NULL if no payload is needed.
 * @param  payload_length Length of the payload.
 * @param  priority Priority with which the message should be sent (higher priority messages may
          overtake lower priority ones en-route).
 * @param  request_ack Request that the IPC mechanism confirm delivery to the remote core.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_send_message(cores_t dst, ipc_action_t action, const ipc_mailbox_payload *mbox_payload);

/**
 * @brief  Busy wait for ipc idle, this func will hold current task!
 * Send an IPC message containing the provided action and payload to the specified core. If the message
 * can't be sent immediately the call fails and IPC_STATUS_MAILBOX_BUSY is returned, it is up to the
 * caller to retry. Expected usage is that the caller will yield/busy-wait if the needed mailslot is
 * not available. Note that IPC messages between the APPS and PROTOCOL cores are sent via the security core,
 * which also handles the priorities.
 * @param  dst Destination core for the message.
 * @param  action Type of message being sent.
 * @param  payload_p Pointer to the payload to be sent to the remote core. Can be NULL if no payload is needed.
 * @param  payload_length Length of the payload_p.
 * @param  priority Priority with which the message should be sent (higher priority messages may
 * overtake lower priority ones en-route).
 * @param  request_ack Request that the IPC mechanism confirm delivery to the remote core.
 * @param  timeout_msec  msec, if dst busy we try until timeout.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_spin_send_message_timeout(cores_t dst, ipc_action_t action,
    const ipc_mailbox_payload *mbox_payload, uint32_t timeout_msec);

/**
 * @brief  Send data to dst core use ipc timeout.
 * Caller will yield/busy-wait if the needed mailslot is not available.
 * @param  dst Destination core for the data.
 * @param  length Length of the data to send.
 * @param  buffer pointer of the data to send.
 * @param  timeout_msec if dst busy we try until timeout.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_send_data_timeout(cores_t dst, uint8_t *buffer, uint16_t length, uint32_t timeout_msec);

/**
 * @brief  IPC ACTION Callback. Called whenever the registered ipc_action_t message is received.
 * @param  message The ipc_action_t of the received message.
 * @param  payload_p Data payload of the received mssage. Note that this is the uinion, not the individual memeber.
 * @param  src Sending core.
 * @param  id Internal message id. Can be used to detect duplicate delivery and for debug.
 * @return True if the incoming mailslot shluld be released or false if the message was
 *         not (fully) processed and should be redelivered.
 */
typedef bool (*ipc_action_handler)(ipc_action_t message,
    const volatile ipc_payload *payload, cores_t src, uint32_t id);

/**
 * @brief  Register a specific callback to handle a specific ipc_action_t. The action must not already have a handler
 * registered. The callback should copy the payload data passed to it as the IPC system will mark the mailslot
 * as 'free' if the callback returns true. Should the message not be fully processed, the callback should return false,
 * which will cause the entire message to be redelivered a short time later.
 * @param  action The ipc_action_t to register the callback for.
 * @param  handler The callback function to install as callback handler.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_register_handler(ipc_action_t action, ipc_action_handler handler);

/**
 * @brief  Unregister a specific callback for a specific ipc_action_t.
 * @param  action The ipc_action_t to de-register the callback for.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_unregister_handler(ipc_action_t action);

#endif
