/**
  ******************************************************************************
  * file           : example.c
  * brief          : A half-duplex synchronous SPI communication as controller,
  *                  in polling mode, using the HAL API.
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
#define TIMEOUT 1000U
/* @user: set the payload size in bytes. The SPI message is a string made of this payload and terminated by '\0'. */
#define SPI_PAYLOAD_SIZE 52U
/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted SPI message is a null-terminated string.
  */
#define BUFFER_SIZE (SPI_PAYLOAD_SIZE + 1U)

/** A delay is needed to ensure that the controller initiates the communication when the responder is ready.
  * @user: set the delay in milliseconds. It must be adapt to the application.
  */
#define SPI_SYNC_DELAY  50U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_spi_handle_t *pSPI; /* Pointer referencing the SPI handle from the generated code */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update SPI_PAYLOAD_SIZE if necessary.
  */
static const uint8_t BufferA[BUFFER_SIZE] = "SPI Half Duplex Two Boards Communication - Message A";
static const uint8_t BufferB[BUFFER_SIZE] = "SPI Half Duplex Two Boards Communication - Message B";
/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;
/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code);

/** ########## Step 1 ##########
  * The init of the SPI instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pSPI = mx_example_spi_init();

  if (pSPI != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the SPI TX/RX operations */
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  uint32_t spi_error_code;   /* memorizes the SPI error code limited to the last process */

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;

    /* Additional delay to let time to the responder to be ready to receive */
    HAL_Delay(SPI_SYNC_DELAY);

    /** ########## Step 2 ##########
      * Initiates communication in half duplex blocking mode by first transmitting data
      * with the controller within a specific timeout period.
      */
    hal_status = HAL_SPI_Transmit(pSPI, pTxData, BUFFER_SIZE, TIMEOUT);
    if (hal_status != HAL_OK)
    {
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI);
      return_status  = HandleTransferError(hal_status, spi_error_code);
      continue;
    }

    /* Additional delay to let time to the responder to be ready to transmit */
    HAL_Delay(SPI_SYNC_DELAY);

    /** ########## Step 3 ##########
      * Receives data in polling mode within a specific timeout period.
      */
    hal_status = HAL_SPI_Receive(pSPI, RxBuffer, BUFFER_SIZE, TIMEOUT);
    if (hal_status != HAL_OK)
    {
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI);
      return_status  = HandleTransferError(hal_status, spi_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the SPI before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_spi_deinit();

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

  memset(RxBuffer, 0U, BUFFER_SIZE);
} /* end UpdateBuffers */

/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (memcmp(pTxData, RxBuffer, BUFFER_SIZE) == 0)
  {
    PRINTF("[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of %s \n", pTxData);

    return_status = EXEC_STATUS_OK;
  }
  else
  {
    PRINTF("[ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.\n");
    /* Take the time to check the setup before restarting the communication */
    HAL_Delay(1000U);
  }

  return return_status;
} /* end HandleTransferCplt */

/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the SPI TX/RX operations.
  * param spi_error_code:  SPI Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
