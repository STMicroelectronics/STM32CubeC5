/**
  ******************************************************************************
  * file           : example.c
  * brief          : The controller's application handling a data transfer over
  *                  I3C in a non-blocking mode using DMA and HAL API.
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
/* Define Target Address */
#define DEVICE_TARGET_ADDR 0x3FU

/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U

/* @user: set the payload size in bytes. The I3C message is a string made of this payload and terminated by '\0'. */
#define I3C_PAYLOAD_SIZE 40U
/**
  * Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted I3C message is a null-terminated string.
  */
#define BUFFER_SIZE (I3C_PAYLOAD_SIZE + 1U)

#define DMA_ALIGNMENT 64U /* user define to adjust the data alignment to the DMA IP of the SoC */

#define COUNTOF(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C; /* pointer referencing the I3C handle from the generated code */

/* Context buffer related to Frame context, contain different buffer value for a communication */
hal_i3c_transfer_ctx_t ContextBuffers;

volatile uint8_t TransferComplete = 0U; /* Set to 1 if the transfer is correctly completed */
volatile uint8_t TransferError = 0U;    /* Set to 1 if a transmission or a reception error is detected */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update I3C_PAYLOAD_SIZE if necessary.
  */
const uint8_t BufferA[BUFFER_SIZE] = "I2C Two Boards Communication - Message A";

const uint8_t BufferB[BUFFER_SIZE] = "I2C Two Boards Communication - Message B";

/** Reception buffer for CPU and DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  */
/* Buffer used for reception */
__attribute__((section("non_cacheable_area"), aligned(DMA_ALIGNMENT)))
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Buffer for transfer context */
__attribute__((section("non_cacheable_area"), aligned(DMA_ALIGNMENT)))
uint32_t aControlBuffer[20] = {0U};

/* Descriptor for private data transmit */
hal_i3c_private_desc_t PrivateDescriptor[2] =
{
  {DEVICE_TARGET_ADDR, BUFFER_SIZE, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, BUFFER_SIZE, HAL_I3C_DIRECTION_READ}
};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlXferCpltCallback(hal_i3c_handle_t *pI3C);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);

/** ########## Step 1 ##########
  * The init of the I3C and DMA instances are triggered by the applicative code.
  * The user callbacks for I3C master TX/RX transfer completed and transfer error are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status; /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code; /* Memorizes the I3C error code limited to the last process */

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the I3C Controller callbacks */
  if (HAL_I3C_CTRL_RegisterTransferCpltCallback(pI3C, CtrlXferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  hal_status = HAL_I3C_CTRL_ResetTransferCtx(&ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the reset a transfer. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  hal_status = HAL_I3C_CTRL_InitTransferCtxTc(
                 &ContextBuffers, aControlBuffer,
                 HAL_I3C_GET_CTRL_BUFFER_SIZE_WORD(COUNTOF(PrivateDescriptor), HAL_I2C_PRIVATE_WITHOUT_ARB_STOP));

  if (hal_status != HAL_OK)
  {
    /* An error occurs at the transfer context */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  hal_status = HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, BufferB, BUFFER_SIZE);

  if (hal_status != HAL_OK)
  {
    /* An error occurs at the initialise the transfer context with buffer A */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  hal_status = HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, BUFFER_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the initialise the transfer context with RxBuffer */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  hal_status = HAL_I3C_CTRL_BuildTransferCtxPrivate(&ContextBuffers, PrivateDescriptor, COUNTOF(PrivateDescriptor),
                                                    HAL_I2C_PRIVATE_WITHOUT_ARB_STOP);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the build a transfer */
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
  hal_status_t hal_status;    /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t com_attempts = 0U; /* Memorizes the number of attempts made to exchange a message between the boards */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the I3C interrupts flags before starting a new transfer */
    TransferComplete = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 2 ##########
      * Transmits in DMA mode the buffer A or buffer B, which contains a null-terminated string,
      * and receives another null-terminated string buffer, both using I3C in DMA mode.
      */
    hal_status = HAL_I3C_CTRL_Transfer_DMA(pI3C, &ContextBuffers);

    if (hal_status != HAL_OK)
    {
      /* The write and read transfer is not correctly completed. Report the error, then restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Waits for one of these I3C interrupts: write transfer complete or transfer error.
      */
    while ((TransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* The write and read transfer is not correctly completed. Report the error, then restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/** Master transmission end of transfer callback
  * @user This implementation of the I3C master transfer complete callback can be customized.
  * This function is executed if the master transfer complete interrupt is generated.
  */
static void CtrlXferCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to step 3 */
  TransferComplete = 1U;
}

/** I3C transfer error callback
  * @user This implementation of the I3C master transfer error callback can be customized.
  * This function is executed if the master transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to step 3 and 5 */
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

  return_status = EXEC_STATUS_OK;
  return return_status;
} /* end HandleTransferCplt */

/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I2C TX/RX operations.
  * param i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n",
         hal_status, i3c_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
