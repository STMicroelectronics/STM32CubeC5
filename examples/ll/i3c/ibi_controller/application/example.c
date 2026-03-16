/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C IBI controller for multi-target example with LL API using interrupt-driven events with LL API.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @user Targets device number for Mx_I3Cx communication.
  */
#define TARGETS_NUMBER              (2U)

/** @user Targets device 1 information for Mx_I3Cx communication:
  * update target information as needed for your hardware
  */
#define DEVICE_TARGET1_NAME         "Device target 1"
#define DEVICE_TARGET1_ID           (0U)
#define DEVICE_TARGET1_STATIC_ADDR  (0U)
#define DEVICE_TARGET1_DYNAMIC_ADDR (0x32U)

/** @user Targets device 2 information for Mx_I3Cx communication:
  * update target information as needed for your hardware
  */
#define DEVICE_TARGET2_NAME         "Device target 2"
#define DEVICE_TARGET2_ID           (1U)
#define DEVICE_TARGET2_STATIC_ADDR  (0U)
#define DEVICE_TARGET2_DYNAMIC_ADDR (0x34U)

/** @user Maximum number of initial attempts for dynamic address assignment.
  */
#define MAX_DAA_ATTEMPTS            (10U)

/** @user Expected payload data value to be received from the target.
  */
#define EXPECTED_PAYLOAD_DATA       (0x00EFCDABU)

/** @user Expected payload data size to be received from the target.
  */
#define EXPECTED_PAYLOAD_DATA_SIZE  (0x03U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when the IBI process is completed */
volatile uint8_t IBIProcessComplete = 0U;
/* Set to 1 when a DAA attempt has failed */
volatile uint8_t DAAFailedAttent = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

/* Number of targets detected and assigned during ENTDAA */
uint32_t TargetCount = 0U;

/* Descriptor table for known targets: the targets BCR/DCR/PID for each target are filled during address assignment */
target_desc_t TargetsDescriptor[TARGETS_NUMBER] =
{
  {DEVICE_TARGET1_NAME, DEVICE_TARGET1_ID, 0U, DEVICE_TARGET1_STATIC_ADDR, DEVICE_TARGET1_DYNAMIC_ADDR},
  {DEVICE_TARGET2_NAME, DEVICE_TARGET2_ID, 0U, DEVICE_TARGET2_STATIC_ADDR, DEVICE_TARGET2_DYNAMIC_ADDR},
};

/* Variable to retrieve IBI information */
uint32_t IbiPayload          = 0U;
uint32_t IbiPayloadCountByte = 0U;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized by the application code.
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
  uint32_t daa_attempts      = 0U; /* Memorizes the number of initial attempts for Dynamic Address Assignment (DAA) */
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Initiates and manages the Dynamic Address Assignment (DAA) process for the controller.
    * Warning: if the target MX_I3Cx peripheral is enabled before the controller starts the first ENTDAA,
    * the first DAA attempt can fail on some setups.
    */
  while (daa_attempts < MAX_DAA_ATTEMPTS)
  {
    DAAProcessComplete = 0U;
    DAAFailedAttent    = 0U;

    daa_attempts++;

    /* Initiate Dynamic Address Assignment (DAA) process for the controller */
    I3C_AssignDynamicAddr_IT();

    /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
    while ((DAAProcessComplete == 0U) && (DAAFailedAttent == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if ((DAAFailedAttent == 1U) && (daa_attempts != 0U))
    {
      /* DAA attempt aborted due to an error; retry. */
      continue;
    }
    else
    {
      break;
    }
  }

  if (DAAProcessComplete == 1U)
  {
    IBIProcessComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */


    /** ########## Step 3 ##########
      * Configure IBI handling for all detected targets and enable IBI notification.
      * Waits for one of these MX_I3Cx interrupts: IBI completion or transfer error.
      */
    /* Configure controller DEVRx entries for each target detected during ENTDAA (enables proper IBI handling). */
    I3C_ConfigureBusDevices(TargetsDescriptor, TargetCount);

    I3C_ActivateNotification();

    /* Waits for one of these MX_I3Cx interrupts: IBI completion or transfer error when USE_LL_APP_ERROR == 1.*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((IBIProcessComplete == 0U) && (TransferError == 0U))
#else
    while (IBIProcessComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1U)
    {
      /* An error occurs during the IBI process. */
      goto _app_process_exit;
    }
#endif /* USE_LL_APP_ERROR */


    /** ########## Step 4 ##########
      * After IBI reception, verify payload length and payload value.
      */
    /* Getting the information from the last IBI request */
    IbiPayloadCountByte = I3C_GetIBIPayloadByteCount();
    IbiPayload          = I3C_GetIBIPayload();

    /* Check number of payload bytes received after IBI. */
    if (IbiPayloadCountByte != EXPECTED_PAYLOAD_DATA_SIZE)
    {
      goto _app_process_exit;
    }

    /* Check IBI payload value received by the controller. */
    if (IbiPayload != EXPECTED_PAYLOAD_DATA)
    {
      goto _app_process_exit;
    }

    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the MX_I3Cx DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
void I3C_DAACompleteCallback(void)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}

/**
  * Target requests dynamic address callback
  * @user This implementation of the MX_I3Cx target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
void I3C_TargetReqDynamicAddrCallback(uint64_t targetPayload)
{
  /* Assign received payload and dynamic address to the target */
  TargetsDescriptor[TargetCount].target_bcr_dcr_pid = targetPayload;
  I3C_SetDynamicAddr(TargetsDescriptor[TargetCount].dynamic_addr);
  TargetCount++;
}

/**
  * MX_I3Cx controller notification callback (interrupt mode).
  * @user: Customize this function to handle controller notifications.
  * This function is executed when a notification event occurs, such as IBI reception.
  */
void I3C_NotificationCallback(void)
{
  /* IBI event received and acknowledged in ISR */
  IBIProcessComplete = 1U;
}

/**
  * MX_I3Cx transfer error callback
  * @user This implementation of the MX_I3Cx controller transfer error callback can be customized.
  * This function is executed when the controller transfer error interrupt is generated.
  */
void I3C_ErrorCallback(void)
{
  /* Asynchronous processing for DAA error */
  DAAFailedAttent = 1U;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Asynchronous processing for transfer/IBI error */
  TransferError = 1U;
#endif /* USE_LL_APP_ERROR */
}
