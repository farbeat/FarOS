/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi_tx self test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef __TEST_MIPI_TX_H__
#define __TEST_MIPI_TX_H__

#define RESET_GPIO_DIR_ADDR          0x5003E014
#define RESET_GPIO_OUT_LOW_VAL_ADDR  0x5003E004
#define RESET_GPIO_OUT_HIGH_VAL_ADDR 0x5003E008
#define RESET_GPIO_POS               4
#define TE_SIGNAL_MUX_ADDR           0x500057bc
#define TE_FUNC_POS                  0

#define TEST_BITS_PER_BYTE    8
#define test_dsi_short_packet(p1, p0) ((((p1) & 0xff) << TEST_BITS_PER_BYTE) | ((p0) & 0xff));
/*
* mipi_tx colorbar mode test.
*/
void add_test_mipi_tx(void);

#endif