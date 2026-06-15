/**
  ******************************************************************************
  * @file           : mx_levelx_app.c
  * @brief          : LevelX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_levelx_license.md file
  * in the same directory as the generated code.
  * If no mx_levelx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_levelx_app.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Application LevelX Initialization.
  * @param  None
  * @retval 0 on success -1 otherwise
  */
UINT app_levelx_init(VOID)
{
  UINT ret;
  /* Initialize LevelX NAND flash */
  ret = lx_nand_flash_initialize();
  if (ret != LX_SUCCESS)
  {
    return -1;
  }



  return LX_SUCCESS;
}



/*
 * @brief  LevelX operations processing
 * @param NULL
 * @retval 0 on success -1 otherwise
 */
UINT app_levelx_process(VOID)
{
  UINT status = LX_SUCCESS;

  return status;
}
