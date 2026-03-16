/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller device switch to Target role using IT
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
/* Structure describing a known I3C target device */
typedef struct
{
  uint32_t  target_id;          /* Unique identifier for the target */
  uint64_t  target_bcr_dcr_pid; /* Device identification (BCR/DCR/PID) */
  uint8_t   dynamic_addr;       /* Dynamic address assigned */
} target_desc_t;

/* Private define ------------------------------------------------------------*/
/* Device addresses and buffer sizes ------------------------------------------*/
#define DEVICE_TARGET_X_ADDR    0x32U /* Target X address for I3C */
#define DEVICE_TARGET_Y_ADDR    0x34U /* Target Y address for I3C */
#define MAX_COM_ATTEMPTS_INI    2U    /* Maximum number of initial attempts */
/** Buffer Sizes */
#define I3C_PAYLOAD_SIZE        40U   /* Payload size in bytes */
#define RX_BUFFER_SIZE          (I3C_PAYLOAD_SIZE + 1U) /* RX buffer size */
#define CONTROL_BUFFER_SIZE     2U    /* Control buffer size */
#define CTRL_BUFFER_SIZE        20U   /* Size of the control buffer */
#define NUMBER_OF_DESCRIPTORS   1U    /* Number CCC transfer descriptor */

/** Common Command Codes */
#define I3C_DIRECT_GETACCCR_CCC  0x91 /* GETACCCR CCC code */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;      /* Pointer to I3C handle from the generated code */
hal_i3c_ccc_info_t CCCInfo;  /* Holds Common Command Code (CCC) information */

/* Context buffer for frame context, contains different buffer values */
hal_i3c_transfer_ctx_t ContextBuffers;

volatile uint32_t ControllerRoleRequestReceived = 0U; /* Controller role request (CRR) flag */
volatile uint32_t DynamicAddressAssignmentCompleted = 0U; /* Dynamic Address Assignment (DAA) flag */
volatile uint32_t TransferComplete = 0U;   /* Data transfer operation flag */
volatile uint32_t TransferError = 0U;      /* Data transfer error flag */

uint8_t TargetCount  = 0U; /* Number of targets assigned */

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[RX_BUFFER_SIZE] = {0U};

/* Buffer for transfer context */
uint32_t ControlBuffer[CTRL_BUFFER_SIZE] = {0U};

/* GETACCCR Common Command Code (CCC) read descriptors */
hal_i3c_ccc_desc_t GETACCR_Descriptor =
{
  DEVICE_TARGET_X_ADDR, I3C_DIRECT_GETACCCR_CCC, 1U, HAL_I3C_DIRECTION_READ,
};

/* Descriptor table for known targets */
target_desc_t Targets_Descriptor[2] =
{
  {0U, 0, DEVICE_TARGET_X_ADDR}, /* Target 0: default values */
  {1U, 0, DEVICE_TARGET_Y_ADDR}, /* Target 1: default values */
};

/* Private function prototypes -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);   /* Handle transfer completion */
static app_status_t HandleTransferError(void);  /* Handle transfer error */

/* Configure dynamically the I3C callbacks instead of weak functions */
/* Target requests dynamic address */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t Payload);
/* DAA complete callback */
static void CtrlDAACpltCallback(hal_i3c_handle_t *hi3c);
/* Transfer complete callback */
static void CtrlXferCpltCallback(hal_i3c_handle_t *hi3c);
/* Transfer error callback */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c);
/* Notification callback */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId);

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
  if (HAL_I3C_CTRL_RegisterTgtReqDynAddrCallback(pI3C, CtrlTgtReqDynAddrCallback)
      != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterDAACpltCallback(pI3C, CtrlDAACpltCallback)
      != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterTransferCpltCallback(pI3C, CtrlXferCpltCallback)
      != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback)
      != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback)
      != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t com_attempts_init = 0U; /* Number of initial attempts */

  /** ########## Step 2 ##########
    * Initiates and manages the Dynamic Address Assignment (DAA) process.
    * The DAA process is executed only once per power-up in app_process.
    */
  while ((com_attempts_init < MAX_COM_ATTEMPTS_INI)
         && (return_status == EXEC_STATUS_ERROR))
  {
    DynamicAddressAssignmentCompleted = 0U;
    com_attempts_init++;

    /* Initiate Dynamic Address Assignment (DAA) process for the controller */
    HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA);

    /* Wait for the Dynamic Address Assignment (DAA) process to complete */
    while ((DynamicAddressAssignmentCompleted == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* DAA process did not complete as expected. */
      return_status = HandleTransferError();
      TransferError = 0U;
      continue;
    }
  }

  PRINTF("[INFO] Step 2: DAA process COMPLETED.\n");

  /** ########## Step 3 ##########
    * Configure device-specific parameters for each assigned target,
    * including controller role request acknowledgments.
    */
  hal_i3c_ctrl_device_config_t DeviceConf[TargetCount];

  for (uint8_t TargetIndex = 0; TargetIndex < TargetCount; TargetIndex++)
  {
    /* Index value of the target device in the DEVRx register */
    DeviceConf[TargetIndex].device_index = Targets_Descriptor[TargetIndex].target_id;
    /* Dynamic address of the target device */
    DeviceConf[TargetIndex].tgt_dynamic_addr = Targets_Descriptor[TargetIndex].dynamic_addr;
    /* IBI acknowledgement capability of the target device (Set default value) */
    DeviceConf[TargetIndex].ibi_ack = HAL_I3C_CTRL_IBI_ACK_DISABLED;
    /* IBI payload size of the target device (Set default value, not used) */
    DeviceConf[TargetIndex].ibi_payload = HAL_I3C_CTRL_IBI_PAYLOAD_DISABLED;
    /* Controller Role Request acknowledgement setting */
    DeviceConf[TargetIndex].ctrl_role_req_ack = HAL_I3C_CTRL_ROLE_ACK_ENABLED;
    /* Stop transfer setting (Set default value to Disable) */
    DeviceConf[TargetIndex].ctrl_stop_transfer = HAL_I3C_CTRL_STOP_TRANSFER_DISABLED;
  }
  /* Bus device configuration configuration (controller) */
  if (HAL_I3C_CTRL_SetConfigBusDevices(pI3C, DeviceConf, TargetCount) != HAL_OK)
  {
    return_status  = HandleTransferError();
  }
  PRINTF("[INFO] Step 3: Device configuration COMPLETED.\n");


  /** ########## Step 4 ##########
    * Activate Controller Role Request notification and wait for completion.
    */
  HAL_I3C_CTRL_ActivateNotification(pI3C, HAL_I3C_CTRL_NOTIFICATION_CR);
  while ((ControllerRoleRequestReceived == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state */
    __WFI();
  }
  if (TransferError == 1U)
  {
    /* An error occurs during the write/read transfer */
    return_status = HandleTransferError();
    TransferError = 0U;
  }
  PRINTF("[INFO] Step 4: CRR notification RECEIVED.\n");
  /* Retrieve Common Command Code info to check dynamic address assignment */
  if (HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_CTRL_NOTIFICATION_CR, &CCCInfo) != HAL_OK)
  {
    return_status = HandleTransferError();
  }

  /** ########## Step 5 ##########
    * Perform a direct read of the GETACCCR Common Command Code (CCC) from targets.
    * This reads device characteristics after address assignment.
    */
  uint32_t ctrl_buff_size =
    HAL_I3C_GET_CTRL_BUFFER_SIZE_WORD(NUMBER_OF_DESCRIPTORS,
                                      HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_STOP);

  /* Reset the controller transfer context buffers */
  HAL_I3C_CTRL_ResetTransferCtx(&ContextBuffers);
  /* Initialise the transfer context with Rx buffer to receive GETACCCR data */
  HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, RX_BUFFER_SIZE);
  /* Initialise the transfer context with Tc buffer for control data */
  HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer, ctrl_buff_size);
  /* Update Target address in the Get CCC list */
  GETACCR_Descriptor.tgt_addr = CCCInfo.ibi_cr_tgt_addr;
  /* Build the transfer context for GETACCCR CCC direct read */
  HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                   &GETACCR_Descriptor,
                                   NUMBER_OF_DESCRIPTORS,
                                   HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_STOP);
  /* Start the GETACCCR data reception in interrupt mode */
  HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
  /* Wait for the GETACCCR data reception to complete */
  while (TransferComplete == 0U)
  {
    __WFI();
  }

  if (TransferError == 1U)
  {
    return_status = HandleTransferError();
    TransferError = 0U;
  }

  /** Check received target address
    * RxBuffer[0] uses bits [7:1] to hold the 7-bit dynamic address.
    */
  if ((RxBuffer[0] >> 1) != CCCInfo.ibi_cr_tgt_addr)
  {
    /* Dynamic address does not match */
    return_status  = HandleTransferError();
  }
  else
  {
    PRINTF("[INFO] Step 5: GETACCCR data reception completed.\n");
    return_status = HandleTransferCplt();
  }

  return return_status;
} /* end app_process */


/** I3C target requests Dynamic Address callback
  * @user This implementation of the I3C transfer complete callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t payload)
{
  Targets_Descriptor[TargetCount].target_bcr_dcr_pid = payload;
  HAL_I3C_CTRL_SetDynAddr(hi3c, Targets_Descriptor[TargetCount].dynamic_addr);
  TargetCount++;
}

/** Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the I3C transfer complete callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void CtrlDAACpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to DAA completion */
  DynamicAddressAssignmentCompleted = 1U;
}

/** I3C target notification callback (interrupt mode)
  * @user This implementation of the I3C transfer complete callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId)
{
  if ((notifyId & HAL_I3C_CTRL_NOTIFICATION_CR) == HAL_I3C_CTRL_NOTIFICATION_CR)
  {
    ControllerRoleRequestReceived = 1U;
  }
}

/** Controller transmission end of transfer callback
  * @user This implementation of the I3C transfer complete callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void CtrlXferCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer completion (Step 3) */
  TransferComplete = 1U;
}


/** I3C transfer error callback
  * @user This implementation of the I3C transfer error callback can be customized.
  * This function is executed if the transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error */
  TransferError = 1U;
}


/** De-initializes the I3C instances before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
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
  PRINTF("[INFO] Controller - Tx/Rx Buffers IDENTICAL. \
         Transfer COMPLETED of %s \n", RxBuffer);

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param:  HAL status.
  * param:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = HAL_ERROR. \
         TRYING AGAIN.\n");

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
