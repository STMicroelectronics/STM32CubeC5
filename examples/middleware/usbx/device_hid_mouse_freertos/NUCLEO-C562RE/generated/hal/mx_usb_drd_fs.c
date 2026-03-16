/**
  ******************************************************************************
  * @file           : mx_usb_drd_fs.c
  * @brief          : USB_DRD_FS Peripheral initialization
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

/* Includes ------------------------------------------------------------------*/
#include "mx_usb_drd_fs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_pcd_handle_t hUSB_DRD_FS_PCD;

/******************************************************************************/
/* Exported functions for USB_DRD_FS DEVICE (PCD) in HAL layer */
/******************************************************************************/

hal_pcd_handle_t *mx_usb_drd_fs_device_init(void)
{
  hal_pcd_config_t pcd_config;

  /* Configure gh_pcd_USB_DRD_FS */
  if (HAL_PCD_Init(&hUSB_DRD_FS_PCD, HAL_PCD_DRD_FS) != HAL_OK)
  {
    return NULL;
  }

  /* Peripheral USB clock enable */
  HAL_RCC_USB_EnableClock();

  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_HSIDIV3) != HAL_OK)
  {
    return NULL;
  }

  pcd_config.phy_interface = HAL_PCD_PHY_EMBEDDED_FS;
  pcd_config.pcd_speed = HAL_PCD_SPEED_FS;
  pcd_config.battery_charging_enable = HAL_PCD_BCD_DISABLED;
  pcd_config.lpm_enable = HAL_PCD_LPM_DISABLED;
  pcd_config.bulk_doublebuffer_enable = HAL_PCD_BULK_DB_DISABLED;

  HAL_PCD_SetConfig(&hUSB_DRD_FS_PCD, &pcd_config);

  HAL_PCD_PMAConfig(&hUSB_DRD_FS_PCD, 0x80, HAL_PCD_SNG_BUF, 0x20);
  HAL_PCD_PMAConfig(&hUSB_DRD_FS_PCD, 0x0, HAL_PCD_SNG_BUF, 0x60);
  HAL_PCD_PMAConfig(&hUSB_DRD_FS_PCD, 0x81, HAL_PCD_SNG_BUF, 0xA0);

  /* Enable the interruption for PCD */
  HAL_CORTEX_NVIC_SetPriority(USB_DRD_FS_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_6, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USB_DRD_FS_IRQn);

  return &hUSB_DRD_FS_PCD;
}
void mx_usb_drd_fs_device_deinit(void)
{
  /* Deinititialize hUSB_DRD_FS_PCD */
  HAL_PCD_DeInit(&hUSB_DRD_FS_PCD);
}

/**
  * @brief Get USB PCD handle
  * @param None
  * @retval USB PCD handle
  */
hal_pcd_handle_t *mx_usb_drd_fs_device_gethandle(void)
{
  return &hUSB_DRD_FS_PCD;
}

/******************************************************************************/
/*                     Interruption and Exception Handlers                    */
/******************************************************************************/
void USB_DRD_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hUSB_DRD_FS_PCD);
}
