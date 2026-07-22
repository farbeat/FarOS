#include "string.h"
#include "FreeRTOS.h"
#ifdef XLTCFG_SUPPORT_MEMMNG
#include "xlt_memory.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void *zalloc(size_t size)
{
    void *ptr = NULL;

    if (size == 0) {
        return NULL;
    }

#ifdef XLTCFG_SUPPORT_MEMMNG
    ptr = xlt_mem_alloc((void *)OS_EXTEND_MEM_ADDR, (unsigned int)size);
#else
    ptr = pvPortMallocExtendMem(size);
#endif
    if (ptr != NULL) {
        (void)memset(ptr, 0, size);
    }
    return ptr;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
