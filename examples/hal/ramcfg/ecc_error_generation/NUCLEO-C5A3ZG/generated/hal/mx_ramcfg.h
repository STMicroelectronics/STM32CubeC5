/**
  ******************************************************************************
  * @file           : mx_ramcfg.h
  * @brief          : Header for mx_ramcfg.h file.
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
#ifndef MX_RAMCFG_H
#define MX_RAMCFG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for RAMCFG_SRAM2 in HAL layer */
/******************************************************************************//**
* @brief mx_ramcfg_sram2 init function
* This function configures the hardware resources used in this example
* @retval pointer to handle or NULL in case of failure
*/
hal_ramcfg_t mx_ramcfg_sram2_init(void);
void mx_ramcfg_sram2_deinit(void);

/******************************************************************************/
/* RAMCFG global interrupt (Grouping interrupts of all active RAMCFG instances)*/
/******************************************************************************/
void RAMCFG_IRQHandler(void);

/******************************************************************************/
/* RAMCFG NMI interrupt */
/******************************************************************************/
__WEAK system_status_t RAMCFG_SRAM2_NMI_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_RAMCFG_H */
