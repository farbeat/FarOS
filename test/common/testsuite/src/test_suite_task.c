/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides test suite task \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-19, Create file. \n
 */

#include <stdint.h>
#include "securec.h"
#include "common_def.h"
#include "soc_osal.h"
#include "test_suite_console.h"
#include "test_suite_commands_processor.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_task.h"

#define TEST_SUITE_MSG_QUEUE_LEN      5
#define TEST_SUITE_MSG_QUEUE_MAX_SIZE 128
#define TEST_SUITE_RECEIVE_MSGQUEUE_BUFFER_SIZE TEST_SUITE_MSG_QUEUE_MAX_SIZE
#define TEST_SUITE_QUEUE_DELAY 0xFFFFFFFF
#define CONFIG_TEST_SUITE_COMMAND_BUFFER_SIZE   300

osal_task *g_test_suite_task_id = NULL;
unsigned long g_test_suite_msgqueue_id;
static char g_test_suite_command_buffer_array[CONFIG_TEST_SUITE_COMMAND_BUFFER_SIZE + 1];
static bool g_exit_test_suite_task = false;

static int32_t test_suite_task_entry(void *data);

static void test_suite_create_msgqueue(void)
{
    if (osal_msg_queue_create("test_suite_queue", TEST_SUITE_MSG_QUEUE_LEN, \
        (unsigned long *)&g_test_suite_msgqueue_id, 0, TEST_SUITE_MSG_QUEUE_MAX_SIZE) != OSAL_SUCCESS) {
        test_suite_log_string("Testsuite message queue create failed!\n");
    }
}

static void test_suite_delete_msgqueue(void)
{
    osal_msg_queue_delete(g_test_suite_msgqueue_id);
}

int32_t test_suite_write_msgqueue(const uint8_t *buffer_addr, uint16_t buffer_size)
{
    return osal_msg_queue_write_copy(g_test_suite_msgqueue_id, (void *)buffer_addr, \
                                     (uint32_t)buffer_size, 0);
}

static int32_t test_suite_receive_msgqueue(char *buffer_addr, uint32_t *buffer_size)
{
    return osal_msg_queue_read_copy(g_test_suite_msgqueue_id, (void *)buffer_addr, \
                                    buffer_size, TEST_SUITE_QUEUE_DELAY);
}

static void test_suite_task_create(void)
{
    g_test_suite_task_id = osal_kthread_create((osal_kthread_handler)test_suite_task_entry, \
                                               NULL, "test_suite_thread", CONFIG_TEST_SUITE_TASK_STACK_SIZE);
    if (g_test_suite_task_id ==  NULL) {
        test_suite_log_string("osal_kthread_create test_suite_task FAILED\n");
    }
}

static int32_t test_suite_alloc(uint32_t len, char **buf)
{
    *buf = (char *)osal_kmalloc(len, OSAL_GFP_KERNEL);
    if (*buf == NULL) {
        test_suite_log_string("Testsuite alloc commands buffer failed!\n");
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

void test_suite_task_init(void)
{
    test_suite_task_set_exit(false);
    test_suite_create_msgqueue();
    test_suite_task_create();
}

void test_suite_task_destroy(void)
{
    test_suite_delete_msgqueue();
    osal_kthread_destroy(g_test_suite_task_id, 0);
}

void test_suite_task_set_exit(bool status)
{
    g_exit_test_suite_task = status;
}

bool test_suite_task_is_exit(void)
{
    return g_exit_test_suite_task;
}

static void test_suite_process_input(char *char_buf, uint16_t length)
{
    char received_char;
    char *string = char_buf;
    uint16_t len = length;

    /* process every character */
    while (len > 0) {
        received_char = *(string++);
        test_suite_console_process_char(received_char);
        len--;
    }
}

static void test_suite_rx_buf_init(char *buffer_addr, uint32_t *buffer_size)
{
    *buffer_size = TEST_SUITE_RECEIVE_MSGQUEUE_BUFFER_SIZE;
    (void)memset_s(buffer_addr, *buffer_size, 0, *buffer_size);
}

static int32_t test_suite_task_entry(void *data)
{
    unused(data);

    volatile char * const test_suite_command_buffer = g_test_suite_command_buffer_array;
    test_suite_console_register_command_buffer((char *)test_suite_command_buffer);

    for (; ;) {
        if (test_suite_task_is_exit() == true) {
            break;
        }
        test_suite_console_enable();

        char *rx_buf;
        uint32_t rx_length = TEST_SUITE_RECEIVE_MSGQUEUE_BUFFER_SIZE;
        if (test_suite_alloc(rx_length, &rx_buf) != OSAL_SUCCESS) {
            return OSAL_FAILURE;
        }

        while (test_suite_console_is_enabled()) {
            test_suite_rx_buf_init(rx_buf, &rx_length);
            test_suite_receive_msgqueue(rx_buf, &rx_length);
            test_suite_process_input(rx_buf, (uint16_t)rx_length);
        }
        osal_kfree(rx_buf);
        test_suite_commands_prase((char *)test_suite_command_buffer);
    }
    return OSAL_FAILURE;
}

void test_suite_nonos_loop(void)
{
    static int flag = 0;
    static volatile char * const test_suite_command_buffer = g_test_suite_command_buffer_array;

    if (flag == 0) {
        test_suite_console_register_command_buffer((char *)test_suite_command_buffer);
        flag = 1;
    }

    if (test_suite_task_is_exit() == true) {
        return;
    }
    test_suite_console_enable();

    char *rx_buf;
    uint32_t rx_length = TEST_SUITE_RECEIVE_MSGQUEUE_BUFFER_SIZE;
    if (test_suite_alloc(rx_length, &rx_buf) != OSAL_SUCCESS) {
        return;
    }

    while (test_suite_console_is_enabled()) {
        test_suite_rx_buf_init(rx_buf, &rx_length);
        test_suite_receive_msgqueue(rx_buf, &rx_length);
        test_suite_process_input(rx_buf, (uint16_t)rx_length);
    }
    osal_kfree(rx_buf);
    test_suite_commands_prase((char *)test_suite_command_buffer);
}

