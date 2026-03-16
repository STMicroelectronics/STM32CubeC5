/**
  ******************************************************************************
  * @file           : mx_cortex_mpu.c
  * @brief          : CORTEX_MPU Peripheral initialization
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
#include "mx_cortex_mpu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/******************************************************************************/
/* Exported functions for MPU in LL layer (SW instance MyCORTEX_MPU_1) */
/******************************************************************************/

system_status_t mx_cortex_mpu_init(void)
{
  /* Disables the MPU */
  ARM_MPU_Disable();

  /*
     Initializes and configures the MPU attributes
  */
  ARM_MPU_SetMemAttr(0, (MPU_ATTR_NORMAL_OUTER_NON_CACHEABLE << 4U));

  /*
     Initializes and configures the MPU Region
  */

  ARM_MPU_SetRegion(0, (0x8FFE000
                    | (ARM_MPU_AP_(ARM_MPU_AP_RO, ARM_MPU_AP_NP) << MPU_RBAR_AP_Pos)
                    | (ARM_MPU_XN << MPU_RBAR_XN_Pos)),
                    ((0x8FFFFFF & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));

  STM32_WRITE_REG(MPU->RNR, 0);
  STM32_SET_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);

  /* Enables the MPU */
  ARM_MPU_Enable(0x00 | MPU_CTRL_PRIVDEFENA_Msk);

  return SYSTEM_OK;
}

void mx_cortex_mpu_deinit(void)
{
  /* Disables the MPU */
  ARM_MPU_Disable();

  STM32_WRITE_REG(MPU->RNR, 0);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 1);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 2);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 3);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 4);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 5);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 6);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
  STM32_WRITE_REG(MPU->RNR, 7);
  STM32_CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);

  /*
     Deinitializes and configures the MPU attributes to default
  */
  ARM_MPU_SetMemAttr(0, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(1, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(2, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(3, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(4, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(5, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(6, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);
  ARM_MPU_SetMemAttr(7, ARM_MPU_ATTR_DEVICE_nGnRnE << 2U);

  /*
     Deinitializes and configures the MPU Region to default
  */
  ARM_MPU_SetRegion(0, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(1, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(2, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(3, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(4, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(5, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(6, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));
  ARM_MPU_SetRegion(7, (0x00000000
                    | ((ARM_MPU_AP_RW | ARM_MPU_AP_PO) << MPU_RBAR_AP_Pos)
                    | (0x00 << MPU_RBAR_XN_Pos)),
                    ((0x00000000 & 0xFFFFFFE0U)
                    | (0 << MPU_RLAR_AttrIndx_Pos)));}