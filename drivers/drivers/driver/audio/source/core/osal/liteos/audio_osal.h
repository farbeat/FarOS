/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 * Create: 2019-12-19
 */

#ifndef __AUDIO_OSAL_H__
#define __AUDIO_OSAL_H__

#include "cmsis_os.h"

#include "chip_type.h"
#include "audio_type.h"
#include "audio_struct.h"
#include "soc_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef osal_semaphore audio_osal_semaphore;
typedef osal_mutex audio_osal_mutex;
typedef osal_irq_handler audio_osal_irq_handler;

#define audio_osal_irq_request osal_irq_request
#define audio_osal_irq_free osal_irq_free
#define audio_osal_irq_enable osal_irq_enable
#define audio_osal_irq_disable osal_irq_disable

#define audio_osal_sema_init(sem, val) osal_sem_init((td_void *)(sem), (val))
#define audio_osal_sema_destroy(sem) osal_sem_destroy((td_void *)(sem))
#define audio_osal_sema_down(sem) osal_sem_down((td_void *)(sem))
#define audio_osal_sema_up(sem) osal_sem_up((td_void *)(sem))
#define audio_osal_sema_down_interruptible(sem) osal_sem_down_interruptible((td_void *)(sem))

#ifdef BRANDY_PROT
#define audio_osal_mutex_init(mutex) (td_void)((osal_mutex *)(mutex))
#define audio_osal_mutex_deinit(mutex) (td_void)((osal_mutex *)(mutex))
#define audio_osal_mutex_lock(mutex) (td_void)((osal_mutex *)(mutex))
#define audio_osal_mutex_unlock(mutex) (td_void)((osal_mutex *)(mutex))
#else
#define audio_osal_mutex_init(mutex) osal_mutex_init((osal_mutex *)(mutex))
#define audio_osal_mutex_deinit(mutex) osal_mutex_destroy((osal_mutex *)(mutex))
#define audio_osal_mutex_lock(mutex) osal_mutex_lock((osal_mutex *)(mutex))
#define audio_osal_mutex_unlock(mutex) osal_mutex_unlock((osal_mutex *)(mutex))
#endif

#define audio_osal_mdelay osal_mdelay
#define audio_osal_udelay osal_udelay
#define audio_osal_msleep osal_msleep
#define audio_osal_usleep osal_udelay

td_u32 audio_osal_gettime(td_void);
td_u32 audio_osal_gettime_us(td_void);
td_u32 audio_osal_cali_time_diff_us(td_u32 begin_us, td_u32 end_us);

td_void *audio_osal_malloc(td_ulong size);
td_void *audio_osal_calloc(td_ulong size);
td_void audio_osal_free(td_void *addr);
td_void *audio_osal_malloc_aligned(td_u32 size, td_u32 align_bytes);
td_void audio_osal_free_aligned(td_void *addr);

td_void *audio_osal_address_map(td_ulong phys_addr, td_ulong size);
td_void audio_osal_address_unmap(td_ulong phys_addr, td_void *virt_addr);

td_void audio_osal_dcache_region_wb(td_ulong phys_addr, td_void *virt_addr, td_ulong size);
td_void audio_osal_dcache_region_inv(td_void *addr, td_ulong size);
#define audio_osal_icache_region_inv(addr, size)

#define audio_osal_strncmp(buf1, size1, buf2, size2) osal_strncmp(buf1, buf2, size1)
#define audio_osal_system_idle()

td_s32 audio_osal_mmz_alloc(audio_buffer *mmz_buf, td_u32 size);
td_void audio_osal_mmz_release(audio_buffer *mmz_buf);
td_void audio_osal_mmz_map(audio_buffer *mmz_buf);
td_void audio_osal_mmz_unmap(audio_buffer *mmz_buf);

typedef osal_atomic audio_osal_atomic;
#define audio_osal_atomic_init(atomic) osal_atomic_init((osal_atomic *)(atomic))
#define audio_osal_atomic_destroy(atomic) osal_atomic_destroy((osal_atomic *)(atomic))
#define audio_osal_atomic_read(atomic) osal_atomic_read((osal_atomic *)(atomic))
#define audio_osal_atomic_set(atomic, i) osal_atomic_set((osal_atomic *)(atomic), (i))
#define audio_osal_atomic_inc_return(atomic) osal_atomic_inc_return((osal_atomic *)(atomic))
#define audio_osal_atomic_dec_return(atomic) osal_atomic_dec_return((osal_atomic *)(atomic))

#define audio_osal_serial_init()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_OSAL_H__ */
