/**
  ******************************************************************************
  * file           : example.c
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
#include "example.h"
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t AlarmExpired = 0U;

/* Private functions prototype -----------------------------------------------*/
system_status_t pre_system_init_hook(void);


/** ########## Step 1 ##########
  * The applicative code initializes the MX_RTCx instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_rtcx_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * Starts the MX_RTCx alarm in interrupt mode.
    */
  /* Disable MX_RTCx Domain Write Protection */
  PWR_DisableRTCDomainWriteProtection();

  RTC_StartAlarm_IT();

  /** ########## Step 3 ##########
    * Waits for the MX_RTCx alarm interrupt.
    */
  while (AlarmExpired == 0U)
  {
    /** Put the CPU in Wait For Interrupt state. The MX_RTCx alarm or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the MX_RTCx alarm. It can be replaced by your own code.
      */
    __WFI();
  }

  /* Enable MX_RTCx Domain Write Protection */
  PWR_EnableRTCDomainWriteProtection();

  return EXEC_STATUS_OK;
} /* end app_process */


app_status_t app_deinit(void)
{
  mx_rtcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** MX_RTCx alarm callback
  * This function is executed if the Alarm interrupt is generated.
  */
void RTC_AlarmEventCallback(void)
{
  /* Asynchronous processing related to step 3 */
  AlarmExpired = 1U;
}
