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

#include "dpal.h"

#define DPAL_INVALID_BIT_INDEX 32

#ifdef EMMC_BOOT
#include "boot_rom.h"
#include "malloc_rom.h"
#include "securec.h"
#include "serial_dw.h"

#include "riscv_irq.h"
#include "hwi_boot.h"

#define EVENT_MAX_CNT     32
#define MUTEX_TIMEOUT     100 // ms
#define DPAL_QUEUE_EMPTY  DPAL_NOK

#ifndef CTZ
#define CTZ(value)                                  (0)
#endif

#ifndef MCYCLE
#define MCYCLE  0xB00
#define MCYCLEH 0xB80
#endif

#ifndef TIME_RATION_C
#define TIME_RATION_C 1000LL
#endif

#ifndef CPU_CLOCK_HZ
#define CPU_CLOCK_HZ (40 * 1000 * 1000)
#endif

#define MSTATUS_MIE               0x08
#define SET_CSR(csrReg, csrBit) do { \
    ({ uint32_t tmp_;                              \
    if (__builtin_constant_p(csrBit) && (uint32_t)(csrBit) < 32) {                  \
        __asm volatile ("csrrs %0, " #csrReg ", %1" : "=r"(tmp_) : "i"(csrBit));  \
    } else {                                                                      \
        __asm volatile ("csrrs %0, " #csrReg ", %1" : "=r"(tmp_) : "r"(csrBit));  \
    }                                                                             \
    tmp_;})                                                                       \
} while (0)
void enable_global_irq(void)
{
    SET_CSR(mstatus, MSTATUS_MIE);
}

static uint64_t HalGetCpuMCycles(void)
{
    uint64_t low = (uint64_t)READ_CUSTOM_CSR(MCYCLE);
    uint64_t high = (uint64_t)READ_CUSTOM_CSR(MCYCLEH);
    return low | (high << 32); // high 32 bits of 64 bits in cycles
}

static uint32_t g_event_vector = 0;
static uint32_t g_mutex_handle = 0;

void *dpal_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    void *ptr = boot_malloc(size);
    if (ptr == NULL) {
        boot_msg0("malloc buffer error");
    }
    return ptr;
}

void dpal_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    (void)boot_free(ptr);
}

void *dpal_zalloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    void *ptr = boot_malloc(size);
    if (ptr != NULL) {
        (void)memset_s(ptr, size, 0, size);
    }

    return ptr;
}

void *dpal_memalign(size_t boundary, size_t size)
{
    (void)boundary;
    return dpal_malloc(size);
}

uint32_t dpal_mux_create(uint32_t *mux_handle)
{
    if (mux_handle == NULL) {
        return DPAL_NOK;
    }
    *mux_handle = 0;
    return DPAL_OK;
}

uint32_t dpal_mux_delete(uint32_t mux_handle)
{
    (void)mux_handle;
    return DPAL_OK;
}

uint32_t dpal_mux_pend(uint32_t mux_handle, uint32_t timeout)
{
    (void)mux_handle;
    (void)timeout;
    return DPAL_OK;
}

uint32_t dpal_mux_post(uint32_t mux_handle)
{
    (void)mux_handle;
    return DPAL_OK;
}

void dpal_atomic_inc(atomic *v)
{
    if (v == NULL) {
        boot_msg0("inc: atomic value invalid error");
        return;
    }
    uint32_t int_save;

    int_save = dpal_irq_lock();
    *v += 1;
    dpal_irq_restore(int_save);
}

void dpal_atomic_dec(atomic *v)
{
    if (v == NULL) {
        boot_msg0("dec: atomic value invalid error");
        return;
    }
    uint32_t int_save;

    int_save = dpal_irq_lock();
    *v -= 1;
    dpal_irq_restore(int_save);
}

int32_t dpal_atomic_read(const atomic *v)
{
    if (v == NULL) {
        return -1;
    }
    return *(volatile int32_t *)v;
}

void dpal_mdelay(uint32_t msecs)
{
    mdelay(msecs);
}

void dpal_udelay(uint32_t usecs)
{
    udelay(usecs);
}

void dpal_msleep(uint32_t msecs)
{
    mdelay(msecs);
}

uint32_t dpal_irq_create(uint32_t irq, dpal_hwi_proc_func handler, uint32_t flags, const char *name, void *param)
{
    enable_global_irq();
    hwi_irq_param_s irq_param;
    irq_param.irq_num = (int32_t)irq;
    irq_param.dev_id = param;
    irq_param.irq_name = name;

    uint32_t ret = boot_hwi_create(irq, handler, &irq_param);
    if (ret == DPAL_OK) {
        (void)boot_hwi_unmask(irq);
    }
    boot_msg1("boot_hwi_create ret: ", ret);
    return ret;
}

uint32_t dpal_irq_delete(uint32_t irq, void *param)
{
    hwi_irq_param_s irq_param;
    irq_param.irq_num = (int32_t)irq;
    irq_param.dev_id = param;

    return boot_hwi_delete(irq, &irq_param);
}

uint32_t dpal_irq_enable(uint32_t irq)
{
    return boot_hwi_unmask(irq);
}

uint32_t dpal_irq_disable(uint32_t irq)
{
    return boot_hwi_mask(irq);
}

uint32_t dpal_irq_lock(void)
{
    return boot_int_lock();
}

uint32_t dpal_irq_unlock(void)
{
    return boot_int_unlock();
}

void dpal_irq_restore(uint32_t int_save)
{
    boot_int_restore(int_save);
}

uint32_t dpal_event_init(dpal_event_t *event_info)
{
    if (event_info == NULL) {
        return DPAL_NOK;
    }
    event_info->event_id = 0;
    event_info->event_bit_idx = 0;

    return DPAL_OK;
}

uint32_t dpal_event_destroy(dpal_event_t *event_info)
{
    if (event_info == NULL) {
        return DPAL_NOK;
    }
    event_info->event_id = 0;
    event_info->event_bit_idx = 0;

    return DPAL_OK;
}

uint32_t dpal_event_read(dpal_event_t *event_info, uint32_t event_mask, uint32_t mode, uint32_t timeout)
{
    if (event_info == NULL || event_info->event_bit_idx >= EVENT_MAX_CNT) {
        return DPAL_NOK;
    }
    (void)event_mask;
    (void)mode;
    const uint32_t SLEEP_TIME = 10;

    uint32_t event_bit_mask = (1 << event_info->event_bit_idx);
    uint32_t ret = dpal_mux_pend(g_mutex_handle, MUTEX_TIMEOUT);
    if (ret != 0) {
        dprintf("dpal_mux_pend: error!\n");
        return DPAL_NOK;
    }
    if ((g_event_vector & event_bit_mask) != 0) {
        g_event_vector &= ~event_bit_mask;
        dpal_mux_post(g_mutex_handle);
        return DPAL_OK;
    }
    dpal_mux_post(g_mutex_handle);

    if (timeout == 0) {
        dprintf("dpal event read: error, no event\n");
        return DPAL_NOK;
    }

    ret = dpal_mux_pend(g_mutex_handle, MUTEX_TIMEOUT);
    if (ret != 0) {
        dprintf("dpal_mux_pend: error!\n");
        return DPAL_NOK;
    }
    while (timeout > SLEEP_TIME) {
        if ((g_event_vector & event_bit_mask) != 0) {
            g_event_vector &= ~event_bit_mask;
            dpal_mux_post(g_mutex_handle);
            return DPAL_OK;
        }
        dpal_mdelay(SLEEP_TIME);
        timeout -= SLEEP_TIME;
    }
    dpal_mux_post(g_mutex_handle);

    dprintf("dpal event read: error, no event\n");
    return DPAL_NOK;
}

uint32_t dpal_event_write(dpal_event_t *event_info, uint32_t event_mask)
{
    if (event_info == NULL || event_info->event_bit_idx >= EVENT_MAX_CNT) {
        return DPAL_NOK;
    }
    (void)event_mask;

    uint32_t event_bit_mask = (1 << event_info->event_bit_idx);
    if ((g_event_vector & event_bit_mask) != 0) {
        dprintf("event_write: error, prev event not read\n");
        return DPAL_NOK;
    }

    uint32_t ret = dpal_mux_pend(g_mutex_handle, MUTEX_TIMEOUT);
    if (ret != 0) {
        dprintf("dpal_mux_pend: error!\n");
        return DPAL_NOK;
    }
    g_event_vector |= event_bit_mask;
    dpal_mux_post(g_mutex_handle);
    return DPAL_OK;
}

uint32_t dpal_queue_create(uint32_t *qid, uint16_t queue_len, uint32_t msg_size)
{
    (void)qid;
    (void)queue_len;
    (void)msg_size;
    return DPAL_OK;
}

uint32_t dpal_queue_delete(uint32_t qid)
{
    (void)qid;
    return DPAL_OK;
}

uint32_t dpal_queue_read(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    (void)qid;
    (void)msg;
    (void)msg_size;
    (void)timeout;
    return DPAL_QUEUE_EMPTY;
}

uint32_t dpal_queue_write(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    (void)qid;
    (void)msg;
    (void)msg_size;
    (void)timeout;
    return DPAL_OK;
}

uint32_t dpal_queue_write_head(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    (void)qid;
    (void)msg;
    (void)msg_size;
    (void)timeout;
    return DPAL_OK;
}

uint32_t dpal_task_create(uint32_t *taskid, const dpal_task_attr_t *attr, dpal_tsk_entry_func entry, void *arg)
{
    (void)taskid;
    (void)attr;
    (void)entry;
    (void)arg;
    return DPAL_OK;
}

uint32_t dpal_task_delete(uint32_t taskid)
{
    (void)taskid;
    return DPAL_OK;
}

uint16_t dpal_low_bit_get(uint32_t bitmap)
{
    if (bitmap == 0) {
        return DPAL_INVALID_BIT_INDEX;
    }

    return CTZ(bitmap);
}

uint64_t dpal_gettime_mono(bool millisecond)
{
    uint64_t cycle = HalGetCpuMCycles();
    if (millisecond) {
        return cycle * TIME_RATION_C / CPU_CLOCK_HZ; /* milliseconds */
    } else {
        return cycle * TIME_RATION_C * TIME_RATION_C / CPU_CLOCK_HZ;  /* microseconds */
    }
}

void dpal_dma_cache_clean(uintptr_t start, uintptr_t end)
{
    (void)start;
    (void)end;
}

void dpal_dma_cache_inv(uintptr_t start, uintptr_t end)
{
    (void)start;
    (void)end;
}

void dpal_dma_cache_flush(uintptr_t start, uintptr_t end)
{
    (void)start;
    (void)end;
}

int32_t dpal_get_curr_task(void)
{
    return 0;
}

int32_t dpal_platform_driver_register(struct dpal_platform_driver *drv)
{
    return dpal_plat_driver_register(drv);
}

int32_t dpal_platform_driver_unregister(struct dpal_platform_driver *drv)
{
    return dpal_plat_driver_unregister(drv);
}

dpal_resource_t* dpal_platform_get_resource(struct dpal_platform_device *dev, uint32_t type, uint32_t num)
{
    return dpal_plat_get_resource(dev, type, num);
}

int32_t dpal_platform_device_register(struct dpal_platform_device *dev)
{
    return dpal_plat_device_register(dev);
}

void dpal_platform_device_unregister(struct dpal_platform_device *dev)
{
    return dpal_plat_device_unregister(dev);
}

void dpal_task_lock(void)
{
    return;
}

void dpal_task_unlock(void)
{
    return;
}

#else

#define US_TO_NSEC 1000
#include "securec.h"
#include "stdlib.h"
#include "string.h"
#include "dpal_task.h"
#include "dpal_lock.h"
#include "nuttx.h"
#include "td_type.h"
#include "osal_list.h"
#include "soc_osal.h"

#define INVALID_MUX_ID 0xffffffff
#define DPAL_EVENT_WAITMODE_AND         4
#define DPAL_EVENT_WAITMODE_OR          2
#define DPAL_EVENT_WAITMODE_CLR         1
#define EVENT_MAX_NUM 5

#define DPAL_DEFAULT_PRIORITY 1


#if defined ( __ICCARM__ )
#ifndef CTZ
#define ctz(value)                                  (__UNDEFINED(value))
#endif

/* for GNU Compiler */
#elif defined ( __GNUC__ )
#ifndef CTZ
#define ctz(value)                                  (__builtin_ctz(value))
#endif
#endif

#ifndef container_of
#if __GNUC__ > 3
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t)((char *)&(((type *)0)->member) - (char *)0))
#endif

#define container_of(p, t, m) ((t*)((char *)(p) - offsetof(t, m)))
#endif

void *dpal_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }
#ifdef CONFIG_DPAL_OSAL_ALLOC
    void *ptr = osal_kmalloc(size, OSAL_GFP_KERNEL);
#else
    void *ptr = memalign(CACHE_ALIGNED_SIZE, size);
#endif
    return ptr;
}

void dpal_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
#ifdef CONFIG_DPAL_OSAL_ALLOC
    osal_kfree(ptr);
#else
    free(ptr);
#endif
}

void *dpal_zalloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    void *ptr = (void *)dpal_malloc(size);
    if (ptr != NULL) {
        (void)memset_s(ptr, size, 0, size);
    }
    return ptr;
}

void *dpal_memalign(size_t boundary, size_t size)
{
#ifdef CONFIG_DPAL_OSAL_ALLOC
    return osal_kmalloc_align(size, 0, boundary);
#else
    return memalign(boundary, size);
#endif
}

uint32_t dpal_mux_create(uint32_t *mux_handle)
{
    debug_printf();
    if (mux_handle == NULL) {
        return DPAL_NOK;
    }
    uint32_t ret;
    osal_mutex osal_mutex = { 0 };
    ret = (uint32_t)osal_mutex_init(&osal_mutex);
    if (ret != 0) {
        dprintf("osal_mutex_init fialed, ret = %u\n", ret);
        return ret;
    }
    *mux_handle = (uint32_t)osal_mutex.mutex;
    return DPAL_OK;
}

uint32_t dpal_mux_delete(uint32_t mux_handle)
{
    debug_printf();
    osal_mutex osal_mutex = { 0 };
    if (mux_handle == INVALID_MUX_ID) {
        return DPAL_NOK;
    }
    osal_mutex.mutex = (void *)(uintptr_t)mux_handle;
    osal_mutex_destroy(&osal_mutex);
    return DPAL_OK;
}

uint32_t dpal_mux_pend(uint32_t mux_handle, uint32_t timeout)
{
    debug_printf();
    uint32_t ret;
    osal_mutex osal_mutex = { 0 };

    if (mux_handle == INVALID_MUX_ID) {
        return DPAL_NOK;
    }
    osal_mutex.mutex = (void *)(uintptr_t)mux_handle;
    ret = (uint32_t)osal_mutex_lock_timeout(&osal_mutex, timeout);
    if (ret != 0) {
        dprintf("osal_mutex_lock_timeout fialed, ret = %d\n", ret);
        return ret;
    }
    return ret;
}

uint32_t dpal_mux_post(uint32_t mux_handle)
{
    debug_printf();
    osal_mutex osal_mutex = { 0 };
    if (mux_handle == INVALID_MUX_ID) {
        return DPAL_NOK;
    }
    osal_mutex.mutex = (void *)(uintptr_t)mux_handle;
    osal_mutex_unlock(&osal_mutex);
    return DPAL_OK;
}

void dpal_atomic_inc(atomic *v)
{
    if (v == NULL) {
        return;
    }
    debug_printf();
    osal_atomic_inc_return((osal_atomic *)v);
}

void dpal_atomic_dec(atomic *v)
{
    if (v == NULL) {
        return;
    }
    debug_printf();
    osal_atomic_dec_return((osal_atomic *)v);
}

int32_t dpal_atomic_read(const atomic *v)
{
    if (v == NULL) {
        return -1;
    }
    debug_printf();
    return osal_atomic_read((osal_atomic *)v);
}

void dpal_mdelay(uint32_t msecs)
{
    osal_mdelay(msecs);
}

void dpal_udelay(uint32_t usecs)
{
    osal_udelay(usecs);
}

void dpal_msleep(uint32_t msecs)
{
    osal_msleep(msecs);
}

uint32_t dpal_irq_create(uint32_t irq, dpal_hwi_proc_func handler, uint32_t flags, const char *name, void *param)
{
    debug_printf();
    uint32_t ret;
    ret = (uint32_t)osal_irq_request(irq, (osal_irq_handler)handler, (osal_irq_handler)handler, name, param);
    if (ret != 0) {
        dprintf("Irq request fialed\n");
        return ret;
    }
    osal_irq_set_priority(irq, DPAL_DEFAULT_PRIORITY);
    osal_irq_enable(irq);
    return DPAL_OK;
}

uint32_t dpal_irq_delete(uint32_t irq, void *param)
{
    debug_printf();
    osal_irq_free(irq, NULL);
    return DPAL_OK;
}

uint32_t dpal_irq_enable(uint32_t irq)
{
    debug_printf();
    osal_irq_enable(irq);
    return DPAL_OK;
}

uint32_t dpal_irq_disable(uint32_t irq)
{
    debug_printf();
    osal_irq_disable(irq);
    return DPAL_OK;
}

uint32_t dpal_irq_lock(void)
{
    debug_printf();
    return osal_irq_lock();
}

uint32_t dpal_irq_unlock(void)
{
    debug_printf();
    // not used and not be implemented
    return DPAL_OK;
}

void dpal_irq_restore(uint32_t int_save)
{
    debug_printf();
    osal_irq_restore(int_save);
}

uint32_t dpal_event_init(dpal_event_t *event_info)
{
    debug_printf();
    if (event_info == NULL) {
        return DPAL_NOK;
    }
    uint32_t ret = (uint32_t)osal_event_init(&event_info->event);
    if (ret != DPAL_OK) {
        dprintf("event init failed\n");
        return ret;
    }
    return ret;
}

uint32_t dpal_event_destroy(dpal_event_t *event_info)
{
    debug_printf();
    uint32_t ret;

    if (event_info == NULL) {
        return DPAL_NOK;
    }

    ret = (uint32_t)osal_event_clear(&event_info->event, event_info->event_bits);
    if (ret != DPAL_OK) {
        dprintf("event clear failed\n");
        return ret;
    }

    ret = (uint32_t)osal_event_destroy(&event_info->event);
    if (ret != DPAL_OK) {
        dprintf("event destroy failed\n");
        return ret;
    }

    return DPAL_OK;
}

uint32_t dpal_event_read(dpal_event_t *event_info, uint32_t event_mask, uint32_t mode, uint32_t timeout)
{
    debug_printf();
    if (event_info == NULL) {
        return DPAL_NOK;
    }
    uint32_t ret = (uint32_t)osal_event_read(&event_info->event, event_mask,
        timeout, DPAL_EVENT_WAITMODE_OR | DPAL_EVENT_WAITMODE_CLR);
    event_info->event_bits = ret;
    return ret;
}

uint32_t dpal_event_write(dpal_event_t *event_info, uint32_t event_mask)
{
    debug_printf();
    if (event_info == NULL) {
        return DPAL_NOK;
    }
    return (uint32_t)osal_event_write(&event_info->event, event_mask);
}

uint32_t dpal_queue_create(uint32_t *qid, uint16_t queue_len, uint32_t msg_size)
{
    debug_printf();
    if (qid == NULL) {
        return DPAL_NOK;
    }
    unsigned long msg_queue_id = 0;
    uint32_t ret = (uint32_t)osal_msg_queue_create("dpal", queue_len, &msg_queue_id, 0, (uint16_t)msg_size);
    *qid = (uint32_t)msg_queue_id;
    return ret;
}

uint32_t dpal_queue_delete(uint32_t qid)
{
    debug_printf();
    osal_msg_queue_delete(qid);
    return DPAL_OK;
}

uint32_t dpal_queue_read(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    debug_printf();
    return (uint32_t)osal_msg_queue_read_copy(qid, msg, &msg_size, timeout);
}

uint32_t dpal_queue_write(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    debug_printf();
    return (uint32_t)osal_msg_queue_write_copy(qid, &msg, msg_size, timeout);
}

uint32_t dpal_queue_write_head(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout)
{
    debug_printf();
    return (uint32_t)osal_msg_queue_write_head_copy(qid, &msg, msg_size, timeout);
}

uint32_t dpal_task_create(uint32_t *taskid, const dpal_task_attr_t *attr, dpal_tsk_entry_func entry, void *arg)
{
    debug_printf();
    if (attr == NULL || taskid == NULL) {
        return DPAL_NOK;
    }
    osal_task *task = osal_kthread_create((osal_kthread_handler)entry, arg, attr->task_name, attr->stack_size);
    if (task == NULL) {
        return DPAL_FAIL;
    }
    *taskid = (uint32_t)task->task;

    uint32_t ret = (uint32_t)osal_kthread_set_priority(task, attr->task_prio);

    return ret;
}

uint32_t dpal_task_delete(uint32_t taskid)
{
    debug_printf();
    osal_task task;
    task.task = &taskid;
    osal_kthread_destroy(&task, 0);
    return DPAL_OK;
}

uint16_t dpal_low_bit_get(uint32_t bitmap)
{
    debug_printf();
    if (bitmap == 0) {
        return DPAL_INVALID_BIT_INDEX;
    }

    return (uint16_t)ctz(bitmap);
}

uint64_t dpal_gettime_mono(bool millisecond)
{
    debug_printf();
    return osal_get_jiffies();
}

void dpal_dma_cache_clean(uintptr_t start, uintptr_t end)
{
    debug_printf();
    if (end <= start) {
        return;
    }
    osal_dcache_region_clean((void *)start, end - start);
}

void dpal_dma_cache_inv(uintptr_t start, uintptr_t end)
{
    debug_printf();
    if (end <= start) {
        return;
    }
    osal_dcache_region_inv((void *)start, end - start);
}

void dpal_dma_cache_flush(uintptr_t start, uintptr_t end)
{
    debug_printf();
    (void)start;
    (void)end;
    osal_dcache_flush_all();
}

int32_t dpal_get_curr_task(void)
{
    int32_t task_id = (int32_t)osal_get_current_tid();
    return task_id;
}

int32_t dpal_platform_driver_register(struct dpal_platform_driver *drv)
{
    debug_printf();
    return dpal_plat_driver_register(drv);
}

int32_t dpal_platform_driver_unregister(struct dpal_platform_driver *drv)
{
    debug_printf();
    return dpal_plat_driver_unregister(drv);
}

dpal_resource_t* dpal_platform_get_resource(struct dpal_platform_device *dev, uint32_t type, uint32_t num)
{
    debug_printf();
    return dpal_plat_get_resource(dev, type, num);
}

int32_t dpal_platform_device_register(struct dpal_platform_device *dev)
{
    debug_printf();
    return dpal_plat_device_register(dev);
}

void dpal_platform_device_unregister(struct dpal_platform_device *dev)
{
    debug_printf();
    return dpal_plat_device_unregister(dev);
}

char *OsShellGetWorkingDirectory(VOID);
char *dpal_get_shell_work_dir(void)
{
    return OsShellGetWorkingDirectory();
}

void dpal_spin_lock_init(dpal_spinlock *lock)
{
    if (lock == NULL) {
        return;
    }
    osal_spinlock osal_lock = {};
    (void)osal_spin_lock_init(&osal_lock);
    lock->lock = osal_lock.lock;
}

void dpal_spin_lock_irqsave(dpal_spinlock *lock, unsigned long *flags)
{
    if (lock == NULL) {
        return;
    }
    osal_spinlock osal_lock = {};
    osal_lock.lock = lock->lock;
    osal_spin_lock_irqsave(&osal_lock, flags);
    return;
}

void dpal_spin_unlock_irqrestore(dpal_spinlock *lock, unsigned long *flags)
{
    if (lock == NULL) {
        return;
    }
    osal_spinlock osal_lock = {};
    osal_lock.lock = lock->lock;
    osal_spin_unlock_irqrestore(&osal_lock, flags);
    return;
}

int dpal_sem_init(nu_sem *sem, int32_t val)
{
    if (sem == NULL) {
        return DPAL_NOK;
    }
    osal_semaphore osal_sem = {};
    int ret = osal_sem_init(&osal_sem, val);
    if (ret != DPAL_OK) {
        return DPAL_NOK;
    }
    sem->sem = osal_sem.sem;
    return DPAL_OK;
}

void dpal_sem_up(nu_sem *sem)
{
    if (sem == NULL) {
        return;
    }
    osal_semaphore osal_sem = {};
    osal_sem.sem = sem->sem;
    osal_sem_up(&osal_sem);
}

int dpal_sem_down_interruptible(nu_sem *sem)
{
    if (sem == NULL) {
        return DPAL_NOK;
    }
    osal_semaphore osal_sem = {};
    osal_sem.sem = sem->sem;
    return osal_sem_down_interruptible(&osal_sem);
}

void dpal_sem_destroy(nu_sem *sem)
{
    if (sem == NULL) {
        return;
    }
    osal_semaphore osal_sem = {};
    osal_sem.sem = sem->sem;
    osal_sem_destroy(&osal_sem);
}

int dpal_sem_down_timeout(nu_sem *sem, unsigned int timeout)
{
    if (sem == NULL) {
        return DPAL_NOK;
    }
    osal_semaphore osal_sem = {};
    osal_sem.sem = sem->sem;
    return osal_sem_down_timeout(&osal_sem, timeout);
}

void dpal_task_lock(void)
{
    osal_kthread_lock();
}

void dpal_task_unlock(void)
{
    osal_kthread_unlock();
}

void dpal_task_pri_set(uint32_t taskid, uint32_t priority)
{
    osal_task task = {};
    task.task = (void *)(uintptr_t)taskid;
    osal_kthread_set_priority(&task, priority);
}

unsigned long long dpal_sched_clock(void)
{
    return osal_sched_clock();
}

/* be used to get/set errno */
#ifdef __FREERTOS__
#include "errno.h"
void set_errno(int err_code)
{
    errno = err_code;
}
int get_errno(void)
{
    return errno;
}
#endif

#endif
