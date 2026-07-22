/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: device
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#ifdef LOSCFG_FS_VFS
#include <fs/fs.h>
#endif
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include "soc_osal.h"

#ifdef LOSCFG_FS_VFS
#include "fs_poll.h"

typedef struct osal_fileops_info_ {
    void *private_data;
    osal_fileops *fops;
} osal_fileops_info;

static int osal_open(struct file *file)
{
    int ret = 0;
    osal_fileops *fops = NULL;
    osal_dev *dev = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL || file->f_inode == NULL) {
        osal_log("file invalid\n");
        return -1;
    }
    dev = (osal_dev *)file->f_inode->i_private;
    if (dev == NULL || dev->fops == NULL || dev->fops->open == NULL) {
        osal_log("invalid param!\n");
        return -1;
    }
    fops = dev->fops;

    fileops_node = (osal_fileops_info *)osal_kmalloc(sizeof(osal_fileops_info), OSAL_GFP_KERNEL);
    if (fileops_node == NULL) {
        return -1;
    }

    ret = fops->open((void *)&(fileops_node->private_data));
    if (ret != 0) {
        osal_kfree(fileops_node);
        return ret;
    }

    fileops_node->fops = fops;
    file->f_priv = fileops_node;

    return ret;
}

static ssize_t osal_read(struct file *file, char *buf, size_t size)
{
    int ret = 0;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL) {
        osal_log("file invalid\n");
        return -1;
    }
    fileops_node = file->f_priv;
    if (fileops_node == NULL) {
        return -1;
    }

    fops = fileops_node->fops;
    if (fops == NULL || fops->read == NULL) {
        return -1;
    }

    ret = fops->read(buf, (int)size, (long *)&(file->f_pos), (void *)&(fileops_node->private_data));
    return ret;
}

static ssize_t osal_write(struct file *file, const char __user *buf, size_t size)
{
    return 0;
}

static off_t osal_llseek(struct file *file, off_t offset, int whence)
{
    return 0;
}

static int osal_release(struct file *file)
{
    if (file == NULL) {
        osal_log("file invalid\n");
        return -1;
    }
    osal_fileops_info *fileops_node = file->f_priv;
    if (fileops_node == NULL) {
        return -1;
    }

    if (fileops_node->fops != NULL && fileops_node->fops->release != NULL) {
        (void)fileops_node->fops->release((void *)&(fileops_node->private_data));
    }

    osal_kfree(fileops_node);
    file->f_priv = NULL;

    return 0;
}

#define ARG_BUF_TEMP_SIZE 256

static int osal_ioctl_copy_from_user(unsigned int cmd, unsigned long arg,
                                     void **ioctl_arg, unsigned int arg_size, char *arg_buf)
{
    int ret = -1;

    /*  Copy arguments into temp kernel buffer  */
    switch (_IOC_DIR(cmd)) {
        case _IOC_NONE:
            *ioctl_arg = NULL;
            break;
        case _IOC_READ:
        case _IOC_WRITE:
        case (_IOC_WRITE | _IOC_READ):
            if (arg_size <= ARG_BUF_TEMP_SIZE) {
                *ioctl_arg = arg_buf;
            } else {
                /* too big to alloc from stack; <16K use osal_kmalloc */
                *ioctl_arg = osal_kmalloc(arg_size, OSAL_GFP_KERNEL);
                if (*ioctl_arg == NULL) {
                    osal_log("osal_kmalloc cmd buffer failed!\n");
                    return -ENOMEM;
                }
            }

            if ((_IOC_DIR(cmd) & _IOC_WRITE) != 0) {
                ret = copy_from_user(*ioctl_arg, (void __user *)arg, arg_size);
                if (ret != 0) {
                    osal_log("copy_from_user failed, cmd=0x%x\n", cmd);
                    return -1;
                }
            }
            break;
        default:
            osal_log("cmd %x invalid\n", _IOC_DIR(cmd));
    }

    return 0;
}

static int osal_ioctl_call_fun(unsigned int cmd, void *ioctl_arg, osal_fileops *fops, void *private_data)
{
    unsigned int i = 0;
    // call func; only used inosal_ioctl; no need check cmd, ioctl_arg, fops
    for (i = 0; i < fops->cmd_cnt; i++) {
        if (fops->cmd_list[i].cmd != cmd || fops->cmd_list[i].handler == NULL) {
            continue;
        }
        return fops->cmd_list[i].handler(cmd, ioctl_arg, private_data);
    }

    return OSAL_FAILURE;
}

static int osal_ioctl_copy_to_user(unsigned int cmd, unsigned long arg, void *ioctl_arg, unsigned int arg_size)
{
    /*  Copy results into user buffer  */
    switch (_IOC_DIR(cmd)) {
        case _IOC_READ:
        case (_IOC_WRITE | _IOC_READ): {
                unsigned long ret = (int)copy_to_user((void __user *)arg, ioctl_arg, arg_size);
                return (ret == 0) ? 0 : -1;
            }
        default:
            osal_log("cmd %x invalid\n", _IOC_DIR(cmd));
    }

    return 0;
}

static int osal_ioctl(struct file *file, int cmd, unsigned long arg)
{
    int ret;
    unsigned int arg_size;
    char arg_buf[ARG_BUF_TEMP_SIZE] = {}; // temp to prevent tiny vmalloc
    void *ioctl_arg = NULL;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;

    if (file == NULL) {
        osal_log("file invalid\n");
        return -1;
    }

    arg_size = _IOC_SIZE(cmd);

    if (_IOC_DIR(cmd) != _IOC_NONE) {
        if ((arg_size == 0) || (((char *)(UINTPTR)arg) == NULL)) {
            return -1; // arg_size max 0x4000, no need to check max
        }
    }
    fileops_node = file->f_priv;
    if (fileops_node == NULL) {
        return -1;
    }

    fops = fileops_node->fops;
    if (fops == NULL) {
        return -1;
    }

    if ((fops->cmd_cnt == 0) || (fops->cmd_list == NULL)) {
        return -1;
    }

    ret = osal_ioctl_copy_from_user(cmd, arg, &ioctl_arg, arg_size, arg_buf);
    if (ret != 0) {
        goto OUT;
    }

    ret = osal_ioctl_call_fun(cmd, ioctl_arg, fops, (void *)&(fileops_node->private_data));
    if (ret != 0) {
        goto OUT;
    }

    ret = osal_ioctl_copy_to_user(cmd, arg, ioctl_arg, arg_size);
    if (ret != 0) {
        goto OUT;
    }

OUT:
    if ((ioctl_arg != NULL) && (arg_size > ARG_BUF_TEMP_SIZE)) {
        osal_kfree(ioctl_arg);
    }

    return ret;
}

static int osal_fs_poll(struct file *file, poll_table *table)
{
    unsigned int ret = 0;
    osal_poll t;
    osal_fileops *fops = NULL;
    osal_fileops_info *fileops_node = NULL;
    if (file == NULL) {
        osal_log("file invalid\n");
        return -1;
    }
    t.poll_table = table;
    t.data = file;

    fileops_node = file->f_priv;
    if (fileops_node == NULL) {
        return -1;
    }

    fops = fileops_node->fops;
    if (fops == NULL || fops->poll == NULL) {
        return -1;
    }

    ret = fops->poll(&t, (void *)&(fileops_node->private_data));

    return ret;
}

static struct file_operations_vfs g_osal_fops = {
    .open = osal_open,
    .read = osal_read,
    .write = osal_write,
    .seek = osal_llseek,
    .ioctl = osal_ioctl,
    .close = osal_release,
    .poll = osal_fs_poll
};

static int osal_probe(struct dpal_platform_device *pfdev)
{
    char dev_name[OSAL_DEV_NAME_LEN] = {};
    int ret;
    if (pfdev == NULL) {
        osal_log("pfdev is NULL\n");
        return -1;
    }
    ret = snprintf_s(dev_name, sizeof(dev_name), sizeof(dev_name) - 1, "/dev/%s", pfdev->name);
    if (ret < 0) {
        osal_log("snprintf_s failed. ret = %#x.\n", ret);
        return -1;
    }
    ret = register_driver(dev_name, &g_osal_fops, 0, pfdev->dev.platform_data);
    if (ret != 0) {
        osal_log("register_driver failed. ret = %#x.\n", ret);
        return -1;
    }
    return 0;
}

static int osal_remove(struct dpal_platform_device *pfdev)
{
    char dev_name[OSAL_DEV_NAME_LEN] = {};
    int ret;
    if (pfdev == NULL) {
        osal_log("pfdev is NULL\n");
        return -1;
    }
    ret = snprintf_s(dev_name, sizeof(dev_name), sizeof(dev_name) - 1, "/dev/%s", pfdev->name);
    if (ret < 0) {
        osal_log("snprintf_s failed. ret = %#x.\n", ret);
        return -1;
    }
    ret = unregister_driver(dev_name);
    if (ret != 0) {
        osal_log("unregister_driver failed. ret = %#x.\n", ret);
        return -1;
    }
    return 0;
}

static int osal_pm_suspend(struct device_dpal *pdev)
{
    if (pdev == NULL) {
        return -1;
    }
    osal_dev *odev = (osal_dev *)pdev->platform_data;
    if (odev == NULL || odev->pmops == NULL) {
        return -1;
    }
    if (odev->pmops->pm_suspend != NULL) {
        return odev->pmops->pm_suspend(odev->pmops->private_data);
    }
    return -1;
}

static int osal_pm_resume(struct device_dpal *pdev)
{
    if (pdev == NULL) {
        return -1;
    }
    osal_dev *odev = (osal_dev *)pdev->platform_data;
    if (odev == NULL || odev->pmops == NULL) {
        return -1;
    }
    if (odev->pmops->pm_resume != NULL) {
        return odev->pmops->pm_resume(odev->pmops->private_data);
    }
    return -1;
}

static const struct dpal_dev_pm_op g_osal_dev_pm_ops = {
    .suspend = osal_pm_suspend,
    .resume = osal_pm_resume
};

static int osal_platform_register(osal_dev *dev, struct dpal_platform_device *pfdev, struct dpal_platform_driver *pfdrv)
{
    int ret;

    if (dev == NULL || pfdev == NULL || pfdrv == NULL) {
        osal_log("parameter invalid!\n");
        return -1;
    }

    pfdev->name = dev->name;
    pfdev->dev.platform_data = (void*)dev;

    ret = dpal_platform_device_register(pfdev);
    if (ret != 0) {
        osal_log("platform_device_register failed! ret = %#x.\n", ret);
        return -1;
    }

    pfdrv->probe = osal_probe;
    pfdrv->remove = osal_remove;
    pfdrv->drv.name = dev->name;
    pfdrv->drv.pm = &g_osal_dev_pm_ops;

    ret = dpal_platform_driver_register(pfdrv);
    if (ret != 0) {
        dpal_platform_device_unregister(pfdev);
        osal_log("platform_device_unregister failed! ret = %#x.\n", ret);
        return -1;
    }

    return 0;
}

static int osal_platform_unregister(osal_dev *dev, struct dpal_platform_device *pfdev,
    struct dpal_platform_driver *pfdrv)
{
    int ret;

    pfdrv->probe = osal_probe;
    pfdrv->remove = osal_remove;
    pfdrv->drv.name = dev->name;
    pfdrv->drv.pm = &g_osal_dev_pm_ops;
    ret = dpal_platform_driver_unregister(pfdrv);
    if (ret != 0) {
        osal_log("platform_drive_unregister failed! ret = %#x.\n", ret);
        return -1;
    }

    pfdev->name = dev->name;
    pfdev->dev.platform_data = (void*)dev;
    dpal_platform_device_unregister(pfdev);

    return 0;
}

typedef struct osal_dev_info_ {
    struct dpal_platform_device pfdev;
    struct dpal_platform_driver pfdrv;
} osal_dev_info;

osal_dev *osal_dev_create(const char *name)
{
    osal_dev *dev = NULL;

    if (name == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }
    dev = osal_kmalloc(sizeof(osal_dev), OSAL_GFP_KERNEL);
    if (dev == NULL) {
        osal_log("osal_kmalloc failed!\n");
        return NULL;
    }

    if (memset_s(dev, sizeof(osal_dev), 0, sizeof(osal_dev)) != EOK) {
        osal_kfree(dev);
        osal_log("memset_s failed!\n");
        return NULL;
    }
    if (strcpy_s(dev->name, sizeof(dev->name), name) != EOK) {
        osal_kfree(dev);
        osal_log("memset_s failed!\n");
        return NULL;
    }

    return dev;
}

int osal_dev_destroy(osal_dev *dev)
{
    if (dev == NULL) {
        osal_log("parameter invalid!\n");
        return -1;
    }
    osal_kfree(dev);
    return 0;
}
#endif

int osal_dev_register(osal_dev *dev)
{
#ifdef LOSCFG_FS_VFS
    int ret;
    osal_dev_info *dev_pf = NULL;
    if (dev == NULL || dev->dev != NULL) {
        osal_log("parameter invalid!\n");
        return -1;
    }

    if (dev->fops == NULL) {
        osal_log("fops parameter invalid!\n");
        return -1;
    }
    dev_pf = (osal_dev_info *)osal_kmalloc(sizeof(osal_dev_info), OSAL_GFP_KERNEL);
    if (dev_pf == NULL) {
        osal_log("osal_kmalloc failed!\n");
        return -1;
    }
    ret = memset_s(dev_pf, sizeof(osal_dev_info), 0, sizeof(osal_dev_info));
    if (ret != EOK) {
        osal_kfree(dev_pf);
        dev_pf = NULL;
        osal_log("memset_s failed! ret = %#x.\n", ret);
        return -1;
    }
    dev->dev = dev_pf;

    ret = osal_platform_register(dev, &(dev_pf->pfdev), &(dev_pf->pfdrv));
    if (ret != 0) {
        osal_kfree(dev_pf);
        return -1;
    }
#endif
    return 0;
}

void osal_dev_unregister(osal_dev *dev)
{
#ifdef LOSCFG_FS_VFS
    int ret;
    osal_dev_info *dev_pf = NULL;
    if (dev == NULL || dev->dev == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    dev_pf = (osal_dev_info *)(dev->dev);

    ret = osal_platform_unregister(dev, &(dev_pf->pfdev), &(dev_pf->pfdrv));
    if (ret != 0) {
        osal_log("osal_platform_unregister failed!\n");
    }
    osal_kfree(dev_pf);
    dev->dev = NULL;
#endif
}

#ifdef LOSCFG_FS_VFS
void osal_poll_wait(osal_poll *table, osal_wait *wait)
{
    if (table == NULL || table->data == NULL || table->poll_table == NULL) {
        osal_log("table(NULL) invalid!\n");
        return;
    }

    if (wait == NULL || wait->wait == NULL) {
        osal_log("wait(NULL) invalid!\n");
        return;
    }
    poll_wait ((struct file *)table->data, (wait_queue_head_t *)(wait->wait), table->poll_table);
}

void osal_notify_poll(osal_wait *wait)
{
    if (wait == NULL || wait->wait == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    notify_poll((wait_queue_head_t *)(wait->wait));
}
#endif

int osal_remap_pfn_range(osal_vm *vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    return 0;
}

int osal_try_to_freeze(void)
{
    return 0;
}

int osal_set_freezable(void)
{
    return 0;
}

int osal_kobject_uevent_env(osal_dev *dev, osal_kobject_action action, char *envp[])
{
    return 0;
}
