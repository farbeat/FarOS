/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT Service Task Manager module.
 */
#ifndef __BTSRV_TASK_H__
#define __BTSRV_TASK_H__

#include <td_base.h>

#define EXT_MSG_INVALID_MSG_NUM 0xFFFFFFFF
#define EXT_SYS_MSG_PARAM_NUM_MAX 4
typedef struct {
    uint32_t msg_id; /* < Message ID.CNcomment:消息ID CNend */
    uintptr_t param[EXT_SYS_MSG_PARAM_NUM_MAX]; /* < Message parameter.CNcomment:消息参数 CNend */
} ext_sys_queue_msg;

/* 蓝牙服务任务消息ID */
enum {
    BTSRV_MSG_ID_LOCAL_DEVICE_MANAGER,      /* 本地设备管理模块消息 */
    BTSRV_MSG_ID_CONNECTION_MANAGER,        /* 连接管理模块消息 */
    BTSRV_MSG_ID_REMOTE_DEV_MANAGER,        /* 远端设备管理模块消息 */
    BTSRV_MSG_ID_A2DP_SERVICE,              /* A2DP服务模块消息 */
    BTSRV_MSG_ID_A2DP_SNK_SERVICE,          /* A2DP SNK服务模块消息 */
    BTSRV_MSG_ID_HFP_HF_SERVICE,            /* HFP-HF服务模块消息 */
    BTSRV_MSG_ID_HFP_AG_SERVICE,            /* HFP-AG服务模块消息 */
    BTSRV_MSG_ID_SPP_SERVICE,               /* SPP服务模块消息 */
    BTSRV_MSG_ID_AVRCP_SERVICE,             /* AVRCP服务模块消息 */
    BTSRV_MSG_ID_GATT_SERVER,               /* GATT Server模块消息 */
    BTSRV_MSG_ID_GATT_CLIENT,               /* GATT Client模块消息 */
    BTSRV_MSG_ID_PAN_SERVICE,               /* PAN服务模块消息 */
    BTSRV_MSG_ID_SLE_DEV_DISCOVERY,         /* SLE 设备发现模块消息 */
    BTSRV_MSG_ID_SLE_CONN_MANAGER,          /* SLE 设备发现模块消息 */
    BTSRV_MSG_ID_SLE_SSAPS,                 /* SLE ssap server */
    BTSRV_MSG_ID_SLE_HADM,                  /* SLE hadm管理 */
    BTSRV_MSG_ID_SLE_FACTORY,               /* SLE 产测管理 */
    BTSRV_MSG_ID_MAX,
};

typedef void (*btsrv_task_msg_handle_func_type)(const ext_sys_queue_msg *msg);

typedef struct {
    uint16_t task_prio;  /* Task priority.CNcomment:任务优先级 CNend */
    uint32_t stack_size; /* Task stack size.CNcomment:栈大小 CNend */
    uint32_t task_policy; /* Task schedule policy. Not used by default.CNcomment:任务调度策略。默认没有使用。CNend */
    uint32_t task_nice;   /* Task nice value. Not used by default.CNcomment:任务优先权。默认没有使用。CNend */
    uint32_t task_cpuid;  /* CPUID to which the task belongs. Not used by default.
                             CNcomment:任务所属的CPUID。默认没有使用。CNend */
    td_char *task_name; /* Task name.CNcomment:任务名 CNend */
    void *resved; /* Reserved. Not used by default.CNcomment:预留字段。默认没有使用。CNend */
} btsrv_task_attr;

void btsrv_task_init(void);
void btsrv_handle_task_msg(ext_sys_queue_msg *msg);
ext_errno btsrv_send_simple_msg(uint32_t msg_id, uintptr_t param0,
    uintptr_t param1, uintptr_t param2, uintptr_t param3);
void btsrv_register_msg_handle_func(int32_t msg_id, btsrv_task_msg_handle_func_type func);

#endif /* __BTSRV_TASK_H__ */
