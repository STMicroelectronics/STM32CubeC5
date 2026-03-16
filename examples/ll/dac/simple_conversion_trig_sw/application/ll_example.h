/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define DAC_VREFPLUS_VALUE (VDD_VALUE) /* Assumption: pin Vref plus connected to Vdd at board level */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief:  Calibrate the MX_DACx channel buffer.
  */
void DAC_CalibrateChannelBuffer(void);

/** brief:  Start the MX_DACx Channel in polling mode.
  */
void DAC_StartChannel(void);

/** brief:  Get the MX_DACx raw data values of voltages in mV.
  * retval: raw data value
  */
uint8_t DAC_GetVoltageRawData(uint16_t dac_voltage_value);

/** brief: Set the MX_DACx data voltage output.
  */
void DAC_SetChannelData(uint8_t data);

/** brief:  Enable the MX_DACx channel software trigger conversion.
  */
void DAC_TrigSWConversionChannel(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
