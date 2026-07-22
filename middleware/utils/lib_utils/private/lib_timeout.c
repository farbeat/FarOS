/*
 * Copyright (c) CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:  Some functions for checing the timeout periodic.
 * Author:
 * Create: 2019-05-30
 */

#include "lib_utils.h"
#include "tcxo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TIMEOUT_CHECK_PERIOD 10000

bool lib_timeout_check_timeout_periodic(uint64_t timeout, uint64_t start_time, uint32_t *check_times)
{
    (*check_times)++;
    if ((*check_times) > TIMEOUT_CHECK_PERIOD) {
        if ((timeout == 0) || ((uapi_tcxo_get_ms() - start_time) > timeout)) {
            return true;
        }
        (*check_times) = 0;
    }

    return false;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
