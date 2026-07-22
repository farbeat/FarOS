/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:   IPC mechanism - Definition of supported messages and payload
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef SRC_DRIVERS_IPC_SHARED_IPC_ACTIONS_H
#define SRC_DRIVERS_IPC_SHARED_IPC_ACTIONS_H
#include "core.h"

/**
 * @addtogroup connectivity_drivers_non_os_ipc
 * @{
 */
#define IPC_BT_VERSION_CURRENT 4
#define IPC_BT_VERSION_LAST_SUPPORTED 4
#define MAX_AP_BT_HOOK_DATA_LEN 256   // The max length of hook data command for BT core.
#define CALIBRATION_PARAMS_LENGTH 256 // The max length of calibration parameters.
#define HCI_DATA_LRN 160              // The max length of hci parameters.
#define CHR_EXTEND_PARAM_MAX_LEN 128
#define BT_CUSTOMIZE_NV_RESERVED 86
#define BT_CUSTOMIZE_CHNL_MAP_LEN 10

#include "chip_definitions.h"
#include "platform_core.h"
/**
 * @brief  This enum describes all the supported IPC messages. Each message needs to have an associated payload type
 * defined in the ipc_payload union.
 * Note: Gaps in numbering lead to wasted memory, as also used to iterate over an array of callbacks.
 */
typedef enum {
    IPC_ACTION_CORE_STARTED = 0,                 // !< Sent to the security core if another core has started
    IPC_ACTION_PROTOCOL_REBOOT_APPS = 1,         // !< Protocol Send Reboot Apps To security Implement
    IPC_ACTION_EXCEPTION_TEST = 2,               // !< Sent to security core to test exception handle
    IPC_ACTION_UNUSED_3 = 3,                     // !<
    IPC_ACTION_FLASH_OP_CNF = 4,                 // !<
    IPC_ACTION_FLASH_OP_IND = 5,                 // !<
    IPC_ACTION_SYS_REBOOT_REQ = 6,               // !< Request a complete system reboot.
    IPC_ACTION_LOG_INFO = 7,                     // !< Inform there are logs available.
    IPC_ACTION_RPC_COMMAND = 8,                  // !< Used to send rpc style commands between the cores
    IPC_ACTION_DATA = 9,                         // !< Used to send data
    IPC_ACTION_WAKUP_INFO = 10,                  // !< Apps ipc wakeup interrupt.
    IPC_ACTION_PLL_MICRO_REQ = 11,               // !< DSP ipc request PLL micro adjust interrupt.
    IPC_ACTION_SET_LOG_LEVEL = 12,               // !< Used by App core to set the log level of BT core or DSP core.
    IPC_ACTION_EXCEPTION_IND = 13,               // !< Used by Any core to inform the current exception.
    IPC_ACTION_AP_BT_HOOK_DATA = 14,             // !< Used by App core to hook the data.
    IPC_ACTION_BT_AP_HOOK_DATA = 15,             // !< Used by BT core to send the result of hook data.
    IPC_ACTION_BT_HIFI_MEM_REQ = 16,             // !< Used by BT core to request the memory.
    IPC_ACTION_HIFI_BT_MEM_CNF = 17,             // !< Used by DSP core to response the memory request.
    IPC_ACTION_AP_BT_CALIBRATION_PARAM_REQ = 18, // !< Used by App core to send the calibration params to BT core.
    IPC_ACTION_SET_SYSTEM_CLOCKS = 19,           // !< Used to switch system clocks.
    IPC_ACTION_MASS_DATA_INFORM = 20,            // !< Used to inform there are coming mass data from bt&dsp core.
    IPC_ACTION_SET_LOG_TIME = 21,                // !< Used to set the rtc time of compress log.
    IPC_ACTION_REDUCE_VOLTAGE_AFTER_WAKEUP = 22, // !< Used to reduce voltage from 0.9v to 0.8v after wakeup.
    IPC_ACTION_AUDIO_HOOK_DATA = 23,             // !< Used to hook audio data
    IPC_ACTION_GET_CAL_FREQ_REQ = 24,            // !< Used by App core to get the freq of clock calibration.
    IPC_ACTION_GET_CAL_FREQ_CNF = 25,            // !< Used by BT core to report the freq of clock calibration.
    IPC_ACTION_HCI_INFORM = 26,                  // !< Used to inform there are coming hci data from app core.
    IPC_ACTION_TS_MESSAGE_REQ = 27,              // !< Used by the test suite to send a command.
    IPC_ACTION_TS_MESSAGE_CNF = 28,    // !< Used by the test suite to send the response to a previous command.
    IPC_ACTION_SET_LOG_SWITCH = 29,    // !< Used to config log switch by AT cmd.
    IPC_ACTION_BT_POWER_ON = 30,       // !< Used by BT core to inform App core no xip access
    IPC_ACTION_UPDATA_TRNG = 31,       // !< Used to BT core to inform App core updata trng share memory
    IPC_ACTION_SLAVE_ADJUST_FREQ = 32, // !< Used by Master core to inform slave core updata freq
    IPC_ACTION_TEMP_CHANGE = 33,       // !< Used by BT core to inform App core temperature change
    IPC_ACTION_DSP_CURRENT_FREQ = 34,  // !< Used by Master core to inform dsp core current freq
    IPC_ACTION_BT_POWER_OFF = 35,      // !< Used by Master core to inform bt core power off.
    IPC_ACTION_BT_HIGH_POWER = 36,     // !< Used by BT core to inform app core open or close high power.
    IPC_ACTION_MASS_DATA_INFORM_WITH_EXTEND = 37, // !< Used to inform mass data by slave core in wear.
    IPC_ACTION_BT_SERVICE_ON = 38, // !< Used by BT core to inform app core Calibration end in wear.
    IPC_ACTION_BT_WRITE_NV = 39,                  // !< Used by BT core to inform app core bt write nv in product line.
    IPC_ACTION_BT_READ_NV = 40,                   // !< Used by BT core to inform app core bt read nv before power on.
    IPC_ACTION_APP_LOAD_NV = 41,                  // !< Used by APP core to inform bt core app read nv before power on.
    IPC_ACTION_DSP_POWER_STATE = 42,              // !< Used by DSP core to inform App core POWER state
    IPC_ACTION_BT_READ_NV_CUSTOMIZE = 43,  // !< Used by BT core to inform app core bt needs to load customized nv.
    IPC_ACTION_APP_LOAD_NV_CUSTOMIZE = 44, // !< Used by APP core to inform bt core customized nv exist or not.
    IPC_ACTION_GNSS_START = 50,            // !< [50:] is used by gnss.
    IPC_ACTION_GNSS_LOG_INFO = IPC_ACTION_GNSS_START + 0,
    IPC_ACTION_GNSS_MSG = IPC_ACTION_GNSS_START + 1,
    IPC_ACTION_GNSS_POWER_OFF = IPC_ACTION_GNSS_START + 2,
    IPC_ACTION_GNSS_END = 70,
    IPC_ACTION_BT_ATCMD = 71,
    IPC_ACTION_BT_CALI = 72,
    IPC_ACTION_NUM_ACTIONS,         // !< Used to size arrays.
    IPC_ACTION_FORCE_SIZE = 0x7FFF, // !< Used to force enum to 2 bytes
} ipc_action_t;

/*
 * Placeholder for the various IPC_ACTION_ENCRYPT messages.
 */
typedef struct {
    uint32_t source_ptr;
    uint32_t dest_ptr;
    uint16_t length;
} ipc_payload_memory;

#define IPC_TEST_SUITE_STRING_LENTGH 60

/**
 * @brief  Defines the information passed around by the test suite.
 * USed to send a command (text string from the uart) and to provide
 * response strings back for our output (to the uart).
 */
typedef struct {
    uint16_t status;                        // !< Provides information regarding the latest command. Pass, fail, etc
    uint16_t length;                        // !< Length in bytes of the msg.
    char msg[IPC_TEST_SUITE_STRING_LENTGH]; // !< Command or response string.
} ipc_payload_test_suite_string;

typedef enum {
    IPC_FLASH_CLAIM_SECTION,
    IPC_FLASH_FLUSH_SECTION,
    IPC_FLASH_ERASE_SECTION,
    IPC_FLASH_RELEASE_SECTION,
} ipc_flash_command;

typedef enum {
    IPC_MEMORY_TRANSFER_COMMAND_TRANSFER_BUFFER,
    IPC_MEMORY_TRANSFER_COMMAND_ALLOCATION_REQUEST,
    IPC_MEMORY_TRANSFER_IND_DONE_WITH_BUFFER,
} ipc_memory_transfer_command_e;

/**
 * @brief  The hook types which is used to notify the App core.
 */
typedef enum {
    HOOK_TYPE_REGS,
    HOOK_TYPE_MEMS,
    HOOK_TYPE_BLOCK_MEM,
    HOOK_TYPE_SAMPLE_DATAS,
    HOOK_TYPE_DISABLE_MEM_ACCESS,
    HOOK_TYPE_NORMAL_DATA_TRANS,
} ipc_bt_ap_hook_type_e;

typedef enum {
    EXCEPTION_TEST_COMMAND_BT_WDT_REBOOT,
    EXCEPTION_TEST_COMMAND_BT_HARDFAULT,
    EXCEPTION_TEST_COMMAND_BT_PANIC,
    EXCEPTION_TEST_COMMAND_BT_STD_CHIP_WDT_FRST,
    EXCEPTION_TEST_COMMAND_APP_WTD_REBOOT,
    EXCEPTION_TEST_COMMAND_APP_PANIC,
    EXCEPTION_TEST_COMMAND_APP_HARDFAULT,
    EXCEPTION_TEST_COMMAND_APP_WDT_FRST,
    EXCEPTION_TEST_COMMAND_GNSS_WDT_REBOOT,
    EXCEPTION_TEST_COMMAND_GNSS_HARDFAULT,
    EXCEPTION_TEST_COMMAND_GNSS_PANIC,
    EXCEPTION_TEST_COMMAND_NUM,
} ipc_exception_test_command_e;

typedef enum {
    SEC_TRNG_RAND_BUF_UPDATA_ALL,
    SEC_TRNG_RAND_BUF_UPDATA_UP_SIDE,
    SEC_TRNG_RAND_BUF_UPDATA_DOWN_SIDE,
} ipc_upadta_trng_buffer_e;

/**
 * @brief  Structure to hold ipc test command
 */
typedef struct {
    ipc_exception_test_command_e command;
} ipc_payload_exception_test;

/**
 * @brief  Structure to hold FLASH requests.
 */
typedef struct {
    // Always used.
    ipc_flash_command command; // !< The command sent to the flash.

    uint8_t type; // !< Type of section to act on.
    uint8_t id;   // !< ID of section to act on.

    // Usage depends on 'command'.
    uint32_t length;  // !< Length of the section. Used with IPC_FLASH_CLAIM_SECTION.
    uint32_t address; // !< Address of buffer in local RAM. Used with IPC_FLASH_CLAIM_SECTION.
} ipc_payload_flash_req;

/**
 * @brief  Structure to hold FLASH response.
 */
typedef struct {
    ipc_flash_command command; // !< The command sent to the flash.

    uint8_t type; // !< Type of section to act on.
    uint8_t id;   // !< ID of section to act on.
} ipc_payload_flash_cnf, ipc_payload_flash_ind;

/**
 * @brief  Structure to hold the ipc_reboot cause.
 */
typedef struct {
    uint32_t reboot_cause;
} ipc_payload_core_started;

/**
 * @brief  Reasonable payload size for an RPC message.
 * All fixed sized messages should fit within this size.
 * Large variable sized messages will be fragmented but this tries to provide a reasonable trade-off.
 */
#define IPC_RPC_PAYLOAD_SIZE RPC_PAYLOAD_MAX_SIZE

/* only used for gnss-app ipc */
#if CHIP_ASIC
#define IPC_RPC_PAYLOAD_SIZE_ADAPT 16384 /* 16384 for asic temp config */
#else
#define IPC_RPC_PAYLOAD_SIZE_ADAPT 8192
#endif
/**
 * @brief  Defines a suitable RPC payload buffer, to help prevent excessive IPC message fragmentation.
 */
typedef struct {
    uint8_t data[IPC_RPC_PAYLOAD_SIZE];
} ipc_payload_rpc;

typedef struct {
    uint8_t data[IPC_RPC_PAYLOAD_SIZE_ADAPT];
} ipc_payload_rpc_adapt;

/**
 * @brief  A struct to hold why one core has asked the security core to reboot.
 */
typedef struct {
    uint16_t requested_reboot_reason;
} ipc_payload_request_reboot;

/**
 * @brief  A struct to hold the log level to be set.
 */
typedef struct {
    uint32_t log_level;
} ipc_payload_set_log_level;

/**
 * @brief  A struct to hold the content of hook data command.
 */
typedef struct {
    ipc_bt_ap_hook_type_e hook_type;
    uint8_t command[MAX_AP_BT_HOOK_DATA_LEN];
    uint32_t command_len;
} ipc_payload_ap_bt_hook_data;

/**
 * @brief  A struct to hold the result to deal the hook command
 */
typedef struct {
    ipc_bt_ap_hook_type_e hook_type;
    uint32_t addr;
    uint32_t len;
    uint32_t value;
} ipc_payload_bt_ap_hook_data;

/**
 * @brief  A struct to hold the size of memory which is requested by BT core.
 */
typedef struct {
    uint32_t size;
} ipc_payload_bt_hifi_mem_req;

/**
 * @brief  A struct to hold the result of request memory by BT core.
 */
typedef struct {
    uint32_t result;
} ipc_payload_hifi_bt_mem_cnf;

/**
 * @brief  A struct to hold the calibration params.
 */
typedef struct {
    uint8_t params[CALIBRATION_PARAMS_LENGTH];
    uint32_t length;
} ipc_payload_ap_bt_calibration_param_req;

/**
 * @brief  A struct to hold the hci params.
 */
typedef struct {
    uint8_t data[HCI_DATA_LRN];
    uint16_t length;
} ipc_payload_hci_data_type;

/**
 * @brief  A strcut to hold the mass data type.
 */
#if (BTH_WITH_SMART_WEAR == YES)
#pragma pack(1)
typedef struct {
    uint8_t type; // 0-event, 1-error.
    uint8_t event_id;
    uint8_t info1;
    uint8_t info2;
    uint16_t info3;
} ipc_payload_mass_data_type;
#pragma pack()
#else
typedef struct {
    uint8_t type;           // 0-event, 1-error.
    uint8_t core;           // src core.
    uint8_t event_info;     // chr info3.
    uint8_t chr_up_type;    // chr up format type.
    uint32_t subevent_info; // chr info4.
    uint32_t time_stamp;    // time stamp.
    uint32_t event_id;      // global chr event id.
    uint32_t psn;
    uint8_t role;
} ipc_payload_mass_data_type;
#endif

#pragma pack(1)
typedef struct {
    ipc_payload_mass_data_type mass_data;
    uint8_t data_len;
    uint8_t data[CHR_EXTEND_PARAM_MAX_LEN];
} ipc_payload_mass_data_extend_type;
#pragma pack()

/**
 * @brief  A struct to store the type of the clock type to get by App core.
 */
typedef struct {
    uint32_t clock_type;
} ipc_payload_get_cal_freq_req_t;

/**
 * @brief  A struct to store the result of the clock wants to get.
 */
typedef struct {
    uint32_t result;
} ipc_payload_get_cal_freq_cnf_t;

/**
 * @brief  A struct to store the flag of bt power on or off.
 */
typedef struct {
    bool flag;
} ipc_payload_bt_power_on_t;

/**
 * @brief  A struct to store the flag of dsp power on or off.
 */
typedef struct {
    bool flag;
} ipc_payload_dsp_power_on_t;

/**
 * @brief  A struct to store the flag of temperature.
 */
typedef struct {
    bool flag;
} ipc_payload_temperature_t;

/**
 * @brief  A struct to store the signal of updata trng, 0 opdata all side, 1 updata up side, 2 updata down side
 */
typedef struct {
    uint32_t updata;
} ipc_payload_updata_trng_t;

/**
 * @brief  A struct to store the freq of slave.
 */
typedef struct {
    uint32_t slave_freq;
} ipc_payload_adjust_slave_freq_t;

/**
 * @brief  A struct to store the frequency of dsp.
 */
typedef struct {
    uint32_t freq;
} ipc_payload_dsp_current_freq_t;

typedef struct {
    uint32_t power_switch;
} ipc_payload_high_power_t;

typedef struct {
    uint16_t config_nv_1;
    uint16_t config_nv_2;
    uint16_t config_nv_3;
    uint16_t config_nv_4;
    uint16_t config_nv_5[5];
    uint16_t config_nv_6;
    uint16_t config_nv_7;
    uint16_t config_nv_8;
    uint16_t config_nv_9;
    uint8_t config_nv_10;
    int8_t config_nv_11;
    int8_t config_nv_12;
    uint8_t flag;
} ipc_payload_config_nv_info_t;

typedef struct {
    uint32_t nv_customize_config;
    uint32_t nv_customize_config_reserved;
    uint8_t nv_max_ble_tx_power_cfg_val;
    uint8_t nv_max_gle_tx_power_cfg_val;
    uint8_t fcc_flag;
    uint8_t customize_four_k_support;
    uint8_t customize_freq_map[BT_CUSTOMIZE_CHNL_MAP_LEN];
    uint8_t reserved[BT_CUSTOMIZE_NV_RESERVED];
} ipc_payload_bt_nv_customize_info_t;

/**
 * @brief  This union defines the IPC message payload, i.e. encapsulates the actual data transported by the IPC message.
 */
typedef union {
    ipc_payload_memory memory; // !< Used to share memory between cores.

    // 'Remote' flash accesses.
    ipc_payload_flash_req flash_req; // !< Used to specify which flash operation to perform.
    ipc_payload_flash_cnf flash_cnf; // !< Used to pass a response to a previous request.
    ipc_payload_flash_ind flash_ind; // !< Used to pass 'asynchronous' data to a 'remote' client.

    ipc_payload_core_started core_started; // !< Used to signal to the security core that a core has rebooted

    ipc_payload_rpc rpc_payload; // !< Not actually used.  Just to ensure ipc_payload union is a reasonable size.

    // !< Used to request a reboot from the security core, the reason is then passed to the cores on startup
    ipc_payload_request_reboot request_reboot;

    ipc_payload_set_log_level set_log_level; // !< Used by App core to set the log level of BT core or DSP core.

    ipc_payload_ap_bt_hook_data ap_bt_hook_data; // !< Used by App core to hook the data.
    ipc_payload_bt_ap_hook_data bt_ap_hook_data; // !< Used by BT core to send the result of hook data.

    ipc_payload_bt_hifi_mem_req bt_hifi_mem_req; // !< Used by BT core to request the memory.
    ipc_payload_hifi_bt_mem_cnf hifi_bt_mem_cnf; // !< Used by DSP core to response the memory request.
    // !< Used by App core to send the calibration params to BT core.
    ipc_payload_ap_bt_calibration_param_req ap_bt_calibration_param_req;
    ipc_payload_exception_test exception_test_cmd; // !< Used  to send a exception test command.

    ipc_payload_mass_data_type mass_data_type; // !< Used by Bt&DSP core to inform A core the type of mass data.
    // !< Used by Bt&DSP&GNSS core to inform A core the type of mass data with extend parameter.
    ipc_payload_mass_data_extend_type mass_data_extend_type;
    ipc_payload_hci_data_type hci_data_type; // !< Used by App core to inform Bt core the type of hci data.

    ipc_payload_get_cal_freq_req_t get_cal_freq_req; // !< Used by App to get the calibration result of clock.
    ipc_payload_get_cal_freq_cnf_t get_cal_freq_cnf; // !< Used by BT to report the calibration result of clock.

    ipc_payload_test_suite_string testsuite;      // !< Used by the test-suite to send/receive command/response strings.
    ipc_payload_bt_power_on_t bt_power_on_flag;   // !< Used by BT core to inform App core bt power on.
    ipc_payload_dsp_power_on_t dsp_power_on_flag; // !< Used by dsp core to inform App core dsp power on.
    ipc_payload_temperature_t temperature_change_flag; // !< Used by BT core to inform App core temperature_change.
    ipc_payload_updata_trng_t updata_trng_signal; // !< Used by BT core to inform App core updata trng share memory.
    ipc_payload_adjust_slave_freq_t adjust_slave_freq; // !< Used by Master core to inform slave core adjust freq.
    ipc_payload_dsp_current_freq_t dsp_freq;           // !< Used by Master core to inform dsp core current freq.
    ipc_payload_high_power_t bt_high_power;
    ipc_payload_config_nv_info_t bt_nv_info;
    ipc_payload_bt_nv_customize_info_t bt_nv_customize_info;
} ipc_payload;

/* only used for gnss-app ipc */
typedef union {
    ipc_payload_rpc_adapt rpc_payload; // !< Not actually used.  Just to ensure ipc_payload union is a reasonable size.
} ipc_payload_adapt;

/**
 * @}
 */
#endif