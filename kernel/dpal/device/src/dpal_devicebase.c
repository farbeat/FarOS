/* ---------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Driver Framework Driver Management
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

#include "dpal_driverbase.h"
#include "dpal.h"
#include "dpal_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

DPAL_DL_LIST_HEAD(g_device_pm_list);

static uint32_t dpal_device_init(struct dpal_device *dev)
{
    uint32_t mux_handle;
    uint32_t ret = dpal_mux_create(&mux_handle);
    if (ret != DPAL_OK) {
        return DPAL_ERRNO_DRIVER_DEVICE_INITIALFAIL;
    }
    dev->mutex = mux_handle;
    dev->drv = NULL;

    return DPAL_OK;
}

static void dpal_device_deinit(const struct dpal_device *dev)
{
    (void)dpal_mux_delete(dev->mutex);
}

static inline void dpal_add_device(struct dpal_device *dev)
{
    DRIVER_BASE_LOCK(*get_driver_base_mutex());
    dpal_list_tail_insert(get_device_list(), &dev->device_item);
    DRIVER_BASE_UNLOCK(*get_driver_base_mutex());

    dev->is_registered = true;
}

static void dpal_device_pm_add(struct dpal_device *dev)
{
    PM_LOCK(*get_pm_list_mutex());
    dpal_list_tail_insert(&g_device_pm_list, &(dev->pm_node));
    PM_UNLOCK(*get_pm_list_mutex());
}

static uint32_t dpal_device_attach(struct dpal_device *dev)
{
    uint32_t ret = DPAL_OK;
    struct dpal_driver *drv = NULL;

    DEVICE_LOCK(dev);

    if (dev->drv == NULL) {
        DPAL_DL_LIST_FOR_EACH_ENTRY(drv, get_driver_list(), struct dpal_driver, driver_item) {
            if (dpal_device_match_driver(dev, drv)) {
                ret = dpal_driver_attach_device(drv, dev);
            }
        }
    }

    DEVICE_UNLOCK(dev);
    return ret;
}

static uint32_t dpal_device_add(struct dpal_device *dev)
{
    dpal_add_device(dev);
    dpal_device_pm_add(dev);

    /*
     * When device is registering ahead of the matched driver registration,
     * the matched driver would not be found. It should return ok to let the
     * registration continue, and the attaching can take place when the
     * the matched driver is registered.
     */
    return dpal_device_attach(dev);
}

uint32_t dpal_device_register(struct dpal_device *dev)
{
    if (dev == NULL || dev->name == NULL) {
        return DPAL_ERRNO_DRIVER_INPUT_INVALID;
    }

    if (dev->is_registered) {
        return DPAL_ERRNO_DRIVER_DEVICE_REGISTERED;
    }

    uint32_t ret = dpal_device_init(dev);
    if (ret != DPAL_OK) {
        goto OUT;
    }
    ret = dpal_device_add(dev);
    if (ret != DPAL_OK) {
        goto OUT_DEINIT;
    }

    return ret;

OUT_DEINIT:
    dpal_device_deinit(dev);
OUT:
    return ret;
}
/********************device remove**********************/
static void dpal_device_detach(struct dpal_device *dev)
{
    DEVICE_LOCK(dev);

    if (dev->drv != NULL) {
        (void)dpal_driver_detach_device(dev->drv, dev);
    }

    DEVICE_UNLOCK(dev);
}

static void dpal_device_pm_remove(struct dpal_device *dev)
{
    PM_LOCK(*get_pm_list_mutex());
    dpal_list_deinit(&(dev->pm_node));
    PM_UNLOCK(*get_pm_list_mutex());
}

static inline void dpal_remove_device(struct dpal_device *dev)
{
    DRIVER_BASE_LOCK(*get_driver_base_mutex());
    dpal_list_delete(&dev->device_item);
    DRIVER_BASE_UNLOCK(*get_driver_base_mutex());

    dev->is_registered = FALSE;
}

static void dpal_device_delete(struct dpal_device *dev)
{
    dpal_device_detach(dev);

    dpal_device_pm_remove(dev);
    dpal_remove_device(dev);
}

void dpal_device_unregister(struct dpal_device *dev)
{
    if (dev == NULL || !dev->is_registered) {
        return;
    }

    dpal_device_delete(dev);
    dpal_device_deinit(dev);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
