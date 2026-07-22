#include "securec.h"
#include "hal_uart.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int printf(const char *restrict fmt, ...)
{
    #define PRINT_BUF_SIZE 256
    char buf[PRINT_BUF_SIZE] = {0};
    va_list ap;

    va_start(ap, fmt);
    int len = vsnprintf_s(buf, PRINT_BUF_SIZE, PRINT_BUF_SIZE - 1, fmt, ap);
    if (len < 0 && buf[0] == 0) {
        const char *p = "Output illegal string! vsnprintf_s failed!\n";
        uart_puts(p, strlen(p));
        va_end(ap);
        return -1;
    }

    va_end(ap);
    uart_puts(buf, strlen(buf));

    return len;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
