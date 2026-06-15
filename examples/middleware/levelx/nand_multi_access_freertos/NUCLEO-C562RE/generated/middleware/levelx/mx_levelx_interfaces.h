/**
  ******************************************************************************
  * @file           : mx_levelx_interfaces.h
  * @brief          : header for the mx_levelx_interfaces.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LEVELX_INTERFACES_H
#define MX_LEVELX_INTERFACES_H

/* Includes ------------------------------------------------------------------*/
#include "mx_w25n01gvxx.h"
#include "stm32_lx_w25n01gvxx_nand_flash_dma_os_driver.h"

/* Define the list of the LevelX generic functions to be called by the example */
#define mx_lx_nand_flash_driver_0_initialize  stm32_lx_w25n01gvxx_nand_flash_dma_os_driver_initialize

/* Define the functions to get the levelx interface context */
VOID *mx_lx_nand_flash_driver_0_get_ctx(VOID);


#endif /* MX_LEVELX_INTERFACES_H */
