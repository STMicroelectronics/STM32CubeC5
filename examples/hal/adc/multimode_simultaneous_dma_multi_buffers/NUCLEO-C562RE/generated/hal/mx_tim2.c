/**
  ******************************************************************************
  * file    mx_tim2.c
  * brief   Initialisation of all the peripherals used in this project
  *          File per peripheral instance that is handling the peripheral
  *          initialization and the IRQ handlers. Init parameters provided
  *          as a set of defines configurable/generated for a given target
  *          through STM32CubeMX2.
  *
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

/* Includes ------------------------------------------------------------------*/
#include "mx_tim2.h"
#include "mx_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables  -------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
/* TIM handle */
hal_tim_handle_t hTIM;

/**
  * @brief Timer Initialization Function
  * @param None
  * @retval -1 in case of failure
  */
void *tim2_cfg1_init(void)
{
  HAL_RCC_TIM2_EnableClock();

  hal_tim_config_t sTimConfig;

  if (HAL_TIM_Init(&hTIM, HAL_TIM2) != HAL_OK)
  {
    return NULL;
  }

  sTimConfig.prescaler = PRESCALER_VALUE;
  sTimConfig.counter_mode = HAL_TIM_COUNTER_UP;
  sTimConfig.period = PERIOD_VALUE;
  sTimConfig.repetition_counter = 0;
  sTimConfig.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  sTimConfig.clock_sel.trigger = HAL_TIM_TRIG_ITR0;

  /* Sampling Clock */
  if (HAL_TIM_SetDTSPrescaler(&hTIM, HAL_TIM_DTS_DIV1) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_SetDTS2Prescaler(&hTIM, HAL_TIM_DTS2_DIV1) != HAL_OK)
  {
    return NULL;
  }


  if (HAL_TIM_SetConfig(&hTIM, &sTimConfig) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_SetTriggerOutput(&hTIM, HAL_TIM_TRGO_UPDATE) != HAL_OK)
  {
    return NULL;
  }

  return &hTIM;
}

/**
  * @brief  Get the tim2 object.
  * @param None
  * @retval Pointer on the TIM1 Handle
  */
void *tim2_gethandle(void)
{
  return (void *)&hTIM;
}

/**
  * @brief Timer peripheral deinitialization function
  * @param None
  * @retval -1 in case of failure
  */
void tim2_deinit(void)
{
  HAL_RCC_TIM2_DisableClock();
  /* Deinitialization the timer peripheral */
  HAL_TIM_DeInit(&hTIM);
}
