/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Counting external pulses with the LPTIM (Low-Power Timer) using the LL API
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
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
  *  Start the MX_LPTIMx instance in interrupt mode.
  *  SYSTEM_PERIPHERAL_ERROR MX_LPTIMx start error
  *  SYSTEM_OK               MX_LPTIMx start success
  */
inline system_status_t LPTIM_Start_IT(void)
{
  uint32_t cfgr;
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = LPTIM_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  /* Set the MX_LPTIMx mode */
  cfgr = LL_LPTIM_READ_REG(MX_LPTIMx, CFGR);
  cfgr |= (LL_LPTIM_OPERATING_MODE_CONTINUOUS & (LL_LPTIM_OC_WAVEFORM_SETONCE | LPTIM_CFGR_TIMOUT));
  LL_LPTIM_WRITE_REG(MX_LPTIMx, CFGR, cfgr);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableIT(MX_LPTIMx,
                    (LL_LPTIM_DIER_ARROKIE | LL_LPTIM_DIER_ARRMIE | LL_LPTIM_DIER_REPOKIE | LL_LPTIM_DIER_UEIE));

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
} /* LPTIM_Start_IT */


/**
  *  Reset the MX_LPTIMx counter Register.
  */
inline void LPTIM_ResetCounter(void)
{
  LL_LPTIM_ResetCounter(MX_LPTIMx);
} /* LPTIM_ResetCounter */


/**
  *  Handles the MX_LPTIMx interrupts.
  */
void mx_lptimx_irqhandler(void)
{
  uint32_t flag_status        = LL_LPTIM_READ_REG(MX_LPTIMx, ISR);
  uint32_t flag_status_masked = flag_status & LL_LPTIM_READ_REG(MX_LPTIMx, DIER);

  /* Autoreload matched interrupt */
  if ((flag_status_masked & LL_LPTIM_ISR_ARRM) != 0U)
  {
    LL_LPTIM_ClearFlag_ARRM(MX_LPTIMx);

    LPTIM_AutoReloadMatchCallback();
  }

  /* Repetition counter underflowed or == 0 and LPTIM counter overflow */
  if ((flag_status_masked & LL_LPTIM_ISR_UE) != 0U)
  {
    LL_LPTIM_ClearFlag_UE(MX_LPTIMx);
  }
} /* mx_lptimx_irqhandler() */
