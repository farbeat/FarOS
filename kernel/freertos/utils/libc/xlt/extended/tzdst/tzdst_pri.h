#ifndef __LINUX_TZDST_PRI_H__
#define __LINUX_TZDST_PRI_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern int32_t dst_forward_sec_get(void);
extern bool dst_period_check(const struct tm * const tm, int64_t seconds);
extern uint32_t g_tzdst_lock;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LINUX_TZDST_PRI_H__ */
