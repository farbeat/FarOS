/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  Codeloader Commands Header File
 * Author:
 * Create:
 */

#ifndef CODELOADER_COMMAND_CODES_H
#define CODELOADER_COMMAND_CODES_H

/**
 * @brief  If a new command is added the CODELOADER_VERSION must be incremented.
 * Codes for the different codeloader commands, INIT EXIT and NACK are fixed numbers for compatibility
 * This is the list of command codes implemented in the codeloader.
 * Any change to this file must be documented in NL-002662-SP.
 * Codeloader command codes must not be reused.
 */
typedef enum {
#ifdef BUILD_APPLICATION_ROM
    CODELOADER_COMMAND_LAYER_COMMAND_IN_INIT = 1, // !< Gets and returns the current permission level
    CODELOADER_COMMAND_LAYER_COMMAND_IN_NACK = 9, // !< A NACK of the last command we sent
    CODELOADER_COMMAND_LAYER_COMMAND_IN_EXIT = 10, // !< Exits the codeloader and continues the boot of the chip
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_SPEED = 18, // !< Sets the UART baud rate, resets after the given delay
    CODELOADER_COMMAND_LAYER_FSB_ADD_KEYSTORE = 19, // !< Add a root pubkey
    CODELOADER_COMMAND_LAYER_FSB_ADD_CERTIFICATE = 20, // !< Add a subkey sign certificate
    CODELOADER_COMMAND_LAYER_FSB_ADD_SIGNATURE = 21, // !< Add a ssb signature
    CODELOADER_COMMAND_LAYER_COMMAND_READ_VERSION_INFO = 37, // !< Read version info from current image
    CODELOADER_COMMAND_LAYER_COMMAND_SET_PSRAM_MODE = 46, // !< Sets the ps ram mode bit in OTP used in ROM
    CODELOADER_COMMAND_LAYER_FSB_WRITE_PAGE = 252, // !< Write a flash page
    CODELOADER_COMMAND_LAYER_FSB_RECEIVE_SSB_SHA_DATA = 253, // !< Receive the SHA for the SSB loaded
    CODELOADER_COMMAND_LAYER_FSB_CHECK_RECEIVED_SHA = 254, // !< Check the SSB SHA sent matches with the loaded one
    CODELOADER_COMMAND_LAYER_FSB_WRITE_RECEIVED_SHA = 255, // !< Write the sent SHA to OTP.
#else
    CODELOADER_COMMAND_LAYER_COMMAND_INVILIAD = 0,
    CODELOADER_COMMAND_LAYER_COMMAND_IN_INIT = 1, // !< Gets and returns the current permission level
    // !< Gets the chip to check that the checksum matches the checksum given in this message
    CODELOADER_COMMAND_LAYER_COMMAND_IN_VALIDATE_CHECKSUM = 2,
    CODELOADER_COMMAND_LAYER_COMMAND_IN_START_FLASHING_PAGE = 3, // !< Sets the page we will flash to
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SEND_DATA = 4, // !< A message containing data to be flashed to the current page
    // !< Signals that we have finished sending data to the current page
    CODELOADER_COMMAND_LAYER_COMMAND_IN_END_FLASHING_PAGE = 5,
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_MEM_CONFIG = 6, // !< Sets the flash and ram limits for the given core
    CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_CORE = 7, // !< Erases the code for the given core
    CODELOADER_COMMAND_LAYER_COMMAND_IN_EXIT = 8, // !< Exits the codeloader and continues the boot of the chip
    CODELOADER_COMMAND_LAYER_COMMAND_IN_NACK = 9, // !< A NACK of the last command we sent
    // !< Sets the permanent OTP codeloader lock for the given core
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_PERMANENT_LOCK = 10,
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_TEMPORAL_LOCK = 11, // !< Sets the flash codeloader lock for the given core
    CODELOADER_COMMAND_LAYER_COMMAND_IN_RESET = 12, // !< Resets the chip
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_SPEED = 13, // !< Sets the UART baud rate, resets after the given delay
    CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_NV_CONFIG = 14, // !< Erases the NV data, needs security permissions
    CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_SSB = 15, // !< Erases whole ssb flash
    // !< Erases all flash that can be erased (keeps the SSB), needs Security permission
    CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_CHIP = 16,
    // !< Sets the flash limit of each core including updater cores, needs security core permissions
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_FLASH_LIMITS = 17,
    CODELOADER_COMMAND_LAYER_COMMAND_GET_CORE_KEYS = 18, // !< Gets a list of all the keys for the given core
    CODELOADER_COMMAND_LAYER_COMMAND_IN_ADD_SIGNATURE = 21, // !< Add a signature
    CODELOADER_COMMAND_LAYER_COMMAND_KEYSTORE_SHA_DATA = 22, // !< Get keystore SHA to be loaded into OTP
    // !< Check downloaded keystore SHA matches calculated SHA
    CODELOADER_COMMAND_LAYER_COMMAND_KEYSTORE_CHECK_RECEIVED_SHA = 23,
    CODELOADER_COMMAND_LAYER_COMMAND_KEYSTORE_WRITE_RECEIVED_SHA = 24, // !< Write keystore SHA to OTP
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_CMG_GROUP_ID = 25, // !< Write CMG group ID to OTP
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_CMG_LIFECYCLE = 26, // !< Write CMG Lifecycle to OTP
    CODELOADER_COMMAND_LAYER_COMMAND_READ_KEYSTORE_SHA = 27, // !< Read Keystore SHA from OTP
    CODELOADER_COMMAND_LAYER_COMMAND_READ_VERSION_INFO = 28, // !< Read version info from current image
    CODELOADER_COMMAND_LAYER_COMMAND_SET_AUTH_RULE = 29, // !< Set authentication rule
    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_KV_KEY = 30, // !< Sets a KV key for a given core
    CODELOADER_COMMAND_LAYER_COMMAND_READ_KEY = 31, // !< Gets the data for the given key
    CODELOADER_COMMAND_LAYER_COMMAND_READ_DATA = 33, // !< Gets the data from assigned addr
    CODELOADER_COMMAND_LAYER_COMMAND_SET_PSRAM_MODE = 46, // !< Sets the ps ram mode bit in OTP used in ROM
    // !< download fota package(without restruct, just for BBIT)
    CODELOADER_COMMAND_LAYER_COMMAND_IN_START_DOWNLOAD_PACKAGE = 47,

    // !< Some commands for debug only.
    CODELOADER_COMMAND_LAYER_COMMAND_DBG_SET_UPGRADE_FLAG = 48,
    CODELOADER_COMMAND_LAYER_COMMAND_DBG_DUMP_ERASE_FLASH_DATA = 49,
    CODELOADER_COMMAND_LAYER_COMMAND_DBG_SHOW_DISK_INFO = 50,

    // !< Download some data in data-flash region.
    CODELOADER_COMMAND_LAYER_COMMAND_SET_DATA_FLASH_REGION = 51,
    CODELOADER_COMMAND_LAYER_COMMAND_SET_DATA_FLASH_DATA = 52,
    CODELOADER_COMMAND_LAYER_COMMAND_CLEAR_DATA_FLASH_DATA = 53,
    CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_HEAD_INFO = 54,
    CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_SHA_DATA = 55,

    CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_SSB_OTP_BIT = 250, // !< Sets the loaded bit for the SSB in OTP
    CODELOADER_COMMAND_LAYER_FSB_ERASE_ALL_FLASH = 251, // !< Erase all flash in chip
    CODELOADER_COMMAND_LAYER_FSB_WRITE_PAGE = 252, // !< Write a flash page
    CODELOADER_COMMAND_LAYER_FSB_RECEIVE_SSB_SHA_DATA = 253, // !< Receive the SHA for the SSB loaded
    CODELOADER_COMMAND_LAYER_FSB_CHECK_RECEIVED_SHA = 254, // !< Check the SSB SHA sent matches with the loaded one
    CODELOADER_COMMAND_LAYER_FSB_WRITE_RECEIVED_SHA = 255, // !< Write the sent SHA to OTP.
#endif
} codeloader_command_layer_command_in_e;
#endif