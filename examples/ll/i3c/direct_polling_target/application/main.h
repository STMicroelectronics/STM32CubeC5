/**
  ******************************************************************************
  * file           : main.h
  * brief          : Header for main.c.
  *                  This file contains the common defines of the application.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "mx_system.h"  /* target-specific generated code providing system services */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define LED_ON()  LL_GPIO_WriteOutputPin(MX_STATUS_LED_PORT, MX_STATUS_LED_PIN, MX_STATUS_LED_ACTIVE_STATE)

#define LED_OFF() LL_GPIO_WriteOutputPin(MX_STATUS_LED_PORT, MX_STATUS_LED_PIN, MX_STATUS_LED_INACTIVE_STATE)

/* Exported functions ------------------------------------------------------- */
void HardFault_Handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_H */
