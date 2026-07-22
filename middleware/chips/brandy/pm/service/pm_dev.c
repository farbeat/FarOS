/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm dev manage code
 */

#include "pm_dev.h"
#include "pm_os.h"
#include "pm_core.h"

#define ops_(a, b) ((a) ? (a) : (b))

typedef struct pm_dev_list pm_dev_node_t;
struct pm_dev_list {
    pm_dev_node_t *pre;
    pm_dev_node_t *next;
    pm_dev_id_t dev_id;
    pm_dev_ops_t dev_ops;
};

pm_dev_node_t *g_pm_dev_list = NULL;

#ifdef PM_DEBUG
uint32_t g_dev_debug_ret = 0;
uint32_t g_dev_debug_dev_id = 0;
#endif

static pm_dev_ops_t *pm_get_dev_ops(pm_dev_id_t dev_id)
{
    if (dev_id >= PM_DEV_MAX) {
        return NULL;
    }

    pm_dev_node_t *dev_list = g_pm_dev_list;
    while (dev_list != NULL) {
        if (dev_list->dev_id == dev_id) {
            return (pm_dev_ops_t *)&(dev_list->dev_ops);
        }
        dev_list = dev_list->next;
    }
    return NULL;
}

errcode_t uapi_pm_register_dev_ops(pm_dev_id_t dev_id, pm_dev_ops_t *dev_ops)
{
    if (dev_id >= PM_DEV_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    pm_dev_node_t *dev_list = g_pm_dev_list;
    while (dev_list != NULL) {
        if (dev_list->dev_id == dev_id) {
            break;
        }

        if (dev_list->next == NULL) {
            break;
        }
        dev_list = dev_list->next;
    }

    if (dev_list != NULL && dev_list->dev_id == dev_id) {
        dev_list->dev_ops.suspend = ops_(dev_ops->suspend, dev_list->dev_ops.suspend);
        dev_list->dev_ops.resume = ops_(dev_ops->resume, dev_list->dev_ops.resume);
        dev_list->dev_ops.ioctl = ops_(dev_ops->ioctl, dev_list->dev_ops.ioctl);
    } else {
        pm_dev_node_t *dev_node = pm_os_malloc(sizeof(pm_dev_node_t));
        if (dev_node == NULL) {
            return ERRCODE_PM_MEM_ALLOC_ERROR;
        }
        dev_node->dev_id = dev_id;
        dev_node->pre = NULL;
        dev_node->next = NULL;

        (void)memcpy_s(&dev_node->dev_ops, sizeof(pm_dev_ops_t), dev_ops, sizeof(pm_dev_ops_t));
        if (dev_list == NULL) {
            g_pm_dev_list = dev_node;
        } else {
            dev_node->pre = dev_list;
            dev_list->next = dev_node;
        }
    }
    return ERRCODE_SUCC;
}

errcode_t pm_unregister_dev_ops(pm_dev_id_t dev_id)
{
    if (dev_id >= PM_DEV_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }
    pm_dev_node_t *dev_list = g_pm_dev_list;

    while (dev_list != NULL) {
        if (dev_list->dev_id == dev_id) {
            dev_list->pre->next = dev_list->next;
            pm_os_free(dev_list);
            break;
        }
        dev_list = dev_list->next;
    }
    return ERRCODE_SUCC;
}

errcode_t pm_dev_control(pm_dev_id_t dev_id, uint8_t type, uint8_t *data, uint8_t len)
{
    pm_dev_ops_t *ops = pm_get_dev_ops(dev_id);
    if (ops != NULL && ops->ioctl != NULL) {
        (void)ops->ioctl(type, data, len);
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_PM_ERROR;
    }
}

errcode_t pm_dev_suspend(void)
{
    uint32_t ret = 0;
    errcode_t pm_ret = ERRCODE_SUCC;
    pm_dev_node_t *dev_list = g_pm_dev_list;
    while (dev_list != NULL) {
        if (dev_list->dev_ops.suspend != NULL) {
            ret = dev_list->dev_ops.suspend();
            if (ret != 0) {
#ifdef PM_DEBUG
                g_dev_debug_ret = ret;
                g_dev_debug_dev_id = dev_list->dev_id;
#endif
                pm_ret = ERRCODE_PM_ERROR;
                break;
            }
        }
        dev_list = dev_list->next;
    }

    if (pm_ret == ERRCODE_SUCC) {
        return ERRCODE_SUCC;
    }

    while (dev_list != NULL) {
        if (dev_list->dev_ops.resume != NULL) {
            dev_list->dev_ops.resume();
        }
        dev_list = dev_list->pre;
    }
    return pm_ret;
}

void pm_dev_resume(void)
{
    pm_dev_node_t *dev_list = g_pm_dev_list;
    while (dev_list != NULL) {
        if (dev_list->dev_ops.resume != NULL) {
            dev_list->dev_ops.resume();
        }
        dev_list = dev_list->next;
    }
}
