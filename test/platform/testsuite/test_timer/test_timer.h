/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test timer header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup testcase_timer TIMER
 * @ingroup  testcase
 * @{
 */
#if defined(TEST_TIMER_INDEX) && (TEST_TIMER_INDEX == 3)
#define DEFAULT_TEST_TIMER 3
#elif defined(TEST_TIMER_INDEX) && (TEST_TIMER_INDEX == 2)
#define DEFAULT_TEST_TIMER 2
#elif defined(TEST_TIMER_INDEX) && (TEST_TIMER_INDEX == 1)
#define DEFAULT_TEST_TIMER 1
#else
#define DEFAULT_TEST_TIMER 0
#endif

#if defined(CONFIG_TIMER_USING_V150)
#define TIMER_LOAD_COUNT_REG_OFFSET (0x00)
#define TIMER_CURRENT_COUNT_REG_OFFSET (0x08)
#define TIMER_CONTROL_REG_OFFSET (0x10)
#else
#define TIMER_LOAD_COUNT_REG_OFFSET (0x00)
#define TIMER_CURRENT_COUNT_REG_OFFSET (0x04)
#define TIMER_CONTROL_REG_OFFSET (0x08)
#endif

/**
 * @if Eng
 * @brief  timer init for base test.
 * @param  No Params.
 * @else
 * @brief  timer 基础测试
 * @param  无需输入参数
 * @endif
 */
int test_timer_init(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Timer use case validation in the default timer interrupt, can start more than one, \
 * control the number through TimerNums.
 * @param  Params: <TimerNums>(1~16), <delay>(us) (hex or decimal)
 * @else
 * @brief  默认timer中断中起定时器用例验证，可以起多个，通过TimerNums控制数量
 * @param  Params: <TimerNums>(1~16), <delay>(us) (hex or decimal)
 * @endif
 */
int test_timer_start(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Wait for the timer task to time out, restart the timer task, can start more than one, \
 * control the number through TimerNums.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us).
 * @else
 * @brief  等待定时任务超时后，重启定时任务，可以起多个，通过TimerNums控制数量
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us).
 * @endif
 */
int test_timer_restart(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Before the previous batch of timer tasks time out, reset the timer tasks, can start more than one, \
 * control the number through TimerNums.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <resetdelay>(us).
 * @else
 * @brief  在前一批定时任务超时前，重新设置定时任务，可以起多个，通过TimerNums控制数量
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <resetdelay>(us).
 * @endif
 */
int test_timer_reset_timerinfo(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Timed tasks of different duration.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us)...<ndelay><us>".
 * @else
 * @brief  启动不同时长的定时任务
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us)...<ndelay><us>".
 * @endif
 */
int test_timer_start_diff_times(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Delay 1s before the previous batch of timer tasks time out, then reset the timer tasks, \
 * can start more than one, control the number through TimerNums.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <resetdelay>(us).
 * @else
 * @brief  在前一批定时任务超时前，delay 1s，然后重新设置定时任务，可以起多个，通过TimerNums控制数量.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <resetdelay>(us).
 * @endif
 */
int test_timer_delay_reset_timerinfo(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Start timed tasks of different lengths with 1s intervals. \
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us)...<ndelay><us>.
 * @else
 * @brief  启动不同时长的定时任务，中间间隔1s.
 * @param  Params: <TimerNums>(1~16), <firstdelay>(us), <secdelay>(us)...<ndelay><us>.
 * @endif
 */
int test_timer_start_diff_times_intervals(int argc, char *argv[]);

/**
 * @if Eng
 * @brief  Add the test_timer use case.
 * @param  None.
 * @else
 * @brief  添加test_timer 用例.
 * @param  无.
 * @endif
 */
void add_timer_test_case(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
