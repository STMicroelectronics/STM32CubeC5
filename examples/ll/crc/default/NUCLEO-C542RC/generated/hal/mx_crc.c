/**
  ******************************************************************************
  * @file           : mx_crc.c
  * @brief          : CRC Peripheral initialization
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
#include "mx_crc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for CRC (SW instance MyCRC_1 in LL) layer */
/******************************************************************************/
CRC_TypeDef *mx_crc_init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);
  /*
    Default values are used for the polynomial and init values.
    Equivalent LL functions have been commented:

    - CRC polynomial: Reset value is 0x04C11DB7 (32-bit polynomial)
    LL_CRC_SetPolynomialCoef(CRC, 0x04C11DB7);
    LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLY_SIZE_32B);

    - CRC init value: Reset value is 0xFFFFFFFF
    LL_CRC_SetInitialData(CRC, 0xFFFFFFFF);

    - Invert Input & Output Data: Reset values are LL_CRC_INDATA_REVERSE_NONE and LL_CRC_OUTDATA_REVERSE_NONE
    LL_CRC_SetDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE, LL_CRC_OUTDATA_REVERSE_NONE);

  */
  return CRC;
}

void mx_crc_deinit(void)
{
  LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_CRC);
  LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_CRC);

  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_CRC);
}
