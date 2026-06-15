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
#include "mx_i2c1.h"
#include "mx_icache.h"
#include "mx_iwdg.h"
#include "mx_usb_drd_fs.h"
#include "mx_rcc.h"
#include "mx_spi2.h"
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
    FLASH: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_flash_init
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

  /* *************************************************************
    I2C1: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_i2c1_i2c_init
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_init
    ************************************************************* */

  /* *************************************************************
    IWDG: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_iwdg_init
    ************************************************************* */

  /* ***********************************************************
      USB_DRD_FS_DEVICE: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_usb_drd_fs_device with HAL layer
  *         Name of the User label:
  *                   mx_example_pcd
  * @retval hal_pcd_handle_t Pointer on the handle on the USB_DRD_FS_DEVICE instance
  */
#define mx_example_pcd_init mx_usb_drd_fs_device_init

  /* *************************************************************
    RCC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_rcc_init
    ************************************************************* */

  /* *************************************************************
    SPI2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_spi2_init
    ************************************************************* */

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

  /* *************************************************************
    FLASH: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_flash_deinit
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

  /* *************************************************************
    I2C1: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_i2c1_i2c_deinit
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* *************************************************************
    IWDG: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_iwdg_deinit
    ************************************************************* */

  /* ***********************************************************
      USB_DRD_FS_DEVICE: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_usb_drd_fs_device with HAL layer
  *         Name of the User label:
  *                   mx_example_pcd
  */
#define mx_example_pcd_deinit mx_usb_drd_fs_device_deinit

  /* *************************************************************
    SPI2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_spi2_deinit
    ************************************************************* */

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_deinit
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

  /* *************************************************************
    FLASH: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_flash_gethandle
    ************************************************************* */

  /* *************************************************************
    I2C1: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_i2c1_i2c_gethandle
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_gethandle
    ************************************************************* */

  /* *************************************************************
    IWDG: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_iwdg_gethandle
    ************************************************************* */

  /* ***********************************************************
      USB_DRD_FS_DEVICE: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for USB_DRD_FS_DEVICE
  *         Name of the User label:
  *                   mx_example_pcd
  * @retval hal_pcd_handle_t Pointer on the handle on the USB_DRD_FS_DEVICE instance
  */
#define mx_example_pcd_gethandle mx_usb_drd_fs_device_gethandle

  /* *************************************************************
    SPI2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_spi2_gethandle
    ************************************************************* */

  /* *************************************************************
    USART2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_usart2_uart_gethandle
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* *************************************************************
    SPI2: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for SPI2_IRQHandler and SPI2_IRQn
    ************************************************************* */

  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
