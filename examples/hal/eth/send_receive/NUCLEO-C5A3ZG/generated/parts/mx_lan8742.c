/**
  ******************************************************************************
  * file           : mx_lan8742.c
  * brief          : LAN8742 Part resources defines
  ******************************************************************************
  * @attention
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
/* Includes ------------------------------------------------------------------*/
#include "mx_lan8742.h"
lan8742_obj_t lan8742_obj;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

int32_t lan8742_io_init(lan8742_io_t *pio)
{
  switch (pio->id)
  {
    case MX_LAN8742_0 :
      pio->heth    = (hal_eth_handle_t *) mx_eth1_hal_gethandle();
      pio->addr    = MX_LAN8742_0_ETH_ADDRESS;
      break;

    default:
      return -1;
  }
  return 0;
}

lan8742_obj_t *mx_lan8742_getObj(void)
{
  return &lan8742_obj;
}