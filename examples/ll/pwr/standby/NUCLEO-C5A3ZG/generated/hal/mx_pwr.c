/**
  ******************************************************************************
  * @file           : mx_pwr.c
  * @brief          : PWR Peripheral initialization
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
#include "mx_pwr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for PWR in LL layer (SW instance PWR) */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Wakeup Pins Configurations                                               */
  /****************************************************************************/
  /* Configuration of selected Wakeup pins with pull NO and HIGH polarity */
  /* LL_PWR_SetWakeUpPinPolarity(LL_PWR_WAKEUP_PIN_4, LL_PWR_WAKEUP_PIN_POLARITY_HIGH); */ /* Configuration matches register reset state at startup. */
  /* LL_PWR_SetWakeUpPinPull(LL_PWR_WAKEUP_PIN_4, LL_PWR_WAKEUP_PIN_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Enable all configured Wakeup Pins                                        */
  /****************************************************************************/
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}

system_status_t mx_pwr_deinit(void)
{
  /****************************************************************************/
  /* Disable all configured Wakeup Pins                                       */
  /****************************************************************************/
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}
