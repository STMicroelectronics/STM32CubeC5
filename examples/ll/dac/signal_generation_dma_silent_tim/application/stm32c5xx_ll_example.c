/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : DAC signal generation using DMA in silent mode and timer as trigger with LL API
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DELAY_TRIM_US    (50UL)  /*!< Delay of 50 us for DAC minimum trimming time */
#define DAC_DELAY_STARTUP_US (15UL)  /*!< Delay of 15 us for DAC channel voltage settling time from DAC channel
                                          startup (transition from disable to enable) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void DelayUs(uint32_t delay_us);

/* Private functions ---------------------------------------------------------*/

/**
  *  Calibrate the MX_DACx channel buffer.
  */
inline void DAC_CalibrateChannelBuffer(void)
{
  uint32_t output_buffer_mode;
  uint32_t trimming_value;
  uint32_t delta;
  uint32_t flag;

  /* Store output buffer configuration */
  output_buffer_mode = LL_DAC_GetOutputBuffer(MX_DACx, MX_DAC_CHANNEL);

  LL_DAC_Disable(MX_DACx, MX_DAC_CHANNEL);

  /* Set mode in MCR for calibration */
  LL_DAC_SetOutputBuffer(MX_DACx, MX_DAC_CHANNEL, LL_DAC_OUTPUT_BUFFER_ENABLE);

  /* Enable the selected DAC channel calibration */
  LL_DAC_SetMode(MX_DACx, MX_DAC_CHANNEL, LL_DAC_MODE_CALIBRATION);

  /* Init trimming counter */
  /* Medium value */
  trimming_value = 0x10UL;
  delta = 0x08UL;

  while (delta != 0UL)
  {
    /* Set candidate trimming */
    LL_DAC_SetTrimmingValue(MX_DACx, MX_DAC_CHANNEL, (trimming_value & DAC_CCR_OTRIM1));

    /* Wait minimum time needed between two calibration steps (OTRIM) */
    DelayUs(DAC_DELAY_TRIM_US);

    flag = LL_DAC_IsActiveFlag_CAL(MX_DACx, MX_DAC_CHANNEL);
    if (flag == 1U)
    {
      /* DAC_SR_CAL_FLAGx is HIGH try lower trimming */
      trimming_value -= delta;
    }
    else
    {
      /* DAC_SR_CAL_FLAGx is LOW try higher trimming */
      trimming_value += delta;
    }

    delta >>= 1UL;
  }

  /* Still need to check if  trimming_value calibration is current value or one step below */
  /* Indeed the first value that causes the DAC_SR_CAL_FLAGx bit to change from 0 to 1  */
  /* Set candidate trimming */
  LL_DAC_SetTrimmingValue(MX_DACx, MX_DAC_CHANNEL, (trimming_value & DAC_CCR_OTRIM1));

  /* Wait minimum time needed between two calibration steps (OTRIM) */
  DelayUs(DAC_DELAY_TRIM_US);

  flag = LL_DAC_IsActiveFlag_CAL(MX_DACx, MX_DAC_CHANNEL);

  if (flag == 0UL)
  {
    /* Check trimming value below maximum */
    if (trimming_value < DAC_CCR_OTRIM1)
    {
      /* Trimming is actually one value more */
      trimming_value++;
    }

    /* Set right trimming */
    LL_DAC_SetTrimmingValue(MX_DACx, MX_DAC_CHANNEL, (trimming_value & DAC_CCR_OTRIM1));
  }

  /* Disable the DAC channel calibration */
  LL_DAC_SetMode(MX_DACx, MX_DAC_CHANNEL, LL_DAC_MODE_NORMAL_OPERATION);

  /* Restore configuration */
  LL_DAC_SetOutputBuffer(MX_DACx, MX_DAC_CHANNEL, output_buffer_mode);
}


/**
  *  Get the MX_DACx raw data values of voltages in mV.
  *  uint16_t raw data value
  */
inline uint16_t DAC_GetVoltageRawData(uint32_t dac_voltage_value)
{
  return (LL_DAC_CALC_VOLTAGE_TO_DATA(DAC_VDDA_VALUE, dac_voltage_value, LL_DAC_RESOLUTION_12B));
}


/**
  *  Start the MX_DACx channel in DMA silent mode.
  */
inline void DAC_StartChannel_DMA_Silent(const void *p_data, uint32_t size_byte)
{
  uint32_t *dacx_chy_dma_lli_reg = mx_dmax_chy_dma_get_lli_reg();

  LL_DAC_EnableDMAReq(MX_DACx, MX_DAC_CHANNEL);

  /* Update the DMA channel LLi fields for DAC transfer */
  dacx_chy_dma_lli_reg[LL_DMA_NODE_CBR1_REG_OFFSET] = size_byte;
  dacx_chy_dma_lli_reg[LL_DMA_NODE_CSAR_REG_OFFSET] = (uint32_t)p_data;
  dacx_chy_dma_lli_reg[LL_DMA_NODE_CDAR_REG_OFFSET] = LL_DAC_DMA_GetRegAddr(MX_DACx,
                                                                            MX_DAC_CHANNEL,
                                                                            LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED);

  LL_DMA_ClearFlag(MX_DMAX_CHY_CH, LL_DMA_FLAG_ALL);

  LL_DMA_DisableIT(MX_DMAX_CHY_CH, LL_DMA_IT_ALL);

  LL_DMA_SetBlkDataLength(MX_DMAX_CHY_CH, 0U);

  LL_DMA_EnableChannel(MX_DMAX_CHY_CH);

  LL_DAC_Enable(MX_DACx, MX_DAC_CHANNEL);

  /* Ensure minimum wait before using peripheral after enabling it */
  DelayUs(DAC_DELAY_STARTUP_US);
}


/**
  *  Start the MX_TIMx in polling mode.
  */
inline void TIM_Start(void)
{
  LL_TIM_EnableCounter(MX_TIMx);
}


/**
  * Delays execution for an approximate number of microseconds.
  * Calculates the wait loop count based on the CMSIS variable "SystemCoreClock".
  * The delay is approximate and affected by CPU pipeline, branch prediction, cache, and compiler optimizations.
  * For core clock frequencies below 500 kHz, the delay is limited to a few CPU cycles.
  *
  * warning: This function provides only an approximate delay and must not be used for precise timing.
  * param: delay_us Number of microseconds to delay.
  */
static inline void DelayUs(uint32_t delay_us)
{
  volatile uint32_t wait_loop_index = ((delay_us * (SystemCoreClock >> 19U)) >> 2U);
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }
}
