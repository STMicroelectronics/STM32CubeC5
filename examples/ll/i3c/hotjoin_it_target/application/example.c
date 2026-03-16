/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C HotJoin target interrupt example with LL API.
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

/* Private define ------------------------------------------------------------*/
/** Dynamic address value expected from the controller.
  * @user: you need to change this value to use several target (for example: 0x34).
  */
#define DEVICE_TARGET_DYNAMIC_ADDR (0x32U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t HotJoinReqComplete = 0U;     /* Set to 1 when the HotJoin process is completed                */
volatile uint8_t ReceivedDynamicAddress = 0U; /* The returned dynamic address value after the HotJoin process .*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t HotJoinError = 0U;           /* Set to 1 when an error detected during Hotjoin mechanism      */
#endif /* USE_LL_APP_ERROR */

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */


/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized and activated by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /**
    * ########## Step 2 ##########
    * Send a HotJoin request to the controller in interrupt mode.
    */
  I3C_SendHotJoinReq_IT();

  /* Wait for the HotJoin process to complete or for an error to occur when USE_LL_APP_ERROR == 1 */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((HotJoinReqComplete == 0U) && (HotJoinError  == 0U))
#else
  while (HotJoinReqComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (HotJoinError  == 1U)
  {
    /** The HotJoin request is not correctly completed.
      * Report the error, then restart the communication.
      */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }

  /* Check if the assigned dynamic address matches the expected value */
  if (ReceivedDynamicAddress != DEVICE_TARGET_DYNAMIC_ADDR)
  {
    /** Dynamic address does not match. It can be matched for other connected targets
      * Report the error, then restart the communication.
      */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  return_status = EXEC_STATUS_OK;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
_app_process_exit:
#endif /* USE_LL_APP_ERROR */
  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_I3Cx HotJoin Cplt callback (interrupt mode).
  * @user This implementation of the MX_I3Cx HotJoin Cplt callback can be customized.
  * This function is executed when a HotJoin notification occurs.
  */
void I3C_HotJoinCpltCallback(uint8_t dynamic_address)
{
  ReceivedDynamicAddress = dynamic_address;

  HotJoinReqComplete = 1U;
} /* end I3C_HotJoinCpltCallback */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * MX_I3Cx error callback (interrupt mode).
  * @user This implementation of the MX_I3Cx error callback can be customized.
  * This function is executed when an error interrupt occurs during the HotJoin procedure.
  */
void I3C_ErrorCallback(void)
{
  /* Signal error during HotJoin procedure */
  HotJoinError = 1U;
} /* end I3C_ErrorCallback */


/** brief:  This function is executed in case of a data transfer error.
  * @user:  - This function implementation only illustrates error processing.
  *         - It can be customized to match the application recovery strategy.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
