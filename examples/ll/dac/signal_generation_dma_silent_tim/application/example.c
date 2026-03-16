/**
  ******************************************************************************
  * file           : example.c
  * brief          : DAC signal generation using DMA in silent mode and timer as trigger with LL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** Buffer size for sine complete period wave values
  * @user: The buffer size can be adjusted to modify the complete period wave samples number
  */
#define BUFFER_SIZE     (60U)

/** Amplitude for the generated output signal in millivolts
  * @user: The amplitude can be adjusted to modify the output signal
  */
#define SINE_AMPLITUDE  (DAC_VDDA_VALUE / 2)

/** Offset for the generated output signal in millivolts
  * @user: The offset can be adjuste to modify the output signal
  */
#define SINE_OFFSET     (DAC_VDDA_VALUE / 4)

/** The DAC must handle the maximum voltage of the generated output signal.
  * Both offset and amplitude values must be positive.
  */
#if (((SINE_AMPLITUDE + SINE_OFFSET) > DAC_VDDA_VALUE) || (SINE_AMPLITUDE < 0) || (SINE_OFFSET < 0))
#error Amplitude and Offset for the signal do not match DAC output range
#endif /* ((SINE_AMPLITUDE + SINE_OFFSET) > DAC_VDDA_VALUE) || (SINE_AMPLITUDE < 0) || (SINE_OFFSET < 0) */

/* Private variables ---------------------------------------------------------*/
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
  * The init of the MX_DACx in DMA mode and MX_TIMx instances are triggered by the applicative code.
  * - MX_DACx is configured.
  * - MX_DACx channel buffer is calibrated.
  * - MX_TIMx is configured.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_dacx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* The calibration allows a better output voltage precision */
  DAC_CalibrateChannelBuffer();

  if (mx_timx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /** ########## Step 2 ##########
    * Converts the data from millivolt to raw data because DAC cannot handle directly millivolt data but only raw data
    * between 0 and the maximale DAC resolution.
    */
  for (uint32_t i = 0U; i < BUFFER_SIZE; i++)
  {
    /* Conversion from unitary sine signal into sine signal in millivolt, with a desired offset and amplitude */
    uint32_t sine_voltage_value_mv = SINE_OFFSET + (SineWaveUnitary[i] * SINE_AMPLITUDE) / 1000U;

    SineWave[i] = DAC_GetVoltageRawData(sine_voltage_value_mv);
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 3 ##########
  * The MX_DACx is started using the DMA feature in silent mode and the TIM is started to trigger the DAC.
  */
app_status_t app_process(void)
{
  /** Enable the MX_DACx channel using DMA transfer in silent mode.
    * The size of the buffer to be transferred through DMA is in bytes so the type of variable and the number of
    * elements in the buffer must be taken into account.
    */
  DAC_StartChannel_DMA_Silent(SineWave, (BUFFER_SIZE * sizeof(*SineWave)));

  TIM_Start();

  return EXEC_STATUS_OK;
} /* end app_process */


/**
  * Deinitializes the MX_DACx and the MX_TIMx instances.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_dacx_deinit();

  mx_timx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
