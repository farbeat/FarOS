/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides opi driver API \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-07, Create file. \n
 */
#ifndef OPI_H
#define OPI_H

#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_driver_opi OPI
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief  OPI return code.
 * @else
 * @brief  OPI 返回值
 * @endif
 */
typedef enum {
    OPI_RET_OK,           /*!< The operation has completed successfully */
    OPI_RET_BUSY,         /*!< The operation is in process */
    OPI_RET_UNINIT,       /*!< The operation fail because of not config */
    OPI_RET_UNCLAIM,      /*!< The operation fail because of do not claim bus */
    OPI_RET_TIMEOUT,      /*!< The operation fail because of timout */
    OPI_RET_CLK_ERR,      /*!< The operation fail because of clock error */
    OPI_RET_WRONG_BUS,    /*!< The operation fail because of wrong bus number */
    OPI_RET_WRONG_PARA,   /*!< The operation fail because of illegal parameter */
    OPI_RET_WRONG_DEVICE, /*!< The operation fail because of illegal device */
    OPI_RET_XIP_EN,       /*!< The operation fail because of XIP is enable */
    OPI_RET_XIP_DIS,      /*!< The operation fail because of XIP is disable */
    OPI_RET_SLEEP,        /*!< The operation fail because of PSRAM is sleep */
    OPI_RET_WAKE,         /*!< The operation fail because of PSRAM is wakeup */
    OPI_RET_ERROR,        /*!< The operation fail because of other reasons */
} opi_ret_e;

/**
 * @if Eng
 * @brief  Suspend opi interface, means that swtich to lpc.
 * @retval OPI_RET_OK    Success.
 * @retval OPI_RET_SLEEP OPI has been Suspended.
 * @else
 * @brief  挂起opi接口，即切入低功耗模式。
 * @retval OPI_RET_OK    成功
 * @retval OPI_RET_SLEEP OPI已经被挂起
 * @endif
 */
opi_ret_e uapi_opi_suspend(void);

/**
 * @if Eng
 * @brief  Resume opi interface, means that exit from lpc.
 * @retval OPI_RET_OK   Success.
 * @retval OPI_RET_WAKE OPI has been Resumed.
 * @else
 * @brief  恢复opi接口，即退出低功耗模式。
 * @retval OPI_RET_OK   成功
 * @retval OPI_RET_WAKE OPI已经恢复
 * @endif
 */
opi_ret_e uapi_opi_resume(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OPI_H */
