/**
  ******************************************************************************
  * file           : example.c
  * brief          : An I2S transmission as controller, in DMA mode, using HAL
  *                  APIs.
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
#include "audio_sample.h"   /* audio sample data */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i2s_handle_t *pI2S;  /* Pointer to the I2S handle defined in the MX-generated code */
volatile uint8_t TxTransferComplete = 0U; /* Transmission complete flag */
volatile uint8_t TransferError = 0U; /* Transfer error flag */

/* Private functions prototype -----------------------------------------------*/
static  void TxTransferCpltCallback(hal_i2s_handle_t *pI2S);
static  void TransferErrorCallback(hal_i2s_handle_t *pI2S);

/** ########## Step 1 ##########
  * The application code initializes the I2S instance and registers the user callbacks.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI2S = mx_i2sx_init();

  if (pI2S == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the I2S callbacks */
  if (HAL_I2S_RegisterTxCpltCallback(pI2S, TxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I2S_RegisterErrorCallback(pI2S, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  hal_status_t hal_status;   /* memorizes the HAL status of the I2S operations */

  /** ########## Step 2 ##########
    * Initiates the transmission of the audio sample data buffer in DMA mode.
    */
  hal_status = HAL_I2S_Transmit_DMA(pI2S, AUDIO_SAMPLE, sizeof(AUDIO_SAMPLE));

  if (hal_status != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: Transmission error\n");
    return_status =  EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }
  else
  {
    PRINTF("[INFO] Step 2: Transmission COMPLETED.\n");
    return_status = EXEC_STATUS_OK;
  }

  /** ########## Step 3 ##########
    * Waits for one of these I2S interrupts: transfer complete or transfer error.
    */
  while ((TxTransferComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. A I2S interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError != 0U)
  {
    PRINTF("[ERROR] Step 3: Transfer error detected\n");
    return_status = EXEC_STATUS_ERROR;
  }
_app_process_exit:
  return return_status;
} /* end app_process */


/** brief:  De-initializes the SPI instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_i2sx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Transfer complete callback.
  * param:  pI2S pointer to a I2S handle structure
  */
static void TxTransferCpltCallback(hal_i2s_handle_t *pI2S)
{
  TxTransferComplete = 1U;
}

/** brief:  Transfer error callback. This function is called if the I2S transfer error interrupt is generated.
  * param:  pI2S pointer to a I2S handle structure
  */
static void TransferErrorCallback(hal_i2s_handle_t *pI2S)
{
  TransferError = 1U;
}
