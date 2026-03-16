/**
  ******************************************************************************
  * @file           : mx_usb_drd_fs.h
  * @brief          : Header for mx_usb_drd_fs.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_USB_DRD_FS_H
#define MX_USB_DRD_FS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for USB_DRD_FS DEVICE (PCD) in HAL layer */
/******************************************************************************/
/**
  * @brief mx_usb_drd_fs_device init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_pcd_handle_t *mx_usb_drd_fs_device_init(void);

/**
  * @brief  De-initialize usb_drd_fs instance and return it.
  */
void mx_usb_drd_fs_device_deinit(void);

/**
  * @brief  Get the USB_DRD_FS object.
  * @retval Pointer on the USB_DRD_FS Handle
  */
hal_pcd_handle_t *mx_usb_drd_fs_device_gethandle(void);

/**
  * @brief Handling USB event interrupt request
  * @param None
  * @retval None
  */
void USB_DRD_FS_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USB_DRD_FS_H */
