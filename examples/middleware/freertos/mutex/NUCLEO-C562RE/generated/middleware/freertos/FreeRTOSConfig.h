/*
 * FreeRTOS Kernel V11.2.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configENABLE_MPU                              0U
#define configENABLE_FPU                              0U
#define configUSE_TICKLESS_IDLE                       0U
#define configUSE_PREEMPTION                          1U
#define configUSE_TIME_SLICING                        1U
#define configMINIMAL_STACK_SIZE                      128U
#define configMAX_PRIORITIES                          32U
#define configUSE_MUTEXES                             1U
#define configUSE_RECURSIVE_MUTEXES                   1U
#define configUSE_COUNTING_SEMAPHORES                 1U
#define configUSE_TASK_NOTIFICATIONS                  1U
#define configUSE_QUEUE_SETS                          1U
#define configQUEUE_REGISTRY_SIZE                     8U
#define configUSE_EVENT_GROUPS                        1U
#define configUSE_STREAM_BUFFERS                      0U
#define configUSE_TIMERS                              1U
#define configTIMER_TASK_PRIORITY                     2U
#define configTIMER_QUEUE_LENGTH                      10U
#define configTIMER_TASK_STACK_DEPTH                  128U
#define configIDLE_SHOULD_YIELD                       1U
#define configUSE_APPLICATION_TASK_TAG                0U
#define configUSE_PORT_OPTIMISED_TASK_SELECTION       0U
#define configENABLE_BACKWARD_COMPATIBILITY           1U
#define configMAX_TASK_NAME_LEN                       16U
#define configSTACK_DEPTH_TYPE                        size_t
#define configMESSAGE_BUFFER_LENGTH_TYPE              size_t
#define configRUN_TIME_COUNTER_TYPE                   uint32_t
#define configRECORD_STACK_HIGH_ADDRESS               0U
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS       5U
#define configKERNEL_PROVIDED_STATIC_MEMORY           1U
#define configUSE_TRACE_FACILITY                      0U
#define configUSE_NEWLIB_REENTRANT                    0U

/* Clock and tick configuration definitions. */
#define configCPU_CLOCK_HZ                            144000000U
#define configSYSTICK_CLOCK_HZ                        18000000U
#define configTICK_RATE_HZ                            (TickType_t)1000U
#define configUSE_16_BIT_TICKS                        0U
#define configCHECK_HANDLER_INSTALLATION              1U

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY       15U

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values). */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5U

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION               0U
#define configSUPPORT_DYNAMIC_ALLOCATION              1U
#define configTOTAL_HEAP_SIZE                         (size_t)8192U
#define configAPPLICATION_ALLOCATED_HEAP              0U
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP     0U
#define configHEAP_CLEAR_MEMORY_ON_FREE               0U
#define configUSE_MINI_LIST_ITEM                      1U
#define configENABLE_HEAP_PROTECTOR                   0U

/* Set the following definitions to 1 to include the API function, or zero to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                      1U
#define INCLUDE_vTaskDelete                           1U
#define INCLUDE_vTaskDelay                            1U
#define INCLUDE_vTaskSuspend                          1U
#define INCLUDE_uxTaskPriorityGet                     1U
#define INCLUDE_xTaskDelayUntil                       1U
#define INCLUDE_xTaskGetSchedulerState                1U
#define INCLUDE_xTaskResumeFromISR                    1U
#define INCLUDE_uxTaskGetStackHighWaterMark           1U
#define INCLUDE_uxTaskGetStackHighWaterMark2          0U
#define INCLUDE_xTaskGetCurrentTaskHandle             1U
#define INCLUDE_eTaskGetState                         1U
#define INCLUDE_xTaskAbortDelay                       0U
#define INCLUDE_xTaskGetHandle                        1U

#define INCLUDE_xQueueGetMutexHolder                  1U
#define INCLUDE_xSemaphoreGetMutexHolder              1U
#define INCLUDE_xEventGroupSetBitFromISR              0U
#define INCLUDE_xTimerPendFunctionCall                0U

/* Hook and callback function related definitions. */
#define configUSE_IDLE_HOOK                           0U
#define configUSE_TICK_HOOK                           0U
#define configCHECK_FOR_STACK_OVERFLOW                0U
#define configUSE_MALLOC_FAILED_HOOK                  0U
#define configUSE_DAEMON_TASK_STARTUP_HOOK            0U
#define configUSE_SB_COMPLETED_CALLBACK               0U

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS                 0U
#define configUSE_STATS_FORMATTING_FUNCTIONS          0U

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                         0U
#define configMAX_CO_ROUTINE_PRIORITIES               2U

/* ARMv8-M ports related definitions. */
#define configENABLE_TRUSTZONE                        0U
#define configRUN_FREERTOS_SECURE_ONLY                0U
#define configENABLE_MVE                              0U

/* ARMv8.1-M ports related definitions. */
#define configENABLE_PAC                              0U
#define configENABLE_BTI                              0U

/* Cortex-M specific definitions. Use the system definition, if there is one. */
#ifdef __NVIC_PRIO_BITS
 /* __NVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS                             __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS                             4U
#endif

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY   	     ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY              ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x )                            if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}

#endif /* FREERTOS_CONFIG_H */
