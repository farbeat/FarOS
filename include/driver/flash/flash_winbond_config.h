/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides flash WINBOND Configuration information. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-11-15， Create file. \n
 */
#ifndef FLASH_WINBOND_CONFIG_H
#define FLASH_WINBOND_CONFIG_H

/**
 * @defgroup drivers_driver_flash_winbond_config Flash Winbond Config
 * @ingroup  drivers_driver_flash
 * @{
 */

#include "flash_common_config.h"

/* Winbond enter qspi mode cmd config. */
/* W25Q32/W25Q64/W25Q128/W25Q128JW winbond flash config. */
static flash_cmd_exe_t g_winbond_w25qxx_enter_qspi_mode_cmd[] = {
    /* Write enable for Volatils status register. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0x50 } },
    /* Write status register-2 to 0x2, enable QPI write bit. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 2, { 0x31, 0x02 } },
    /* Waite config done. */
    { FLASH_CMD_TYPE_PROCESSING, HAL_SPI_FRAME_FORMAT_STANDARD, 3, { FLASH_RDSR1_CMD, 0, 0 } },
    /* Enable QPI. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0x38 } },
    /* Set dummy clocks 6, maxinum read freq 104MHZ. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_QUAD, 2, { 0xC0,  0x20} },
    /* End cmd, will not execute and need return. */
    { FLASH_CMD_TYPE_END,        HAL_SPI_FRAME_FORMAT_MAX_NUM,   0, { 0 } }
};

/* W25Q256 winbond flash config. */
static flash_cmd_exe_t g_winbond_w25q256_enter_qspi_mode_cmd[] = {
    /* Write enable for Volatils status register. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0x50 } },
    /* Write status register-2 to 0x2, enable QPI write bit. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 2, { 0x31, 0x02 } },
    /* Waite config done. */
    { FLASH_CMD_TYPE_PROCESSING, HAL_SPI_FRAME_FORMAT_STANDARD, 3, { FLASH_RDSR1_CMD, 0, 0 } },
    /* Write enable for Volatils status register. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0x50 } },
    /* ENTER 4-BYTE ADDRESS MODE. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0xB7 } },
    /* Waite config done. */
    { FLASH_CMD_TYPE_PROCESSING, HAL_SPI_FRAME_FORMAT_STANDARD, 3, { FLASH_RDSR1_CMD, 0, 0 } },
    /* Enable QPI. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_STANDARD, 1, { 0x38 } },
    /* Set dummy 4, maxinum read freq 80MHZ. */
    { FLASH_CMD_TYPE_CMD,        HAL_SPI_FRAME_FORMAT_QUAD, 2, { 0xC0,  0x10} },
    /* End cmd, will not execute and need return. */
    { FLASH_CMD_TYPE_END,        HAL_SPI_FRAME_FORMAT_MAX_NUM,   0, { 0 } }
};


/* Winbond enter xip mode config. */
/* W25Q32/W25Q64/W25Q128 winbond flash config. */
static flash_qspi_xip_config_t g_winbond_w25qxx_config = {
    /* Enter XIP mode before config. */
    WAIT_CYCLES_4,
    HAL_SPI_INST_LEN_8,
    HAL_SPI_ADDR_LEN_32,
    HAL_SPI_TRANS_TYPE_INST_Q_ADDR_Q,
    /* Enter XIP mode after config. */
    WAIT_CYCLES_4,
    HAL_SPI_INST_LEN_0,
    HAL_SPI_ADDR_LEN_32,
    HAL_SPI_TRANS_TYPE_INST_Q_ADDR_Q,
    /* XIP 32bit addr & wrap config. */
    false,
    false,
};

/* W25Q256 winbond flash config. */
static flash_qspi_xip_config_t g_winbond_w25q256_config = {
    /* Enter XIP mode before config. */
    WAIT_CYCLES_2,
    HAL_SPI_INST_LEN_0,
    HAL_SPI_ADDR_LEN_32,
    HAL_SPI_TRANS_TYPE_INST_Q_ADDR_Q,
    /* Enter XIP mode after config. */
    WAIT_CYCLES_2,
    HAL_SPI_INST_LEN_8,
    HAL_SPI_ADDR_LEN_32,
    HAL_SPI_TRANS_TYPE_INST_Q_ADDR_Q,
    /* XIP 32bit addr & wrap config. */
    true,
    false,
};

/**
  * @}
  */
#endif