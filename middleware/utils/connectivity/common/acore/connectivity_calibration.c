/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: calibration interface
 * Author:
 * Create:
 */
#ifdef SUPPORT_IPC
#include "connectivity_calibration.h"
#endif
#include "securec.h"

#ifdef SUPPORT_IPC
calibration_ret_e send_bt_calibration_params(const uint8_t *params, uint32_t length)
{
    errno_t sec_ret;
    ipc_status_t ipc_returned_value;
    ipc_payload_ap_bt_calibration_param_req calibration_req;

    if (params == NULL || length == 0 || length > CALIBRATION_PARAMS_LENGTH) {
        return CALIBRATION_INVALID_PARAM;
    }

    sec_ret = memcpy_s(calibration_req.params, CALIBRATION_PARAMS_LENGTH, params, length);
    if (sec_ret != EOK) {
        return CALIBRATION_MEMCPY_FAIL;
    }
    calibration_req.length = length;

    ipc_returned_value = ipc_spin_send_message_timeout(CORES_BT_CORE,
                                                       IPC_ACTION_AP_BT_CALIBRATION_PARAM_REQ,
                                                       (ipc_payload *)&calibration_req,
                                                       sizeof(ipc_payload_ap_bt_calibration_param_req),
                                                       IPC_PRIORITY_LOWEST, false, IPC_SPIN_SEND_DEFAULT_TIMEOUT);
    if (ipc_returned_value != IPC_STATUS_OK) {
        return CALIBRATION_RET_ERROR;
    }

    return CALIBRATION_RET_OK;
}
#endif

