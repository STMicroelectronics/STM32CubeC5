/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : OPAMP configured in follower mode using the ll.
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
  *  Start the MX_OPAMPx.
  */
inline void OPAMP_Start(void)
{
  LL_OPAMP_Enable(MX_OPAMPx);
}
