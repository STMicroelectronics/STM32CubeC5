/**
  *********************************************************************************
  * file           : example.c
  * brief          : Use of systick to measure a processing duration thanks to HAL API.
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

/** Arbitrary number to be checked.
  * It is a high prime number so that processing duration is higher than hundreds of milliseconds and up to few seconds.
  */
#define NUMBER 34567901UL

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Computed processing duration in milliseconds */
volatile uint32_t ProcessingDuration;

/* Private functions prototype -----------------------------------------------*/
static uint8_t Prime(uint32_t n);


/** There is nothing to initialize, because systick is already configured by HAL initialization
  */
app_status_t app_init(void)
{
  return EXEC_STATUS_INIT_OK;
} /* end app_init */


/** ########## Step 1 ##########
  * Computes the processing duration.
  * Get the milliseconds tick value before and after the processing to be measured.
  */
app_status_t app_process(void)
{
  uint32_t tick_start;
  uint32_t tick_stop;
  uint8_t is_prime;
  app_status_t return_status = EXEC_STATUS_ERROR;


  /* Get tick value in milliseconds, which is incremented based on the 1ms SysTick interruption  */
  tick_start = HAL_GetTick();

  /* Arbitrary processing to be measured */
  /* @user: It is possible to change the processing function */
  is_prime = Prime(NUMBER);

  tick_stop = HAL_GetTick();

  /* Compute the processing duration in milliseconds */
  ProcessingDuration = tick_stop - tick_start;

  if (is_prime)
  {
    PRINTF("[INFO] Step 1: %lu is prime --- Processing duration = %" PRIu32 " ms\n", NUMBER, ProcessingDuration);
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Application deinitialization
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief: Check whether a number is prime
  * n: number to be checked
  * ret: 1 number is prime, 0 otherwise.
  */
static uint8_t Prime(uint32_t n)
{
  uint8_t ret = 1;

  for (uint32_t i = 2; i <= n / 2; i++)
  {
    if (n % i == 0)
    {
      /* Not a prime number */
      ret = 0;
      break;
    }
  }

  return ret;
}
