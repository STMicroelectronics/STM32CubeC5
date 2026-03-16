/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C private IT target example with LL API
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
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
#define DEVICE_TARGET_DYNAMIC_ADDR     (0x32U)
#endif /* USE_LL_APP_ERROR */

/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS                (10U)

/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE                     (40U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t DAAProcessComplete; /* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t RxTransferComplete; /* Set to 1 when an RX transfer is successfully completed                  */
volatile uint8_t TxTransferComplete; /* Set to 1 when a TX transfer is successfully completed                   */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;      /* Set to 1 when a transmission or reception error is detected             */
#endif /* USE_LL_APP_ERROR */

uint8_t DAACompleted = 0U;           /* Tracks if DAA process completed for this power cycle                    */

/* RX buffer used for MX_I3Cx reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */


/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts      = 0U; /* Memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;


  /**
    * ########## Step 2 ##########
    * Perform DAA (Dynamic Address Assignment) process and verify dynamic address assignment.
    * This step activates the notification, waits for DAA completion, and checks the assigned address.
    * The DAA process is executed only once after the MX_I3Cx initialization.
  */
  if (DAACompleted == 0U)
  {
    DAAProcessComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

    /* Activate notification for DAA process (Dynamic Address Assignment) */
    I3C_ActivateNotification();

    /* Wait for the DAA process to complete or for an error to occur when USE_LL_APP_ERROR == 1 */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((DAAProcessComplete == 0U) && (TransferError == 0U))
#else
    while (DAAProcessComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if ((TransferError == 1U) || (I3C_GetOwnDynamicAddress() != DEVICE_TARGET_DYNAMIC_ADDR))
    {
      return_status  = HandleTransferError();
      goto _app_process_exit;
    }
#endif /* USE_LL_APP_ERROR */

    /* DAA completed successfully. */
    DAACompleted = 1U;
  }

  if (DAAProcessComplete == 1U)
  {
    /* Clear the MX_I3Cx RX buffer before starting a new transfer */
    memset(RxBuffer, 0U, BUFFER_SIZE);

    while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
    {
      TxTransferComplete = 0U;
      RxTransferComplete = 0U;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      TransferError      = 0U;
#endif /* USE_LL_APP_ERROR */

      com_attempts++;


      /** ########## Step 3 ##########
        * MX_I3Cx receives buffer in interrupt mode.
        */
      I3C_Receive_IT(RxBuffer, BUFFER_SIZE);


      /** ########## Step 4 ##########
        * Waits for one of these interrupts: RX transfer complete or transfer error when USE_LL_APP_ERROR == 1.
        */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      while ((RxTransferComplete == 0U) && (TransferError == 0U))
#else
      while (RxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */

      {
        /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
          * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
          */
        __WFI();
      }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      if (TransferError == 1U)
      {
        /* RX transfer is not correctly completed. Report the error, then restart the communication. */
        return_status = HandleTransferError();
        continue;
      }
#endif /* USE_LL_APP_ERROR */


      /** ########## Step 5 ##########
        * MX_I3Cx transmits the received buffer in interrupt mode.
        */
      I3C_Transmit_IT(RxBuffer, sizeof(RxBuffer));


      /** ########## Step 6 ##########
        * Waits for one of these MX_I3Cx interrupts: TX transfer complete or transfer error when USE_LL_APP_ERROR == 1.
        */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      while ((TxTransferComplete == 0U) && (TransferError == 0U))
#else
      while (TxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
      {
        /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
          * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
          */
        __WFI();
      }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      if (TransferError == 1U)
      {
        /* TX transfer error occurred. Report the error, then restart the communication.. */
        return_status  = HandleTransferError();
        continue;
      }
#endif /* USE_LL_APP_ERROR */

      return_status = HandleTransferCplt();
    } /* end while */
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
_app_process_exit:
#endif /* USE_LL_APP_ERROR */
  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the MX_I3Cx DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
void I3C_DAACompleteCallback(void)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}

/**
  * MX_I3Cx target Rx transfer complete callback.
  * @user This implementation of the MX_I3Cx target transfer complete callback can be customized.
  * This function is executed when the RX transfer is completed by an interrupt.
  */
void I3C_RxCompleteCallback(void)
{
  /* Signal RX transfer completion */
  RxTransferComplete = 1U;
}

/**
  * MX_I3Cx target Tx transfer complete callback.
  * @user This implementation of the MX_I3Cx target transfer complete callback can be customized.
  * This function is executed when the TX transfer is completed by an interrupt.
  */
void I3C_TxCompleteCallback(void)
{
  /* Signal TX transfer completion */
  TxTransferComplete = 1U;
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * MX_I3Cx target transfer error callback.
  * @user This implementation of the MX_I3Cx target transfer error callback can be customized.
  * This function is executed when a transfer error interrupt occurs.
  */
void I3C_ErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */

/** brief:  Checks the correctness of the data when the transfer is completed.
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  return EXEC_STATUS_OK;
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
