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

/**
 * @defgroup DPAL API define
 * @ingroup driver
 */

#ifndef DPAL_H
#define DPAL_H

#include "dpal_typedef.h"
#include "dpal_driver.h"
#include "securec.h"
#include "bits/alltypes.h"

#if !defined(__FREERTOS__) && !defined(EMMC_BOOT)
#include "sys/select.h"
#endif

#ifdef SOC_OSAL
#include "soc_osal.h"
#include "osal_adapt.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DPALCFG_BASE_CORE_TICK_PER_SECOND 100
#define DPAL_MS_PER_SECOND         1000
#define DPAL_NS_PER_SECOND         1000000000
#define DPAL_WAIT_FOREVER          0xFFFFFFFF
#define DPAL_TASK_STATUS_DETACHED  0x0100U
#define DPAL_WAITMODE_CLR          1U
#define DPAL_WAITMODE_AND          4U
#define DPAL_WAITMODE_OR           2U
#define DPAL_ERRNO_EVENT_READ_TIMEOUT DPAL_ERRNO_OS_ERROR(DPAL_MOD_EVENT, 0x01)

#ifdef CONFIG_PSEUDOFS_SOFTLINKS
#define SETUP_SEARCH(d, p, n) do { \
        (d)->path     = (p); \
        (d)->node     = NULL; \
        (d)->peer     = NULL; \
        (d)->parent   = NULL; \
        (d)->relpath  = NULL; \
        (d)->linktgt  = NULL; \
        (d)->buffer   = NULL; \
        (d)->nofollow = (n); \
} while (0)

/* only for sdio, define NULL */
#define RELEASE_SEARCH(d)

#else

#define SETUP_SEARCH(d, p, n) \
    do { \
        (d)->path     = (p); \
        (d)->node     = NULL; \
        (d)->peer     = NULL; \
        (d)->parent   = NULL; \
        (d)->relpath  = NULL; \
    } while (0)

#define RELEASE_SEARCH(d)
#endif

#define ATOMIC_INIT(x) (x)
#define	MMC_PEND_REQUEST_DONE (1U << 0)

#ifndef CACHE_ALIGNED_SIZE
#define CACHE_ALIGNED_SIZE 32
#endif

/* This API is used to fence for memory. */
static inline void dpal_mb(void)
{
    __asm__ __volatile__("fence":::"memory");
}

#define DPAL_WRITE_UINT8(value, addr) ({ dpal_mb(); *((volatile uint8_t *)((uintptr_t)(addr))) = (value); })
#define DPAL_WRITE_UINT16(value, addr) ({ dpal_mb(); *((volatile uint16_t *)((uintptr_t)(addr))) = (value); })
#define DPAL_WRITE_UINT32(value, addr) ({ dpal_mb(); *((volatile uint32_t *)((uintptr_t)(addr))) = (value); })
#define DPAL_GET_UINT8(addr) ({ uint8_t r = *((volatile uint8_t *)((uintptr_t)(addr))); dpal_mb(); r; })
#define DPAL_GET_UINT16(addr) ({ uint16_t r = *((volatile uint16_t *)((uintptr_t)(addr))); dpal_mb(); r; })
#define DPAL_GET_UINT32(addr) ({ uint32_t r = *((volatile uint32_t *)((uintptr_t)(addr))); dpal_mb(); r; })
#define DPAL_READ_UINT32(value, addr) ({ (value) = *((volatile uint32_t *)((uintptr_t)(addr))); dpal_mb(); })

#define dpal_writeb(value, address)  DPAL_WRITE_UINT8(value, address)
#define dpal_writew(value, address)  DPAL_WRITE_UINT16(value, address)
#define dpal_writel(value, address)  DPAL_WRITE_UINT32(value, address)

#define dpal_readb(address)          DPAL_GET_UINT8(address)
#define dpal_readw(address)          DPAL_GET_UINT16(address)
#define dpal_readl(address)          DPAL_GET_UINT32(address)
#define DPAL_BSWAP_OFFSET 24

static inline uint32_t dpal_bswap32(uint32_t x)
{
    return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << DPAL_BSWAP_OFFSET);
}

static inline uintptr_t dpal_align(uintptr_t addr, uint32_t boundary)
{
    return (addr + boundary - 1) & ~((uintptr_t)(boundary - 1));
}

#if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define dpal_be32toh(x) dpal_bswap32(x)
#else
#define dpal_be32toh(x) (uint32_t)(x)
#endif
#endif

#ifndef DPAL_MIN
#define DPAL_MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef DPAL_MAX
#define DPAL_MAX(x, y) ((x) < (y) ? (y) : (x))
#endif

#ifdef EMMC_BOOOT
/* define NULL */
#define READ_CSR(csrReg) ({ uint32_t tmp_;             \
    __asm volatile ("csrr %0, " #csrReg : "=r"(tmp_)); \
    tmp_;})

void enable_global_irq(void);
#endif

#if defined(__FREERTOS__)
#define dprintf printf

#elif defined(EMMC_BOOT)
#define dprintf
#define PRINTK
#else
extern void dprintf(const char *fmt, ...);
#endif

#define	dpal_log_info(fmt, args...) do {	\
    dprintf("[WARNING] %s:%d: ", __FUNCTION__, __LINE__);	\
    dprintf(fmt, ##args); \
} while (0)

#define	dpal_log_warn(fmt, args...) do {	\
    dprintf("[WARNING] %s:%d: ", __FUNCTION__, __LINE__);	\
    dprintf(fmt, ##args); \
} while (0)

#define	dpal_log_err(fmt, args...) do {	\
    dprintf("[ERROR] %s:%d: ", __FUNCTION__, __LINE__);	\
    dprintf(fmt, ##args); \
} while (0)

/* NEED TO DEFINE INTERFACE_DEBUG */
#ifdef INTERFACE_DEBUG
#define debug_printf() dprintf("%s, %d\n", __FUNCTION__, __LINE__)
#else
#define debug_printf()
#endif

typedef int (*dpal_hwi_proc_func)(int, void*);
typedef void *(*dpal_tsk_entry_func)(void *param);

typedef struct {
    void *wait;
} wait_t;
/**
 * @ingroup dpal_event
 * Event control structure
 */
typedef struct {
    uint32_t event_id;        /**< Event mask in the event control block,
                                 indicating the event that has been logically processed. */
    uint32_t event_max_num;   /**< Maximum number of user-defined events. */
    uint32_t event_bits;      /**< Event bit, value range is [0 ~ EVENT_SUPPORT_BITS]. */
    uint32_t event_bit_idx;   /**< Event bit index, value range is [0 ~ EVENT_MAX_CNT -1]. */
#ifndef EMMC_BOOT
#ifdef SOC_OSAL
    osal_event event;
#endif
#endif
} dpal_event_t;

/**
 * @ingroup dpal_task_attr
 * Task attr control structure
 */
typedef struct {
    uint16_t task_prio;   /**< Task priority. */
    uint32_t stack_size;  /**< Task stack size. */
    uint32_t task_policy; /**< Task schedule policy. Not used by default. */
    uint32_t task_nice;   /**< Task nice value. Not used by default. */
    uint32_t task_cpuid;  /**< CPUID to which the task belongs. Not used by default. */
    char *task_name;    /**< Task name. */
    void *reserved;     /**< Reserved. Not used by default. */
} dpal_task_attr_t;

void *dpal_malloc(size_t size);
void dpal_free(void *ptr);
void *dpal_zalloc(size_t size);
void *dpal_memalign(size_t boundary, size_t size);

uint32_t dpal_mux_create(uint32_t *mux_handle);
uint32_t dpal_mux_delete(uint32_t mux_handle);
uint32_t dpal_mux_pend(uint32_t mux_handle, uint32_t timeout);
uint32_t dpal_mux_post(uint32_t mux_handle);

void dpal_atomic_inc(atomic *v);
void dpal_atomic_dec(atomic *v);
int32_t dpal_atomic_read(const atomic *v);

void dpal_mdelay(uint32_t msecs);
void dpal_udelay(uint32_t usecs);
void dpal_msleep(uint32_t msecs);

uint32_t dpal_irq_create(uint32_t irq, dpal_hwi_proc_func handler, uint32_t flags, const char *name, void *param);
uint32_t dpal_irq_delete(uint32_t irq, void *param);
uint32_t dpal_irq_enable(uint32_t irq);
uint32_t dpal_irq_disable(uint32_t irq);
uint32_t dpal_irq_lock(void);
uint32_t dpal_irq_unlock(void);
void dpal_irq_restore(uint32_t int_save);

uint32_t dpal_event_init(dpal_event_t *event_info);
uint32_t dpal_event_destroy(dpal_event_t *event_info);
uint32_t dpal_event_read(dpal_event_t *event_info, uint32_t event_mask, uint32_t mode, uint32_t timeout);
uint32_t dpal_event_write(dpal_event_t *event_info, uint32_t event_mask);

uint32_t dpal_queue_create(uint32_t *qid, uint16_t queue_len, uint32_t msg_size);
uint32_t dpal_queue_delete(uint32_t qid);
uint32_t dpal_queue_read(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout);
uint32_t dpal_queue_write(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout);
uint32_t dpal_queue_write_head(uint32_t qid, void *msg, uint32_t msg_size, uint32_t timeout);

uint32_t dpal_task_create(uint32_t *taskid, const dpal_task_attr_t *attr, dpal_tsk_entry_func entry, void *arg);
uint32_t dpal_task_delete(uint32_t taskid);

uint16_t dpal_low_bit_get(uint32_t bitmap);
uint64_t dpal_gettime_mono(bool millisecond);

void dpal_dma_cache_clean(uintptr_t start, uintptr_t end);
void dpal_dma_cache_inv(uintptr_t start, uintptr_t end);
void dpal_dma_cache_flush(uintptr_t start, uintptr_t end);

int32_t dpal_get_curr_task(void);
void dpal_task_pri_set(uint32_t taskid, uint32_t priority);
void dpal_task_lock(void);
void dpal_task_unlock(void);

int32_t dpal_platform_driver_register(struct dpal_platform_driver *drv);
int32_t dpal_platform_driver_unregister(struct dpal_platform_driver *drv);
dpal_resource_t* dpal_platform_get_resource(struct dpal_platform_device *dev, uint32_t type, uint32_t num);
int32_t dpal_platform_device_register(struct dpal_platform_device *dev);
void dpal_platform_device_unregister(struct dpal_platform_device *dev);

char *dpal_get_shell_work_dir(void);
unsigned long long dpal_sched_clock(void);

void set_errno(int err_code);
int get_errno(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DPAL_H__ */
