/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller private transfer example using interrupt (IT) mode and HAL API.
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

/* @user Target device address for I3C communication. */
#define DEVICE_TARGET_ADDR      0x32U

/* @user Set the maximum number of attempts to complete a data transfer between the two boards. */
#define MAX_COM_ATTEMPTS        10U

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define MAX_COM_ATTEMPTS_INI    2U

/* @user Payload size in bytes. The I3C message is a string of this payload size, terminated by '\0'. */
#define I3C_PAYLOAD_SIZE        40U

/**
  * @user Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted I3C message is a null-terminated string.
  */
#define BUFFER_SIZE             (I3C_PAYLOAD_SIZE + 1U)

/* Number of private descriptors used for private transfers */
#define CONTROL_BUFFER_SIZE     2U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;   /* pointer referencing the I3C handle from the generated code */

/* Context buffer related to frame context, contains different buffer values for a communication */
hal_i3c_transfer_ctx_t ContextBuffers;

/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when a data transfer operation is complete */
volatile uint8_t TransferComplete = 0U;
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;

uint8_t DaaCompleted = 0U; /* Tracks if DAA process completed for this power cycle */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update I3C_PAYLOAD_SIZE if necessary.
  */
const uint8_t BufferA[BUFFER_SIZE] = "I3C Two Boards Communication - Message A";

const uint8_t BufferB[BUFFER_SIZE] = "I3C Two Boards Communication - Message B";

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Buffer for transfer context */
uint32_t ControlBuffer[10] = {0U};

/* Descriptor for private data transmit */
hal_i3c_private_desc_t PrivateDescriptor[2] =
{
  {DEVICE_TARGET_ADDR, BUFFER_SIZE, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, BUFFER_SIZE, HAL_I3C_DIRECTION_READ}
};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload);
static void CtrlDAACpltCallback(hal_i3c_handle_t *pI3C);
static void CtrlXferCpltCallback(hal_i3c_handle_t *pI3C);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * Initialization of the I3C peripheral and configuration of the transfer context are triggered by the application code
  * This example demonstrates I3C communication in interrupt (IT) mode.
  * The user callbacks for I3C controller events are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;       /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */

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

  if (HAL_I3C_CTRL_RegisterTransferCpltCallback(pI3C, CtrlXferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Reset the transfer context to ensure a clean state before configuring new transfer parameters. */
  hal_status = HAL_I3C_CTRL_ResetTransferCtx(&ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the reset a transfer. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status  = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context control structure with the control buffer and descriptor size. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers,
                                              ControlBuffer,
                                              HAL_I3C_GET_CTRL_BUFFER_SIZE_WORD(CONTROL_BUFFER_SIZE,
                                                                                HAL_I3C_PRIVATE_WITH_ARB_STOP));
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the transfer context */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context for transmission with BufferB. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, BufferB, BUFFER_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the initialise the transfer context with BufferB */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context for reception with RxBuffer. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, BUFFER_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs while initializing the transfer context for reception with RxBuffer. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Build the private transfer context using the provided descriptors and options. */
  hal_status = HAL_I3C_CTRL_BuildTransferCtxPrivate(&ContextBuffers,
                                                    PrivateDescriptor,
                                                    CONTROL_BUFFER_SIZE,
                                                    HAL_I3C_PRIVATE_WITH_ARB_STOP);
  if (hal_status != HAL_OK)
  {
    /* An error occurs while building the private transfer context. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
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
  uint32_t com_attempts = 0U;    /* Memorizes the number of attempts made to exchange a message between the boards */
  uint32_t com_attemptsini = 0U; /* Memorizes the number of initial attempts for Dynamic Address Assignment (DAA)*/
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */

  /** ########## Step 2 ##########
    * Initiates and manages the Dynamic Address Assignment (DAA) process for the controller.
    * The DAA process is executed only once per power-up in app_process.
    */
  if (!DaaCompleted)
  {
    while ((com_attemptsini < MAX_COM_ATTEMPTS_INI) && (return_status == EXEC_STATUS_ERROR))
    {
      DAAProcessComplete = 0U;

      com_attemptsini++;

      /* Initiate Dynamic Address Assignment (DAA) process for the controller */
      hal_status = HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA);
      if (hal_status != HAL_OK)
      {
        /* Error occurred during DAA process initiation. Handle and retry. */
        i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
        return_status = HandleTransferError(hal_status, i3c_error_code);
        continue;
      }

      /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
      while ((DAAProcessComplete == 0) && (TransferError == 0))
      {
        /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
          * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
          */
        __WFI();
      }

      if (TransferError == 1)
      {
        /* DAA process did not complete as expected. Report the error and restart the communication attempt. */
        i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
        return_status = HandleTransferError(hal_status, i3c_error_code);
        continue;
      }
    }
    DaaCompleted = 1U;

    PRINTF("[INFO] Step 2: DAA process COMPLETED.\n");
  }

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the I3C interrupts flags before starting a new transfer */
    TransferComplete = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts ++;

    /* Let the target finish wake-up before starting the transfer. */
    HAL_Delay(5);

    /** ########## Step 3 ##########
      * Transmits in IT mode the buffer A or buffer B, which contains a null-terminated string,
      * and receives another null-terminated string buffer, both using I3C in IT (interrupt) mode.
      * @user: When this controller is used with a target wakeup example, the target wakes up
      * when the controller starts the private transfer.
      * For this wakeup use case only, add a short delay on the controller side, and use RX stall,
      * if the target needs more time to restore its clock and flash execution after wakeup.
      */
    hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);

    if (hal_status != HAL_OK)
    {
      /* An error occurs during the write/read transfer. Report the error and restart the communication attempt. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Waits for one of these I3C interrupts: write transfer complete or transfer error.
      */
    while ((TransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* An error occurs during the write/read transfer. Report the error and restart the communication attempt. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 5 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
  /* end app_process */
}


/**
  * Target requests dynamic address callback
  * @user This implementation of the I3C target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload)
{
  /* Assign the dynamic address to the target device */
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


/** Controller transmission end of transfer callback
  * @user This implementation of the I3C controller transfer complete callback can be customized.
  * This function is executed when the controller transfer complete interrupt is generated.
  */
static void CtrlXferCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer completion (Step 3) */
  TransferComplete = 1U;
}


/**
  * I3C transfer error callback
  * @user This implementation of the I3C controller transfer error callback can be customized.
  * This function is executed when the controller transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error (Step 3 and Step 5) */
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


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Update the context to toggle the message being sent between BufferA and BufferB */
  if (ContextBuffers.p_tx_data == BufferA)
  {
    HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, BufferB, BUFFER_SIZE);
  }
  else
  {
    HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, BufferA, BUFFER_SIZE);
  }
  memset(RxBuffer, 0U, BUFFER_SIZE);
} /* end UpdateBuffers */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (memcmp(ContextBuffers.p_tx_data, RxBuffer, BUFFER_SIZE) == 0)
  {
    PRINTF("[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of %s \n", RxBuffer);
    return_status = EXEC_STATUS_OK;
  }
  else
  {
    PRINTF("[ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.\n");
  }

  return return_status;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I3C TX/RX operations.
  * param i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu8 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
