/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: calibration interface header
 * Author:
 * Create:
 */
#ifndef SRC_LIB_CONNECTIVITY_CALIBRATION_H
#define SRC_LIB_CONNECTIVITY_CALIBRATION_H

#include "ipc.h"

/**
 * @defgroup connectivity_libs_connectivity_calibration CONNECTIVITY Calibration
 * @ingroup  connectivity_libs_connectivity
 * @{
 */
/**
 * @brief  Definitions of the operation result
 */
typedef enum {
    CALIBRATION_RET_OK,             // !< operation successful.
    CALIBRATION_PARAM_NULL,         // !< the poiner is NULL.
    CALIBRATION_INVALID_PARAM,      // !< the parameters fron App core is invalid.
    CALIBRATION_MEMCPY_FAIL,        // !< memory copy fail.
    CALIBRATION_MUTEX_NOT_READY,    // !< The mutex error.
    CALIBRATION_RET_ERROR           // !< operation other fail.
} calibration_ret_e;

#if CORE == APPS

/**
 * @brief  Send the calibration params of BT.
 * @param  params: The calibration parameters.
 * @param  length: The length of the calibration parameters.
 * @return calibration_ret_e.
 */
calibration_ret_e send_bt_calibration_params(const uint8_t *params, uint32_t length);

#elif CORE == BT

typedef struct {
    uint32_t length;
    uint8_t params[CALIBRATION_PARAMS_LENGTH];
} calibration_config_t;

/**
 * @brief  Init the calibration configuration module.
 * @return calibration_ret_e.
 */
calibration_ret_e calibration_init(void);

/**
 * @brief  Get the configuration parameters.
 * @return the valid parameters or NULL.
 */
calibration_config_t *calibration_config_get(void);

#if defined(BUILD_APPLICATION_STANDARD)
/**
 * @brief  Release sem
 * @return NULL.
 */
void calibration_release_sem(void);

/**
 * @brief  set _calibration param for wear bth(eg. bth get param by ipc_action_data message)
 * @param  cali_data pointer of calibration param data.
 * @param  length    lenght of calibration param data.
 * @return success or not.
 */
bool bt_calibration_param_set(uint8* cali_data, uint16_t length);
#endif
#endif

/**
 * @}
 */
#endif
