/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: BT Codeloader Module
 */

#include <stdint.h>
#include <stdbool.h>
#include "non_os.h"
#include "codeloader.h"
#include "tcxo.h"
#include "hal_uart.h"
#include "hal_reboot.h"
#include "flash.h"
#include "flash_porting.h"
#include "flash_common_config.h"
#include "codeloader_common.h"
#include "codeloader_uart.h"
#include "codeloader_message_layer.h"
#include "codeloader_command_layer.h"
#include "codeloader_line_layer.h"
#include "securec.h"
#include "codeloader_command_processor_utils.h"
#include "codeloader_locking.h"
#include "codeloader_execute_command_control_commands.h"
#ifdef SUPPORT_PARTITION_INFO
#include "partition.h"
#endif

#if defined(BUILD_APPLICATION_SSB) || defined(BUILD_APPLICATION_ROM)
#include "watchdog.h"
#endif

uint16_t flash_config_get_kv_pages(void);

/*
 *  Configuration parameters
 */
/** Milliseconds to wait for connection after the init message has been sent */
#define CODELOADER_SHORT_WAIT_FOR_CONNECTION_MS 200UL    // !< 200 milliseconds
#define CODELOADER_LONG_WAIT_FOR_CONNECTION_MS  600000UL // !< 10 minutes

/** Milliseconds between successive code loader init messages issued over UART */
#define CODELOADER_INIT_MESSAGE_REPEAT_MS 18000UL

/** Maximum receiver buffer size for the codeloader */
#ifdef LARGE_BUFFER
#define CODELOADER_MAX_RX_BUFFER_SIZE ((5 * FLASH_PAGE_SIZE) + 256)
#else
#define CODELOADER_MAX_RX_BUFFER_SIZE ((2 * FLASH_PAGE_SIZE) + 256)
#endif
/*
 *  Private definitions
 */
/** Size of the line, layer and message headers before the parameters */
#define CODELOADER_MAIN_HEADER_SIZE_BEFORE_PARAMETERS 2
/** Buffer offset that prevents miss-alignment of the data to write to the flash */
#define CODELOADER_MAIN_BUFFER_OFFSET (4 - CODELOADER_MAIN_HEADER_SIZE_BEFORE_PARAMETERS % 4)
/** Codeloader UART Internal Reception buffer size (bytes) */
#define CODELOADER_UART_RX_BUFFER_SIZE (CODELOADER_MAX_RX_BUFFER_SIZE - CODELOADER_MAIN_BUFFER_OFFSET)

//lint -esym(754, codeloader_rx_buffer_container_struct::paddding_bytes)   Padding
struct __attribute__((__packed__)) codeloader_rx_buffer_container_struct {
    uint8_t paddding_bytes[CODELOADER_MAIN_BUFFER_OFFSET];
    uint8_t codeloader_reception_buffer[CODELOADER_MAX_RX_BUFFER_SIZE];
} codeloader_rx_buffer_container __attribute__((aligned(4)));

/** Codeloader module state vector */
static volatile codeloader_state_e g_codeloader_main_state;
/** Codeloader UART Internal Reception buffer for initialization */
static codeloader_rx_buffer_t g_codeloader_main_rx_buffer;
/** Codeloader UART Internal Reception buffer to process */
static codeloader_rx_buffer_t g_codeloader_main_received_buffer_to_process;
/** flag to indicate that there is data to process */
static volatile bool g_codeloader_main_there_is_data_to_process;
/** Pointer to the codeloader reception buffer  */
//lint -esym(843, g_codeloader_reception_buffer)  Don't whine about var could be constant.
static uint8_t *g_codeloader_reception_buffer = codeloader_rx_buffer_container.codeloader_reception_buffer;
/** Indicates if Codeloader has updated the security core image */
static bool g_codeloader_security_core_updated = false;

/**
 * Erases the supplied codeloader rx buffer
 * @param codeloader_buffer Pointer to codeloader rx buffer to be erased
 */
static void codeloader_erase_buffer(const codeloader_rx_buffer_t *codeloader_buffer);

/**
 * Sends the welcome message
 */
static void codeloader_main_send_init_message(const codeloader_options_t *options);

/**
 * Get function for the data process internal flag.
 * @return true if there is valid data to process that should be given to the data handler, false otherwise.
 */
static bool codeloader_main_is_there_data_to_process(void);

/**
 * Takes care of the received data and generates a reply internally.
 * @param received_buffer pointer to the received buffer
 */
static void codeloader_main_rx_data_handler(const codeloader_rx_buffer_t *received_buffer);

/**
 * Callback being invoked when a valid frame has arrived to the uart or an error has ocurred.
 * @param data pointer to the data buffer
 * @param cause Reason for the callback
 */
static void codeloader_main_rx_data_callback(const codeloader_rx_buffer_t *data, codeloader_callback_cause_e cause);

/**
 *  Callback being invoked when the transmission of the reply has been completed.
 */
static void codeloader_main_tx_data_callback(void);

/* Initialize the codeloader resources */
void codeloader_init(const codeloader_options_t *options)
{
    g_codeloader_main_rx_buffer.buffer = g_codeloader_reception_buffer;
    g_codeloader_main_rx_buffer.length = CODELOADER_UART_RX_BUFFER_SIZE;
    g_codeloader_main_state = CODELOADER_STATE_WAITING_FOR_INIT;

    g_codeloader_security_core_updated = false;
    codeloader_lock_init(options);

    codeloader_uart_default_init(&g_codeloader_main_rx_buffer, codeloader_main_rx_data_callback,
                                 codeloader_main_tx_data_callback);
}

/* De-initialize the Codeloader resources */
void codeloader_deinit(void)
{
    codeloader_uart_deinit();
    codeloader_erase_buffer(&g_codeloader_main_rx_buffer);
}

/*
 * Erases the supplied codeloader rx buffer
 */
static void codeloader_erase_buffer(const codeloader_rx_buffer_t *codeloader_buffer)
{
    if ((codeloader_buffer != NULL) && (codeloader_buffer->buffer != NULL)) {
        memset_s(codeloader_buffer->buffer, codeloader_buffer->length, 0, codeloader_buffer->length);
    }
}

/* Main loop to run while the Codeloader is active */
bool codeloader_main_loop(const codeloader_options_t *options, codeloader_retp_t *cl_retp)
{
    bool codeloader_executed = false;  // true if it passes the init message

    cl_retp->running_from_bootloader_ram = false;

    codeloader_init(options);  // Initialise the needed modules and internal state

    // TCXO time when the codeloader considers it has timed out
    uint64_t start_time = uapi_tcxo_get_ms();
    uint64_t time_out = CODELOADER_SHORT_WAIT_FOR_CONNECTION_MS;
    if (options->use_long_timeout) {  // Flash is *not* valid, so wait for an extended amount of time in the codeloader
        time_out = CODELOADER_LONG_WAIT_FOR_CONNECTION_MS;
    }

    // Will cause an immediate send of the first codeloader init message
    uint32_t last_init_message_period = (uint32_t)-1;
    while ((codeloader_main_get_state() == CODELOADER_STATE_WAITING_FOR_INIT) &&
           ((uapi_tcxo_get_ms() - start_time) < time_out)) {
        uint32_t init_message_period = (uint32_t)((uapi_tcxo_get_ms() - start_time) /
                                                  CODELOADER_INIT_MESSAGE_REPEAT_MS);
        if (init_message_period != last_init_message_period) {  // Issue codeloader init message
            // Should not be re-issued if codeloader has moved out of its WAITING_FOR_INIT state
            last_init_message_period = init_message_period;
            codeloader_main_send_init_message(options);
        }

        if (codeloader_main_is_there_data_to_process()) {
            codeloader_main_rx_data_handler(&g_codeloader_main_received_buffer_to_process);
        }

#if defined(BUILD_APPLICATION_SSB) || defined(BUILD_APPLICATION_ROM)
        // kick the watch dog, we could be here some time
        uapi_watchdog_kick();
#endif
    }

    // if we are in waiting for init state we have timedout
    if (codeloader_main_get_state() != CODELOADER_STATE_WAITING_FOR_INIT) {
        codeloader_executed = true;
        // while we don't receive the exit code we keep running the codeloader
        while (codeloader_main_get_state() != CODELOADER_STATE_EXIT) {
            if (codeloader_main_is_there_data_to_process()) {
                codeloader_main_rx_data_handler(&g_codeloader_main_received_buffer_to_process);
            }

#if defined(BUILD_APPLICATION_SSB) || defined(BUILD_APPLICATION_ROM)
            // kick the watch dog, we could be here some time
            uapi_watchdog_kick();
#endif
        }
    }

    codeloader_deinit();
    cl_retp->running_from_bootloader_ram = codeloader_command_processor_is_running_from_bootloader_ram();
    cl_retp->security_core_updated = g_codeloader_security_core_updated;
    return codeloader_executed;
}

void codeloader_main_set_state(codeloader_state_e state)
{
    non_os_enter_critical();
    g_codeloader_main_state = state;
    non_os_exit_critical();
}

codeloader_state_e codeloader_main_get_state(void)
{
    return g_codeloader_main_state;
}

/*
 * Log that Codeloader has updated the security core image
 */
void codeloader_set_security_core_updated(void)
{
    g_codeloader_security_core_updated = true;
}

static void codeloader_main_send_init_message(const codeloader_options_t *options)
{
    codeloader_message_layer_send_init_message(options);
}

static bool codeloader_main_is_there_data_to_process(void)
{
    return g_codeloader_main_there_is_data_to_process;
}

static void codeloader_line_layer_succ_action(codeloader_rx_buffer_t *processing_buffer,
                                              codeloader_common_generic_buffer_t *output_buffer,
                                              codeloader_message_layer_nack_message_code_e *nack_message,
                                              bool *send_message)
{
    codeloader_message_layer_return_e message_layer_return_value;
    codeloader_command_layer_return_e codeloader_layer_return_value;
    codeloader_common_generic_buffer_t input_buffer;

    message_layer_return_value = codeloader_message_layer_process_input_message(processing_buffer);
    if (message_layer_return_value == CODELOADER_MESSAGE_LAYER_RETURN_SUCCESS) {
        control_command_uart_message_recieved_set(true);  // Used as a flag for the change uart speed command
        input_buffer.buffer = processing_buffer->buffer;
        input_buffer.length = (uint16_t)processing_buffer->length;
        codeloader_layer_return_value = codeloader_command_layer_process_input_message(processing_buffer,
                                                                                       output_buffer);

        switch (codeloader_layer_return_value) {
            case CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER:
                // only form the reply message if needed
                codeloader_message_layer_form_reply(output_buffer, &input_buffer);
                break;
            case CODELOADER_COMMAND_LAYER_RETURN_DO_NOT_SEND_ANY_REPLY:
                *send_message = false;
                break;
            case CODELOADER_COMMAND_LAYER_RETURN_RESEND_LAST_MESSAGE:
                // if no message has been sent just ignore it
                *send_message = (codeloader_uart_get_last_message(output_buffer)) ? true : false;
                break;
            default:
                break;
        }
    } else {
        // only reply if the state is not wating for init
        if (codeloader_main_get_state() == CODELOADER_STATE_WAITING_FOR_INIT) {
            *send_message = false;
        } else {
            *nack_message = CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_MESSAGE_BAD_FORMED;
            codeloader_message_layer_form_nack_message(output_buffer, *nack_message);
        }
    }
}

static void codeloader_main_rx_data_handler(const codeloader_rx_buffer_t *received_buffer)
{
    codeloader_rx_buffer_t processing_buffer;
    codeloader_line_layer_return_e line_layer_return_value;
    bool send_message = true;
    codeloader_message_layer_nack_message_code_e nack_message;
    codeloader_common_generic_buffer_t output_buffer;

    processing_buffer.buffer = received_buffer->buffer;
    processing_buffer.length = received_buffer->length;

    line_layer_return_value = codeloader_line_layer_process_input_message(&processing_buffer);
    if (line_layer_return_value == CODELOADER_LINE_LAYER_RETURN_SUCCESS) {
        codeloader_line_layer_succ_action(&processing_buffer, &output_buffer, &nack_message, &send_message);
    } else {
        switch (line_layer_return_value) {
            case CODELOADER_LINE_LAYER_RETURN_BAD_CRC:
                nack_message = CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_LINE_BAD_CRC;
                break;
            case CODELOADER_LINE_LAYER_RETURN_BAD_FORMED_MESSAGE:
                nack_message = CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_LINE_BAD_FORMED;
                break;
            default:
                nack_message = CODELOADER_MESSAGE_LAYER_NACK_GENERIC_ERROR;
                break;
        }
        // only reply if the state is not wating for init
        if (codeloader_main_get_state() == CODELOADER_STATE_WAITING_FOR_INIT) {
            send_message = false;
        } else {
            codeloader_message_layer_form_nack_message(&output_buffer, nack_message);
        }
    }

    // only reply if the state is not wating for init
    if (send_message) {
        codeloader_line_layer_send(&output_buffer);
    } else {  // listen again
        g_codeloader_main_there_is_data_to_process = false;
        codeloader_uart_enable_reception();
    }
}

//lint -esym(459, codeloader_main_rx_data_callback)
//lint -esym(459, codeloader_main_tx_data_callback)
static void codeloader_main_rx_data_callback(const codeloader_rx_buffer_t *received_buffer,
    codeloader_callback_cause_e event)
{
    if (event == CODELOADER_CALLBACK_CAUSE_VALID_BUFFER) {
        codeloader_uart_disble_reception();
        g_codeloader_main_received_buffer_to_process.buffer = received_buffer->buffer;
        g_codeloader_main_received_buffer_to_process.length = received_buffer->length;
        g_codeloader_main_there_is_data_to_process = true;
    }
}

static void codeloader_main_tx_data_callback(void)
{
    g_codeloader_main_there_is_data_to_process = false;
    if (codeloader_main_get_state() == CODELOADER_STATE_UART_SPEED_CHANGING) {
        codeloader_command_processor_set_uart_speed_set();
#ifndef BUILD_APPLICATION_ROM
    } else if (codeloader_main_get_state() == CODELOADER_STATE_ERASE_NV_AND_RESET) {
#ifdef SUPPORT_PARTITION_INFO
        partition_information_t info;
        errcode_t ret_val = uapi_partition_get_info(PARTITION_NV_DATA, &info);
        if (ret_val == ERRCODE_SUCC && info.type == PARTITION_BY_ADDRESS) {
            uapi_flash_block_erase(CHIP_FLASH_ID, info.part_info.addr_info.addr, info.part_info.addr_info.size, true);
        }
#else
        uint32_t start_addr = ((flash_config_get_kv_pages() - SYSTEM_RESERVED_FLASH_PAGES) - 1) * FLASH_PAGE_SIZE;
        uapi_flash_block_erase(CHIP_FLASH_ID, start_addr, SYSTEM_RESERVED_FLASH_PAGES * FLASH_PAGE_SIZE, true);
#endif
        hal_reboot_chip();
    } else if (codeloader_main_get_state() == CODELOADER_STATE_ERASE_SSB_AND_RESET) {
        codeloader_uart_wait_to_finish_transmission();
        memset_s((void *)SSB_BOOT_RAM_ORIGIN, APP_VECTORS_LENGTH, 0, APP_VECTORS_LENGTH);
        hal_reboot_chip();
#endif
    } else {
        codeloader_uart_enable_reception();
    }
}

void codeloader_error_log_record(uint8_t cmd_code, int8_t cmd_ret_value)
{
#ifndef BUILD_APPLICATION_ROM
    if (cmd_ret_value != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
        codeloader_log_t error_log;
        error_log.command_code = cmd_code;
        error_log.command_ret_value = cmd_ret_value;
        // write codeloader error log to flash. To do.
        UNUSED(error_log);
    }
#else
    UNUSED(cmd_code);
    UNUSED(cmd_ret_value);
#endif
}
