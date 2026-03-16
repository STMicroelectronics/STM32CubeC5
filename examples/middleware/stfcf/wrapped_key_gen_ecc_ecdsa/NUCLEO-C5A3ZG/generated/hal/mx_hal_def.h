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
#include "mx_saes.h"
#include "mx_ccb.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_gpio_default.h"
#include "mx_hash.h"
#include "mx_icache.h"
#include "mx_pka.h"
#include "mx_rcc.h"
#include "mx_rng.h"
#include "mx_usart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

  /* ***********************************************************
      SAES: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_saes with HAL layer
  *         Name of the User label:
  *                   mx_example_saes
  * @retval hal_aes_handle_t Pointer on the handle on the SAES instance
  */
#define mx_example_saes_init mx_saes_init

  /* ***********************************************************
      CCB: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_ccb with HAL layer
  *         Name of the User label:
  *                   mx_example_ccb
  * @retval hal_ccb_handle_t Pointer on the handle on the CCB instance
  */
#define mx_example_ccb_init mx_ccb_init

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
      gpio_default: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_status_led
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_gpio_status_led_init mx_gpio_default_init

  /* ***********************************************************
      HASH: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_hash with HAL layer
  *         Name of the User label:
  *                   mx_example_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_example_hash_init mx_hash_init

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_init
    ************************************************************* */

  /* ***********************************************************
      PKA: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_pka with HAL layer
  *         Name of the User label:
  *                   mx_example_pka
  * @retval hal_pka_handle_t Pointer on the handle on the PKA instance
  */
#define mx_example_pka_init mx_pka_init

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

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_init
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

  /* ***********************************************************
      SAES: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_saes with HAL layer
  *         Name of the User label:
  *                   mx_example_saes
  */
#define mx_example_saes_deinit mx_saes_deinit

  /* ***********************************************************
      CCB: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_ccb with HAL layer
  *         Name of the User label:
  *                   mx_example_ccb
  */
#define mx_example_ccb_deinit mx_ccb_deinit

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
      gpio_default: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_status_led
  */
#define mx_gpio_status_led_deinit mx_gpio_default_deinit

  /* ***********************************************************
      HASH: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_hash with HAL layer
  *         Name of the User label:
  *                   mx_example_hash
  */
#define mx_example_hash_deinit mx_hash_deinit

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* ***********************************************************
      PKA: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_pka with HAL layer
  *         Name of the User label:
  *                   mx_example_pka
  */
#define mx_example_pka_deinit mx_pka_deinit

  /* ***********************************************************
      RNG: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_rng with HAL layer
  *         Name of the User label:
  *                   mx_example_rng
  */
#define mx_example_rng_deinit mx_rng_deinit

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_deinit
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

  /* ***********************************************************
      SAES: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for SAES
  *         Name of the User label:
  *                   mx_example_saes
  * @retval hal_aes_handle_t Pointer on the handle on the SAES instance
  */
#define mx_example_saes_gethandle mx_saes_gethandle

  /* ***********************************************************
      CCB: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for CCB
  *         Name of the User label:
  *                   mx_example_ccb
  * @retval hal_ccb_handle_t Pointer on the handle on the CCB instance
  */
#define mx_example_ccb_gethandle mx_ccb_gethandle

  /* ***********************************************************
      HASH: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for HASH
  *         Name of the User label:
  *                   mx_example_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_example_hash_gethandle mx_hash_gethandle

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_gethandle
    ************************************************************* */

  /* ***********************************************************
      PKA: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for PKA
  *         Name of the User label:
  *                   mx_example_pka
  * @retval hal_pka_handle_t Pointer on the handle on the PKA instance
  */
#define mx_example_pka_gethandle mx_pka_gethandle

  /* ***********************************************************
      RNG: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for RNG
  *         Name of the User label:
  *                   mx_example_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_example_rng_gethandle mx_rng_gethandle

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
