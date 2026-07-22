#include "string.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

char *strnstr(const char *s, const char *find, size_t slen)
{
    size_t len1, len2, tmp;

    len2 = strlen(find);
    if (len2 == 0) {
        return (char *)s;
    }
    if (slen < len2) {
        return NULL;
    }
    len1 = strlen(s);
    tmp = slen > len1 ? len1 : slen;

    for (; tmp >= len2; tmp--) {
        if (!strncmp(s, find, len2)) {
            return (char *)s;
        }
        s++;
    }

    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
