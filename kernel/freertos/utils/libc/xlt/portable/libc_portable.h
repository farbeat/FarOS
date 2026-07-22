#ifndef __LIBC_PORTABLE__
#define __LIBC_PORTABLE__

#include <stdarg.h>

enum {
    XLT_LIBC_LOG_FATAL = 0,
    XLT_LIBC_LOG_ERROR,
    XLT_LIBC_LOG_WARN,
    XLT_LIBC_LOG_NOTICE,
    XLT_LIBC_LOG_INFO,
    XLT_LIBC_LOG_DEBUG
}; 

int libc_dprintf(int level, const char *fmt, ...);

#define PRINT_ERR(fmt, ... ) \
    libc_dprintf(XLT_LIBC_LOG_ERROR, fmt, ##__VA_ARGS__)

#endif /* __LIBC_PORTABLE__ */
