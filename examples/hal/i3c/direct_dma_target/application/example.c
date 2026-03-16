/**
  ******************************************************************************
  * file    example.c
  * brief   I3C target application handling direct data transfers
  *          with the HAL API, using interrupts.
  *          Demonstrates efficient, responsive communication with an I3C controller.
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
#define TARGET_CHECKED_DYNAMIC_ADDRESS   0x32U   /* Dynamic address expected from the controller */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* pointer referencing the I3C handle from the generated code */
hal_i3c_handle_t *pI3C;
/* Holds Common Command Code (CCC) information, used to verify dynamic address assignment after DAA. */
hal_i3c_ccc_info_t CCCInfo;

volatile uint8_t DAAProcessComplete    = 0U; /* Set to 1 when DAA process is completed */
volatile uint8_t SETMRLProcessComplete = 0U; /* Set to 1 when a SETMRL event is process is completed */
volatile uint8_t SETMWLProcessComplete = 0U; /* Set to 1 when a SETMWL event is process is completed */

/* Tracks if DAA process completed for this power cycle */
uint8_t DaaCompletionDone = 0U;

/* Set maximum read length expected */
uint32_t MaximumReadLength = 0x04;
/* Set maximum write length expected */
uint32_t MaximumWriteLength = 0x04;
/* Set to 1 when a transmission or a reception error is detected */
volatile uint8_t TransferError = 0U;

/* Private function prototype -----------------------------------------------*/
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code);

/* Function allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId);


/** ########## Step 1 ##########
  * The initialization of the I3C instance is triggered by the application code.
  * The user callbacks for I3C target TX/RX transfer completed and transfer error are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the I3C target callbacks */

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Activate target notifications for DAA (Dynamic Address Assignment), SETMRL, and SETMWL */
  if (HAL_I3C_TGT_ActivateNotification(pI3C, \
                                       (uint8_t *)NULL, \
                                       0U, \
                                       HAL_I3C_TGT_NOTIFICATION_DAU \
                                       | HAL_I3C_TGT_NOTIFICATION_SETMRL \
                                       | HAL_I3C_TGT_NOTIFICATION_SETMWL) != HAL_OK)
  {
    goto _app_init_exit;
  }
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
    * Perform DAA (Dynamic Address Assignment) process and verify dynamic address assignment.
    * This step activates the notification, waits for DAA completion, and checks the assigned address.
    */
  if (!DaaCompletionDone)
  {
    /* Wait for DAA process to complete (set by NotifyCallback) */
    while (DAAProcessComplete == 0U)
    {

      /* Put the CPU in Wait For Interrupt state. An I3C or SysTick interrupt can wake up the CPU.
         This illustrates the use of IT mode. Replace with your own code if needed. */
      __WFI();
    }

    /* Check if DAA process completed successfully */
    if (DAAProcessComplete != 1U)
    {
      /* DAA process did not complete correctly. Handle error and retry. */
      goto _app_process_exit;
    }

    /* Retrieve CCC (Common Command Code) info to check dynamic address assignment */
    hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
    if (hal_status != HAL_OK)
    {
      /* Error occurred while retrieving CCC info. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /* Check if the assigned dynamic address matches the expected value */
    if (CCCInfo.dynamic_addr != TARGET_CHECKED_DYNAMIC_ADDRESS)
    {
      /* Dynamic address does not match. Handle error and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    DaaCompletionDone = 1U;
    PRINTF("[INFO] Step 2: Dynamic address verification COMPLETED.\n");
  }

  /* Wait for SETMRL and SETMWL notifications */
  while ((SETMRLProcessComplete == 0U) && (SETMWLProcessComplete == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }
  /**
    * ########## Step 3 ##########
    * Verify that SETMWL and SETMRL events have been received and processed.
    */
  if (SETMRLProcessComplete == 1U)
  {
    hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_SETMRL, &CCCInfo);
    if (hal_status != HAL_OK)
    {
      /* Error occurred while retrieving CCC info. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /* Compare data updated with expected data */
    if (CCCInfo.max_read_data_size_byte != MaximumReadLength)
    {
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }
    SETMRLProcessComplete = 0U;
    PRINTF("[INFO] - SETMRL processing and verification COMPLETED.\n");
  }

  if (SETMWLProcessComplete == 1U)
  {
    hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_SETMWL, &CCCInfo);
    if (hal_status != HAL_OK)
    {
      /* Error occurred while retrieving CCC info. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /* Compare data updated with expected data */
    if (CCCInfo.max_write_data_size_byte != MaximumWriteLength)
    {
      /* Error_Handler() function is called when error occurs. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
    }
    SETMWLProcessComplete = 0U;
    PRINTF("[INFO] - SETMWL processing and verification COMPLETED.\n");
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * I3C target notification callback (interrupt mode).
  * @user This implementation of the I3C target notification callback can be customized.
  * This function is executed when a notification event occurs, such as DAA process completion (step 2).
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId)
{
  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DAAProcessComplete = 1U;
  }
  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_SETMRL) == HAL_I3C_TGT_NOTIFICATION_SETMRL)
  {
    SETMRLProcessComplete = 1U;
  }

  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_SETMWL) == HAL_I3C_TGT_NOTIFICATION_SETMWL)
  {
    SETMWLProcessComplete = 1U;
  }
}


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I3C operations.
  * param i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Target - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
