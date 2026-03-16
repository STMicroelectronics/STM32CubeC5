/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : adjust the internal clock using the CRS polling method
  ******************************************************************************
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
#define CRS_SYNC_TIMEOUT_MS             1000UL
#endif /* USE_LL_APP_TIMEOUT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_CRSx synchronization.
  */
inline void CRS_StartSynchronization(void)
{
  LL_CRS_EnableFreqErrorCounter(MX_CRSx);
}

/**
  *  Polling for the end of the MX_CRSx synchronization.
  *  SYSTEM_OK MX_CRSx synchronization success.
  *  SYSTEM_PERIPHERAL_ERROR MX_CRSx synchronization error.
  */
inline system_status_t CRS_PollForSynchronization(void)
{
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = CRS_SYNC_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  uint32_t flags;
  do
  {
    flags = LL_CRS_READ_REG(MX_CRSx, ISR);

    /* Check CRS Expected SYNC flag */
    if ((flags & LL_CRS_ISR_ESYNCF) != 0U)
    {
      LL_CRS_ClearFlag_ESYNC(MX_CRSx);
    }

    /* Check CRS SYNCOK flag */
    if ((flags & LL_CRS_ISR_SYNCOKF) != 0U)
    {
      LL_CRS_ClearFlag_SYNCOK(MX_CRSx);

      return SYSTEM_OK;
    }

    /* Check CRS ERR and SYNCWARN flags */
    if (((flags & LL_CRS_ISR_ERRF) == 0U) && ((flags & LL_CRS_ISR_SYNCWARNF) == 0U))
    {
      flags = 0U;
    }
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    else /* Error flag or SYNC warning flag set */
    {
      LL_CRS_ClearFlag(MX_CRSx, (LL_CRS_ICR_ERRC | LL_CRS_ICR_SYNCWARNC));
    }
#endif /* USE_LL_APP_ERROR */
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout_ms-- == 0)
      {
        return SYSTEM_PERIPHERAL_ERROR;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  } while (flags == 0U);

  return SYSTEM_PERIPHERAL_ERROR;

}