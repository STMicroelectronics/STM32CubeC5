/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Timeout using the LPTIM through the LL LPTIM API.
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
void mx_lptimx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Configure the MX_LPTIMx to operate in low power mode.
  */
inline void LPTIM_LowPowerConfig(void)
{
  LL_APB3_GRP1_EnableClockLowPower(LL_APB3_GRP1_PERIPH_LPTIM1);
}


/**
  *  Start the MX_LPTIMx timeout in interrupt mode.
  *  SYSTEM_PERIPHERAL_ERROR MX_LPTIMx start error
  *  SYSTEM_OK               MX_LPTIMx start success
  */
inline system_status_t LPTIM_Start_IT(void)
{
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = LPTIM_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableTimeout(MX_LPTIMx);

  LL_LPTIM_EnableIT(MX_LPTIMx, LL_LPTIM_DIER_CC1IE);

  while (LL_LPTIM_IsActiveFlag_DIEROK(MX_LPTIMx) == 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout_ms-- == 0U)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  LL_LPTIM_ClearFlag_DIEROK(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_CONTINUOUS);

  return return_status;
}


/**
  *  Reset the MX_LPTIMx counter Register.
  */
inline void LPTIM_ResetCounter(void)
{
  LL_LPTIM_ResetCounter(MX_LPTIMx);
}


/**
  *  Handles the MX_LPTIMx interrupts.
  */
void mx_lptimx_irqhandler(void)
{
  /* Check if the interrupt is due to Autoreload match (timeout) */
  if (LL_LPTIM_IsActiveFlag_CC1(MX_LPTIMx) != 0U)
  {
    LL_LPTIM_ClearFlag_CC1(MX_LPTIMx);

    LPTIM_CompareMatchCallback();
  }
}
