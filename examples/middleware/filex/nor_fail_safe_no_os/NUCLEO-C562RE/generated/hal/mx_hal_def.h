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
#include "mx_flash.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_rng.h"
#include "mx_spi1.h"
#include "mx_tim2.h"
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

  /* *************************************************************
    gpio_default: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_gpio_default_init
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

  /* ***********************************************************
      RNG: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_rng with HAL layer
  *         Name of the User label:
  *                   mx_example_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_example_rng_init mx_rng_init

  /* ***********************************************************
      SPI1: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_spi1 with HAL layer
  *         Name of the User label:
  *                   mx_example_memory_bus
  * @retval hal_spi_handle_t Pointer on the handle on the SPI1 instance
  */
#define mx_example_memory_bus_init mx_spi1_init

  /* ***********************************************************
      TIM2: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_tim2 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim
  * @retval hal_tim_handle_t Pointer on the handle on the TIM2 instance
  */
#define mx_example_tim_init mx_tim2_init

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_init
    ************************************************************* */

/* ########################################################### */

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

  /* ***********************************************************
      FLASH: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_flash with HAL layer
  *         Name of the User label:
  *                   mx_example_flash
  */
#define mx_example_flash_deinit mx_flash_deinit

  /* *************************************************************
    gpio_default: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_gpio_default_deinit
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* ***********************************************************
      RNG: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_rng with HAL layer
  *         Name of the User label:
  *                   mx_example_rng
  */
#define mx_example_rng_deinit mx_rng_deinit

  /* ***********************************************************
      SPI1: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_spi1 with HAL layer
  *         Name of the User label:
  *                   mx_example_memory_bus
  */
#define mx_example_memory_bus_deinit mx_spi1_deinit

  /* ***********************************************************
      TIM2: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_tim2 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim
  */
#define mx_example_tim_deinit mx_tim2_deinit

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_deinit
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

  /* ***********************************************************
      FLASH: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for FLASH
  *         Name of the User label:
  *                   mx_example_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_example_flash_gethandle mx_flash_gethandle

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_gethandle
    ************************************************************* */

  /* ***********************************************************
      RNG: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for RNG
  *         Name of the User label:
  *                   mx_example_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_example_rng_gethandle mx_rng_gethandle

  /* ***********************************************************
      SPI1: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for SPI1
  *         Name of the User label:
  *                   mx_example_memory_bus
  * @retval hal_spi_handle_t Pointer on the handle on the SPI1 instance
  */
#define mx_example_memory_bus_gethandle mx_spi1_gethandle

  /* ***********************************************************
      TIM2: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for TIM2
  *         Name of the User label:
  *                   mx_example_tim
  * @retval hal_tim_handle_t Pointer on the handle on the TIM2 instance
  */
#define mx_example_tim_gethandle mx_tim2_gethandle

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_gethandle
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
