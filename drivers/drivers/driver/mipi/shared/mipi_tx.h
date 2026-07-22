/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: mipi tx header file.
 * Author: @CompanyNameTag
 * Create: 2022-04-14
 */

#ifndef __MIPI_TX_H__
#define __MIPI_TX_H__

#include "stdio.h"
#include "soc_osal.h"
#include "soc_mipi_tx.h"
#include "hal_mipi_tx.h"
#include "debug_print.h"

#define MIPI_TX_SET_DEV_CFG              1
#define MIPI_TX_SET_CMD                  2
#define MIPI_TX_ENABLE                   3
#define MIPI_TX_GET_CMD                  4
#define MIPI_TX_DISABLE                  5

#ifdef MIPI_TX_DEBUG
#define mipi_tx_print(fmt, ...)            \
    do {                                \
        PRINT(fmt, ##__VA_ARGS__);     \
    } while (0)
#else
#define mipi_tx_print(ftm, ...)
#endif

#define mipi_proc_print(fmt, ...)       \
    do {                                \
        PRINT(fmt, ##__VA_ARGS__);     \
    } while (0)

#define mipi_tx_err(x, ...) \
    do { \
        PRINT(x, ##__VA_ARGS__); \
    } while (0)

#define mipi_tx_check_null_ptr_return(ptr) \
    do { \
        if ((ptr) == NULL) { \
            mipi_tx_err("NULL pointer\r\n"); \
            return EXT_ERR_MIPI_INVALID_PARAM; \
        } \
    } while (0)

void mipi_tx_set_lp_clk_en(uint8_t lp_clk_en);
uint8_t mipi_tx_get_lp_clk_en(void);
uint32_t mipi_tx_get_lane_num(const int16_t lane_id[], uint32_t lane_id_len);

uint32_t mipi_tx_set_colorbar(colorbar_orien_mode orien, colorbar_input_mode mode);
uint32_t mipi_tx_unset_colorbar(void);

uint32_t mipi_tx_cfg_show(void);

void mipi_tx_drv_get_phy_cfg(void);

void mipi_tx_reset(void);

void mipi_tx_drv_set_partial_refresh(combo_dev_cfg_t *dev_cfg);

uint32_t mipi_tx_partial_refresh(sync_info_t *dev_sync);

uint32_t mipi_tx_proc_show(uint32_t s);
bool uapi_mipi_tx_is_in_ulps(void);

#endif
