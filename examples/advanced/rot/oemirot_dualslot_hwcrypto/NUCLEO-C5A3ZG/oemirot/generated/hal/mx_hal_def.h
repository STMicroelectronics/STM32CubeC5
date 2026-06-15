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
#include "mx_cortex_nvic.h"
#include "mx_crc.h"
#include "mx_flash.h"
#include "mx_hash.h"
#include "mx_icache.h"
#include "mx_pka.h"
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

  /* ***********************************************************
      SAES: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_saes with HAL layer
  *         Name of the User label:
  *                   mx_rot_saes
  *                   mysaes_1
  *                   mx_stfcf_mbedtls_alt_hal_aes
  * @retval hal_aes_handle_t Pointer on the handle on the SAES instance
  */
#define mx_rot_saes_init mx_saes_init

#define mysaes_1_init mx_saes_init

#define mx_stfcf_mbedtls_alt_hal_aes_init mx_saes_init

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_init
    ************************************************************* */

  /* ***********************************************************
      CRC: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_crc with HAL layer
  *         Name of the User label:
  *                   mx_rot_crc
  * @retval hal_crc_handle_t Pointer on the handle on the CRC instance
  */
#define mx_rot_crc_init mx_crc_init

  /* ***********************************************************
      FLASH: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_flash with HAL layer
  *         Name of the User label:
  *                   mx_rot_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_rot_flash_init mx_flash_init

  /* ***********************************************************
      HASH: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_hash with HAL layer
  *         Name of the User label:
  *                   mx_rot_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_rot_hash_init mx_hash_init

  /* ***********************************************************
      ICACHE: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_icache with HAL layer
  *         Name of the User label:
  *                   mx_rot_icache
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define mx_rot_icache_init mx_icache_init

  /* ***********************************************************
      PKA: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_pka with HAL layer
  *         Name of the User label:
  *                   mx_rot_pka
  *                   mypka_1
  *                   mx_stfcf_mbedtls_alt_hal_pka
  * @retval hal_pka_handle_t Pointer on the handle on the PKA instance
  */
#define mx_rot_pka_init mx_pka_init

#define mypka_1_init mx_pka_init

#define mx_stfcf_mbedtls_alt_hal_pka_init mx_pka_init

  /* ***********************************************************
      RAMCFG_SRAM2: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_ramcfg_sram2 with HAL layer
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
  * @brief  Initialize the mx_rng with HAL layer
  *         Name of the User label:
  *                   mx_rot_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_rot_rng_init mx_rng_init

  /* ***********************************************************
      SBS: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_sbs with HAL layer
  *         Name of the User label:
  *                   mx_rot_sbs
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_rot_sbs_init mx_sbs_init

  /* ***********************************************************
      TAMP: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_tamp with HAL layer
  *         Name of the User label:
  *                   mx_rot_tamp
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_rot_tamp_init mx_tamp_init

  /* ***********************************************************
      USART2: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_usart2_uart with HAL layer
  *         Name of the User label:
  *                   mx_rot_uart
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define mx_rot_uart_init mx_usart2_uart_init

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

  /* ***********************************************************
      SAES: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_saes with HAL layer
  *         Name of the User label:
  *                   mx_rot_saes
  *                   mysaes_1
  *                   mx_stfcf_mbedtls_alt_hal_aes
  */
#define mx_rot_saes_deinit mx_saes_deinit

#define mysaes_1_deinit mx_saes_deinit

#define mx_stfcf_mbedtls_alt_hal_aes_deinit mx_saes_deinit

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_deinit
    ************************************************************* */

  /* ***********************************************************
      CRC: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_crc with HAL layer
  *         Name of the User label:
  *                   mx_rot_crc
  */
#define mx_rot_crc_deinit mx_crc_deinit

  /* ***********************************************************
      FLASH: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_flash with HAL layer
  *         Name of the User label:
  *                   mx_rot_flash
  */
#define mx_rot_flash_deinit mx_flash_deinit

  /* ***********************************************************
      HASH: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_hash with HAL layer
  *         Name of the User label:
  *                   mx_rot_hash
  */
#define mx_rot_hash_deinit mx_hash_deinit

  /* ***********************************************************
      ICACHE: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_icache with HAL layer
  *         Name of the User label:
  *                   mx_rot_icache
  */
#define mx_rot_icache_deinit mx_icache_deinit

  /* ***********************************************************
      PKA: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_pka with HAL layer
  *         Name of the User label:
  *                   mx_rot_pka
  *                   mypka_1
  *                   mx_stfcf_mbedtls_alt_hal_pka
  */
#define mx_rot_pka_deinit mx_pka_deinit

#define mypka_1_deinit mx_pka_deinit

#define mx_stfcf_mbedtls_alt_hal_pka_deinit mx_pka_deinit

  /* ***********************************************************
      RAMCFG_SRAM2: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_ramcfg_sram2 with HAL layer
  *         Name of the User label:
  *                   mx_rot_ramcfg
  */
#define mx_rot_ramcfg_deinit mx_ramcfg_sram2_deinit

  /* ***********************************************************
      RNG: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_rng with HAL layer
  *         Name of the User label:
  *                   mx_rot_rng
  */
#define mx_rot_rng_deinit mx_rng_deinit

  /* ***********************************************************
      SBS: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_sbs with HAL layer
  *         Name of the User label:
  *                   mx_rot_sbs
  */
#define mx_rot_sbs_deinit mx_sbs_deinit

  /* ***********************************************************
      TAMP: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_tamp with HAL layer
  *         Name of the User label:
  *                   mx_rot_tamp
  */
#define mx_rot_tamp_deinit mx_tamp_deinit

  /* ***********************************************************
      USART2: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_usart2_uart with HAL layer
  *         Name of the User label:
  *                   mx_rot_uart
  */
#define mx_rot_uart_deinit mx_usart2_uart_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

  /* ***********************************************************
      SAES: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for SAES
  *         Name of the User label:
  *                   mx_rot_saes
  *                   mysaes_1
  *                   mx_stfcf_mbedtls_alt_hal_aes
  * @retval hal_aes_handle_t Pointer on the handle on the SAES instance
  */
#define mx_rot_saes_gethandle mx_saes_gethandle

#define mysaes_1_gethandle mx_saes_gethandle

#define mx_stfcf_mbedtls_alt_hal_aes_gethandle mx_saes_gethandle

  /* ***********************************************************
      CRC: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for CRC
  *         Name of the User label:
  *                   mx_rot_crc
  * @retval hal_crc_handle_t Pointer on the handle on the CRC instance
  */
#define mx_rot_crc_gethandle mx_crc_gethandle

  /* ***********************************************************
      FLASH: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for FLASH
  *         Name of the User label:
  *                   mx_rot_flash
  * @retval hal_flash_handle_t Pointer on the handle on the FLASH instance
  */
#define mx_rot_flash_gethandle mx_flash_gethandle

  /* ***********************************************************
      HASH: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for HASH
  *         Name of the User label:
  *                   mx_rot_hash
  * @retval hal_hash_handle_t Pointer on the handle on the HASH instance
  */
#define mx_rot_hash_gethandle mx_hash_gethandle

  /* ***********************************************************
      ICACHE: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for ICACHE
  *         Name of the User label:
  *                   mx_rot_icache
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define mx_rot_icache_gethandle mx_icache_gethandle

  /* ***********************************************************
      PKA: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for PKA
  *         Name of the User label:
  *                   mx_rot_pka
  *                   mypka_1
  *                   mx_stfcf_mbedtls_alt_hal_pka
  * @retval hal_pka_handle_t Pointer on the handle on the PKA instance
  */
#define mx_rot_pka_gethandle mx_pka_gethandle

#define mypka_1_gethandle mx_pka_gethandle

#define mx_stfcf_mbedtls_alt_hal_pka_gethandle mx_pka_gethandle

  /* ***********************************************************
      RNG: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for RNG
  *         Name of the User label:
  *                   mx_rot_rng
  * @retval hal_rng_handle_t Pointer on the handle on the RNG instance
  */
#define mx_rot_rng_gethandle mx_rng_gethandle

  /* ***********************************************************
      USART2: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for USART2
  *         Name of the User label:
  *                   mx_rot_uart
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define mx_rot_uart_gethandle mx_usart2_uart_gethandle

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* *************************************************************
    SRAM2: RAM configuration global interrupt
    Alias on RAMCFG_IRQHandler and RAMCFG_IRQn
    ************************************************************* */
#define mx_rot_ramcfg_irqhandler RAMCFG_IRQHandler
#define MX_ROT_RAMCFG_IRQN       RAMCFG_IRQn

  /* *************************************************************
    TAMP: Tamper global interrupts
    Alias on TAMP_IRQHandler and TAMP_IRQn
    ************************************************************* */
#define mx_rot_tamp_irqhandler TAMP_IRQHandler
#define MX_ROT_TAMP_IRQN       TAMP_IRQn

  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
