/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides HAL i2c \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15, Create file. \n
 */

#include "common_def.h"
#include "tcxo.h"
#include "hal_i2c.h"
#include "hal_i2c_v100_regs_op.h"
#include "hal_i2c_v100_comm.h"

#define HAL_I2C_VALID_ADDR_VALUE_MASK    0x3FF  /* Address bits mask for VALID address */

static hal_i2c_inner_ctrl_t *g_hal_i2c_ctrl_func[I2C_BUS_MAX_NUM];
static hal_i2c_ctrl_info_t g_hal_i2c_ctrl_info[I2C_BUS_MAX_NUM];

hal_i2c_callback_t g_hal_i2c_callback;

static hal_i2c_addr_attr_t *hal_i2c_v100_get_address_attr(uint16_t addr)
{
    /* Addr permission default table, if needed, please customize for system */
#ifdef CONFIG_I2C_ADDR_VALID_CHECK
    static hal_i2c_addr_region_attr_t i2c_v100_valid_addr[] = {
        {{0x0, 0x0}, {I2C_ADDR_TYPE_FOR_GC, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_SLAVE, 0}},
        {{0x1, 0x1}, {I2C_ADDR_TYPE_FOR_CBUS, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_INVALID, 0}},
        {{0x2, 0x3}, {I2C_ADDR_TYPE_FOR_RESERVED, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_INVALID, 0}},
        {{0x4, 0x7}, {I2C_ADDR_TYPE_FOR_HS_MODE, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_INVALID, 0}},
        {{0x8, 0x77}, {I2C_ADDR_TYPE_FOR_I2C_TARGET, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_MASTER_SLAVE, 0}},
        {{0x78, 0x77FF}, {I2C_ADDR_TYPE_INVALID, I2C_INVALID_BITS_ADDR, I2C_ADDR_PERMISION_INVALID, 0}},
        {{0x7800, 0x7BFF}, {I2C_ADDR_TYPE_FOR_I2C_TARGET, I2C_10_BITS_ADDR, I2C_ADDR_PERMISION_MASTER_SLAVE, 0}},
        {{0x7C00, 0x7FFF}, {I2C_ADDR_TYPE_FOR_RESERVED, I2C_10_BITS_ADDR, I2C_ADDR_PERMISION_INVALID, 0}},
    };
#else
    /* Addr permission default table, just check max addr. */
    static hal_i2c_addr_region_attr_t i2c_v100_valid_addr[] = {
        {{0x0, 0x7F}, {I2C_ADDR_TYPE_FOR_I2C_TARGET, I2C_7_BITS_ADDR, I2C_ADDR_PERMISION_MASTER_SLAVE, 0}},
        {{0x80, 0x7FFF}, {I2C_ADDR_TYPE_FOR_I2C_TARGET, I2C_10_BITS_ADDR, I2C_ADDR_PERMISION_MASTER_SLAVE, 0}},
    };
#endif

    for (uint32_t i = 0; i < sizeof(i2c_v100_valid_addr) / sizeof(i2c_v100_valid_addr[0]); i++) {
        hal_i2c_addr_region_t *region = &i2c_v100_valid_addr[i].region;
        if ((addr >= region->min_addr) && (addr <= region->max_addr)) {
            return &i2c_v100_valid_addr[i].attr;
        }
    }
    return NULL;
}

bool hal_i2c_v100_get_address_cfg(uint16_t addr, hal_i2c_addr_permission_t permission,
                                  hal_i2c_addr_cfg_attr_t *cfg_attr)
{
    hal_i2c_addr_attr_t *attr = hal_i2c_v100_get_address_attr(addr);

    if (attr == NULL) {
        return false;
    }

    if (((uint8_t)permission & attr->permission) != (uint8_t)permission) {
        return false;
    }

    cfg_attr->real_addr = addr & HAL_I2C_VALID_ADDR_VALUE_MASK;
    cfg_attr->addr_type = attr->type;
    cfg_attr->addr_width = attr->width;

    return true;
}

void hal_i2c_v100_com_clr_int(i2c_bus_t bus)
{
    hal_i2c_clr_tx_abrt_intr(bus);
    hal_i2c_clr_all_int(bus);
}

void hal_i2c_v100_com_prepare_int(i2c_bus_t bus)
{
    hal_i2c_v100_unmask_int(bus, I2C_INT_START_DET);
    hal_i2c_v100_unmask_int(bus, I2C_INT_STOP_DET);
}

void hal_i2c_v100_com_restore_int(i2c_bus_t bus)
{
    hal_i2c_v100_mask_int(bus, I2C_INT_START_DET);
    hal_i2c_v100_mask_int(bus, I2C_INT_STOP_DET);
}

void hal_i2c_v100_prepare_int_tx(i2c_bus_t bus)
{
    hal_i2c_v100_unmask_int(bus, I2C_INT_TX_OVER);
    hal_i2c_v100_unmask_int(bus, I2C_INT_RD_REQ);
    hal_i2c_v100_unmask_int(bus, I2C_INT_TX_ABRT);
    hal_i2c_v100_unmask_int(bus, I2C_INT_TX_EMPTY);
}

void hal_i2c_v100_restore_int_tx(i2c_bus_t bus)
{
    hal_i2c_v100_mask_int(bus, I2C_INT_TX_EMPTY);
    hal_i2c_v100_mask_int(bus, I2C_INT_TX_OVER);
    hal_i2c_v100_mask_int(bus, I2C_INT_RD_REQ);
    hal_i2c_v100_mask_int(bus, I2C_INT_TX_ABRT);
}

void hal_i2c_v100_prepare_int_rx(i2c_bus_t bus)
{
    hal_i2c_v100_unmask_int(bus, I2C_INT_RX_OVER);
    hal_i2c_v100_unmask_int(bus, I2C_INT_RX_FULL);
    hal_i2c_v100_unmask_int(bus, I2C_INT_RX_DONE);
}

void hal_i2c_v100_restore_int_rx(i2c_bus_t bus)
{
    hal_i2c_v100_mask_int(bus, I2C_INT_RX_OVER);
    hal_i2c_v100_mask_int(bus, I2C_INT_RX_FULL);
    hal_i2c_v100_mask_int(bus, I2C_INT_RX_DONE);
}

static uint32_t hal_i2c_v100_get_tx_fifo_remain(i2c_bus_t bus)
{
    hal_i2c_ctrl_info_t *i2c_ctrl = &g_hal_i2c_ctrl_info[bus];
    if (i2c_ctrl->tx_fifo_max_deepth > i2c_ctrl->tx_tl) {
        return i2c_ctrl->tx_fifo_max_deepth - i2c_ctrl->tx_tl;
    }
    return 0;
}

static uint32_t hal_i2c_v100_get_default_rx_tl(i2c_bus_t bus)
{
    hal_i2c_ctrl_info_t *i2c_ctrl = &g_hal_i2c_ctrl_info[bus];
    uint32_t rx_burst_num = i2c_ctrl->rx_fifo_max_deepth >> 1;
    return rx_burst_num > 1 ? rx_burst_num : 1;
}

void hal_i2c_v100_update_rx_tl(i2c_bus_t bus, uint32_t rx_tl)
{
    uint32_t real_rx_tl = rx_tl > 1 ? rx_tl : 1;
    hal_i2c_set_receive_threshold_level(bus, real_rx_tl - 1);
}

errcode_t hal_i2c_v100_check_transmit_abrt(i2c_bus_t bus, uintptr_t param)
{
    uint32_t *abrt_status = (uint32_t *)param;

    *abrt_status = hal_i2c_v100_get_int(bus, RAW_INTR_STAT, I2C_INT_TX_ABRT);

    return ERRCODE_SUCC;
}

errcode_t hal_i2c_v100_get_write_num(i2c_bus_t bus, uintptr_t param)
{
    *(uint32_t *)param = hal_i2c_v100_get_tx_fifo_remain(bus);
    return ERRCODE_SUCC;
}

errcode_t hal_i2c_v100_get_read_num(i2c_bus_t bus, uintptr_t param)
{
    *(uint32_t *)param = hal_i2c_v100_get_default_rx_tl(bus);
    return ERRCODE_SUCC;
}

errcode_t hal_i2c_v100_check_rx_available(i2c_bus_t bus, uintptr_t param)
{
    unused(bus);
    *(uint32_t *)param = true;
    return ERRCODE_SUCC;
}

errcode_t hal_i2c_v100_check_tx_transmit_end(i2c_bus_t bus, uintptr_t param)
{
    *(uint32_t *)param = hal_i2c_v100_get_int(bus, RAW_INTR_STAT, I2C_INT_STOP_DET);
    return ERRCODE_SUCC;
}

static uint32_t hal_i2c_v100_tx_not_full(i2c_bus_t bus)
{
    ic_status_data_t status;
    status.d32 = hal_i2c_get_status(bus);
    return status.b.tfnf;
}

void hal_i2c_register_callback(hal_i2c_callback_t callback)
{
    g_hal_i2c_callback = callback;
}

void hal_i2c_v100_load_ctrl_func(i2c_bus_t bus, hal_i2c_inner_ctrl_t *func_table)
{
    g_hal_i2c_ctrl_func[bus] = func_table;
}

void hal_i2c_v100_unload_ctrl_func(i2c_bus_t bus)
{
    g_hal_i2c_ctrl_func[bus] = NULL;
}

hal_i2c_ctrl_info_t *hal_i2c_v100_get_ctrl_info(i2c_bus_t bus)
{
    return &g_hal_i2c_ctrl_info[bus];
}

errcode_t hal_i2c_v100_write(i2c_bus_t bus, hal_i2c_buffer_wrap_t *data)
{
    if ((data == NULL) || (data->buffer == NULL) || data->len == 0) {
        return ERRCODE_I2C_SEND_PARAM_INVALID;
    }

    uint64_t start_time = uapi_tcxo_get_ms();
    uint32_t loop = 0;
    uint8_t *buffer = data->buffer;
    uint32_t len = data->len;
    uint64_t current_time = 0;
    uint64_t time_diff;
    do {
        current_time = uapi_tcxo_get_ms();
        time_diff = current_time - start_time;
        if (current_time > start_time && time_diff > CONFIG_I2C_WAIT_CONDITION_TIMEOUT) {
            return ERRCODE_I2C_TIMEOUT;
        }
        if (hal_i2c_v100_tx_not_full(bus) != 0) {
            uint32_t first_flag = (loop == 0);
            uint32_t last_flag = ((loop + 1) == len);
            uint32_t restart_flag = data->restart_flag;
            uint32_t stop_flag = data->stop_flag;

            uint32_t write_cmd = hal_i2c_v100_get_data_write_cmd(first_flag, restart_flag, last_flag, stop_flag);
            hal_i2c_v100_set_transmit_data(bus, write_cmd, buffer[loop]);
            loop++;
        }
    } while (loop < data->len);
    return ERRCODE_SUCC;
}

static bool hal_i2c_v100_rx_not_empty(i2c_bus_t bus)
{
    ic_status_data_t status;
    status.d32 = hal_i2c_get_status(bus);
    return (bool)status.b.rfne;
}

errcode_t hal_i2c_v100_read(i2c_bus_t bus, hal_i2c_buffer_wrap_t *data)
{
    if ((data == NULL) || (data->buffer == NULL) || data->len == 0) {
        return ERRCODE_I2C_RECEIVE_PARAM_INVALID;
    }
    uint64_t start_time = uapi_tcxo_get_ms();
    uint32_t loop = 0;
    uint64_t current_time = 0;
    uint64_t time_diff;
    do {
        current_time = uapi_tcxo_get_ms();
        time_diff = current_time - start_time;
        if (current_time > start_time && time_diff > CONFIG_I2C_WAIT_CONDITION_TIMEOUT) {
            return ERRCODE_I2C_TIMEOUT;
        }
        if (hal_i2c_v100_rx_not_empty(bus)) {
            data->buffer[loop] = hal_i2c_get_receive_data(bus);
            loop++;
        }
    } while (loop < data->len);
    return ERRCODE_SUCC;
}

errcode_t hal_i2c_v100_deinit(i2c_bus_t bus)
{
    hal_i2c_set_ic_disable(bus);
    hal_i2c_v100_mask_all_int(bus);
    hal_i2c_v100_com_clr_int(bus);
    hal_i2c_v100_unload_ctrl_func(bus);
    return ERRCODE_SUCC;
}

static uint16_t hal_i2c_v100_normalize_fifo_deepth(uint8_t deepth_param)
{
    if (deepth_param == 0) {
        return HAL_I2C_V100_DEFAULT_FIFO_DEEPTH;
    }

    return deepth_param + 1;
}

void hal_i2c_v100_init_comp_param(i2c_bus_t bus)
{
    ic_comp_param_data_t comp_para;

    comp_para.d32 = hal_i2c_get_component_parameter(bus);

    hal_i2c_ctrl_info_t *hal_i2c_ctrl_info = &g_hal_i2c_ctrl_info[bus];

    hal_i2c_ctrl_info->tx_fifo_max_deepth = hal_i2c_v100_normalize_fifo_deepth(comp_para.b.tx_buffer_depth);
    hal_i2c_ctrl_info->rx_fifo_max_deepth = hal_i2c_v100_normalize_fifo_deepth(comp_para.b.rx_buffer_depth);

    hal_i2c_ctrl_info->tx_tl = hal_i2c_ctrl_info->tx_fifo_max_deepth >> 1;
    hal_i2c_ctrl_info->rx_tl = (hal_i2c_ctrl_info->rx_fifo_max_deepth >> 1) - 1;

    hal_i2c_set_transmit_threshold_level(bus, hal_i2c_ctrl_info->tx_tl);
    hal_i2c_set_receive_threshold_level(bus, hal_i2c_ctrl_info->rx_tl);

    if (comp_para.b.has_dma == true) {
        hal_i2c_ctrl_info->tx_dma_valid = true;
        /* The dma_tx_req signal is generated when the number of valid data entries in the transmit FIFO */
        hal_i2c_ctrl_info->tx_dma_tl = HAL_I2C_TX_DMA_TL;

        hal_i2c_ctrl_info->rx_dma_valid = true;
        /* Dma_rx_req is generated when the number of valid data entries in the receive FIFO */
        hal_i2c_ctrl_info->rx_dma_tl = HAL_I2C_RX_DMA_TL;
    } else {
        hal_i2c_ctrl_info->tx_dma_valid = false;
        hal_i2c_ctrl_info->rx_dma_valid = false;
    }
}

errcode_t hal_i2c_v100_ctrl(i2c_bus_t bus, hal_i2c_ctrl_id_t id, uintptr_t param)
{
    if (g_hal_i2c_ctrl_func[bus] == NULL) {
        return ERRCODE_I2C_NOT_INIT;
    }
    if (g_hal_i2c_ctrl_func[bus][id] != NULL) {
        return g_hal_i2c_ctrl_func[bus][id](bus, param);
    }
    return ERRCODE_I2C_NOT_IMPLEMENT;
}

errcode_t hal_i2c_v100_get_data_addr(i2c_bus_t bus, uintptr_t param)
{
    uint32_t *addr = (uint32_t *)param;
    *addr = (uint32_t)(uintptr_t)(&g_i2c_regs[bus]->data_cmd);
    return ERRCODE_SUCC;
}

void hal_i2c_v100_irq_handler(i2c_bus_t bus)
{
    if (!g_hal_i2c_callback) {
        return;
    }

    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_STOP_DET) != 0) {
        hal_i2c_v100_set_normal_mode(bus);
        hal_i2c_v100_com_clr_int(bus);
        hal_i2c_v100_com_restore_int(bus);
        hal_i2c_v100_restore_int_tx(bus);
        g_hal_i2c_callback(bus, I2C_EVT_TRANSMITION_DONE, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_RD_REQ) != 0) {
        hal_i2c_v100_prepare_int_tx(bus);
        hal_i2c_v100_com_prepare_int(bus);
        hal_i2c_clr_all_int(bus);
        hal_i2c_v100_mask_int(bus, I2C_INT_RD_REQ);
        g_hal_i2c_callback(bus, I2C_EVT_TX_AVAILABLE, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_TX_EMPTY) != 0) {
        g_hal_i2c_callback(bus, I2C_EVT_TX_READY, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_RX_FULL) != 0) {
        g_hal_i2c_callback(bus, I2C_EVT_RX_READY, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_START_DET) != 0) {
        hal_i2c_clr_start_det_intr(bus);
        g_hal_i2c_callback(bus, I2C_EVT_TRANSMITION_START, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_STOP_DET) != 0) {
        g_hal_i2c_callback(bus, I2C_EVT_TRANSMITION_DONE, 0);
    }
    if (hal_i2c_v100_get_int(bus, INTR_STAT, I2C_INT_TX_ABRT) != 0) {
        g_hal_i2c_callback(bus, I2C_EVT_TRANSMITION_ABRT, 0);
    }
    hal_i2c_clr_all_int(bus);
}