/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C Controller example for multi-target HotJoin using interrupt-driven events with LL API.
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
#define I3C_DAA_IT (LL_I3C_IER_FCIE | LL_I3C_IER_RXFNEIE | LL_I3C_IER_TXFNFIE) /* Controleer DAA IT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_i3cx_ev_irqhandler(void); /* Function to handle event interrupt notification */

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void mx_i3cx_err_irqhandler(void); /* Function to handle error interrupt notification */
#endif /* USE_LL_APP_ERROR */

/** Activates the HotJoin feature of the MX_I3Cx instance.
  */
inline void I3C_ActivateNotification(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (I3C_DAA_IT | LL_I3C_IER_HJIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (I3C_DAA_IT | LL_I3C_IER_HJIE));
#endif /* USE_LL_APP_ERROR */
} /* end I3C_ActivateNotification */


/** Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
inline void I3C_AssignDynamicAddr_IT(void)
{
  /* Write ENTDAA CCC information in the control register */
  LL_I3C_ControllerHandleCCC(MX_I3Cx, LL_I3C_BROADCAST_ENTDAA, 0UL, LL_I3C_GENERATE_STOP);
} /* end I3C_AssignDynamicAddr_IT */


/** Set a target dynamic address for the MX_I3Cx instance.
  * param : arget_address Target dynamic address.
  */
inline void I3C_SetDynamicAddr(uint64_t target_address)
{
  /* Write device address in the TDR register */
  LL_I3C_TransmitData8(MX_I3Cx, target_address);
} /* end I3C_SetDynamicAddr */


/**
  *  Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  uint64_t tgt_payload = 0UL;

  /* Check HotJoin flag. */
  if (LL_I3C_IsActiveMaskFlag_HJ(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_HJ(MX_I3Cx);

    I3C_HotJoinReqCallback();

    return;
  }

  /* Check Tx FIFO not full flag. */
  if (LL_I3C_IsActiveFlag_TXFNF(MX_I3Cx))
  {
    /* handles the CCC DAA command. */
    if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
    {
      /* Retrieve the 64 bits payload used to handle the target request dynamic address callback */
      for (uint32_t index = 0U; index < 8U; index++)
      {
        tgt_payload |= ((uint64_t)LL_I3C_ReceiveData8(MX_I3Cx) << (index * 8U));
      }
    }

    I3C_TargetReqDynamicAddrCallback(tgt_payload);
  }

  /* Check frame complete flag. */
  if (LL_I3C_IsActiveMaskFlag_FC(MX_I3Cx) != 0U)
  {
    LL_I3C_DisableIT(MX_I3Cx, I3C_DAA_IT);

    LL_I3C_ClearFlag_FC(MX_I3Cx);

    I3C_DAACpltCallback();
  }
} /* end mx_i3cx_ev_irqhandler */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  *  Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  /** Clear all errors raised in the I3C peripheral.
    */
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

  /** Handle the error in the DAA error callback function.
    */
  I3C_DAAErrorCallback();
} /* end mx_i3cx_err_irqhandler */
#endif /* USE_LL_APP_ERROR */
