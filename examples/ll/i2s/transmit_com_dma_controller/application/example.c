/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to manage an I2S transmission in DMA mode as controller using the LL APIs.
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
#include "audio_sample.h" /* audio sample data */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TxTransferComplete = 0U; /* Transmission complete flag */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError      = 0U; /* Transfer error flag        */
#endif /* USE_LL_APP_ERROR */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_I2Sx instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i2sx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  /** ########## Step 2 ##########
    * Initiates the MX_I2Sx transmission of the audio sample data buffer in DMA mode.
    */
  I2S_Transmit_DMA(AUDIO_SAMPLE, sizeof(AUDIO_SAMPLE));


  /** ########## Step 3 ##########
    * Waits for one of these MX_I2Sx interrupts: transfer complete or transfer error when USE_LL_APP_ERROR == 1.
    */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((TxTransferComplete == 0U) && (TransferError == 0U))
#else
  while (TxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. A MX_I2Sx interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (TransferError != 0U)
  {
    return EXEC_STATUS_ERROR;
  }
#endif /* USE_LL_APP_ERROR */

  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the MX_I2Sx instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_i2sx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** This function is executed when the Tx transfer complete interrupt is generated.
  * @user: the Tx transfer callback can be customized according to the application needs.
  */
void I2S_TxTransferCpltCallback(void)
{
  TxTransferComplete = 1U;
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** This function is executed when the transfer error interrupt is generated.
  * @user: the error transfer callback can be customized according to the application needs.
  */
void I2S_TransferErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */
