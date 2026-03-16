/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Compare a voltage level applied on a GPIO pin
  *                  to the internal voltage reference, in interrupt mode (LL API).
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
static inline void DelayUs(uint32_t delay_us);
void mx_compx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

inline void COMP_Activate_IT(void)
{
  /* Enable the COMPx hardware instance */
  LL_COMP_Enable(MX_COMPx);

  /** Stabilization delay.
    * Wait for both:
    * - the comparator startup time,
    * - and  VrefInt to reach a stable value after enabling the COMPx hardware instance.
    * Select the maximum value of LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US and LL_COMP_DELAY_STARTUP_US.
    */
  DelayUs(LL_COMP_DELAY_VOLTAGE_SCALER_STAB_US);

  LL_EXTI_ClearRisingFlag_32_63(LL_EXTI_LINE_34);
  LL_EXTI_ClearFallingFlag_32_63(LL_EXTI_LINE_34);

  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_34);
}


/**
  *  Handles the MX_COMPx interrupts.
  */
void mx_compx_irqhandler(void)
{
  /* Check the interrupt concerns the MX_COMPx trigger */
  if ((LL_EXTI_IsActiveRisingFlag_32_63(LL_EXTI_LINE_34) != 0UL)
      || (LL_EXTI_IsActiveFallingFlag_32_63(LL_EXTI_LINE_34) != 0UL))
  {
    /* Clear the EXTI flags */
    LL_EXTI_ClearRisingFlag_32_63(LL_EXTI_LINE_34);
    LL_EXTI_ClearFallingFlag_32_63(LL_EXTI_LINE_34);
    /* Call the user callback */
    COMP_TriggerOutputCallback();
  }
}


/**
  * Delays execution for an approximate number of microseconds.
  * Calculates the wait loop count based on the CMSIS variable "SystemCoreClock".
  * The delay is approximate and affected by CPU pipeline, branch prediction, cache, and compiler optimizations.
  * For core clock frequencies below 500 kHz, the delay is limited to a few CPU cycles.
  *
  * warning: This function provides only an approximate delay and must not be used for precise timing.
  * param: delay_us Number of microseconds to delay.
  */
static inline void DelayUs(uint32_t delay_us)
{
  volatile uint32_t wait_loop_index = ((delay_us * (SystemCoreClock >> 19U)) >> 2U);
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }
}
