/**
  ******************************************************************************
  * file           : mx_lan8742.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MX_LAN8742_H
#define __MX_LAN8742_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lan8742.h"
#include "mx_hal_def.h"

/* Exported macro ------------------------------------------------------------*/
/* Part Config ID */
#define LAN8742_0                              0 /* Main label */

/* No label has been defined for this part driver instance,
 * so no alias is generated for LAN8742_0. */

/* alias for the getter function based on the extra label */
#define mx_lan8742_1_getobject() mx_lan8742_0_getobject()

/* Resource bindings */
#define LAN8742_0_ETH_GETHANDLE               mx_eth1_gethandle

/* PHY address for LAN8742_0 device */
#define LAN8742_0_ETH_ADDRESS                 0


 
 

/* Exported types ------------------------------------------------------------*/

/* MX_PHY_INTERFACE DEFINITION -----------------------------------------------
 * The following definitions bring a generic interface for Ethernet PHY parts drivers.
 * The definition of MX_PHY_INTERFACE_VERSION prevents 
 *  - multiple exports by different part drivers
 *  - interface version mismatch
 */
#ifndef MX_PHY_INTERFACE_VERSION
#define MX_PHY_INTERFACE_VERSION 1
typedef enum
{
  MX_PHY_STATUS_ERROR = -1,              /*!< Error Status     */
  MX_PHY_STATUS_OK = 0                   /*!< Success Status   */
} mx_phy_status_t;

typedef enum
{
  MX_PHY_LINK_DOWN = 0U,
  MX_PHY_LINK_UP = 1U
} mx_phy_link_status_t;

typedef enum
{
  MX_PHY_LINK_SPEED_NONE = 0U,
  MX_PHY_LINK_SPEED_10 = 10U,
  MX_PHY_LINK_SPEED_100 = 100U,
  MX_PHY_LINK_SPEED_1000 = 1000U
} mx_phy_link_speed_t;

typedef enum
{
  MX_PHY_LINK_DUPLEX_NONE = 0U,
  MX_PHY_LINK_HALF_DUPLEX = 1U,
  MX_PHY_LINK_FULL_DUPLEX = 2U
} mx_phy_link_duplex_t;

/**
  * @brief PHY ethernet link structure.
  */
typedef struct
{
  mx_phy_link_status_t status;   /*!< Link status: MX_PHY_LINK_DOWN or MX_PHY_LINK_UP */
  mx_phy_link_speed_t speed;     /*!< Link speed: MX_PHY_LINK_SPEED_10 or MX_PHY_LINK_SPEED_100 */
  mx_phy_link_duplex_t duplex;   /*!< Duplex mode: MX_PHY_LINK_HALF_DUPLEX or MX_PHY_LINK_FULL_DUPLEX */
} mx_phy_link_mode_t;

/**
 * @brief Get link mode function type
 *
 */
typedef void (*mx_phy_get_link_mode_fn)(mx_phy_link_mode_t*);

/**
 * @brief Event callback function type
 *
 */
typedef void (*mx_phy_event_callback_fn)(void);

/**
 * @brief Generic Ethernet PHY interface structure.
 *
 * This structure defines the interface for interacting with an Ethernet PHY device.
 * It provides function pointers for PHY operations, such as retrieving the current link mode.
 */
typedef struct {
  /**
   * @brief Function pointer to retrieve the current link mode of the PHY.
   * @param[out] mx_phy_link_mode_t* Pointer to a structure to receive the link mode information.
   */
  mx_phy_get_link_mode_fn get_link_mode;
} mx_phy_interface_t;

#define MX_PHY_LINK_MODE_CLEAR(p_mode) do { \
  (p_mode)->status = MX_PHY_LINK_DOWN; \
  (p_mode)->speed = MX_PHY_LINK_SPEED_NONE; \
  (p_mode)->duplex = MX_PHY_LINK_DUPLEX_NONE; \
} while (0)

#endif /* MX_PHY_INTERFACE_VERSION */
/* end of MX_PHY_INTERFACE DEFINITION -------------------------------------- */
/* Exported functions ------------------------------------------------------ */
lan8742_obj_t *mx_lan8742_0_getobject(void);
 
 

mx_phy_status_t mx_lan8742_init(lan8742_obj_t* p_phy, uint32_t dev_id);

mx_phy_status_t mx_lan8742_deinit(lan8742_obj_t* p_phy);

void mx_lan8742_get_link_mode(lan8742_obj_t* p_phy, mx_phy_link_mode_t *p_link);

#ifdef __cplusplus
}
#endif

#endif /* __MX_LAN8742_H */
