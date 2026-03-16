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
/* @user: You can set the number of times that the IWDG will be refreshed before generating the reset */
#define PROCESS_LOOP_NUMBER   3
#define IWDG_MIN_TIME_VALUE   0
#define IWDG_MAX_TIME_VALUE   1000
#define IWDG_EWU_TIME_VALUE   0     /* Early wakeup feature not used */

#define IWDG_VALID_REFRESH    (IWDG_MAX_TIME_VALUE - (IWDG_MAX_TIME_VALUE / 10))
#define IWDG_INVALID_REFRESH  (IWDG_MAX_TIME_VALUE + (IWDG_MAX_TIME_VALUE / 10))

/* Private variables ---------------------------------------------------------*/

hal_iwdg_handle_t *pIWDG;  /* pointer referencing the IWDG handle from the generated code */

uint8_t IWDG_RestartFromIWDGReset = 0;  /* Flag to know if the device restart due to IWDG reset */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the IWDG instance is triggered by the applicative code.
  * If the device is restarted due to a IWDG reset, the IWDG initialization is bypassed.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if ((HAL_RCC_GetResetSource() & HAL_RCC_RESET_FLAG_IWDG) == HAL_RCC_RESET_FLAG_IWDG)
  {
    HAL_RCC_ClearResetFlags();
    IWDG_RestartFromIWDGReset = 1;
    return_status = EXEC_STATUS_INIT_OK;
    goto _app_init_exit;
  }

  /** Initialization of the IWDG.
    * The IWDG is initialized with the parameters that can be configured by the user through the cube interface.
    */
  pIWDG = mx_example_iwdg_init();
  if (pIWDG == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_IWDG_Start(pIWDG, IWDG_MIN_TIME_VALUE, IWDG_MAX_TIME_VALUE, IWDG_EWU_TIME_VALUE) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** Checking if the device restarted due to the IWDG. If not, the IWDG reset is triggered on purpose.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (IWDG_RestartFromIWDGReset == 0U)
  {
    /* Watchdog has not fired, so the next steps demonstrate how IWDG triggers reset */

    /** ########## Step 2 ##########
      * The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times and to prevent watchdog reset.
      */
    for (uint8_t i = 0; i < PROCESS_LOOP_NUMBER; i++)
    {
      HAL_Delay(IWDG_VALID_REFRESH); /* IWDG refresh delay */

      if (HAL_IWDG_Refresh(pIWDG) != HAL_OK)
      {
        goto _app_process_exit;
      }

      PRINTF("[INFO] Step 2: IWDG refreshed.\n");
    }

    /** ########## Step 3 ##########
      * Do not refresh the IWDG to trigger a reset.
      */
    PRINTF("[INFO] Step 3: IWDG not refreshed on purpose.\n");

    HAL_Delay(IWDG_INVALID_REFRESH);
    PRINTF("[ERROR] The IWDG is supposed to reset before reaching this stage due to the Step 3.\n");
  }
  else
  {
    /* Watchdog has already fired, that means the previous steps (Step1,2,3) have already been executed */

    /** ########## Step 4 ##########
      * The device is restarted due to IWDG
      */
    PRINTF("[INFO] Step 4: Device's restart completed due to IWDG.\n");
    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** Nothing to do because the IWDG IP cannot be de-initialized.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
