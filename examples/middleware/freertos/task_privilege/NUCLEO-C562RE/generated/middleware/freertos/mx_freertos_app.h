/**
  ******************************************************************************
  * file           : mx_freertos_app.h
  * brief          : Header for mx_freertos_app.c
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef MX_FREERTOS_APP_H
#define MX_FREERTOS_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Private define ------------------------------------------------------------*/
#include <inttypes.h>       /* PRIx8 macros family */
#include <stdio.h>          /* printf */
#include "mx_hal_def.h"     /* aliases to the target-specific generated code */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "mx_led.h"

#define APP_DEFAULT_STACK_SIZE    (2 * configMINIMAL_STACK_SIZE)
#define APP_HIGH_PRIORITY          5
#define APP_LOW_PRIORITY           4

/** Direct to task notification event sent directly to the Idle task to indicate the example status.
  * Attention: configTASK_NOTIFICATION_ARRAY_ENTRIES does not need to be set to 2
  * as we process only 1 notification (success or error).
  */
typedef enum
{
  NOTIFICATION_VALUE_UNKNOWN = 0x00,   /* Default value. */
  NOTIFICATION_VALUE_SUCCESS = 0x01,   /* The example ran as expected. */
  NOTIFICATION_VALUE_ERROR   = 0x02    /* Unrecoverable error encountered while running the example. */
} ExampleStatusNotification_t;

#define EXAMPLE_FLASH_START ( uint32_t * ) 0x08000000
#define EXAMPLE_FLASH_END   ( uint32_t * ) 0x080FFFFF
#define EXAMPLE_SRAM_START  ( uint32_t * ) 0x20000000
#define EXAMPLE_SRAM_END    ( uint32_t * ) 0x20017FFF

#define EXAMPLE_PRIVILEGED_FUNC_START ( uint32_t * ) 0x08000000
#define EXAMPLE_PRIVILEGED_FUNC_END   ( uint32_t * ) 0x08007FFF
#define EXAMPLE_PRIVILEGED_SRAM_START ( uint32_t * ) 0x20000000
#define EXAMPLE_PRIVILEGED_SRAM_END   ( uint32_t * ) 0x20007FFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __CC_ARM ) || defined(__ARMCC_VERSION)

extern uint32_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base;
extern uint32_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Limit;

extern uint32_t Image$$ER_IROM_UNPRIVILEGED$$Base;
extern uint32_t Image$$ER_IROM_UNPRIVILEGED$$Limit;

/* Memory map needed for MPU setup. Must match the one defined in
 * the scatter-loading file (Project.sct). */
extern const uint32_t * __FLASH_segment_start__;
extern const uint32_t * __FLASH_segment_end__;
extern const uint32_t * __SRAM_segment_start__;
extern const uint32_t * __SRAM_segment_end__;

extern const uint32_t * __privileged_functions_start__;
extern const uint32_t * __privileged_functions_end__;
extern const uint32_t * __privileged_sram_start__;
extern const uint32_t * __privileged_sram_end__;

extern const uint32_t * __unprivileged_flash_start__;
extern const uint32_t * __unprivileged_flash_end__;

extern const uint32_t * __syscalls_flash_start__;
extern const uint32_t * __syscalls_flash_end__;
#endif

/* Private prototypes --------------------------------------------------------*/
int32_t app_synctasks_init(void);
void MemManage_Recover(void);
void MemManage_Handler(void);

/** logging macro - just redirects to printf()
  * libc's stdout is redirected to UART thanks to the Basic stdio utility.
  *
  * @user You can enable logs by defining USE_TRACE=1 in the build options.
  */
#if defined(USE_TRACE) && USE_TRACE != 0
#define PRINTF(...)    printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_FREERTOS_APP_H */
