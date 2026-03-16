/**
  ******************************************************************************
  * file           : mx_w25q128j.c
  * brief          : Code generation for the W25Q128J part driver.
  ******************************************************************************
  * attention
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
#include "mx_w25q128j.h"

static w25q128j_obj_t  w25q128j_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t w25q128j_io_init(w25q128j_io_t *pio)
{
  /* Part Status */
  int ret = 0;

  switch (pio->id)
  {
    case W25Q128J_0:
      pio->phspi       = W25Q128J_0_SPI_GETHANDLE();
      pio->cs_port     = W25Q128J_0_CS_PORT;
      pio->cs_pin      = W25Q128J_0_CS_PIN;
      break;
 
 
    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object 0.
  * @retval part object.
  */
w25q128j_obj_t *mx_w25q128j_0_getobject(void)
{
  return &w25q128j_0_obj;
}
 
 
