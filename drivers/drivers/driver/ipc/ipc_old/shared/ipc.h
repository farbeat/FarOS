/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  IPC Common interface.
 * Author: @CompanyNameTag
 * Create:
 */
#ifndef SRC_DRIVERS_IPC_SHARED_IPC_H
#define SRC_DRIVERS_IPC_SHARED_IPC_H

#include <stdint.h>
#include "core.h"
#include "ipc_actions.h"
#include "product.h"

/**
 * @defgroup connectivity_drivers_non_os_ipc IPC
 * @ingroup  connectivity_drivers_non_os
 * @{
 */
#define IPC_SPIN_SEND_DEFAULT_TIMEOUT 0x5a5a5a5a
#define IPC_SPIN_SEND_MAX_TIMEOUT     0x7FFFFFFF

typedef enum {
    IPC_BT_PROTOCOL_SHARE_MEM = 0,
    IPC_BT_APP_SHARE_MEM,
    IPC_PROTOCOL_BT_SHARE_MEM,
    IPC_PROTOCOL_APP_SHARE_MEM,
    IPC_APP_BT_SHARE_MEM,
    IPC_APP_PROTOCOL_SHARE_MEM,
#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
    IPC_BT_PROTOCOL_1_SHARE_MEM,
    IPC_PROTOCOL_1_BT_SHARE_MEM,
    IPC_PROTOCOL_1_APP_SHARE_MEM,
    IPC_APP_PROTOCOL_1_SHARE_MEM,
#endif
#if SECURITY_EXIST == YES
    IPC_APP_SEC_SHARE_MEM,
    IPC_SEC_APP_SHARE_MEM,
#endif
    IPC_NUM_SHARE_MEM = IPC_SHARE_NUMS,
} ipc_share_mem_index_e;

/* only used for gnss-app ipc */
typedef enum {
    IPC_APP_GNSS_SHARE_MEM = 0,
    IPC_GNSS_APP_SHARE_MEM,
    IPC_NUM_SHARE_MEM_ADAPT,
} ipc_share_mem_index_adapt_e;

/**
 * @brief  Enum specifying the relative priority of the IPC message, with 0 indicating the highest priority.
 * Using the incrementing values allows for easy loops
 *      for (ipc_priority_e p = IPC_PRIORITY_HIGHEST; p <= IPC_PRIORITY_LOWEST; p++)
 */
typedef enum {
    IPC_PRIORITY_HIGHEST = 0, // !< Highest priority (lowest numeric value).
    IPC_PRIORITY_LOWEST,      // !< Lowest priority (highest numeric value).
    IPC_PRIORITY_MAX_NUM,     // !< Used to size arrays.
} ipc_priority_e;

/**
 * @brief  Enum holding the various status codes that an IPC message can pass through during delivery.
 * Note that this is different to ipc_status_t enum which documents the IPC mechanism error codes.
 */
typedef enum {
    IPC_ACTION_STATUS_CLEAR = 0,  // !< Message available for use in the message pool.
    IPC_ACTION_STATUS_PROCESS,    // !< Message is 'en-route' and awaiting further processing.
} ipc_action_status_e;

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

/**
 * @brief  This structure defines the complete IPC message, and is the basic element passed around within
 * the internal IPC mechanism. Do NOT change the size of this structure, as all cores MUST use
 * the same definition for the IPC mechanism to work!
 */
typedef struct {
    uint32_t id;              // !< Incrementing message id. Numbering is unique to sending core only.
    ipc_priority_e priority;  // !< Priority to use when processing the message internally.

    cores_t src;  // !< Sending core.
    cores_t dst;  // !< Destination core.

    // User fields.
    ipc_action_t action;    // !< Type of IPC message being sent. Used to route message on the destination.
    ipc_payload payload;  // !< Union of user data types. The action field determines which union member to use.

    bool request_ack;  // !< Request that a delivery confirmation be sent to the sender. UNUSED!
    uint8_t chksum;      // !< Simple integrity checksum. UNUSED!

    // !< Internal 'state' of the message as it is processed. Must be the last element written to!
    ipc_action_status_e status;
} ipc_mailbox_entry;

/* only used for gnss-app ipc */
typedef struct {
    uint32_t id;              // !< Incrementing message id. Numbering is unique to sending core only.
    ipc_priority_e priority;  // !< Priority to use when processing the message internally.

    cores_t src;  // !< Sending core.
    cores_t dst;  // !< Destination core.

    // User fields.
    ipc_action_t action;    // !< Type of IPC message being sent. Used to route message on the destination.
    ipc_payload_adapt payload;  // !< Union of user data types. The action field determines which union member to use.

    bool request_ack;  // !< Request that a delivery confirmation be sent to the sender. UNUSED!
    uint8_t chksum;      // !< Simple integrity checksum. UNUSED!

    // !< Internal 'state' of the message as it is processed. Must be the last element written to!
    ipc_action_status_e status;
} ipc_mailbox_entry_adapt;

/* ipc send spin fail info */
typedef struct _ipc_spin_send_info_ {
    uint32_t err_cnt : 31; /* total */
    uint32_t pending : 1;  /* log pending */

    /* cpu regs */
    uint32_t pc;
    uint32_t lr;
    uint32_t sp;
    uint32_t called; /* called func */
    uint32_t loop_cnt;
} ipc_spin_send_info;

/* ipc send info */
typedef struct _ipc_send_info_ {
    uint32_t send_cnt;
    uint32_t loop_cnt;
    uintptr_t send_data_called;
    ipc_spin_send_info spin_send_info;
} ipc_send_info;

/**
 * @brief  IPC ACTION Callback. Called whenever the registered ipc_action_t message is received.
 * @param  message The ipc_action_t of the received message.
 * @param  payload_p Data payload of the received mssage. Note that this is the uinion, not the individual memeber.
 * @param  src Sending core.
 * @param  id Internal message id. Can be used to detect duplicate delivery and for debug.
 * @return True if the incoming mailslot shluld be released or false if the message was
 *         not (fully) processed and should be redelivered.
 */
typedef bool (*ipc_action_handler)(ipc_action_t message, const volatile ipc_payload *payload_p,
                                   cores_t src, uint32_t id);

typedef bool (*ipc_idle_callback_handler)(cores_t dst);

/**
 * @brief  Initialise the IPC mechanism. Simply initialise internal data structures,
 * remove registered callbacks and enable appropriate interrupts.
 * @return Nothing.
 */
void ipc_init(void);

/**
 * @brief  De-Initialise the IPC mechanism. Simply disable appropriate interrupts and reset internal timers.
 * @return Nothing.
 */
void ipc_deinit(void);

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
 * @brief  De-Register a specific callback for a specific ipc_action_t.
 * @param  action The ipc_action_t to de-register the callback for.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_unregister_handler(ipc_action_t action);

/**
 * @brief  Send data to dst core use ipc.
 * Caller will yield/busy-wait if the needed mailslot is not available.
 * @param  dst Destination core for the data.
 * @param  length Length of the data to send.
 * @param  buffer pointer of the data to send.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_send_data(cores_t dst, uint16_t length, void *buffer);

/**
 * @brief  Send an IPC message containing the provided action and payload to the specified core. If the message
 * can't be sent immediately the call fails and IPC_STATUS_MAILBOX_BUSY is returned, it is up to the
 * caller to retry. Expected usage is that the caller will yield/busy-wait if the needed mailslot is
 * not available. Note that IPC messages between the APPS and PROTOCOL cores are sent via the security core,
 * which also handles the priorities.
 * @param  dst Destination core for the message.
 * @param  action Type of message being sent.
 * @param  payload_p Pointer to the payload to be sent to the remote core. Can be NULL if no payload is needed.
 * @param  payload_length Length of the payload_p.
 * @param  priority Priority with which the message should be sent (higher priority messages may
          overtake lower priority ones en-route).
 * @param  request_ack Request that the IPC mechanism confirm delivery to the remote core.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_send_message(cores_t dst, ipc_action_t action,
                              const ipc_payload *payload_p, uint16_t payload_length,
                              ipc_priority_e priority, bool request_ack);

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
                                           const ipc_payload *payload_p, uint16_t payload_length,
                                           ipc_priority_e priority, bool request_ack, uint32_t timeout_msec);

/**
 * @brief  get ipc spin fail info
 * @param  spin_sinfo Length of the data to send.
 * @param  dst Destination core for the data.
 * @return true or false
 */
bool ipc_get_spin_fail_info(ipc_spin_send_info **spin_sinfo, cores_t dst);

/**
 * @brief  Check if the outbox of the dst core is clear up or not, if it's clearup, class the callback of IDLE.
 * @param  dst         The core of desination
 * @return ipc_status_t  See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_check_idle(cores_t dst);

/**
 * @brief  Callback from HAL low level IRQ handler to indirect the HW specific functionality which is core specific.
 * This ends up calling the registered callback, if any,  in an interrupt context.
 * @return None
 */
void ipc_receive_message(cores_t src);

/*
 * Check the core's outbox is clear up or not.
 */
bool ipc_check_outbox_free(cores_t dst);

/**
 * @brief  Register the callback of IDLE status
 * @param  handler the registered callback
 * @return None
 */
ipc_status_t ipc_register_idle_callback_handler(ipc_idle_callback_handler handler);

/**
 * @brief  UnRegister the callback of IDLE status
 * @return None
 */
ipc_status_t ipc_unregister_idle_callback_handler(void);

#ifdef TEST_SUITE
void ipc_inspect_mailboxes(void);
#endif

/**
 * @brief  Send data to dst core use ipc timeout.
 * Caller will yield/busy-wait if the needed mailslot is not available.
 * @param  dst Destination core for the data.
 * @param  length Length of the data to send.
 * @param  buffer pointer of the data to send.
 * @param  timeout_msec if dst busy we try until timeout.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_send_data_timeout(cores_t dst, uint16_t length, void *buffer, uint32_t timeout_msec);

/**
 * @brief  get the ipc status of the dst core.
 * @param  dst The core of desination.
 * @return See definition of ipc_status_t for details on returned error codes and their meaning.
 */
ipc_status_t ipc_check_status(cores_t dst);

/**
 * @brief  interrupt a given core.
 * @param  dst The core of desination.
 * @return None
 */
void ipc_interrupt_core(cores_t dst);

/*
 * set the core's outbox clear.
 * @param  dst Destination core for free ipc outbox.
 * @return None
 */
void ipc_set_outbox_free(cores_t dst);

/*
 * Obtains the power status of the remote core.
 * @param  dst Destination core for the message.
 * @param  action Type of message being sent.
 * @return Errcode
 */
uint32_t ipc_slave_power_mode(cores_t dst, ipc_action_t action);
/**
 * @}
 */
#endif
