/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Stricmp functions.
 * Author:
 * Create:
 */

#include <lib_utils.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define isupper(c) (((c) >= 'A') && ((c) <= 'Z'))

int stricmp(const char *s1, const char *s2)
{
    unsigned char c1;
    unsigned char c2;
    int r;

    for (;;) {
        c1 = (unsigned char)*s1++;
        if (isupper(c1)) {
            c1 += 'a' - 'A';
        }

        c2 = (unsigned char)*s2;
        if (isupper(c2)) {
            c2 += 'a' - 'A';
        }

        r = (char)c1 - (char)c2;
        if (r != 0) {
            break;
        }

        if (!(*s2++)) {
            break;
        }
    }
    return r;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
