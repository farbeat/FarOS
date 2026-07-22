/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides key deriv driver\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef KEY_DERIV_H
#define KEY_DERIV_H

#include <hal_key_deriv.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

errcode_t key_deriv_init(void);
void key_deriv_deinit(void);
void key_deriv_start(key_deriv_attr_t *key_deriv_attr);
bool key_deriv_get_result(deriv_key_id_t key_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
