/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : generate a finite sequence of pulses with precise timing control followed by a permanent reset of
  *                  the output signal using LL API.
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
void mx_lptimx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_LPTIMx output compare channel in interrupt mode.
  */
inline void LPTIM_StartCompareMatchChannel_IT(void)
{
  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableIT(MX_LPTIMx, (LL_LPTIM_DIER_CMP1OKIE | LPTIM_DIER_CC1IE));

  LL_LPTIM_ClearFlag_DIEROK(MX_LPTIMx);

  LL_LPTIM_CC_SetChannelMode(MX_LPTIMx, MX_LPTIM_X_CHANNEL, LL_LPTIM_CCMODE_OUTPUT_PWM);

  LL_LPTIM_CC_EnableChannel(MX_LPTIMx, MX_LPTIM_X_CHANNEL);
} /* LPTIM_StartCompareMatchChannel_IT */


/**
  *  Start the MX_LPTIMx in interrupt mode.
  */
inline void LPTIM_Start_IT(void)
{
  volatile uint32_t cfgr = (LL_LPTIM_READ_REG(MX_LPTIMx, CFGR) & (~(LL_LPTIM_OC_WAVEFORM_SETONCE | LPTIM_CFGR_TIMOUT)));

  LL_LPTIM_Disable(MX_LPTIMx);

  LL_LPTIM_WRITE_REG(MX_LPTIMx, CFGR, cfgr);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableIT(MX_LPTIMx,
                    (LL_LPTIM_DIER_ARROKIE | LL_LPTIM_DIER_ARRMIE | LL_LPTIM_DIER_REPOKIE | LL_LPTIM_DIER_UEIE));

  LL_LPTIM_ClearFlag_DIEROK(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_ONESHOT);
} /* LPTIM_Start_IT */


/**
  *  Get the MX_LPTIMx current value of the repetition counter.
  *  uint32_t Current value of the repetition counter.
  */
uint32_t LPTIM_GetRepetitionCounter(void)
{
  return LL_LPTIM_GetRepetition(MX_LPTIMx);
} /* LPTIM_GetRepetitionCounter */


/**
  *  Handles the MX_LPTIMx interrupts.
  */
void mx_lptimx_irqhandler(void)
{
  const uint32_t flag_status        = LL_LPTIM_READ_REG(MX_LPTIMx, ISR);
  const uint32_t flag_status_masked = flag_status & LL_LPTIM_READ_REG(MX_LPTIMx, DIER);

  /* Capture compare 1 interrupt caught */
  if ((flag_status_masked & LL_LPTIM_ISR_CC1IF) != 0U)
  {
    LL_LPTIM_ClearFlag_CC1(MX_LPTIMx);

    LPTIM_CompareMatchCallback();
  }

  /* Autoreload matched interrupt */
  if ((flag_status_masked & LL_LPTIM_ISR_ARRM) != 0U)
  {
    LL_LPTIM_ClearFlag_ARRM(MX_LPTIMx);
  }

  /* Repetition counter underflowed or == 0 and LPTIM counter overflow */
  if ((flag_status_masked & LL_LPTIM_ISR_UE) != 0U)
  {
    LL_LPTIM_ClearFlag_UE(MX_LPTIMx);

    LPTIM_UpdateCallback();
  }
} /* mx_lptimx_irqhandler */
