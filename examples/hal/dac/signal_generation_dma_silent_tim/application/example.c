/**
  ******************************************************************************
  * file           : example.c
  * brief          : DAC signal generation using DMA and timer as trigger
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
/* Private macro -------------------------------------------------------------*/
/** VDDA must be computed not static.
  * @user: The Analog Voltage supply, VDDA, might be different from VDD and might require to be tuned.
  */
#define VDDA_APPLI_MILLIVOLT  3300UL

#define BUFFER_SIZE           60U

/** Amplitude for the generated output signal in millivolts
  * @user: The amplitude can be adjusted to modify the output signal
  */
#define SINE_AMPLITUDE  (VDDA_APPLI_MILLIVOLT / 2)

/** Offset for the generated output signal in millivolts
  * @user: The offset can be adjuste to modify the output signal
  */
#define SINE_OFFSET     (VDDA_APPLI_MILLIVOLT / 4)


/** The DAC must handle the maximum voltage of the generated output signal.
  * Both offset and amplitude values must be positive.
  */
#if (((SINE_AMPLITUDE + SINE_OFFSET) > VDDA_APPLI_MILLIVOLT) || (SINE_AMPLITUDE < 0) || (SINE_OFFSET < 0))
#error Amplitude and Offset for the signal do not match DAC output range
#endif /* ((SINE_AMPLITUDE + SINE_OFFSET) > VDDA_APPLI_MILLIVOLT) || (SINE_AMPLITUDE < 0) || (SINE_OFFSET < 0) */

/* Private variables ---------------------------------------------------------*/

hal_dac_handle_t *pDAC;   /* pointer referencing the DAC handle from the generated code */
hal_dma_handle_t *pDMA;   /* pointer referencing the DMA handle from the generated code */
hal_tim_handle_t *pTIM;   /* pointer referencing the TIM handle from the generated code */

/* Sine wave values for a complete period for an unitary sine between 0 and 1000 (no units) */
const uint16_t SineWaveUnitary[BUFFER_SIZE] =
{
  500, 552, 604, 655, 703, 750, 794, 835, 872, 905, 933, 957, 976, 989, 997, 1000, 997, 989, 976, 957, 933, 905, 872,
  835, 794, 750, 703, 655, 604, 552, 500, 448, 396, 345, 297, 250, 206, 165, 128, 95, 67, 43, 24, 11, 3, 0, 3, 11, 24,
  43, 67, 95, 128, 165, 206, 250, 297, 345, 396, 448
};

/* Buffer to store converted raw data to be transferred through DMA */
uint16_t SineWave[BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the DAC, DMA and TIM instances are triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDAC = mx_example_dac_init();
  if (pDAC == NULL)
  {
    goto _app_init_exit;
  }

  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  /* The calibration allows a better output voltage precision */
  if (HAL_DAC_CalibrateChannelBuffer(pDAC, MX_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }

  for (uint32_t i = 0; i < BUFFER_SIZE; i++)
  {
    /* Conversion from unitary sine signal into sine signal in millivolt, with a desired offset and amplitude */
    uint32_t sine_voltage_value_mv = SINE_OFFSET + (SineWaveUnitary[i] * SINE_AMPLITUDE) / 1000;

    /** ########## Step 2 ##########
      * Converts the data from millivolt to raw data because DAC cannot handle directly millivolt data but only raw data
      * between 0 and the maximale DAC resolution (LL_DAC_DIGITAL_SCALE(LL_DAC_RESOLUTION_12B) = 4095)
      */
    SineWave[i] = LL_DAC_CALC_VOLTAGE_TO_DATA(VDDA_APPLI_MILLIVOLT, sine_voltage_value_mv, LL_DAC_RESOLUTION_12B);
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 3 ##########
  * The DAC is started using the DMA feature and the TIM is started to trigger the DAC.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** Enable the DAC channel using DMA transfer
    * The size of the buffer to be transferred through DMA is in bytes so the type of variable and the number of
    * elements in the buffer must be taken into account.
    */
  if (HAL_DAC_StartChannel_DMA(pDAC, MX_DAC_CHANNEL, SineWave, BUFFER_SIZE * sizeof(*SineWave)) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the DAC, DMA and TIM instances.
  */
app_status_t app_deinit(void)
{
  mx_example_dac_deinit();
  mx_example_tim_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */
