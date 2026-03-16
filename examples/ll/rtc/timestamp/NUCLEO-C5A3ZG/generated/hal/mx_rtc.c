/**
  ******************************************************************************
  * @file           : mx_rtc.c
  * @brief          : RTC Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_rtc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/** Exported functions for RTC in LL layer (SW instance MyRTC_1)            **/
/******************************************************************************/

system_status_t mx_rtc_init(void)
{
  /* Disable RTC Domain Write Protection */
  LL_PWR_DisableRTCDomainWriteProtection();

  /* Clock configuration */
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
  LL_RCC_EnableRTC();

  /* Disable write protection */
  LL_RTC_DisableWriteProtection();

  /* Enter init mode */
  uint32_t timeout = 1000;
  if (LL_RTC_IsActiveFlag_INIT() == 0U)
  {
    LL_RTC_EnableInitMode();
    while (LL_RTC_IsActiveFlag_INIT() != 1)
    {
      LL_Delay_NoISR(1);
      timeout--;
      if (timeout == 0)
      {
        return SYSTEM_PERIPHERAL_ERROR;
      }
    }
  }

  /* Configure mode and RTC prescalers */
  uint32_t asynchronous_prescaler = 128 - 1;
  uint32_t synchronous_prescaler = 256 - 1;
  LL_RTC_SetPrescalers(asynchronous_prescaler, synchronous_prescaler);
  LL_RTC_SetBinaryMode(LL_RTC_BINARY_NONE);

  /* Configure calendar */
  uint32_t calendar_date = 0x31U;
  uint32_t calendar_hours = 0x12U;
  uint32_t calendar_minutes = 0x0U;
  uint32_t calendar_seconds = 0x0U;

  LL_RTC_SetHourFormatAndShadowRegBypass(LL_RTC_HOUR_FORMAT_24HOUR, LL_RTC_SHADOW_REG_KEEP);
  LL_RTC_DATE_Config(LL_RTC_WEEKDAY_SATURDAY, calendar_date, LL_RTC_MONTH_JULY, 0x25);
  LL_RTC_TIME_Config(LL_RTC_TIME_FORMAT_AM_24H, calendar_hours, calendar_minutes, calendar_seconds);

  /* Exit init mode */
  timeout = 1000;
  LL_RTC_DisableInitMode();
  if (LL_RTC_IsEnabledBypassShadowReg() == 0U)
  {
    if (LL_RTC_IsActiveFlag_INIT() == 0U)
    {
      LL_RTC_ClearFlag_RS();
      while (LL_RTC_IsActiveFlag_RS() == 0U)
      {
        LL_Delay_NoISR(1) ;
        timeout--;
        if (timeout == 0)
        {
          return SYSTEM_PERIPHERAL_ERROR;
        }
      }
    }
  }

  /* Configure timestamp */
  LL_RTC_TS_SetActiveEdge(LL_RTC_TIMESTAMP_EDGE_RISING);
  LL_RTC_TS_Enable();

  /* Enable write protection */
  LL_RTC_EnableWriteProtection();

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**
    RTC GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC13    ------>   RTC_TS
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

  NVIC_SetPriority(RTC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RTC_IRQn);

  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
  NVIC_DisableIRQ(RTC_IRQn);
  /* ### GPIO deinitialization of RTC: RTC_TS ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_13, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PIN_RESET);
}

/******************************************************************************/
/*    RTC global non-secure interrupts is managed directly in user code.   */
/******************************************************************************/
/*
void RTC_IRQHandler(void)
{
}
  */
