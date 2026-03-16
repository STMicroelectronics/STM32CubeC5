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
#include "mx_cortex_nvic.h"
#include "mx_crc.h"
#include "mx_flash.h"
#include "mx_hash.h"
#include "mx_icache.h"
#include "mx_ramcfg.h"
#include "mx_rcc.h"
#include "mx_rng.h"
#include "mx_sbs.h"
#include "mx_tamp.h"
#include "mx_usart2.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* ***********************************************************
              Aliases to RAMCFG_SRAM2 used in HAL
  *********************************************************** */
#define mx_rot_ramcfg  HAL_RAMCFG_SRAM2

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
    CRC: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_crc with HAL layer.
  *         Name of the User label:
  *                   mx_rot_crc
  * @retval hal_crc_handle_t Pointer on the handle on the CRC instance
  */
#define mx_rot_crc_init mx_crc_init

/* ***********************************************************
    FLASH: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_flash with HAL layer.
  *         Name of the User label:
  *                   mx_rot_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_rot_flash_init mx_flash_init

/* ***********************************************************
    HASH: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_hash with HAL layer.
  *         Name of the User label:
  *                   mx_rot_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_rot_hash_init mx_hash_init

/* ***********************************************************
    ICACHE: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_icache with HAL layer.
  *         Name of the User label:
  *                   mx_rot_icache
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define mx_rot_icache_init mx_icache_init

/* ***********************************************************
    RAMCFG_SRAM2: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_ramcfg_sram2 with HAL layer.
  *         Name of the User label:
  *                   mx_rot_ramcfg
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_rot_ramcfg_init mx_ramcfg_sram2_init

/* *************************************************************
  RCC: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_rcc_init
  ************************************************************* */

/* ***********************************************************
    RNG: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_rng with HAL layer.
  *         Name of the User label:
  *                   mx_rot_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_rot_rng_init mx_rng_init

/* ***********************************************************
    SBS: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_sbs with HAL layer.
  *         Name of the User label:
  *                   mx_rot_sbs
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_rot_sbs_init mx_sbs_init

/* ***********************************************************
    TAMP: aliases for initialization functions
  *********************************************************** */

/**
  * @brief  Initialize the mx_tamp with HAL layer.
  *         Name of the User label:
  *                   mx_rot_tamp
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_rot_tamp_init mx_tamp_init

/* ***********************************************************
    USART2: aliases for initialization functions
  *********************************************************** */

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
    CRC: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_crc with HAL layer.
  *         Name of the User label:
  *                   mx_rot_crc
  */
#define mx_rot_crc_deinit mx_crc_deinit

/* ***********************************************************
    FLASH: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_flash with HAL layer.
  *         Name of the User label:
  *                   mx_rot_flash
  */
#define mx_rot_flash_deinit mx_flash_deinit

/* ***********************************************************
    HASH: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_hash with HAL layer.
  *         Name of the User label:
  *                   mx_rot_hash
  */
#define mx_rot_hash_deinit mx_hash_deinit

/* ***********************************************************
    ICACHE: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_icache with HAL layer.
  *         Name of the User label:
  *                   mx_rot_icache
  */
#define mx_rot_icache_deinit mx_icache_deinit

/* ***********************************************************
    RAMCFG_SRAM2: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_ramcfg_sram2 with HAL layer.
  *         Name of the User label:
  *                   mx_rot_ramcfg
  */
#define mx_rot_ramcfg_deinit mx_ramcfg_sram2_deinit

/* ***********************************************************
    RNG: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_rng with HAL layer.
  *         Name of the User label:
  *                   mx_rot_rng
  */
#define mx_rot_rng_deinit mx_rng_deinit

/* ***********************************************************
    SBS: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_sbs with HAL layer.
  *         Name of the User label:
  *                   mx_rot_sbs
  */
#define mx_rot_sbs_deinit mx_sbs_deinit

/* ***********************************************************
    TAMP: aliases for De-Initialization functions
  *********************************************************** */

/**
  * @brief  De-Initialize the mx_tamp with HAL layer.
  *         Name of the User label:
  *                   mx_rot_tamp
  */
#define mx_rot_tamp_deinit mx_tamp_deinit

/* *************************************************************
  USART2: No software label has been defined for this peripheral instance
    in the STM32CubeMX2 configuration panel.
    As a result, no aliases are generated for mx_usart2_uart_deinit
  ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* ***********************************************************
    CRC: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for CRC.
  *         Name of the User label:
  *                   mx_rot_crc
  * @retval hal_crc_handle_t Pointer on the handle on the CRC instance
  */
#define mx_rot_crc_gethandle mx_crc_gethandle

/* ***********************************************************
    FLASH: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for FLASH.
  *         Name of the User label:
  *                   mx_rot_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_rot_flash_gethandle mx_flash_gethandle

/* ***********************************************************
    HASH: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for HASH.
  *         Name of the User label:
  *                   mx_rot_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_rot_hash_gethandle mx_hash_gethandle

/* ***********************************************************
    ICACHE: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for ICACHE.
  *         Name of the User label:
  *                   mx_rot_icache
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define mx_rot_icache_gethandle mx_icache_gethandle

/* ***********************************************************
    RNG: aliases for get handle functions
  *********************************************************** *//**
  * @brief  Get the HAL handle for RNG.
  *         Name of the User label:
  *                   mx_rot_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_rot_rng_gethandle mx_rng_gethandle

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
