/**
  ******************************************************************************
  * @file    mx_levelx_app.h
  * @brief   LevelX applicative header file
  ******************************************************************************
    * @attention
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MX_LEVELX_APP_H__
#define __MX_LEVELX_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"
#include "mx_levelx_interfaces.h"

/* Private includes ----------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include <stdio.h>
#include <string.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define APP_DEFAULT_STACK_SIZE    ( configMINIMAL_STACK_SIZE)
#define APP_TASK_PRIORITY       5
#define MAIN_TASK_PRIORITY      10

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
/* Exported macros -----------------------------------------------------------*/
#define TIMEOUT              1000
/* Exported functions prototypes ---------------------------------------------*/
UINT app_levelx_init(VOID);

/* Private defines -----------------------------------------------------------*/
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
#endif
#endif /* __APP_LEVELX_H__ */
