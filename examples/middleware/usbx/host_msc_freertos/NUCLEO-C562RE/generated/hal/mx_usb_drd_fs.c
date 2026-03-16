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
static hal_hcd_handle_t gh_hcd_usb_drd_fs;

/* GPIO initialization functions */
void *mx_hcd_usb_drd_fs_cfg1_init(void)
{
  hal_hcd_config_t  config_hcd;

  /* Peripheral USB clock enable */
  HAL_RCC_USB_EnableClock();


  /* Configure gh_hcd_usb_drd_fs */
  if (HAL_HCD_Init(&gh_hcd_usb_drd_fs, HAL_HCD_DRD_FS) != HAL_OK)
  {
      return NULL;
  }

  config_hcd.hcd_speed = HAL_HCD_SPEED_FS;
  config_hcd.phy_interface = HAL_HCD_PHY_EMBEDDED_FS;
  if (HAL_HCD_SetConfig(&gh_hcd_usb_drd_fs, &config_hcd) != HAL_OK)
  {
      return NULL;
  }

  /* USB_DRD_FS interrupt Init */
  HAL_CORTEX_NVIC_SetPriority(USB_DRD_FS_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_6, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USB_DRD_FS_IRQn);

  return (void *)&gh_hcd_usb_drd_fs;
}

void mx_hcd_usb_drd_fs_cfg1_deinit(void)
{
  /* DeInit USB interrupts */
  HAL_CORTEX_NVIC_DisableIRQ(USB_DRD_FS_IRQn);
}

void *mx_usb_hcd_gethandle(void)
{
  return (void *)&gh_hcd_usb_drd_fs;
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
  HAL_HCD_IRQHandler(&gh_hcd_usb_drd_fs);
}