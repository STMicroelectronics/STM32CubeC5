/**
  ******************************************************************************
  * @file           : mx_hal_def.h
  * @brief          : Exporting peripherals initialization
  *                   Include entry for the target folder to the application.
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
#ifndef MX_HAL_DEF_H
#define MX_HAL_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_def.h"
#include "stm32_hal.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_crc.h"
#include "mx_flash.h"
#include "mx_rcc.h"
#include "mx_usart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

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

/* *************************************************************
  CRC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_crc_init
  ************************************************************* */

/* *************************************************************
  FLASH: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_flash_init
  ************************************************************* */

/* *************************************************************
  ICACHE: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_icache_init
  ************************************************************* */

/* *************************************************************
  RCC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_rcc_init
  ************************************************************* */

/* *************************************************************
  USART2: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_usart2_uart_init
  ************************************************************* */

/* ########################################################### */

/**
  * @brief  Initialize the mx_crc with HAL layer.
  *         Name of the User label:
  *                   mx_example_crc
  * @retval hal_crc_handle_t Pointer on the handle on the CRC instance
  */
#define mx_example_crc_init mx_crc_init

/* ########### Aliases to De-Initialization functions ########### */

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

/* *************************************************************
  CRC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_crc_deinit
  ************************************************************* */

/* *************************************************************
  FLASH: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_flash_deinit
  ************************************************************* */

/* *************************************************************
  ICACHE: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_icache_deinit
  ************************************************************* */

/* *************************************************************
  USART2: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_usart2_uart_deinit
  ************************************************************* */

/**
  * @brief  De-Initialize the mx_usart2_uart with HAL layer
  *         Name of the User label:
  *                   mx_basic_stdio
  */
#define mx_basic_stdio_deinit mx_usart2_uart_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* *************************************************************
  CRC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_crc_gethandle
  ************************************************************* */

/* *************************************************************
  FLASH: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_flash_gethandle
  ************************************************************* */

/* *************************************************************
  ICACHE: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_icache_gethandle
  ************************************************************* */

/* *************************************************************
  USART2: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_usart2_uart_gethandle
  ************************************************************* */

/* ***********************************************************
    USART2: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for USART2.
  *         Name of the User label:
  *                   mx_basic_stdio
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define mx_basic_stdio_gethandle mx_usart2_uart_gethandle

/* ########### Aliases to get IRQ Handlers functions ########### */
/* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
