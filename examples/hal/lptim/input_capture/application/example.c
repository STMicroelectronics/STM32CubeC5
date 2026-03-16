/**
  ******************************************************************************
  * file           : example.c
  * brief          : LPTIM Input Capture Mode with HAL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_TOLERANCE           1.10
#define MIN_TOLERANCE           0.90
#define INPUT_SIGNAL_FREQ         100     /* 100Hz */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_lptim_handle_t *pLPTIM;  /* pointer referencing the LPTIM handle from the generated code */

uint16_t        MeasuredInputSignalCounter;    /* Counter measured from the input signal */
uint16_t        ExpectedInputSignalCounter;    /* Expected counter value for the input signal */
uint16_t        DmaCapturedValue[2] = {0};    /* Buffer to store captured values from the input signal using DMA */

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The applicative code initializes the LPTIM instance in Input Capture Mode,
      with an external signal connected to LPTIM Channel.
    */

  pLPTIM = mx_example_lptim_init();
  if (pLPTIM == NULL)
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * The DMA starts to transfer data, without any optional interrupt.
    *   - In particular half-complete interrupt, to avoid wakeup before the end of the transfer.
    * Then the device goes in stop mode
    */

  PRINTF("[INFO] Step 2: The device goes in stop mode, waiting for wakeup from the DMA complete transfer.\n");

  if (HAL_LPTIM_IC_StartChannel_DMA_Opt(pLPTIM, MX_LPTIM_CHANNEL, (uint8_t *)DmaCapturedValue, 4,
                                        HAL_DMA_OPT_IT_NONE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_LPTIM_Start(pLPTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Suspend Tick */
  HAL_SuspendTick();

  /* Enter in Stop mode */
  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_PWR_STOPy_MODE);

  /* Re-configure system after wake-up from stop mode */
  HAL_ResumeTick();
  mx_example_rcc_init();


  /** ########## Step 3 ##########
    * Upon completion of the transfer, the DMA generates an interrupt to wake up the MCU.
    * Calculates the expected and measured input signal counter, and turns the status LED ON if they match.
    */

  PRINTF("[INFO] Step 3: The transfer completes and the DMA generates an interrupt to wake up the MCU.\n");

  /* Compute expected tick value */
  MeasuredInputSignalCounter = DmaCapturedValue[1] - DmaCapturedValue[0];
  ExpectedInputSignalCounter = LSE_VALUE / INPUT_SIGNAL_FREQ;

  /* Get the Input Capture value */
  if (DmaCapturedValue[1] == DmaCapturedValue[0])
  {
    /* If capture values are equal, we have reached the limit of frequency measures */
    PRINTF("[ERROR] Step 3: Measure Frequency ERROR.\n");
    goto _app_process_exit;
  }

  if ((MeasuredInputSignalCounter  < (ExpectedInputSignalCounter * MAX_TOLERANCE))
      && (MeasuredInputSignalCounter  > (ExpectedInputSignalCounter * MIN_TOLERANCE)))
  {
    PRINTF("[INFO] Step 3: The expected and measured input signal frequencies match.\n");
    return_status = EXEC_STATUS_OK;
    goto _app_process_exit;
  }

  HAL_LPTIM_IC_StopChannel_DMA(pLPTIM, MX_LPTIM_CHANNEL);

_app_process_exit:

  return return_status;
} /* end app_process */


/** De-initializes the LPTIM instance before leaving the scenario. */
app_status_t app_deinit(void)
{

  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
