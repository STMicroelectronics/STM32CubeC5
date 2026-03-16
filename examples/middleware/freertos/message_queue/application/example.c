/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "mx_freertos_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/** the FreeRTOS initialization is triggered by the applicative code
  * all FreeRTOS resources are created in the app_synctasks_init().
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  if (app_synctasks_init() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** The applicative processing is handled via the FreeRTOS tasks.
  * The app_process() equivalent content is offloaded to app_freertos.c.
  */

app_status_t app_deinit(void)
{
  /** This API is not used in this example (infinite loop).
    * It is optimized out by the toolchain.
    */
  return EXEC_STATUS_ERROR;
} /* end app_deinit */
