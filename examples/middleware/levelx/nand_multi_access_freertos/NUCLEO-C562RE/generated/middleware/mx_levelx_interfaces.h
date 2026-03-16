/**
  ******************************************************************************
  * file           : mx_levelx_interfaces.h
  * brief          : header for the mx_lx_nor_flash.c
  ******************************************************************************
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

#ifndef MX_LEVELX_INTERFACES
#define MX_LEVELX_INTERFACES

/* Includes ------------------------------------------------------------------*/
#include "mx_w25n01gvxx.h"
#include "stm32_lx_w25n01gvxx_nand_flash_dma_os_driver.h"

/* define the list of the levelx generic functions to be called by the example */
#define mx_lx_nand_flash_driver_0_initialize  stm32_lx_w25n01gvxx_nand_flash_dma_os_driver_initialize

VOID *mx_lx_nand_flash_driver_0_get_ctx(VOID);
#endif /* MX_LEVELX_INTERFACES */
