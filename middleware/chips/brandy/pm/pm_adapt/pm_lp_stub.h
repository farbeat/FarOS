/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: pm lp stub header file
 */

#ifndef SRC_LIB_PM_LP_STUB_H
#define SRC_LIB_PM_LP_STUB_H
#include "platform_core.h"

void gpio_pad_suspend_tmp(void);
void gpio_pad_resume_tmp(void);

void clk_switch_to_rc(void);
void clk_switch_to_xo_dll2(void);

void uart_h0_suspend_tmp(void);
void uart_h0_resume_tmp(void);

void uart_h1_suspend_tmp(void);
void uart_h1_resume_tmp(void);

void qspi0_suspend_tmp(void);
void qspi0_resume_tmp(void);

void power_suspend_tmp(void);
void power_resume_tmp(void);
void storage_device_resume(void);
void storage_device_suspend(void);

void pm_misc_regs_resume(void);
#endif