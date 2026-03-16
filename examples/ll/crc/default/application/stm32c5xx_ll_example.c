/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : CRC default polynomial with LL API
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

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

inline uint32_t CRC_Calculate(const uint32_t *p_data, uint32_t size_word)
{
  uint32_t count;

  /* Reset the MX_CRCx calculation unit before start a new calculation */
  LL_CRC_ResetCRCCalculationUnit(MX_CRCx);

  /* Enter 32-bit input data to the MX_CRCx calculator */
  for (count = 0; count < size_word; count++)
  {
    LL_CRC_FeedData32(MX_CRCx, __REV(*(const uint32_t *)p_data));
    p_data ++;
  }

  /* Return the MX_CRCx computed value */
  return (LL_CRC_ReadData32(MX_CRCx));
} /* CRC_Calculate */
