/*
 * Copyright (c) CompanyNameMagicTag 2013-2020. All rights reserved.
 * Description: OS Abstract Layer.
 * Author: audio
 */

#ifndef __DRV_AUDIO_OSAL_H__
#define __DRV_AUDIO_OSAL_H__

#include "td_type.h"
#include "soc_osal.h"

#define drv_audio_osal_vmalloc(size) osal_vmalloc((size))
#define drv_audio_osal_vfree(addr) osal_vfree((addr))

#define drv_audio_osal_copy_from_user osal_copy_from_user
#define drv_audio_osal_copy_to_user osal_copy_to_user

#define drv_audio_osal_max(x, y) osal_max((x), (y))
#define drv_audio_osal_min(x, y) osal_min((x), (y))

#define drv_audio_osal_abs(x) osal_abs((x))

/* proc */
#define DRV_AUDIO_OSAL_PROC_NAME_LENGTH 32
#define DRV_AUDIO_OSAL_PROC_CMD_SINGEL_LENGTH_MAX 32

struct drv_audio_osal_list_head {
    struct drv_audio_osal_list_head *next, *prev;
};

typedef struct {
    td_char name[DRV_AUDIO_OSAL_PROC_NAME_LENGTH];
    td_s32 (*handler)(td_u32 argc, td_char (*argv)[DRV_AUDIO_OSAL_PROC_CMD_SINGEL_LENGTH_MAX], td_void *private);
} drv_audio_osal_proc_cmd;

typedef struct {
    td_char name[DRV_AUDIO_OSAL_PROC_NAME_LENGTH];
    td_s32 (*open)(td_void *private);
    td_s32 (*read)(td_void *seqfile, td_void *private);
    drv_audio_osal_proc_cmd *cmd_list;
    td_u32 cmd_cnt;
    td_void *private;
    td_void *proc_dir_entry;
    td_void *seqfile;
    struct drv_audio_osal_list_head node;
} drv_audio_osal_proc_entry;

drv_audio_osal_proc_entry *drv_audio_osal_proc_add(const td_char *name, td_ulong name_size);
td_void drv_audio_osal_proc_remove(const td_char *name, td_ulong name_size);
td_s32 drv_audio_osal_proc_print(td_void *seqfile, const td_char *fmt, ...) __attribute__((format(printf, 2, 3)));

td_s32 drv_audio_osal_proc_set_mask(td_u8 proc_mask);
td_void drv_audio_osal_proc_get_mask(td_u8 *proc_mask);

/* device api */
#define DRV_AUDIO_DEV_NAME_LEN OSAL_DEV_NAME_LEN
typedef osal_poll drv_audio_osal_poll;
typedef osal_ioctl_cmd drv_audio_osal_ioctl_cmd;
typedef osal_vm drv_audio_osal_vm;
typedef osal_fileops drv_audio_osal_fileops;
typedef osal_pmops drv_audio_osal_pmops;
typedef osal_dev drv_audio_osal_dev;

#define drv_audio_osal_dev_register(dev) osal_dev_register((osal_dev *)(dev))
#define drv_audio_osal_dev_unregister(dev) osal_dev_unregister((osal_dev *)(dev))

// kfile
#define DRV_AUDIO_OSAL_O_ACCMODE 00000003
#define DRV_AUDIO_OSAL_O_RDONLY 00000000
#define DRV_AUDIO_OSAL_O_WRONLY 00000001
#define DRV_AUDIO_OSAL_O_RDWR 00000002
#define DRV_AUDIO_OSAL_O_CREAT 00000100
#define DRV_AUDIO_OSAL_O_APPEND 00002000
#define DRV_AUDIO_OSAL_O_TRUNC 00001000
#define DRV_AUDIO_OSAL_O_CLOEXEC 02000000

#define DRV_AUDIO_OSAL_SEEK_SET 0
#define DRV_AUDIO_OSAL_SEEK_CUR 1
#define DRV_AUDIO_OSAL_SEEK_END 2

#define drv_audio_osal_klib_fopen osal_klib_fopen
#define drv_audio_osal_klib_fclose osal_klib_fclose
#define drv_audio_osal_klib_fwrite osal_klib_fwrite
#define drv_audio_osal_klib_fread osal_klib_fread
#define drv_audio_osal_klib_fseek osal_klib_fseek
#define drv_audio_osal_klib_fsync osal_klib_fsync
#define drv_audio_osal_klib_user_fwrite osal_klib_user_fwrite
#define drv_audio_osal_klib_user_fread osal_klib_user_fread
#define drv_audio_osal_klib_get_store_path osal_klib_get_store_path
#define drv_audio_osal_klib_set_store_path osal_klib_set_store_path

/* gpio */
#define drv_audio_osal_gpio_get_direction osal_gpio_get_direction
#define drv_audio_osal_gpio_set_direction osal_gpio_set_direction
#define drv_audio_osal_gpio_set_value osal_gpio_set_value
#define drv_audio_osal_gpio_get_value osal_gpio_get_value
#define drv_audio_osal_gpio_irq_request osal_gpio_irq_request
#define drv_audio_osal_gpio_irq_free osal_gpio_irq_free

// atomic api
typedef osal_atomic drv_audio_osal_atomic;

#define drv_audio_osal_atomic_init(atomic)
#define drv_audio_osal_atomic_destroy(atomic)
#define drv_audio_osal_atomic_inc_return(atomic)
#define drv_audio_osal_atomic_dec_return(atomic)

#define drv_audio_osal_module_param(name, type, perm)
#define DRV_AUDIO_OSAL_MODULE_PARM_DESC(_parm, desc)
#define drv_audio_osal_module_init(x)
#define drv_audio_osal_module_exit(x)
#define DRV_AUDIO_OSAL_MODULE_AUTHOR(_author)
#define DRV_AUDIO_OSAL_MODULE_DESCRIPTION(_description)
#define DRV_AUDIO_OSAL_MODULE_LICENSE(_license)
#define DRV_AUDIO_OSAL_MODULE_VERSION(_version)

#define drv_audio_osal_random_get_random_bytes osal_random_get_random_bytes

#endif /* __DRV_AUDIO_OSAL_H__ */
