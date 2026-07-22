#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

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

    // TODO : implement based on the specific memory management module.
    return;
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

    // TODO : implement based on the specific memory management module.
    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
