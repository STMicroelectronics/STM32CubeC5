/**
  ******************************************************************************
  * file           : example.c
  * brief          : The USART controller communication side in DMA mode using LL API.
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

/* @user: set the payload size in bytes. The USART message is a string made of this payload and terminated by '\0'. */
#define USART_PAYLOAD_SIZE (42U)

/**
  * Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted USART message is a null-terminated string.
  */
#define BUFFER_SIZE   (USART_PAYLOAD_SIZE + 1U)

#define DMA_ALIGNMENT (64U) /* user define to adjust the data alignment to the DMA IP of the SoC */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed       */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed        */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */
#endif /* USE_LL_APP_ERROR */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the buffers content and length, update BUFFER_SIZE if necessary.
  */
static const uint8_t BufferA[BUFFER_SIZE] = "USART Two Boards Communication - Message A";
static const uint8_t BufferB[BUFFER_SIZE] = "USART Two Boards Communication - Message B";

/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;

/** Reception buffer for CPU and DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  */
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */


/** ########## Step 1 ##########
  * The init of the MX_USARTx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_usartx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts      = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the MX_USARTx interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

    /* Start a new communication attempt */
    com_attempts ++;


    /** ########## Step 2 ##########
      * Transmits in DMA mode a buffer pointed by pTxData using MX_USARTx.
      */
    USART_Transmit_DMA(pTxData, BUFFER_SIZE);


    /** ########## Step 3 ##########
      * Waits for one of these MX_USARTx interrupts: write transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((TxTransferComplete == 0) && (TransferError == 0))
#else
    while (TxTransferComplete == 0)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_USARTx interrupt or a SystTick interrupt can wake up the CPU.
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

    /* Add a delay before the receiving communication */
    DelayMs(10U);


    /** ########## Step 4 ##########
      * Receives in DMA mode a buffer using MX_USARTx.
      */
    USART_Receive_DMA(RxBuffer, BUFFER_SIZE);


    /** ########## Step 5 ##########
      * Waits for one of these MX_USARTx interrupts: read transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((RxTransferComplete == 0) && (TransferError == 0))
#else
    while (RxTransferComplete == 0)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_USARTx interrupt or a SystTick interrupt can wake up the CPU.
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

    /** ########## Step 6 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();
  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the MX_USARTx instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_usartx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Transmission end of transfer completed callback
  * @user This implementation of the MX_USARTx Tx transfer complete callback can be customized.
  * This function is executed when the Tx transfer complete interrupt is generated.
  */
void USART_TxTransferCpltCallback(void)
{
  TxTransferComplete = 1U;
}


/** Reception end of transfer completed callback
  * @user This implementation of the MX_USARTx Rx transfer complete callback can be customized.
  * This function is executed when the Rx transfer complete interrupt is generated.
  */
void USART_RxTransferCpltCallback(void)
{
  RxTransferComplete = 1U;
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** MX_USARTx transfer error callback
  * @user This implementation of the MX_USARTx transfer error callback can be customized.
  * This function is executed when the transfer error interrupt is generated.
  */
void USART_TransferErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */

/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
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

/** brief:  This function is executed when the data transfer is completed.
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
