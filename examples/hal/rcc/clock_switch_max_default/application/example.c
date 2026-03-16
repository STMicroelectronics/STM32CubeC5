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

/** @user: MCO divider,
  * you might have to tune it to output a frequency in coherence with your measurement tool.
  * (You also need to update MCO_DIVIDER_INT, only used for printf purpose)
  */
#define MCO_DIVIDER_INT 10U

/* Private macro -------------------------------------------------------------*/
/* @user: configure the delay in milliseconds between intermediate steps */
#define EXAMPLE_STEP_DELAY_MS 1000U

/* Private variables ---------------------------------------------------------*/

/* variables to store the current Default & Max SYSCLK frequency */
uint32_t DefaultSysclkFrequency = 0;
uint32_t MaxSysclkFrequency = 0;

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * In RCC examples, the init has already been triggered by mx_system_init() from main.
  * Generated SYSCLK signal is routed to the MCO pin.
  */
app_status_t app_init(void)
{
  MaxSysclkFrequency = HAL_RCC_GetSYSCLKFreq();
  PRINTF("[INFO] Step 1: Output SYSCLK (%" PRId32 "kHz) to MCO pin, with a %u divider" \
         " (MCO freq: %" PRId32 "kHz). \n",
         MaxSysclkFrequency / 1000,
         MCO_DIVIDER_INT,
         MaxSysclkFrequency / (1000 * MCO_DIVIDER_INT));

  return EXEC_STATUS_INIT_OK;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Switch the clock source from Max to Default during 1s.
    */

  HAL_RCC_Reset();
  /* Previous RCC API reset previously clock configuration for all IPs - Need to re-configure it */
  if (mx_rcc_peripherals_clock_config() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  DefaultSysclkFrequency = HAL_RCC_GetSYSCLKFreq();
  PRINTF("[INFO] Step 2: Default settings are used to generate SYSCLK at %" PRId32 "kHz, (MCO freq: %" PRId32 "kHz).\n",
         DefaultSysclkFrequency / 1000,
         DefaultSysclkFrequency / (1000 * MCO_DIVIDER_INT));
  HAL_Delay(EXAMPLE_STEP_DELAY_MS);

  /** ########## Step 3 ##########
    * Switch the clock source from Default to Max during 1s.
    */

  if (mx_example_rcc_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }
  MaxSysclkFrequency = HAL_RCC_GetSYSCLKFreq();
  PRINTF("[INFO] Step 3: Max settings are used to generate SYSCLK at %" PRId32 "kHz, (MCO freq: %" PRId32 "kHz).\n",
         MaxSysclkFrequency / 1000,
         MaxSysclkFrequency / (1000 * MCO_DIVIDER_INT));


  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/* RCC peripheral cannot be de-initialized */
app_status_t app_deinit(void)
{

  return EXEC_STATUS_OK;
} /* end app_deinit */
