/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of display bus.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __LCD_MIPI_H__
#define __LCD_MIPI_H__

#include "soc_mipi_tx.h"
#include "soc_lcd_api.h"
#include "lcd_bus.h"

#define BITS_PER_BYTE            8
#define bus_short_packet(p1, p0) ((((p1) & 0xff) << BITS_PER_BYTE) | ((p0) & 0xff))

/**
 * @brief Initialize mipi tx bus
 *
 * @param  [in]  bus_cfg config of mipi bus. see @ref combo_dev_cfg_t
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_init(combo_dev_cfg_t *bus_cfg);

/**
 * @brief Deinitialize mipi tx bus
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_deinit(void);

/**
 * @brief Enable mipi tx display
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_enable(void);

/**
 * @brief Disable mipi tx display
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_disable(void);

/**
 * @brief Write command to screen by mipi tx bus
 *
 * @param  [in]  cmd command to write. see @ref lcd_cmd_sequ
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_write(lcd_cmd_sequ *cmd);

/**
 * @brief Read register from screen by mipi tx bus
 *
 * @param  [in]  cmd register addr to read.
 * @param  [out] buff data read from screen register.
 * @param  [in]  buff_len data len to read.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_read(uint32_t cmd, uint8_t *buff, uint8_t buff_len);

/**
 * @brief Control lowerpower status of mipi tx bus
 *
 * @param  [in]  mode lowerpower mode. see @ref lcd_bus_lp_ctrl_t
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_lp_ctrl(lcd_bus_lp_ctrl_t mode);

/**
 * @brief Register panel information
 *
 * @param  [in]  x_start screen column extent of the frame memory.
 * @param  [in]  y_start screen raw extent of the frame memory.
 * @param  [in]  fps screen frame per second.
 *
 * @retval NONE.
 */
void mipi_bus_register_panel_info(uint16_t x_start, uint16_t y_start, uint32_t fps);

/**
 * @brief Get panel timing information
 *
 * @param  [in]  rect partial display area.
 * @param  [out] info panel timing information.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_get_panel_timing(const ext_rect *rect, bus_panel_info_t *info);

/**
 * @brief Get partial display cmd
 *
 * @param  [in]  rect partial display area.
 * @param  [out] cmd partial display cmd. see @ref partial_display_cmd_t
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_get_partial_display_cmd(const ext_rect *rect, partial_display_cmd_t *cmd);

/**
 * @brief Update panel by mipi tx bus
 *
 * @param  [in]  opt update option. see @ref panel_option_t
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_update_panel(panel_option_t opt);

/**
 * @brief refresh display area
 *
 * @param  [in]  rect partial display area.
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_refresh_area(const ext_rect *rect);

/**
 * @brief mipi suspend
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_suspend(void);

/**
 * @brief mipi resume
 *
 * @param  NONE
 *
 * @retval EXT_ERR_SUCCESS Success.
 * @retval Other           Failure. For details, see @ref soc_errno
 */
ext_errno mipi_bus_resume(void);
#endif