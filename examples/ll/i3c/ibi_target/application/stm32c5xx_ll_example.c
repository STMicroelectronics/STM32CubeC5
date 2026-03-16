/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C IBI target example using interrupt-driven events with LL API
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

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_i3cx_ev_irqhandler(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void mx_i3cx_err_irqhandler(void);
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/

/** brief: Activates the MX_I3Cx notification.
  */
inline void I3C_ActivateNotification(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE));
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


/** brief: Requests an IBI in interrupt mode.
  * param: p_payload         Pointer to the payload bytes.
  * param: payload_size_byte Payload size in bytes.
  */
inline void I3C_RequestIBI_IT(const uint8_t *p_payload, uint32_t payload_size_byte)
{
  uint32_t payload_value = 0U;

  /* Pack payload bytes into a 32-bit value (LSB first). */
  for (uint32_t index = 0U; index < payload_size_byte; index++)
  {
    payload_value |= ((uint32_t)p_payload[index] << (index * 8U));
  }

  /* Load IBI payload data */
  LL_I3C_SetIBIPayload(MX_I3Cx, payload_value);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_IBIENDIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_IBIENDIE));
#endif /* USE_LL_APP_ERROR */

  LL_I3C_TargetHandleMessage(MX_I3Cx, LL_I3C_TARGET_MTYPE_IBI, payload_size_byte);
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

  /* IBI end event. */
  if (LL_I3C_IsActiveMaskFlag_IBIEND(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_IBIEND(MX_I3Cx);

    LL_I3C_DisableIT(MX_I3Cx, LL_I3C_IER_IBIENDIE);

    I3C_IBICompleteCallback();
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
