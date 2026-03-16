/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Target device switch to Controller role using IT
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
#define TARGET_DYNAMIC_ADDRESS   0x32U   /* Dynamic address expected */
#define VALID_ADDRESS            1U      /* Dynamic address valid */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;      /* Pointer to I3C handle from the generated code */
hal_i3c_ccc_info_t CCCInfo;  /* Common Command Code (CCC) information */

volatile uint32_t DynamicAddressAssignmentReceived = 0U;  /* DAA process received */
volatile uint32_t ControllerRoleRequestCompleted = 0U;    /* CRR process completed */
volatile uint32_t TransferError = 0U;          /* Transfer error detected */

uint8_t DAACompleted = 0U;  /* DAA process completed for this power cycle */

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(void);

/* Functions allowing the user to configure dynamically the I3C callbacks */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * Initialization of the I3C peripheral.
  * This example demonstrates I3C communication in interrupt (IT) mode.
  * The user callbacks for I3C controller events are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }

  /* Register Callbacks ------------------------------------------------------*/
  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
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
  hal_status_t hal_status;    /* HAL status of the I3C operations */

  /** ########## Step 2 ##########
    * Activate notification for the Dynamic Address Assignment (DAA) process.
    * The DAA process is executed only once per power-up in app_process.
    */
  /* Activate notification for DAA process (Dynamic Address Assignment) */
  hal_status = HAL_I3C_TGT_ActivateNotification(pI3C,
                                                (uint8_t *)NULL,
                                                0U,
                                                HAL_I3C_TGT_NOTIFICATION_DAU);

  if (hal_status != HAL_OK)
  {
    /* Error occurred while activating notification. Handle and retry. */
    return_status = HandleTransferError();
  }

  /* Wait for DAA process to complete or for an error to occur */
  while ((DynamicAddressAssignmentReceived == 0U) && (TransferError == 0U))
  {
    /* Wait For Interrupt state. */
    __WFI();
  }

  if (TransferError == 1U)
  {
    return_status = HandleTransferError();
  }

  /* Retrieve Common Command Code info to check dynamic address assignment */
  hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
  if (hal_status != HAL_OK)
  {
    return_status  = HandleTransferError();
  }

  /* Check the validity of the assigned dynamic address */
  if ((CCCInfo.dynamic_addr_valid != VALID_ADDRESS)
      || (CCCInfo.dynamic_addr != TARGET_DYNAMIC_ADDRESS))
  {
    return_status  = HandleTransferError();
  }


  PRINTF("[INFO] Step 2: DAA process and verification COMPLETED.\n");

  /**
    * Delay to allow the I3C Controller to complete its post-DAA bus configuration,
    * before it starts listening for Control Role Requests from the Target.
    */
  HAL_Delay(1);

  /** ########## Step 3 ##########
    * Control Role Request and transfer completion.
    * Request to take control of the I3C bus, wait for the handoff to complete,
    * and handle the transfer complete or error event.
    */
  hal_status = HAL_I3C_TGT_ControlRoleReq_IT(pI3C);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while retrieving CCC info. */
    return_status  = HandleTransferError();
  }

  while ((ControllerRoleRequestCompleted == 0) && (TransferError == 0))
  {
    __WFI();
  }

  if (TransferError == 1U)
  {
    return_status  = HandleTransferError();
  }

  /* Configure the new timing */
  hal_i3c_ctrl_config_t i3c_ctrl_config;
  i3c_ctrl_config.timing_reg0 = (uint32_t)MX_I3C_TIMING_REGISTER_0;
  i3c_ctrl_config.timing_reg1 = (uint32_t)MX_I3C_TIMING_REGISTER_1;
  if (HAL_I3C_CTRL_SetConfig(pI3C, &i3c_ctrl_config) != HAL_OK)
  {
    return_status  = HandleTransferError();
  }

  /* Check if the Target became controller */
  if (HAL_I3C_GetMode(pI3C) == HAL_I3C_MODE_CTRL)
  {
    return_status = HandleTransferCplt();
  }

  return return_status;
} /* end app_process */


/** I3C target notification callback (interrupt mode)
  * @user This implementation of the I3C callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId)
{
  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DynamicAddressAssignmentReceived = 1U;
  }

  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_GETACCCR)
      == HAL_I3C_TGT_NOTIFICATION_GETACCCR)
  {
    ControllerRoleRequestCompleted = 1U;
  }
}

/** I3C transfer error callback
  * @user This implementation of the I3C callback can be customized.
  * This function is executed if the transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error */
  TransferError = 1U;
}


/** De-initializes the I3C instances before leaving the scenario.
  * In this example, app_deinit is never called, it is provided as a reference.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  PRINTF("[INFO] Target - Switch to controller COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param:  HAL status.
  * param:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  PRINTF("[ERROR] Target - Communication ERROR: hal_status = HAL_ERROR. \
         TRYING AGAIN.\n");

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
