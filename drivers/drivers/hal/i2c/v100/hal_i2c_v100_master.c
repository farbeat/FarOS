/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides HAL i2c \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15, Create file. \n
 */

#include <stdint.h>
#include "securec.h"
#include "tcxo.h"
#include "common_def.h"
#include "i2c_porting.h"
#include "hal_i2c.h"
#include "hal_i2c_v100_comm.h"
#include "hal_i2c_v100_master.h"

#define HAL_I2C_SS_MIN_SCL_LCNT 13
#define HAL_I2C_SS_MIN_SCL_HCNT 14
#define HAL_I2C_FS_MIN_SCL_LCNT 16
#define HAL_I2C_FS_MIN_SCL_HCNT 14
#define HAL_I2C_HS_MIN_SCL_LCNT 22
#define HAL_I2C_HS_MIN_SCL_HCNT 14

#define HAL_I2C_LCNT_FIXED_OFFSET 1
#define HAL_I2C_HCNT_FIXED_OFFSET 8

static bool hal_i2c_cfg_taget_address(i2c_bus_t bus, uint16_t addr, hal_i2c_addr_permission_t permission)
{
    hal_i2c_addr_cfg_attr_t cfg_attr;

    bool cfg_valid = hal_i2c_v100_get_address_cfg(addr, permission, &cfg_attr);
    if (cfg_valid == false) {
        return false;
    }

    hal_i2c_set_ic_disable(bus);
    hal_i2c_set_con_master_address_mode(bus, cfg_attr.addr_width);

    if (cfg_attr.addr_type == I2C_ADDR_TYPE_FOR_GC) {
        hal_i2c_v100_set_gc_mode(bus);
    } else {
        hal_i2c_v100_set_normal_mode(bus);
        hal_i2c_set_master_target_address(bus, cfg_attr.real_addr);
    }
    hal_i2c_set_ic_enable(bus);
    return true;
}

static hal_i2c_speed_mode_t hal_i2c_v100_get_speed_mode(uint32_t baudrate)
{
    if (baudrate <= I2C_SS_MODE_BAUDRATE_HIGH_LIMIT) {
        return I2C_SPEED_MODE_SS;
    } else if (baudrate <= I2C_FS_MODE_BAUDRATE_HIGH_LIMIT) {
        return I2C_SPEED_MODE_FS;
    }
    return I2C_SPEED_MODE_HS;
}

static void hal_i2c_v100_calc_scl_cnt(const hal_i2c_scl_cnt_calc_input_t *input,
                                      hal_i2c_scl_cnt_calc_output_t *output)
{
    /* ref scl_cnt @Databook */
    static hal_i2c_scl_region_t scl_cnt_min[I2C_SPEED_MODE_MAX_NUM] = {
        {HAL_I2C_SS_MIN_SCL_LCNT, HAL_I2C_SS_MIN_SCL_HCNT},
        {HAL_I2C_FS_MIN_SCL_LCNT, HAL_I2C_FS_MIN_SCL_HCNT},
        {HAL_I2C_HS_MIN_SCL_LCNT, HAL_I2C_HS_MIN_SCL_HCNT}
    };

    uint32_t min_lcnt = scl_cnt_min[input->speed_mode].lcnt;
    uint32_t min_hcnt = scl_cnt_min[input->speed_mode].hcnt;

    uint32_t total_cnt = (input->freq + input->baudrate) / input->baudrate;
    uint32_t lcnt_add = 0;
    uint32_t hcnt_add = 0;

    if (total_cnt > (min_lcnt + min_hcnt)) {
        lcnt_add = (total_cnt - (min_lcnt + min_hcnt)) >> 1;
        hcnt_add = total_cnt - lcnt_add - min_lcnt - min_hcnt;
    }

    output->hcnt = min_hcnt + hcnt_add - HAL_I2C_HCNT_FIXED_OFFSET;
    output->lcnt = min_lcnt + lcnt_add - HAL_I2C_LCNT_FIXED_OFFSET;
}

static void hal_i2c_v100_set_scl_cnt(i2c_bus_t bus, hal_i2c_speed_mode_t speed_mode,
                                     hal_i2c_scl_cnt_calc_output_t *output)
{
    if (speed_mode == I2C_SPEED_MODE_SS) {
        hal_i2c_set_ss_scl_hcnt(bus, output->hcnt);
        hal_i2c_set_ss_scl_lcnt(bus, output->lcnt);
    } else if (speed_mode == I2C_SPEED_MODE_FS) {
        hal_i2c_set_fs_scl_hcnt(bus, output->hcnt);
        hal_i2c_set_fs_scl_lcnt(bus, output->lcnt);
    } else {
        hal_i2c_set_hs_scl_hcnt(bus, output->hcnt);
        hal_i2c_set_hs_scl_lcnt(bus, output->lcnt);
    }
}

static void hal_i2c_v100_master_cfg_baudrate(i2c_bus_t bus, hal_i2c_speed_mode_t speed_mode, uint32_t baudrate)
{
    hal_i2c_scl_cnt_calc_input_t input;
    hal_i2c_scl_cnt_calc_output_t output = {0};
    input.freq = i2c_port_get_clock_value(bus);
    input.speed_mode = speed_mode;
    input.baudrate = baudrate;

    hal_i2c_v100_calc_scl_cnt(&input, &output);
    hal_i2c_v100_set_scl_cnt(bus, speed_mode, &output);
}

static errcode_t hal_i2c_v100_master_write_prepeare(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_prepare_config_t *cfg = (hal_i2c_prepare_config_t *)param;

    bool rst = hal_i2c_cfg_taget_address(bus, cfg->addr, I2C_ADDR_PERMISION_MASTER);
    if (!rst) {
        return ERRCODE_I2C_PERMISSION_INVALID;
    }

    hal_i2c_v100_com_clr_int(bus);
    hal_i2c_v100_get_ctrl_info(bus)->write_operate_type = cfg->operation_type;
    if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_INT) {
        hal_i2c_v100_com_prepare_int(bus);
        hal_i2c_v100_prepare_int_tx(bus);
    } else if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_DMA) {
        hal_i2c_set_transmit_dma_en(bus, true);
        hal_i2c_set_transmit_dma_level(bus, HAL_I2C_TX_DMA_TL);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_write_restore(i2c_bus_t bus, uintptr_t param)
{
    unused(param);
    hal_i2c_v100_mask_int(bus, I2C_INT_TX_EMPTY);
    hal_i2c_set_transmit_dma_en(bus, false);

    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_read_prepeare(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_prepare_config_t *cfg = (hal_i2c_prepare_config_t *)param;

    bool rst = hal_i2c_cfg_taget_address(bus, cfg->addr, I2C_ADDR_PERMISION_SLAVE);
    if (!rst) {
        return ERRCODE_I2C_PERMISSION_INVALID;
    }

    hal_i2c_v100_com_clr_int(bus);
    hal_i2c_v100_get_ctrl_info(bus)->read_operate_type = cfg->operation_type;
    if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_INT) {
        hal_i2c_v100_prepare_int_rx(bus);
    } else if (cfg->operation_type == I2C_DATA_OPERATION_TYPE_DMA) {
        hal_i2c_set_transmit_dma_level(bus, HAL_I2C_TX_DMA_TL);
        hal_i2c_set_receive_dma_level(bus, HAL_I2C_RX_DMA_TL - 1);

        hal_i2c_set_transmit_dma_en(bus, true);
        hal_i2c_set_receive_dma_en(bus, true);
    }

    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_read_restore(i2c_bus_t bus, uintptr_t param)
{
    unused(param);
    hal_i2c_v100_com_clr_int(bus);
    hal_i2c_v100_com_restore_int(bus);
    hal_i2c_v100_restore_int_rx(bus);
    hal_i2c_set_transmit_dma_en(bus, false);
    hal_i2c_set_receive_dma_en(bus, false);

    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_check_tx_available(i2c_bus_t bus, uintptr_t param)
{
    ic_status_data_t status;
    status.d32 = hal_i2c_get_status(bus);
    *(uint32_t *)param = status.b.tfnf;
    return ERRCODE_SUCC;
}

static uint32_t hal_i2c_v100_txfifo_not_full(i2c_bus_t bus)
{
    ic_status_data_t status;
    status.d32 = hal_i2c_get_status(bus);
    return status.b.tfnf;
}

static errcode_t hal_i2c_v100_master_flush_rx_fifo(i2c_bus_t bus, uintptr_t param)
{
    hal_i2c_buffer_wrap_t *read_data = (hal_i2c_buffer_wrap_t *)param;
    if ((read_data == NULL) || read_data->len == 0) {
        return ERRCODE_I2C_RECEIVE_PARAM_INVALID;
    }
    uint64_t start_time = uapi_tcxo_get_ms();
    uint32_t len = read_data->len;
    hal_i2c_v100_update_rx_tl(bus, len);
    uint32_t loop = 0;
    uint64_t current_time = 0;
    uint64_t time_diff;
    do {
        current_time = uapi_tcxo_get_ms();
        time_diff = current_time - start_time;
        if (current_time > start_time && time_diff > CONFIG_I2C_WAIT_CONDITION_TIMEOUT) {
            return ERRCODE_I2C_TIMEOUT;
        }
        if (hal_i2c_v100_txfifo_not_full(bus) != 0) {
            uint32_t first_flag = (loop == 0);
            uint32_t last_flag = ((loop + 1) == len);
            uint32_t restart_flag = read_data->restart_flag;
            uint32_t stop_flag = read_data->stop_flag;

            uint32_t read_cmd = hal_i2c_v100_get_data_read_cmd(first_flag, restart_flag, last_flag, stop_flag);
            hal_i2c_set_read_direction(bus, read_cmd);
            loop++;
        }
    } while (loop < len);

    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_check_rx_transmit_end(i2c_bus_t bus, uintptr_t param)
{
    *(uint32_t *)param = hal_i2c_v100_get_int(bus, RAW_INTR_STAT, I2C_INT_STOP_DET);
    return ERRCODE_SUCC;
}

static errcode_t hal_i2c_v100_master_check_reastart_ready(i2c_bus_t bus, uintptr_t param)
{
    unused(bus);
    *(uint32_t *)param = true;
    return ERRCODE_SUCC;
}

static hal_i2c_inner_ctrl_t g_hal_i2c_master_ctrl_func_array[I2C_CTRL_MAX] = {
    hal_i2c_v100_master_write_prepeare,               /* I2C_CTRL_WRITE_PREPARE */
    hal_i2c_v100_master_write_restore,                /* I2C_CTRL_WRITE_RESTORE */
    hal_i2c_v100_master_read_prepeare,                /* I2C_CTRL_READ_PREPARE */
    hal_i2c_v100_master_read_restore,                 /* I2C_CTRL_READ_RESTORE */
    hal_i2c_v100_get_write_num,                       /* I2C_CTRL_GET_WRITE_NUM */
    hal_i2c_v100_get_read_num,                        /* I2C_CTRL_GET_READ_NUM */
    hal_i2c_v100_master_check_tx_available,           /* I2C_CTRL_CHECK_TX_AVAILABLE */
    hal_i2c_v100_check_rx_available,                  /* I2C_CTRL_CHECK_RX_AVAILABLE */
    hal_i2c_v100_master_flush_rx_fifo,                /* I2C_CTRL_FLUSH_RX_FIFO */
    hal_i2c_v100_check_tx_transmit_end,               /* I2C_CTRL_CHECK_TX_PROCESS_DONE */
    hal_i2c_v100_master_check_rx_transmit_end,        /* I2C_CTRL_CHECK_RX_PROCESS_DONE */
    hal_i2c_v100_master_check_reastart_ready,         /* I2C_CTRL_CHECK_RESTART_READY */
    hal_i2c_v100_check_transmit_abrt,                 /* I2C_CTRL_CHECK_TRANSMIT_ABRT */
    hal_i2c_v100_get_data_addr,                       /* I2C_CTRL_GET_DMA_DATA_ADDR */
};

errcode_t hal_i2c_v100_master_init(i2c_bus_t bus,
    uint32_t baudrate, uint8_t hscode, hal_i2c_callback_t callback)
{
    hal_i2c_v100_regs_init(bus);

    hal_i2c_ctrl_info_t *hal_i2c_ctrl_info = hal_i2c_v100_get_ctrl_info(bus);
    hal_i2c_v100_deinit(bus);

    hal_i2c_v100_init_comp_param(bus);
    hal_i2c_set_con_master_mode(bus);
    hal_i2c_set_con_master_address_mode(bus, I2C_7_BITS_ADDR);
    hal_i2c_set_con_rx_full_hold(bus, IC_CON_ENABLED_BIT);

    /* First ensure baudrate is valid, the cfg mode and code, make sure the cfg can be restore */
    hal_i2c_speed_mode_t speed_mode = hal_i2c_v100_get_speed_mode(baudrate);
    hal_i2c_v100_master_cfg_baudrate(bus, speed_mode, baudrate);
    hal_i2c_set_speed_mode(bus, (uint32_t)speed_mode + 1);
    hal_i2c_ctrl_info->max_speed_mode = speed_mode;

    if (speed_mode == I2C_SPEED_MODE_HS) {
        hal_i2c_set_hs_address_code(bus, hscode);
        hal_i2c_ctrl_info->hscode = hscode;
    }
    hal_i2c_ctrl_info->work_baudrate = baudrate;

    hal_i2c_v100_mask_all_int(bus);
    hal_i2c_v100_com_clr_int(bus);

    hal_i2c_set_ic_enable(bus);
    hal_i2c_register_callback(callback);
    hal_i2c_v100_load_ctrl_func(bus, g_hal_i2c_master_ctrl_func_array);
    return ERRCODE_SUCC;
}