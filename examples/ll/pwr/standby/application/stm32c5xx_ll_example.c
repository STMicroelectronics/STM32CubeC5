/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Enter and exit the Standby mode through a wakeup pin using the LL API
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

/** brief: Clear Standby flag.
  */
inline void PWR_ClearStandbyFlag(void)
{
  LL_PWR_ClearFlag_SB();
}


/** brief: Clear wake-up pin flag.
  */
inline void PWR_ClearWakeupPinFlag(void)
{
  LL_PWR_ClearFlag_WU(LL_PWR_WAKEUP_PIN_4);
}


/** brief: Enter the Standby mode request.
  */
inline void PWR_EnterStandbyMode(void)
{
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB_EnableDeepSleep();

  LL_PWR_SetPowerMode(LL_PWR_STANDBY_MODE);

  /* Wait For Interrupt Request */
  __WFI();
}
