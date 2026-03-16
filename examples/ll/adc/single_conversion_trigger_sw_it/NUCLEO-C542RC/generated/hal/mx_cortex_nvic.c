/**
  ******************************************************************************
  * @file           : mx_cortex_nvic.c
  * @brief          : CORTEX_NVIC Peripheral initialization
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
#include "mx_cortex_nvic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for NVIC in LL layer (SW instance MyCORTEX_NVIC_1) */
/******************************************************************************/
system_status_t mx_cortex_nvic_init(void)
{
  /* Enable DebugMonitor exception */
  STM32_SET_BIT(DCB->DEMCR, DCB_DEMCR_MON_EN_Msk);

  /* Configure the Priority grouping */
  NVIC_SetPriorityGrouping(3);

  /* System service call via SWI instruction */
  NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

  /* Debug Monitor */
  NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

  /* Pendable request for system service */
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

  return SYSTEM_OK;
}

/******************************************************************************/
/* System service call via SWI instruction is managed directly in user code.  */
/******************************************************************************/
/* void SVC_Handler(void)
{
}
  */

/******************************************************************************/
/*   Pendable request for system service is managed directly in user code.    */
/******************************************************************************/
/* void PendSV_Handler(void)
{
}
  */

/******************************************************************************/
/*              Debug Monitor is managed directly in user code.               */
/******************************************************************************/
/* void DebugMonitor_Handler(void)
{
}
  */
