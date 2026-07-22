/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: BT CODELOADER UART MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "non_os.h"
#include "uart.h"
#include "codeloader_common.h"  // codeloader_common_generic_buffer_t
#include "codeloader_uart.h"
#include <string.h>  // memcpy
#ifdef SUPPORT_ONE_LINE_UART_BURN
#include "chip_io.h"
#endif

/** Codeloader UART baud rate Configuration */
#define CODELOADER_UART_DEFAULT_BAUD_RATE 115200
/** Codeloader UART data bits configuration */
#define CODELOADER_UART_DATA_BITS UART_DATA_BIT_8
/** Codeloader UART parity mode configuration */
#define CODELOADER_UART_PARITY UART_PARITY_NONE
/** Codeloader UART stop bits configuration */
#define CODELOADER_UART_STOP_BITS UART_STOP_BIT_1
/** Codeloader UART Transmission PIN to use */
#define CODELOADER_UART_CTS_PIN PIN_NONE
/** Codeloader UART Reception PIN to use */
#define CODELOADER_UART_RTS_PIN PIN_NONE
/** Codeloader UART Transmission buffer size (bytes) */
#define CODELOADER_UART_TX_MAX_BUFFER_SIZE CODELOADER_TX_BUFFER_SIZE  // Maximum output message must fit here
/** Codeloader UART Internal Reception buffer size (bytes) */
#define CODELOADER_UART_RX_MAX_BUFFER_SIZE 1  // Set to 1 because we want to check every byte
/** Beginning of frame flag */
#define CODELOADER_UART_FLAG_CHAR_START 's'
/** End of frame flag */
#define CODELOADER_UART_FLAG_CHAR_END 'e'
/** Escape flag */
#define CODELOADER_UART_FLAG_CHAR_ESCAPE 0x5C  // 0x5C = '\'
#define NIBBLE_LENGTH 10
#define BIN_BYTE_RIGHT_SHIFT_BIT 4
#define HEX_LENGTH 2

/**
 * Codeloader Transmission Buffer Type
 */
//lint -esym(754, codeloader_tx_buf_t::buffer_size)
typedef struct {
    volatile bool in_use;
    volatile uint16_t buffer_size;
    uint8_t buffer[CODELOADER_UART_TX_MAX_BUFFER_SIZE];
} __attribute__((packed)) codeloader_tx_buf_t;


/** Codeloader UART Internal Reception buffer  */
static codeloader_tx_buf_t g_codeloader_tx_uart_buffer __attribute__((aligned(4)));
/** Codeloader Callback to invoke when an event has ocurred in the line or a valid frame has been received */
static CODELOADER_UART_RX_DATA_CALLBACK g_codeloader_uart_received_data_callback;
/** Codeloader Callback to invoke when the data to send has been sent completelly */
static CODELOADER_UART_TX_DATA_CALLBACK g_codeloader_uart_sent_data_callback;
/** Codeloader UART Handle to interact with the driver */
static uart_bus_t g_codeload_uart = CODELOADER_UART_BUS;
/** Pointer to the external RX buffer */
static codeloader_rx_buffer_t *g_cl_uart_ex_rx_buffer;
/** External RX buffer size */
static uint32_t g_codeloader_uart_external_rx_buffer_size;
/** Internal buffer for the driver to write the data received */
static uint8_t g_codeloader_uart_internal_buffer[CODELOADER_UART_RX_MAX_BUFFER_SIZE];
/** Buffer pointing to the last data sent */
static codeloader_common_generic_buffer_t g_codeloader_uart_last_data_sent;
/** Flag to indicate if the reception is enabled or not.  If it is not enabled any received data will be ignored. */
static bool g_codeloader_uart_reception_enabled;
#ifdef SUPPORT_ONE_LINE_UART_BURN
static bool g_codeloader_first_msg = true;
static bool g_codeloader_one_line_callback = false;
#endif
/*
 *  Private function declarations
 */
/**
 * Callback to handle the data received from the uart driver
 * @param buffer buffer received
 * @param length length of the data received
 * @param error indicates if an error has been detected in the uart
 */
static void codeloader_uart_callback(const void *buffer, uint16_t length, bool error);
#ifdef SUPPORT_ONE_LINE_UART_BURN
static void codeloader_one_line_uart_callback(const void *buffer, uint16_t length, bool error);
#endif

/*
 *  Public function definitions
 */
uart_bus_t get_codeloader_uart(void)
{
    return g_codeload_uart;
}

#ifdef SUPPORT_ONE_LINE_UART_BURN
void codeloader_uart_switch_trx(bool switch_trx)
{
    uint32_t cfg = readl(UART_SWITCH_TRX_REG_ADDR);

    if (switch_trx) {
        cfg = cfg | BIT(ONE_LINE_UART_SWITCH_BIT_OFFSET);
    } else {
        cfg = cfg & (~BIT(ONE_LINE_UART_SWITCH_BIT_OFFSET));
    }

    writel(UART_SWITCH_TRX_REG_ADDR, cfg);
}
#endif

static void codeloader_uart_write_immediately_char(uart_bus_t uart, char c)
{
    char buffer[2];
    buffer[0] = c;
    buffer[1] = 0;
    UNUSED(buffer);
#ifdef SUPPORT_ONE_LINE_UART_BURN
    if (g_codeloader_first_msg) {
        /* for init message send to both uart. */
        uapi_uart_write(CODELOADER_ONE_LINE_UART_BUS, (const void *)buffer, 1, 0);
    }
#endif
    uapi_uart_write(uart, (const void *)buffer, 1, 0);
}

static void codeloader_uart_write_scatter(uart_bus_t uart, const codeloader_common_generic_buffer_t *data_to_send)
{
    uint8_t *buffer = NULL;
    uint16_t length;

    g_codeloader_uart_last_data_sent.buffer = data_to_send->buffer;
    g_codeloader_uart_last_data_sent.length = data_to_send->length;

    buffer = data_to_send->buffer;
    length = (uint16_t)data_to_send->length;
#ifdef SUPPORT_ONE_LINE_UART_BURN
    /* switch uart tx mode when sending with one_line uart */
    if (uart == CODELOADER_ONE_LINE_UART_BUS || g_codeloader_first_msg) {
        codeloader_uart_switch_trx(false);
    }
#endif

    // send the start sequence
    codeloader_uart_write_immediately_char(uart, CODELOADER_UART_FLAG_CHAR_ESCAPE);
    codeloader_uart_write_immediately_char(uart, CODELOADER_UART_FLAG_CHAR_START);

    // send the frame
    while (length > 0) {
        codeloader_uart_write_immediately_char(uart, *buffer);
        if (*buffer == CODELOADER_UART_FLAG_CHAR_ESCAPE) {  // if it is the scape character send it again
            codeloader_uart_write_immediately_char(uart, *buffer);
        }
        buffer++;
        length--;
    }

    // send the end sequence
    codeloader_uart_write_immediately_char(uart, CODELOADER_UART_FLAG_CHAR_ESCAPE);

    //    for the last character make it atomic with the callback
    non_os_enter_critical();
    codeloader_uart_write_immediately_char(uart, CODELOADER_UART_FLAG_CHAR_END);

    g_codeloader_tx_uart_buffer.in_use = false;

    if (g_codeloader_uart_sent_data_callback) {
        g_codeloader_uart_sent_data_callback();
    }

    non_os_exit_critical();

#ifdef SUPPORT_ONE_LINE_UART_BURN
    if (uart == CODELOADER_ONE_LINE_UART_BUS || g_codeloader_first_msg) {
        while (uapi_uart_has_pending_transmissions(CODELOADER_ONE_LINE_UART_BUS)) {};
        codeloader_uart_switch_trx(true);
    }
    g_codeloader_first_msg = false;
#endif
}

void codeloader_uart_write(const codeloader_common_generic_buffer_t *data_to_send)
{
    codeloader_uart_write_scatter(g_codeload_uart, data_to_send);
}

static void bin_to_hex(uint8_t bin_byte, uint8_t *hex_output, const uint8_t max_hex_output_len)
{
    UNUSED(max_hex_output_len);
    uint8_t nibble;

    nibble = (bin_byte & 0x0f);
    if (nibble < NIBBLE_LENGTH) {
        *(hex_output + 1) = nibble + '0';
    } else {
        *(hex_output + 1) = (nibble - 0x0a) + 'A';
    }
    nibble = bin_byte >> BIN_BYTE_RIGHT_SHIFT_BIT;
    if (nibble < NIBBLE_LENGTH) {
        *hex_output = nibble + '0';
    } else {
        *hex_output = (nibble - 0x0a) + 'A';
    }
}

void codeloader_uart_write_raw_bin_to_hex(const uint8_t *buffer, uint16_t length)
{
    uint8_t hex_output[HEX_LENGTH];

    // send the buffer
    while (length > 0) {
        bin_to_hex(*buffer, hex_output, sizeof(hex_output));
        codeloader_uart_write_immediately_char(g_codeload_uart, hex_output[1]);
        codeloader_uart_write_immediately_char(g_codeload_uart, hex_output[0]);
        buffer++;
        length--;
    }
}

void codeloader_uart_write_crlf(void)
{
#define ASCII_LF 0x0a
#define ASCII_CR 0x0d
    codeloader_uart_write_immediately_char(g_codeload_uart, ASCII_CR);
    codeloader_uart_write_immediately_char(g_codeload_uart, ASCII_LF);
}

bool codeloader_uart_get_last_message(codeloader_common_generic_buffer_t *output_buffer)
{
    if (g_codeloader_uart_last_data_sent.buffer != NULL && g_codeloader_uart_last_data_sent.length > 0) {
        output_buffer->buffer = g_codeloader_uart_last_data_sent.buffer;
        output_buffer->length = g_codeloader_uart_last_data_sent.length - 1;
        return true;
    } else {
        return false;
    }
}

static void codeloader_uart_init(uart_bus_t uart, uart_rx_callback_t callback)
{
    uart_pin_config_t codeload_uart_pins;
    uart_attr_t uart_line_config;
    uart_buffer_config_t uart_buffer_config;

    // Claim the UART here
    codeload_uart_pins.tx_pin = CODELOADER_UART_TX_PIN;
    codeload_uart_pins.rx_pin = CODELOADER_UART_RX_PIN;
#ifdef SUPPORT_ONE_LINE_UART_BURN
    if (uart == CODELOADER_ONE_LINE_UART_BUS) {
        codeload_uart_pins.tx_pin = CODELOADER_ONE_LINE_UART_TX_PIN;
        codeload_uart_pins.rx_pin = CODELOADER_ONE_LINE_UART_TX_PIN;
    }
#endif

    codeload_uart_pins.rts_pin = CODELOADER_UART_RTS_PIN;
    codeload_uart_pins.cts_pin = CODELOADER_UART_CTS_PIN;

    // UART LINE Configuration
    uart_line_config.baud_rate = CODELOADER_UART_DEFAULT_BAUD_RATE;
    uart_line_config.data_bits = CODELOADER_UART_DATA_BITS;
    uart_line_config.parity = CODELOADER_UART_PARITY;
    uart_line_config.stop_bits = CODELOADER_UART_STOP_BITS;

    uart_buffer_config.rx_buffer = g_codeloader_uart_internal_buffer;
    uart_buffer_config.rx_buffer_size = CODELOADER_UART_RX_MAX_BUFFER_SIZE;

    (void)uapi_uart_init(uart, &codeload_uart_pins, &uart_line_config, NULL, &uart_buffer_config);
    uapi_uart_register_rx_callback(uart, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
        CODELOADER_UART_RX_MAX_BUFFER_SIZE, callback);
}

void codeloader_uart_default_init(codeloader_rx_buffer_t *external_rx_buffer,
    CODELOADER_UART_RX_DATA_CALLBACK rx_callback, CODELOADER_UART_TX_DATA_CALLBACK tx_callback)
{
    g_cl_uart_ex_rx_buffer = external_rx_buffer;
    g_codeloader_uart_external_rx_buffer_size = external_rx_buffer->length;
    external_rx_buffer->length = 0;
    g_codeloader_uart_last_data_sent.buffer = NULL;
    g_codeloader_uart_last_data_sent.length = 0;
    g_codeloader_tx_uart_buffer.in_use = false;

    g_codeloader_uart_received_data_callback = rx_callback;
    g_codeloader_uart_sent_data_callback = tx_callback;

#ifndef SUPPORT_ONE_LINE_UART_BURN
    codeloader_uart_init(g_codeload_uart, codeloader_uart_callback);
#else
    if (g_codeloader_first_msg) {
        /* Before send init message, to distinguish which serial port responds to the handshake information,
           we initialize two UARTs with different callbacks. */
        codeloader_uart_init(g_codeload_uart, codeloader_uart_callback);
        codeloader_uart_init(CODELOADER_ONE_LINE_UART_BUS, codeloader_one_line_uart_callback);
    } else {
        /* After init handshake, g_codeload_uart equal to connected uart_bus.
           And there is no need to register different callback. */
        codeloader_uart_init(g_codeload_uart, codeloader_uart_callback);
    }
#endif
}

void codeloader_uart_deinit(void)
{
    while (uapi_uart_has_pending_transmissions(g_codeload_uart)) {};

    uapi_uart_deinit(g_codeload_uart);
#ifdef SUPPORT_ONE_LINE_UART_BURN
    codeloader_uart_switch_trx(false);
    uapi_uart_deinit(CODELOADER_ONE_LINE_UART_BUS);
#endif
    return;
}

void codeloader_uart_change_baud(uint32_t new_baud)
{
    /*
     * this is the same as the de-init code, but I did not want to copy that in-case the
     * de-init changed to release buffers as well
     */
    while (uapi_uart_has_pending_transmissions(g_codeload_uart)) {};
    uapi_uart_deinit(g_codeload_uart);

    // now re-init the uart with the new settings
    uart_attr_t uart_line_config;
    uart_buffer_config_t uart_buffer_config;
    uart_pin_config_t codeload_uart_pins;

    // TX configuration
    codeload_uart_pins.tx_pin = CODELOADER_UART_TX_PIN;
    codeload_uart_pins.rts_pin = CODELOADER_UART_RTS_PIN;
    // RX configuration
    codeload_uart_pins.rx_pin = CODELOADER_UART_RX_PIN;
    codeload_uart_pins.cts_pin = CODELOADER_UART_CTS_PIN;
#ifdef SUPPORT_ONE_LINE_UART_BURN
    if (g_codeload_uart == CODELOADER_ONE_LINE_UART_BUS) {
        codeload_uart_pins.tx_pin = CODELOADER_ONE_LINE_UART_TX_PIN;
        codeload_uart_pins.rx_pin = CODELOADER_ONE_LINE_UART_TX_PIN;
    }
#endif

    // UART LINE Configuration
    uart_line_config.baud_rate = new_baud;
    uart_line_config.data_bits = CODELOADER_UART_DATA_BITS;
    uart_line_config.parity = CODELOADER_UART_PARITY;
    uart_line_config.stop_bits = CODELOADER_UART_STOP_BITS;

    uart_buffer_config.rx_buffer = g_codeloader_uart_internal_buffer;
    uart_buffer_config.rx_buffer_size = CODELOADER_UART_RX_MAX_BUFFER_SIZE;

    uapi_uart_init(g_codeload_uart, &codeload_uart_pins, &uart_line_config, NULL, &uart_buffer_config);
    uapi_uart_register_rx_callback(g_codeload_uart, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
        CODELOADER_UART_RX_MAX_BUFFER_SIZE, codeloader_uart_callback);
}

/* get buffer for transmission */
void codeloader_uart_get_tx_buffer(codeloader_common_generic_buffer_t *output_buffer, uint16_t length)
{
    non_os_enter_critical();
    g_codeloader_tx_uart_buffer.in_use = true;
    non_os_exit_critical();

    output_buffer->buffer = g_codeloader_tx_uart_buffer.buffer;
    output_buffer->length = length;
}

/* enables the recording of the received messages */
void codeloader_uart_enable_reception(void)
{
    non_os_enter_critical();
    g_codeloader_uart_reception_enabled = true;
    non_os_exit_critical();
}

/* Ignores any received messages */
void codeloader_uart_disble_reception(void)
{
    non_os_enter_critical();
    g_codeloader_uart_reception_enabled = false;
    non_os_exit_critical();
}

void codeloader_uart_wait_to_finish_transmission(void)
{
    while (uapi_uart_has_pending_transmissions(g_codeload_uart)) {};
}

static bool codeloader_uart_callback_check_end(const void *buffer, uint16_t length, codeloader_callback_cause_e *cause)
{
    bool invoke_external_callback = false;
    uint8_t *received_buffer = (uint8_t *)buffer;
    static bool escaped_flag = false;
    bool write_to_buffer = true;
    while (length > 0) {
        write_to_buffer = true;
        uint8_t received_data = *received_buffer;

        if (!escaped_flag) {
            if (received_data == CODELOADER_UART_FLAG_CHAR_ESCAPE) {
                escaped_flag = true;
                write_to_buffer = false;
            }
        } else {  // the previous one was a escape
            escaped_flag = false;
            if (received_data == CODELOADER_UART_FLAG_CHAR_ESCAPE) {  // double escape
            } else if (received_data == CODELOADER_UART_FLAG_CHAR_END) {
                write_to_buffer = false;
                invoke_external_callback = true;
                *cause = CODELOADER_CALLBACK_CAUSE_VALID_BUFFER;
            } else if (received_data == CODELOADER_UART_FLAG_CHAR_START) {
                g_cl_uart_ex_rx_buffer->length = 0;
                write_to_buffer = false;
            } else {  // if any other flag restart ( this should not happen )
                g_cl_uart_ex_rx_buffer->length = 0;
                write_to_buffer = false;
                invoke_external_callback = true;  // buffer full
                *cause = CODELOADER_CALLBACK_CAUSE_FRAME_ERROR;
            }
        }

        if (write_to_buffer) {
            *(g_cl_uart_ex_rx_buffer->buffer + g_cl_uart_ex_rx_buffer->length) = *received_buffer;
            g_cl_uart_ex_rx_buffer->length++;
            if (g_cl_uart_ex_rx_buffer->length >= g_codeloader_uart_external_rx_buffer_size) {
                invoke_external_callback = true;  // buffer full
                *cause = CODELOADER_CALLBACK_CAUSE_BUFFER_FULL;
            }
        }

        if (invoke_external_callback) {
            escaped_flag = false;  // and not write anything to the buffer
            break;
        }
        received_buffer++;
        length--;
    }
    return invoke_external_callback;
}

//lint -esym(459, codeloader_uart_callback)
static void codeloader_uart_callback(const void *buffer, uint16_t length, bool error)
{
    if (error) {
        if (g_codeloader_uart_received_data_callback != NULL) {
            g_codeloader_uart_received_data_callback(g_cl_uart_ex_rx_buffer, CODELOADER_CALLBACK_CAUSE_UART_ERROR);
            g_cl_uart_ex_rx_buffer->length = 0;
        }
        return;
    }

    if (!g_codeloader_uart_reception_enabled) {  // if the buffer is in use discard the data
        return;
    }

    // iterate to check for the end of message
    // Initially assume the frame is incorrect, parsing will prove otherwise
    codeloader_callback_cause_e cause = CODELOADER_CALLBACK_CAUSE_FRAME_ERROR;
    bool invoke_external_callback = codeloader_uart_callback_check_end(buffer, length, &cause);
    if (invoke_external_callback && g_codeloader_uart_received_data_callback != NULL) {
        g_codeloader_uart_received_data_callback(g_cl_uart_ex_rx_buffer, cause);
        g_cl_uart_ex_rx_buffer->length = 0;
#ifdef SUPPORT_ONE_LINE_UART_BURN
    } else {
        g_codeloader_one_line_callback = false;
    }
#else
    }
#endif
}

#ifdef SUPPORT_ONE_LINE_UART_BURN

bool codeloader_uart_get_rx_status(void)
{
    return g_codeloader_one_line_callback;
}

void codeloader_uart_set_global_uart(uart_bus_t id)
{
    g_codeload_uart = id;
}

static void codeloader_one_line_uart_callback(const void *buffer, uint16_t length, bool error)
{
    g_codeloader_one_line_callback = true;
    codeloader_uart_callback(buffer, length, error);
}
#endif
