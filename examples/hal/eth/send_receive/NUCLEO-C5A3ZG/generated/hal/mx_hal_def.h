/**
  ******************************************************************************
  * @file           : mx_hal_def.h
  * @brief          : Exporting peripherals initialization
  *                   Include entry for the target folder to the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_HAL_DEF_H
#define MX_HAL_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_usart2.h"
#include "mx_eth1.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_icache_hal with HAL layer
  *         Name of the User label:
  *                   MyICACHE_1
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define myicache_1_init mx_icache_hal_init


/* *********************************************************** */
/*        RCC_MyRCC_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_rcc_hal with HAL layer
  *         Name of the User label:
  *                   MyRCC_1
  * @retval system_status_t Value based on the system_status_t
  */
#define myrcc_1_init mx_rcc_hal_init

/* *********************************************************** */
/*        CORTEX_MPU_MyCORTEX_MPU_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_cortex_mpu_hal with HAL layer
  *         Name of the User label:
  *                   MyCORTEX_MPU_1
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_cortex_mpu_hal_init mx_cortex_mpu_init


/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_gpio_status_led
  * @retval system_status_t Value based on the system_status_t
  */
#define mygpio_1_init mx_gpio_default_hal_init

#define mx_gpio_status_led_init mx_gpio_default_hal_init


/* *********************************************************** */
/*        USART2_MyUSART_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_usart2_hal_usart with HAL layer
  *         Name of the User label:
  *                   MyUSART_1
  *                   mx_basic_stdio
  * @retval hal_usart_handle_t Pointer on the handle on the USART2 instance
  */

#define mx_basic_stdio_init mx_usart2_uart_init


/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */


/* *********************************************************** */
/*        CORTEX_MPU_MyCORTEX_MPU_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_cortex_mpu_hal with HAL layer
  *         Name of the User label:
  *                   MyCORTEX_MPU_1
  */
#define mycortex_mpu_1_deinit mx_cortex_mpu_hal_deinit


/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default_hal with HAL layer
  *         Name of the User label:
  *                   mx_gpio_status_led
  */
#define mx_gpio_status_led_deinit mx_gpio_default_hal_deinit


/* *********************************************************** */
/*        USART2_MyUSART_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_usart2_hal_usart with HAL layer
  *         Name of the User label:
  *                   MyUSART_1
  *                   mx_basic_stdio
  */

#define mx_basic_stdio_deinit mx_usart2_hal_uart_deinit


/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */


/* *********************************************************** */
/*        USART2: aliases for get handle functions */
/* *********************************************************** */

/**
  * @brief  Get the HAL handle for USART2
  *         Name of the User label:
  *                   mx_basic_stdio
  * @retval hal_usart_handle_t Pointer on the handle on the USART2 instance
  */

#define mx_basic_stdio_gethandle mx_usart2_uart_gethandle


/* ########################################################### */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
