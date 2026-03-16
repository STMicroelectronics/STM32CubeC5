/**
  ******************************************************************************
  * @file           : mx_pka.h
  * @brief          : Header for mx_pka.c file.
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
#ifndef MX_PKA_H
#define MX_PKA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for PKA in HAL layer */
/******************************************************************************/
/**
  * @brief mx_pka init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_pka_handle_t *mx_pka_init(void);

/**
  * @brief  De-initialize pka instance and return it.
  */
void mx_pka_deinit(void);

/**
  * @brief  Get the PKA object.
  * @retval Pointer on the PKA Handle
  */
hal_pka_handle_t *mx_pka_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_PKA_H */
