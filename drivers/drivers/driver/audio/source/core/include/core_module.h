/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for common module
 * Author: audio
 */

#ifndef __CORE_MODULE_H__
#define __CORE_MODULE_H__

#include "connector_list.h"
#include "audio_struct.h"

#include "osal_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ******************************************************************************
 * 宏开关定义区（例如编译、调试、打印开关）
 * ***************************************************************************** */
#define MOD_ADP_MAX_NUM 8
#define MOD_MSG_MAX_NUM 4
#define MOD_PORT_MAX_NUM 2
#define MOD_VAD_MAX_NUM 1
#define MOD_AI_MAX_NUM 2
#define MOD_SEA_MAX_NUM 2
#define MOD_AO_MAX_NUM 2
#define MOD_AOP_MAX_NUM 2
#define MOD_TRACK_MAX_NUM 2
#define MOD_ADEC_MAX_NUM 2
#define MOD_AENC_MAX_NUM 1
#define MOD_ANC_MAX_NUM 0
#define MOD_ANPU_MAX_NUM 0
#define MOD_HAID_MAX_NUM 1
#define MOD_DPM_MAX_NUM 1

/* ******************************************************************************
 * 宏操作定义区
 * ***************************************************************************** */

#define core_mod_get_handle(mod) ((mod)->head.mid.u32)
#define core_mod_get_type(mod) ((mod)->head.mid.bits.type)

/* ******************************************************************************
 * 枚举类型定义区
 * ***************************************************************************** */
/* * MODULE类型枚举定义 */
typedef enum {
    SAP_MOD_NULL = 0x00,
    SAP_MOD_ADP,
    SAP_MOD_PORT,
    SAP_MOD_AI,
    SAP_MOD_VAD,
    SAP_MOD_SEA,
    SAP_MOD_ADEC,
    SAP_MOD_AENC,
    SAP_MOD_TRACK,
    SAP_MOD_AO,
    SAP_MOD_AOP,
    SAP_MOD_ANC,
    SAP_MOD_MSG,
    SAP_MOD_SYS,
    SAP_MOD_DPM,
    SAP_MOD_MAX
} sap_mod_type;

/* ******************************************************************************
 * 数据结构类型和联合体类型定义
 * ***************************************************************************** */
typedef td_s32 (*core_mod_event_proc)(td_void *handle, td_s32 event, td_void *param, td_void *context);

/* MODULE标识描述 */
typedef union {
    struct {
        td_u8 type;   /* 模块类型::sap_mod_type */
        td_u8 index;  /* 模块索引 */
        td_u8 sub_id; /* 模块子索引 */
        td_u8 reserved;
    } bits;
    td_u32 u32;
} sap_mod_id;

static inline td_u32 get_sap_mod_type(td_handle handle)
{
    sap_mod_id mod_id = {
        .u32 = handle,
    };

    return mod_id.bits.type;
}

static inline td_u32 get_sap_mod_index(td_handle handle)
{
    sap_mod_id mod_id = {
        .u32 = handle,
    };

    return mod_id.bits.index;
}

/* MODULE头部参数描述结构 */
typedef struct mod_head_p {
    sap_mod_id mid;
    struct mod_head_p *prev; /* 上一个模块 */
    struct mod_head_p *next; /* 下一个模块 */
    core_mod_event_proc mod_event_proc;
    td_void *context;
    td_u32 ext_id;
    const td_void *ops;
} sap_mod_head, *sap_mod_handle;

typedef struct {
    td_s32 (*attach_input_connector)(sap_mod_handle module, connector_handle connector);
    td_s32 (*detach_input_connector)(sap_mod_handle module, connector_handle connector);
    td_s32 (*detach_output_connector)(sap_mod_handle module, connector_handle connector, td_u32 data_source);
    td_s32 (*attach_output_connector)(sap_mod_handle module, connector_handle connector, td_u32 data_source);
} sap_mod_ops;

/* MODULE参数描述结构 */
typedef struct {
    td_handle lock; /* 同步控制块互斥锁 */
    sap_mod_head free_list;
    sap_mod_head used_list;

    td_u32 node_size;    /* 单元大小 */
    td_u8 node_count;    /* 容器容量 */
    td_u8 free_num;      /* 空闲个数 */
    td_u8 used_num;      /* 使用个数 */
    td_u8 mod_type;      /* 模块类型::sap_mod_type */
    td_void *base_addr;  /* 容器内存基地址 */
    td_u8 pool_start[4]; /* 模块基地址 */
} sap_mod_pool, *sap_mod_pool_handle;

/* * 模块初始化函数
 *
 * @param[in]  type    模块类型::sap_mod_type
 * @param[in]  size    模块大小
 * @param[in]  count   模块总数
 * @param[out] pool    模块池句柄
 *
 * @retval ::AUDIO_SUCCESS           初始化成功
 * @retval ::MOD_E_ERR_XXX     初始化失败
 */
td_s32 core_mod_init(sap_mod_pool_handle *pool, td_u8 type, td_u32 size, td_u16 count);

/* * 模块去初始化函数
 *
 * @param[in]  pool    模块池句柄
 *
 * @retval ::AUDIO_SUCCESS           去初始化成功
 * @retval ::AUDIO_ERR_XXX     去初始化失败
 */
td_s32 core_mod_deinit(sap_mod_pool_handle pool);

/* * 获取指定模块ID的链表节点
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  mod_id      模块标识
 * @param[out] module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           获取成功
 * @retval ::AUDIO_ERR_XXX     获取失败
 */
td_s32 core_mod_get_module(sap_mod_pool_handle pool, td_u32 mod_id, sap_mod_handle *module);

/* * 从模块资源池中分配一个模块来使用
 * 从FREE链表中取出表头节点放到USED列表中
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           分配成功
 * @retval ::AUDIO_ERR_XXX     分配失败
 */
td_s32 core_mod_alloc(sap_mod_pool_handle pool, sap_mod_handle *module);

/* * 归还模块节点给模块资源池
 * 从USED链表中取出指定节点放到FREE列表中
 * @param[in]  pool        模块池句柄
 * @param[in]  module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           归还成功
 * @retval ::AUDIO_ERR_XXX     归还失败
 */
td_s32 core_mod_free(sap_mod_pool_handle pool, sap_mod_handle module);

/* * 从模块资源池中根据模块ID分配一个模块来使用
 * 从USED链表中取出指定节点放到FREE列表中
 * @param[in]  pool        模块池句柄
 * @param[in]  mod_id      模块标识
 * @param[out] module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           归还成功
 * @retval ::AUDIO_ERR_XXX     归还失败
 */
td_s32 core_mod_alloc_by_id(sap_mod_pool_handle pool, td_u32 mod_id, sap_mod_handle *module);

/* * 归还指定索引的模块节点给模块资源池
 * 在USED链表中搜索指定索引的模块节点，并
 * 从USED链表中取出该模块节点放到FREE列表中
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  mod_id      模块标识
 *
 * @retval ::AUDIO_SUCCESS           归还成功
 * @retval ::AUDIO_ERR_XXX     归还失败
 */
td_s32 core_mod_free_by_id(sap_mod_pool_handle pool, td_u32 mod_id);

/* * 注册外部事件处理回调到模块节点
 *
 * @param[in]  module      模块句柄
 * @param[in]  event_proc  事件处理回调
 * @param[in]  context   事件处理回调中透传的上下文
 */
td_void core_mod_register_event_proc(sap_mod_handle module, core_mod_event_proc event_proc, td_void *context);

/* * 通过事件回调上报事件给模块会话处理
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  module      模块句柄
 * @param[in]  event       外部事件ID
 * @param[in]  param       外部事件参数
 *
 * @retval ::AUDIO_SUCCESS           上报返回成功
 * @retval ::AUDIO_ERR_XXX     上报返回失败
 */
td_s32 core_mod_report_event(sap_mod_handle module, td_s32 event, td_void *param);

/* * 设置模块外部ID，用于在USED_LIST中的排序
 *
 * @param[in]  module  模块句柄
 * @param[in]  ext_id  模块外部ID
 *
 * @retval ::AUDIO_SUCCESS          设置成功
 * @retval ::AUDIO_ERR_XXX    设置失败
 */
td_s32 core_mod_set_eid(sap_mod_handle module, td_u32 ext_id);

/* * 根据模块外部ID查找模块句柄
 *
 * @param[in]  pool    模块池句柄
 * @param[in]  ext_id  模块外部ID
 * @param[out] module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           查找成功
 * @retval ::AUDIO_ERR_XXX     查找失败
 */
td_s32 core_mod_get_module_by_eid(sap_mod_pool_handle pool, td_u32 ext_id, sap_mod_handle *module);

/* * 获取指定模块ID的链表节点
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  mod_id      模块标识
 * @param[out] mod_idx     模块索引
 *
 * @retval ::AUDIO_SUCCESS           获取成功
 * @retval ::AUDIO_ERR_XXX     获取失败
 */
td_s32 core_mod_get_index(sap_mod_pool_handle pool, td_u32 mod_id, td_u32 *mod_idx);

/* * 获取指定模块ID的链表节点
 *
 * @param[in]  pool        模块池句柄
 * @param[in]  mod_idx     模块索引
 * @param[out] module      模块句柄
 *
 * @retval ::AUDIO_SUCCESS           获取成功
 * @retval ::AUDIO_ERR_XXX     获取失败
 */
td_s32 core_mod_get_handle_by_index(sap_mod_pool_handle pool, td_u32 mod_idx, sap_mod_handle *module);

/* * 广播事件给所有模块
 *
 * @param[in]  pool    模块池句柄
 * @param[in]  event   事件ID
 * @param[in]  param   事件携带的参数
 *
 * @retval 事件处理成功的模块数
 */
td_s32 core_mod_broadcast_event(sap_mod_pool_handle pool, td_s32 event, td_void *param);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_MODULE_H__ */
