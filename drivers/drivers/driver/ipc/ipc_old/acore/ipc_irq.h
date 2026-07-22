/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: IPC HAL interface.
 * Author: @CompanyNameTag
 * Create:  2020-06-10
 */

#ifndef HAL_IPC_IRQ_H
#define HAL_IPC_IRQ_H

/**
 * @brief  Enable the appropriate interrupts for the IPC mechanism on this core (core specific).
 * @return None.
 */
void ipc_init_interrupts(void);

/**
 * @brief  Disable interrupts used by the IPC mechanism on this core (core specific).
 * @return None.
 */
void ipc_deinit_interrupts(void);

#endif
