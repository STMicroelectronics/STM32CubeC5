/**
  ******************************************************************************
  * @file           : mx_hash.h
  * @brief          : Header for mx_hash.c file.
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
#ifndef MX_HASH_H
#define MX_HASH_H

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
/* Exported functions for DCACHE in HAL layer */
/******************************************************************************/
/**
  * @brief mx_hash init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_hash_handle_t *mx_hash_init(void);

/**
  * @brief  De-initialize hash instance and return it.
  */
void mx_hash_deinit(void);

/**
  * @brief  Get the HASH object.
  * @retval Pointer on the HASH Handle
  */
hal_hash_handle_t *mx_hash_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HASH_H */
