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
#include "mx_flash.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_usart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

/* *************************************************************
  ICACHE: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_icache_init
  ************************************************************* */

/* *************************************************************
  Cortex_MPU: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_mpu_init
  ************************************************************* */

/* *************************************************************
  Cortex_NVIC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_nvic_init
  ************************************************************* */

/* ***********************************************************
    FLASH: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_flash with HAL layer
  *         Name of the User label:
  *                   mx_example_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_example_flash_init mx_flash_init

/* ***********************************************************
    gpio_default: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_button
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_gpio_button_init mx_gpio_default_init

/* *************************************************************
  RCC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_rcc_init
  ************************************************************* */

/* ***********************************************************
    USART2: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_usart2_uart with HAL layer
  *         Name of the User label:
  *                   mx_basic_stdio
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define mx_basic_stdio_init mx_usart2_uart_init

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

/* *************************************************************
  ICACHE: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_icache_deinit
  ************************************************************* */

/* *************************************************************
  Cortex_MPU: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_mpu_deinit
  ************************************************************* */

/* *************************************************************
  Cortex_NVIC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_nvic_deinit
  ************************************************************* */

/* ***********************************************************
    FLASH: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_flash with HAL layer
  *         Name of the User label:
  *                   mx_example_flash
  */
#define mx_example_flash_deinit mx_flash_deinit

/* ***********************************************************
    gpio_default: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_button
  */
#define mx_gpio_button_deinit mx_gpio_default_deinit

/* ***********************************************************
    USART2: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_usart2_uart with HAL layer
  *         Name of the User label:
  *                   mx_basic_stdio
  */
#define mx_basic_stdio_deinit mx_usart2_uart_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* *********************************************************** */
/*        GPIO: aliases for get EXTI object functions */
/* *********************************************************** */

/**
  * @brief  Get the EXTI handle for EXTI13 used for PC13
  *         Name of the User label:
  *                   pc13 (user label)
  * @retval hal_exti_handle_t Pointer on the handle on the EXTI instance
  */
#define pc13_exti_gethandle   mx_gpio_exti13_gethandle

/* *************************************************************
ICACHE: No software label has been defined for this peripheral instance
  in the STM32CubeMX2 configuration panel.
  As a result, no aliases are generated for mx_icache_gethandle
************************************************************* */

/* *************************************************************
  Cortex_MPU: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_mpu_gethandle
  ************************************************************* */

/* *************************************************************
  Cortex_NVIC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_cortex_nvic_gethandle
  ************************************************************* */

/* ***********************************************************
    FLASH: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for FLASH
  *         Name of the User label:
  *                   mx_example_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_example_flash_gethandle mx_flash_gethandle

/* ***********************************************************
    USART2: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for USART2
  *         Name of the User label:
  *                   mx_basic_stdio
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define mx_basic_stdio_gethandle mx_usart2_uart_gethandle

/* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
