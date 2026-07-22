/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: BRANDY OTP HAL interface.
 * Author: @CompanyNameTag
 * Create: 2022-06-28
 */

#ifndef HAL_OTP_H
#define HAL_OTP_H

#include "stdint.h"
#include "stdbool.h"

/** @defgroup DRIVER_HAL_OTP HAL OTP Driver
  * @ingroup DRIVER_HAL
  * @{
  */

#define OTP_REGION_NUM       3
#define OTP_REGION_MAX_BITS  1024
#define OTP_REGION_MAX_BYTES (OTP_REGION_MAX_BITS >> 3)  // MAX_BIT / 8
#define OTP_MAX_BITS         (OTP_REGION_MAX_BITS * OTP_REGION_NUM)
#define OTP_MAX_BYTES        (OTP_MAX_BITS >> 3)  // MAX_BIT / 8
#define OTP_MAX_BIT_POS      8U

#define HAL_OTP_ENABLE_VDD18           0x58004240
#define HAL_OTP_ENABLE_VDD18_VALID_BIT 1
#define HAL_OTP_POWER_ON_SWITCH     0x57004258
#define HAL_OTP_POWER_ON_VAL        0xea69
#define HAL_OTP_POWER_OFF_VAL       0x0

#define FUSE_CTL_RB_BASE            0x57028000
#define FUSE_CTL_MCU_RB_BASE        0x52082000

#define HAL_OTP0_CTRL              (FUSE_CTL_RB_BASE + 0xD00)
#define HAL_OTP1_CTRL              (FUSE_CTL_MCU_RB_BASE + 0xD10)
#define HAL_OTP2_CTRL              (FUSE_CTL_MCU_RB_BASE + 0xD20)
#define HAL_OTP0_WRITE_BASE_ADDR   (FUSE_CTL_RB_BASE + 0x700)
#define HAL_OTP1_WRITE_BASE_ADDR   (FUSE_CTL_MCU_RB_BASE + 0x700)
#define HAL_OTP2_WRITE_BASE_ADDR   (FUSE_CTL_MCU_RB_BASE + 0x700)
#define HAL_OTP0_READ_BASE_ADDR    (FUSE_CTL_RB_BASE + 0x800)
#define HAL_OTP1_READ_BASE_ADDR    (FUSE_CTL_MCU_RB_BASE + 0xA00)
#define HAL_OTP2_READ_BASE_ADDR    (FUSE_CTL_MCU_RB_BASE + 0xC00)

#define HAL_OTP_WRITE_MODE 0xa5a5
#define HAL_OTP_READ_MODE  0x5a5a

typedef enum {
    HAL_OTP_REGION_0,
    HAL_OTP_REGION_1,
    HAL_OTP_REGION_2,
    HAL_OTP_REGION_MAX,
} hal_otp_region_e;

/**
 * @brief  Enables power to the otp system, blocks until the otp is ready to be used
 */
void hal_otp_init(void);

/**
 * @brief  Removes power from the otp system
 */
void hal_otp_deinit(void);

/**
 * @brief  Flush select otp region, write the value in register to efuse.
 * @param  region Select OTP region
 * @return true on OK, false otherwise.
 */
bool hal_otp_flush_write(hal_otp_region_e region);

/**
 * @brief  Refresh read region to read mode after write.
 * @param  region Select OTP region
 * @return true on OK, false otherwise.
 */
bool hal_otp_refresh_read(hal_otp_region_e region);

/**
 * @brief  Reads the byte at the given OTP memory location
 * @param  byte_address The OTP byte address of the byte to read
 * @param  value The pointer of read value to store
 * @return true on OK, or false on error
 */
bool hal_otp_read_byte(uint32_t byte_address, uint8_t *value);

/**
 * @brief  Writes the value to the given OTP memory location
 * @param  byte_address The OTP byte address of the byte to write
 * @param  value The OTP byte value of the byte to write
 * @return true on OK, or false on error
 */
bool hal_otp_write_byte(uint32_t byte_address, uint8_t value);

/**
 * @brief  Clear all write register to protect next efuse write.
 * @param  region  The region of OTP to write
 */
void hal_otp_clear_all_write_regs(hal_otp_region_e region);

/**
 * @brief  Get the region of a otp byte address
 * @param  byte_addr the addr of the byte to get register
 * @return region The region of otp
 */
hal_otp_region_e hal_otp_get_region(uint32_t byte_addr);

/**
 * @brief  Writes the value to the given OTP memory location
 * @param  address The byte address of OTP to write to
 * @param  value  The value to write to OTP memory
 * @param  region  The region of OTP to write
 * @return The OTP write operation result, true mean SUCCESS, false mean FAILED
 */
bool hal_otp_write_operation(uint32_t address, uint8_t value, hal_otp_region_e region);

/**
 * @brief  Writes the buffer to the given OTP memory location
 * @param  address The byte address of OTP to write to
 * @param  buffer  The buffer to write to OTP memory
 * @param  length  The length of buffer to write to OTP memory
 * @return The OTP write operation result, true mean SUCCESS, false mean FAILED
 */
bool hal_otp_write_buffer_operation(uint32_t address, const uint8_t *buffer, uint16_t length);

/**
  * @}
  */

#endif
