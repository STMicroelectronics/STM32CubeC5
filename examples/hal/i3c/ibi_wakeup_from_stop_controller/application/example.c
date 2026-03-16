/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller example for IBI with entry to stop mode using interrupt handling
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

/* Target descriptor structure for ENTDAA target management */
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

/** Target device addresses for I3C communication.
  * @user: it is possible to modify these addresses as needed.
  */
#define DEVICE_TARGET_ADDR      0x32U

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define DAA_MAX_ATTEMPTS    2U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;   /* Pointer referencing the I3C handle from the generated code */

hal_i3c_ccc_info_t CCCInfo; /* Holds Common Command Code (CCC) info, used to check IBI details after reception */

hal_i3c_ctrl_device_config_t DeviceConf[4]; /* buffer for I3C controller device configuration and IBI settings */

/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when the IBI process is completed */
volatile uint8_t IBIProcessComplete = 0U;
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;


/** Descriptor for the known target.
  * @user: it is possible to modify STATIC_ADDR and DYNAMIC_ADDR as needed for your hardware.
  */
target_desc_t Targets_Descriptor = {"", 0U, 0, 0, DEVICE_TARGET_ADDR};

/* Expected IBI payload value received by the controller */
uint32_t ReceivedPayloadData = 0x00EFCDAB;


/* Private functions prototype -----------------------------------------------*/
static app_status_t PerformDAAProcess(void);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t target_payload);
static void CtrlDAACpltCallback(hal_i3c_handle_t *pI3C);
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);

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
  if (HAL_I3C_CTRL_RegisterTgtReqDynAddrCallback(pI3C, CtrlTgtReqDynAddrCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterDAACpltCallback(pI3C, CtrlDAACpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
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
  hal_status_t hal_status;       /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */
  uint32_t bcr;


  /* Reset IBI and error flags at start of process */
  IBIProcessComplete = 0U;
  TransferError = 0U;


  /** ########## Step 2 ##########
    * Controller initiates dynamic address assignment by sending ENTDAA CCC command.
    * All DAA logic is encapsulated in the PerformDAAProcess() function.
    */

  return_status = PerformDAAProcess();
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: DAA process COMPLETED.\n");

  /** ########## Step 3 ##########
    * Configure IBI handling for the target and enable IBI/wakeup notifications.
    */

  /* Configure the controller device table entry for this target.
    * This configuration is required to handle IBI correctly:
    * - identify the target (device index + dynamic address)
    * - indicate whether the controller accepts (ACK) IBI requests
    * - declare whether an optional IBI payload is supported/expected
    */

  /* Application target identifier (for this config entry). */
  DeviceConf[0].device_index = Targets_Descriptor.target_id;

  /* Target dynamic address (assigned during ENTDAA). */
  DeviceConf[0].tgt_dynamic_addr = Targets_Descriptor.dynamic_addr;

  bcr = HAL_I3C_GET_BCR(Targets_Descriptor.target_bcr_dcr_pid);

  /* IBI capability (from BCR). */
  DeviceConf[0].ibi_ack = (HAL_I3C_GET_IBI_CAPABLE(bcr) == HAL_I3C_IBI_REQ_ENABLED) ? HAL_I3C_CTRL_IBI_ACK_ENABLED : HAL_I3C_CTRL_IBI_ACK_DISABLED;

  /* IBI payload capability (from BCR). */
  DeviceConf[0].ibi_payload = (HAL_I3C_GET_IBI_PAYLOAD(bcr) == HAL_I3C_IBI_PAYLOAD_ENABLED) ? HAL_I3C_CTRL_IBI_PAYLOAD_ENABLED: HAL_I3C_CTRL_IBI_PAYLOAD_DISABLED;


  /* Controller role request capability (from BCR). */
  DeviceConf[0].ctrl_role_req_ack = (HAL_I3C_GET_CTRL_ROLE_CAPABLE(bcr) == HAL_I3C_CTRL_ROLE_ENABLED) ? HAL_I3C_CTRL_ROLE_ACK_ENABLED : HAL_I3C_CTRL_ROLE_ACK_DISABLED;

  /* No forced STOP transfer for this target. */
  DeviceConf[0].ctrl_stop_transfer = HAL_I3C_CTRL_STOP_TRANSFER_DISABLED;

  hal_status = HAL_I3C_CTRL_SetConfigBusDevices(pI3C, &DeviceConf[0], 1U);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while configuring the device for IBI. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  /* Enable IBI notification for the controller */
  hal_status = HAL_I3C_CTRL_ActivateNotification(pI3C, HAL_I3C_CTRL_NOTIFICATION_IBI | HAL_I3C_TGT_NOTIFICATION_WKP);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while activating IBI notification. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: Activates the notification IBI COMPLETED.\n");

  /** ########## Step 4 ##########
    * Waits for one of these I3C interrupts: IBI completion or transfer error.
    * Put the MCU in STOP Mode and Wait For Interrupt state.
    */

  PRINTF("[INFO] Step 4: Wait for interrupt. Enter STOP Mode while no reception.\n");

  while ((IBIProcessComplete == 0U) && (TransferError == 0U))
  {

    /** @User: This pin is used to check the stop mode:
      * MX_STATUS_WAKE_PIN is ON when MCU is in RUN mode and OFF when MCU in STOP mode
      */

    HAL_GPIO_WritePin(MX_STATUS_WAKEUP_PIN_GPIO_PORT, MX_STATUS_WAKEUP_PIN, MX_STATUS_WAKEUP_PIN_INACTIVE_STATE);

    /** Suspend Tick increment to prevent WakeUp by Systick interrupt.
      * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
      */
    HAL_SuspendTick();

    /* Put the MCU in STOP Mode and Wait For Interrupt state. Wait for an I2C interrupt to wake up the MCU.*/
    HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, HAL_PWR_STOP_MODE);

    /** Resume tick then configure the clock.
      * Clock configuration needs ticks to manage timeout.
      * Reconfiguring the clock ensures the I3C peripheral restores previous clock settings.
      */
    HAL_ResumeTick();
    mx_rcc_init();

    /* @User: This pin is used to check the wakeup: check the STATUS_WAKE_PIN set on wakeup */
    HAL_GPIO_WritePin(MX_STATUS_WAKEUP_PIN_GPIO_PORT, MX_STATUS_WAKEUP_PIN, MX_STATUS_WAKEUP_PIN_ACTIVE_STATE);

  }

  if (TransferError == 1U)
  {
    /* Error occurred while configuring device for IBI. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: IBI received, MCU woke up from STOP mode.\n");

  /** ########## Step 5 ##########
    * After IBI reception, verify CCC info, payload length, target address, and payload value.
    */

  /* Retrieve and verify CCC (Common Command Code) info after IBI reception */
  hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_CTRL_NOTIFICATION_IBI, &CCCInfo);
  if (hal_status != HAL_OK)
  {
    /* Error occurred while retrieving CCC info. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status  = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  /* Check number of payload bytes received after IBI.
    * In this example, the target is configured to send a fixed 3-byte (24-bit) IBI payload to the controller.
    */
  if (CCCInfo.ibi_tgt_nb_payload != 3U)
  {
    PRINTF("[ERROR] IBI payload length mismatch.\n");
    goto _app_process_exit;
  }

  /* Check target address received during accepted IBI */
  if (CCCInfo.ibi_cr_tgt_addr != DEVICE_TARGET_ADDR)
  {
    PRINTF("[ERROR] IBI target address mismatch.\n");
    goto _app_process_exit;
  }

  /* Check IBI payload value received by the controller */
  if (CCCInfo.ibi_tgt_payload != ReceivedPayloadData)
  {
    PRINTF("[ERROR] IBI payload value mismatch.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5: All values received from the target are correct.\n");
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** brief:  Initiates and completes Dynamic Address Assignment (DAA) for all targets using ENTDAA.
  * param:  none
  * retval: example status (EXEC_STATUS_OK if successful, EXEC_STATUS_ERROR otherwise)
  */
static app_status_t PerformDAAProcess(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the last I3C operation */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */
  uint32_t daa_attempts = 0U; /* Memorizes the number of attempts for Dynamic Address Assignment */

  while ((daa_attempts < DAA_MAX_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Reset DAA status flags before starting a new DAA attempt */
    TransferError = 0U;
    DAAProcessComplete = 0U;

    daa_attempts++;

    /* Initiate Dynamic Address Assignment (DAA) process for the controller */
    hal_status = HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA);
    if (hal_status != HAL_OK)
    {
      /* Error occurred during DAA process initiation. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;

    }

    while ((DAAProcessComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* Transfer process did not complete as expected. Report the error and restart the communication attempt. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }
    else
    {
      return_status = EXEC_STATUS_OK;

    }
  }

_app_process_exit:
  return return_status;
}


/**
  * Target requests dynamic address callback
  * @user This implementation of the I3C target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t target_payload)
{
  /* Assign received payload and dynamic address to the target */
  Targets_Descriptor.target_bcr_dcr_pid = target_payload;
  HAL_I3C_CTRL_SetDynAddr(hi3c, DEVICE_TARGET_ADDR);
}


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
  * I3C controller notification callback (interrupt mode).
  * @user: Customize this function to handle controller notifications.
  * This function is executed when a notification event occurs, such as IBI reception.
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notify_id)
{
  if ((notify_id & HAL_I3C_CTRL_NOTIFICATION_IBI) == HAL_I3C_CTRL_NOTIFICATION_IBI)
  {
    IBIProcessComplete = 1U;
  }
}


/**
  * I3C transfer error callback
  * @user This implementation of the I3C controller transfer error callback can be customized.
  * This function is executed when the controller transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error */
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
  PRINTF("[ERROR] Transfer error: hal_status = %" PRIu8 ", HAL_I3C_GetLastErrorCodes = %" PRIu32
         " TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
