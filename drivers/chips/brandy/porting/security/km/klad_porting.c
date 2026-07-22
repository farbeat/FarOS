/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "common_def.h"
#include "klad_porting.h"

uint8_t g_klad_key_addr[KLAD_KEY_LEN]  __attribute__ ((aligned (KLAD_ADDR_ALIGN))) = {0};
