/**
  ******************************************************************************
  * file           : example.c
  * brief          : Configure the TIM peripheral in PWM mode using DMA burst to change duty cycle.
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
#define BUFFER_SIZE 6U
#define DATA1 0x2666
#define DATA2 0x4CCC
#define DATA3 0x7333
#define DATA4 0x9999
#define DATA5 0xBFFF
#define DATA6 0xE666

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_tim_handle_t *pTIM;   /* pointer referencing the TIM handle from the generated code */

/* Capture compare buffer containing Capture Compare Register (CCR) values */
uint32_t CaptureCompareBuffer[BUFFER_SIZE] = {DATA1, DATA2, DATA3, DATA4, DATA5, DATA6};

/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * Initializes the GPIO and the TIM for PWM generation.
  * The init of the TIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
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
    * Starts the Timer PWM generation and updates the duty cycle through a DMA burst.
    * Data buffer size in byte
    */

  if (HAL_TIM_OC_StartChannel(pTIM, MX_TIM_1X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_TIM_2X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM, MX_TIM_3X_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_StartDMABurst(pTIM, HAL_TIM_DMABURST_WRITE, (uint8_t *)CaptureCompareBuffer,
                            BUFFER_SIZE * sizeof(uint32_t)) != HAL_OK)
  {
    goto _app_process_exit;
  }

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
