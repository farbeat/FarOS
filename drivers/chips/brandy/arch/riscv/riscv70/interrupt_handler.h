/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description:  RISCV70 Core interrupt handler header file
 * Author: @CompanyNameTag
 * Create:  2022-03-22
 */
#ifndef RISCV770_INTERRUPT_HANDLER_H
#define RISCV770_INTERRUPT_HANDLER_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @brief  Interrupt0 handler function.
 */
void interrupt0_handler(void);

/**
 * @brief  Interrupt1 handler function.
 */
void interrupt1_handler(void);

/**
 * @brief  Interrupt2 handler function.
 */
void interrupt2_handler(void);

/**
 * @brief  Interrupt3 handler function.
 */
void interrupt3_handler(void);

/**
 * @brief  Interrupt4 handler function.
 */
void interrupt4_handler(void);

/**
 * @brief  Interrupt5 handler function.
 */
void interrupt5_handler(void);

/**
 * @brief  Local interrupt handler function.
 */
void local_interrupt_handler(void);

/**
 * @brief  Init local interrupt priority.
 */
void local_interrupt_priority_init(void);

/**
 * @brief  Get interrupt number.
 * @return Interrupt number.
 */
uint32_t interrupt_number_get(void);

/**
 * @brief Get interrupt count.
 * @return Interrupt count.
 */
uint32_t interrupt_count_get(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif