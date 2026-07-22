/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pmp porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-26， Create file. \n
 */

#include "hal_pmp_riscv70.h"
#include "pmp_porting.h"

void pmp_port_register_hal_funcs(void)
{
    hal_pmp_register_funcs(hal_pmp_riscv70_funcs_get());
}

void pmp_port_unregister_hal_funcs(void)
{
    hal_pmp_unregister_funcs();
}