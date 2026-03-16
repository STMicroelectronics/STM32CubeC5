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
#include "mx_rcc.h"
#include "mx_system.h"
#include "mx_crc.h"
#include "mx_flash.h"
#include "mx_icache.h"
#include "mx_usart2.h"
#include "mx_gpio_default.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_icache_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyICACHE_1
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define myicache_1_init mx_icache_cfg1_hal_init


/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_example_gpio
  * @retval hal_gpio_handle_t Pointer on the handle on the gpio_default instance
  */
#define mygpio_1_init mx_gpio_default_cfg1_hal_init

#define mx_example_gpio_init mygpio_1_init


/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_icache_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyICACHE_1
  */
#define myicache_1_deinit mx_icache_cfg1_hal_deinit

#define mx_example_flash_init           flash_instance0_cfg1_init

#define MX_EEPROM_FLASH_HANDLE     mx_hal_flash_gethandle

/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_example_gpio
  */


#define mx_status_led_init            mx_gpio_default_cfg1_hal_deinit

#define mygpio_1_deinit mx_gpio_default_cfg1_hal_deinit

#define mx_example_gpio_deinit mygpio_1_deinit

/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_crc_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_example_gpio
  */
#define mx_status_led_init            mx_gpio_default_cfg1_hal_deinit

#define mygpio_1_deinit mx_gpio_default_cfg1_hal_deinit

#define mx_example_gpio_deinit mygpio_1_deinit

/* *********************************************************** */
/*        mx_CRC : aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_crc_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_example_gpio
  */
#define MX_EEPROM_CRC_HANDLE mx_crc_hal_crc_gethandle

#define mycrc_init crc_instance0_cfg1_init

#define mx_crc_deinit crc_deinit

/* *********************************************************** */
/*        USART2_MyUART_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_usart2_cfg1_hal_uart with HAL layer
  *         Name of the User label:
  *                   MyUART_1
  *                   mx_basic_stdio
  * @retval hal_uart_handle_t Pointer on the handle on the USART2 instance
  */
#define myuart_1_init mx_usart2_cfg1_hal_uart_init

#define mx_basic_stdio_gethandle mx_usart2_hal_uart_gethandle

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* *********************************************************** */
/*        GPIO: aliases for get EXTI object functions */
/* *********************************************************** */

/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for get handle functions */
/* *********************************************************** */

/**
  * @brief  Get the HAL handle for ICACHE
  *         Name of the User label:
  *                   MyICACHE_1
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define myicache_1_gethandle mx_icache_hal_gethandle


/* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
