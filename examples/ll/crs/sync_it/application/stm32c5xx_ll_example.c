/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : CRS sync to adjust the 48MHz internal clock using IT method
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
void mx_crsx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_CRSx synchronization in interrupt mode.
  */
inline void CRS_StartSynchronization_IT(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_CRS_EnableIT(MX_CRSx, (LL_CRS_CR_SYNCOKIE | LL_CRS_CR_SYNCWARNIE | LL_CRS_CR_ERRIE | LL_CRS_CR_ESYNCIE));
#else
  LL_CRS_EnableIT(MX_CRSx, (LL_CRS_CR_SYNCOKIE | LL_CRS_CR_ESYNCIE));
#endif /* USE_LL_APP_ERROR */
  LL_CRS_EnableFreqErrorCounter(MX_CRSx);
}

/**
  *  Stop the MX_CRSx synchronization.
  */
inline void CRS_StopSynchronization_IT(void)
{
  LL_CRS_DisableFreqErrorCounter(MX_CRSx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_CRS_DisableIT(MX_CRSx, (LL_CRS_CR_SYNCOKIE | LL_CRS_CR_SYNCWARNIE | LL_CRS_CR_ERRIE | LL_CRS_CR_ESYNCIE));
  LL_CRS_ClearFlag(MX_CRSx, (LL_CRS_ICR_SYNCOKC | LL_CRS_ICR_SYNCWARNC | LL_CRS_ICR_ERRC | LL_CRS_ICR_ESYNCC));
#else
  LL_CRS_DisableIT(MX_CRSx, (LL_CRS_CR_SYNCOKIE | LL_CRS_CR_ESYNCIE));
  LL_CRS_ClearFlag(MX_CRSx, (LL_CRS_ICR_SYNCOKC | LL_CRS_ICR_ESYNCC));
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_CRSx interrupts.
  */
void mx_crsx_irqhandler(void)
{
  uint32_t it_flags_sources;
  /* Get current IT flags and IT sources values */
  it_flags_sources = LL_CRS_READ_REG(MX_CRSx, ISR);
  it_flags_sources &= LL_CRS_READ_REG(MX_CRSx, CR);

  /* Check CRS SYNCOK flag */
  if ((it_flags_sources & LL_CRS_ISR_SYNCOKF) != 0U)
  {
    LL_CRS_ClearFlag_SYNCOK(MX_CRSx);

    CRS_SynchronizationOKCallback();
  }
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Check CRS Error flags */
  if ((it_flags_sources & LL_CRS_ISR_ERRF) != 0U)
  {
    LL_CRS_ClearFlag_ERR(MX_CRSx);

    CRS_ErrorCallback();
  }

  /* Check CRS SYNCWARN flag  */
  if ((it_flags_sources & LL_CRS_ISR_SYNCWARNF) != 0U)
  {
    LL_CRS_ClearFlag_SYNCWARN(MX_CRSx);

    CRS_WarningCallback();
  }
#endif /* USE_LL_APP_ERROR */
}