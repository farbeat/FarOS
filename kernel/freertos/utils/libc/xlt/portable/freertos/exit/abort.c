#include "features.h"
#include "libc_portable.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

_Noreturn void abort(void)
{
    PRINT_ERR("System was being aborted\n");
    taskDISABLE_INTERRUPTS();
    for (;;) {}
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
