/**
  ******************************************************************************
  * @file           : mx_rtc.h
  * @brief          : Header for mx_rtc.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_RTC_H
#define MX_RTC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for RTC in HAL layer */
/******************************************************************************//**
  * @brief mx_rtc init function
  * This function configures the hardware resources used in this example
  */
system_status_t mx_rtc_init(void);

/**
  * @brief  De-initialize rtc instance and return it.
  */
void mx_rtc_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_RTC_H */
