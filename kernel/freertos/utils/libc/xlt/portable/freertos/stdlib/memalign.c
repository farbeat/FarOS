#include "FreeRTOS.h"
#ifdef XLTCFG_SUPPORT_MEMMNG
#include "xlt_memory.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * allocates a block of size bytes whose address is a multiple of boundary.
 * The boundary must be a power of two!
 */
void *memalign(size_t boundary, size_t size)
{
    if (size == 0) {
        return NULL;
    }
#ifdef XLTCFG_SUPPORT_MEMMNG
    return (void *)xlt_mem_alloc_align((void *)OS_EXTEND_MEM_ADDR, (unsigned int)size, (unsigned int)boundary);
#else
    if (boundary != portBYTE_ALIGNMENT) {
        return NULL;
    }
    return pvPortMallocExtendMem(size);
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
