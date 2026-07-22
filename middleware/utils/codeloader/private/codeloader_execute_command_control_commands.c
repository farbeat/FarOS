/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: codeloader control commands
 * Author:
 * Create:
 */

#include <stdbool.h>
#include "codeloader_command_processor_utils.h"
#include "codeloader.h"       // received_main ..
#include "codeloader_uart.h"  // resend last message
#include "systick.h"
#include "non_os_reboot.h"
#include "securec.h"
#include "codeloader_locking.h"
#include "build_version.h"
#include "codeloader_message_layer.h"
#include "codeloader_version.h"
#include "bootloader_utils.h"
#include "error_code.h"
#include "non_os.h"
#include "codeloader_execute_command_control_commands.h"

#define MIN_UART_SPEED           9600
#define MAX_UART_SPEED           10000000
#define MIN_UART_CHANGE_DELAY_MS 10

static volatile bool g_control_command_uart_message_recieved;
static uint32_t g_control_command_uart_speed;
//lint -esym(843, g_codeloader_command_processor_running_from_bl_ram)
static bool g_codeloader_command_processor_running_from_bl_ram = false;

//lint -esym(754, codeloader_init_out_t::reserved)
typedef struct {
    bool security_core_allowed;
    bool protocol_core_allowed;
    bool apps_core_allowed;
    uint32_t production_lock;  // codeloader production lock bitmap
    uint32_t reserved;         // in case codeloader production lock bitmap exceeds 32 bits
} __attribute__((packed)) codeloader_init_out_t;

//lint -esym(754, codeloader_nack_in_t::error_code)
typedef struct {
    uint8_t error_code;
} __attribute__((packed)) codeloader_nack_in_t;

/**
 * Reset the chip after a delay
 */
typedef struct {
    uint32_t reset_delay_ms;
} __attribute__((packed)) codeloader_reset_in_t;

typedef struct {
    codeloader_type_e codeloader_type;
    uint32_t firmware_code_version;
    codeloader_chip_e chip;
    uint32_t codeloader_version;
    uint32_t epoch;
    uint8_t codeloader_intmsg_length;
    uint8_t version_length;
    uint8_t git_length;
    // used as pointer to the buffer to write,
    // the actual size is codeloader_intmsg_length + version_length + git_length
    uint8_t variable_data[1];
} __attribute__((packed)) cl_cmd_r_ver_info_out_para_t; //lint !e959

/**
 * Set UART speed
 */
typedef struct {
    uint32_t new_uart_speed;
    uint32_t delay_until_reset_ms;
} __attribute__((packed)) codeloader_set_uart_speed_in_t;

void control_command_uart_message_recieved_set(bool val)
{
    non_os_enter_critical();
    g_control_command_uart_message_recieved = val;
    non_os_exit_critical();
}

//lint -esym(459, codeloader_command_processor_get_permissions)
/* Get permissions command function */
int8_t codeloader_command_processor_get_permissions(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_init_out_t *output_params = NULL;
    uint32_t production_lock = codeloader_lock_read();

#ifdef SUPPORT_ONE_LINE_UART_BURN
    if (codeloader_uart_get_rx_status()) {
        codeloader_uart_set_global_uart(CODELOADER_ONE_LINE_UART_BUS);
        uapi_uart_unregister_rx_callback(CODELOADER_UART_BUS);
        uapi_uart_deinit(CODELOADER_UART_BUS);
    } else {
        uapi_uart_unregister_rx_callback(CODELOADER_ONE_LINE_UART_BUS);
        uapi_uart_deinit(CODELOADER_ONE_LINE_UART_BUS);
    }
#endif
    if (input_buffer->length == 0) {
        output_params = (codeloader_init_out_t *)
            cl_cmd_proc_utils_cons_gen_out_msg(output_buffer, CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS,
                sizeof(codeloader_init_out_t)); //lint !e826
        if (output_params != NULL) {
            output_params->security_core_allowed = codeloader_check_permissions(CORES_BT_CORE);
            output_params->protocol_core_allowed = codeloader_check_permissions(CORES_PROTOCOL_CORE);
            output_params->apps_core_allowed = codeloader_check_permissions(CORES_APPS_CORE);
            output_params->production_lock = bswap(production_lock);
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_BUFFER_NOT_AVAILABLE;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    if (return_code != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
        error_code_set(ERROR_CODE_CODELOADER_INIT_ERROR);
    }
    return return_code;
}

void codeloader_command_processor_set_uart_speed_set(void)
{
    uint32_t new_speed = g_control_command_uart_speed;
    g_control_command_uart_message_recieved = false;
    codeloader_uart_change_baud(new_speed);
    codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
    codeloader_uart_enable_reception();
    UNUSED(g_control_command_uart_message_recieved);
}

//lint -esym(459, codeloader_command_processor_set_uart_speed)
/* Set the UART speed */
int8_t codeloader_command_processor_set_uart_speed(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    codeloader_set_uart_speed_in_t *in_parameters = NULL;

    if (input_buffer->length == sizeof(codeloader_set_uart_speed_in_t)) {
        in_parameters = (codeloader_set_uart_speed_in_t *)input_buffer->buffer;  //lint !e826

        in_parameters->delay_until_reset_ms = bswap(in_parameters->delay_until_reset_ms);
        in_parameters->new_uart_speed = bswap(in_parameters->new_uart_speed);

        if (in_parameters->new_uart_speed >= MIN_UART_SPEED && in_parameters->new_uart_speed <= MAX_UART_SPEED) {
            if (in_parameters->delay_until_reset_ms > MIN_UART_CHANGE_DELAY_MS) {
                /* The actual speed change is called by the TX callback in the codeloader.c file */
                codeloader_main_set_state(CODELOADER_STATE_UART_SPEED_CHANGING);
                g_control_command_uart_speed = in_parameters->new_uart_speed;

                return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            }
        }
    }
    return return_code;
}

/* Exit command function */
int8_t codeloader_command_processor_exit(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (input_buffer->length != 0) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    if (return_code != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
        error_code_set(ERROR_CODE_CODELOADER_EXIT_ERROR);
    }
    return return_code;
}

/* nack command function */
int8_t codeloader_command_processor_nack(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (input_buffer->length != sizeof(codeloader_nack_in_t)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}

int8_t codeloader_command_processor_reset(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    // Since resetting should be safe, and we are already in the codeloader mode any core should
    // be allowed to do it, so no need to check for permissions
    codeloader_reset_in_t *input_parameters = NULL;
    uint64_t end_time;

    if (input_buffer->length == sizeof(codeloader_reset_in_t)) {
        input_parameters = (codeloader_reset_in_t *)input_buffer->buffer;  //lint !e826
        input_parameters->reset_delay_ms = bswap(input_parameters->reset_delay_ms);
        // systick_get_ms() returns a 64-bit value, which has approximatley 43-bit resolution
        // This has a maximum value before wrapping of approximately 272 years 72 days
        // The value of reset_delay_ms read from the command line is a 32-bit unsigned value
        // As a 32-bit unsigned value, at maximum
        //  * will cause the security core to spin for around 47 days
        //  * will have no impact on the while loop wrapping
        //  * allows the system to be putinto a know stable state for production testing
        if (input_parameters->reset_delay_ms > 0) {
            end_time = uapi_systick_get_ms();
            while ((uapi_systick_get_ms() - end_time) < input_parameters->reset_delay_ms) {}  // Spin
        }
        reboot_chip();
    }

    // At least, I hope this is why we got here...
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
}

/**
 * return pointers and length for version info, git info and epoch
 * @param version_length pointer to return version data length or 0 if no data
 * @param version_data pointer to return version data or NULL if no data
 * @param version_length pointer to return git data length or 0 if no data
 * @param version_data pointer to return git data or NULL if no data
 * @param epoch pointer to return 32-bit epoch or 0 if no data
 */
// should make general, but not having SSB as a core makes it less general
// can probably have it common, taking the CORE parameter and for the SSB build
// ignore the CORE value. The use this in all cases for reading the version info.
static void read_version_info(
    uint8_t *version_length, const char **version_data, uint8_t *git_length, const char **git_data, uint32_t *epoch)
{
#if defined(BUILD_APPLICATION_ROM)
    uint8_t *version_offset_per_core = (uint8_t *)(uintptr_t)(ROM_START + VERSION_INFORMATION_OFFSET);
#else
    uint8_t *version_offset_per_core = (uint8_t *)(uintptr_t)SSB_VERSION_LOAD_ORIGIN;
#endif

    *version_length = 0;
    *version_data = NULL;
    *git_length = 0;
    *git_data = NULL;
    *epoch = 0;

#if defined(BUILD_APPLICATION_SSB)
    // use the physical address from start of flash
    build_version_info *version_addr = (build_version_info *)(version_offset_per_core);      //lint !e826 !e838
    if ((version_addr != NULL) && (version_addr->build_version_info_magic == BUILD_VERSION_MAGIC_NUMBER)
        && (version_addr->build_version_info_version == BUILD_VERSION_INFO_VERSION)) { //lint !e774
        *epoch = version_addr->epoch;
        *version_data = version_addr->string;
        *version_length = FIRMWARE_VERSION_STRING_MAX_LENGTH;
        *git_data = version_addr->hash;
        *git_length = FIRMWARE_GIT_HASH_LENGTH;
    }
#elif defined(BUILD_APPLICATION_ROM)
    // use the physical address from start of rom
    build_version_info_rom *version_addr = (build_version_info_rom *)(version_offset_per_core);
    *epoch = version_addr->epoch;
    *version_data = version_addr->string;
    *version_length = FIRMWARE_VERSION_STRING_MAX_LENGTH;
#else
    UNUSED(version_offset_per_core);
    // do nothing if not SSB or SC UPDATER
#endif
}

int8_t codeloader_command_processor_read_version_info(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(input_buffer);
    uint8_t version_length, git_length;
    const char *version_data = NULL;
    const char *git_data = NULL;
    uint32_t epoch;

    read_version_info(&version_length, &version_data, &git_length, &git_data, &epoch);
    uint8_t intmsg_length = sizeof(CODELOADER_INIT_MESSAGE);
    cl_cmd_r_ver_info_out_para_t *output_params = (cl_cmd_r_ver_info_out_para_t *)
        cl_cmd_proc_utils_cons_gen_out_msg(output_buffer, CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS,
            version_length + git_length + intmsg_length + sizeof(cl_cmd_r_ver_info_out_para_t) - 1);  //lint !e826
    if (output_params != NULL) {
#if defined(BUILD_APPLICATION_ROM)
        output_params->codeloader_type = CODELOADER_TYPE_FSB;
        output_params->firmware_code_version = bootloader_utils_get_rom_version();
#elif defined(BUILD_APPLICATION_SSB)
        output_params->codeloader_type = CODELOADER_TYPE_SSB;
        output_params->firmware_code_version = bootloader_utils_get_ssb_version();
#endif
        output_params->chip = CODELOADER_CHIP_CURRENT_CHIP;
        output_params->codeloader_version = CODELOADER_VERSION;
        output_params->epoch = epoch;
        output_params->codeloader_intmsg_length = intmsg_length;
        output_params->version_length = version_length;
        output_params->git_length = git_length;

        if (memcpy_s(output_params->variable_data, output_params->version_length +
            output_params->codeloader_intmsg_length + output_params->git_length,
            CODELOADER_INIT_MESSAGE, intmsg_length) != EOK) {  //lint !e419 !e416
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
        }
        if ((version_data != 0) && (memcpy_s(output_params->variable_data + intmsg_length,
            output_params->version_length + output_params->git_length,
            version_data, version_length) != EOK)) {  //lint !e419 !e416
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
        }
        if ((git_length != 0) && (memcpy_s(output_params->variable_data + intmsg_length + version_length,
            output_params->git_length, git_data, git_length) != EOK)) {  //lint !e419 !e416
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
        }
    } else {
        // return the output buffer to null so the caller function creates the error output message
        output_buffer->buffer = NULL;
        output_buffer->length = 0;
        error_code_set(ERROR_CODE_CODELOADER_READ_VERSION_ERROR);
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_BUFFER_NOT_AVAILABLE;
    }

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}

bool codeloader_command_processor_is_running_from_bootloader_ram(void)
{
    return g_codeloader_command_processor_running_from_bl_ram;
}
