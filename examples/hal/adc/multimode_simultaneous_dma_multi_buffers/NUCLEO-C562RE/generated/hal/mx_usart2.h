/**
  ******************************************************************************
  * file           : mx_usart2.h
  * brief          : Header for mx_usart2.c file.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_USART2_H
#define STM32_USART2_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Private define ------------------------------------------------------------*/
#define USART2_BAUDRATE 115200
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief usart2 init function
  * This function configures the hardware resources used in this example
  * retval pointer to handle or NULL in case of failure
  */
void *usart2_instance1_cfg1_init(void);

/**
  * brief  De-initialize usart2 instance and return it.
  * retval None
  */
void usart2_deinit(void);

/**
  * brief  Get the USART2 object.
  * retval Pointer on the USART2 Handle
  */
void *usart2_gethandle(void);


#ifdef __cplusplus
}
#endif

#endif /* STM32_USART2_H */
