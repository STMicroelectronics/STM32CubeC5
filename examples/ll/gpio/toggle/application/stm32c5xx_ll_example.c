/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : GPIO toggle with LL API
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
/* Private functions ---------------------------------------------------------*/

/**
  *  Toggle GPIO pin.
  */
inline void  GPIO_TogglePin(void)
{
  LL_GPIO_TogglePin(MX_STATUS_LED_PORT, MX_STATUS_LED_PIN);
}
