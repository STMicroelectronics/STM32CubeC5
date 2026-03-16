/**
  ******************************************************************************
  * file    example.c
   * brief   Demonstrates IBI (In-Band Interrupt) wakeup using interrupt (IT) mode.
   *         Sends a 3-byte IBI payload to the controller.
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

#define TARGET_CHECKED_DYNAMIC_ADDRESS   0x32U   /* Dynamic address expected from the controller */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_i3c_handle_t *pI3C;   /* Pointer referencing the I3C handle from the generated code */

hal_i3c_ccc_info_t CCCInfo; /* Holds Common Command Code (CCC) information, used to verify dynamic address assignment */

/* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when the IBI (In-Band Interrupt) process is completed */
volatile uint8_t IBIProcessComplete = 0U;
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;

/** IBI payload sent by the target (default length: 3 bytes).
  * @user: it possible to modify the payload content and length as needed.
  */
uint8_t IBIPayloadData[3] = {0xAB, 0xCD, 0xEF};

/* Private functions prototype -----------------------------------------------*/
static app_status_t PerformDAAProcess(void);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * The I3C peripheral is initialized by the application code.
  * User callbacks for I3C Target events are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the I3C Target callbacks */

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
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
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */

  /**
    * ########## Step 2 ##########
    * Activates notification, waits for DAA completion, and verifies assigned address.
    * All DAA logic is encapsulated in the PerformDAAProcess() function.
    */

  return_status = PerformDAAProcess();

  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: DAA process COMPLETED.\n");


  /**
    * ########## Step 3 ##########
    * Send IBI (In-Band Interrupt) request to controller and wait for completion.
    */

  /* Send IBI request with payload (3 bytes) to controller. */
  hal_status = HAL_I3C_TGT_IBIReq_IT(pI3C, IBIPayloadData, 3U);
  if (hal_status != HAL_OK)
  {
    /* Error during IBI request. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;

  }

  /* Wait for IBI process to complete or for an error to occur. */
  while ((IBIProcessComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1U)
  {
    /* Handle transfer error after IBI process. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }
  else
  {
    return_status = EXEC_STATUS_OK;

    PRINTF("[INFO] Step 3: IBI process COMPLETED.\n");
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** brief:  Handles the Dynamic Address Assignment (DAA) process for the I3C target.
  *         Activates notification, waits for DAA completion, and verifies the assigned address.
  * param:  none
  * retval: EXEC_STATUS_OK if DAA completes successfully, error status otherwise.
  */
static app_status_t PerformDAAProcess(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;
  uint32_t i3c_error_code;

  /* Activate notification for DAA process (Dynamic Address Assignment) */
  hal_status = HAL_I3C_TGT_ActivateNotification(pI3C, (uint8_t *)NULL, 0U, HAL_I3C_TGT_NOTIFICATION_DAU);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while activating notification. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;

  }

  /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
  while ((DAAProcessComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1U)
  {
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Retrieve CCC (Common Command Code) info to check dynamic address assignment */
  hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while retrieving CCC info. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
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

_app_process_exit:
  return return_status;
}


/**
  * I3C target notification callback (interrupt mode).
  * @user This implementation of the I3C target notification callback can be customized.
  * This function is executed when a notification event occurs, e.g. DAA or IBI completion.
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id)
{
  if ((notify_id & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DAAProcessComplete = 1U;
  }

  if ((notify_id & HAL_I3C_TGT_NOTIFICATION_IBIEND) == HAL_I3C_TGT_NOTIFICATION_IBIEND)
  {
    IBIProcessComplete = 1U;
  }

}


/**
  * I3C target transfer error callback (interrupt mode).
  * @user This implementation of the I3C target transfer error callback can be customized.
  * This function is executed when a transfer error interrupt occurs.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  TransferError = 1U;
}


/** De-initializes the I3C and DMA instances before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Handles an I3C transfer error (DAA/ENTDAA, IBI, or configuration).
  * param hal_status:  HAL status returned by the failed I3C operation.
  * param i3c_error_code:  I3C Error Code..
  * retval: example status
  */
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Target - Transfer ERROR: hal_status = %" PRIu8 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
