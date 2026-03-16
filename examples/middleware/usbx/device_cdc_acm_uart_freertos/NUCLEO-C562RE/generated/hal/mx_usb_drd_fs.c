/**
  ******************************************************************************
  * @file           : mx_usb_drd_fs.c
  * @brief          : STM32 usb drd fs program body
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
/* Includes ------------------------------------------------------------------*/
#include "mx_usb_drd_fs.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_pcd_handle_t gh_pcd_usb_drd_fs;

/* GPIO initialization functions */
void *mx_pcd_usb_drd_fs_cfg1_init(void)
{
  hal_pcd_config_t  config_pcd;

  /* Peripheral USB clock enable */
  HAL_RCC_USB_EnableClock();


  /* Configure gh_pcd_usb_drd_fs */
  if (HAL_PCD_Init(&gh_pcd_usb_drd_fs, HAL_PCD_DRD_FS) != HAL_OK)
  {
      return NULL;
  }

  config_pcd.pcd_speed = HAL_PCD_SPEED_FS;
  config_pcd.phy_interface = HAL_PCD_PHY_EMBEDDED_FS;
  config_pcd.sof_enable = HAL_PCD_SOF_DISABLED;
  config_pcd.lpm_enable = HAL_PCD_LPM_DISABLED;
  config_pcd.battery_charging_enable = HAL_PCD_BCD_DISABLED;
  config_pcd.bulk_doublebuffer_enable = HAL_PCD_BULK_DB_DISABLED;
  config_pcd.vbus_sensing_enable = HAL_PCD_VBUS_SENSE_DISABLED;
  if (HAL_PCD_SetConfig(&gh_pcd_usb_drd_fs, &config_pcd) != HAL_OK)
  {
      return NULL;
  }

HAL_PCD_PMAConfig(&gh_pcd_usb_drd_fs, 0x00, HAL_PCD_SNG_BUF, 0x14);
HAL_PCD_PMAConfig(&gh_pcd_usb_drd_fs, 0x80, HAL_PCD_SNG_BUF, 0x54);
HAL_PCD_PMAConfig(&gh_pcd_usb_drd_fs, 0x81, HAL_PCD_SNG_BUF, 0x94);
HAL_PCD_PMAConfig(&gh_pcd_usb_drd_fs, 0x01, HAL_PCD_SNG_BUF, 0xD4);
HAL_PCD_PMAConfig(&gh_pcd_usb_drd_fs, 0x82, HAL_PCD_SNG_BUF, 0x114);

  /* USB_DRD_FS interrupt Init */
  HAL_CORTEX_NVIC_SetPriority(USB_DRD_FS_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_6, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USB_DRD_FS_IRQn);

  return (void *)&gh_pcd_usb_drd_fs;
}

void mx_pcd_usb_drd_fs_cfg1_deinit(void)
{
  /* DeInit USB interrupts */
  HAL_CORTEX_NVIC_DisableIRQ(USB_DRD_FS_IRQn);
}

void *mx_usb_pcd_gethandle(void)
{
  return (void *)&gh_pcd_usb_drd_fs;
}

/******************************************************************************/
/*                     Interruption and Exception Handlers                    */
/******************************************************************************/
/**
  * @brief Handling USB event interrupt request
  * @param None
  * @retval None
  */
void USB_DRD_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&gh_pcd_usb_drd_fs);
}