
/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core OS Initialize Interface Header for Standard
 */

#ifndef BOARD_INIT_H
#define BOARD_INIT_H

#include "stdint.h"
#include "gpio_porting.h"

typedef enum display_class {
    DISPLAY_CLASS_MIPI,
    DISPLAY_CLASS_QSPI,
    DISPLAY_CLASS_UNDEFINED,
} display_class;

typedef enum board_class {
    BOARD_CLASS_E0B2,
    BOARD_CLASS_E0B4,
    BOARD_CLASS_UNDEFINED,
} board_class;

display_class get_display_init_select(void);
board_class get_board_init_select(void);

/**
 * @defgroup MAIN_ENTRY CHIP Main Entry
 * @{
 */
/**
 * @brief init board config
 * @param flash_id
 * @param flash_unique_id
 */
void board_init(uint32_t flash_id, uint32_t flash_unique_id);

/**
 * @}
 */

#endif
