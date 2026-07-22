/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module software timer headerfile
 */
#ifndef SWTMR_DEBUG_H
#define SWTMR_DEBUG_H

#include "FreeRTOS.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup swtmr_debug
 * @par Description:
 * This API is used to Print the software timer information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vSwtmrInfoPrint(void);

/**
 * @ingroup swtmr_debug
 * @par Description:
 * This API is used to get the software timer number of our system.
 *
 * @param void.
 *
 * @retval UBaseType_t.
 */
UBaseType_t xSwTmrCntGet(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SWTMR_DEBUG_H */
