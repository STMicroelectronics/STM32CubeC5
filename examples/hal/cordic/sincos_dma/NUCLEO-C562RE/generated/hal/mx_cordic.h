/**
  ******************************************************************************
  * @file           : mx_cordic.h
  * @brief          : Header for mx_cordic.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_CORDIC_H
#define MX_CORDIC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for CORDIC in HAL layer */
/******************************************************************************/

/**
  * @brief mx_cordic_cfg1_hal_init init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_cordic_handle_t *mx_cordic_cfg1_hal_init(void);

/**
  * @brief  De-initialize mx_cordic_cfg1_hal_deinit instance and return it.
  * @retval None
  */
void mx_cordic_cfg1_hal_deinit(void);

/**
  * @brief  Get the mx_cordic_hal_gethandle object.
  * @retval Pointer on the mx_cordic_halHandle
  */
hal_cordic_handle_t *mx_cordic_hal_gethandle(void);

/******************************************************************************/
/*                     CORDIC global interrupt                    */
/******************************************************************************/
void CORDIC_IRQHandler(void);

/******************************************************************************/
/*                     LPDMA1 channel0 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void);

/******************************************************************************/
/*                     LPDMA1 channel1 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH1_IRQHandler(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CORDIC_H */
