/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C HotJoin target interrupt example with LL API.
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
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_i3cx_ev_irqhandler(void); /* Function to handle event interrupt notification */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void mx_i3cx_err_irqhandler(void); /* Function to handle error interrupt notification */
#endif /* USE_LL_APP_ERROR */

/* Private functions prototype -----------------------------------------------*/

/** Target send Hot-Join request in interrupt mode.
  */
inline void I3C_SendHotJoinReq_IT(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, LL_I3C_IER_DAUPDIE);
#endif /* USE_LL_APP_ERROR */

  /* Request Hot-Join */
  LL_I3C_TargetHandleMessage(MX_I3Cx, LL_I3C_TARGET_MTYPE_HOT_JOIN, 0UL);
} /* end I3C_HotJoinReq_IT */


/**
  *  Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  /* Manages the received MX_I3Cx target dynamic address update event */
  if (LL_I3C_IsActiveMaskFlag_DAUPD(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_DAUPD(MX_I3Cx);

    I3C_HotJoinCpltCallback(LL_I3C_GetOwnDynamicAddress(MX_I3Cx));
  }
} /* end mx_i3cx_ev_irqhandler */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  *  Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

  I3C_ErrorCallback();
} /* end mx_i3cx_err_irqhandler */
#endif /* USE_LL_APP_ERROR */
