/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t AlarmExpired = 0U;
/* Private functions prototype -----------------------------------------------*/
system_status_t pre_system_init_hook(void);

/** ########## Step 1 ##########
  * The applicative code initializes the RTC instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_example_rtc_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * Starts RTC alarm A in interrupt mode.
    */
  HAL_RTC_ALARM_Start(HAL_RTC_ALARM_A, HAL_RTC_ALARM_IT_ENABLE);

  /** ########## Step 3 ##########
    * Waits for the RTC alarm interrupt.
    */
  while (AlarmExpired == 0U)
  {
    /** Put the CPU in Wait For Interrupt state. The RTC alarm or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the RTC alarm. It can be replaced by your own code.
      */
    __WFI();
  }

  return EXEC_STATUS_OK;
} /* end app_process */


app_status_t app_deinit(void)
{
  /** Nothing to deinitialize for RTC.
    */
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** RTC alarm A callback
  * @user This implementation of the Alarm callback can be customized.
  * This function is executed if the Alarm interrupt is generated.
  */
void HAL_RTC_AlarmAEventCallback(void)
{
  /* asynchronous processing related to step 3 */
  AlarmExpired = 1U;
}

/**  User hook function called before the HAL_Init() function
  */
system_status_t pre_system_init_hook(void)
{
  /** Reset the backup domain to prevent reusing any previous RTC calendar and alarm. So RTC peripheral registers and
    * RTC clock source (LSE or LSI) will be in their reset state whatever was programmed before.
    * This is implemented before the LSE or the LSI is properly configured in `system_clock_config()`.
    */
  HAL_PWR_DisableRTCDomainWriteProtection();
  HAL_RCC_ResetRTCDomain();

  return SYSTEM_OK;
}
