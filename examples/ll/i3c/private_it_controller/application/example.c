/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C private IT controller example with LL API
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
/* @user Target device information for MX_I3Cx communication: update target information as needed for your hardware */
#define DEVICE_TARGET_NAME         "Device target"
#define DEVICE_TARGET_ID           (0U)
#define DEVICE_TARGET_STATIC_ADDR  (0U)
#define DEVICE_TARGET_DYNAMIC_ADDR (0x32U)

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define MAX_DAA_ATTEMPTS           (10U)

/* @user Set the maximum number of attempts to complete a data transfer between the two boards. */
#define MAX_COM_ATTEMPTS           (10U)

/** @user Buffer size in bytes:
  *       update buffer size as needed by your application.
  * @Note This example uses 32-bit (word) accesses in the TX/RX FIFO treatments, so BUFFER_SIZE must be
  *       a multiple of 4.
  */
#define BUFFER_SIZE                (40U)

/* @user MX_I3Cx descriptor operation number: update operation number to add or remove operations from
                                              PrivateDescriptor                                        */
#define I3C_DESC_OPERATION_NUM     (2U)

/* MX_I3Cx transfer direction values for private operations */
#define I3C_DIRECTION_WRITE        (0U)
#define I3C_DIRECTION_READ         (0x10000UL)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when a data transfer operation is complete */
volatile uint8_t TransferComplete = 0U;

/* Set to 1 when a DAA attempt has failed */
volatile uint8_t DAAFailedAttent = 0U;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

uint8_t DAACompleted = 0U;           /* Tracks if DAA process completed for this power cycle */

/** TxBufferA, TxBufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the payload content and length, update I3C_PAYLOAD_SIZE if necessary.
  */
const uint8_t TxBufferA[BUFFER_SIZE] = "I3C Two Boards Communication - Message A";
const uint8_t TxBufferB[BUFFER_SIZE] = "I3C Two Boards Communication - Message B";

/* RX buffer used for MX_I3Cx reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE];

/* Buffer for control transfer context */
uint32_t ControlBuffer[CONTROL_BUFFER_SIZE];

/**
  * Descriptor array for private MX_I3Cx write/read operations.
  * Each entry defines a single private transfer frame for the MX_I3Cx controller:
  *   {target_addr, data_size_bytes, direction}
  */
i3c_private_desc_t PrivateDescriptor[I3C_DESC_OPERATION_NUM] =
{
  {DEVICE_TARGET_DYNAMIC_ADDR, BUFFER_SIZE, I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_DYNAMIC_ADDR, BUFFER_SIZE, I3C_DIRECTION_READ}
};

/* Pointer to the TX buffer used for MX_I3Cx transmission */
const uint8_t *pTxData;

/* Private functions prototype -----------------------------------------------*/
static void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized and the transfer context is configured by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Build the private transfer context using the provided descriptors and options. */
  I3C_BuildPrivateTransferContext(ControlBuffer, PrivateDescriptor);

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t     daa_attempts = 0U; /* Memorizes the number of initial attempts for Dynamic Address Assignment (DAA)  */
  uint32_t     com_attempts = 0U; /* Memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Initiates and manages the Dynamic Address Assignment (DAA) process for the controller.
    * The DAA process is executed only once after the MX_I3Cx initialization.
    * Warning: if the target MX_I3Cx peripheral is enabled before the controller starts the first ENTDAA,
    * the first DAA attempt can fail on some setups.
    */
  if (DAACompleted == 0U)
  {
    while (daa_attempts < MAX_DAA_ATTEMPTS)
    {
      DAAProcessComplete = 0U;
      DAAFailedAttent    = 0U;

      daa_attempts++;

      /* Initiate Dynamic Address Assignment (DAA) process for the controller */
      I3C_AssignDynamicAddr_IT();

      /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
      while ((DAAProcessComplete == 0U) && (DAAFailedAttent == 0U))
      {
        /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
          * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
          */
        __WFI();
      }

      if ((DAAFailedAttent == 1U) && (daa_attempts != 0U))
      {
        /* DAA attempt aborted due to an error; retry. */
        continue;
      }
      else
      {
        /* DAA completed successfully. */
        DAACompleted = 1U;
        break;
      }
    }
  }

  if (DAAProcessComplete == 1U)
  {
    UpdateBuffers();

    while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
    {
      TransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

      com_attempts ++;


      /** ########## Step 3 ##########
        * Transmits in interrupt mode the buffer A or buffer B (fixed-size payload),
        * and receives a fixed-size payload buffer.
        */
      I3C_Transfer_IT(ControlBuffer, CONTROL_BUFFER_SIZE, pTxData, BUFFER_SIZE, RxBuffer, BUFFER_SIZE);


      /** ########## Step 4 ##########
        * Waits for one of these MX_I3Cx interrupts: transfer complete or transfer error when USE_LL_APP_ERROR == 1.
        */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      while ((TransferComplete == 0U) && (TransferError == 0U))
#else
      while (TransferComplete == 0U)
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
        /* An error occurs during the write/read transfer. Report the error and restart the communication attempt. */
        return_status = HandleTransferError();
        continue;
      }
#endif /* USE_LL_APP_ERROR */


      /** ########## Step 5 ##########
        * Checks the correctness of the received buffer after the data transfer is completed.
        */
      return_status = HandleTransferCplt();
    }  /* end while */
  }

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
  * Target requests dynamic address callback
  * @user This implementation of the MX_I3Cx target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
void I3C_TargetReqDynamicAddrCallback(void)
{
  /* Assign the dynamic address to the target device */
  I3C_SetDynamicAddr(DEVICE_TARGET_DYNAMIC_ADDR);
}

/** Controller transmission end of transfer callback
  * @user This implementation of the MX_I3Cx controller transfer complete callback can be customized.
  * This function is executed when the controller transfer complete interrupt is generated.
  */
void I3C_TransferCompleteCallback(void)
{
  /* Asynchronous processing for transfer completion (Step 3) */
  TransferComplete = 1U;
}

/**
  * MX_I3Cx transfer error callback
  * @user This implementation of the MX_I3Cx controller transfer error callback can be customized.
  * This function is executed when the controller transfer error interrupt is generated.
  */
void I3C_ErrorCallback(void)
{
  if (DAACompleted == 0U)
  {
    DAAFailedAttent = 1U;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  TransferError = 1U;
#endif /* USE_LL_APP_ERROR */
}

/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between BufferA and BufferB to select the message to send */
  if (pTxData == TxBufferA)
  {
    pTxData = TxBufferB;
  }
  else
  {
    pTxData = TxBufferA;
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
