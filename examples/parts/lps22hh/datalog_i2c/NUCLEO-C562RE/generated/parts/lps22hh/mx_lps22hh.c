/**
  ******************************************************************************
  * file           : mx_lps22hh.c
  * brief          : Code generation for the LPS22HH part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lps22hh_license.md file
  * in the same directory as the generated code.
  * If no mx_lps22hh_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_lps22hh.h"

static lps22hh_object_t lps22hh_0_obj;
int32_t lps22hh_io_init(lps22hh_io_t *pio)
{
  switch (pio->id)
  {

  case LPS22HH_0:
    pio->hi2c = LPS22HH_0_I2C_GETHANDLE();
    pio->addr = LPS22HH_0_I2C_ADDRESS;
    pio->hexti = NULL;
    break;
  default:
    break;
  }
  return 0;
}

lps22hh_object_t *mx_lps22hh_0_getobject(void)
{
  return &lps22hh_0_obj;
}

