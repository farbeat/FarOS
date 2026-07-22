/* ---------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Driver Framework Platform Management
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "linux/platform_device.h"
#include "linux/device.h"
#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/pm.h"
#include "linux/suspend.h"
#include "los_hwi.h"

#define PLATDEV_GET_DEV(dev)        (&(dev)->dev)
#define PLATDRV_GET_DRV(drv)        (&(drv)->driver)

#define DEV_GET_LOSDEVICE(dev)      (&(dev)->losdevice)
#define DRV_GET_LOSDRIVER(drv)      (&(drv)->losdriver)

/* convertion from LiteOS device/driver to device/device_driver */
#define LOSDEVICE_TO_DEV(dev)       container_of((dev), struct device, losdevice)
#define LOSDRIVER_TO_DRV(drv)       container_of((drv), struct device_driver, losdriver)

/* convertion from LiteOS device/driver to plattform device/device */
#define LOSDEVICE_2_PLATDEV(dev)    to_platform_device(LOSDEVICE_TO_DEV(dev))
#define LOSDRIVER_2_PLATDRV(drv)    to_platform_driver(LOSDRIVER_TO_DRV(drv))

STATIC INT32 DriverMapError(UINT32 err)
{
    INT32 mapError = 0;

    switch (err) {
        case LOS_ERRNO_DRIVER_INPUT_INVALID:
        case LOS_ERRNO_DRIVER_DRIVER_NOTFOUND:
            mapError = -EINVAL;
            break;
        case LOS_ERRNO_DRIVER_DRIVER_REGISTERED:
        case LOS_ERRNO_DRIVER_DEVICE_REGISTERED:
        case LOS_ERRNO_DRIVER_DEVICE_BUSY:
            mapError = -EBUSY;
            break;
        case LOS_ERRNO_DRIVER_MUX_FAIL:
        case LOS_ERRNO_DRIVER_DEVICE_INITIALFAIL:
            mapError = -ENOMEM;
            break;
        case LOS_ERRNO_DRIVER_DRIVER_PROBE_FAIL:
            mapError = -EAGAIN;
            break;
        default:
            PRINT_ERR("invalid option\n");
            mapError = -EINVAL;
            break;
    }

    return mapError;
}

STATIC INT32 PlatformDrvProbe(struct LosDevice *losDevice)
{
    INT32 ret = LOS_OK;
    struct platform_driver *platformDrv = NULL;
    struct platform_device *platformDev = NULL;

    if ((losDevice == NULL) || (losDevice->driver == NULL)) {
        return -EINVAL;
    }

    platformDrv = LOSDRIVER_2_PLATDRV(losDevice->driver);
    platformDev = LOSDEVICE_2_PLATDEV(losDevice);

    if (platformDrv->probe != NULL) {
        ret = platformDrv->probe(platformDev);
    }

    return ret;
}

STATIC INT32 PlatformDrvRemove(struct LosDevice *losDevice)
{
    INT32 ret = LOS_OK;
    struct platform_driver *platformDrv = NULL;
    struct platform_device *platformDev = NULL;

    if ((losDevice == NULL) || (losDevice->driver == NULL)) {
        return -EINVAL;
    }

    platformDrv = LOSDRIVER_2_PLATDRV(losDevice->driver);
    platformDev = LOSDEVICE_2_PLATDEV(losDevice);

    if (platformDrv->remove != NULL) {
        ret = platformDrv->remove(platformDev);
    }

    return ret;
}

INT32 platform_driver_register(struct platform_driver *drv)
{
    UINT32 ret;
    struct device_driver *driver = NULL;
    struct LosDriver *losDriver = NULL;

    if (drv == NULL) {
        return -EINVAL;
    }

    driver = PLATDRV_GET_DRV(drv);
    losDriver = DRV_GET_LOSDRIVER(driver);

    losDriver->name = driver->name;
    losDriver->ops.probe = PlatformDrvProbe;

    ret = LOS_DriverRegister(losDriver);
    if (ret != LOS_OK) {
        return DriverMapError(ret);
    }

    return 0;
}

INT32 platform_driver_unregister(struct platform_driver *drv)
{
    UINT32 ret;
    struct device_driver *driver = NULL;
    struct LosDriver *losDriver = NULL;

    if (drv == NULL) {
        return -EINVAL;
    }

    driver = PLATDRV_GET_DRV(drv);
    losDriver = DRV_GET_LOSDRIVER(driver);

    losDriver->ops.remove = PlatformDrvRemove;

    ret = LOS_DriverUnregister(losDriver);
    if (ret != LOS_OK) {
        return DriverMapError(ret);
    }

    return 0;
}

INT32 platform_device_register(struct platform_device *dev)
{
    UINT32 ret;
    struct device *device = NULL;
    struct LosDevice *losDevice = NULL;

    if ((dev == NULL) || (dev->name == NULL)) {
        return -EINVAL;
    }

    device = PLATDEV_GET_DEV(dev);
    losDevice = DEV_GET_LOSDEVICE(device);

    /* set losDevice name to make LiteOS device work */
    losDevice->name = dev->name;
    device->name = dev->name;

    ret = LOS_DeviceRegister(losDevice);
    if (ret != LOS_OK) {
        return DriverMapError(ret);
    }

    return 0;
}

VOID platform_device_unregister(struct platform_device *dev)
{
    if (dev != NULL) {
        LOS_DeviceUnregister(DEV_GET_LOSDEVICE(&dev->dev));
    }
}

struct resource *platform_get_resource(struct platform_device *dev,
                                       UINT32 type, unsigned int num)
{
    UINT32 i, j;
    INT32 found = 0;

    if ((dev == NULL) || (dev->resource == NULL)) {
        PRINT_ERR("platform_get_resource :the input in invalid\n");
        return NULL;
    }

    for (i = 0, j = 0; i < dev->num_resources; i++) {
        if (dev->resource[i].flags & type) {
            j++;
        }

        if (j == (num + 1)) {
            found = 1;
            break;
        }
    }

    if (found == 1) {
        return &dev->resource[i];
    } else {
        PRINT_ERR("can't found the resource\n");
        return NULL;
    }
}

long platform_get_irq(struct platform_device *dev, unsigned int num)
{
    struct resource *res = NULL;

    res = platform_get_resource(dev, IORESOURCE_IRQ, num);
    if (res == NULL) {
        return -1;
    }

    return (long)res->start;
}

void *platform_ioremap_resource(struct resource *res)
{
    if (res == NULL) {
        PRINT_ERR("platform_ioremap_resource :the input in invalid\n");
        return NULL;
    }
    return (void *)res->start;
}

int pm_suspend(suspend_state_t state)
{
    (VOID)state;
    PRINT_ERR("%s is not supported\n", __FUNCTION__);
    return -EINVAL;
}

void pm_resume(void)
{
    PRINT_ERR("%s is not supported\n", __FUNCTION__);
}

int dpm_suspend_start(dev_pm_message_t state)
{
    (VOID)state;
    PRINT_ERR("%s is not supported\n", __FUNCTION__);
    return -EINVAL;
}

void dpm_resume_end(dev_pm_message_t state)
{
    (VOID)state;
    PRINT_ERR("%s is not supported\n", __FUNCTION__);
}
