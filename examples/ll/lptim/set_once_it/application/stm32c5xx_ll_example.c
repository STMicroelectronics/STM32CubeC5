/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : single delayed high pulse for low-power event signaling using the Low-Power Timer (LPTIM) through
  *                  the LL LPTIM API.
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
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
#define LPTIM_DIEROK_TIMEOUT (1000U) /* 1s timeout */
#endif /* USE_LL_APP_TIMEOUT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_lptimx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief:  Start the MX_LPTIMx output compare channel in interrupt mode.
  * retval: SYSTEM_OK When MX_LPTIM_X_CHANNEL channel started successfully.
  * retval: SYSTEM_PERIPHERAL_ERROR When MX_LPTIM_X_CHANNEL channel not started successfully.
  */
inline system_status_t LPTIM_StartCompareMatchChannel_IT(void)
{
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout = LPTIM_DIEROK_TIMEOUT;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableIT(MX_LPTIMx, (LL_LPTIM_DIER_CMP1OKIE | LPTIM_DIER_CC1IE));

  while (LL_LPTIM_IsActiveFlag_DIEROK(MX_LPTIMx) == 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout-- == 0)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  if (return_status == SYSTEM_OK)
  {
    LL_LPTIM_ClearFlag_DIEROK(MX_LPTIMx);

    LL_LPTIM_CC_SetChannelMode(MX_LPTIMx, MX_LPTIM_X_CHANNEL, LL_LPTIM_CCMODE_OUTPUT_PWM);

    LL_LPTIM_CC_EnableChannel(MX_LPTIMx, MX_LPTIM_X_CHANNEL);
  }

  return return_status;
} /* LPTIM_StartCompareMatchChannel_IT */

/**
  * brief:  Start the MX_LPTIMx in polling mode.
  */
inline void LPTIM_Start(void)
{
  LL_LPTIM_Disable(MX_LPTIMx);

  LL_LPTIM_SetWaveform(MX_LPTIMx, LL_LPTIM_OC_WAVEFORM_SETONCE);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_ONESHOT);
} /* LPTIM_Start */


/**
  * brief:  Handles the MX_LPTIMx interrupt.
  */
void mx_lptimx_irqhandler(void)
{
  const uint32_t flag_status = LL_LPTIM_READ_REG(MX_LPTIMx, ISR);
  const uint32_t flag_status_masked = flag_status & LL_LPTIM_READ_REG(MX_LPTIMx, DIER);

  /* Capture compare match */
  if ((flag_status_masked & LL_LPTIM_ISR_CC1IF) != 0U)
  {
    LL_LPTIM_ClearFlag_CC1(MX_LPTIMx);

    LPTIM_CompareMatchCallback();
  }
} /* mx_lptimx_irqhandler */
