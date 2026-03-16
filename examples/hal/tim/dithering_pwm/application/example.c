/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM PWM output with HAL API
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
#define DELAY 50 /* 50 milliseconds */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_tim_handle_t *pTIM;  /* pointer referencing the TIM handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the GPIO and the TIM for PWM generation.
  * The init of the TIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pTIM = mx_example_tim_init();
  if (pTIM != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t arr_value; /* ARRx register value */
  uint32_t ccr_value; /* CCRx register value */
  uint32_t Compare;


  /** ########## Step 2 ##########
    * Enable Dithering.
    * Starts the Timer PWM generation for both channels.
    */

  /* Enabling dithering is possible only when timer is stopped.
    * Stop PWM generation
    */
  if (HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Stop(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_EnableDithering(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* rewrite ARR register when dither mode active */
  arr_value = HAL_TIM_GetPeriod(pTIM);
  if (HAL_TIM_SetDitheredPeriod(pTIM, arr_value, HAL_TIM_DITHERING_0_16) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Start PWM generation */
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Get CCR value */
  ccr_value = HAL_TIM_OC_GetCompareUnitPulse(pTIM, hal_tim_oc_channel_to_compare_unit(MX_PWM_CHANNEL_Z));
  for (uint32_t idx = ccr_value; idx < arr_value * 16; idx++)
  {
    HAL_Delay(DELAY);
    /* Increment compare match */
    Compare = idx + 1;
    HAL_TIM_OC_SetCompareUnitPulse(pTIM, hal_tim_oc_channel_to_compare_unit(MX_PWM_CHANNEL_Z), Compare);
  }

  /** ########## Step 3 ##########
    * Disable Dithering.
    * Starts the Timer PWM generation for both channels.
    */
  /* Disabling dithering is possible only when timer is stopped.
    * Stop PWM generation
    */
  if (HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Stop(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_DisableDithering(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* rewrite ARR register when dither mode inactive */
  if (HAL_TIM_SetPeriod(pTIM, arr_value) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* rewrite CCR register when dither mode inactive */
  if (HAL_TIM_OC_SetCompareUnitPulse(pTIM, hal_tim_oc_channel_to_compare_unit(MX_PWM_CHANNEL_Z), 1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Start PWM generation */
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  for (uint32_t idx = 1; idx < arr_value; idx++)
  {
    HAL_Delay(DELAY * 16);
    /* Increment compare match */
    Compare = idx + 1;
    HAL_TIM_OC_SetCompareUnitPulse(pTIM, hal_tim_oc_channel_to_compare_unit(MX_PWM_CHANNEL_Z), Compare);
  }
  /* Initialize CCR to 1 */
  if (HAL_TIM_OC_SetCompareUnitPulse(pTIM, hal_tim_oc_channel_to_compare_unit(MX_PWM_CHANNEL_Z), 1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_tim_deinit();

  return return_status;
} /* end app_deinit */
