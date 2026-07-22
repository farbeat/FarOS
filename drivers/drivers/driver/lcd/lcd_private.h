/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of lcd private interface.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __LCD_PRIVATE_H__
#define __LCD_PRIVATE_H__

typedef struct {
    void (*power_on)(void);
    void (*power_off)(void);
} lcd_drv_private;

lcd_drv_private *lcd_drv_get_private_api(void);

#endif