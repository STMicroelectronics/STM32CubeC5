/**
  ******************************************************************************
  * file           : example.c
  * brief          : The controller's application handling a data transfer over
  *                  FDCAN in polling mode with HAL API.
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
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U
/* @user: set the timeout period in milliseconds for transmitting and receiving data */
#define FDCAN_COM_TIMEOUT 1000U
/* @user: define the controller requested Frame ID in Standard Mode (11bits) */
#define FDCAN_CONTROLLER_FRAME_ID 0x101
/* Data size of the TX and RX FDCAN frames in bytes. */
#define MSG_DATA_LENGTH   HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE

/* Data size of the TX FDCAN buffer in bytes */
#define TX_BUFFER_SIZE       8U
/** Data size of the RX FDCAN buffer in bytes to be set to maximum frame size.
  * @user: For CAN Classical frame maximum bytes size is 8, for FDCAN frame maximum bytes size is 64.
  */
#define RX_BUFFER_SIZE       64U


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_fdcan_handle_t *pFDCAN;    /* pointer referencing the FDCAN handle from the generated code */

/* interrupts flags */
volatile uint8_t TransmissionCompleteFlag = 0U;   /* Set to 1 if a transmission complete is detected */
volatile uint8_t ReceptionFlag = 0U;   /* Set to 1 if a transmission complete is detected */
volatile uint8_t TransferError = 0U;   /* Set to 1 if a transmission or a reception error is detected */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length.
  */
static const uint8_t BufferA[TX_BUFFER_SIZE] = {0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE};
static const uint8_t BufferB[TX_BUFFER_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;

/* Buffer used for reception */
uint8_t RxBuffer[RX_BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(hal_fdcan_rx_header_t rx_element_header);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t fdcan_error_code);

/* Functions allowing the user to configure dynamically the FDCAN callbacks instead of weak functions */
static void TxTransmissionCompleteCallback(hal_fdcan_handle_t *hfdcan, uint32_t buffer_indexes);
static void RxFifoEventCallback(hal_fdcan_handle_t *hfdcan, uint32_t buffer_indexes);
static void TransferErrorCallback(hal_fdcan_handle_t *hfdcan);


/** ########## Step 1 ##########
  * The init of the FDCAN instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;

  pFDCAN = mx_example_fdcan_init();
  if (pFDCAN == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the FDCAN callbacks */
  if (HAL_FDCAN_RegisterTxBufferCompleteCallback(pFDCAN, TxTransmissionCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_FDCAN_RegisterRxFifo0Callback(pFDCAN, RxFifoEventCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_FDCAN_RegisterErrorCallback(pFDCAN, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

  /* Start FDCAN module */
  HAL_FDCAN_Start(pFDCAN);

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the FDCAN TX/RX operations */
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  uint32_t fdcan_error_code;   /* memorizes the FDCAN error code limited to the last process */

  hal_fdcan_rx_header_t rx_element_header;

  /* Prepare FDCAN transmit message header */
  hal_fdcan_tx_header_t tx_element_header =
  {
    .b.identifier = FDCAN_CONTROLLER_FRAME_ID,
    .b.frame_type = HAL_FDCAN_FRAME_DATA,
    .b.identifier_type = HAL_FDCAN_ID_STANDARD,
    .b.error_state_indicator = HAL_FDCAN_ERROR_STATE_IND_ACTIVE,
    .b.message_marker = 52U,
    /* @user: change to HAL_FDCAN_FRAME_FORMAT_CAN to send in classical CAN format.*/
    .b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN,
    .b.bit_rate_switch = HAL_FDCAN_BIT_RATE_SWITCH_ON,
    .b.event_fifo_control = HAL_FDCAN_TX_EVENTS_FIFO_STORE,
    .b.data_length = MSG_DATA_LENGTH,
  };

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the FDCAN interrupts flags before starting a new transfer */
    TransmissionCompleteFlag = 0U;
    ReceptionFlag = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 2 ##########
      * Transmit in interrupt mode the buffer pointed by pTxData using FDCAN.
      */
    hal_status = HAL_FDCAN_ReqTransmitMsgFromFIFOQ(pFDCAN, &tx_element_header, pTxData);
    if (hal_status != HAL_OK)
    {
      /* An error occurs during the FDCAN write transfer. Report it, then restart the communication */
      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }

    /* Waiting for FDCAN TransmissionCompleteFlag interrupt */
    while ((TransmissionCompleteFlag == 0) && (TransferError == 0))
    {
      __WFI();
    }

    if (TransferError == 1U)
    {

      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Waits for one of these FDCAN interrupts: buffer reception complete or transfer error
      */


    while ((ReceptionFlag == 0) && (TransferError == 0))
    {

      __WFI();
    }
    if (TransferError == 1U)
    {
      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }
    hal_status = HAL_FDCAN_GetReceivedMessage(pFDCAN, HAL_FDCAN_RX_FIFO_0, &rx_element_header, RxBuffer);
    if (hal_status != HAL_OK)
    {

      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status  = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt(rx_element_header);

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the FDCAN instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  /* Stop FDCAN module */
  HAL_FDCAN_Stop(pFDCAN);

  mx_example_fdcan_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between BufferA and BufferB to select the message to send */
  if (pTxData == BufferA)
  {
    pTxData = BufferB;
  }
  else
  {
    pTxData = BufferA;
  }

  memset(RxBuffer, 0U, TX_BUFFER_SIZE);
} /* end UpdateBuffers */

/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(hal_fdcan_rx_header_t rx_element_header)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* As the FDCAN filter configuration rejects all unwanted frames, no need to check rx_element_header identifier */
  if ((rx_element_header.b.data_length == MSG_DATA_LENGTH)
      && (memcmp(pTxData, RxBuffer, TX_BUFFER_SIZE) == 0))
  {
    PRINTF("[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of %" PRIu32 " bytes.\n",
           (uint32_t)TX_BUFFER_SIZE);

    return_status = EXEC_STATUS_OK;
  }
  else
  {
    PRINTF("[ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.\n");
  }

  return return_status;
} /* end HandleTransferCplt */

/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the FDCAN TX/RX operations.
  * param FDCAN_error_code:  FDCAN Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t fdcan_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
    HAL_FDCAN_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, fdcan_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */


/** transmission Complete callback
  * @user This implementation of the FDCAN Tx Transmission  complete callback can be customized.
  * This function is executed if the Tx Transmission  complete interrupt is generated.
  */
static void TxTransmissionCompleteCallback(hal_fdcan_handle_t *hfdcan, uint32_t buffer_indexes)
{
  TransmissionCompleteFlag = 1U;
}

/** Rx Fifo0 callback
  * @user This implementation of the FDCAN Rx Fifo0 callback can be customized.
  * This function is executed if new message interrupt is generated.
  */
static void RxFifoEventCallback(hal_fdcan_handle_t *hfdcan, uint32_t buffer_indexes)
{
  /* Asynchronous processing related to step 5 */
  ReceptionFlag = 1U;
}
/** transfer Error callback
  * @user This implementation of the FDCAN transfer Error callback can be customized.
  * This function is executed if transfer Error interrupt is generated.
  */
static void TransferErrorCallback(hal_fdcan_handle_t *hfdcan)
{
  /* Asynchronous processing related to step 3 and 5 */
  TransferError = 1U;
}
