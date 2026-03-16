/**
  ******************************************************************************
  * file           : mx_phy.h
  * brief          : generic ethernet PHY interface.
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
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MX_PHY1_H
#define __MX_PHY1_H

/* Includes ------------------------------------------------------------------*/
#include "mx_lan8742.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @brief Initialize the PHY1 hardware.
 * @retval MX_PHY_STATUS_OK on success, MX_PHY_STATUS_ERROR otherwise.
 */
mx_phy_status_t mx_phy1_init(void);

/**
 * @brief De-initialize the PHY1 hardware.
 * @retval MX_PHY_STATUS_OK on success, MX_PHY_STATUS_ERROR otherwise.
 */
mx_phy_status_t mx_phy1_deinit(void);

/**
 * @brief Get the PHY1 interface structure.
 * @retval Pointer to the mx_phy_interface_t structure for PHY1.
 */
mx_phy_interface_t* mx_phy1_get_interface(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MX_PHY1_H */
