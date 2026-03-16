/**
  ******************************************************************************
  * @file           : mx_flash.h
  * @brief          : Header for mx_flash.c file.
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
#ifndef MX_FLASH_H
#define MX_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for FLASH in HAL layer */
/******************************************************************************/
/**
  * @brief mx_flash init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_flash_handle_t *mx_flash_init(void);

/**
  * @brief  De-initialize flash instance and return it.
  */
void mx_flash_deinit(void);

/**
  * @brief  Get the FLASH object.
  * @retval Pointer on the FLASH Handle
  */
hal_flash_handle_t *mx_flash_gethandle(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_FLASH_H */