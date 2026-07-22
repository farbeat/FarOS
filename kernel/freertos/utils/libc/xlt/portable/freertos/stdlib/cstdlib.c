#include "stdlib.h"
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

/*
 * Allocates the requested memory and returns a pointer to it. The requested
 * size is nitems each size bytes long (total memory requested is nitems*size).
 * The space is initialized to all zero bits.
 */
void *calloc(size_t nitems, size_t size)
{
    size_t real_size;
    void *ptr = NULL;

    if (nitems == 0 || size == 0 || nitems > (UINT32_MAX / size)) {
        return NULL;
    }

    real_size = (size_t)(nitems * size);
    ptr = pvPortMallocExtendMem(real_size);
    if (ptr != NULL) {
        (void)memset((void *)ptr, 0, real_size);
    }
    return ptr;
}

/*
 * Deallocates the memory previously allocated by a call to calloc, malloc, or
 * realloc. The argument ptr points to the space that was previously allocated.
 * If ptr points to a memory block that was not allocated with calloc, malloc,
 * or realloc, or is a space that has been deallocated, then the result is undefined.
 */
void free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }

    (void)vPortFreeExtendMem(ptr);
}

/*
 * Allocates the requested memory and returns a pointer to it. The requested
 * size is size bytes. The value of the space is indeterminate.
 */
void *malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    return (void *)pvPortMallocExtendMem(size);
}

/*
 * Attempts to resize the memory block pointed to by ptr that was previously
 * allocated with a call to malloc or calloc. The contents pointed to by ptr are
 * unchanged. If the value of size is greater than the previous size of the
 * block, then the additional bytes have an undeterminate value. If the value
 * of size is less than the previous size of the block, then the difference of
 * bytes at the end of the block are freed. If ptr is null, then it behaves like
 * malloc. If ptr points to a memory block that was not allocated with calloc
 * or malloc, or is a space that has been deallocated, then the result is
 * undefined. If the new space cannot be allocated, then the contents pointed
 * to by ptr are unchanged. If size is zero, then the memory block is completely
 * freed.
 */
void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL) {
        ptr = malloc(size);
        return ptr;
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }
#ifdef XLTCFG_SUPPORT_MEMMNG
    return (void *)xlt_mem_realloc((void *)OS_EXTEND_MEM_ADDR, (void *)ptr, (UINT32)size);
#else
    return NULL;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
