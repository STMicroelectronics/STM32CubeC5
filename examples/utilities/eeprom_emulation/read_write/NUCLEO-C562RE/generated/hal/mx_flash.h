/**
  ******************************************************************************
  * @file           : mx_flash.h
  * @brief          : Header for mx_flash.c file.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_FLASH_H
#define MX_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Private define ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief nvm init function

  * @param None
  * @retval
  */
hal_flash_handle_t *flash_instance0_cfg1_init(void);

/**
  * @brief nvm peripheral deinitialization function
  * @param None
  * @retval None
  */
void flash_deinit(void);

/**
  * @brief  Get the nvm object.
  * @param None
  * @retval Pointer on the nvm Handle
  */
hal_flash_handle_t *mx_hal_flash_gethandle(void);

#ifdef __cplusplus
}
#endif

#endif /* MX_FLASH_H */
