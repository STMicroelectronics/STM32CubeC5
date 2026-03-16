/**
  ******************************************************************************
  * file           : example.c
  * brief          : GPIO EXTI detection in interrupt mode with LL API
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Variable to check the EXTI interrupt detection via the IRQ CallBack.
  * Declared as volatile because updated from an external event (interrupt).
  */
volatile uint8_t EXTIDetected = 0U;

/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Configures a GPIO pin to the corresponding EXTI line in interrupt mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_extix_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
}/* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Starts the EXTI line and wait for the interrupt.
    */
  EXTI_EnableLine_IT();

  while (EXTIDetected == 0U)
  {
    /** Put CPU in Wait For Interrupt state. An EXTI interrupt can wake up the CPU.
      * user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */
    __WFI();
  }

  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Trigger detection callback
  * @user This implementation of the MX_EXTIx trigger detection callback can be customized.
  * This function is executed when the trigger detection interrupt is generated.
  */
void EXTI_TriggerDetectionCallback(void)
{
  EXTIDetected = 1U;
}


/** ########## Step 4 ##########
  * Deinitializes the GPIO before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
}/* end app_deinit */
