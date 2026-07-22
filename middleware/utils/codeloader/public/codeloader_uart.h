/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER UART MODULE HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_UART_H
#define SRC_APPLICATION_CODELOADER_UART_H

#include "platform_core.h"
#include "codeloader_common.h"
#include "uart_porting.h"

/**
 * @brief  Cause for the reception buffer to be invoked
 */
typedef enum {
    CODELOADER_CALLBACK_CAUSE_VALID_BUFFER,
    CODELOADER_CALLBACK_CAUSE_FIRST_CHARACTER_RECEIVED,
    CODELOADER_CALLBACK_CAUSE_UART_ERROR,
    CODELOADER_CALLBACK_CAUSE_FRAME_ERROR,
    CODELOADER_CALLBACK_CAUSE_BUFFER_FULL,
} codeloader_callback_cause_e;

/**
 * @brief  Type for the transmission callback
 * @param  received_data received data is a pointer to the received buffer
 * @param  event Reason why this callback was triggered. If cause is CODELOADER_CALLBACK_CAUSE_VALID_BUFFER a valid
 * frame has been received
 */
typedef void (*CODELOADER_UART_RX_DATA_CALLBACK)(const codeloader_rx_buffer_t *received_data,
                                                 codeloader_callback_cause_e cause);

/** Type for the transmission callback
 *  @note   This callback is invoked when codeloader_uart_write finishes  */
typedef void (*CODELOADER_UART_TX_DATA_CALLBACK)(void);

/**
 * @brief  Initializes the codeloader uart module with the default uart baud
 * @param  external_rx_buffer buffer to write the input frames to
 * @param  rx_callback callback to invoke with the input frame
 * @param  tx_callback callback to invoke when codeloader_uart_write has sent the data
 */
void codeloader_uart_default_init(codeloader_rx_buffer_t *external_rx_buffer,
    CODELOADER_UART_RX_DATA_CALLBACK rx_callback, CODELOADER_UART_TX_DATA_CALLBACK tx_callback);

/**
 * @brief  Deinitialize the codeloader uart module
 */
void codeloader_uart_deinit(void);

/**
 * @brief  Changes the baudrate of the uart
 * @param  new_baud The new baud rate to use
 */
void codeloader_uart_change_baud(uint32_t new_baud);

/**
 * @brief  get codeload uart bus
 * @param  none
 * @return codeloader uart
 */
uart_bus_t get_codeloader_uart(void);

/**
 * @brief  Writes data to the buffer.
 * @param  data_to_send Pointer to the buffer to send
 * @note   when it has put the last byte in the transmission fifo it invokes the transmission calback
 * registered in the invocation of codeloader_uart_init.
 */
void codeloader_uart_write(const codeloader_common_generic_buffer_t *data_to_send);

/**
 * @brief  Writes binary data as hex
 * @param  buffer to data to send
 * @param  length of data
 */
void codeloader_uart_write_raw_bin_to_hex(const uint8_t *buffer, uint16_t length);

/**
 * @brief  Returns a pointer to the transmission buffer requested.
 * @param  output_buffer pointer to the tranmission buffer
 * @param  length length of the buffer requested
 */
void codeloader_uart_get_tx_buffer(codeloader_common_generic_buffer_t *output_buffer, uint16_t length);

/**
 * @brief  Enable the reception of frames
 */
void codeloader_uart_enable_reception(void);

/**
 * @brief  Disable the reception of frames. Any received message while uart is disabled will be ignored.
 */
void codeloader_uart_disble_reception(void);

/**
 * @brief  Put the last message sent in the output_buffer
 * @param  output_buffer buffer containing the last message sent (without the crc or the start end flags)
 * @return false if no message was sent before, true otherwise.
 */
bool codeloader_uart_get_last_message(codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Waits till the last byte has been transmited
 */
void codeloader_uart_wait_to_finish_transmission(void);

/**
 * @brief  write cr lf
 */
void codeloader_uart_write_crlf(void);

#ifdef SUPPORT_ONE_LINE_UART_BURN
/**
 * @brief  get codeloader uart rx callback status
 */
bool codeloader_uart_get_rx_status(void);

/**
 * @brief  set codeloader uart
 */
void codeloader_uart_set_global_uart(uart_bus_t id);
#endif

#endif