/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module system infomation headerfile
 */
#ifndef SYSTEM_DEBUG_H
#define SYSTEM_DEBUG_H

#include "FreeRTOS.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup sysinfo_debug
 * @par Description:
 * This API is used to Print the system information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vSystemInfoPrint(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SYSTEM_DEBUG_H */
