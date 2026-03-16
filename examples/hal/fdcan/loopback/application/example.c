/**
  ******************************************************************************
  * file           : example.c
  * brief          : Loopback application handling a data transfer over
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
/* @user: set the timeout period in milliseconds for transmitting and receiving data */
#define FDCAN_COM_TIMEOUT 100U

/** @user: define the two frame ID used in this project
  * one in Standard Mode (11bits) and one in Extended Mode (29bits)
  * These parameters are defined in generated files for filter configuration
  */
#define FDCAN_STANDARD_FRAME_ID   0x101
#define FDCAN_EXTENDED_FRAME_ID   0x18000002

/* Data size of the TX and RX FDCAN frames in bytes. */
#define MSG_DATA_LENGTH   HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE

/** Data size of the FDCAN buffer in bytes.
  * @user: For CAN Classical frame maximum bytes size is 8, for FDCAN frame maximum bytes size is 64.
  */
#define BUFFER_SIZE       8U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_fdcan_handle_t *pFDCAN;    /* pointer referencing the FDCAN handle from the generated code */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length.
  */
static const uint8_t BufferA[BUFFER_SIZE] = {0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE};
static const uint8_t BufferB[BUFFER_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;

/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t fdcan_error_code);
static uint8_t BuffersMatch(const uint8_t *buffer1, uint8_t *buffer2, uint16_t buffer_length);

/** --------------------------------------------------------------------------------------------------------------
  * Applicative code demonstrating a data transfer on the FDCAN-bus protocol, in polling and loopback mode.
  * --------------------------------------------------------------------------------------------------------------
  */

/** ########## Step 1 ##########
  * The init of the FDCAN instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;

  pFDCAN = mx_example_fdcan_init();

  if (pFDCAN != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the FDCAN TX/RX operations */
  uint32_t rx_elements = 0U;  /* memorizes the number of frame in Rx Fifo */
  uint32_t fdcan_error_code;   /* memorizes the FDCAN error code limited to the last process */
  uint32_t rx_msg_start_tick = 0U; /* memorizes the starting tick to detect reception timeout */

  hal_fdcan_rx_header_t rx_element_header;

  /* Prepare FDCAN transmit header for 1st message with STANDARD ID */
  hal_fdcan_tx_header_t tx_element_header_A =
  {
    .b.identifier = FDCAN_STANDARD_FRAME_ID,
    .b.frame_type = HAL_FDCAN_FRAME_DATA,
    .b.identifier_type = HAL_FDCAN_ID_STANDARD,
    .b.error_state_indicator = HAL_FDCAN_ERROR_STATE_IND_ACTIVE,
    .b.data_length = MSG_DATA_LENGTH,
    .b.bit_rate_switch = HAL_FDCAN_BIT_RATE_SWITCH_ON,
    /* @user: change to HAL_FDCAN_FRAME_FORMAT_CAN to send in classical CAN format.*/
    .b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN,
    .b.event_fifo_control = HAL_FDCAN_TX_EVENTS_FIFO_STORE,
    .b.message_marker = 1U,
  };

  /* Prepare FDCAN transmit header for 2nd message with EXTENDED ID */
  hal_fdcan_tx_header_t tx_element_header_B =
  {
    .b.identifier = FDCAN_EXTENDED_FRAME_ID,
    .b.frame_type = HAL_FDCAN_FRAME_DATA,
    .b.identifier_type = HAL_FDCAN_ID_EXTENDED,
    .b.error_state_indicator = HAL_FDCAN_ERROR_STATE_IND_ACTIVE,
    .b.data_length = MSG_DATA_LENGTH,
    .b.bit_rate_switch = HAL_FDCAN_BIT_RATE_SWITCH_ON,
    /* @user: change to HAL_FDCAN_FRAME_FORMAT_CAN to send in classical CAN format.*/
    .b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN,
    .b.event_fifo_control = HAL_FDCAN_TX_EVENTS_FIFO_STORE,
    .b.message_marker = 1U,
  };

  /** ########## Step 2 ##########
    * Transmits FDCAN messages.
    */

  /* Start FDCAN module */
  HAL_FDCAN_Start(pFDCAN);

  hal_status = HAL_FDCAN_ReqTransmitMsgFromFIFOQ(pFDCAN, &tx_element_header_A, BufferA);
  if (hal_status != HAL_OK)
  {
    /* Report Error if it occurs during the FDCAN write transfer */
    fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
    return_status  = HandleTransferError(hal_status, fdcan_error_code);
    goto _app_process_exit;
  }

  hal_status = HAL_FDCAN_ReqTransmitMsgFromFIFOQ(pFDCAN, &tx_element_header_B, BufferB);
  if (hal_status != HAL_OK)
  {
    /* Report Error if it occurs during the FDCAN write transfer */
    fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
    return_status  = HandleTransferError(hal_status, fdcan_error_code);
    goto _app_process_exit;
  }

  /** Wait transmissions complete
    * Check that Tx FIFO is fully available
    */
  while (HAL_FDCAN_GetTxFifoFreeLevel(pFDCAN) != HAL_FDCAN_TX_FIFO_FREE_LEVEL_3) {}

  PRINTF("[INFO] Step 2: Messages transmitted.\n");

  /** ########## Step 3 ##########
    * Receives back and check the FDCAN messages
    */

  /** Manage received messages in Rx FIFO 0
    * Check FIFO level, then get message and check data
    */
  hal_status = HAL_TIMEOUT;
  rx_msg_start_tick = HAL_GetTick();
  while (HAL_GetTick() - rx_msg_start_tick < FDCAN_COM_TIMEOUT)
  {
    HAL_FDCAN_GetRxFifoFillLevel(pFDCAN, HAL_FDCAN_RX_FIFO_0, &rx_elements);
    if (rx_elements > 0)
    {
      hal_status = HAL_FDCAN_GetReceivedMessage(pFDCAN, HAL_FDCAN_RX_FIFO_0, &rx_element_header, RxBuffer);
      break;
    }
  }

  if (hal_status != HAL_OK)
  {
    /* Report Error if it occurs during the FDCAN receive transfer */
    fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
    return_status  = HandleTransferError(hal_status, fdcan_error_code);
    goto _app_process_exit;
  }

  if (BuffersMatch(BufferA, RxBuffer, BUFFER_SIZE) == 0)
  {
    goto _app_process_exit;
  }

  /** Manage received messages in Rx FIFO 1
    * Check FIFO level, then get message and check data
    */
  hal_status = HAL_TIMEOUT;
  rx_msg_start_tick = HAL_GetTick();
  while (HAL_GetTick() - rx_msg_start_tick < FDCAN_COM_TIMEOUT)
  {
    HAL_FDCAN_GetRxFifoFillLevel(pFDCAN, HAL_FDCAN_RX_FIFO_1, &rx_elements);
    if (rx_elements > 0)
    {
      hal_status = HAL_FDCAN_GetReceivedMessage(pFDCAN, HAL_FDCAN_RX_FIFO_1, &rx_element_header, RxBuffer);
      break;
    }
  }

  if (hal_status != HAL_OK)
  {
    /* Report Error if it occurs during the FDCAN receive transfer */
    fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
    return_status  = HandleTransferError(hal_status, fdcan_error_code);
    goto _app_process_exit;
  }

  if (BuffersMatch(BufferB, RxBuffer, BUFFER_SIZE) == 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: All received messages are correct.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:

  return return_status;
} /* end app_process */


/* De-initializes the FDCAN instance before leaving the scenario.*/
app_status_t app_deinit(void)
{
  /* Stop FDCAN module */
  HAL_FDCAN_Stop(pFDCAN);

  mx_example_fdcan_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Compares the data of two buffers.
  * param buffer_:  buffers to be compareds.
  * param buffer_length:  length of buffers.
  * retval: 0 or 1
  */
static uint8_t BuffersMatch(const uint8_t *buffer1, uint8_t *buffer2, uint16_t buffer_length)
{
  for (uint8_t i = 0; i < buffer_length ; i++)
  {
    if (*buffer1 != *buffer2)
    {
      PRINTF("[ERROR] Tx/Rx Buffers different.\n");
      return 0;
    }

    buffer1++;
    buffer2++;
  }
  return 1;
} /* end BuffersCompare */


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

