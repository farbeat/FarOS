/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test int source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#include "test_int.h"
#include "non_os.h"
#include "test_suite_log.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "debug_print.h"
#include "oal_interface.h"

static void irq_handler_test2(void)
{
    PRINT("irq_handler_test2 start\r\n");
    PRINT("irq_handler_test2 end\r\n");
}

static int int_test(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    PRINT("   Begin INT Test   " NEWLINE);
    oal_int_create(0, 0, irq_handler_test2, 0);

    int_enable_irq(0);
    int_set_pendind_irq(0);

    uint32_t counter = 0xff;
    while (counter != 0) {
        counter--;
    }
    PRINT("   End INT Test   " NEWLINE);
    return TEST_SUITE_OK;
}

void add_int_test_case(void)
{
    uapi_test_suite_add_function("interrupt_test", "interrupt test.", int_test);
}