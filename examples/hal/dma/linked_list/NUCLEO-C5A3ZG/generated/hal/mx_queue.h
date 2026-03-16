/**
  **********************************************************************************************************************
  * @file           : mx_queue.h
  * @brief          : Header for mx_queue.c file.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *********************************************************************************************************************
  */

/* Define to prevent recursive inclusion ----------------------------------------------------------------------------*/
#ifndef MX_QUEUE_H
#define MX_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported functions ---------------------------------------------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for QUEUE in HAL layer */
/******************************************************************************/
  /**
  * @brief Queue Initialization Function
  * @param None
  * @retval Pointer on the QUEUE structure, NULL in case of error
  */
hal_q_t *mx_queue1_init(void);
hal_q_t *mx_queue1_build(void);

/**
  * @brief  DeInitialize the DmaQueue through HAL layer.
  */
void mx_queue1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_QUEUE_H */
