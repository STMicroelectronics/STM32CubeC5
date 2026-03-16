/**
  ******************************************************************************
  * @file           : mx_system.c
  * @brief          : STM32 system program body
  *                   Applicative target system level initialization
  *                   (system clock, cache, TZ...) and system level peripherals
  *                   initialization. mx_system_init() service is called by the main
  *                   before jumping the example entry point.
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

/* Includes ------------------------------------------------------------------*/
#include "mx_system.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

system_status_t mx_system_init(void)
{
  if (pre_system_init_hook() != SYSTEM_OK)
  {
    return SYSTEM_PRESYSTEM_ERROR;
  }

  /*
    CORTEX MPU initialization in case of isolation is not activated
  */
  if (mx_cortex_mpu_init() != SYSTEM_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /*
    Interruptions section
  */
  if (mx_cortex_nvic_init() != SYSTEM_OK)
  {
    return SYSTEM_INTERRUPTS_ERROR;
  }

  /*
    ICACHE section
  */
  if (mx_icache_init() == NULL)
  {
    return SYSTEM_STARTUP_ERROR;
  }

  LL_ICACHE_Enable(ICACHE);

  /*
    Clock system section
  */
  if (mx_rcc_init() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* peripherals clock configuration and activation
    is centralized: no clock activation/deactivation in pppi_init */

  if (mx_rcc_peripherals_clock_config() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /*
    Peripheral init section
  */
  /*
  */
  if (mx_gpio_default_init() != SYSTEM_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /** mx_i2c1_i2c_init()has been generated,
    * (mx_i2cx_init)
    * but it is expected that application will call it when best needed
    * according to application needs.
    * See Cube code generator options: Generate and call Initialization function
    */

  if (post_system_init_hook() != SYSTEM_OK)
  {
    return SYSTEM_POSTSYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  User hook function called before the HAL_Init() function
  * @retval system_status_t System status
  */
__WEAK system_status_t pre_system_init_hook(void)
{
  /* NOTE : This function must not be modified. When the callback is needed,
            the pre_system_init_hook can be implemented in the user file
   */
  return SYSTEM_OK;
}

/**
  * @brief  User hook function called after the HAL_Init() and Peripheral init functions
  * @retval system_status_t System status
  */
__WEAK system_status_t post_system_init_hook(void)
{
  /* NOTE : This function must not be modified. When the callback is needed,
            the post_system_init_hook can be implemented in the user file
   */
  return SYSTEM_OK;
}
