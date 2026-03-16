/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_extix_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * Enables the EXTI line in interrupt mode.
  */
inline void EXTI_EnableLine_IT(void)
{
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
}

/**
  * Handles the EXTI line interrupts.
  */
void mx_extix_irqhandler(void)
{
  if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_13) != 0U)
  {
    LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_13);

    EXTI_TriggerDetectionCallback();
  }
}
