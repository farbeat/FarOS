/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Codeloader Module Header File
 * Author:
 * Date:
 */
#ifndef CODELOADER_H
#define CODELOADER_H

#include <stdbool.h>
#include "codeloader_common.h"

/**
 * @brief  Codeloader Type
 * Any change to this definitions must be documented in NL-002662-SP.
 */
typedef enum {
    CODELOADER_TYPE_LEGACY_FSB = 0,
    CODELOADER_TYPE_LEGACY_SCFLASH = 1,
    CODELOADER_TYPE_FSB = 2,
    CODELOADER_TYPE_SSB = 3,
    CODELOADER_TYPE_SCFLASH = 4,
} codeloader_type_e;

/**
 * @brief  Codeloader Chip
 * Any change to this definitions must be documented in NL-002662-SP.
 */
typedef enum {
    CODELOADER_CHIP_0 = 0,
    CODELOADER_CHIP_1 = 1,
} codeloader_chip_e;

#define CODELOADER_CHIP_CURRENT_CHIP 1

/**
 * @brief  Codeloader Internal States
 */
typedef enum {
    CODELOADER_STATE_WAITING_FOR_INIT,
    CODELOADER_STATE_WAITING_FOR_COMMAND,
    CODELOADER_STATE_RECEIVING_DATA_TO_FLASH,
    CODELOADER_STATE_EXIT,
    CODELOADER_STATE_UART_SPEED_CHANGING,
    CODELOADER_STATE_ERASE_NV_AND_RESET,
    CODELOADER_STATE_ERASE_SSB_AND_RESET,
} codeloader_state_e;

/**
 * @brief  Codeloader function processor type for specific commands.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer created (if any)
 * @return if > 0 the value must be interpreted as codeloader_command_general_return_code_e.
 * if < 0 a particular error must be interpreted.
 * @note   if output_buffer was created in the command function that will be the message sent.
 * Otherwise a generic ack message with the return
 * value will be sent.
 */
typedef int8_t (*COMMAND_PROCESSOR_EXECUTION_FUNCTION)(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader Chip
 * Any change to this definitions must be documented in NL-002662-SP.
 */
typedef struct {
    codeloader_type_e codeloader_type;  // !< Codeloader info - Codeloader type identifier
    uint32_t rom_code_version;          // !< Codeloader info - what is the ROM version
    codeloader_chip_e chip;             // !< Codeloader info - Codeloader chip identifier
    uint32_t codeloader_version;        // !< Codeloader info - what is the codeoader version
} codeloader_init_msg_info_t;

typedef struct {
    bool fsb_ssb_loaded;               // !< Codeloader fsb info - is an ssb loaded?
    bool fsb_fscl_prevent_codeloader;  // !< Codeloader fsb info - do we block running codeloader?
    uint8_t fsb_dieid_length;            // !< Codeloader fsb info - length of dieid to print
    uint8_t *fsb_dieid;                  // !< Codeloader fsb info - dieid (binary)
} codeloader_fsb_info_t;

/**
 * @brief  Contains the information needed to start up the codeloader
 */
typedef struct {
    bool security_core_allowed;     // !< Codeloader Permissions - is core security unlocked
    bool protocol_core_allowed;     // !< Codeloader Permissions - is core protocol unlocked
    bool apps_core_allowed;         // !< Codeloader Permissions - is core apps unlocked
    codeloader_init_msg_info_t info;  // !< Codeloader info - details to output as part of the init message
    bool use_long_timeout;          // !< Codeloader configuration - use long timeout to wait in the init loop
} codeloader_options_t;

/**
 * @brief  Contains the information needed to start up the codeloader
 */
typedef struct {
    bool running_from_bootloader_ram;  // !< Codeloader Permissions - is security unlocked
    bool security_core_updated;        // !< Codeloader has updated the security core image
} codeloader_retp_t;

/**
 * @brief  Contains the information needed to record in the codeloader log
 */
typedef struct {
    uint8_t command_code;
    int8_t command_ret_value;
} codeloader_log_t;

/**
 * @brief  Initialises the codeloader resources.
 * @note   Assumes the uart driver has not been initialised
 */
void codeloader_init(const codeloader_options_t *options);

/**
 * @brief  Initialises the codeloader resources.
 * @note   It deinitialises the uart driver.
 */
void codeloader_deinit(void);

/**
 * @brief  Main loop to run while the Codeloader is active
 * @param  options Contains the options to init the codeloader with (which cores are allowed + version info)
 * @param  cl_retp Pointer to the location where extra return parameters are stored. If the function returns true
 * cl_retp->running_from_bootloader_ram indicates if the system is running from RAM after the execution of codeloader.
 * @return true if the codeloader received the codeloader reached the loading state (potentially change flash).
 */
bool codeloader_main_loop(const codeloader_options_t *options, codeloader_retp_t *cl_retp);

/**
 * @brief  Returns the current state of the Codeloader
 * @return Codeloader current state.
 */
codeloader_state_e codeloader_main_get_state(void);

/**
 * @brief  Changes the codeloader internal state
 * @param  state state to set it to.
 */
void codeloader_main_set_state(codeloader_state_e state);

/**
 * @brief  Log that Codeloader has updated the security core image
 */
void codeloader_set_security_core_updated(void);

/**
 * @brief  save codeloder error log in kv region
 */
void codeloader_error_log_record(uint8_t cmd_code, int8_t cmd_ret_value);
#endif
