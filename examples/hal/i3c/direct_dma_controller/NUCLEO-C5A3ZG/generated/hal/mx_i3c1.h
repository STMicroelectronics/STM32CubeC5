/**
  ******************************************************************************
  * @file           : mx_i3c1.h
  * @brief          : Header for mx_i3c1.c file.
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
#ifndef MX_I3C1_H
#define MX_I3C1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for I3C1_SCL pin */
#define PB6_PORT                              HAL_GPIOB
#define PB6_PIN                               HAL_GPIO_PIN_6

/** Primary aliases for I3C1_SDA pin */
#define PB7_PORT                              HAL_GPIOB
#define PB7_PIN                               HAL_GPIO_PIN_7

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for I3C in HAL layer */
/******************************************************************************/
/**
  * @brief mx_i3c1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_i3c_handle_t *mx_i3c1_init(void);

/**
  * @brief  De-initialize i3c1 instance and return it.
  */
void mx_i3c1_deinit(void);

/**
  * @brief  Get the I3C1 object.
  * @retval Pointer on the I3C1 Handle
  */
hal_i3c_handle_t *mx_i3c1_gethandle(void);

/******************************************************************************/
/*                      LPDMA1 channel0 global interrupt                      */
/******************************************************************************/

void LPDMA1_CH0_IRQHandler(void);

/******************************************************************************/
/*                      LPDMA1 channel1 global interrupt                      */
/******************************************************************************/

void LPDMA1_CH1_IRQHandler(void);

/******************************************************************************/
/*                      LPDMA1 channel2 global interrupt                      */
/******************************************************************************/

void LPDMA1_CH2_IRQHandler(void);

/******************************************************************************/
/*                            I3C1 event interrupt                            */
/******************************************************************************/

void I3C1_EV_IRQHandler(void);

/******************************************************************************/
/*                            I3C1 error interrupt                            */
/******************************************************************************/

void I3C1_ERR_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_I3C1_H */
