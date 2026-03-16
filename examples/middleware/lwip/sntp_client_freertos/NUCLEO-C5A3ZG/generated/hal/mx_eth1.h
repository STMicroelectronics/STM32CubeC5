/**
  ******************************************************************************
  * @file           : mx_eth1.h
  * @brief          : Header for mx_eth1.c file.
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
#ifndef MX_ETH1_H
#define MX_ETH1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for ETH1_CLK pin */
#define PA10_PORT                             HAL_GPIOA
#define PA10_PIN                              HAL_GPIO_PIN_10

/** Primary aliases for ETH1_MDC pin */
#define PC1_PORT                              HAL_GPIOC
#define PC1_PIN                               HAL_GPIO_PIN_1

/** Primary aliases for ETH1_MDIO pin */
#define PE12_PORT                             HAL_GPIOE
#define PE12_PIN                              HAL_GPIO_PIN_12

/** Primary aliases for ETH1_RMII_CRS_DV pin */
#define PD1_PORT                              HAL_GPIOD
#define PD1_PIN                               HAL_GPIO_PIN_1

/** Primary aliases for ETH1_RMII_REF_CLK pin */
#define PA1_PORT                              HAL_GPIOA
#define PA1_PIN                               HAL_GPIO_PIN_1

/** Primary aliases for ETH1_RMII_RXD0 pin */
#define PC4_PORT                              HAL_GPIOC
#define PC4_PIN                               HAL_GPIO_PIN_4

/** Primary aliases for ETH1_RMII_RXD1 pin */
#define PC5_PORT                              HAL_GPIOC
#define PC5_PIN                               HAL_GPIO_PIN_5

/** Primary aliases for ETH1_RMII_TX_EN pin */
#define PG11_PORT                             HAL_GPIOG
#define PG11_PIN                              HAL_GPIO_PIN_11

/** Primary aliases for ETH1_RMII_TXD0 pin */
#define PG13_PORT                             HAL_GPIOG
#define PG13_PIN                              HAL_GPIO_PIN_13

/** Primary aliases for ETH1_RMII_TXD1 pin */
#define PG12_PORT                             HAL_GPIOG
#define PG12_PIN                              HAL_GPIO_PIN_12

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for ETH in HAL layer */
/******************************************************************************/
/**
  * @brief mx_eth1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_eth_handle_t *mx_eth1_init(void);

/**
  * @brief  De-initialize eth1 instance and return it.
  */
void mx_eth1_deinit(void);

/**
  * @brief  Get the ETH1 object.
  * @retval Pointer on the ETH1 Handle
  */
hal_eth_handle_t *mx_eth1_gethandle(void);

/******************************************************************************/
/*                             Ethernet interrupt                             */
/******************************************************************************/

void ETH1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_ETH1_H */
