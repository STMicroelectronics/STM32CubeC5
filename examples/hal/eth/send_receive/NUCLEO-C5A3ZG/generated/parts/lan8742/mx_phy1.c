/**
  ******************************************************************************
  * file           : mx_phy.c
  * brief          : Implementation of generic ethernet PHY interface for LAN8742.
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
#include "mx_phy1.h"

/* Private defines -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Retrieve the current link mode of PHY1.
 * @param[out] p_link Pointer to a mx_phy_link_mode_t structure to receive the link mode information.
 */
static void mx_phy1_get_link_mode(mx_phy_link_mode_t *p_link);

/* Private variables ---------------------------------------------------------*/

/**
 * @brief Static handle for the LAN8742 PHY1 instance.
 *
 * This object maintains the state and configuration for the LAN8742 PHY hardware
 * used by this interface implementation.
 */
static lan8742_obj_t phy_handle_1;

/**
 * @brief Static instance of the PHY interface for PHY1 (LAN8742).
 *
 * This structure provides the function pointer(s) for interacting with the PHY1 hardware.
 * It is returned by mx_phy1_get_interface().
 */
static mx_phy_interface_t phy_interface_1 = {
  .get_link_mode = mx_phy1_get_link_mode
};

/* Private Functions Definition ----------------------------------------------*/

static void mx_phy1_get_link_mode(mx_phy_link_mode_t *p_link)
{
  mx_lan8742_get_link_mode(&phy_handle_1, p_link);
}

/* Functions Definition ------------------------------------------------------*/

mx_phy_status_t mx_phy1_init(void)
{
  uint32_t dev_id = LAN8742_0;
  return mx_lan8742_init(&phy_handle_1, dev_id);
}

mx_phy_status_t mx_phy1_deinit(void)
{
  return mx_lan8742_deinit(&phy_handle_1);
}

mx_phy_interface_t* mx_phy1_get_interface(void)
{
  return &phy_interface_1;
}

