/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: sdk common
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STACK_API_H
#define STACK_API_H

#include <stdint.h>
#include "soc_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HEADER_FIELD_LEN 1
#define BT_STACK_SHIFT_8 8

#define IPC_MAX_FIELD_LEN 24

/* 同步或者异步接口调用 */
typedef enum {
    STACK_API_SYNC = 0,
    STACK_API_ASYNC,
} t_stack_api_flag;

typedef void (stack_api_cfm_cb)(void *context, uint32_t result, void *parameter);

struct handle_param {
    uint16_t opcode;
    uint8_t field_len;
    uint8_t field_id[0];
};

struct handle_struct {
    uint8_t sync_flag;    /* 用于标记调用该接口的操作是异步还是同步 */
    uint16_t opcode;
    uint8_t field_len;
    uint8_t *field_id;
    osal_semaphore sem;    /* 针对同步调用模式下 , 需要等待的信号量的handle id */
    uint16_t out_len;       /* 调用结束后结果长度 */
    uint8_t *out_data;     /* 调用结束后结果数据 */
    stack_api_cfm_cb *cfm_cbk;    /* 针对异步调用的场景，需要注册的回调接口 */
};

void stack_mpc_init_handle(void);
void stack_mpc_deinit_handle(void);
struct handle_struct *stack_mpc_find_handle(uint16_t opcode, uint8_t field_len, uint8_t *field_id);
struct handle_struct *stack_mpc_create_handle(uint8_t sync_flag, struct handle_param *param, void *cbk);
uint32_t stack_mpc_wait_handle(struct handle_struct *hdl);
uint8_t stack_mpc_delete_handle(struct handle_struct *hdl);
uint8_t stack_mpc_activate_handle(uint16_t opcode, uint8_t field_len, uint8_t *field_id,
                                  uint16_t out_len, const uint8_t *out_data);
int32_t stack_mpc_msg_send_sync(struct handle_param *param, uint16_t in_len,
                                uint8_t *in_para, uint16_t *out_len, uint8_t **out_value);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STACK_API_H */
