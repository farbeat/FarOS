/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: osal adapt header \n
 * Author: @CompanyNameTag \n
 */

#ifndef OSAL_ADAPT_H
#define OSAL_ADAPT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef void (*print_cb_t)(const char *fmt, ...);

void osal_dcache_flush_all(void);
void osal_register_print(print_cb_t print);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
