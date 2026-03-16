/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : RTC start alarm in interrupt mode with LL API
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
void mx_rtcx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Disable the MX_RTCx domain write protection (RCC MX_RTCx domain control register RCC_BDCR, MX_RTCx registers,
  *  TAMP registers, backup registers and backup SRAM).
  */
inline void PWR_DisableRTCDomainWriteProtection(void)
{
  LL_PWR_DisableRTCDomainWriteProtection();
}


/**
  *  Start the MX_RTCx alarm in interrupt mode.
  */
inline void RTC_StartAlarm_IT(void)
{
  LL_RTC_ALM_Start(LL_RTC_ALARM_A, LL_RTC_ALMA_IT_ENABLE);
}


/**
  *  Enable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *  Tamper registers, backup registers and backup SRAM).
  */
inline void PWR_EnableRTCDomainWriteProtection(void)
{
  LL_PWR_EnableRTCDomainWriteProtection();
}


/**
  *  Handles the MX_RTCx interrupts.
  */
void mx_rtcx_irqhandler(void)
{
  uint32_t flags = LL_RTC_READ_REG(SR);

  if (LL_RTC_ALARM_A_GET_FLAG(flags) != 0U)
  {
    LL_RTC_ClearFlag_ALRA();

    RTC_AlarmEventCallback();
  }
}
