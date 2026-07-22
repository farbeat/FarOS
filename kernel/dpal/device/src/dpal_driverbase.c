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

static inline void dpal_device_unbind_driver(struct dpal_device *dev, struct dpal_driver *drv)
{
    dpal_list_delete(&dev->driver_node);
}

uint32_t dpal_driver_base_init(void)
{
    uint32_t ret;

    dpal_list_init(get_device_list());
    dpal_list_init(get_driver_list());

    ret = dpal_mux_create(get_driver_base_mutex());
    if (ret != DPAL_OK) {
        dprintf("g_driver_base_mutex creat failed, ret = %u\n", ret);
        return ret;
    }

    ret = dpal_mux_create(get_pm_list_mutex());
    if (ret != DPAL_OK) {
        dprintf("g_pm_list_mutex creat failed, ret = %u\n", ret);
        (void)dpal_mux_delete(*get_driver_base_mutex());
    }

    return ret;
}

uint32_t dpal_driver_attach_device(struct dpal_driver *drv, struct dpal_device *dev)
{
    if (drv == NULL || dev == NULL) {
        return DPAL_ERRNO_DRIVER_INPUT_INVALID;
    }
    dev->drv = drv;

    if (drv->ops.probe != NULL) {
        if (drv->ops.probe(dev) != DPAL_OK) {
            DPAL_PRINT_WARN("probing device %s with driver %s failed\n", dev->name, drv->name);
            dev->drv = NULL;
            return DPAL_ERRNO_DRIVER_DRIVER_PROBE_FAIL;
        }

        dpal_device_bind_driver(dev, drv);
    }

    return DPAL_OK;
}

uint32_t dpal_driver_detach_device(struct dpal_driver *drv, struct dpal_device *dev)
{
    if (drv == NULL || dev == NULL) {
        return DPAL_ERRNO_DRIVER_INPUT_INVALID;
    }
    if (drv->ops.remove != NULL) {
        if (drv->ops.remove(dev) != DPAL_OK) {
            DPAL_PRINT_WARN("remove device %s with driver %s failed\n", dev->name, drv->name);
            return DPAL_ERRNO_DRIVER_DEVICE_BUSY;
        }
    }

    dpal_device_unbind_driver(dev, drv);
    dev->drv = NULL;

    return DPAL_OK;
}

static inline void dpal_add_driver(struct dpal_driver *drv)
{
    DRIVER_BASE_LOCK(*get_driver_base_mutex());
    dpal_list_tail_insert(get_driver_list(), &drv->driver_item);
    DRIVER_BASE_UNLOCK(*get_driver_base_mutex());

    drv->is_registered = true;
}

static inline void dpal_remove_driver(struct dpal_driver *drv)
{
    DRIVER_BASE_LOCK(*get_driver_base_mutex());
    dpal_list_delete(&drv->driver_item);
    DRIVER_BASE_UNLOCK(*get_driver_base_mutex());

    drv->is_registered = false;
}

static uint32_t dpal_driver_attach(struct dpal_driver *drv)
{
    uint32_t ret = DPAL_OK;
    struct dpal_device *dev = NULL;

    DPAL_DL_LIST_FOR_EACH_ENTRY(dev, get_device_list(), struct dpal_device, device_item) {
        if (dpal_device_match_driver(dev, drv) && (dev->drv == NULL)) {
            ret = dpal_driver_attach_device(drv, dev);
        }
    }

    return ret;
}

static uint32_t dpal_driver_detach(struct dpal_driver *drv)
{
    struct dpal_device *dev = NULL;
    dpal_dl_list_t *prev = NULL;
    uint32_t ret = DPAL_OK;

    DRIVER_LOCK(drv);

    DPAL_DL_LIST_FOR_EACH_ENTRY(dev, &(drv->device_list), struct dpal_device, driver_node) {
        prev = (dev->driver_node).prev;
        ret = dpal_driver_detach_device(drv, dev);
        if (ret != DPAL_OK) {
            DRIVER_UNLOCK(drv);
            return ret;
        }
        dev = DPAL_DL_LIST_ENTRY(prev, struct dpal_device, driver_node);
    }

    DRIVER_UNLOCK(drv);

    return ret;
}

uint32_t dpal_driver_register(struct dpal_driver *drv)
{
    uint32_t ret;

    if (drv == NULL || drv->name == NULL) {
        return DPAL_ERRNO_DRIVER_INPUT_INVALID;
    }

    if (drv->is_registered) {
        return DPAL_ERRNO_DRIVER_DRIVER_REGISTERED;
    }

    dpal_list_init(&drv->device_list);
    ret = dpal_mux_create(&drv->mutex);
    if (ret != DPAL_OK) {
        return DPAL_ERRNO_DRIVER_MUX_FAIL;
    }

    dpal_add_driver(drv);

    ret = dpal_driver_attach(drv);
    if (ret != DPAL_OK) {
        dpal_remove_driver(drv);
        (void)dpal_mux_delete(drv->mutex);
        return ret;
    }

    return DPAL_OK;
}

uint32_t dpal_driver_unregister(struct dpal_driver *drv)
{
    uint32_t ret;

    if ((drv == NULL) || (drv->name == NULL)) {
        return DPAL_ERRNO_DRIVER_INPUT_INVALID;
    }

    if (!drv->is_registered) {
        return DPAL_ERRNO_DRIVER_DRIVER_NOTFOUND;
    }

    ret = dpal_driver_detach(drv);
    if (ret != DPAL_OK) {
        return ret;
    }

    dpal_remove_driver(drv);

    (void)dpal_mux_delete(drv->mutex);

    return DPAL_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
