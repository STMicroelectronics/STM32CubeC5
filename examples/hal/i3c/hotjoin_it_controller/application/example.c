/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller example for multi-target HotJoin using interrupt-driven events.
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
/* Target descriptor structure for ENTDAA multi-target management */
typedef struct
{
  char         *target_name;        /* Human-readable name or reference for the target device */
  uint32_t      target_id;          /* Unique identifier for the target on the I3C bus
                                       (can link to index in device table) */
  uint64_t      target_bcr_dcr_pid; /* Concatenated value of PID, BCR, and DCR for device identification
                                       and ENTDAA verification */
  uint8_t       static_addr;        /* Static address from device datasheet (used before dynamic assignment) */
  uint8_t       dynamic_addr;       /* Dynamic address assigned by controller during ENTDAA */
} target_desc_t;

/* Private define ------------------------------------------------------------*/
/* @user Targets device address for I3C communication. */
#define DEVICE_TARGET1_ADDR      0x32U
#define DEVICE_TARGET2_ADDR      0x34U

/* Array to store ENTDAA payloads (BCR/DCR/PID) received from each target during address assignment */
uint64_t TargetPayload[10];

/* Number of targets detected and assigned during ENTDAA */
uint32_t TargetCount = 0U;

/* @user Descriptor table for known targets: update STATIC_ADDR and DYNAMIC_ADDR as needed for your hardware */
target_desc_t TargetsDescriptor[2] =
{
  {"", 0U, 0, 0, DEVICE_TARGET1_ADDR},
  {"", 1U, 0, 0, DEVICE_TARGET2_ADDR},
};

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define TARGET_NUMBERS    2U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C; /* pointer referencing the I3C handle from the generated code */

/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t DAAError = 0U;
/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when the HotJoin Request arrives at the Controller */
volatile uint8_t HotJoinRequest  = 0U;

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlDAACpltCallback(hal_i3c_handle_t *pI3C);
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload);
static void DAAErrorCallback(hal_i3c_handle_t *pI3C);
static void HotJoinReqCallback(hal_i3c_handle_t *pI3C, uint32_t notify_id);
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

  /* Activate HotJoin notification in controller side */
  if (HAL_I3C_CTRL_ActivateNotification(pI3C, HAL_I3C_CTRL_NOTIFICATION_HJ) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register the I3C Controller callbacks */
  if (HAL_I3C_RegisterErrorCallback(pI3C, DAAErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterDAACpltCallback(pI3C, CtrlDAACpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterTgtReqDynAddrCallback(pI3C, CtrlTgtReqDynAddrCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterNotifyCallback(pI3C, HotJoinReqCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED with a HotJoin mechanism ACTIVATED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t i3c_error_code = 0U;  /* Memorizes the I3C error code limited to the last process */

  /** ########## Step 2 ##########
    * The Controller waits for a HotJoin request from a new target.
    * A new target "hot-joined" the communication via IBI signal to alert the main controller of its presence.
    * Upon receiving the request, the controller initiates DAA process to assign a unique address.
    */

  /* Waits the HotJoin request from the Target side */
  while ((HotJoinRequest == 0) && (DAAError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  PRINTF("[INFO] Step 2: HotJoin request RECEIVED.\n");
  /* Reset HotJoinRequest before starting a new attempt */
  HotJoinRequest = 0U;

  /* Reset DAA before starting a new DAA attempt */
  DAAProcessComplete = 0U;

  /** ########## Step 3 ##########
    * Initiates Dynamic Address Assignment (DAA) process for the controller.
    */
  if (HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA) != HAL_OK)
  {
    /* Error occurred during DAA process initiation. Handle and retry. */
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Waits the completion of the DAA process or a DAA error interrupt.
    */

  while ((DAAProcessComplete == 0) && (DAAError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (DAAError == 1U)
  {
    /** DAA process not completed as expected.
      * Report the error and restart the communication attempt.
      */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    HandleTransferError(i3c_error_code);
    /* Reset the DAA error flag */
    DAAError = 0U;
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: DAA process COOMPLETED.");

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
  PRINTF("[ERROR] Step 3: Error occurs DAA process is = %" PRIu32, error_codes);
} /* end HandleTransferError */


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the I3C DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
static void CtrlDAACpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}


/**
  * Target requests dynamic address callback.
  * @user This implementation of the I3C target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload)
{
  /* Assign received payload and dynamic address to the target */
  TargetsDescriptor[TargetCount].target_bcr_dcr_pid = targetPayload;
  HAL_I3C_CTRL_SetDynAddr(hi3c, TargetsDescriptor[TargetCount].dynamic_addr);
  TargetCount++;
}


/**
  * I3C HotJoin request notification callback.
  * @user This implementation of the I3C HotJoin request notification callback can be customized.
  * This function is executed when an HotJoin request arrived in the controller.
  */
static void HotJoinReqCallback(hal_i3c_handle_t *pI3C, uint32_t notify_id)
{
  if ((notify_id & HAL_I3C_CTRL_NOTIFICATION_HJ) == HAL_I3C_CTRL_NOTIFICATION_HJ)
  {
    /* Asynchronous processing related to HotJoin request notification */
    HotJoinRequest = 1U;
  }
} /* end HotJoinReqCallback */


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
