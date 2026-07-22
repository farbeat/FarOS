/* ---------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Driver Framework Header File
 * Create: 2021-06-08
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

/**
 * @defgroup drivers Drivers
 * @defgroup dpal_driverbase Driverbase
 * @ingroup drivers
 *
 */

#ifndef DPAL_DRIVERBASE_H
#define DPAL_DRIVERBASE_H

#include "string.h"
#include "dpal_typedef.h"
#include "dpal_driver.h"
#include "dpal.h"

#define DEVICE_LOCK(dev)            (void)dpal_mux_pend((dev)->mutex, DPAL_WAIT_FOREVER)
#define DEVICE_UNLOCK(dev)          (void)dpal_mux_post((dev)->mutex)

#define DRIVER_LOCK(drv)            (void)dpal_mux_pend((drv)->mutex, DPAL_WAIT_FOREVER)
#define DRIVER_UNLOCK(drv)          (void)dpal_mux_post((drv)->mutex)

#define PM_LOCK(lock)               (void)dpal_mux_pend((lock), DPAL_WAIT_FOREVER)
#define PM_UNLOCK(lock)             (void)dpal_mux_post((lock))

#define DRIVER_BASE_LOCK(lock)      (void)dpal_mux_pend((lock), DPAL_WAIT_FOREVER)
#define DRIVER_BASE_UNLOCK(lock)    (void)dpal_mux_post((lock))

/* errno */
/**
 * @ingroup los_driverbase
 * Task error code: Invalid input.
 *
 * Value: 0x02004110
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_INPUT_INVALID                        DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x10)

/**
 * @ingroup los_driverbase
 * Task error code: Mutex failed.
 *
 * Value: 0x02004111
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_MUX_FAIL                             DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x11)

/**
 * @ingroup los_driverbase
 * Task error code: Do match function failed.
 *
 * Value: 0x02004120
 *
 * Solution: This error code is not in use temporarily.
 */
#define DPAL_ERRNO_DRIVER_DRIVER_MATCH_FAIL                    DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x20)

/**
 * @ingroup los_driverbase
 * Task error code: Do probe function failed.
 *
 * Value: 0x02004121
 *
 * Solution:  This error code is not in use temporarily.
 */
#define DPAL_ERRNO_DRIVER_DRIVER_PROBE_FAIL                    DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x21)

/**
 * @ingroup los_driverbase
 * Task error code: driver register twice.
 *
 * Value: 0x02004122
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DRIVER_REGISTERED                    DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x22)

/**
 * @ingroup los_driverbase
 * Task error code: Invalid input.
 *
 * Value: 0x02004123
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DRIVER_NOTFOUND                      DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x23)

/**
 * @ingroup los_driverbase
 * Task error code: Device already attach to driver.
 *
 * Value: 0x02004130
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DEVICE_BOUNDED                       DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x30)

/**
 * @ingroup los_driverbase
 * Task error code: Invalid input.
 *
 * Value: 0x02004131
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DEVICE_INITIALFAIL                   DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x31)

/**
 * @ingroup los_driverbase
 * Task error code: Device register twice.
 *
 * Value: 0x02004132
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DEVICE_REGISTERED                    DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x32)

/**
 * @ingroup los_driverbase
 * Task error code: Device busy.
 *
 * Value: 0x02004133
 *
 * Solution: Check the Input.
 */
#define DPAL_ERRNO_DRIVER_DEVICE_BUSY                          DPAL_ERRNO_OS_ERROR(DPAL_MOD_DRIVER, 0x33)

/* driver api */
/**
 * @ingroup los_driverbase
 * @brief register a new driver to the system.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to register a driver to the system.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The param probe function is applyed after attach the device to the driver.</li>
 * <li>The param remove function is used when device or driver is deleted.</li>
 * </ul>
 *
 * @param  drv      [IN]A pointer to dpal_driver.
 *
 * @retval #DPAL_ERRNO_DRIVER_INPUT_INVALID        Invalid input.drv/drv->name can not be NULL.
 * @retval #DPAL_ERRNO_DRIVER_DRIVER_REGISTERED    Invalid input.drv node register twice.
 * @retval #DPAL_ERRNO_DRIVER_MUX_FAIL             Mux creat failed.
 * @retval #DPAL_OK                                The driver register success.
 * <ul><li>los_driverbase.h: the header file that contains the API declaration.</li></ul>
 */
uint32_t dpal_driver_register(struct dpal_driver *drv);

/**
 * @ingroup los_driverbase
 * @brief unregister a driver from the system.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to unregister a driver from the system.</li>
 * </ul>
 *
 * @param  drv      [IN]A pointer to device_driver.drv/drv->name can not be NULL.
 *
 * @retval #DPAL_ERRNO_DRIVER_INPUT_INVALID        Invalid input.drv and drv.driver->name can not be NULL.
 * @retval #DPAL_ERRNO_DRIVER_DRIVER_NOTFOUND      Drvier not found.
 * @retval #DPAL_ERRNO_DRIVER_DEVICE_BUSY          Device busy.
 * @retval #DPAL_OK                                The driver unregister success.
 *
 * <ul><li>los_driverbase.h: the header file that contains the API declaration.</li></ul>
 */
uint32_t dpal_driver_unregister(struct dpal_driver *drv);

/* device api */
/**
 * @ingroup los_driverbase
 * @brief register a new device to the system.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to register a device to the system.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The same dev node can not be registered twice.</li>
 * </ul>
 *
 * @param  dev      [IN]A pointer to device.
 *
 * @retval #LOS_ERRNO_DRIVER_INPUT_INVALID        Invalid input.dev/dev->name can not be NULL.
 * @retval #LOS_ERRNO_DRIVER_DEVICE_REGISTERED    Device register twice.
 * @retval #LOS_OK                                The device register success.
 * <ul><li>los_driverbase.h: the header file that contains the API declaration.</li></ul>
 */
uint32_t dpal_device_register(struct dpal_device *dev);

/**
 * @ingroup los_driverbase
 * @brief unregister a device from the system.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to unregister a device from the system.</li>
 * </ul>
 * @param  dev      [IN]A pointer to device.dev/dev->name can not be NULL.
 *
 * <ul><li>los_driverbase.h: the header file that contains the API declaration.</li></ul>
 */
void dpal_device_unregister(struct dpal_device *dev);

static inline bool dpal_device_match_driver(struct dpal_device *dev, struct dpal_driver *drv)
{
    if (dev == NULL || dev->name == NULL || drv == NULL || drv->name == NULL) {
        return false;
    }
    return (strncmp(dev->name, drv->name, strlen(dev->name)) == 0);
}

static inline void dpal_device_bind_driver(struct dpal_device *dev, struct dpal_driver *drv)
{
    if (dev == NULL || drv == NULL) {
        return;
    }
    DRIVER_LOCK(drv);
    dpal_list_tail_insert(&drv->device_list, &dev->driver_node);
    DRIVER_UNLOCK(drv);
}

uint32_t dpal_driver_base_init(void);
uint32_t dpal_driver_attach_device(struct dpal_driver *drv, struct dpal_device *dev);
uint32_t dpal_driver_detach_device(struct dpal_driver *drv, struct dpal_device *dev);

#endif
