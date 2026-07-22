/*
 * Copyright (c) @CompanyNameMagicTag 2015-2020. All rights reserved.
 * Description: ivt sched
 */

#ifndef _IVT_SCHED_H_
#define _IVT_SCHED_H_

/* 调度事件 */
typedef enum _ivt_sched_event_ {
    IVT_SCHED_EV_RECV = 0x01, /* 接收事件 */
    IVT_SCHED_EV_TASK = 0x02, /* 调度处理 */
} ivt_sched_event_em;

/*
 * 函数声明
 */
void ivt_sched_set_event(ivt_sched_event_em ev);
#endif /* end of _IVT_SCHED_H_ */
