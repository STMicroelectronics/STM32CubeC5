/**
  ******************************************************************************
  * file           : mx_hal_def.h
  * brief          : Exporting peripherals initialization
  *                  Include entry for the target folder to the application.
  ******************************************************************************
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
#ifndef STM32_MCU_DEF_H
#define STM32_MCU_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32_hal.h"
#include "mx_gpio.h"
#include "mx_usart2.h"
#include "mx_adc1.h"
#include "mx_adc2.h"
#include "mx_tim2.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MX_STATUS_LED                  MX_STATUS_LED /* Input parameter to the LED  API */
#define MX_STATUS_LED_GPIO_PORT        HAL_GPIOA
#define MX_STATUS_LED_PIN              HAL_GPIO_PIN_5
#define MX_STATUS_LED_ACTIVE_STATE     HAL_GPIO_PIN_SET
#define MX_STATUS_LED_INACTIVE_STATE   HAL_GPIO_PIN_RESET
#define MX_STATUS_LED_INIT_STATE       HAL_GPIO_PIN_RESET

/* Exported macros -----------------------------------------------------------*/

#define mx_system_init  system_init

/* enum values will be generated in mx_def.h */
typedef enum
{
  SYSTEM_OK = 0,                    /* System initialization successfully       */
  SYSTEM_PRESYSTEM_ERROR,           /* Error during System pre-initialization   */
  SYSTEM_STARTUP_ERROR,             /* Error during startup initialization      */
  SYSTEM_INTERRUPTS_ERROR,          /* Error during interrupts initialization   */
  SYSTEM_CLOCK_ERROR,               /* Error during clock initialization        */
  SYSTEM_RESOURCES_ISOLATION_ERROR, /* Error during Cortex MPU initialization   */
  SYSTEM_POWER_ERROR,               /* Error during power initialization        */
  SYSTEM_PERIPHERAL_ERROR,          /* Error during peripherals initialization  */
  SYSTEM_POSTSYSTEM_ERROR           /* Error during System post-initialization  */
} system_status_t;

/* Aliases to initialization functions */
#define mx_example_adc_master_init    mx_adc1_cfg1_hal_init
#define mx_example_adc_slave_init    mx_adc2_cfg1_hal_init
#define mx_basic_stdio_init    usart2_cfg1_hal_uart_init
#define mx_status_led_init     gpio_cfg1_init
#define mx_example_tim_init  tim2_cfg1_init


/* Aliases to De-Initialization functions */
#define mx_example_adc_master_deinit  mx_adc1_cfg1_hal_deinit
#define mx_example_adc_slave_deinit  mx_adc2_cfg1_hal_deinit
#define mx_example_tim_deinit  tim2_deinit


/* Aliases to get Object functions */
#define mx_example_adc1_gethandle  adc1_gethandle
#define mx_example_adc2_gethandle  adc2_gethandle
#define mx_basic_stdio_gethandle   usart2_gethandle

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* STM32_MCU_DEF_H */
