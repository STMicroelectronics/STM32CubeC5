/**
  ******************************************************************************
  * @file           : mx_crs.h
  * @brief          : Header for mx_crs.c file.
  ******************************************************************************
  * @attention
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_CRS_H
#define STM32_CRS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "stm32c5xx_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

hal_status_t crs_init(void);
hal_status_t crs_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32_CRS_H */
