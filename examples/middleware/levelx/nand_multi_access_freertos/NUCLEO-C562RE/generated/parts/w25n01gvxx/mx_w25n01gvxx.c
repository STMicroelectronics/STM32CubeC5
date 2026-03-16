/**
  ******************************************************************************
  * file           : mx_w25n01gvxx.c
  * brief          : Code generation for the W25N01GVXX part driver.
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
#include "mx_w25n01gvxx.h"

static w25n01gvxx_obj_t  w25n01gvxx_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t w25n01gvxx_io_init(w25n01gvxx_io_t *pio)
{
  /* Part Status */
  int ret = 0;

  switch (pio->id)
  {
    case W25N01GVXX_0:
      pio->phspi       = W25N01GVXX_0_SPI_GETHANDLE();
      pio->cs_port     = W25N01GVXX_0_CS_PORT;
      pio->cs_pin      = W25N01GVXX_0_CS_PIN;
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
w25n01gvxx_obj_t *mx_w25n01gvxx_0_getobject(void)
{
  return &w25n01gvxx_0_obj;
}
 
 
