/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: memory debug implementation
 */


#include "xlt_memory.h"
#include "xlt_memory_pri.h"
#include "xlt_slab_pri.h"
#include "xlt_slab.h"
#include "src/bestfit/xlt_memory_internal.h"
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
#include "los_exc_pri.h"
#endif
#include "asm/memmap_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void WriteExcInfoToBuf(const char *format, ...);

static unsigned int os_mem_info_get(const void *pool, xlt_mem_pool_status *pool_status)
{
    const xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    xlt_mem_dyn_node *tmp_node = NULL;
    unsigned int total_used_size = 0;
    unsigned int total_free_size = 0;
    unsigned int max_free_node_size = 0;
    unsigned int used_node_num = 0;
    unsigned int free_node_num = 0;

    tmp_node = (xlt_mem_dyn_node *)xlt_mem_end_node(pool, pool_info->pool_size);
    tmp_node = (xlt_mem_dyn_node *)xlt_mem_align(tmp_node, XLT_MEM_ALIGN_SIZE);
    if (!xlt_mem_magic_valid(tmp_node->self_node.magic)) {
        print_err("wrong mem pool addr: %p\n, line:%d", pool_info, __LINE__);
        return XLT_NOK;
    }

    for (tmp_node = xlt_mem_first_node(pool_info); tmp_node <= xlt_mem_end_node(pool_info, pool_info->pool_size);
        tmp_node = xlt_mem_next_node(tmp_node)) {
        if (!xlt_mem_node_get_used_flag(tmp_node->self_node.size_and_flag)) {
            ++free_node_num;
            total_free_size += xlt_mem_node_get_size(tmp_node->self_node.size_and_flag);
            if (max_free_node_size < xlt_mem_node_get_size(tmp_node->self_node.size_and_flag))
                max_free_node_size = xlt_mem_node_get_size(tmp_node->self_node.size_and_flag);
        } else {
            ++used_node_num;
            total_used_size += xlt_mem_node_get_size(tmp_node->self_node.size_and_flag);
        }
    }

    pool_status->uw_total_used_size = total_used_size;
    pool_status->uw_total_free_size = total_free_size;
    pool_status->uw_max_free_node_size = max_free_node_size;
    pool_status->uw_used_node_num = used_node_num;
    pool_status->uw_free_node_num = free_node_num;

    return XLT_OK;
}


void os_mem_info_print(const void *pool)
{
    const xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    xlt_mem_pool_status status = { 0 };
    if (xlt_mem_info_get(pool, &status) == XLT_NOK)
        return;

    xlt_printk("\npool addr          pool size    used size     free size    "
        "max free node size   used node num     free node num\n");
    xlt_printk("---------------    --------     -------       --------     "
        "--------------       -------------      ------------\n");
    xlt_printk("%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x\n", pool_info->pool,
        pool_info->pool_size, status.uw_total_used_size, status.uw_total_free_size, status.uw_max_free_node_size,
        status.uw_used_node_num, status.uw_free_node_num);
}


unsigned int xlt_mem_info_get(const void *pool, xlt_mem_pool_status *pool_status)
{
    const xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned int ret;
    unsigned int int_save;

    if (pool_status == NULL) {
        print_err("can't use NULL addr to save info\n");
        return XLT_NOK;
    }

    if ((pool_info == NULL) || ((UINTPTR)pool != (UINTPTR)pool_info->pool)) {
        print_err("wrong mem pool addr: %p, line:%d\n", pool_info, __LINE__);
        return XLT_NOK;
    }

    int_save = (unsigned int)xlt_mem_lock();
    ret = os_mem_info_get(pool_info, pool_status);
    if (ret != XLT_OK) {
        xlt_printk("mem get status fail ret = %u\n", ret);
        xlt_mem_unlock(int_save);
        return ret;
    }

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    xlt_slab_status slab_status = {0};
    UINT32 max_free_node_size;
    ret = os_slab_statistics_get(pool, &slab_status);
    if (ret != XLT_OK) {
        xlt_printk("slab get status fail ret = %u\n", ret);
        xlt_mem_unlock(int_save);
        return ret;
    }

    pool_status->uw_total_used_size -= slab_status.free_size;
    pool_status->uw_total_free_size += slab_status.free_size;
    max_free_node_size = os_slab_get_max_free_blk_size(pool);
    pool_status->uw_max_free_node_size = max_free_node_size > pool_status->uw_max_free_node_size ?
        max_free_node_size : pool_status->uw_max_free_node_size;
    pool_status->uw_used_node_num += slab_status.alloc_count;
    pool_status->uw_free_node_num += slab_status.free_count;
#endif
    xlt_mem_unlock(int_save);

    return ret;
}


unsigned int xlt_mem_total_used_get(void *pool)
{
    xlt_mem_dyn_node *tmp_node = NULL;
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned int mem_used = 0;
    unsigned int int_save;

    if (pool == NULL)
        return XLT_NOK;

    int_save = (unsigned int)xlt_mem_lock();

    for (tmp_node = xlt_mem_first_node(pool); tmp_node <= xlt_mem_end_node(pool, pool_info->pool_size);
        tmp_node = xlt_mem_next_node(tmp_node)) {
        if (xlt_mem_node_get_used_flag(tmp_node->self_node.size_and_flag))
            mem_used += xlt_mem_node_get_size(tmp_node->self_node.size_and_flag);
    }

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    xlt_slab_status slab_status = {0};
    UINT32 ret = os_slab_statistics_get(pool, &slab_status);
    if (ret != XLT_OK) {
        xlt_printk("slab get status fail ret = %u\n", ret);
        xlt_mem_unlock(int_save);
        return ret;
    }

    mem_used -= slab_status.free_size;
#endif

    xlt_mem_unlock(int_save);

    return mem_used;
}

static UINTPTR os_mem_task_id_get(const xlt_mem_pool_info *pool_info, const void *ptr)
{
    xlt_mem_dyn_node *tmp_node = NULL;
    unsigned int int_save;

    int_save = (unsigned int)xlt_mem_lock();

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    UINTPTR task_id = 0;
    if (os_slab_task_id_get(ptr, &task_id) == TRUE) {
        xlt_mem_unlock(int_save);
        return task_id;
    }
#endif

    for (tmp_node = xlt_mem_first_node(pool_info); tmp_node <= xlt_mem_end_node(pool_info, pool_info->pool_size);
        tmp_node = xlt_mem_next_node(tmp_node)) {
        if ((UINTPTR)ptr < (UINTPTR)tmp_node) {
            if (xlt_mem_node_get_used_flag(tmp_node->self_node.pre_node->self_node.size_and_flag)) {
                xlt_mem_unlock(int_save);
                return tmp_node->self_node.pre_node->self_node.task_id;
            } else {
                xlt_mem_unlock(int_save);
                print_err("input ptr %p is belong to a free mem node\n", ptr);
                return OS_INVALID;
            }
        }
    }

    xlt_mem_unlock(int_save);

    return OS_INVALID;
}

static inline unsigned int os_mem_check_addr_range(const xlt_mem_pool_info *pool_info, const void *ptr)
{
    if ((ptr == NULL) || ((UINTPTR)ptr < (UINTPTR)xlt_mem_first_node(pool_info)) ||
        ((UINTPTR)ptr > (UINTPTR)xlt_mem_end_node(pool_info, pool_info->pool_size))) {
        return OS_INVALID;
    }

    return XLT_OK;
}


UINTPTR xlt_mem_task_id_get(const void *ptr)
{
    xlt_mem_pool_info *pool_info;
    unsigned int ret = XLT_OK;

#ifdef XLTCFG_MEM_MUL_POOL
    pool_info = (xlt_mem_pool_info *)os_mem_mul_pool_head_get();
    while (pool_info != NULL) {
        ret = os_mem_check_addr_range(pool_info, ptr);
        if (ret == XLT_OK) {
            break;
        }

        pool_info = pool_info->next_pool;
    }

    if (pool_info == NULL) {
        print_err("input ptr %p is out of system memory range\n", ptr);
        return OS_INVALID;
    }
#else
    pool_info = (xlt_mem_pool_info *)(void *)OS_EXTEND_MEM_ADDR;
    ret = os_mem_check_addr_range(pool_info, ptr);
    if (ret != XLT_OK) {
        print_err("input ptr %p is out of system memory range[%p, %p]\n", ptr, xlt_mem_first_node(pool_info),
            xlt_mem_end_node(pool_info, pool_info->pool_size));
        return OS_INVALID;
    }
#endif

    return os_mem_task_id_get(pool_info, ptr);
}


#ifdef XLTCFG_BASE_MEM_NODE_INTEGRITY_CHECK
#define NODEDUMPSIZE 64 /* the dump size of current broken node when memcheck error */

static unsigned int os_mem_pool_dlink_check(const xlt_mem_pool_info *pool, xlt_dl_list list_head)
{
    if (((UINTPTR)list_head.pst_prev < (UINTPTR)(pool + 1)) ||
        ((UINTPTR)list_head.pst_prev >= ((UINTPTR)pool + pool->pool_size)) ||
        ((UINTPTR)list_head.pst_next < (UINTPTR)(pool + 1)) ||
        ((UINTPTR)list_head.pst_next >= ((UINTPTR)pool + pool->pool_size)) ||
        !is_aligned(list_head.pst_prev, sizeof(void *)) || !is_aligned(list_head.pst_next, sizeof(void *)))
        return XLT_NOK;

    return XLT_OK;
}

/*
 * Description : show mem pool header info
 * Input       : pool --Pointer to memory pool
 */
void os_mem_pool_head_info_print(const void *pool)
{
    const xlt_mem_pool_info *pool_info = (const xlt_mem_pool_info *)pool;
    unsigned int dlink_num;
    unsigned int flag = 0;
    const xlt_multiple_dlink_head *dlink_head = NULL;

    if (!is_aligned(pool_info, sizeof(void *))) {
        print_err("wrong mem pool addr: %p, func:%s,line:%d\n", pool_info, __FUNCTION__, __LINE__);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
        WriteExcInfoToBuf("wrong mem pool addr: %p, func:%s,line:%d\n", pool_info, __FUNCTION__, __LINE__);
#endif
        return;
    }

    dlink_head = (const xlt_multiple_dlink_head *)(void *)(pool_info + 1);
    for (dlink_num = 0; dlink_num < OS_MULTI_DLNK_NUM; dlink_num++) {
        if (os_mem_pool_dlink_check(pool, dlink_head->list_head[dlink_num])) {
            flag = 1;
            print_err("DlinkHead[%u]: pst_prev:%p, pst_next:%p\n", dlink_num, dlink_head->list_head[dlink_num].pst_prev,
                dlink_head->list_head[dlink_num].pst_next);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
            WriteExcInfoToBuf("DlinkHead[%u]: pst_prev:%p, pst_next:%p\n", dlink_num,
                dlink_head->list_head[dlink_num].pst_prev, dlink_head->list_head[dlink_num].pst_next);
#endif
        }
    }
    if (flag) {
        xlt_printk("mem pool info: poolAddr:%p, pool_size:0x%x\n", pool_info->pool, pool_info->pool_size);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
        WriteExcInfoToBuf("mem pool info: poolAddr:%p, pool_size:0x%x\n", pool_info->pool, pool_info->pool_size);
#endif
    }
}

static unsigned int os_bestfit_integrity_check(const void *pool, xlt_mem_dyn_node **tmp_node,
    xlt_mem_dyn_node **pre_node)
{
    const xlt_mem_pool_info *pool_info = (const xlt_mem_pool_info *)pool;
    const unsigned char *end_pool = (const unsigned char *)pool + pool_info->pool_size;

    os_mem_pool_head_info_print(pool);

    *pre_node = xlt_mem_first_node(pool);
    for (*tmp_node = xlt_mem_first_node(pool); *tmp_node < xlt_mem_end_node(pool, pool_info->pool_size);
        *tmp_node = xlt_mem_next_node(*tmp_node)) {
        if (xlt_mem_node_get_used_flag((*tmp_node)->self_node.size_and_flag)) {
            if (!xlt_mem_magic_valid((*tmp_node)->self_node.magic)) {
                print_err("[%s], %d, memory check error!\n"
                    "memory used but magic num wrong, free_node_info.pst_prev(magic num):%p \n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_prev);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                    "memory used but magic num wrong, free_node_info.pst_prev(magic num):%p \n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_prev);
#endif
                return XLT_NOK;
            }
        } else { /* is free node, check free node range */
            if (!xlt_mem_middle_addr_open_end(pool, (*tmp_node)->self_node.free_node_info.pst_prev, end_pool)) {
                print_err("[%s], %d, memory check error!\n"
                    " free_node_info.pst_prev:%p is out of legal mem range[%p, %p]\n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_prev, pool, end_pool);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                    " free_node_info.pst_prev:%p is out of legal mem range[%p, %p]\n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_prev, pool, end_pool);
#endif
                return XLT_NOK;
            }
            if (!xlt_mem_middle_addr_open_end(pool, (*tmp_node)->self_node.free_node_info.pst_next, end_pool)) {
                print_err("[%s], %d, memory check error!\n"
                    " free_node_info.pst_next:%p is out of legal mem range[%p, %p]\n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_next, pool, end_pool);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                    " free_node_info.pst_next:%p is out of legal mem range[%p, %p]\n",
                    __FUNCTION__, __LINE__, (*tmp_node)->self_node.free_node_info.pst_next, pool, end_pool);
#endif
                return XLT_NOK;
            }
        }

        *pre_node = *tmp_node;
    }

    return XLT_OK;
}

unsigned int os_mem_integrity_check(const void *pool, xlt_mem_dyn_node **tmp_node, xlt_mem_dyn_node **pre_node)
{
    unsigned int ret = os_bestfit_integrity_check(pool, tmp_node, pre_node);
    if (ret != XLT_OK)
        return ret;

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    os_slab_block_node *slab_tmp_node = NULL;
    ret = os_slab_integrity_check(pool, &slab_tmp_node);
    if (ret != XLT_OK) {
        xlt_printk("[%s:%d] memory errno:0x%x ptr:0x%x", __FUNCTION__, __LINE__, XLT_ERRNO_MEMCHECK_ERR,
            (void *)slab_tmp_node);
    }
#endif

    return XLT_OK;
}

void os_dump_mem_byte(size_t length, UINTPTR addr)
{
    size_t data_len;
    UINTPTR *align_addr = NULL;
    unsigned int count = 0;

    data_len = ALIGN(length, sizeof(UINTPTR));
    align_addr = (UINTPTR *)TRUNCATE(addr, sizeof(UINTPTR));
    if ((data_len == 0) || (align_addr == NULL))
        return;

    while (data_len) {
        if (is_aligned(count, sizeof(char *))) {
            xlt_printk("\n 0x%lx :", align_addr);
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
            WriteExcInfoToBuf("\n 0x%lx :", align_addr);
#endif
        }
#ifdef __LP64__
        xlt_printk("%0+16lx ", *align_addr);
#else
        xlt_printk("%0+8lx ", *align_addr);
#endif
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
#ifdef __LP64__
        WriteExcInfoToBuf("0x%0+16x ", *align_addr);
#else
        WriteExcInfoToBuf("0x%0+8x ", *align_addr);
#endif
#endif
        align_addr++;
        data_len -= sizeof(char *);
        count++;
    }
    xlt_printk("\n");
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
    WriteExcInfoToBuf("\n");
#endif

    return;
}

static void os_mem_node_info(const xlt_mem_dyn_node *tmp_node, const xlt_mem_dyn_node *pre_node)
{
    if (tmp_node == pre_node) {
        xlt_printk("\n the broken node is the first node\n");
#ifdef XLTCFG_SHELL_EXCINFO_DUMP
        WriteExcInfoToBuf("\n the broken node is the first node\n");
#endif
    }

    xlt_printk("\n free broken node head: prev node %p, next node %p\n", tmp_node->self_node.free_node_info.pst_prev,
        tmp_node->self_node.free_node_info.pst_next);
    xlt_printk(" free pre node head   : prev node %p, next node %p\n", pre_node->self_node.free_node_info.pst_prev,
        pre_node->self_node.free_node_info.pst_next);

    xlt_printk(" OR\n");
    xlt_printk(" broken node head     : pre node %p, size_and_flag 0x%x\n", tmp_node->self_node.pre_node,
        tmp_node->self_node.size_and_flag);
    xlt_printk(" pre node head        : pre node %p, size_and_flag 0x%x\n", pre_node->self_node.pre_node,
        pre_node->self_node.size_and_flag);

#ifdef XLTCFG_SHELL_EXCINFO_DUMP
    WriteExcInfoToBuf("\n broken node head: %p  %p  %p  0x%x, pre node head: %p  %p  %p  0x%x\n",
        tmp_node->self_node.free_node_info.pst_prev, tmp_node->self_node.free_node_info.pst_next,
        tmp_node->self_node.pre_node, tmp_node->self_node.size_and_flag, pre_node->self_node.free_node_info.pst_prev,
        pre_node->self_node.free_node_info.pst_next, pre_node->self_node.pre_node, pre_node->self_node.size_and_flag);
#endif

    xlt_printk("\n---------------------------------------------\n");
    UINTPTR dump_end = (((UINTPTR)tmp_node + NODEDUMPSIZE) > (UINTPTR)tmp_node) ? ((UINTPTR)tmp_node + NODEDUMPSIZE) :
                                                                                  (UINTPTR)(UINTPTR_MAX);
    unsigned int dump_size = (UINTPTR)dump_end - (UINTPTR)tmp_node;
    xlt_printk(" dump mem tmp_node:%p ~ %p\n", tmp_node, dump_end);
    os_dump_mem_byte(dump_size, (UINTPTR)tmp_node);
    xlt_printk("\n---------------------------------------------\n");
    if (pre_node != tmp_node) {
        xlt_printk(" dump mem :%p ~ tmp_node:%p\n", ((UINTPTR)tmp_node - NODEDUMPSIZE), tmp_node);
        os_dump_mem_byte(NODEDUMPSIZE, ((UINTPTR)tmp_node - NODEDUMPSIZE));
        xlt_printk("\n---------------------------------------------\n");
    }
}

void os_mem_integrity_check_error(const xlt_mem_dyn_node *tmp_node, const xlt_mem_dyn_node *pre_node)
{
    os_mem_node_info(tmp_node, pre_node);
    return;
}

/*
 * Description : memory pool integrity checking
 * Input       : pool --Pointer to memory pool
 * Return      : XLT_OK --memory pool integrate or XLT_NOK--memory pool impaired
 */
unsigned int xlt_mem_integrity_check(void *pool)
{
    xlt_mem_dyn_node *tmp_node = NULL;
    xlt_mem_dyn_node *pre_node = NULL;
    unsigned int int_save;

    if (pool == NULL)
        return XLT_NOK;

    int_save = xlt_mem_lock();
    if (os_mem_integrity_check(pool, &tmp_node, &pre_node)) {
        goto ERROR_OUT;
    }
    xlt_mem_unlock(int_save);
    return XLT_OK;

ERROR_OUT:
    os_mem_integrity_check_error(tmp_node, pre_node);
    xlt_mem_unlock(int_save);
    return XLT_NOK;
}
#else
unsigned int xlt_mem_integrity_check(void *pool)
{
    return XLT_OK;
}
#endif

#ifdef XLTCFG_MEM_MUL_POOL
unsigned int xlt_mem_pool_list(void)
{
    void *next_pool = os_mem_mul_pool_head_get();
    unsigned int index = 0;
    while (next_pool != NULL) {
        xlt_printk("pool%u :\n", index);
        index++;
        os_mem_info_print(next_pool);
        next_pool = ((xlt_mem_pool_info *)next_pool)->next_pool;
    }
    return index;
}
#endif

#ifdef FREERTOS_DEBUG
void xlt_mem_nodestitle_print(const xlt_mem_pool_info *pool, mem_info_data_proc infoProc);
unsigned int xlt_mem_nodeusage_withtaskid(UINT32 taskId, const void *pool,
                                          UINT32 *memUsage, mem_info_data_proc infoProc);

unsigned int xlt_mem_tasknodes_print(UINT32 taskId, mem_info_data_proc infoProc)
{
    xlt_mem_pool_info *poolInfo = NULL;
    UINT32 nodeCount = 0;
    UINT32 memUsage = 0;
    if (taskId != (UINT32)(-1)) {
        xlt_printk("[ERROR] taskId %d is not valid\n", taskId);
        return 0;
    }
#ifndef XLTCFG_MEM_MUL_POOL
    /* If Multi-pool is not enabled, then trace SYSTEM MEM only */

    poolInfo = (xlt_mem_pool_info *)OS_SYS_MEM_POOL;
    xlt_mem_nodestitle_print(poolInfo, infoProc);
    nodeCount = xlt_mem_nodeusage_withtaskid(taskId, poolInfo, &memUsage, infoProc);
#else
    poolInfo = (xlt_mem_pool_info *)os_mem_mul_pool_head_get();
    while (poolInfo != NULL) {
        xlt_mem_nodestitle_print(poolInfo, infoProc);
        nodeCount += xlt_mem_nodeusage_withtaskid(taskId, poolInfo, &memUsage, infoProc);
        poolInfo = poolInfo->next_pool;
    }
#endif
    if (memUsage) {
        infoProc("Total %u mem nodes malloc by task[%d], memUsage [%u B]\n",
            nodeCount, taskId, memUsage);
    } else {
        infoProc("No mem node malloc by task[%d]\n", taskId);
    }

    return memUsage;
}

void xlt_mem_nodestitle_print(const xlt_mem_pool_info *pool, mem_info_data_proc infoProc)
{
    if (infoProc == os_mem_no_print) {
        return;
    }
 
    xlt_printk("\r\n Mem pool 0x%x Nodes Info:", pool);
    xlt_printk("\nMem node    size(B)   TaskId     ");
#ifdef XLTCFG_MEM_MUL_MODULE
    xlt_printk("mid  ");
#endif
#if defined(XLTCFG_MEM_LEAKCHECK) || defined(XLTCFG_MEM_LR_BACKUP)
    for (UINT32 i = 0; i < XLT_RECORD_LR_CNT; i++) {
        xlt_printk("LPC%u        ", i);
    }
#endif
}
 
unsigned int xlt_mem_nodeusage_withtaskid(UINT32 taskId, const void *pool, UINT32 *memUsage,
    mem_info_data_proc infoProc)
{
    xlt_mem_dyn_node *tmpNode = NULL;
    xlt_mem_dyn_node *backupNode = NULL;
    xlt_mem_pool_info *poolInfo = (xlt_mem_pool_info *)pool;
    UINT32 nodeCount = 0;
    UINT32 nodeSize;
 
    for (tmpNode = xlt_mem_first_node(poolInfo); tmpNode <= xlt_mem_end_node(poolInfo, poolInfo->pool_size);
        tmpNode = xlt_mem_next_node(tmpNode)) {
        if (backupNode == tmpNode) {
            break;
        }
        if (tmpNode->self_node.task_id == taskId || (taskId == (UINT32)(-1))) {
            nodeSize = xlt_mem_node_get_size(tmpNode->self_node.size_and_flag);
            if (xlt_mem_node_get_used_flag(tmpNode->self_node.size_and_flag)) {
                infoProc("\n0x%-10x%-10u%-11u", tmpNode, nodeSize, tmpNode->self_node.task_id);
                nodeCount++;
                (*memUsage) += nodeSize;
            } else {
                infoProc("\n0x%-10x%-10uFreeNode   ",
                    tmpNode, nodeSize);
            }
#ifdef XLT_CFG_MEM_MUL_MODULE
            infoProc("%u  ", tmpNode->self_node.module_id);
#endif
#if defined(XLTCFG_MEM_LEAKCHECK) || defined(XLTCFG_MEM_LR_BACKUP)
            if (infoProc != OsMemNoPrint) {
                OsMemPrintLinkPC(tmpNode);
            }
#endif
        }
 
        backupNode = tmpNode;
    }
 
#ifdef XLTCFG_KERNEL_MEM_SLAB_EXTENTION
    (*memUsage) += OsSlabNodesUsedWithTaskId(pool, taskId, memUsage, infoProc);
#endif
    infoProc("\n");
 
    return nodeCount;
}
#endif /* FREERTOS_DEBUG */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
