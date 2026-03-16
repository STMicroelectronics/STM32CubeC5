/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM EXT Trigger Synchro with HAL API
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
hal_tim_handle_t *pTIMa;  /* TIMx captures external signals and generates PWM */
hal_tim_handle_t *pTIMb;  /* TIMy synchronized with TIMx and generates PWM */
hal_tim_handle_t *pTIMc;  /* TIMz synchronized with TIMy and generates PWM */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the GPIO and the TIM for PWM generation.
  * The init of the TIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pTIMa = mx_example_tim_a_init();
  if (pTIMa == NULL)
  {
    goto _app_init_exit;
  }
  pTIMb = mx_example_tim_b_init();
  if (pTIMb == NULL)
  {
    goto _app_init_exit;
  }
  pTIMc = mx_example_tim_c_init();
  if (pTIMc == NULL)
  {
    goto _app_init_exit;
  }
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Start all three timers to generate synchronized signals
    * when an external trigger occurs.
    */

  /* Start the timer's PWM generation */
  if (HAL_TIM_OC_StartChannel(pTIMa, MX_PWM_A_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIMb, MX_PWM_B_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIMc, MX_PWM_C_CHANNEL_Y) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Start the timer input channel */
  if (HAL_TIM_IC_StartChannel(pTIMa, MX_PWM_A_CHANNEL_Z) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Start the timer a */
  if (HAL_TIM_Start(pTIMa) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Start the timer b */
  if (HAL_TIM_Start(pTIMb) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Start the timer c */
  if (HAL_TIM_Start(pTIMc) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** The app_deinit() function is provided as a reference only.
  * This can be used to de-initialize GPIO and TIM peripherals in case of error.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_tim_a_deinit();
  mx_example_tim_b_deinit();
  mx_example_tim_c_deinit();

  return return_status;
} /* end app_deinit */
