/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C direct IT target example with LL API
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
void mx_i3cx_ev_irqhandler(void); /* Function to handle event interrupt notification */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void mx_i3cx_err_irqhandler(void); /* Function to handle error interrupt notification */
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/

/** brief: brief: Activates the MX_I3Cx notification (DAA update, MRL/MWL update).
  */
inline void I3C_ActivateNotification(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE | LL_I3C_IER_MRLUPDIE | LL_I3C_IER_MWLUPDIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE | LL_I3C_IER_MRLUPDIE | LL_I3C_IER_MWLUPDIE));
#endif /* USE_LL_APP_ERROR */
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  Gets the MX_I3Cx own dynamic address.
  * retval: Value of the own dynamic address.
  */
inline uint8_t I3C_GetOwnDynamicAddress(void)
{
  return (LL_I3C_GetOwnDynamicAddress(MX_I3Cx));
}
#endif /* USE_LL_APP_ERROR */


/** brief: Gets the MX_I3Cx Max Read Length value.
  * retval: Value of the Max Read Length.
  */
inline uint32_t I3C_GetMaxReadLength(void)
{
  return (LL_I3C_GetMaxReadLength(MX_I3Cx));
}


/** brief: Gets the MX_I3Cx Max Write Length value.
  * retval: Value of the Max Write Length.
  */
inline uint32_t I3C_GetMaxWriteLength(void)
{
  return (LL_I3C_GetMaxWriteLength(MX_I3Cx));
}


/** brief: Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  /* Dynamic Address Assignment update event: own dynamic address is updated. */
  if (LL_I3C_IsActiveMaskFlag_DAUPD(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_DAUPD(MX_I3Cx);

    I3C_DAACompleteCallback();

    return;
  }

  /* MWL update event (direct SETMWL CCC). */
  if (LL_I3C_IsActiveMaskFlag_MWLUPD(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_MWLUPD(MX_I3Cx);

    I3C_MWLCompleteCallback();
  }

  /* MRL update event (direct SETMRL CCC). */
  if (LL_I3C_IsActiveMaskFlag_MRLUPD(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_MRLUPD(MX_I3Cx);

    I3C_MRLCompleteCallback();
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

  I3C_ErrorCallback();
}
#endif /* USE_LL_APP_ERROR */

