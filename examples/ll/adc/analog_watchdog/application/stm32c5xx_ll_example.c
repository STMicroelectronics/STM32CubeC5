/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : ADC Analog Wotchdog to monitor a channel in interrupt mode with LL API
  *                  (in interrupt mode) with LL API services
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
/* MX_ADCx delay between end of calibration and enable operations (unit: CPU clock cycles). */
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 4U)
#define ADC_ENABLE_TIMEOUT_MS             (2U)       /* MX_ADCx enable time-out value (unit: milli seconds)          */
#define ADC_DISABLE_TIMEOUT_MS            (2U)       /* MX_ADCx disable time-out value (unit: milli seconds)         */
#define ADC_CALIBRATION_TIMEOUT_MS        (500U)     /* MX_ADCx calibration time-out value (unit: milli seconds)     */
#define ADC_VREFPLUS_VALUE                (VDD_VALUE) /* Assumption: pin Vref+ connected to Vdd at board level        */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void DelayUs(uint32_t delay_us);
void mx_adcx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Activate the MX_ADCx.
  *  SYSTEM_PERIPHERAL_ERROR MX_ADCx error
  *  SYSTEM_OK               MX_ADCx success
  */
inline system_status_t ADC_Activate(void)
{
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = ADC_ENABLE_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  if (LL_ADC_IsEnabled(MX_ADCx) == 0U)
  {
    /* Enable MX_ADCx instance after clearing the MX_ADCx ready flag */
    LL_ADC_ClearFlag_ADRDY(MX_ADCx);
    LL_ADC_Enable(MX_ADCx);

    /* Wait for MX_ADCx effectively enabled */
    while (LL_ADC_IsActiveFlag_ADRDY(MX_ADCx) == 0U)
    {
      /* If ADEN bit is set less than 4 MX_ADCx clock cycles after the ADCAL bit has been cleared (after a calibration),
         ADEN bit is reset by the calibration logic.
         The workaround is to continue setting ADEN until ADRDY is becomes 1.
         Additionally, ADC_ENABLE_TIMEOUT_MS is defined to encompass this 4 MX_ADCx clock cycle duration */
      /* Note: Test of MX_ADCx enabled required due to hardware constraint to not enable MX_ADCx if already enabled. */
      if (LL_ADC_IsEnabled(MX_ADCx) == 0U)
      {
        LL_ADC_Enable(MX_ADCx);
      }

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
      if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
      {
        if (timeout_ms-- == 0U)
        {
          return_status = SYSTEM_PERIPHERAL_ERROR;
          break;
        }
      }
#endif /* USE_LL_APP_TIMEOUT */
    }

  }

  return return_status;
}


/**
  *  Calibrate the MX_ADCx.
  *  SYSTEM_PERIPHERAL_ERROR MX_ADCx error
  *  SYSTEM_OK               MX_ADCx success
  */
inline system_status_t ADC_Calibrate(void)
{
  /* Note: wait_loop_index variable is equal to ADC_DELAY_CALIB_ENABLE_CPU_CYCLES divided by 2 to compensate partially
     CPU processing cycles                                                                                            */
  uint32_t wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1U);
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = ADC_DISABLE_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  /* Disable MX_ADCx instance before clearing the MX_ADCx ready flag */
  LL_ADC_Disable(MX_ADCx);
  LL_ADC_ClearFlag_ADRDY(MX_ADCx);

  /* Wait for MX_ADCx effectively deactivated */
  while (LL_ADC_IsEnabled(MX_ADCx) != 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout_ms-- == 0U)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  timeout_ms = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */

  /* Disable the MX_ADCx deep power down feature (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(MX_ADCx);

  /* Enable MX_ADCx internal regulator voltage and wait for MX_ADCx internal voltage regulator stabilization */
  LL_ADC_EnableInternalRegulator(MX_ADCx);
  DelayUs(LL_ADC_DELAY_INTERNAL_REGUL_STAB_US);

  /* Starts the MX_ADCx self-calibration in single ended mode */
  LL_ADC_StartCalibration(MX_ADCx, LL_ADC_IN_SINGLE_ENDED);
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  timeout_ms = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */
  /* Poll for MX_ADCx calibration completion */
  while (LL_ADC_IsCalibrationOnGoing(MX_ADCx) != 0UL)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout_ms-- == 0U)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  /* Delay for MX_ADCx stabilization after calibration and before the activation of the MX_ADCx for conversions */
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }

  ADC_Activate();

  return return_status;
}


/**
  *  Start the MX_ADCx conversion.
  */
inline void ADC_StartConversion_IT(void)
{
  /*  Start the MX_ADCx group regular with interruption: analog watchdog. */
  /* Note: Done only once due to continuous mode enabled */
  if (LL_ADC_REG_IsConversionOngoing(MX_ADCx) == 0U)
  {
    LL_ADC_ClearFlag_AWD1(MX_ADCx);
    LL_ADC_EnableIT_AWD1(MX_ADCx);

    LL_ADC_REG_StartConversion(MX_ADCx);
  }
}


/**
  *  Get the MX_ADCx GPIO data voltage in mV.
  *  uint16_t data voltage value in mV
  */
inline uint16_t ADC_GetDataGPIOmV(void)
{
  return (LL_ADC_CALC_DATA_TO_VOLTAGE(ADC_VREFPLUS_VALUE,
                                      LL_ADC_REG_ReadConversionData12(MX_ADCx),
                                      LL_ADC_RESOLUTION_12B));
}


/**
  *  Handles the MX_ADCx interrupts.
  */
void mx_adcx_irqhandler(void)
{
  uint32_t flag_status        = LL_ADC_READ_REG(MX_ADCx, ISR);
  uint32_t flag_status_masked = flag_status & LL_ADC_READ_REG(MX_ADCx, IER);

  if ((flag_status_masked & LL_ADC_FLAG_AWD1) != 0UL)
  {
    LL_ADC_ClearFlag(MX_ADCx, LL_ADC_FLAG_AWD1);

    AnalogWatchdogCallback();
  }
}


/**
  * Delays execution for an approximate number of microseconds.
  * Calculates the wait loop count based on the CMSIS variable "SystemCoreClock".
  * The delay is approximate and affected by CPU pipeline, branch prediction, cache, and compiler optimizations.
  * For core clock frequencies below 500 kHz, the delay is limited to a few CPU cycles.
  *
  * warning: This function provides only an approximate delay and must not be used for precise timing.
  * param: delay_us Number of microseconds to delay.
  */
static inline void DelayUs(uint32_t delay_us)
{
  volatile uint32_t wait_loop_index = ((delay_us * (SystemCoreClock >> 19U)) >> 2U);
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }
}
