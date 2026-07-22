/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of qspi bus.
 * Author: @CompanyNameTag
 * Create: 2022-08-05
 */

#ifndef __LCD_QSPI_H__
#define __LCD_QSPI_H__

#include "soc_mipi_tx.h"
#include "soc_lcd_api.h"
#include "hal_gpio.h"
#include "lcd_bus.h"
#include "platform_core.h"

/**
 * @brief Initialize qspi display bus
 *
 * @param  [in]  bus_cfg config of qspi bus. see @ref combo_dev_cfg_t
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_init(combo_dev_cfg_t *bus_cfg);

/**
 * @brief Deinitialize qspi display bus
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_deinit(void);

/**
 * @brief Enable qspi display display
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_enable(void);

/**
 * @brief Disable qspi display display
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_disable(void);

/**
 * @brief Write command to screen by qspi display bus
 *
 * @param  [in]  cmd command to write. see @ref lcd_cmd_sequ
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_write_cmd(lcd_cmd_sequ *cmd);

/**
 * @brief Read register from screen by qspi display bus
 *
 * @param  [in]  cmd register addr to read.
 * @param  [out] buff data read from screen register.
 * @param  [in]  buff_len data len to read.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_read_cmd(uint32_t cmd, uint8_t *buff, uint8_t len);

/**
 * @brief Tranfer frame by dma and qspi
 *
 * @param  [in]  addr address saving frame to send.
 * @param  [in]  callback pointer to callback, called when frame fransfer complete.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_send_frame(uint32_t addr, qspi_send_frame_cb_t callback);

/**
 * @brief Register TE ISR callback
 *
 * @param  [in]  callback pointer to callback, called when TE trigger.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_register_te_isr(gpio_callback_t callback);

/**
 * @brief qspi suspend
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_suspend(void);

/**
 * @brief qspi resume
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno lcd_qspi_resume(void);
#endif
