/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM 6 STEP PWM with HAL API
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
/* Step Index */
uint8_t Step = 0;
/* Authorize TIM COM event generation */
uint32_t AuthorizeTimComEvent = 0;

/* Private functions prototype -----------------------------------------------*/
static void SixStepCommutationCallback(hal_tim_handle_t *htim);

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
  /* Register the callback for commutation interrupt */
  HAL_TIM_RegisterCommutationCallback(pTIM, SixStepCommutationCallback);

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Starts the Timer PWM generation for 6 channels
    */
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_X) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_XN) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_YN) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_ZN) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start_IT_Opt(pTIM, HAL_TIM_OPT_IT_COMMUTATION) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Authorize TIM COM event generation */
  AuthorizeTimComEvent = 1;

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_tim_deinit();
  return return_status;
} /* end app_deinit */


/** brief  Commutation event callback in non blocking mode
  * param  htim : Timer handle
  * retval None
  */
static void SixStepCommutationCallback(hal_tim_handle_t *htim)
{
  hal_tim_oc_compare_unit_config_t tim_oc_compare_unit_config;
  /* Entry state */
  if (Step == 0)
  {
    /* Next step: Step 1 Configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_X;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_1, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_X);
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_XN);
    HAL_TIM_Start(pTIM);
    /* ChannelZ configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_Z;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_3, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_ZN);
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Z);
    HAL_TIM_Start(pTIM);
    /* ChannelY configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Y);
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_YN);
    Step++;
  }

  if (Step == 1)
  {
    /* Next step: Step 2 Configuration */

    /** ChannelX configuration
      * Same configuration as the previous step
      */

    /* ChannelY configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_Y;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_2, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_YN);

    /* ChannelZ configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_ZN);

    Step++;
  }

  else if (Step == 2)
  {
    /* Next step: Step 3 Configuration */

    /** ChannelY configuration
      * Same configuration as the previous step
      */

    /* ChannelZ configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_Z;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_3, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Z);

    /* ChannelX configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_X);

    Step++;
  }

  else if (Step == 3)
  {
    /* Next step: Step 4 Configuration */

    /** ChannelZ configuration
      * Same configuration as the previous step
      */

    /* ChannelY configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_YN);

    /* ChannelX configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_X;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_1, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_XN);

    Step++;
  }
  else if (Step == 4)
  {
    /* Next step: Step 5 Configuration */

    /* ChannelZ configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Z);

    /** ChannelX configuration
      * Same configuration as the previous step */

    /* ChannelY configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_Y;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_2, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_Y);

    Step++;
  }

  else if (Step == 5)
  {
    /* Next step: Step 6 Configuration */

    /* ChannelZ configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_Z;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_3, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_ZN);

    /* ChannelX configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_XN);

    /** ChannelY configuration
      * Same configuration as the previous step */

    Step++;
  }

  else
  {
    /* Next step: Step 1 Configuration */

    /* ChannelX configuration */
    tim_oc_compare_unit_config.mode     = HAL_TIM_OC_PWM1;
    tim_oc_compare_unit_config.pulse = MX_PULSE_X;
    HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_1, &tim_oc_compare_unit_config);
    HAL_TIM_OC_StartChannel(pTIM, MX_PWM_CHANNEL_X);

    /** ChannelZ configuration
      * Same configuration as the previous step */

    /* ChannelY configuration */
    HAL_TIM_OC_StopChannel(pTIM, MX_PWM_CHANNEL_Y);

    Step = 1;
  }
}


/** brief  SYSTICK callback to generate a commutation event
  */
void HAL_CORTEX_SYSTICK_Callback(void)
{
  if (AuthorizeTimComEvent)
  {
    /* Generate a commutation event each 1 ms */
    HAL_TIM_GenerateEvent(pTIM, HAL_TIM_SW_EVENT_COM);
  }
}
