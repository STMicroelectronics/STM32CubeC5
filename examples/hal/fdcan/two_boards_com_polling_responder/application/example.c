/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder's application handling a data transfer over
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
/* @user: define the responder requested Frame ID in Standard Mode (11bits) */
#define FDCAN_RESPONDER_FRAME_ID 0x102
/** Data size of the RX FDCAN buffer in bytes to be set to maximum frame size.
  * @user: For CAN Classical frame maximum bytes size is 8, for FDCAN frame maximum bytes size is 64.
  */
#define BUFFER_SIZE       64U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_fdcan_handle_t *pFDCAN;    /* pointer referencing the FDCAN handle from the generated code */

/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t fdcan_error_code);

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

  /* Start FDCAN module */
  HAL_FDCAN_Start(pFDCAN);

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the FDCAN TX/RX operations */
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  uint32_t rx_elements = 0U;  /* memorizes the number of frame in Rx Fifo */
  uint32_t fdcan_error_code;   /* memorizes the FDCAN error code limited to the last process */
  uint32_t rx_msg_start_tick = 0U; /* memorizes the starting tick to detect reception timeout */

  hal_fdcan_rx_header_t rx_element_header;

  /* Prepare FDCAN transmit message header */
  hal_fdcan_tx_header_t tx_element_header =
  {
    .b.identifier = FDCAN_RESPONDER_FRAME_ID,
    .b.frame_type = HAL_FDCAN_FRAME_DATA,
    .b.identifier_type = HAL_FDCAN_ID_STANDARD,
    .b.error_state_indicator = HAL_FDCAN_ERROR_STATE_IND_ACTIVE,
    .b.data_length = HAL_FDCAN_DATA_LEN_CAN_FDCAN_0_BYTE,
    .b.bit_rate_switch = HAL_FDCAN_BIT_RATE_SWITCH_ON,
    /* @user: change to HAL_FDCAN_FRAME_FORMAT_CAN to send in classical CAN format.*/
    .b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_FD_CAN,
    .b.event_fifo_control = HAL_FDCAN_TX_EVENTS_FIFO_STORE,
    .b.message_marker = 52U,
  };

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 2 ##########
      * Receives in polling mode a buffer using FDCAN.
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
      /* An error occurs during the FDCAN read transfer. Report it, then restart the communication */
      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status  = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Transmits in polling mode the previous FDCAN frame (DATA and DLC).
      */
    tx_element_header.b.data_length = rx_element_header.b.data_length;
    hal_status = HAL_FDCAN_ReqTransmitMsgFromFIFOQ(pFDCAN, &tx_element_header, RxBuffer);
    if (hal_status != HAL_OK)
    {
      /* An error occurs during the FDCAN write transfer. Report it, then restart the communication */
      fdcan_error_code = HAL_FDCAN_GetLastErrorCodes(pFDCAN);
      return_status  = HandleTransferError(hal_status, fdcan_error_code);
      continue;
    }

    PRINTF("[INFO] Responder - Message received and sent back\n");
    return_status = EXEC_STATUS_OK;

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


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the FDCAN TX/RX operations.
  * param FDCAN_error_code:  FDCAN Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t fdcan_error_code)
{
  PRINTF("[ERROR] Responder - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_FDCAN_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, fdcan_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(FDCAN_COM_TIMEOUT);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
