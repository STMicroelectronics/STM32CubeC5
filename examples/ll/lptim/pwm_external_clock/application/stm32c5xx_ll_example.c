/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Generate LPTIM PWM signal using external clock with LL API.
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
  * Configure the MX_LPTIMx to operate in low power mode.
  */
inline void LPTIM_LowPowerConfig(void)
{
  LL_APB3_GRP1_EnableClockLowPower(LL_APB3_GRP1_PERIPH_LPTIM1);
}


/**
  *  Enable the MX_LPTIMx output channel in polling mode.
  */
inline void LPTIM_EnableOutputCompareChannel(void)
{
  LL_LPTIM_CC_DisableChannel(MX_LPTIMx, MX_LPTIM_X_CHANNEL);
  LL_LPTIM_CC_SetChannelMode(MX_LPTIMx, MX_LPTIM_X_CHANNEL, LL_LPTIM_CCMODE_OUTPUT_PWM);
  LL_LPTIM_CC_EnableChannel(MX_LPTIMx, MX_LPTIM_X_CHANNEL);
}


/**
  *  Start the low power timer in polling mode.
  */
inline void LPTIM_Start(void)
{
  volatile uint32_t cfgr = (LL_LPTIM_READ_REG(MX_LPTIMx, CFGR) & ~(LL_LPTIM_OC_WAVEFORM_SETONCE | LPTIM_CFGR_TIMOUT));

  LL_LPTIM_WRITE_REG(MX_LPTIMx, CFGR, cfgr);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
}
