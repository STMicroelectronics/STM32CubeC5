/**
  ******************************************************************************
  * @file    mx_openbl_user.h
  * @brief   Header for mx_openbl_user.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_freertos_license.md file
  * in the same directory as the generated code.
  * If no mx_freertos_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_OPENBL_USER_H
#define MX_OPENBL_USER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

#include "mx_iwdg.h"

#include "mx_usart2.h"

#include "mx_i2c1.h"

#include "mx_spi2.h"

#include "mx_flash.h"


/* Exported types ------------------------------------------------------------*/
#define OPENBL_STACK_SIZE              0x8C00U   /* RAM used by the Open Bootloader stack */

/* Exported constants --------------------------------------------------------*/
#define OPENBL_USART_ITF               USART2

#define OPENBL_I2C_ITF                 I2C1

#define OPENBL_SPI_ITF                 SPI2

#define OPENBL_FLASH_ITF               mx_flash_gethandle()

#define OPENBL_SYS_IWDG                IWDG

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int32_t openbl_user_process_communication(openbl_obj_t *p_openbl_obj);
void SPI2_IRQHandler(void);


/* External variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_OPENBL_USER_H */
