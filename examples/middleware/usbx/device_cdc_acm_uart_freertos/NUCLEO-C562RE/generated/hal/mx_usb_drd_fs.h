/**
  ******************************************************************************
  * @file           : mx_usb_drd_fs.h
  * @brief          : Header for mx_usb_drd_fs.c file.
  ******************************************************************************
  * @attention
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
#ifndef STM32_USB_DRD_FS_H
#define STM32_USB_DRD_FS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_gpio_default.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

void *mx_pcd_usb_drd_fs_cfg1_init(void);
void mx_pcd_usb_drd_fs_cfg1_deinit(void);
void *mx_usb_pcd_gethandle(void);
void USB_DRD_FS_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32_USB_DRD_FS_H */
