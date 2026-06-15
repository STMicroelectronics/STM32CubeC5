/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : LPTIM Encoder interface with LL API
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
#define LPTIM_TIMEOUT_MS (1000U) /* MX_LPTIMx timout 1s */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_LPTIMx timeout in continuous mode.
  *  SYSTEM_PERIPHERAL_ERROR MX_LPTIMx start error
  *  SYSTEM_OK               MX_LPTIMx start success
  */
inline void LPTIM_Start(void)
{
  LL_LPTIM_EnableEncoderMode(MX_LPTIMx);

  /* Enable LPTIM and start in continuous mode */
  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
}
