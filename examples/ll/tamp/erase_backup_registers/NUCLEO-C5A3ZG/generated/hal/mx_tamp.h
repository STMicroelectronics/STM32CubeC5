/**
  ******************************************************************************
  * @file           : mx_tamp.h
  * @brief          : Header for mx_tamp.c file.
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
#ifndef MX_TAMP_H
#define MX_TAMP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_TAMP

/** Primary aliases for TAMP_IN1 pin */
#define PC13_PORT                             GPIOC
#define PC13_PIN                              LL_GPIO_PIN_13
/******************************************************************************/
/* Exported defines for TAMP in LL layer (SW instance MyTAMP_1) */
/******************************************************************************/
#define MX_TAMP_X LL_TAMP_1
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for TAMP in LL layer */
/******************************************************************************/
/**
  * @brief mx_tamp init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
system_status_t mx_tamp_init(void);

/**
  * @brief  De-initialize tamp instance and return it.
  */
void mx_tamp_deinit(void);

/******************************************************************************/
/*    Tamper global interrupts is managed directly in user code.   */
/******************************************************************************/
/* void TAMP_IRQHandler(void); */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_TAMP_H */
