/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : DAC perform a simple conversion in low power mode using
  *                  the sample and hold feature with LL API.
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DELAY_STARTUP_US (15UL)  /*!< Delay of 15 us for DAC channel voltage settling time from DAC channel
                                          startup (transition from disable to enable) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void DelayUs(uint32_t delay_us);

/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_DACx Channel in polling mode.
  */
inline void DAC_StartChannel(void)
{
  LL_DAC_Enable(MX_DACx, MX_DAC_CHANNEL);

  /* Ensure minimum wait before using peripheral after enabling it */
  DelayUs(DAC_DELAY_STARTUP_US);
}


/**
  *  Get the MX_DACx raw data values of voltages in mV.
  *  uint8_t raw data value
  */
inline uint8_t DAC_GetVoltageRawData(uint16_t dac_voltage_value)
{
  return (LL_DAC_CALC_VOLTAGE_TO_DATA(DAC_VREFPLUS_VALUE, dac_voltage_value, LL_DAC_RESOLUTION_8B));
}


/**
  * Set the MX_DACx data voltage output.
  */
inline void DAC_SetChannelData(uint8_t data)
{
  LL_DAC_ConvertData8RightAligned(MX_DACx, MX_DAC_CHANNEL, data);
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
