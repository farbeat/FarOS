/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  Application Core Main function for Standard.
 * Author:
 * Create:
 */
#include "kernel_init.h"
#include "chip_init.h"
#include "board_init.h"
#include "service_init.h"
#include "thread_pri_init.h"
#include "cmsis_os2.h"
#include "application_version.h"
#include "securec.h"
#include "main.h"

static startup_table ssb_to_app_table;
static uint32_t g_psram_density;

uint32_t get_psram_density(void)
{
    return g_psram_density;
}

void app_main(const void *startup_details_table, const uint32_t density)
{
    if (memcpy_s(&ssb_to_app_table, sizeof(ssb_to_app_table), startup_details_table, sizeof(ssb_to_app_table)) != EOK) {
        return;
    }

    g_psram_density = density;

    kernel_init();

    chip_init();

    board_init(ssb_to_app_table.ssb_to_application_flash_id, ssb_to_app_table.ssb_to_application_flash_unique_id);

    service_init();

    thread_pri_init();

    (void)osKernelStart();
}
