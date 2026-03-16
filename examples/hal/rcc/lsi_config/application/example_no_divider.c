/**
  ******************************************************************************
  * file           : example_no_divider.c
  * brief          : LSI configuration with HAL_API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Time during the LSI will be disabled */
#define LSI_DISABLE_DELAY_MS 2000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * In RCC examples, the init has already been triggered by mx_system_init() from main.
  * The MCO pin is configured in the generated code to output the LSI clock.
  */
app_status_t app_init(void)
{
  PRINTF("[INFO] Step 1: MCO pin initialized to output the LSI clock.\n");

  return EXEC_STATUS_INIT_OK;
} /* end app_init */


/** ########## Step 2 ##########
  * The LSI is disabled during LSI_DISABLE_DELAY_MS ms and then re-enabled.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (HAL_RCC_LSI_IsEnabled() != HAL_RCC_OSC_ENABLED)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: LSI enabled (%" PRIu32 " Hz) to MCO pin, with a %u divider (MCO freq: %" PRIu32 " Hz).\n",
         (uint32_t)LSI_VALUE,
         MCO_DIVIDER_INT,
         (uint32_t)(LSI_VALUE / MCO_DIVIDER_INT)
        );

  if (HAL_RCC_LSI_Disable() != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: LSI disabled.\n");

  HAL_Delay(LSI_DISABLE_DELAY_MS);

  if (HAL_RCC_LSI_Enable() != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * In RCC examples, it is not possible to deinitialize the RCC before leaving the scenario.
  * The LSI status is restored and outputted on the MCO pin.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 3: LSI status restored and outputted on MCO pin.\n");
  return EXEC_STATUS_OK;
} /* end app_deinit */
