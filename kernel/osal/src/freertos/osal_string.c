/*
 * Copyright (c) CompanyNameMagicTag 2016-2022. All rights reserved.
 * Description: osal string source file.
 */

#include <string.h>

int osal_strncmp(const char *s1, const char *s2, unsigned long size)
{
    return strncmp(s1, s2, (size_t)size);
}

int osal_strcasecmp(const char *s1, const char *s2)
{
    return strcasecmp(s1, s2);
}

unsigned int osal_strlen(const char *s)
{
    return strlen(s);
}

int osal_memcmp(const void *cs, const void *ct, int count)
{
    return memcmp(cs, ct, (size_t)count);
}

int osal_strcmp(const char *cs, const char *ct)
{
    return strcmp(cs, ct);
}

long osal_strtol(const char *cp, char **endp, unsigned int base)
{
    return strtol(cp, endp, (int)base);
}
 
unsigned long osal_strtoul(const char *cp, char **endp, unsigned int base)
{
    return strtoul(cp, endp, (int)base);
}
