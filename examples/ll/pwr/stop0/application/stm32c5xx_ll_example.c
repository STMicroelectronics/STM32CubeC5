/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Enter and exit the Stop0 mode through an EXTI interrupt using the LL API
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

/** brief: Disable RCC bus clocks.
  */
inline void RCC_DisableBuses(void)
{
  /* Disable Sleep and Stop clocks */
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_ALL);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_ALL);
#if (AHB4PERIPH_BASE)
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_ALL);
#endif /* AHB4PERIPH_BASE */
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_ALL);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_ALL);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_ALL);
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_ALL);

  /* Disable buses clocks */
  LL_AHB1_DisableBusClock();
#if (AHB4PERIPH_BASE)
  LL_AHB4_DisableBusClock();
#endif /* AHB4PERIPH_BASE */
  LL_APB1_DisableBusClock();
  LL_APB2_DisableBusClock();
  LL_APB3_DisableBusClock();
}


/** brief: Enable the EXTI Line 13 interrupt.
  */
inline void EXTI_EnableLineMode_IT(void)
{
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
}


/** brief: Clear the Stop flag.
  */
inline void PWR_ClearStopFlag(void)
{
  LL_PWR_ClearFlag_STOP();
}


/** brief: Enter the Stop0 mode request.
  */
inline void PWR_EnterStop0Mode(void)
{
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB_EnableDeepSleep();

  LL_PWR_SetPowerMode(LL_PWR_STOP0_MODE);

  /* Wait For Interrupt Request */
  __WFI();
}


/** brief:  Check if the system has resumed from Stop0 mode.
  * retval: system_status_t
  */
inline system_status_t PWR_CheckSystemStop0Mode(void)
{
  system_status_t system_status = SYSTEM_PERIPHERAL_ERROR;

  /* Check that the system has resumed from Stop0 mode */
  if (LL_PWR_IsActiveFlag_STOP() != 0U)
  {
    system_status = SYSTEM_OK;
  }

  return system_status;
}


/** brief: Handles the EXTI line interrupts.
  */
void mx_extix_irqhandler(void)
{
  if (LL_EXTI_IsActiveRisingFlag_0_31(MX_EXTIX_EXTI_LINE) != 0U)
  {
    LL_EXTI_ClearRisingFlag_0_31(MX_EXTIX_EXTI_LINE);
  }
}
