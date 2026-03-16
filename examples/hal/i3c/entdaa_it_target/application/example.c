/**
  ******************************************************************************
  * file    example.c
   * brief   I3C target example for ENTDAA: Waits for Dynamic Address Assignment (DAA) from Controller,
   *          validates assigned dynamic address, sends payload, and exits if communication is lost.
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/**
  *  @user: Set this value to the expected dynamic address assigned to each target by the controller.
  *  Update it per target board.
  */
#define TARGET_CHECKED_DYNAMIC_ADDRESS   0x32U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;     /* Pointer referencing the I3C handle from the generated code */
hal_i3c_ccc_info_t CCCInfo; /* Holds Common Command Code (CCC) information, used to verify dynamic address assignment */


volatile uint8_t DAAProcessComplete = 0U; /* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t DAAError =  0U;           /* Set to 1 when a transmission or reception error is detected */

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleDAAError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id);
static void DAAErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * The I3C peripheral is initialized by the application code.
  * User callbacks for I3C controller events are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the I3C Controller callbacks */

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, DAAErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* If all checks pass, initialization is successful */
  return_status = EXEC_STATUS_INIT_OK;

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */


  /**
    * ########## Step 2 ##########
    * Perform Dynamic Address Assignment (DAA) process: activate notification
    * Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur.
    */

  /* Activate notification for DAA process (Dynamic Address Assignment) */
  hal_status = HAL_I3C_TGT_ActivateNotification(pI3C, (uint8_t *)NULL, 0U, HAL_I3C_TGT_NOTIFICATION_DAU);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while activating notification. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleDAAError(hal_status, i3c_error_code);
    goto _app_process_exit;

  }

  /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
  while ((DAAProcessComplete == 0U) && (DAAError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (DAAError == 1U)
  {
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleDAAError(hal_status, i3c_error_code);
  }

  PRINTF("[INFO] Step 2: DAA process COMPLETED.\n");

  /**
    * ########## Step 3 ##########
    * Retrieve CCC info and validate the assigned dynamic address and its validity after DAA completion.
    */

  /* Retrieve CCC (Common Command Code) info to check dynamic address assignment */
  hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while retrieving CCC info. Handle and retry. */
    goto _app_process_exit;

  }

  if (CCCInfo.dynamic_addr_valid != 1)
  {
    PRINTF("[ERROR] Dynamic address assignment is not valid.\n");
    goto _app_process_exit;
  }

  /* Check if the assigned dynamic address matches the expected value */
  if (CCCInfo.dynamic_addr != TARGET_CHECKED_DYNAMIC_ADDRESS)
  {
    PRINTF("[ERROR] Dynamic address does not match expected value.\n");
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

  PRINTF("[INFO] Step 3: Dynamic address received: %" PRIu32 "\n", (uint32_t)CCCInfo.dynamic_addr);

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * I3C target notification callback (interrupt mode).
  * @user This implementation of the I3C target notification callback can be customized.
  * This function is executed when a notification event occurs, such as DAA process completion (step 2).
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id)
{
  if ((notify_id & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DAAProcessComplete = 1U;
  }
}

/**
  * I3C ENTDAA error callback (interrupt mode).
  * @user This implementation of the I3C ENTDAA error callback can be customized.
  * This function is executed when an error interrupt occurs during the Dynamic Address Assignment (ENTDAA) procedure.
  */
static void DAAErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal ENTDAA error during Dynamic Address Assignment procedure */
  DAAError = 1U;
}


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  This function is executed in case of an ENTDAA (Dynamic Address Assignment) error.
  * param hal_status:  HAL status of the I3C operation during ENTDAA.
  * param i3c_error_code:  I3C error code during ENTDAA.
  * retval: example status
  */
static app_status_t HandleDAAError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Target - ENTDAA ERROR: hal_status = %" PRIu8 ", HAL_I3C_GetLastErrorCodes = %" PRIu32
         "TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleDAAError */
