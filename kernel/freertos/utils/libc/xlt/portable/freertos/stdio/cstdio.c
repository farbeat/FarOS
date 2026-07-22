#include "FreeRTOS.h"
#include "print.h"
#include "libc_portable.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int libc_dprintf(int level, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = vUartVprintk(level, fmt, ap);
    va_end(ap);

    return ret;
}

__attribute__((weak)) int printf(const char *restrict fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vUartVprintk(XLT_LIBC_LOG_INFO, fmt, ap);
    va_end(ap);

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
