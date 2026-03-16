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
/** Exported functions for RTC in HAL layer (SW instance MyRTC_1)            **/
/******************************************************************************/
system_status_t mx_rtc_init(void)
{
  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  /* Clock configuration */
  if (HAL_RCC_RTC_SetKernelClkSource(HAL_RCC_RTC_CLK_SRC_LSE) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  HAL_RCC_RTCAPB_EnableClock();

  HAL_RCC_RTC_EnableKernelClock();

  /* Disable write protection */
  if (HAL_RTC_DisableWriteProtection() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enter init mode */
  if (HAL_RTC_EnterInitMode() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Configure mode and RTC prescalers */
  hal_rtc_config_t rtc_config;
  rtc_config.mode = HAL_RTC_MODE_BCD;
  rtc_config.asynch_prediv = 128 - 1;
  rtc_config.synch_prediv = 256 - 1;
  if (HAL_RTC_SetConfig(&rtc_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Configure calendar */
  hal_rtc_calendar_config_t calendar_config;
  calendar_config.hour_format = HAL_RTC_CALENDAR_HOUR_FORMAT_24;
  calendar_config.bypass_shadow_register = HAL_RTC_CALENDAR_SHADOW_REG_KEEP;
  if (HAL_RTC_CALENDAR_SetConfig(&calendar_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  hal_rtc_date_t calendar_date;
  calendar_date.wday = HAL_RTC_WEEKDAY_SATURDAY;
  calendar_date.mon = HAL_RTC_MONTH_JULY;
  calendar_date.mday = 31U;
  calendar_date.year = 25U;

  hal_rtc_time_t calendar_time;
  calendar_time.am_pm = HAL_RTC_TIME_FORMAT_AM_24H;
  calendar_time.hour = 12U;
  calendar_time.min = 0U;
  calendar_time.sec = 0U;
  if (HAL_RTC_CALENDAR_SetDateTime(&calendar_date, &calendar_time) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Exit init mode */
  if (HAL_RTC_ExitInitMode() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Configure timestamp */
  hal_rtc_timestamp_config_t timestamp_config;
  timestamp_config.input_edge_polarity = HAL_RTC_TIMESTAMP_EDGE_RISING;
  if (HAL_RTC_TIMESTAMP_SetConfig(&timestamp_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  if (HAL_RTC_TIMESTAMP_EnablePinSource() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enable write protection */
  if (HAL_RTC_EnableWriteProtection() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  HAL_RCC_GPIOC_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    RTC GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC13    ------>   RTC_TS
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_13, &gpio_config);

  HAL_CORTEX_NVIC_SetPriority(RTC_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(RTC_IRQn);

  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
  /* De-initialize all GPIO pins associated with RTC */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_13);

  HAL_CORTEX_NVIC_DisableIRQ(RTC_IRQn);
}

 /******************************************************************************/
/**                     RTC global non-secure interrupts                     **/
/******************************************************************************/
void RTC_IRQHandler(void)
{
  HAL_RTC_IRQHandler();
}
