/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core OS Initialize Interface Header for Standard
 */

#ifndef KERNEL_INIT_H
#define KERNEL_INIT_H

typedef enum os_class {
    OS_CLASS_LITEOS,
    OS_CLASS_FREERTOS,
    OS_CLASS_UNDEFINED,
} os_class;

os_class get_os_init_select(void);

#ifndef __FREERTOS__
void OsCacheInit(void);
#endif

/**
 * @defgroup MAIN_ENTRY CHIP Main Entry
 * @{
 */
/**
 * @brief  Service init.
 */
void kernel_init(void);

/**
 * @}
 */

#endif
