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
#ifndef USE_LL_APP_TIMEOUT
#define USE_LL_APP_TIMEOUT 0U
#endif /* USE_LL_APP_TIMEOUT */

#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  Activates the MX_ADCx instance.
  * retval: system status (see system_status_t)
  */
system_status_t ADC_Activate(void);

/** brief:  Calibrates the MX_ADCx instance.
  * retval: system status (see system_status_t)
  */
system_status_t ADC_Calibrate(void);

/**
  * brief:  Start the MX_ADCx regular channels conversion in DMA mode.
  * retval: system status (see system_status_t)
  */
void ADC_StartConversion_DMA(void);

/** brief:  Get the MX_ADCx GPIO data in mV.
  * retval: data value in mV
  */
uint16_t ADC_GetDataGPIOmV(void);

/** brief:  Get the MX_ADCx VrefInt data in mV.
  * retval: data value in mV
  */
uint16_t ADC_GetDataVrefIntmV(void);

/** brief:  Get the MX_ADCx temperature sensor data in DegC.
  * retval: data value in DegC
  */
uint16_t ADC_GetDataTemperatureDegC(void);

/** brief:  MX_ADCx conversion complete callback.
  */
void ADC_ConversionCompleteCallback(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  MX_ADCx conversion complete callback. */
void ADC_ConversionErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* LL_EXAMPLE_H */
