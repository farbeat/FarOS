/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test nv header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#ifndef SRC_APP_TEST_SUITE_SHARED_NOT_SECURITY_CORE_TESTS_TEST_SOC_KV_H_
#define SRC_APP_TEST_SUITE_SHARED_NOT_SECURITY_CORE_TESTS_TEST_SOC_KV_H_

#include "td_type.h"

void add_kv_functions(void);

typedef struct {
    td_u32 total;                /* Total space of the memory pool (unit: byte).
                                    CNcomment:内存池总大小（单位：byte）CNend */
    td_u32 used;                 /* Used space of the memory pool (unit: byte).
                                    CNcomment:内存池已经使用大小（单位：byte）CNend */
    td_u32 free;                 /* Free space of the memory pool (unit: byte).
                                    CNcomment:内存池剩余空间（单位：byte）CNend */
    td_u32 free_node_num;        /* Number of free nodes in the memory pool.
                                    CNcomment:内存池剩余空间节点个数 CNend */
    td_u32 used_node_num;        /* Number of used nodes in the memory pool.
                                    CNcomment:内存池已经使用的节点个数 CNend */
    td_u32 max_free_node_size;   /* Maximum size of the node in the free space of the memory pool (unit: byte).
                                    CNcomment:内存池剩余空间节点中最大节点的大小（单位：byte）CNend */
    td_u32 peek_size;            /* Peak memory usage of the memory pool.CNcomment:内存池使用峰值CNend */
} ext_mdm_mem_info;


#endif /* SRC_APP_TEST_SUITE_SHARED_NOT_SECURITY_CORE_TESTS_TEST_SOC_KV_H_ */
