/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core OS Initialize Interface Header for Standard.
 */

#ifndef APP_OS_INIT_H
#define APP_OS_INIT_H

/**
 * @defgroup MAIN_ENTRY CHIP Main Entry
 * @{
 */

/**
 * @brief  Main initialisation function for the apps os. Creates the appropriate queues and threads.
 * @param  unused1 Unused param1
 * @param  unused2 Unused param2
 */
void app_os_init(const void *unused1, unsigned int unused2);

/**
 * @}
 */

#endif
