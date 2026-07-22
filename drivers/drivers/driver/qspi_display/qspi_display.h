/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of display with qspi.
 * Author: @CompanyNameTag
 * Create: 2022-09-05
 */

#ifndef __QSPI_DISPLAY_H__
#define __QSPI_DISPLAY_H__

#include "soc_errno.h"
#include "core.h"
#include "errcode.h"
#include "pinctrl_porting.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t reg_addr;
    uint32_t data_len;
    uint8_t  *data;
} qspi_cmd_info_t;

typedef struct {
    uint32_t frame_addr;            /* memory address saving frame data */
    uint32_t frame_col_byte;        /* column byte equal to column_num * color_mode_bytes, range[1, 4095] */
    uint32_t frame_raw;
    uint32_t col_byte_with_stride;  /* range[1, 4095] */
    uint32_t channel;
} qspi_display_frame_info_t;

typedef void (*qspi_send_frame_cb_t)(void);

ext_errno uapi_qspi_display_init(void);
ext_errno uapi_qspi_display_deinit(void);
ext_errno uapi_qspi_display_enable(qspi_display_frame_info_t *frame_info);
ext_errno uapi_qspi_display_disable(void);
ext_errno uapi_qspi_display_write_cmd(qspi_cmd_info_t *cmd_info);
ext_errno uapi_qspi_display_read_cmd(uint32_t cmd, uint8_t *buff, uint8_t len);
ext_errno uapi_qspi_display_show_on(uintptr_t frame_addr, qspi_send_frame_cb_t callback);
bool is_qspi_transfer_done(void);
errcode_t uapi_pin_set_mode(pin_t pin, pin_mode_t mode);
#endif