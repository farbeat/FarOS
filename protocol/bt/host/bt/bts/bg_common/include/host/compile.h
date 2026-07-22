/*
 * Copyright (c) @CompanyNameMagicTag 2015-2020. All rights reserved.
 */

#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "datatype.h"
#include "sysdep.h"

#ifndef abs
#define abs(x) (((x) < 0) ? (-(x)) : (x))
#endif

#ifdef BTH_TEST
#define INLINE
#endif

static INLINE int str_len(const char *str)
{
    return strlen(str);
}

static INLINE uint8_t *str_str(const char *s, const char *find)
{
    return (uint8_t *)strstr(s, find);
}
static INLINE int str_cmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

static INLINE char* str_tok(char* str, const char* delim)
{
    static unsigned char s_map[32] = {0}; /* delimiter map table */
    static char* s_str = NULL;
    char* str_temp = str;
    char* delim_temp = delim;
#define set_char_map(c, m) (do { (m)[(c) >> 3] |= (1 << ((c) & 7)); } while (0))
#define chk_char_map(c, m) (((m)[(c) >> 3] & (1 << ((c) & 7))) && (c))

    if (str_temp == NULL) {
        str_temp = s_str;
        s_str = NULL;
    }

    if ((str_temp == NULL) || (*str_temp == '\0') || (delim == NULL)) {
        return NULL;
    }

    if (*delim_temp == '\0') {
        return str_temp;
    }

    /* generate delimiter bits map */
    (void)memset_s(s_map, sizeof(s_map), 0x00, sizeof(s_map));
    while (*delim_temp != '\0') {
        set_char_map(*delim_temp, s_map);
        delim_temp++;
    }

    /* Find token beginning */
    while (chk_char_map(*str_temp, s_map)) {
        str_temp++;
    }

    for (s_str = str_temp; *s_str != '\0'; s_str++) {
        if (chk_char_map(*s_str, s_map)) {
            *s_str++ = '\0';
            break;
        }
    }
    if (s_str == str_temp) {
        s_str = str_temp = NULL;
    }

    return str_temp;
}

static INLINE uint8_t *mem_chr(char *s, char c, int size)
{
    char *s_temp = s;
    int size_temp = size;
    if (s_temp == NULL) {
        return NULL;
    }
    while ((size_temp > 0) && (c != s_temp[0])) {
        s_temp++;
        size_temp--;
    }
    return ((size_temp == 0) ? NULL : (uint8_t *)s_temp);
}

static INLINE int wide_to_utf8(char* lpw, char* lpa, int len)
{
    return wide_to_utf8(lpw, lpa, len);
}
static INLINE int utf8_to_wide(char* lpa, char* lpw, int len)
{
    return wide_to_utf8(lpa, lpw, len);
}

#define strlen(str)             (str_len((const char*)(str)))
#define strcmp(s1, s2)          (str_cmp((const char*)(s1), (const char*)(s2)))
#define strtok(src, delim)      (str_tok((char *)(src), (const char *)(delim)))
#define strstr(s, find)         (str_str((const char *)(s), (const char *)(find)))
#define memchr(s, c, size)         (mem_chr((char *)(s), (char)(c), (int)(size)))

#ifdef SHORTEN_STRING
    #undef SHORTEN_STRING
#endif
#ifdef str_stri
    #undef str_stri
#endif
#define str_stri(str, charset)   (bt_util_str_stri((const char *)(str), (const char *)(charset)))
#ifdef tolower
    #undef tolower
#endif
#define tolower(c) (('A' <= (c) && (c) <= 'Z') ? ('a' + (c) - 'A') : (c))
#define wide_to_utf8(lpw, lpa, len) wide_to_utf8((char *)(lpw), (char *)(lpa), (int)(len))
#define utf8to_wide(lpa, lpw, len) utf8_to_wide((char *)(lpa), (char *)(lpw), (int)(len))

#endif /* end of _COMPILE_H_ */
