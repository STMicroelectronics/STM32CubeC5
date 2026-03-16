/**
  ******************************************************************************
  * @file           : mx_crs.h
  * @brief          : Header for mx_crs.c file.
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
#ifndef MX_CRS_H
#define MX_CRS_H

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
/* Exported functions for CRS in HAL layer */
/******************************************************************************/
/**
  * @brief mx_crs init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_crs_handle_t *mx_crs_init(void);

/**
  * @brief  De-initialize crs instance and return it.
  */
void mx_crs_deinit(void);

/**
  * @brief  Get the CRS object.
  * @retval Pointer on the CRS Handle
  */
hal_crs_handle_t *mx_crs_gethandle(void);

/******************************************************************************/
/*                   Clock Recovery System global interrupt                   */
/******************************************************************************/

void CRS_IRQHandler(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CRS_H */
