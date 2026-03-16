/**
  ******************************************************************************
  * @file           : mx_lptim1.h
  * @brief          : Header for mx_lptim1.c file.
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
#ifndef MX_LPTIM1_H
#define MX_LPTIM1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_LPTIM1

/* Aliases of channels labels */
#define MX_LPTIM_X_CHANNEL  LL_LPTIM_CHANNEL_CH1

/** Primary aliases for LPTIM1_ETR pin */
#define NETR16_2_PORT                         GPIOA
#define NETR16_2_PIN                          LL_GPIO_PIN_15

/** Primary aliases for LPTIM1_CH1 pin */
#define PB13_PORT                             GPIOB
#define PB13_PIN                              LL_GPIO_PIN_13

    #define MX_LPTIMX_IRQN       LPTIM1_IRQn
    #define MX_LPTIMX_IRQHANDLER LPTIM1_IRQHandler
    #define MYLPTIM_1_IRQN       MX_LPTIMX_IRQN
    #define MYLPTIM_1_IRQHANDLER MX_LPTIMX_IRQHANDLER
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for SW instance in LL layer */
/******************************************************************************/
/**
  * @brief mx_lptim1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
LPTIM_TypeDef *mx_lptim1_init(void);

/**
  * @brief  De-initialize mx_lptim1 instance and return it.
  */
void mx_lptim1_deinit(void);

/******************************************************************************/
/*         LPTIM1 global interrupt is managed directly in user code.          */
/******************************************************************************/

/* void LPTIM1_IRQHandler(void); */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPTIM1_H */
