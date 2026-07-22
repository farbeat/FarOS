/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description:  Application Core OS Priority Initialize Interface Header for Standard
 */

#ifndef THREAD_PRI_INIT_H
#define THREAD_PRI_INIT_H
#include "stdint.h"

typedef enum store_class {
    STORE_CLASS_EMMC,
    STORE_CLASS_NAND,
    STORE_CLASS_UNDEFINED,
} store_class;

typedef enum {
    PSRAM_DENSITY_16M = 0x5,
    PSRAM_DENSITY_64M = 0x6,
    PSRAM_DENSITY_32M = 0x7,

    PSRAM_DENSITY_MAX
} psram_density_t;

typedef enum {
    CLOCK_SOURCE_XO = 0x0,
    CLOCK_SOURCE_RC = 0x1,

    CLOCK_SOURCE_MAX
} clock_32k_source_t;

int32_t drv_gmmu_module_init(void);
int32_t uapi_vau_module_init(void);
int32_t uapi_dpu_module_init(void);
void los_vfs_init(void);
uint32_t dpal_driver_base_init(void);
void uapi_jpeg_module_init(void);
void rc_calibration_func_notify(void);
/**
 * @defgroup MAIN_ENTRY CHIP Main Entry
 * @{
 */

/**
 * @brief  Thread task creat.
 */
void thread_pri_init(void);

/**
 * @}
 */

#endif
