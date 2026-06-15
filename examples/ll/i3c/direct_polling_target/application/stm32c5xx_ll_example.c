/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C direct polling target example with LL API
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

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** brief: Checks whether the MX_I3Cx Dynamic Address Update (DAUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
inline uint32_t I3C_IsActiveFlagDynamicAddr(void)
{
  return (LL_I3C_IsActiveFlag_DAUPD(MX_I3Cx));
}


/** brief: Gets the MX_I3Cx own dynamic address.
  * retval: value of Own Dynamic Address
  */
inline uint8_t I3C_GetOwnDynamicAddress(void)
{
  return (LL_I3C_GetOwnDynamicAddress(MX_I3Cx));
}


/** brief: Clears the MX_I3Cx Dynamic Address Update (DAUPD) flag.
  * retval: none
  */
inline void I3C_ClearFlagDynamicAddr(void)
{
  LL_I3C_ClearFlag_DAUPD(MX_I3Cx);
}


/** brief: Checks whether the MX_I3Cx Max Read Length Update (MRLUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
inline uint32_t I3C_IsActiveFlagMaxReadLength(void)
{
  return (LL_I3C_IsActiveFlag_MRLUPD(MX_I3Cx));
}


/** brief: Gets the MX_I3Cx Max Read Length value.
  * retval: value of Max Read Length
  */
inline uint32_t I3C_GetMaxReadLength(void)
{
  return (LL_I3C_GetMaxReadLength(MX_I3Cx));
}


/** brief: Clears the MX_I3Cx Max Read Length Update (MRLUPD) flag.
  * retval: none
  */
inline void I3C_ClearFlagMaxReadLength(void)
{
  LL_I3C_ClearFlag_MRLUPD(MX_I3Cx);
}


/** brief: Checks whether the MX_I3Cx Max Write Length Update (MWLUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
inline uint32_t I3C_IsActiveFlagMaxWriteLength(void)
{
  return (LL_I3C_IsActiveFlag_MWLUPD(MX_I3Cx));
}


/** brief: Gets the MX_I3Cx Max Write Length value.
  * retval: value of Max Write Length
  */
inline uint32_t I3C_GetMaxWriteLength(void)
{
  return (LL_I3C_GetMaxWriteLength(MX_I3Cx));
}


/** brief: Clears the MX_I3Cx Max Write Length Update (MWLUPD) flag.
  * retval: none
  */
inline void I3C_ClearFlagMaxWriteLength(void)
{
  LL_I3C_ClearFlag_MWLUPD(MX_I3Cx);
}

