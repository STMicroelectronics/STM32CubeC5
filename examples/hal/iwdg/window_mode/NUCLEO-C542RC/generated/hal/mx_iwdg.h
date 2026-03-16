/**
  ******************************************************************************
  * @file           : mx_iwdg.h
  * @brief          : Header for mx_iwdg.c file.
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
#ifndef MX_IWDG_H
#define MX_IWDG_H

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
/* Exported functions for IWDG in HAL layer */
/******************************************************************************/
/**
  * @brief mx_iwdg init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_iwdg_handle_t *mx_iwdg_init(void);

/**
  * @brief   Start the IWDG. Before exiting the function, the watchdog is refreshed to have a correct time base.
  * @param   None
  * @retval  HAL_OK  Operation completed successfully.
  * @retval  HAL_ERROR Operation completed with error.
  */
hal_status_t mx_iwdg_start(void);
 /**
  * @brief  Get the IWDG object.
  * @retval Pointer on the IWDG Handle
  */
hal_iwdg_handle_t *mx_iwdg_iwdg_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_IWDG_H */
