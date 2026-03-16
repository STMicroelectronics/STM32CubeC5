/**
  ******************************************************************************
  * file           : example.c
  * brief          : A full-duplex synchronous SPI communication as responder,
  *                  in interrupt mode, using the LL API.
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS   (10U)
/* @user: set the payload size in bytes. The SPI message is a string made of this payload and terminated by '\0'. */
#define SPI_PAYLOAD_SIZE   (52U)

/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted SPI message is a null-terminated string.
  */
#define BUFFER_SIZE (SPI_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TxRxTransferComplete; /* Set to 1 if the write-read transfer is correctly completed */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError; /* Set to 1 if a transmission or a reception error is detected */
#endif /* USE_LL_APP_ERROR */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update SPI_PAYLOAD_SIZE if necessary.
  */
static const uint8_t BufferA[BUFFER_SIZE] = "SPI Full Duplex Two Boards Communication - Message A";
static const uint8_t BufferB[BUFFER_SIZE] = "SPI Full Duplex Two Boards Communication - Message B";
/* Pointer to the buffer used for transmission */
const uint8_t *pTxData;
/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the SPI instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_spix_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the SPI interrupts flags before starting a new transfer */
    TxRxTransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Initiates the communication with the controller in a full duplex mode, using interrupts, by sending and
      * receiving data simultaneously.
      * The SPI messages exchanged are null-terminated strings.
      */
    SPI_TransmitReceive_IT((uint8_t *)pTxData, RxBuffer, BUFFER_SIZE);

    /** ########## Step 3 ##########
      * Waits for one of these SPI interrupts: write-read transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((TxRxTransferComplete == 0) && (TransferError == 0))
#else
    while (TxRxTransferComplete == 0)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. A SPI interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1)
    {
      /* The write-read transfer is not correctly completed. Report the error, then restart the communication */
      return_status = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the SPI instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_spix_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** SPI TX/RX Transfer complete callback
  * user: This implementation of the SPI TX/RX transfer complete callback can be customized.
  * This function is executed if the SPI Tx/Rx transfer complete interrupt is generated.
  */
void SPI_TransferCpltCallback(void)
{
  /* Asynchronous processing related to step 3 */
  TxRxTransferComplete = 1U;
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** SPI transfer error callback
  * user: This implementation of the SPI transfer error callback can be customized.
  * This function is executed if the SPI transfer error interrupt is generated.
  */
void SPI_TransferErrorCallback(void)
{
  /* Asynchronous processing related to step 3 */
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */


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
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end HandleTransferCplt */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  This function is executed in case of a data transfer error.
  *  user:  - This function implementation only illustrates error processing.
  *         - It can be customized to match the application recovery strategy.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  /* Take the time to check the setup before restarting the communication */
  DelayMs(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
