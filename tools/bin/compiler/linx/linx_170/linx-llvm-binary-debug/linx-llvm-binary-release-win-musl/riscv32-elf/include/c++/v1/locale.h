// -*- C++ -*-
//===---------------------------- locale.h --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_LOCALE_H
#define _LIBCPP_LOCALE_H

/*
    locale.h synopsis

Macros:

    LC_ALL
    LC_COLLATE
    LC_CTYPE
    LC_MONETARY
    LC_NUMERIC
    LC_TIME

Types:

    lconv

Functions:

   setlocale
   localeconv

*/

#include <__config>

#if defined(_LIBCPP_HAS_NO_LOCALIZATION)
#   error "Localization is not supported by this configuration of libc++"
#endif

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#   pragma GCC system_header
#endif

#include_next <locale.h>

#ifdef _NEWLIB_VERSION
#ifdef __cplusplus

extern "C" {
typedef struct __locale_t *locale_t;
locale_t uselocale (locale_t);

#define LC_ALL_MASK   (1 << LC_ALL)
#define LC_COLLATE_MASK   (1 << LC_COLLATE)
#define LC_CTYPE_MASK   (1 << LC_CTYPE)
#define LC_MONETARY_MASK  (1 << LC_MONETARY)
#define LC_NUMERIC_MASK   (1 << LC_NUMERIC)
#define LC_TIME_MASK    (1 << LC_TIME)
#define LC_MESSAGES_MASK  (1 << LC_MESSAGES)

int isascii (int __c);
size_t strftime_l(char *s, size_t max, const char *format, const struct tm *tm, locale_t);

double  strtod_l (const char *__restrict, char **__restrict, locale_t);
float strtof_l (const char *__restrict, char **__restrict, locale_t);
extern long double strtold_l (const char *__restrict, char **__restrict,
                              locale_t);
long long strtoll_l (const char *__restrict, char **__restrict, int, locale_t);
unsigned long long strtoull_l (const char *__restrict, char **__restrict, int,
                               locale_t __loc);

int isdigit_l (int __c, locale_t __l);
int islower_l (int __c, locale_t __l);
int isupper_l (int __c, locale_t __l);
int isxdigit_l(int __c, locale_t __l);
int tolower_l (int __c, locale_t __l);
int toupper_l (int __c, locale_t __l);

locale_t newlocale (int, const char *, locale_t);
void freelocale (locale_t);
locale_t duplocale (locale_t);
locale_t uselocale (locale_t);

int  strcoll_l (const char *, const char *, locale_t);
char  *strerror_l (int, locale_t);
size_t   strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);

int wcscoll_l (const wchar_t *, const wchar_t *, locale_t);
size_t wcsxfrm_l (wchar_t *__restrict, const wchar_t *__restrict, size_t,
                  locale_t);

int  iswalpha_l (wint_t, locale_t);
int  iswalnum_l (wint_t, locale_t);
int  iswblank_l (wint_t, locale_t);
int  iswcntrl_l (wint_t, locale_t);
int  iswctype_l (wint_t, wctype_t, locale_t);
int  iswdigit_l (wint_t, locale_t);
int  iswgraph_l (wint_t, locale_t);
int  iswlower_l (wint_t, locale_t);
int  iswprint_l (wint_t, locale_t);
int  iswpunct_l (wint_t, locale_t);
int  iswspace_l (wint_t, locale_t);
int  iswupper_l (wint_t, locale_t);
int  iswxdigit_l (wint_t, locale_t);
wint_t towctrans_l (wint_t, wctrans_t, locale_t);
wint_t towupper_l (wint_t, locale_t);
wint_t towlower_l (wint_t, locale_t);
wctrans_t wctrans_l (const char *, locale_t);
wctype_t wctype_l (const char *, locale_t);
}

#endif
#endif

#endif  // _LIBCPP_LOCALE_H
