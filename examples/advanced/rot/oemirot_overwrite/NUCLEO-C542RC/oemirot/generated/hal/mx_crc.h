/**
  ******************************************************************************
  * @file           : mx_crc.h
  * @brief          : Header for mx_crc.c file.
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
#ifndef MX_CRC_H
#define MX_CRC_H

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
/* Exported functions for CRC in HAL layer */
/******************************************************************************/
/**
  * @brief mx_crc init function.
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_crc_handle_t *mx_crc_init(void);

/**
  * @brief  De-initialize crc instance and return it.
  */
void mx_crc_deinit(void);

/**
  * @brief  Get the CRC object.
  * @retval Pointer on the CRC Handle
  */
hal_crc_handle_t *mx_crc_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CRC_H */
