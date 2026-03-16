/**
  ******************************************************************************
  * file           : mx_levelx_interfaces.h
  * brief          : header for the mx_levelx_interfaces.c
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

#ifndef MX_LEVELX_INTERFACES_H
#define MX_LEVELX_INTERFACES_H

/* Includes ------------------------------------------------------------------*/
#include "mx_w25q128j.h"
#include "stm32_lx_w25q128j_nor_flash_dma_no_os_driver.h"

/* define the list of the levelx generic functions to be called by the example */
#define mx_lx_nor_flash_driver_0_initialize   stm32_lx_w25q128j_nor_flash_dma_no_os_driver_initialize
VOID *mx_lx_nor_flash_driver_0_get_ctx(VOID);


#endif /* MX_LX_NOR_FLASH_H */
