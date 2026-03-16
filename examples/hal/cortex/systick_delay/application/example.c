/**
  *********************************************************************************
  * file           : example.c
  * brief          : Use of millisecond precision delay based on default HAL 1ms
  *                  SysTick configuration.
  *                  This delay is demonstrated thanks to a GPIO toggling.
  *********************************************************************************
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
/* Private functions prototype -----------------------------------------------*/


/** There is nothing to initialize, because systick is already configured by HAL initialization
  */
app_status_t app_init(void)
{
  return EXEC_STATUS_INIT_OK;
} /* end app_init */


/** ########## Step 1 ##########
  * Inserts a 50ms delay and toggles the gpio pin.
  * HAL_Delay is by default implemented as an active loop, based on the 1ms SysTick interruption
  */
app_status_t app_process(void)
{
  /* @user: It is possible to modify the duration of the delay to change led blinking frequency */
  HAL_Delay(50);
  led_toggle(MX_STATUS_LED);

  return EXEC_STATUS_OK;
} /* end app_process */


/** GPIO deinitialization
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */

