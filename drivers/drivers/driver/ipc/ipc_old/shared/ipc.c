/*
 * Copyright (c) @CompanyNameMagicTag 2018-2023. All rights reserved.
 * Description:   IPC Common interface.
 * Author: @CompanyNameTag
 * Create:
 */

#include "ipc.h"
#include "ipc_irq.h"
#include "panic.h"
#include "non_os.h"
#include "hal_ipc.h"
#include "securec.h"
#include "rtc.h"
#include "systick.h"
#include "systick_porting.h"
#include "watchdog_porting.h"
#include "watchdog.h"
#if !defined(BUILD_APPLICATION_SSB) && !defined(UNIT_TEST)
#include "cpu_trace.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "log_oam_logger.h"
#include "log_def.h"
#include "log_printf.h"
#endif
#endif
#include "connectivity_log.h"
#include "debug_print.h"
#include "soc_osal.h"
#ifdef IPC_DATA_SEND_TEST
#include "ipc_protocol.h"
#include "rpc_auto_generated_application.h"
#endif

// Try to deliver after a given (small) amount time.
#define IPC_REDELIVERY_TIMEOUT_MS 10
#define IPC_S_TO_MS 1000
#define IPC_DEFAULT_TIMEOUT_MS 5000
#define IPC_SHIFT_BIT 28
#define IPC_COMMPUTE_TIME_COEF 4

#define IPC_CONTINUOUS_FAIL_MAX 200
#define IPC_REG_INT_EN  0x52000020
#define IPC_REG_INT     0x52000024
#define IPC_REG_INT_STS 0x52000028
#define IPC_REG_INT_CLR 0x5200002c
#define IPC_REG_CLR_STS 0x52000030
#define IPC_REG_INT_SET 0x52000034

static volatile uint32_t g_ipc_to_bcore_continuous_fail_nums = 0;

static uint32_t g_ipc_ssend_default_timeout = 0;

static volatile ipc_send_info g_ipc_sinfo[CORES_MAX_NUMBER_PHYSICAL];

static volatile ipc_mailbox_entry *g_ipc_mailboxes[CORES_MAX_NUMBER_PHYSICAL][CORES_MAX_NUMBER_PHYSICAL];
// Core index is defined in platform_types.h.
#if CORE_NUMS == 2
static volatile ipc_mailbox_entry ipc_share_mem0_to_2 __attribute__((section(".ipc_share_mem0_2"))); // core0 to core2
static volatile ipc_mailbox_entry ipc_share_mem2_to_0 __attribute__((section(".ipc_share_mem2_0"))); // core2 to core0
#else
static volatile ipc_mailbox_entry ipc_share_mem0_to_1 __attribute__((section(".ipc_share_mem0_1"))); // core0 to core1
static volatile ipc_mailbox_entry ipc_share_mem0_to_2 __attribute__((section(".ipc_share_mem0_2"))); // core0 to core2
static volatile ipc_mailbox_entry ipc_share_mem1_to_0 __attribute__((section(".ipc_share_mem1_0"))); // core1 to core0
static volatile ipc_mailbox_entry ipc_share_mem1_to_2 __attribute__((section(".ipc_share_mem1_2"))); // core1 to core2
static volatile ipc_mailbox_entry ipc_share_mem2_to_0 __attribute__((section(".ipc_share_mem2_0"))); // core2 to core0
static volatile ipc_mailbox_entry ipc_share_mem2_to_1 __attribute__((section(".ipc_share_mem2_1"))); // core2 to core1
#endif

#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
static volatile ipc_mailbox_entry ipc_share_mem0_to_3 __attribute__((section(".ipc_share_mem0_3"))); // core0 to core3
static volatile ipc_mailbox_entry ipc_share_mem3_to_0 __attribute__((section(".ipc_share_mem3_0"))); // core3 to core0
static volatile ipc_mailbox_entry ipc_share_mem3_to_2 __attribute__((section(".ipc_share_mem3_2"))); // core3 to core2
static volatile ipc_mailbox_entry ipc_share_mem2_to_3 __attribute__((section(".ipc_share_mem2_3"))); // core2 to core3
#endif
#if SECURITY_EXIST == YES
static volatile ipc_mailbox_entry ipc_share_mem2_to_4 __attribute__((section(".ipc_share_mem2_4"))); // core2 to core4
static volatile ipc_mailbox_entry ipc_share_mem4_to_2 __attribute__((section(".ipc_share_mem4_2"))); // core4 to core2
#endif
#if GNSS_EXIST == YES
static volatile ipc_mailbox_entry_adapt *g_ipc_mailboxes_adapt[CORES_MAX_NUMBER_PHYSICAL][CORES_MAX_NUMBER_PHYSICAL];
static volatile ipc_mailbox_entry_adapt ipc_share_mem_adapt2_to_3 __attribute__((section(".ipc_adapt_mem2_3")));
static volatile ipc_mailbox_entry_adapt ipc_share_mem_adapt3_to_2 __attribute__((section(".ipc_adapt_mem3_2")));
#endif
// Array of callback functions. Note that some entries are likely to be empty!
// Note: Using an array here may waste memory due to empty entries,
// but is much faster and more predictable than using a dynamic list.
static ipc_action_handler g_ipc_handlers[IPC_ACTION_NUM_ACTIONS];
static ipc_idle_callback_handler g_ipc_idle_callback_handlers;

// Flag to denote if a callback timer is already running. Avoids starting another timer if one is running already.
// The g_ipc_timer is actually a rtc timer.
static rtc_handle_t g_ipc_timer;

#if CORE == APPS
#define MAX_BT_PC_LR_NUM 10
static uint32_t g_app_record_bt_pc[MAX_BT_PC_LR_NUM] = { 0 };
static uint32_t g_app_record_bt_lr[MAX_BT_PC_LR_NUM] = { 0 };
static uint32_t g_bt_sp_to_record = 0;
static uint32_t g_ipc_timer_index = 0;
#endif

static void ipc_timer_callback(void);
static void ipc_start_timer(void);

static inline void ipc_reg_info_debug(void)
{
    PRINT("ipc reg info:int en:%x,int:%x,int sts:%x.\r\n", readw(IPC_REG_INT_EN), readw(IPC_REG_INT),
        readw(IPC_REG_INT_STS));
    PRINT("int clr:%x,clr sts:%x,int set:%x.\r\n", readw(IPC_REG_INT_CLR), readw(IPC_REG_CLR_STS),
        readw(IPC_REG_INT_SET));
}

static inline void ipc_to_bcore_timeout_proc(cores_t dst)
{
    if (dst == CORES_BT_CORE) {
        g_ipc_to_bcore_continuous_fail_nums++;
        if (g_ipc_to_bcore_continuous_fail_nums > IPC_CONTINUOUS_FAIL_MAX) {
            panic(PANIC_BT_STS_ABNORMAL, __LINE__);
        }
    }
}

static inline void ipc_to_bcore_clr_fail_num(cores_t dst)
{
    if (dst == CORES_BT_CORE) {
        g_ipc_to_bcore_continuous_fail_nums = 0;
    }
}

#ifdef IPC_DATA_SEND_TEST
static bool test_ipc_receive_data_callback(ipc_action_t message, const volatile ipc_payload *payload_p,
                                           cores_t src, uint32_t id)
{
    UNUSED(message);
    UNUSED(id);
    uint16_t data_length;

    ipc_msg_t *ipc_msg = (ipc_msg_t*)(payload_p);

    if (src == CORES_BT_CORE) {
        /* calc the data length need exchange the high 8 bit and low 8 bit */
        data_length = (((ipc_msg->hdr.length & 0x00FF) << 8) | ((ipc_msg->hdr.length & 0xFF00) >> 8));
        data_length += sizeof(ipc_msg_header_t);
        remote_command_send_msg2app(src, data_length, (uint8_t *)ipc_msg);
    } else if (src == CORES_PROTOCOL_CORE) {
        data_length = ipc_msg->hdr.length;
        data_length += sizeof(ipc_msg_header_t);
        remote_command_send_msg2app(src, data_length, (uint8_t *)ipc_msg);
    } else {
        return false;
    }

    return true;
}
#endif

#if CORE == APPS
static void ipc_mailbox_init(void)
{
    g_ipc_mailboxes[CORES_BT_CORE][CORES_APPS_CORE] = &ipc_share_mem0_to_2;
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_BT_CORE] = &ipc_share_mem2_to_0;
#if CORE_NUMS != 2
    g_ipc_mailboxes[CORES_PROTOCOL_CORE][CORES_APPS_CORE] = &ipc_share_mem1_to_2;
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_PROTOCOL_CORE] = &ipc_share_mem2_to_1;
#endif
#if SECURITY_EXIST == YES
    g_ipc_mailboxes[CORES_SEC_CORE][CORES_APPS_CORE] = &ipc_share_mem4_to_2;
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_SEC_CORE] = &ipc_share_mem2_to_4;
#endif
#if GNSS_EXIST == YES
    g_ipc_mailboxes_adapt[CORES_GNSS_CORE][CORES_APPS_CORE] = &ipc_share_mem_adapt3_to_2;
    g_ipc_mailboxes_adapt[CORES_APPS_CORE][CORES_GNSS_CORE] = &ipc_share_mem_adapt2_to_3;
#endif
#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
    g_ipc_mailboxes[CORES_HIFI1_CORE][CORES_APPS_CORE] = &ipc_share_mem3_to_2;
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_HIFI1_CORE] = &ipc_share_mem2_to_3;
#endif
}
#elif CORE == BT
static void ipc_mailbox_init(void)
{
    g_ipc_mailboxes[CORES_BT_CORE][CORES_APPS_CORE] = &ipc_share_mem0_to_2;
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_BT_CORE] = &ipc_share_mem2_to_0;
#if CORE_NUMS != 2
    g_ipc_mailboxes[CORES_BT_CORE][CORES_PROTOCOL_CORE] = &ipc_share_mem0_to_1;
    g_ipc_mailboxes[CORES_PROTOCOL_CORE][CORES_BT_CORE] = &ipc_share_mem1_to_0;
#endif
#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
    g_ipc_mailboxes[CORES_BT_CORE][CORES_HIFI1_CORE] = &ipc_share_mem0_to_3;
    g_ipc_mailboxes[CORES_HIFI1_CORE][CORES_BT_CORE] = &ipc_share_mem3_to_0;
#endif
}
#elif CORE == GNSS
static void ipc_mailbox_init(void)
{
    g_ipc_mailboxes_adapt[CORES_GNSS_CORE][CORES_APPS_CORE] = &ipc_share_mem_adapt3_to_2;
    g_ipc_mailboxes_adapt[CORES_APPS_CORE][CORES_GNSS_CORE] = &ipc_share_mem_adapt2_to_3;
}
#elif CORE == SECURITY
static void ipc_mailbox_init(void)
{
    g_ipc_mailboxes[CORES_APPS_CORE][CORES_SEC_CORE] = &ipc_share_mem2_to_4;
    g_ipc_mailboxes[CORES_SEC_CORE][CORES_APPS_CORE] = &ipc_share_mem4_to_2;
}
#endif

uint32_t ipc_slave_power_mode(cores_t dst, ipc_action_t action)
{
    UNUSED(dst);
    UNUSED(action);
#if CHIP_SOCMN1
    slave_cpu_t cpu = SLAVE_CPU_MAX_NUM;
    if (dst == CORES_BT_CORE) {
        cpu = SLAVE_CPU_BT;
#if DSP_EXIST == YES
    } else if (dst == CORES_PROTOCOL_CORE) {
        cpu = SLAVE_CPU_DSP0;
#if DUAL_DSP_EXIST == YES
    } else if (dst == CORES_HIFI1_CORE) {
        cpu = SLAVE_CPU_DSP1;
#endif
#endif
    }

    if (get_slave_cpu_power_mode(cpu) != SLAVE_CPU_POWER_ON) {
        return ERRCODE_FAIL;
    }
#endif
    return ERRCODE_SUCC;
}

/*
 * Initialise the internal variables.
 */
void ipc_init(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_IPC) == true) {
        return;
    }
    ipc_reg_info_debug();
#if CORE == MASTER_BY_ALL
    memset_s((void *)IPC_MAILBOX_REGION_START, IPC_MAILBOX_REGION_LENGTH, 0, IPC_MAILBOX_REGION_LENGTH);
#endif

    memset_s((void *)g_ipc_sinfo, sizeof(g_ipc_sinfo), 0, sizeof(g_ipc_sinfo));

    ipc_mailbox_init();

    // First real opportunity to check that the IPC mailboxes fit in the region allocated for them by the linker.
#if GNSS_EXIST == YES
    if ((sizeof(ipc_mailbox_entry) * (IPC_NUM_SHARE_MEM) +
        sizeof(ipc_mailbox_entry_adapt) * (IPC_NUM_SHARE_MEM_ADAPT)) > IPC_MAILBOX_REGION_LENGTH) {
#else
    if ((sizeof(ipc_mailbox_entry) * (IPC_NUM_SHARE_MEM)) > IPC_MAILBOX_REGION_LENGTH) {
#endif
        panic(PANIC_IPC, __LINE__);
        return;
    }
    // Clear array of handlers (callbacks).
    memset_s(g_ipc_handlers, sizeof(ipc_action_handler) * IPC_ACTION_NUM_ACTIONS,
             0, sizeof(ipc_action_handler) * IPC_ACTION_NUM_ACTIONS);

    // Finally enable IPC related interrupts on this core.
    hal_ipc_interrupt_en();
    ipc_init_interrupts();

#ifdef IPC_DATA_SEND_TEST
    if (ipc_register_handler(IPC_ACTION_DATA, test_ipc_receive_data_callback) != IPC_STATUS_OK) {
        return;
    }
#endif

    non_os_set_driver_initalised(DRIVER_INIT_IPC, true);
}

/*
 * Disable interrupts to turn off IPC
 */
void ipc_deinit(void)
{
    non_os_set_driver_initalised(DRIVER_INIT_IPC, false);
    memset_s(g_ipc_handlers, sizeof(ipc_action_handler) * IPC_ACTION_NUM_ACTIONS,
             0, sizeof(ipc_action_handler) * IPC_ACTION_NUM_ACTIONS);
    ipc_deinit_interrupts();
}

/*
 * Add a callback to the handler array.
 */
ipc_status_t ipc_register_handler(ipc_action_t action, ipc_action_handler handler)
{
    if (action >= IPC_ACTION_NUM_ACTIONS) {
        return IPC_STATUS_INVALID_ACTION;
    }

    if (handler == NULL) {
        return IPC_STATUS_INVALID_HANDLER;
    }

    // Simply update the entry.
    g_ipc_handlers[action] = handler;

    return IPC_STATUS_OK;
}

ipc_status_t ipc_register_idle_callback_handler(ipc_idle_callback_handler handler)
{
    if (handler == NULL) {
        return IPC_STATUS_INVALID_HANDLER;
    }

    // Simply update the entry.
    g_ipc_idle_callback_handlers = handler;

    return IPC_STATUS_OK;
}

/*
 * De-register the handler. Doesn't have to be registered first for 'simplicity'
 */
ipc_status_t ipc_unregister_handler(ipc_action_t action)
{
    if (action >= IPC_ACTION_NUM_ACTIONS) {
        return IPC_STATUS_INVALID_ACTION;
    }

    // Simply update the entry. Don't bother checking if already registered.
    g_ipc_handlers[action] = NULL;
    return IPC_STATUS_OK;
}

ipc_status_t ipc_unregister_idle_callback_handler(void)
{
    // Simply update the entry. Don't bother checking if already registered.
    g_ipc_idle_callback_handlers = NULL;
    return IPC_STATUS_OK;
}

ipc_status_t ipc_check_idle(cores_t dst)
{
    ipc_action_status_e status;

    // Mark as volatile just in case the status field is updated by the security core to IPC_ACTION_STATUS_CLEAR.
    if (dst >= CORES_MAX_NUMBER_PHYSICAL) {
        return IPC_STATUS_INVALID_CORE;
    }

#if CHIP_LIBRA
#if (CORE == GNSS)
    status = g_ipc_mailboxes_adapt[CORE][dst]->status;
#else
    if (dst == CORES_GNSS_CORE) {
        status = g_ipc_mailboxes_adapt[CORE][dst]->status;
    } else {
        status = g_ipc_mailboxes[CORE][dst]->status;
    }
#endif
#else
    status = g_ipc_mailboxes[CORE][dst]->status;
#endif

    uint32_t irq_sts = osal_irq_lock();
    // Check if resource is available. Note that potentially the security core is running asynchronously to this call!
    if (status == IPC_ACTION_STATUS_CLEAR) {
        if (g_ipc_idle_callback_handlers != NULL) {
            g_ipc_idle_callback_handlers(dst);
        }
    }
    osal_irq_restore(irq_sts);

    return IPC_STATUS_OK;
}

/*
 * Check the core's outbox, and if available create the IPC message
 * and inform the dest core that an IPC message should be forwarded.
 * Send all buffer to dest core.
 * Expected usage is that the caller will yield/busy-wait if the resource is not available.
 */
ipc_status_t ipc_send_data(cores_t dst, uint16_t length, void *buffer)
{
    uint16_t send_length = length;
    uint8_t *send_buffer = (uint8_t *)buffer;
    ipc_status_t ret = IPC_STATUS_MAILBOX_BUSY;
    ipc_action_t acion = IPC_ACTION_DATA;
    ipc_slave_power_mode(dst, acion);
    uint16_t mailbox_len = (uint16_t)sizeof(ipc_payload);
#if CHIP_LIBRA
    if (dst == CORES_GNSS_CORE) {
        mailbox_len = (uint16_t)sizeof(ipc_payload_adapt);
        acion = IPC_ACTION_GNSS_MSG;
    }
#endif
    while (send_length != 0) {
        /* BUffer to send smaller than max IPC payload length */
        if (send_length <= mailbox_len) {
            ret = ipc_spin_send_message_timeout(dst, acion, (const ipc_payload *)send_buffer,
                                                send_length, IPC_PRIORITY_LOWEST, false,
                                                IPC_SPIN_SEND_DEFAULT_TIMEOUT);

            break;
        } else {
            ret = ipc_spin_send_message_timeout(dst, acion, (const ipc_payload *)send_buffer,
                                                mailbox_len, IPC_PRIORITY_LOWEST, false,
                                                IPC_SPIN_SEND_DEFAULT_TIMEOUT);
            if (ret != IPC_STATUS_OK) {
                break;
            }

            send_buffer += mailbox_len;
            send_length -= mailbox_len;
        }
    }

    return ret;
}

#if CORE == APPS
static void app_record_bt_pc_lr(void)
{
    UNUSED(g_bt_sp_to_record);
    UNUSED(g_app_record_bt_lr);
    UNUSED(g_app_record_bt_pc);
    UNUSED(g_ipc_timer_index);
#if !defined(BUILD_APPLICATION_SSB) && !defined(UNIT_TEST)
    if (g_ipc_timer_index < MAX_BT_PC_LR_NUM) {
        bool ret = cpu_trace_get_locked_regs(HAL_CPU_TRACE_TRACED_BCPU, &g_app_record_bt_pc[g_ipc_timer_index],
            &g_app_record_bt_lr[g_ipc_timer_index], &g_bt_sp_to_record);
#ifdef BUILD_APPLICATION_STANDARD
        oml_pf_log_print2(LOG_ACORE_PLT_BSP_DRIVER_IPC, LOG_NUM_DEBUG, LOG_LEVEL_ERROR,
                          "ap => bt ipc error, get cpu trace ret=%d, action = %d",
                          ret, g_ipc_mailboxes[CORE][CORES_BT_CORE]->action);
        oml_pf_log_print3(LOG_ACORE_PLT_BSP_DRIVER_IPC, LOG_NUM_DEBUG, LOG_LEVEL_ERROR,
                          "ap => bt ipc error, get cpu trace bt pc=0x%x, lr=0x%x, sp=0x%x",
                          g_app_record_bt_pc[g_ipc_timer_index], g_app_record_bt_lr[g_ipc_timer_index],
                          g_bt_sp_to_record);
#endif
        UNUSED(ret);
        g_ipc_timer_index++;
    }
#endif
}
#endif

/*
 * Ensure we try again if we didn't fully process an interrupt.
 * Can happen in all directions, but especially if one side is
 * consuming more slowly than the producer.
 */
static void ipc_timer_callback(void)
{
    uint32_t irq_sts = osal_irq_lock();
    // Reset local copy!
    for (uint8_t core = CORES_BT_CORE; core < CORES_MAX_NUMBER_PHYSICAL; core++) {
        // Really should not fail! But just in case!
        volatile ipc_mailbox_entry *mailbox = g_ipc_mailboxes[CORE][core];
        if (mailbox != NULL) {
            // Message not processed yet! Interrupt again.
            if (mailbox->status == IPC_ACTION_STATUS_PROCESS) {
                hal_ipc_interrupt_core((cores_t)core);
                ipc_start_timer();
#if (CORE == APPS)
                app_record_bt_pc_lr();
#endif
            } else {
                ipc_check_idle((cores_t)core);

#if (CORE == APPS)
                g_ipc_timer_index = 0;
#endif
            }
        }
    }

    // Reset local copy!
#if CHIP_LIBRA
    for (uint8_t core = CORES_BT_CORE; core < CORES_MAX_NUMBER_PHYSICAL; core++) {
        // Really should not fail! But just in case!
        volatile ipc_mailbox_entry_adapt *mailbox = g_ipc_mailboxes_adapt[CORE][core];
        if (mailbox != NULL) {
            // Message not processed yet! Interrupt again.
            if (mailbox->status == IPC_ACTION_STATUS_PROCESS) {
                hal_ipc_interrupt_core((cores_t)core);
                ipc_start_timer();
#if (CORE == APPS)
                app_record_bt_pc_lr();
#endif
            } else {
                ipc_check_idle((cores_t)core);

#if (CORE == APPS)
                g_ipc_timer_index = 0;
#endif
            }
        }
    }
#endif
    osal_irq_restore(irq_sts);
}

/*
 * Start the IPC callback timer to autonomously check if something is stuck. This can happen for example if the target
 * core failed to release the incoming mailslot, or the IPC mechanism failed to push the outbound message onto the fifo.
 */
static void ipc_start_timer(void)
{
    // Only start the timer if not already running
    if (g_ipc_timer == NULL) {
        uapi_rtc_create(RTC_0, &g_ipc_timer);
        uapi_rtc_start(g_ipc_timer, IPC_REDELIVERY_TIMEOUT_MS, (rtc_callback_t)ipc_timer_callback, 0);
    } else {
        uapi_rtc_start(g_ipc_timer, IPC_REDELIVERY_TIMEOUT_MS, (rtc_callback_t)ipc_timer_callback, 0);
    }
}

static ipc_status_t ipc_send_message_to_mailbox(cores_t dst, ipc_action_t action, const ipc_payload *payload_p,
                                              uint16_t payload_length, ipc_priority_e priority,
                                              bool request_ack, uint32_t *id)
{
    volatile ipc_mailbox_entry *outbox = g_ipc_mailboxes[CORE][dst];

    // Check if resource is available. Note that potentially the security core is running asynchronously to this call!
    if (outbox->status == IPC_ACTION_STATUS_CLEAR) {
        errno_t sec_ret = EOK;
        // Initialise provided values.
        outbox->dst = dst;
        outbox->action = action;
        outbox->priority = priority;
        outbox->request_ack = request_ack;

        if (payload_p != NULL) {
            // Simply use memcpy to avoid having this code 'know' about the individual payload types.
            sec_ret = memcpy_s((void *)&(outbox->payload), sizeof(ipc_payload), (void *)payload_p, payload_length);
        }

        if (sec_ret != EOK) {
            return IPC_STATUS_MEMORY_ERROR;
        }

        outbox->src = (cores_t)CORE;
        // Just use incrementing numbers for now. Don't care about wrap.
        outbox->id = ((*id)++ & 0xFFFFFFFF) | ((uint32_t)outbox->src << IPC_SHIFT_BIT);
        outbox->chksum = 0;  // Holder for when this will be used (future release)

        // Mark the outbox slot as pending an action...
        outbox->status = IPC_ACTION_STATUS_PROCESS;

        g_ipc_sinfo[dst].send_cnt++;

        // ... and inform the security core for forwarding/processing.
        hal_ipc_interrupt_core(dst);
#ifdef TEST_SUITE
        if (action != IPC_ACTION_WAKUP_INFO) {
            ipc_start_timer();
        }
#endif
    } else {
        // Tigger timer, when ipc call before rpc, rpc will block untill ipc finish.
        hal_ipc_interrupt_core(dst);
        ipc_start_timer();
        // Inform caller, so he can retry.
        return IPC_STATUS_MAILBOX_BUSY;
    }
    return IPC_STATUS_OK;
}

#if CHIP_LIBRA
static ipc_status_t ipc_send_message_to_mailbox_adapt(cores_t dst, ipc_action_t action,
                                                      const ipc_payload_adapt *payload_p, uint16_t payload_length,
                                                      ipc_priority_e priority, bool request_ack, uint32_t *id)
{
    volatile ipc_mailbox_entry_adapt *outbox = g_ipc_mailboxes_adapt[CORE][dst];

    // Check if resource is available. Note that potentially the security core is running asynchronously to this call!
    if (outbox->status == IPC_ACTION_STATUS_CLEAR) {
        errno_t sec_ret = EOK;
        // Initialise provided values.
        outbox->dst = dst;
        outbox->action = action;
        outbox->priority = priority;
        outbox->request_ack = request_ack;

        if (payload_p != NULL) {
            // Simply use memcpy to avoid having this code 'know' about the individual payload types.
            sec_ret = memcpy_s((void *)&(outbox->payload), sizeof(ipc_payload_adapt),
                               (void *)payload_p, payload_length);
        }

        if (sec_ret != EOK) {
            return IPC_STATUS_MEMORY_ERROR;
        }

        outbox->src = (cores_t)CORE;
        // Just use incrementing numbers for now. Don't care about wrap.
        outbox->id = ((*id)++ & MAX_UINT32) | ((uint32_t)outbox->src << IPC_SHIFT_BIT);
        outbox->chksum = 0;  // Holder for when this will be used (future release)

        // Mark the outbox slot as pending an action...
        outbox->status = IPC_ACTION_STATUS_PROCESS;

        g_ipc_sinfo[dst].send_cnt++;

        // ... and inform the security core for forwarding/processing.
        hal_ipc_interrupt_core(dst);
#ifdef TEST_SUITE
        if (action != IPC_ACTION_WAKUP_INFO) {
            ipc_start_timer();
        }
#endif
    } else {
        // Tigger timer, when ipc call before rpc, rpc will block untill ipc finish.
        hal_ipc_interrupt_core(dst);
        ipc_start_timer();
        // Inform caller, so he can retry.
        return IPC_STATUS_MAILBOX_BUSY;
    }
    return IPC_STATUS_OK;
}
#endif
/*
 * Check the core's outbox, and if available create the IPC message
 * and inform the security core that an IPC message should be forwarded.
 * Expected usage is that the caller will yield/busy-wait if the resource is not available.
 */
ipc_status_t ipc_send_message(cores_t dst, ipc_action_t action, const ipc_payload *payload_p,
                            uint16_t payload_length, ipc_priority_e priority, bool request_ack)
{
    ipc_status_t ret_val;
    static uint32_t id = 0;

    // Mark as volatile just in case the status field is updated by the security core to IPC_ACTION_STATUS_CLEAR.
    if (dst >= CORES_MAX_NUMBER_PHYSICAL) { return IPC_STATUS_INVALID_CORE; }

    if (priority >= IPC_PRIORITY_MAX_NUM) { return IPC_STATUS_INVALID_PRIORITY; }

    if (action >= IPC_ACTION_NUM_ACTIONS) { return IPC_STATUS_INVALID_ACTION; }

    if (ipc_slave_power_mode(dst, action) != ERRCODE_SUCC) {
        return IPC_STATUS_POWER_OFF;
    }

    uint32_t irq_sts = osal_irq_lock();

#if CHIP_LIBRA
    if ((dst == CORES_GNSS_CORE) || (CORE == GNSS)) {
        ret_val = ipc_send_message_to_mailbox_adapt(dst, action, (const ipc_payload_adapt *)payload_p, payload_length,
                                                    priority, request_ack, &id);
    } else {
        ret_val = ipc_send_message_to_mailbox(dst, action, (const ipc_payload *)payload_p, payload_length,
                                              priority, request_ack, &id);
    }
#else
    ret_val = ipc_send_message_to_mailbox(dst, action, (const ipc_payload *)payload_p, payload_length, priority,
                                          request_ack, &id);
#endif
    osal_irq_restore(irq_sts);
    return ret_val;
}

static uint32_t ipc_get_spin_send_default_timeout(void)
{
    if (g_ipc_ssend_default_timeout != 0) {
        return g_ipc_ssend_default_timeout;
    }
    uint32_t timeout_msec;
#if CORE == MASTER_BY_ALL
    timeout_msec = (watchdog_get_conifgtime() * IPC_S_TO_MS) / IPC_COMMPUTE_TIME_COEF;
#else
    timeout_msec = (watchdog_get_conifgtime() * IPC_S_TO_MS) / IPC_COMMPUTE_TIME_COEF;
#endif

    g_ipc_ssend_default_timeout = timeout_msec;

    return timeout_msec;
}

bool ipc_get_spin_fail_info(ipc_spin_send_info **spin_sinfo, cores_t dst)
{
    if (dst >= CORES_MAX_NUMBER_PHYSICAL) {
        return false;
    }

    if (g_ipc_sinfo[dst].spin_send_info.pending == 1) {
        if (spin_sinfo != NULL) {
            *spin_sinfo = (ipc_spin_send_info *)&(g_ipc_sinfo[dst].spin_send_info);
            g_ipc_sinfo[dst].spin_send_info.pending = 0;
            return true;
        }
    }

    return false;
}

#if !defined(BUILD_APPLICATION_SSB) && !defined(UNIT_TEST)
static void ipc_spin_send_fail_info_lock(hal_cpu_trace_traced_cpu_t cpu, cores_t dst, ipc_action_t action)
{
    UNUSED(cpu);
    UNUSED(dst);
    UNUSED(action);
    uint32_t pc;
    uint32_t lr;
    uint32_t sp;
    /* sec => ap */
    /* lock cpu */
    g_ipc_sinfo[dst].spin_send_info.loop_cnt = g_ipc_sinfo[dst].loop_cnt;
    g_ipc_sinfo[dst].spin_send_info.called = g_ipc_sinfo[dst].send_data_called;
    cpu_trace_lock_pclr((cpu_trace_traced_cpu_t)cpu);
    pc = cpu_trace_get_locked_pc((cpu_trace_traced_cpu_t)cpu);
    lr = cpu_trace_get_locked_lr((cpu_trace_traced_cpu_t)cpu);
    sp = cpu_trace_get_locked_sp((cpu_trace_traced_cpu_t)cpu);
    g_ipc_sinfo[dst].spin_send_info.pc = pc;
    g_ipc_sinfo[dst].spin_send_info.lr = lr;
    g_ipc_sinfo[dst].spin_send_info.sp = sp;
    g_ipc_sinfo[dst].spin_send_info.pending = 1;

#ifdef BUILD_APPLICATION_STANDARD
    oml_pf_log_print4(LOG_ACORE_PLT_BSP_DRIVER_IPC, LOG_NUM_DEBUG, LOG_LEVEL_ERROR,
                      "ipc error core %d => %d action=%d, called=0x%X", CORE, dst,
                      action, g_ipc_sinfo[dst].send_data_called);
    oml_pf_log_print4(LOG_ACORE_PLT_BSP_DRIVER_IPC, LOG_NUM_DEBUG, LOG_LEVEL_ERROR,
                      "ipc error, core=%d current pc=0x%x, lr=0x%x, sp=0x%x", dst, pc, lr, sp);
#endif
    PRINT("%d => %d ipc error, get pc=0x%x, lr=0x%x sp=0x%X", CORE, dst, pc, lr, sp);
}
#endif

static void ipc_spin_send_fail_process(cores_t dst, ipc_status_t fail_reason, ipc_action_t action)
{
#if !defined(BUILD_APPLICATION_SSB) && !defined(UNIT_TEST)
    hal_cpu_trace_traced_cpu_t lock_cpu = HAL_CPU_TRACE_TRACED_MCPU;
    if  (dst == CORES_BT_CORE) {
        lock_cpu = HAL_CPU_TRACE_TRACED_BCPU;
    } else if (dst == CORES_APPS_CORE) {
        lock_cpu = HAL_CPU_TRACE_TRACED_MCPU;
#if CHIP_LIBRA
    } else if (dst == CORES_GNSS_CORE) {
        lock_cpu = HAL_CPU_TRACE_TRACED_GCPU;
#endif
    } else {
        return;
    }
#endif
    if (fail_reason != IPC_STATUS_MAILBOX_BUSY) {
        /* process timeout reason */
        return;
    }

    if (dst >= CORES_MAX_NUMBER_PHYSICAL) {
        return;
    }

    g_ipc_sinfo[dst].spin_send_info.err_cnt++;
#if !defined(BUILD_APPLICATION_SSB) && !defined(UNIT_TEST)
    ipc_spin_send_fail_info_lock(lock_cpu, dst, action);
#endif
    UNUSED(action);
}

/* busy wait for ipc idle, this func will hold current task! */
ipc_status_t ipc_spin_send_message_timeout(cores_t dst, ipc_action_t action, const ipc_payload *payload_p,
    uint16_t payload_length, ipc_priority_e priority, bool request_ack, uint32_t timeout_msec)
{
    uint32_t i = 0;
    uint32_t j = 0;
    bool failed_flag = true;
    ipc_status_t ret_val;
    uint64_t curr_tick, timeout_tick;
    uint32_t temp_time = timeout_msec;
    if (dst >= CORES_MAX_NUMBER_PHYSICAL) {
        return IPC_STATUS_INVALID_PARAM;
    }
    ipc_slave_power_mode(dst, action);
    g_ipc_sinfo[dst].send_data_called = __return_address();

    uint64_t old_tick = uapi_systick_get_count();

    if (temp_time == IPC_SPIN_SEND_DEFAULT_TIMEOUT || timeout_msec == 0) {
        temp_time = ipc_get_spin_send_default_timeout();
    }

    timeout_tick = old_tick + ms_to_systick(temp_time);

    for (; ;) {
        j++;
        ret_val = ipc_send_message(dst, action, payload_p, payload_length, priority, request_ack);
        if (ret_val != IPC_STATUS_MAILBOX_BUSY) {
            break;
        }

        /* If ipc busy, we keep try, one loop cost about 50us and loop 100 times */
        if ((i++) < 100) {
            continue;
        }

        i = 0;
        if (j > 10000 && failed_flag) {  // 10000 times about 500ms, record m3/m7 info.
            failed_flag = false;
        }
        /* sys tick */
        curr_tick = uapi_systick_get_count();
        if (curr_tick >= timeout_tick) {
            /* timeout */
            ipc_to_bcore_timeout_proc(dst);
            break;
        }
    }

    if (ret_val != IPC_STATUS_OK) {
        /* fail process */
        g_ipc_sinfo[dst].loop_cnt = j;
        ipc_reg_info_debug();
        ipc_spin_send_fail_process(dst, ret_val, action);
    } else {
        ipc_to_bcore_clr_fail_num(dst);
        g_ipc_sinfo[dst].send_data_called = 0;
    }
    UNUSED(failed_flag);
    return ret_val;
}

#if (CORE != GNSS)
static void ipc_receive_message_from_mailbox(cores_t src)
{
    // Mark as volatile just in case the status field is updated by the security core.
    volatile ipc_mailbox_entry *inbox = g_ipc_mailboxes[src][CORE];

    // Only do anything if there is actually something to do!
    if (inbox->status == IPC_ACTION_STATUS_PROCESS) {
        bool release = true;
        ipc_action_t action = IPC_ACTION_FORCE_SIZE;
        cores_t src_core = CORES_NONE;
        uint32_t id = 0;

        if ((inbox->dst == CORE) && (inbox->action < IPC_ACTION_NUM_ACTIONS)) {
            if (g_ipc_handlers[inbox->action] != NULL) {
                // Pass received data to 'user' for processing.
                action = inbox->action;
                src_core = inbox->src;
                id = inbox->id;
                release = (g_ipc_handlers[action])(action, &inbox->payload, src_core, id);
            }
        } else {
            panic(PANIC_IPC, __LINE__);
            return;
        }

        // Regardless of processing, mark the mailbox as 'free' if handler processed all data.
        if (release) {
            inbox->status = IPC_ACTION_STATUS_CLEAR;

            if (inbox->request_ack) {
                // ... and inform the security core so the next message can be posted.
                hal_ipc_interrupt_core(src_core);
            }
        } else {
            panic(PANIC_IPC, action);
            return;
        }
    }
}
#endif

#if CHIP_LIBRA
static void ipc_receive_message_from_mailbox_adapt(cores_t src)
{
    // Mark as volatile just in case the status field is updated by the security core.
    volatile ipc_mailbox_entry_adapt *inbox = g_ipc_mailboxes_adapt[src][CORE];

    // Only do anything if there is actually something to do!
    if (inbox->status == IPC_ACTION_STATUS_PROCESS) {
        bool release = true;
        ipc_action_t action = IPC_ACTION_FORCE_SIZE;
        cores_t src_core = CORES_NONE;
        uint32_t id = 0;

        if ((inbox->dst == CORE) && (inbox->action < IPC_ACTION_NUM_ACTIONS)) {
            if (g_ipc_handlers[inbox->action] != NULL) {
                // Pass received data to 'user' for processing.
                action = inbox->action;
                src_core = inbox->src;
                id = inbox->id;
                release = (g_ipc_handlers[action])(action, (const volatile ipc_payload *)(&inbox->payload),
                                                   src_core, id);
            }
        } else {
            panic(PANIC_IPC, __LINE__);
            return;
        }

        // Regardless of processing, mark the mailbox as 'free' if handler processed all data.
        if (release) {
            inbox->status = IPC_ACTION_STATUS_CLEAR;

            if (inbox->request_ack) {
                // ... and inform the security core so the next message can be posted.
                hal_ipc_interrupt_core(src_core);
            }
        } else {
            panic(PANIC_IPC, action);
            return;
        }
    }
}
#endif
/*
 * Callback from HAL layer IRQ handler.
 */
void ipc_receive_message(cores_t src)
{
    static uint32_t count = 0;
    UNUSED(count);
    count++;
#if (CORE == GNSS)
    ipc_receive_message_from_mailbox_adapt(src);
#else
#if CHIP_LIBRA
    if (src == CORES_GNSS_CORE) {
        ipc_receive_message_from_mailbox_adapt(src);
        return;
    }
#endif
    ipc_receive_message_from_mailbox(src);
#endif
}

/*
 * Check the core's outbox is clear up or not.
 */
bool ipc_check_outbox_free(cores_t dst)
{
#if (CORE == GNSS)
    return g_ipc_mailboxes_adapt[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR;
#else
#if CHIP_LIBRA
    if (dst == CORES_GNSS_CORE) {
        return g_ipc_mailboxes_adapt[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR;
    }
#endif
    return g_ipc_mailboxes[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR;
#endif
}

ipc_status_t ipc_send_data_timeout(cores_t dst, uint16_t length, void *buffer, uint32_t timeout_msec)
{
    uint16_t send_length = length;
    uint8_t *send_buffer = (uint8_t *)buffer;
    ipc_status_t ret = IPC_STATUS_MAILBOX_BUSY;
    ipc_slave_power_mode(dst, IPC_ACTION_DATA);
    while (send_length != 0) {
        /* BUffer to send smaller than max IPC payload length */
        if (send_length <= sizeof(ipc_payload)) {
            ret = ipc_spin_send_message_timeout(dst, IPC_ACTION_DATA, (const ipc_payload *)send_buffer,
                                                send_length, IPC_PRIORITY_LOWEST, false,
                                                timeout_msec);

            break;
        } else {
            ret = ipc_spin_send_message_timeout(dst, IPC_ACTION_DATA, (const ipc_payload *)send_buffer,
                                                sizeof(ipc_payload), IPC_PRIORITY_LOWEST, false,
                                                timeout_msec);
            if (ret != IPC_STATUS_OK) {
                break;
            }

            send_buffer += sizeof(ipc_payload);
            send_length -= (uint16_t)sizeof(ipc_payload);
        }
    }

    return ret;
}

void ipc_interrupt_core(cores_t dst)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_ipc_interrupt_core(dst);
    osal_irq_restore(irq_sts);
}

ipc_status_t ipc_check_status(cores_t dst)
{
    if (dst >= CORES_MAX_NUMBER_PHYSICAL) {
        return IPC_STATUS_INVALID_CORE;
    }
#if (CORE == GNSS)
    if (g_ipc_mailboxes_adapt[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR) {
        return IPC_STATUS_OK;
    } else {
        return IPC_STATUS_MAILBOX_BUSY;
    }
#else
#if CHIP_LIBRA
    if (dst == CORES_GNSS_CORE) {
        if (g_ipc_mailboxes_adapt[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR) {
            return IPC_STATUS_OK;
        } else {
            return IPC_STATUS_MAILBOX_BUSY;
        }
    }
#endif
    if (g_ipc_mailboxes[CORE][dst]->status == IPC_ACTION_STATUS_CLEAR) {
        return IPC_STATUS_OK;
    } else {
        return IPC_STATUS_MAILBOX_BUSY;
    }
#endif
}

/*
 * set the core's outbox clear.
 */
void ipc_set_outbox_free(cores_t dst)
{
    uint32_t irq_sts = osal_irq_lock();
    switch (dst) {
        case CORES_BT_CORE:
            ipc_share_mem0_to_2.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem2_to_0.status = IPC_ACTION_STATUS_CLEAR;
#if CORE_NUMS != 2
            ipc_share_mem0_to_1.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem1_to_0.status = IPC_ACTION_STATUS_CLEAR;
#endif
#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
            ipc_share_mem0_to_3.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem3_to_0.status = IPC_ACTION_STATUS_CLEAR;
#endif
            break;
        case CORES_PROTOCOL_CORE:
#if CORE_NUMS != 2
            ipc_share_mem0_to_1.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem1_to_0.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem1_to_2.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem2_to_1.status = IPC_ACTION_STATUS_CLEAR;
#endif
            break;
        case CORES_APPS_CORE:

            ipc_share_mem0_to_2.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem2_to_0.status = IPC_ACTION_STATUS_CLEAR;
#if CORE_NUMS != 2
            ipc_share_mem1_to_2.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem2_to_1.status = IPC_ACTION_STATUS_CLEAR;
#endif
#if (DUAL_DSP_EXIST == YES) && (DSP1_IPC_ENABLE == YES)
            ipc_share_mem3_to_2.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem2_to_3.status = IPC_ACTION_STATUS_CLEAR;
#endif
#if GNSS_EXIST == YES
            ipc_share_mem_adapt2_to_3.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem_adapt3_to_2.status = IPC_ACTION_STATUS_CLEAR;
#endif
            break;
#if GNSS_EXIST == YES
        case CORES_GNSS_CORE:
            ipc_share_mem_adapt2_to_3.status = IPC_ACTION_STATUS_CLEAR;
            ipc_share_mem_adapt3_to_2.status = IPC_ACTION_STATUS_CLEAR;
            break;
#endif
        default:
            break;
    }
    osal_irq_restore(irq_sts);
}
