/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: at perf \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */

#include "app_test_perf.h"
#include "app_at_process.h"
#include "common_def.h"
#ifdef LOSCFG_KERNEL_PERF
#include "los_event.h"
#include "los_perf.h"
#include "los_task_pri.h"
#include "los_queue.h"

#define LOS_PERF_BUFFER_SIZE 64 * 1024
char g_perf_buf[LOS_PERF_BUFFER_SIZE];
char g_read_buf[LOS_PERF_BUFFER_SIZE];

static void OsPrintBuff(char *data, uint32_t len)
{
    UINT32 intSave;
    SCHEDULER_LOCK(intSave);
    wstp_print("\n--- PERF DATA START ---\n");
    for (uint32_t i = 0; i < len; i++) {
        wstp_print("%02x", (uint8_t)data[i]);
    }
    wstp_print("\n--- PERF DATA END ---\n");
    SCHEDULER_UNLOCK(intSave);
}

uint32_t perf_test_start(void)
{
    (void)memset_s(g_perf_buf, LOS_PERF_BUFFER_SIZE, 0, LOS_PERF_BUFFER_SIZE);
    (void)memset_s(g_read_buf, LOS_PERF_BUFFER_SIZE, 0, LOS_PERF_BUFFER_SIZE);
    uint32_t ret = LOS_PerfInit(g_perf_buf, LOS_PERF_BUFFER_SIZE);
    if (ret != LOS_OK) {
        wstp_print("perf init error 0x%x\n", ret);
        return ERRCODE_FAIL;
    }
    PerfConfigAttr attr1 = {
        .eventsCfg = {
            .type = PERF_EVENT_TYPE_SW,
            .events = {
                [0] = {PERF_COUNT_SW_TASK_SWITCH, 1},
            },
            .eventsNr = 1,
            .predivided = 0,
        },
        .taskIds = {0},
        .taskIdsNr = 0,
        .needSample = 1,
        .sampleType = PERF_RECORD_IP | PERF_RECORD_CALLCHAIN,
    };
    ret = LOS_PerfConfig(&attr1);
    if (ret != LOS_OK) {
        wstp_print("perf config error 0x%x\n", ret);
        return ERRCODE_FAIL;
    }
    LOS_PerfStart(0);
    return ERRCODE_SUCC;
}

uint32_t perf_test_stop(void)
{
    LOS_PerfStop();
    uint32_t len = LOS_PerfDataRead(g_read_buf, LOS_PERF_BUFFER_SIZE);
    wstp_print("LOS_PerfDataRead addr = %p len = %u\n", g_read_buf, len);
    OsPrintBuff(g_read_buf, len);
    LOS_PerfDeinit();
    return ERRCODE_SUCC;
}

#endif  /* LOSCFG_KERNEL_PERF */