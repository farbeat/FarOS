#include "string.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

char *strnchr(const char *s, size_t count, int c)
{
    if (s == NULL) {
        return NULL;
    }

    while ((count-- != 0) && (*s != '\0')) {
        if (*s == (char)c) {
            return (char *)s;
        }
        ++s;
    }
    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
