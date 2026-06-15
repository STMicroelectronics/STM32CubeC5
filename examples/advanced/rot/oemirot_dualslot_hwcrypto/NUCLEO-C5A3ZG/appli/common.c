/**
  ******************************************************************************
  * @file    common.c
  * @brief   COMMON module.
  *          This file provides set of firmware functions to manage Common
  *          functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "com.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Convert a string to an integer.
  * @param  pinputstr: The string to be converted
  * @param  pintnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *pinputstr, uint32_t *pintnum)
{
  uint32_t i = 0U;
  uint32_t res = 0U;
  uint32_t val = 0U;

  if ((pinputstr[0U] == '0') && ((pinputstr[1U] == 'x') || (pinputstr[1U] == 'X')))
  {
    i = 2U;
    while ((i < 11U) && (pinputstr[i] != '\0'))
    {
      if (ISVALIDHEX(pinputstr[i]))
      {
        val = (val << 4U) + CONVERTHEX(pinputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0U;
        break;
      }
      i++;
    }

    /* valid result */
    if (pinputstr[i] == '\0')
    {
      *pintnum= val;
      res = 1U;
    }
  }
  else /* max 10-digit decimal input */
  {
    while ((i < 11U) && (res != 1U))
    {
      if (pinputstr[i] == '\0')
      {
        *pintnum= val;
        /* return 1 */
        res = 1U;
      }
      else if (((pinputstr[i] == 'k') || (pinputstr[i] == 'K')) && (i > 0U))
      {
        val = val << 10U;
        *pintnum= val;
        res = 1U;
      }
      else if (((pinputstr[i] == 'm') || (pinputstr[i] == 'M')) && (i > 0U))
      {
        val = val << 20U;
        *pintnum= val;
        res = 1U;
      }
      else if (ISVALIDDEC(pinputstr[i]))
      {
        val = val * 10U + CONVERTDEC(pinputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0U;
        break;
      }
      i++;
    }
  }

  return res;
}

/**
  * @brief  Transmit a byte to the HyperTerminal.
  * @param  param The byte to be sent
  * @retval hal_status_t HAL_OK if OK
  */
hal_status_t Serial_PutByte(uint8_t uParam)
{
  return COM_Transmit_Y(&uParam, 1U, TX_TIMEOUT);
}
