/**
  ******************************************************************************
  * file           : example.c
  * brief          : WWDG refresh and trigger a reset
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
/* @user: You can set the number of times that the WWDG will be refreshed before generating the reset */
#define PROCESS_LOOP_NUMBER 3

/* Delay to prevent PRINTF overlap due to the delay needed by the WWDG to generate the window reset */
#define WWDG_AVOID_OVERLAP_MS 50

#define WWDG_MIN_TIME_VALUE   78
#define WWDG_MAX_TIME_VALUE   209
#define WWDG_EWU_TIME_VALUE   0     /* Early wakeup feature not used */

/* Delay corresponding to the mean of the WWDG timeout and window delays */
#define WWDG_VALID_REFRESH    ((2 * WWDG_MAX_TIME_VALUE - WWDG_MIN_TIME_VALUE) / 2)

/* Private variables ---------------------------------------------------------*/

hal_wwdg_handle_t *pWWDG;  /* pointer referencing the WWDG handle from the generated code */

uint8_t WWDG_RestartFromWWDGReset = 0;  /* Flag to know if the device restart due to WWDG reset */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the WWDG instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** If the device is restarted due to a WWDG reset, the WWDG initialization is bypassed.
    */
  if ((HAL_RCC_GetResetSource() & HAL_RCC_RESET_FLAG_WWDG) != 0U)
  {
    HAL_RCC_ClearResetFlags();
    WWDG_RestartFromWWDGReset = 1;
    return_status = EXEC_STATUS_INIT_OK;
    goto _app_init_exit;
  }

  /** Initialization of the WWDG.
    * The WWDG is initialized with the parameters that can be configured by the user through the cube interface.
    */
  pWWDG = mx_example_wwdg_init();
  if (pWWDG == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_WWDG_Start(pWWDG, WWDG_MIN_TIME_VALUE, WWDG_MAX_TIME_VALUE, WWDG_EWU_TIME_VALUE) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** Checking if the device restarted due to the WWDG. If not, the WWDG reset is triggered on purpose.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (WWDG_RestartFromWWDGReset == 0U)
  {
    /* The watchdog has not fired, so the next steps demonstrate how to reset the WWDG trigger. */

    /** ########## Step 2 ##########
      * The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times and to prevent watchdog reset.
      */
    for (uint8_t i = 0; i < PROCESS_LOOP_NUMBER; i++)
    {
      HAL_Delay(WWDG_VALID_REFRESH); /* WWDG refresh delay */

      if (HAL_WWDG_Refresh(pWWDG) != HAL_OK)
      {
        goto _app_process_exit;
      }
      PRINTF("[INFO] Step 2: WWDG refreshed.\n");
    }

    /** ########## Step 3 ##########
      * The WWDG is refreshed outside the window to trigger the WWDG reset.
      */
    PRINTF("[INFO] Step 3: WWDG refreshed outside the window.\n");

    if (HAL_WWDG_Refresh(pWWDG) != HAL_OK)
    {
      goto _app_process_exit;
    }
    /** When the WWDG is refreshed outside the window, the WWDG is triggered but it takes one period of the WWDG
      * prescaled clock to generate the interruption and the restart. During these CPU cycles, the program is running
      * and so, the first characters of the following error message are displayed. The purpose of the following delay
      * is to prevent this unwanted display.
      */
    HAL_Delay(WWDG_AVOID_OVERLAP_MS);
    PRINTF("[ERROR] The WWDG is supposed to reset before reaching this stage due to the Step 3.\n");
  }
  else
  {
    /* Watchdog has already fired, that means the previous steps (Step1,2,3) have already been executed */

    /** ########## Step 4 ##########
      * The device is restarted due to WWDG
      */
    PRINTF("[INFO] Step 4: Device's restart completed due to WWDG.\n");
    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** Nothing to do because the WWDG IP cannot be de-initialized.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
