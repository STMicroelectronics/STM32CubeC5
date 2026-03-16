/**
  ******************************************************************************
  * @file           : mx_gpdma1.h
  * @brief          : Header for mx_gpdma1.c file.
  ******************************************************************************
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_GPDMA1_H
#define MX_GPDMA1_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/**
  * @brief dma1 init function
  * This function configures the hardware resources used in this example
  * @param None
  * @retval pointer to handle or NULL in case of failure
  */
hal_dma_handle_t *dma1_channel0_cfg1_init(void);

/**
  * @brief dma1 init function
  * This function configures the hardware resources used in this example
  * @param None
  * @retval pointer to handle or NULL in case of failure
  */
hal_dma_handle_t *dma1_channel1_cfg1_init(void);

/**
  * @brief dma1 peripheral deinitialization function
  * @param None
  * @retval None
  */
void dma1_channel0_deinit(void);

/**
  * @brief dma1 peripheral deinitialization function
  * @param None
  * @retval None
  */
void dma1_channel1_deinit(void);

/**
  * @brief  Get the DMA1 object.
  * @param None
  * @retval Pointer on the DMA1 Handle
  */
hal_dma_handle_t *dma1_channel0_gethandle(void);

/**
  * @brief  Get the DMA1 object.
  * @param None
  * @retval Pointer on the DMA1 Handle
  */
hal_dma_handle_t *dma1_channel1_gethandle(void);

/******************************************************************************/
/*                     Interruption and Exception Handlers                    */
/******************************************************************************/
/**
  * @brief LPDMA1 channel 0 global interrupt
  * @param None
  * @retval None
  */
void LPDMA1_CH0_IRQHandler(void);

/**
  * @brief LPDMA1 Channel 1 global interrupt
  * @param None
  * @retval None
  */
void LPDMA1_CH1_IRQHandler(void);



#ifdef __cplusplus
}
#endif

#endif /* MX_GPDMA1_H */
