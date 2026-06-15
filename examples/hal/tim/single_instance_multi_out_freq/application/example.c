/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM single instance multi output frequencies with HAL API
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_tim_handle_t *pTIM;  /* pointer referencing the TIM handle from the generated code */

/* Private functions prototype -----------------------------------------------*/
static void OutputCompareCallback(hal_tim_handle_t *htim, hal_tim_channel_t channel);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the TIM for output compare.
    */
  pTIM = mx_example_tim_init();
  if (pTIM != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }
  /* Register the callback for output compare interrupt */
  HAL_TIM_RegisterCompareMatchCallback(pTIM, OutputCompareCallback);

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
    * Starts the timer in Output Capture mode for each channel in IT mode.
    */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (HAL_TIM_OC_StartChannel_IT(pTIM, MX_TIM_1X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel_IT(pTIM, MX_TIM_2X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel_IT(pTIM, MX_TIM_3X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel_IT(pTIM, MX_TIM_4X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the TIM peripheral before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_tim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief: Output Compare callback in non blocking mode
  * the callback updates the CCR value to keep the frequency
  * param  htim : Timer handle
  * param  channel : Timer channel
  * retval None
  */
static void OutputCompareCallback(hal_tim_handle_t *htim, hal_tim_channel_t channel)
{
  uint32_t pulse = 0;
  hal_tim_oc_compare_unit_t compare_unit;
  uint32_t autoreload_value;
  uint32_t current_compare = 0;
  uint32_t next_compare = 0;

  /* Get configured autoreload value */
  autoreload_value = HAL_TIM_GetPeriod(htim);
  if (autoreload_value == 0U)
  {
    return;
  }

  switch (channel)
  {
    case MX_TIM_1X_CHANNEL:
      pulse = MX_PULSE_1X;
      compare_unit = HAL_TIM_OC_COMPARE_UNIT_1;
      break;

    case MX_TIM_2X_CHANNEL:
      pulse = MX_PULSE_2X;
      compare_unit = HAL_TIM_OC_COMPARE_UNIT_2;
      break;

    case MX_TIM_3X_CHANNEL:
      pulse = MX_PULSE_3X;
      compare_unit = HAL_TIM_OC_COMPARE_UNIT_3;
      break;

    case MX_TIM_4X_CHANNEL:
      pulse = MX_PULSE_4X;
      compare_unit = HAL_TIM_OC_COMPARE_UNIT_4;
      break;

    default:
      return;
  }

  current_compare = HAL_TIM_OC_GetCompareUnitPulse(htim, compare_unit);
  next_compare = current_compare + pulse;
  /* Check for overflow and wrap around if necessary */
  if (next_compare >= autoreload_value)
  {
    next_compare -= autoreload_value;
  }
  HAL_TIM_OC_SetCompareUnitPulse(htim, compare_unit, next_compare);
}
