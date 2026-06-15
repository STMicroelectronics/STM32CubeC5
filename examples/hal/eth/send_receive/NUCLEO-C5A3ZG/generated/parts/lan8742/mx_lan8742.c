/**
  ******************************************************************************
  * file           : mx_lan8742.c
  * brief          : Code generation for the LAN8742 part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lan8742_license.md file
  * in the same directory as the generated code.
  * If no mx_lan8742_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_lan8742.h"

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static lan8742_obj_t  lan8742_0_obj;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t lan8742_io_init(lan8742_io_t *pio)
{
  /* Part Status */
  int32_t ret = 0;

  switch (pio->id)
  {
    case LAN8742_0:
      pio->heth    = LAN8742_0_ETH_GETHANDLE();
      pio->addr    = LAN8742_0_ETH_ADDRESS;
      /* If the nRST pin is not enabled, set it to an invalid value */
      pio->nrst_pin      = LAN8742_INVALID_PIN;
      pio->nrst_active_state = 0;
      pio->it_pin      = LAN8742_INVALID_PIN;
      pio->hexti       = NULL;
      pio->irq_src     = LAN8742_INVALID_IRQ;
      break;
 
 
    default:
    /* Error -- Unknown ID */
      ret = -1;
    break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object LAN8742_0.
  * @retval part object.
  */
lan8742_obj_t *mx_lan8742_0_getobject(void)
{
  return &lan8742_0_obj;
}
 
 

/**
  * @brief Initialize the LAN8742 PHY device by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @param dev_id  Device ID.
  * @retval MX_PHY_STATUS_OK or MX_PHY_STATUS_ERROR.
  */
mx_phy_status_t mx_lan8742_init(lan8742_obj_t* p_phy, uint32_t dev_id)
{
  mx_phy_status_t phy_status = MX_PHY_STATUS_ERROR;
  lan8742_status_t status;
  status = lan8742_init(p_phy, dev_id);
  if(status == LAN8742_STATUS_OK)
  {
    phy_status = MX_PHY_STATUS_OK;
  }
  return phy_status;
}

/**
  * @brief Deinitialize the LAN8742 PHY device by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @retval MX_PHY_STATUS_OK or MX_PHY_STATUS_ERROR.
  */
mx_phy_status_t mx_lan8742_deinit(lan8742_obj_t* p_phy)
{
  mx_phy_status_t phy_status = MX_PHY_STATUS_ERROR;
  lan8742_status_t status;
  status = lan8742_deinit(p_phy);
  if(status == LAN8742_STATUS_OK)
  {
    phy_status = MX_PHY_STATUS_OK;
  }
  return phy_status;
}

/**
  * @brief Get the current link mode of the LAN8742 PHY by MX_PHY_INTERFACE.
  * @param p_phy   LAN8742 PHY object pointer.
  * @param p_link  Pointer to link mode structure to fill.
  */
void mx_lan8742_get_link_mode(lan8742_obj_t* p_phy, mx_phy_link_mode_t *p_link)
{
  lan8742_link_t lan8742_link_mode = { LAN8742_LINK_DOWN, LAN8742_LINK_SPEED_NONE,
                                       LAN8742_LINK_DUPLEX_NONE
                                     };
  lan8742_status_t lan8742_status = LAN8742_STATUS_OK;

  lan8742_status = lan8742_get_link_mode(p_phy, &lan8742_link_mode);
  if (lan8742_status == LAN8742_STATUS_OK)
  {
    if (lan8742_link_mode.status == LAN8742_LINK_DOWN)
    {
       MX_PHY_LINK_MODE_CLEAR(p_link);
    }
    else
    {
      p_link->status = MX_PHY_LINK_UP;
      switch (lan8742_link_mode.speed)
      {
        case LAN8742_LINK_SPEED_10:
          p_link->speed = MX_PHY_LINK_SPEED_10;
          break;
        case LAN8742_LINK_SPEED_100:
          p_link->speed = MX_PHY_LINK_SPEED_100;
          break;
        default:
           MX_PHY_LINK_MODE_CLEAR(p_link);
          break;
      }
      switch (lan8742_link_mode.duplex)
      {
        case LAN8742_LINK_HALF_DUPLEX:
          p_link->duplex = MX_PHY_LINK_HALF_DUPLEX;
          break;
        case LAN8742_LINK_FULL_DUPLEX:
          p_link->duplex = MX_PHY_LINK_FULL_DUPLEX;
          break;
        default:
           MX_PHY_LINK_MODE_CLEAR(p_link);
          break;
      }
    }
  }
  else
  {
     MX_PHY_LINK_MODE_CLEAR(p_link);
  }
}
