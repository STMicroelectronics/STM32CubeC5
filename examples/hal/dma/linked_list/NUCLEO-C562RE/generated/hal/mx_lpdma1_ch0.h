/**
  **********************************************************************************************************************
  * @file           : mx_lpdma1_ch0.h
  * @brief          : Header for mx_lpdma1_ch0.c file.
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
#ifndef MX_LPDMA1_CH0_H
#define MX_LPDMA1_CH0_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported functions ---------------------------------------------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for DMA in HAL layer */
/******************************************************************************/
/**
  * @brief mx_lpdma1_ch0_cfg1_hal init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_dma_handle_t *mx_lpdma1_ch0_hal_init(void);

/**
  * @brief  De-initialize lpdma1_ch0 instance and return it.
  */
void mx_lpdma1_ch0_hal_deinit(void);

/**
  * @brief  Get the LPDMA1_CH0 object.
  * @retval Pointer on the LPDMA1_CH0 Handle
  */
hal_dma_handle_t *mx_lpdma1_ch0_hal_gethandle(void);


/******************************************************************************/
/*                     LPDMA1 channel0 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPDMA1_CH0_H */
