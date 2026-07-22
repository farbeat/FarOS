/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description:  OAM TRACE
 * Author: @CompanyNameTag
 * Create: 2022-03-22
 */

#ifndef OAM_TRACE_H
#define OAM_TRACE_H

#include <stdint.h>
#ifdef USE_CMSIS_OS
#ifdef __LITEOS__
#include "arch/exception.h"
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @brief  Rigister Exception handle function.
 */
void register_os_exec_hook(void);

#if defined(USE_CMSIS_OS) && defined(__LITEOS__)
/**
 * @brief  Invoked by exception to dump exception information.
 * @param  exc_type Exception type.
 * @param  exc_buff_addr Exception buff addr.
 */
void exec_fault_handler(uint32_t exc_type, const ExcContext *exc_buff_addr);
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif