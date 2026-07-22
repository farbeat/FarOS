/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: API of audio debug
 * Author: audio
 */

#ifndef __AUDIO_DEBUG_H__
#define __AUDIO_DEBUG_H__

#include "chip_type.h"

#if (SAP_OS_TYPE == os_type_xtensa)
#ifdef SAP_IPC_LOG_SUPPORT
#include "audio_debug_ipc.h"
#else
#include "audio_debug_uart.h"
#endif /* SAP_IPC_LOG_SUPPORT */
#else
#ifdef BRANDY_PROT /* bt core id */
#include "audio_debug_uart.h"
#else
#ifdef SAP_DIAG_LOG_SUPPORT
#ifdef SW_UART_DEBUG /* platform uart support macro */
#include "audio_debug_uart.h"
#else
#include "audio_debug_diag.h"
#endif /* SW_UART_DEBUG */
#else
#include "audio_debug_uart.h"
#endif /* SAP_DIAG_LOG_SUPPORT */
#endif /* BRANDY_PROT */
#endif /* SAP_OS_TYPE */

#endif /* __AUDIO_DEBUG_H__ */
