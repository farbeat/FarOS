#include "errno.h"
#include "FreeRTOS.h"
#ifdef XLTCFG_SUPPORT_MEMMNG
#include "xlt_memory.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int posix_memalign(void **memAddr, size_t alignment, size_t size)
{
    if (memAddr == NULL) {
        return EINVAL;
    }

    if ((alignment == 0) || ((alignment & (alignment - 1)) != 0) || ((alignment % sizeof(void *)) != 0)) {
        return EINVAL;
    }

#ifdef XLTCFG_SUPPORT_MEMMNG
    *memAddr = (void *)xlt_mem_alloc_align((void *)OS_EXTEND_MEM_ADDR, (unsigned int)size, (unsigned int)alignment);
#else
    if (alignment != portBYTE_ALIGNMENT) {
        *memAddr = NULL;
    } else {
        *memAddr = pvPortMallocExtendMem(size);
    }
#endif
    if (*memAddr == NULL) {
        return ENOMEM;
    }

    return ENOERR;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
