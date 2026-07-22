/*----------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/

#include "dpal_driver.h"
#include "dpal_driverbase.h"
#include "dpal_errno.h"
#include "dpal_list.h"

DPAL_DL_LIST_HEAD(g_device_list);
DPAL_DL_LIST_HEAD(g_driver_list);
uint32_t g_driver_base_mutex;
uint32_t g_pm_list_mutex;
dpal_dl_list_t *get_device_list(void)
{
    return &g_device_list;
}
dpal_dl_list_t *get_driver_list(void)
{
    return &g_driver_list;
}
uint32_t *get_driver_base_mutex(void)
{
    return &g_driver_base_mutex;
}
uint32_t *get_pm_list_mutex(void)
{
    return &g_pm_list_mutex;
}

#define PLATDEV_GET_DEV(dev)        (&(dev)->dev)
#define PLATDRV_GET_DRV(drv)        (&(drv)->drv)

#define DEV_GET_DPALDEVICE(dev)      (&(dev)->dpal_dev)
#define DRV_GET_DPALDRIVER(drv)      (&(drv)->dpal_drv)

#if __GNUC__ > 3
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t)((char *)&(((type *)0)->member) - (char *)0))
#endif

#define container_of(p, t, m) ((t*)((char *)(p) - offsetof(t, m)))

#define to_platform_driver(x) (container_of((x), struct dpal_platform_driver, drv))
#define to_platform_device(x) container_of((x), struct dpal_platform_device, dev)

/* convertion from dpal device/driver to device/device_driver */
#define DPALDEVICE_TO_DEV(dev)       container_of((dev), struct device_dpal, dpal_dev)
#define DPALDRIVER_TO_DRV(drv)       container_of((drv), struct dpal_device_driver, dpal_drv)

/* convertion from dpal device/driver to plattform device/device */
#define DPALDEVICE_2_PLATDEV(dev)    to_platform_device(DPALDEVICE_TO_DEV(dev))
#define DPALDRIVER_2_PLATDRV(drv)    to_platform_driver(DPALDRIVER_TO_DRV(drv))

static int32_t driver_map_error(uint32_t err)
{
    int32_t map_error = 0;

    switch (err) {
        case DPAL_ERRNO_DRIVER_INPUT_INVALID:
        case DPAL_ERRNO_DRIVER_DRIVER_NOTFOUND:
            map_error = -EINVAL;
            break;
        case DPAL_ERRNO_DRIVER_DRIVER_REGISTERED:
        case DPAL_ERRNO_DRIVER_DEVICE_REGISTERED:
        case DPAL_ERRNO_DRIVER_DEVICE_BUSY:
            map_error = -EBUSY;
            break;
        case DPAL_ERRNO_DRIVER_MUX_FAIL:
        case DPAL_ERRNO_DRIVER_DEVICE_INITIALFAIL:
            map_error = -ENOMEM;
            break;
        case DPAL_ERRNO_DRIVER_DRIVER_PROBE_FAIL:
            map_error = -EAGAIN;
            break;
        default:
            DPAL_PRINT_ERR("invalid option\n");
            map_error = -EINVAL;
            break;
    }

    return map_error;
}

static int32_t platform_drv_probe(struct dpal_device *dev)
{
    if (dev == NULL || dev->drv == NULL) {
        return DPAL_RET_FAILURE;
    }

    struct dpal_platform_driver *platform_drv = DPALDRIVER_2_PLATDRV(dev->drv);
    struct dpal_platform_device *platform_dev = DPALDEVICE_2_PLATDEV(dev);

    if (platform_drv->probe == NULL) {
        return DPAL_RET_FAILURE;
    }

    return platform_drv->probe(platform_dev);
}

static int32_t platform_drv_remove(struct dpal_device *dev)
{
    if (dev == NULL || dev->drv == NULL) {
        return DPAL_RET_FAILURE;
    }

    struct dpal_platform_driver *platform_drv = DPALDRIVER_2_PLATDRV(dev->drv);
    struct dpal_platform_device *platform_dev = DPALDEVICE_2_PLATDEV(dev);

    if (platform_drv->remove == NULL) {
        return DPAL_RET_FAILURE;
    }

    return platform_drv->remove(platform_dev);
}

int32_t dpal_plat_driver_register(struct dpal_platform_driver *drv)
{
    if (drv == NULL) {
        return -EINVAL;
    }

    struct dpal_device_driver *dev_drv = PLATDRV_GET_DRV(drv);
    struct dpal_driver *dpal_drv = DRV_GET_DPALDRIVER(dev_drv);

    dpal_drv->name = dev_drv->name;
    dpal_drv->ops.probe = platform_drv_probe;

    uint32_t ret = dpal_driver_register(dpal_drv);
    if (ret != DPAL_OK) {
        return driver_map_error(ret);
    }

    return DPAL_OK;
}

int32_t dpal_plat_driver_unregister(struct dpal_platform_driver *drv)
{
    if (drv == NULL) {
        return -EINVAL;
    }

    struct dpal_device_driver *dev_drv = NULL;
    struct dpal_driver *dpal_drv = NULL;

    dev_drv = PLATDRV_GET_DRV(drv);
    dpal_drv = DRV_GET_DPALDRIVER(dev_drv);

    dpal_drv->ops.remove = platform_drv_remove;

    uint32_t ret = dpal_driver_unregister(dpal_drv);
    if (ret != DPAL_OK) {
        return driver_map_error(ret);
    }

    return DPAL_OK;
}

int32_t dpal_plat_device_register(struct dpal_platform_device *dev)
{
    if (dev == NULL || dev->name == NULL) {
        return -EINVAL;
    }

    struct device_dpal *device = PLATDEV_GET_DEV(dev);
    struct dpal_device *dpal_dev = DEV_GET_DPALDEVICE(device);

    /* set dpal_dev name to make LiteOS device work */
    dpal_dev->name = dev->name;
    device->name = dev->name;

    uint32_t ret = dpal_device_register(dpal_dev);
    if (ret != DPAL_OK) {
        return driver_map_error(ret);
    }

    return DPAL_OK;
}

VOID dpal_plat_device_unregister(struct dpal_platform_device *dev)
{
    if (dev != NULL) {
        dpal_device_unregister(DEV_GET_DPALDEVICE(&dev->dev));
    }
}

dpal_resource_t *dpal_plat_get_resource(struct dpal_platform_device *dev, uint32_t type, uint32_t num)
{
    if (dev == NULL || dev->resource == NULL) {
        DPAL_PRINT_ERR("platform_get_resource :the input in invalid\n");
        return NULL;
    }

    uint32_t i;
    uint32_t j;
    int32_t found = 0;
    for (i = 0, j = 0; i < dev->num_resources; i++) {
        if ((dev->resource[i].flags & type) != 0) {
            j++;
        }

        if (j == (num + 1)) {
            found = 1;
            break;
        }
    }

    if (found != 0) {
        return &dev->resource[i];
    } else {
        DPAL_PRINT_ERR("can't found the resource\n");
        return NULL;
    }
}
