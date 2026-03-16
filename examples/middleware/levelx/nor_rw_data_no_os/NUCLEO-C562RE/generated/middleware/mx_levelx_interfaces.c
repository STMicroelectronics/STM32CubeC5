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

static STM32_LX_NOR_FLASH_DRIVER_CONTEXT nor_flash_driver_0_ctx;
static ULONG nor_flash_driver_0_read_buffer[W25Q128J_PAGE_SIZE];


/**
  * mx_lx_nor_flash_driver_0_get_ctx()
  */
VOID *mx_lx_nor_flash_driver_0_get_ctx(VOID)
{
  w25q128j_obj_t *pFlashObj;

  /* Retrieve the nor flash object pointer */
  pFlashObj = mx_w25q128j_0_getobject();

  /* Initialize the memory part */
  if (w25q128j_init(pFlashObj, 0) != W25Q128J_OK)
  {
    return NULL;
  }
  /* Setup the levelx nor flash context */
  nor_flash_driver_0_ctx.nor_flash_flags             = STM32_LX_NOR_FLAG_NONE;
  nor_flash_driver_0_ctx.nor_flash_read_buffer       = nor_flash_driver_0_read_buffer;
  nor_flash_driver_0_ctx.nor_flash_get_driver_object = (nor_flash_get_object *)mx_w25q128j_0_getobject;
  nor_flash_driver_0_ctx.nor_flash_op_timeout = W25Q128J_SPI_POLL_TIMEOUT;
  return (VOID *)&nor_flash_driver_0_ctx;
}

