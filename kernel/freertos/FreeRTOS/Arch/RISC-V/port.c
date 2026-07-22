/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V RV32 port.
 *----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "encoding.h"
#include "print.h"
#include "hal_hwi.h"
#include "hal_freertos_timer.h"
#include "soc_osal.h"
#include "asm/register_config.h"
#include "arch_base.h"
#include "pm.h"
#include "pm_sleep.h"

/* Standard includes. */
#include "string.h"

#ifdef configCLINT_BASE_ADDRESS
	#warning The configCLINT_BASE_ADDRESS constant has been deprecated.  configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS are currently being derived from the (possibly 0) configCLINT_BASE_ADDRESS setting.  Please update to define configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS dirctly in place of configCLINT_BASE_ADDRESS.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIME_BASE_ADDRESS
	#warning configMTIME_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtime register then set configMTIME_BASE_ADDRESS to the mapped address.  Otherwise set configMTIME_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIMECMP_BASE_ADDRESS
	#warning configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtimecmp register then set configMTIMECMP_BASE_ADDRESS to the mapped address.  Otherwise set configMTIMECMP_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

uint32_t g_clicCfgLevelBitsNum = 1;
#define CLIC_CFG_INTERRUPT_OFFSET 1
#define SET_SYS_PRIOR(usrPrior)    ((uint8_t)(configHWI_PRIO_LIMIT + CLIC_CFG_INTERRUPT_OFFSET - (usrPrior)))

/* Let the user override the pre-loading of the initial LR with the address of
prvTaskExitError() in case it messes up unwinding of the stack in the
debugger. */
#define configTASK_RETURN_ADDRESS vTaskExitNormal

#ifdef configTASK_RETURN_ADDRESS
	#define portTASK_RETURN_ADDRESS	configTASK_RETURN_ADDRESS
#else
	#define portTASK_RETURN_ADDRESS	prvTaskExitError
#endif

/* The stack used by interrupt service routines.  Set configISR_STACK_SIZE_WORDS
to use a statically allocated array as the interrupt stack.  Alternative leave
configISR_STACK_SIZE_WORDS undefined and update the linker script so that a
linker variable names __freertos_irq_stack_top has the same value as the top
of the stack used by main.  Using the linker script method will repurpose the
stack that was used by main before the scheduler was started for use as the
interrupt stack after the scheduler has started. */
#ifdef configISR_STACK_SIZE_WORDS
	static __attribute__ ((aligned(16))) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
	const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );

	/* Don't use 0xa5 as the stack fill bytes as that is used by the kernerl for
	the task stacks, and so will legitimately appear in many positions within
	the ISR stack. */
	#define portISR_STACK_FILL_BYTE	0xee

    static __attribute__ ((aligned(16))) StackType_t xExceptionStack[ configEXC_STACK_SIZE_WORDS ] = { 0 };
    const StackType_t xExceptionStackTop = ( StackType_t ) &( xExceptionStack[ configEXC_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );
#else
	extern char __irq_stack_top__;
    extern char __excp_stack_top__;

	const StackType_t xISRStackTop = ( StackType_t )(&__irq_stack_top__);
    const StackType_t xExceptionStackTop = ( StackType_t )(&__excp_stack_top__);
#endif

/*
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 */
void vPortSetupTimerInterrupt( void ) __attribute__(( weak ));

/*-----------------------------------------------------------*/

#if( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )
/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
const uint64_t *pullNextTime = &ullNextTime;
const size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ); /* Assumes increment won't go over 32-bits. */
uint32_t const ullMachineTimerCompareRegisterBase = configMTIMECMP_BASE_ADDRESS;
volatile uint64_t * pullMachineTimerCompareRegister = NULL;
#endif

/* Set configCHECK_FOR_STACK_OVERFLOW to 3 to add ISR stack checking to task
stack checking.  A problem in the ISR stack will trigger an assert, not call the
stack overflow hook function (because the stack overflow hook is specific to a
task stack, not the ISR stack). */
#if defined( configISR_STACK_SIZE_WORDS ) && ( configCHECK_FOR_STACK_OVERFLOW > 2 )
	#warning This path not tested, or even compiled yet.

	static const uint8_t ucExpectedStackBytes[] = {
									portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,		\
									portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,		\
									portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,		\
									portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,		\
									portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE };	\

	#define portCHECK_ISR_STACK() configASSERT( ( memcmp( ( void * ) xISRStack, ( void * ) ucExpectedStackBytes, sizeof( ucExpectedStackBytes ) ) == 0 ) )
#else
	/* Define the function away. */
	#define portCHECK_ISR_STACK()
#endif /* configCHECK_FOR_STACK_OVERFLOW > 2 */

/*-----------------------------------------------------------*/

#if( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )

	void vPortSetupTimerInterrupt( void )
	{
	uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
	volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( configMTIME_BASE_ADDRESS ) + 4UL ); /* 8-byte typer so high 32-bit word is 4 bytes up. */
	volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( configMTIME_BASE_ADDRESS );
	volatile uint32_t ulHartId;

		__asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
		pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( ullMachineTimerCompareRegisterBase + ( ulHartId * sizeof( uint64_t ) ) );

		do
		{
			ulCurrentTimeHigh = *pulTimeHigh;
			ulCurrentTimeLow = *pulTimeLow;
		} while( ulCurrentTimeHigh != *pulTimeHigh );

		ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
		ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
		ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
		ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
		*pullMachineTimerCompareRegister = ullNextTime;

		/* Prepare the time to use after the next tick interrupt. */
		ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	}
#else
    void vPortSetupTimerInterrupt( void )
    {
        vTimerTickStart();
    }
#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIME_BASE_ADDRESS != 0 ) */
/*-----------------------------------------------------------*/

void vPortHwiInit(void)
{
    /* set cliccfg reg. nvbits = 0; nlbits = 4; nmbits = 0. */
    uint8_t cliccfg = 0;
    cliccfg |= (4 << 1); /* non-vector, m-mode */
    WRITE_UINT8(cliccfg, CLIC_BASE_REG + SYS_CLIC_CFG);

    for (uint32_t i = 0; i <= configHWI_PRIO_LIMIT; i++) {
        if ((configHWI_PRIO_LIMIT & ((0x80) >> i)) != 0) {
            g_clicCfgLevelBitsNum = 8 - i; /* max 8bit */
            break;
        }
    }
    uint8_t clicintattr = 0;
    /* clicintattr[0] = 0 non-vector; clicintattr[2:1] = 1 edge-triggered; clicintattr[7:6] = 3 M-mode */
    clicintattr |= ((0 << 1) | (3 << 6));

    uint32_t index;
    for (index = 0; index < configHWI_MAX_VECTOR_CNT; index++) {
        WRITE_UINT8(0, CLIC_BASE_REG + sys_clic_intip(index));      /* clear pending */
        WRITE_UINT8(0, CLIC_BASE_REG + sys_clic_intie(index));      /* disabled */
        WRITE_UINT8(clicintattr, CLIC_BASE_REG + sys_clic_intattr(index));
    }
}

BaseType_t xPortStartScheduler( void )
{
extern void xPortStartFirstTask( void );

	#if( configASSERT_DEFINED == 1 )
	{
		/* Check alignment of the interrupt stack - which is the same as the
		stack that was being used by main() prior to the scheduler being
		started. */
		configASSERT( ( xISRStackTop & portBYTE_ALIGNMENT_MASK ) == 0 );

		#ifdef configISR_STACK_SIZE_WORDS
		{
			memset( ( void * ) xISRStack, portISR_STACK_FILL_BYTE, sizeof( xISRStack ) );
		}
		#endif	 /* configISR_STACK_SIZE_WORDS */
	}
	#endif /* configASSERT_DEFINED */

	/* If there is a CLINT then it is ok to use the default implementation
	in this file, otherwise vPortSetupTimerInterrupt() must be implemented to
	configure whichever clock is to be used to generate the tick interrupt. */
	vPortSetupTimerInterrupt();

	#if( ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) )
	{
		/* Enable mtime and external interrupts.  1<<7 for timer interrupt, 1<<11
		for external interrupt.  _RB_ What happens here when mtime is not present as
		with pulpino? */
		__asm volatile( "csrs mie, %0" :: "r"(0x880) );
	}
	#else
	{
		/* Enable external interrupts. */
		__asm volatile( "csrs mie, %0" :: "r"(0x800) );
	}
	#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) */

	xPortStartFirstTask();

	/* Should not get here as after calling xPortStartFirstTask() only tasks
	should be executing. */
	return pdFAIL;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* Not implemented. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static HwiHandle_t *pxHwiHandles[ configHWI_MAX_VECTOR_CNT ];

HwiHandle_t *pxPortHwiGetHandle( BaseType_t xId )
{
    return pxHwiHandles[ xId ];
}

void vPortHwiSetHandle( BaseType_t xId, HwiHandle_t *pxHandle )
{
    pxHwiHandles[ xId ] = pxHandle;
}

BaseType_t xPortHwiSetPriority(BaseType_t hwiNum, BaseType_t level)
{
    uint8_t localLevel;

    configASSERT( level <= configHWI_PRIO_LOWEST && level >= configHWI_PRIO_HIGHEST );
    configASSERT( hwiNum < configHWI_MAX_VECTOR_CNT && hwiNum >= 0 );

    /* riscv's priority range is [1, 7] */
    localLevel = SET_SYS_PRIOR(level);
    localLevel = localLevel  << (8 - g_clicCfgLevelBitsNum); /* max 8 bit */
    uint8_t mask = ((uint8_t)(-1)) >> (g_clicCfgLevelBitsNum);
    uint8_t clicIntCtl = GET_UINT8(CLIC_BASE_REG + sys_clic_intctl(hwiNum));
    clicIntCtl = clicIntCtl & mask;
    clicIntCtl = clicIntCtl | localLevel;
    WRITE_UINT8(clicIntCtl, CLIC_BASE_REG + sys_clic_intctl(hwiNum));
    return pdPASS;
}

BaseType_t xPortHwiDisable( BaseType_t hwiNum )
{
    configASSERT( hwiNum < configHWI_MAX_VECTOR_CNT && hwiNum >= 0 );

    WRITE_UINT8(0, CLIC_BASE_REG + sys_clic_intie(hwiNum));
    return pdPASS;
}

BaseType_t xPortHwiEnable(BaseType_t hwiNum)
{
    configASSERT( hwiNum < configHWI_MAX_VECTOR_CNT && hwiNum >= 0 );

    WRITE_UINT8(1, CLIC_BASE_REG + sys_clic_intie(hwiNum));
    return pdPASS;
}

void vPortHwiClear( BaseType_t xId )
{
#if (defined configHWI_NESTING && configHWI_NESTING == 1)
    write_custom_csr( LOCIPCLR, xId );
    dsb();
#else
    ( void ) xId;
#endif
}

void vTaskExitNormal( void )
{
    /* PRINTK(ePrintkNotice, "Task[%s] exit\n", pcTaskGetName(NULL)); */
    vTaskDelete( NULL );
}

void vExceptionHandle( uint32_t mepc, uint32_t ra, uint32_t sp )
{
    uint32_t m_cause = read_csr(mcause);
    uint32_t c_cause = read_csr(0xfc2);
    uint32_t m_tval = read_csr(mtval);

    #define EXCEPTION_HANDLE_ID 12
    HwiHandle_t *pxHandle = pxPortHwiGetHandle( EXCEPTION_HANDLE_ID );
    if (pxHandle != NULL) {
        pxHandle->pxCb(EXCEPTION_HANDLE_ID, pxHandle->pvParameters);
    } else {
        PRINTK(ePrintkErr, "Enter exception:\n"
                           "mepc = 0x%x, ra = 0x%x\n"
                           "mcause = 0x%x, ccause = 0x%x\n"
                           "mtval = 0x%x, sp = 0x%x\n", read_csr(mepc), ra, m_cause, c_cause, m_tval, sp);
    }

    while (1) {
        CPUwfi();
    }
}

void CPUwfi( void )
{
    __asm__ __volatile__("wfi");
}

#ifdef __GNUC__
uintptr_t __stack_chk_guard = 0x000a0dff;

static uint32_t ulArchGetTimerCnt( void )
{
    return read_csr(0xB00); /* MCYCLE REG Address */
}

/*
 * If the SP compiling options:-fstack-protector-strong or -fstack-protector-all is enabled,
 * We recommend to implement true random number generator function for __stack_chk_guard
 * value to replace the function implementation template shown as below.
 */
//#pragma GCC push_options
//#pragma GCC optimize ("-fno-stack-protector")
void vArchStackGuardInit( void )
{
    int rnd;

    uint32_t seed = ulArchGetTimerCnt();
#ifdef configSUPPORT_RAND
    srand(seed);
    rnd = rand();
#else
    rnd = seed;
#endif
    __stack_chk_guard = (uintptr_t)rnd;
}
//#pragma GCC pop_options

void __stack_chk_fail(void)
{
    /* __builtin_return_address is a builtin function, building in gcc */
    PRINTK(ePrintkFatal, "stack-protector: Kernel stack is corrupted in: %p\n", __builtin_return_address(0));
}

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void *this, void *caller)
{
}

__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void *this, void *caller)
{
}

#endif

void vApplicationStackOverflowHook (TaskHandle_t xTask, char *pcTaskName)
{
    PRINTK(ePrintkFatal, "StackOverflow pcTaskName:%s\r\n", pcTaskName);
    configASSERT(0);
}
void vApplicationIdleHook(void)
{
    uapi_watchdog_kick();
}

#if configUSE_TICKLESS_IDLE == 1
/**
 * Change Systick Count To Rtc Ms
 */
static uint32_t SystickCountToRtcMs(TickType_t SystickCount)
{
    uint32_t RtcMs;
    if (configTICK_RATE_HZ == 1000)
    {
        return (uint32_t)SystickCount;
    }

    if (RtcMs * 1000 < RtcMs)
    {
        RtcMs = (SystickCount / configTICK_RATE_HZ) * 1000;
    }
    else
    {
        RtcMs = (SystickCount * 1000) / configTICK_RATE_HZ;
    }

    return RtcMs;
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#if ENABLE_LOW_POWER == YES
    uint32_t lock = osal_irq_lock();
    uint32_t delay_time_ms = SystickCountToRtcMs(xExpectedIdleTime);
    pm_enter_sleep(delay_time_ms);
    osal_irq_restore(lock);
#else
    CPUwfi();
#endif
}

#endif