/*
 * Copyright (c) CompanyNameMagicTag 2012-2021. All rights reserved.
 * Description:  os function handler module header files
 */
#ifndef OS_HANDLER_H
#define OS_HANDLER_H

#include "stdlib.h"
#include "string.h"
#include "xlt_slab_pri.h"
#include "xlt_memory_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

uint32_t os_mem_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_mem_nodes_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_task_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_queue_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_swtmr_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_sys_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_semphr_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_mutex_handler(uint8_t *para, uint32_t para_len, char *str_cmd);
uint32_t os_hwi_handler(uint8_t *para, uint32_t para_len, char *str_cmd);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_HANDLER_H */
