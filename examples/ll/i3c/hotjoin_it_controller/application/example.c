/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller example for multi-target HotJoin using interrupt-driven events with LL API.
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
/* @user Targets device address for Mx_I3Cx communication. */
/* @user Maximum number of initial attempts for dynamic address assignment. */
#define TARGETS_NUMBER 2U

/* @user Targets device 1 information for Mx_I3Cx communication:
         update target information as needed for your hardware */
#define DEVICE_TARGET1_NAME         "Device target 1"
#define DEVICE_TARGET1_ID           (0U)
#define DEVICE_TARGET1_STATIC_ADDR  (0U)
#define DEVICE_TARGET1_DYNAMIC_ADDR (0x32U)

/* @user Targets device 2 information for Mx_I3Cx communication:
         update target information as needed for your hardware */
#define DEVICE_TARGET2_NAME         "Device target 2"
#define DEVICE_TARGET2_ID           (1U)
#define DEVICE_TARGET2_STATIC_ADDR  (0U)
#define DEVICE_TARGET2_DYNAMIC_ADDR (0x34U)

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t DAAError = 0U; /* Set to 1 when a transmission or reception error is detected */
#endif /* USE_LL_APP_ERROR */

/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
volatile uint8_t HotJoinRequest  = 0U; /* Set to 1 when the HotJoin Request arrives at the Controller */

/* Number of targets detected and assigned during ENTDAA */
uint32_t TargetCount = 0U;

/* Descriptor table for known targets: the targets BCR/DCR/PID for each target are filled during address assignment */
target_desc_t TargetsDescriptor[TARGETS_NUMBER] =
{
  {DEVICE_TARGET1_NAME, DEVICE_TARGET1_ID, 0U, DEVICE_TARGET1_STATIC_ADDR, DEVICE_TARGET1_DYNAMIC_ADDR},
  {DEVICE_TARGET2_NAME, DEVICE_TARGET2_ID, 0U, DEVICE_TARGET2_STATIC_ADDR, DEVICE_TARGET2_DYNAMIC_ADDR},
};

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

  /* Activate HotJoin notification in controller side */
  I3C_ActivateNotification();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * The Controller waits for a HotJoin request from a new target.
    * A new target "hot-joined" the communication via IBI signal to alert the main controller of its presence.
    * Upon receiving the request, the controller initiates DAA process to assign a unique address.
    */

  /* Waits the HotJoin request from the Target side or a DAA error interrupt when USE_LL_APP_ERROR == 1 */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((HotJoinRequest == 0U) && (DAAError == 0U))
#else
  while (HotJoinRequest == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (DAAError == 1U)
  {
    /** The HotJoin request is not correctly completed.
      * Report the error, then restart the communication.
      */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  /* Reset HotJoinRequest and DAA flags before starting a new attempt */
  HotJoinRequest     = 0U;
  DAAProcessComplete = 0U;


  /** ########## Step 3 ##########
    * Initiates Dynamic Address Assignment (DAA) process for the controller.
    */
  I3C_AssignDynamicAddr_IT();


  /** ########## Step 4 ##########
    * Waits the completion of the DAA process or a DAA error interrupt when USE_LL_APP_ERROR == 1.
    */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((DAAProcessComplete == 0U) && (DAAError == 0U))
#else
  while (DAAProcessComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (DAAError == 1U)
  {
    /** The DAA process is not correctly completed.
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


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the Mx_I3Cx DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
void I3C_DAACpltCallback(void)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
} /* end I3C_DAACpltCallback */


/**
  * Target requests dynamic address callback.
  * @user This implementation of the Mx_I3Cx target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
void I3C_TargetReqDynamicAddrCallback(uint64_t targetPayload)
{
  /* Assign received payload and dynamic address to the target */
  TargetsDescriptor[TargetCount].target_bcr_dcr_pid = targetPayload;
  I3C_SetDynamicAddr(TargetsDescriptor[TargetCount].dynamic_addr);
  TargetCount++;
} /* end I3C_TargetReqDynamicAddrCallback */


/**
  * Mx_I3Cx HotJoin request notification callback.
  * @user This implementation of the Mx_I3Cx HotJoin request notification callback can be customized.
  * This function is executed when an HotJoin request arrived in the controller.
  */
void I3C_HotJoinReqCallback(void)
{
  /* Asynchronous processing related to HotJoin request notification */
  HotJoinRequest = 1U;
} /* end I3C_HotJoinReqCallback */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * Mx_I3Cx ENTDAA error callback (interrupt mode).
  * @user This implementation of the Mx_I3Cx ENTDAA error callback can be customized.
  * This function is executed when an error interrupt occurs during the Dynamic Address Assignment (ENTDAA) procedure.
  */
void I3C_DAAErrorCallback(void)
{
  /* Signal ENTDAA error during Dynamic Address Assignment procedure */
  DAAError = 1U;
} /* end I3C_DAAErrorCallback */


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
