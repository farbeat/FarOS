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
 * l_device_driverl_device_driver* to endorse or promote products derived from this software
 * without specific prior written
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

#ifndef DPAL_DRIVER_H
#define DPAL_DRIVER_H

#include "dpal_typedef.h"
#include "dpal_list.h"
#include "dpal_errno.h"
#include "dpal_reg_config.h"

struct dpal_driver;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/**
 * @ingroup los_driverbase
 * LosDevice register config structure.
 *
 */
struct dpal_device_regs {
    uintptr_t base; /**< Register base address of device */
    uintptr_t size; /**< Register size of device */
};

/**
 * @ingroup los_driverbase
 * LosDevice config structure.
 *
 */
struct dpal_device_config {
    uintptr_t irq_num;             /**< Interrupt number of device */
    uint32_t num_regs;             /**< Number of device registers */
    struct dpal_device_regs *regs; /**< Device registers */
};

/**
 * @ingroup los_driverbase
 * LosDevice information structure.
 *
 */
struct dpal_device {
    const char *name;               /**< Device name */
    int32_t id;                       /**< Device id, generally like 0,1... */
    dpal_dl_list_t driver_node;     /**< Node for linking to the attached driver */
    dpal_dl_list_t device_item;     /**< Node for linking to the global device list */
    dpal_dl_list_t pm_node;         /**< Node for linking to the global power manager list */
    uint32_t mutex;                   /**< Mutex */
    bool is_registered;             /**< If device registered to the system */
    struct dpal_driver *drv;        /**< Attached driver */
    struct dpal_device_config cfg;  /**< Standard device configurations */
    void *data;                     /**< Device private data */
};

/**
 * @ingroup los_driverbase
 * LosDevice power manager operation structure.
 *
 */
struct dpal_power_mgr_ops {
    int32_t (*suspend)(struct dpal_device *dev);  /**< Suspend function of power manager ops */
    int32_t (*resume)(struct dpal_device *dev);   /**< Resume function of power manager ops */
    int32_t (*prepare)(struct dpal_device *dev);  /**< Prepare function of power manager ops */
    int32_t (*complete)(struct dpal_device *dev); /**< Complete function of power manager ops */
};

/**
 * @ingroup los_driverbase
 * LosDriver operation structure.
 *
 */
struct dpal_driver_ops {
    int32_t (*probe)(struct dpal_device *dev);   /**< Probe function of driver ops */
    int32_t (*remove)(struct dpal_device *dev);  /**< Remove function of driver ops */
    void (*shutdown)(struct dpal_device *dev); /**< Shutdown function of driver ops */
};

/**
 * @ingroup los_driverbase
 * LosDriver information structure.
 *
 */
struct dpal_driver {
    const char *name;                 /**< Driver name */
    dpal_dl_list_t device_list;       /**< List header for putting attached devices */
    dpal_dl_list_t driver_item;       /**< Node for linking to the global driver list */
    uint32_t mutex;                     /**< Mutex */
    bool is_registered;               /**< If driver registered to the system */
    struct dpal_driver_ops ops;       /**< Driver operations */
    struct dpal_power_mgr_ops pm_ops; /**< Driver power manager operations */
};

typedef struct dpal_pm_message {
    int32_t event;
} dpal_dev_pm_message_t;

struct dpal_dev_pm_info {
    dpal_dev_pm_message_t power_state;
    dpal_dl_list_t entry;
};

/* device define */
/**
 * @ingroup los_drivers
 * Define the structure of the parameters used for device creation.
 */
struct device_dpal {
    struct dpal_device dpal_dev;
    const char *name;
    void *driver_data;
    void *platform_data;            /**< Platform specific data, device core doesn't touch it */
    struct dpal_dev_pm_info power;
};

struct dpal_dev_pm_op {
    int32_t (*suspend)(struct device_dpal *dev);
    int32_t (*resume)(struct device_dpal *dev);
    int32_t (*prepare)(struct device_dpal *dev);
    int32_t (*complete)(struct device_dpal *dev);
};

/* driver define */
/**
 * @ingroup los_drivers
 * Define the structure of the parameters used for driver creation.
 */
struct dpal_device_driver {
    struct dpal_driver dpal_drv;
    const char *name;
    const struct dpal_dev_pm_op *pm;
};

typedef struct dpal_resource {
    uint32_t start;
    uint32_t end;
    const char *name;
    unsigned long flags;
    unsigned long desc;
    struct dpal_resource *parent, *sibling, *child;
} dpal_resource_t;

/**
 * @ingroup dpal_device
 * Define the structure of the parameters used for platform device creation.
 */
struct dpal_platform_device {
    const char *name;
    struct device_dpal dev;
    int32_t id;
    uint32_t num_resources;
    dpal_resource_t *resource;
};

/**
 * @ingroup dpal_drivers
 * Define the structure of the parameters used for platform driver creation.
 */
struct dpal_platform_driver {
    int32_t (*probe)(struct dpal_platform_device *);
    int32_t (*remove)(struct dpal_platform_device *);
    void (*shutdown)(struct dpal_platform_device *);
    int32_t (*suspend)(struct dpal_platform_device *);
    int32_t (*resume)(struct dpal_platform_device *);
    struct dpal_device_driver drv;
};

int32_t dpal_plat_driver_register(struct dpal_platform_driver *drv);
int32_t dpal_plat_driver_unregister(struct dpal_platform_driver *drv);
dpal_resource_t* dpal_plat_get_resource(struct dpal_platform_device *dev, uint32_t type, uint32_t num);
int32_t dpal_plat_device_register(struct dpal_platform_device *dev);
void dpal_plat_device_unregister(struct dpal_platform_device *dev);

dpal_dl_list_t *get_device_list(void);
dpal_dl_list_t *get_driver_list(void);
uint32_t *get_driver_base_mutex(void);
uint32_t *get_pm_list_mutex(void);
#endif
