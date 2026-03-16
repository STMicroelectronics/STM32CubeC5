/**
  ******************************************************************************
  * @file           : mx_cordic.c
  * @brief          : CORDIC Peripheral initialization
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
#include "mx_cordic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_cordic_handle_t hCORDIC;

/******************************************************************************/
/* Exported functions for CORDIC in HAL layer (SW instance MyCORDIC_1) */
/******************************************************************************/
hal_cordic_handle_t *mx_cordic_init(void)
{
  hal_cordic_config_t cordic_config;

  if (HAL_CORDIC_Init(&hCORDIC, HAL_CORDIC) != HAL_OK)
  {
    return NULL;
  }
  /* configure RCC clock activation*/
  HAL_RCC_CORDIC_EnableClock();

  /* Basic Configuration ***************************************************/
  cordic_config.function       = HAL_CORDIC_FUNCTION_SINE;
  cordic_config.in_width       = HAL_CORDIC_IN_WIDTH_32_BIT;
  cordic_config.nb_arg         = HAL_CORDIC_NB_ARG_1;
  cordic_config.out_width      = HAL_CORDIC_OUT_WIDTH_32_BIT;
  cordic_config.nb_result      = HAL_CORDIC_NB_RESULT_2;
  cordic_config.scaling_factor = HAL_CORDIC_SCALING_FACTOR_0;
  cordic_config.precision      = HAL_CORDIC_PRECISION_6_CYCLE;
  if (HAL_CORDIC_SetConfig(&hCORDIC,&cordic_config) != HAL_OK)
  {
    return NULL;
  }

  return &hCORDIC;
}
void mx_cordic_deinit(void)
{
  /* CORDIC De-intialization */
  (void)HAL_CORDIC_DeInit(&hCORDIC);
  /* Disable CORDIC Clock */
  HAL_RCC_CORDIC_Reset();

  HAL_RCC_CORDIC_DisableClock();
}

hal_cordic_handle_t *mx_cordic_gethandle(void)
{
  return &hCORDIC;
}
