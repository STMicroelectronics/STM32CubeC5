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
static hal_exti_handle_t hEXTI16;

/******************************************************************************/
/* Exported functions for PWR in HAL layer (SW instance PWR) */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Configuration of PVD (Programmable Voltage Detector)                     */
  /****************************************************************************/
  /* Enable PVD */
  HAL_PWR_EnableProgrammableVoltageDetector();

  /* Configure PVD EXTI Line */
  hal_exti_config_t exti_config;

  /* Initialize the EXTI for line 16 */
  HAL_EXTI_Init(&hEXTI16, HAL_EXTI_LINE_16);

  /* Set the trigger as RISING */
  exti_config.trigger = HAL_EXTI_TRIGGER_RISING;
  HAL_EXTI_SetConfig(&hEXTI16, &exti_config);

  /* Set line 16 Interrupt priority */
  HAL_CORTEX_NVIC_SetPriority(PWR_PVD_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(PWR_PVD_IRQn);

  return SYSTEM_OK;
}

/******************************************************************************/
/* EXTI get handle.                                                           */
/******************************************************************************/
hal_exti_handle_t *mx_pwr_exti16_gethandle(void)
{
  return &hEXTI16;
}

system_status_t mx_pwr_deinit(void)
{
  /* Deinit PVD EXTI Line */
  /* De-initialize the EXTI for line16 */
  HAL_EXTI_DeInit(&hEXTI16);

  /* Set line 16 Interrupt priority */
  HAL_CORTEX_NVIC_DisableIRQ(PWR_PVD_IRQn);

  return SYSTEM_OK;
}

/******************************************************************************/
/**                     Power Voltage Monitor                     **/
/******************************************************************************/
void PWR_PVD_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hEXTI16);
}
