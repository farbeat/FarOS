/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* See https://www.freertos.org/Using-FreeRTOS-on-RISC-V.html */
#define configMTIME_BASE_ADDRESS        0
#define configMTIMECMP_BASE_ADDRESS     0

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				1


#define configTICK_CLOCK_HZ            (clocks_get_ccrg_clk_freq(1))
#define configCPU_CLOCK_HZ				clocks_get_core_frequency()

#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES			( 56 )
#ifdef CONFIG_PSRAM_SUPPORT
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 768 )
#else
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 256 )
#endif
#define configTOTAL_HEAP_SIZE			( ( size_t ) 307200 ) /* 300K <= g_intheap_size */
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			0
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		8
#define configCHECK_FOR_STACK_OVERFLOW	2
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_MALLOC_FAILED_HOOK	1
#define configUSE_APPLICATION_TASK_TAG	1
#define configUSE_COUNTING_SEMAPHORES	1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configSUPPORT_STATIC_ALLOCATION 1
#define configUSE_POSIX_ERRNO           1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS				1
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH		6
#define configTIMER_TASK_STACK_DEPTH	( 512 )

/* RISC-V stack definitions. */
//#define configISR_STACK_SIZE_WORDS		2048
//#define configEXC_STACK_SIZE_WORDS		1024

/* RISC-V hwi definitions. */
#define configHWI_SYS_VECTOR_CNT 26
#define configHWI_USR_VECTOR_CNT 126
#define configHWI_MAX_VECTOR_CNT ( configHWI_SYS_VECTOR_CNT + configHWI_USR_VECTOR_CNT )

/* Task priorities.  Allow these to be overridden. */
#ifndef uartPRIMARY_PRIORITY
	#define uartPRIMARY_PRIORITY		( configMAX_PRIORITIES - 3 )
#endif

#ifndef configRECORD_STACK_HIGH_ADDRESS
    #define configRECORD_STACK_HIGH_ADDRESS    1
#endif

/* FPU config 0 or 1 */
#define configFPU_ENABLE 1

//#define configPSRAM_SUPPORT

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet			1
#define INCLUDE_uxTaskPriorityGet			1
#define INCLUDE_vTaskDelete					1
#define INCLUDE_vTaskCleanUpResources		1
#define INCLUDE_vTaskSuspend				1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay					1
#define INCLUDE_eTaskGetState				1
#define INCLUDE_xTimerPendFunctionCall		1
#define INCLUDE_xTaskAbortDelay				1
#define INCLUDE_xTaskGetHandle				1
#define INCLUDE_xSemaphoreGetMutexHolder	1

#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_uxTaskGetStackHighWaterMark 1

#define pdMS_TO_TICKS( xTimeInMs ) ( ( xTimeInMs ) == UINT32_MAX ? UINT32_MAX : ( ( TickType_t ) ( ( ( TickType_t ) \
    ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ + ( TickType_t ) 1000U - 1 ) / ( TickType_t ) 1000U ) ) )


#ifndef __ASSEMBLER__
extern void vAssertCalled( const char *pcFile, uint32_t ulLine );
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FUNCTION__, __LINE__ )
#endif /* __ASSEMBLER__ */


#ifdef FREERTOS_CPUP
    #define configGENERATE_RUN_TIME_STATS 1
    #define configUSE_TRACE_FACILITY 1
    #define configUSE_STATS_FORMATTING_FUNCTIONS 1
    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
    #define portGET_RUN_TIME_COUNTER_VALUE() xTaskGetTickCount()
#else
    #define configGENERATE_RUN_TIME_STATS	0
    #define configUSE_TRACE_FACILITY 1
#endif /* FREERTOS_CPUP */

#endif /* FREERTOS_CONFIG_H */
