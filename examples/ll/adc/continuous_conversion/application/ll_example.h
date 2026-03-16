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

/** brief:  Starts the MX_ADCx instance conversion.
  */
void ADC_StartConversion(void);

/** brief:  Wait for conversion on ADC group regular to be completed.
  * retval: system status (see system_status_t)
  */
system_status_t ADC_REG_PollForConv(void);

/** brief:  Get the MX_ADCx GPIO data in mV.
  * retval: data value in mV
  */
uint32_t        ADC_GetDataGPIOmV(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
