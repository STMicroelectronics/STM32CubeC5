/**
  ******************************************************************************
  * file           : mx_levelx_interfaces.c
  * brief          : generate functions to initialize nor flash interfaces
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

#include "mx_levelx_interfaces.h"

static STM32_LX_NAND_FLASH_DRIVER_CONTEXT nand_flash_driver_0_ctx;

/**
  * mx_lx_nand_flash_driver_0_get_ctx() levelx stub interface
  */
VOID *mx_lx_nand_flash_driver_0_get_ctx(VOID)
{
  w25n01gvxx_obj_t *pFlashObj;

  /* Retrieve the NAND flash object pointer */
  pFlashObj = mx_w25n01gvxx_0_getobject();

  /* Initialize the memory part */
  if (w25n01gvxx_init(pFlashObj, MX_W25N01GVXX_1) != W25N01GVXX_OK)
  {
    return NULL;
  }

  /* Setup the levelx nand flash context */
  nand_flash_driver_0_ctx.nand_flash_flags            =  STM32_LX_NAND_FLAG_NONE;
  nand_flash_driver_0_ctx.nand_flash_get_driver_object = (nand_flash_get_object *)mx_w25n01gvxx_0_getobject;
  nand_flash_driver_0_ctx.nand_flash_op_timeout = W25N01GVXX_SPI_POLL_TIMEOUT;

  return (VOID *)&nand_flash_driver_0_ctx;
}
