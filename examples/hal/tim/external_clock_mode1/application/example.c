/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM External Clock Mode 1 with HAL API
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
#define PULSE_DELAY  250

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_tim_handle_t *pTIM;  /* pointer referencing the TIM handle from the generated code */
hal_exti_handle_t *pEXTI; /* pointer referencing the EXTI handle from the generated code */

/** Variable to check the EXTI interrupt detection via the IRQ CallBack.
  * Declared as volatile because updated from an external event (interrupt).
  */
volatile uint8_t ButtonPressedFlag = 0;
volatile uint8_t OutputCompareComplete = 0;

/* Private functions prototype -----------------------------------------------*/
static void PulseCountReached(hal_tim_handle_t *htim, hal_tim_channel_t channel);
static void ButtonCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger);

/** ########## Step 1 ##########
  * Initializes the GPIO and the TIM.
  * Configures the GPIO and TIM with register callback.
  * The init of the TIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pTIM = mx_example_tim_init();
  if (pTIM != NULL)
  {
    /* Register the callback for compare match interrupt */
    if (HAL_TIM_RegisterCompareMatchCallback(pTIM, &PulseCountReached) != HAL_OK)
    {
      goto _app_process_exit;
    }
  }

  mx_example_exti_trigger_init();
  pEXTI = mx_example_exti_trigger_gethandle();
  if (pEXTI != NULL)
  {
    /* Associates user callback function to any interrupt edge event*/
    if (HAL_EXTI_RegisterTriggerCallback(pEXTI, &ButtonCallback) != HAL_OK)
    {
      goto _app_process_exit;
    }
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_process_exit:

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  OutputCompareComplete = 0;

  /** ########## Step 2 ##########
    * Starts the EXTI line and wait for the interrupt.
    * User callback function is called when the interrupt is triggered.
    * Start the TIM output channel in interrupt mode.
    */
  HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT);

  if (HAL_TIM_OC_StartChannel_IT(pTIM, MX_TIM_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  while (OutputCompareComplete == 0)
  {
    if (ButtonPressedFlag != 0)
    {
      /* Generate a 250ms pulse */
      HAL_GPIO_WritePin(MX_EXAMPLE_TIM_IN_PORT, MX_EXAMPLE_TIM_IN_PIN, MX_EXAMPLE_TIM_IN_ACTIVE_STATE);
      HAL_Delay(PULSE_DELAY);
      HAL_GPIO_WritePin(MX_EXAMPLE_TIM_IN_PORT, MX_EXAMPLE_TIM_IN_PIN, MX_EXAMPLE_TIM_IN_INACTIVE_STATE);

      /* Reset the flag */
      ButtonPressedFlag = 0;
    }
  }
  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;

}/* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_tim_deinit();
  /* Stop the EXTI mode for the selected EXTI line */
  HAL_EXTI_Disable(pEXTI);

  return return_status;
} /* end app_deinit */


/** Output Compare callback in non-blocking mode
  */
static void PulseCountReached(hal_tim_handle_t *htim, hal_tim_channel_t channel)
{
  /* Prevent unused argument(s) compilation warning */
  STM32_UNUSED(channel);
  OutputCompareComplete = 1;

}

/**  Defines the user callback function.
  *  This function is triggered when the EXTI interrupt happens according to the configured edge.
  */
static void ButtonCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  /* Prevent unused argument(s) compilation warning */
  STM32_UNUSED(trigger);
  /* Flag set to 1 on each button press */
  ButtonPressedFlag = 1;
}