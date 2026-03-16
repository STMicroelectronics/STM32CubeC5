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
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_CRC

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for CRC in LL layer */
/******************************************************************************/
/**
  * @brief mx_crc init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
CRC_TypeDef *mx_crc_init(void);

/**
  * @brief  De-initialize crc instance and return it.
  */
void mx_crc_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CRC_H */
