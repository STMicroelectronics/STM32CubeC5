/**
  ******************************************************************************
  * @file    low_level_flash.h
  * @brief   This file contains device definition for low_level_flash driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOW_LEVEL_FLASH_H
#define LOW_LEVEL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "mx_hal_def.h"
#include "Driver_Flash.h"

/* Exported macros -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_PROGRAM_UNIT        (4U*sizeof(uint32_t))

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t base;
  uint32_t limit;
} flash_range_t;

typedef struct
{
  uint32_t nb;
  flash_range_t *range;
} flash_vect_t;

typedef struct
{
  flash_vect_t erase;
  flash_vect_t write;
  uint32_t read_error;
} low_level_device_t;

/* External variables --------------------------------------------------------*/
extern low_level_device_t FLASH0_DEV;
extern ARM_DRIVER_FLASH Driver_FLASH0;

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_FLASH_H */
