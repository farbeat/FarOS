/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: IPC HAL interface.
 * Author: @CompanyNameTag
 * Create:  2020-06-10
 */

#ifndef HAL_IPC_H
#define HAL_IPC_H

#include "std_def.h"
#include "platform_types.h"
#include "chip_definitions.h"

/** @defgroup connectivity_drivers_hal_ipc IPC
  * @ingroup  connectivity_drivers_hal
  * @{
  */

#define M_TO_BCPU_INT_OFFSET  BIT(0)
#if CHIP_SOCMN1 || CHIP_BRANDY
#define MCPU_SOFT_INT_EN      (*(volatile unsigned short *)(0x52000020))
#define MCPU_SOFT_INT_SET     (*(volatile unsigned short *)(0x52000034))
#define MCPU_SOFT_INT_CLR     (*(volatile unsigned short *)(0x5200002c))
#define MCPU_SOFT_INT_STS     (*(volatile unsigned short *)(0x52000028))
#define MCPU_SOFT_INT_CLR_STS (*(volatile unsigned short *)(0x52000030))
#define M_TO_DSP_INT_OFFSET   BIT(2)
#define M_TO_DSP_INT1_OFFSET  BIT(3)
#define M_TO_DSP_1_INT_OFFSET BIT(4)
#if CHIP_SOCMN1
#define M_TO_ALL_INT          (M_TO_BCPU_INT_OFFSET | M_TO_DSP_INT_OFFSET | M_TO_DSP_1_INT_OFFSET)
#elif CHIP_BRANDY
#define M_TO_ALL_INT          (M_TO_BCPU_INT_OFFSET | M_TO_DSP_INT_OFFSET | M_TO_DSP_INT1_OFFSET)
#endif
#elif CHIP_LIBRA
#define MCPU_SOFT_INT_EN      (*(volatile unsigned short *)(0x52000020))
#define MCPU_SOFT_INT_SET     (*(volatile unsigned short *)(0x52000034))
#define MCPU_SOFT_INT_CLR     (*(volatile unsigned short *)(0x5200002c))
#define MCPU_SOFT_INT_STS     (*(volatile unsigned short *)(0x52000028))
#define MCPU_SOFT_INT_CLR_STS (*(volatile unsigned short *)(0x52000030))
#define M_TO_DSP_INT_OFFSET   BIT(2)
#define M_TO_DSP_1_INT_OFFSET BIT(4)
#define M_TO_GNSS_INT0_OFFSET BIT(6)
#define M_TO_GNSS_INT1_OFFSET BIT(7)
#define M_TO_SEC_INT0_OFFSET  BIT(8)
#define M_TO_SEC_INT1_OFFSET  BIT(9)
#define M_TO_ALL_INT (M_TO_BCPU_INT_OFFSET | M_TO_DSP_INT_OFFSET | M_TO_DSP_1_INT_OFFSET | M_TO_GNSS_INT0_OFFSET |
                      M_TO_GNSS_INT1_OFFSET)
#else // BS25
#define MCPU_SOFT_INT_EN      (*(volatile unsigned short *)(0x52000020))
#define MCPU_SOFT_INT_SET     (*(volatile unsigned short *)(0x52000034))
#define MCPU_SOFT_INT_CLR     (*(volatile unsigned short *)(0x5200002c))
#define MCPU_SOFT_INT_STS     (*(volatile unsigned short *)(0x52000028))
#define MCPU_SOFT_INT_CLR_STS (*(volatile unsigned short *)(0x52000030))
#define M_TO_DSP_INT_OFFSET   BIT(2)
#define M_TO_DSP_1_INT_OFFSET BIT(4)
#define M_TO_ALL_INT          (M_TO_BCPU_INT_OFFSET)
#endif

/**
 * @brief  Enable the appropriate interrupts for the IPC mechanism on this core (core specific).
 * @return None.
 */
void hal_ipc_init_interrupts(void);

/**
 * @brief  Disable interrupts used by the IPC mechanism on this core (core specific).
 * @return None.
 */
void hal_ipc_deinit_interrupts(void);

/**
 * @brief  Inform the core that an IPC operation is pending. For the APPS and PROTOCOL core this means
 * a message has been places in their inbox, while for the security core, this can have additional
 * meaning as the security core is the man in the middle for all IPC message between cores.
 * @param  core The dest core
 * @return None
 */
void hal_ipc_interrupt_core(cores_t core);

#if CHIP_LIBRA
/**
 * @brief Inform the gnss core that an IPC operation is pending.
 */
void hal_ipc_interrupt1_gcpu(void);
#endif

/**
 * @brief  BCPU trigger MCPU interrupt.
 * @return None.
 */
void hal_ipc_interrupt_apps(void);

/**
 * @brief  MCPU trigger BCPU interrupt..
 * @return None.
 */
void hal_ipc_interrupt_security(void);

/**
 * @brief  MCPU enable interrupt from BCPU and DSP..
 * @return None.
 */
void hal_ipc_interrupt_en(void);

/**
 * @brief  Clear interrupt.
 * @param  core The dest core.
 */
void hal_ipc_interrupt_clear_core(cores_t core);

/**
 * @brief Clear interrupt1.
 * @param core The dest core.
 */
void hal_ipc_interrupt1_clear_core(cores_t core);

/**
  * @}
  */
#endif
