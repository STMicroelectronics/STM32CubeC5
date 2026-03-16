/**
  ******************************************************************************
  * file    example.c
  * brief   I3C target example for HotJoin mechanism: A target send a HotJoin
  *         request to the controller then waits for DAA complete.
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
/** Dynamic address value expected from the controller.
  * @user: you need to change this value to use several target (for example: 0x34).
  */
#define TARGET_CHECKED_DYNAMIC_ADDRESS   0x32U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;     /* pointer referencing the I3C handle from the generated code */
hal_i3c_ccc_info_t CCCInfo; /* Common Command Code (CCC) information, used to verify dynamic address assignment */

volatile uint8_t HotJoinError = 0U;   /* Set to 1 when an error detected during Hotjoin mechanism      */
volatile uint8_t HotJoinReqCplt = 0U; /* Set to 1 when the HotJoin process is completed                */
volatile uint8_t ReceivedAddress = 0U;     /* The returned dynamic address value after the HotJoin process .*/
/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void ErrorCallback(hal_i3c_handle_t *pI3C);
static void HotJoinCpltCallback(hal_i3c_handle_t *hi3c, uint8_t dynamic_address);
static void HandleTransferError(uint32_t error_codes);

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

  /* Register the I3C Target callbacks */
  if (HAL_I3C_RegisterErrorCallback(pI3C, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_TGT_RegisterHotJoinCallback(pI3C, HotJoinCpltCallback) != HAL_OK)
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
  uint32_t i3c_error_code;

  /**
    * ########## Step 2 ##########
    * Send a HotJoin request to the controller within interrupt mode.
    * Wait for the Hotjoin process to complete or for an error to occur.
  */
  if (HAL_I3C_TGT_HotJoinReq_IT(pI3C) != HAL_OK)
  {
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    HandleTransferError(i3c_error_code);
    goto _app_process_exit;
  }

  /* Wait for the HotJoin process to complete or for an error to occur */
  while ((HotJoinReqCplt == 0U) && (HotJoinError  == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  /** Check if an error occurs during the HotJoin ack mechanism.
    * Retrieve the root cause of this error by reading the last error codes stored in the handle.
    * Exit from here.
  */
  if (HotJoinError  == 1U)
  {
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    HandleTransferError(i3c_error_code);
    goto _app_process_exit;
  }

  /* Check if the assigned dynamic address matches the expected value */
  if (ReceivedAddress != TARGET_CHECKED_DYNAMIC_ADDRESS)
  {
    /* Dynamic address does not match. */
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: HotJoin process COMPLETED.\n");

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** brief:  Handles an I3C transfer error (DAA/ENTDAA, IBI, or configuration).
  * param hal_status:  HAL status returned by the failed I3C operation.
  * param error_codes:  I3C Error Code..
  * retval: example status
  */
static void HandleTransferError(uint32_t error_codes)
{
  PRINTF("[ERROR] Step 2: Error occurs during HotJoin process: %" PRIu32 "\n", error_codes);
} /* end HandleTransferError */


/**
  * I3C error callback (interrupt mode).
  * @user This implementation of the I3C error callback can be customized.
  * This function is executed when an error interrupt occurs during the HotJoin procedure.
  */
static void ErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal error during HotJoin procedure */
  HotJoinError = 1U;
} /* end ErrorCallback */


/**
  * I3C HotJoin Cplt callback (interrupt mode).
  * @user This implementation of the I3C HotJoin Cplt callback can be customized.
  * This function is executed when a HotJoin notification occurs.
  */
static void HotJoinCpltCallback(hal_i3c_handle_t *hi3c, uint8_t dynamic_address)
{
  ReceivedAddress = dynamic_address;

  HotJoinReqCplt = 1U;
} /* end HotJoinCpltCallback */


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
