/**
  ******************************************************************************
  * file           : example.c
  * brief          : Enter and exit the Sleep mode through an EXTI interrupt using the HAL API
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
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_exti_handle_t *pEXTI;  /* pointer referencing the EXTI handle from the generated code */

/* Private functions prototype -----------------------------------------------*/
static void RCC_DisableBuses(void);

/** ########## Step 1 ##########
  * The application configures the system to reach typical product consumption.
  * The application configures the wake-up source to wake-up the MCU from Sleep mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the system to reach the typical product consumption */
  if (mx_rccx_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  RCC_DisableBuses();

  /* Configures the wake-up source */
  if (mx_extix_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  pEXTI = mx_extix_gethandle();
  if (pEXTI != NULL)
  {
    if (HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT) == HAL_OK)
    {
      return_status = EXEC_STATUS_INIT_OK;
    }
  }

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The application requests entry into Sleep mode.
  * The application does not need to reconfigures the system clock after exiting Sleep mode.
  */
app_status_t app_process(void)
{
  /* Suspend SysTick to prevent interrupts during Sleep mode */
  HAL_SuspendTick();

  /* Enter Sleep low-power mode */
  HAL_PWR_EnterSleepMode(HAL_PWR_LOW_PWR_MODE_WFI);

  /* Resume SysTick to continue normal operation after wake-up from Sleep mode */
  HAL_ResumeTick();

  return EXEC_STATUS_OK;
} /* end app_process */


/** This function is never called and is provided as reference only.
  * Deinitializes the wake-up source before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief: Disable RCC bus clocks.
  */
static void RCC_DisableBuses(void)
{
  /* Disable Sleep clock */
  HAL_RCC_LP_LPDMA1_DisableClockInSleepMode();
#if defined(LPDMA2)
  HAL_RCC_LP_LPDMA2_DisableClockInSleepMode();
#endif /* LPDMA2 */
  HAL_RCC_LP_FLASH_DisableClockInSleepMode();
  HAL_RCC_LP_CRC_DisableClockInSleepMode();
  HAL_RCC_LP_CORDIC_DisableClockInSleepMode();
  HAL_RCC_LP_RAMCFG_DisableClockInSleepMode();
  HAL_RCC_LP_ICACHE1_DisableClockInSleepMode();
#if defined(ETH1)
  HAL_RCC_LP_ETH1CK_DisableClockInSleepMode();
  HAL_RCC_LP_ETH1_DisableClockInSleepMode();
  HAL_RCC_LP_ETH1TX_DisableClockInSleepMode();
  HAL_RCC_LP_ETH1RX_DisableClockInSleepMode();
#endif /* ETH1 */
  HAL_RCC_LP_SRAM2_DisableClockInSleepMode();
  HAL_RCC_LP_SRAM1_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOA_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOB_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOC_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOD_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOE_DisableClockInSleepMode();
#if defined(GPIOF)
  HAL_RCC_LP_GPIOF_DisableClockInSleepMode();
#endif /* GPIOF */
#if defined(GPIOG)
  HAL_RCC_LP_GPIOG_DisableClockInSleepMode();
#endif /* GPIOG */
  HAL_RCC_LP_GPIOH_DisableClockInSleepMode();
  HAL_RCC_LP_ADC12_DisableClockInSleepMode();
  HAL_RCC_LP_DAC1_DisableClockInSleepMode();
#if defined(AES)
  HAL_RCC_LP_AES_DisableClockInSleepMode();
#endif /* AES */
  HAL_RCC_LP_HASH_DisableClockInSleepMode();
  HAL_RCC_LP_RNG_DisableClockInSleepMode();
#if defined(PKA)
  HAL_RCC_LP_PKA_DisableClockInSleepMode();
#endif /* PKA */
#if defined(SAES)
  HAL_RCC_LP_SAES_DisableClockInSleepMode();
#endif /* SAES */
#if defined(CCB)
  HAL_RCC_LP_CCB_DisableClockInSleepMode();
#endif /* CCB */
#if defined(ADC3)
  HAL_RCC_LP_ADC3_DisableClockInSleepMode();
#endif /* ADC3 */
#if defined(XSPI1)
  HAL_RCC_LP_XSPI1_DisableClockInSleepMode();
#endif /* XSPI1 */
  HAL_RCC_LP_TIM2_DisableClockInSleepMode();
#if defined(TIM3)
  HAL_RCC_LP_TIM3_DisableClockInSleepMode();
#endif /* TIM3 */
#if defined(TIM4)
  HAL_RCC_LP_TIM4_DisableClockInSleepMode();
#endif /* TIM4 */
#if defined(TIM5)
  HAL_RCC_LP_TIM5_DisableClockInSleepMode();
#endif /* TIM5 */
  HAL_RCC_LP_TIM6_DisableClockInSleepMode();
  HAL_RCC_LP_TIM7_DisableClockInSleepMode();
  HAL_RCC_LP_TIM12_DisableClockInSleepMode();
  HAL_RCC_LP_WWDG_DisableClockInSleepMode();
#if defined(OPAMP1)
  HAL_RCC_LP_OPAMP1_DisableClockInSleepMode();
#endif /* OPAMP1 */
  HAL_RCC_LP_SPI2_DisableClockInSleepMode();
#if defined(SPI3)
  HAL_RCC_LP_SPI3_DisableClockInSleepMode();
#endif /* SPI3 */
  HAL_RCC_LP_USART2_DisableClockInSleepMode();
#if defined(USART3)
  HAL_RCC_LP_USART3_DisableClockInSleepMode();
#endif /* USART3 */
  HAL_RCC_LP_UART4_DisableClockInSleepMode();
  HAL_RCC_LP_UART5_DisableClockInSleepMode();
#if defined(USART6)
  HAL_RCC_LP_USART6_DisableClockInSleepMode();
#endif /* USART6 */
#if defined(UART7)
  HAL_RCC_LP_UART7_DisableClockInSleepMode();
#endif /* UART7 */
  HAL_RCC_LP_I2C1_DisableClockInSleepMode();
#if defined(I2C2)
  HAL_RCC_LP_I2C2_DisableClockInSleepMode();
#endif /* I2C2 */
  HAL_RCC_LP_I3C1_DisableClockInSleepMode();
  HAL_RCC_LP_CRS_DisableClockInSleepMode();
  HAL_RCC_LP_COMP12_DisableClockInSleepMode();
#if defined(FDCAN1)
  HAL_RCC_LP_FDCAN_DisableClockInSleepMode();
#endif /* FDCAN1 */
  HAL_RCC_LP_TIM1_DisableClockInSleepMode();
  HAL_RCC_LP_SPI1_DisableClockInSleepMode();
  HAL_RCC_LP_TIM8_DisableClockInSleepMode();
  HAL_RCC_LP_USART1_DisableClockInSleepMode();
  HAL_RCC_LP_TIM15_DisableClockInSleepMode();
#if defined(TIM16)
  HAL_RCC_LP_TIM16_DisableClockInSleepMode();
#endif /* TIM16 */
#if defined(TIM17)
  HAL_RCC_LP_TIM17_DisableClockInSleepMode();
#endif /* TIM17 */
#if defined(USB_DRD_FS_BASE)
  HAL_RCC_LP_USB_DisableClockInSleepMode();
#endif /* USB_DRD_FS_BASE */
  HAL_RCC_LP_SBS_DisableClockInSleepMode();
  HAL_RCC_LP_LPUART1_DisableClockInSleepMode();
#if defined(LPTIM1)
  HAL_RCC_LP_LPTIM1_DisableClockInSleepMode();
#endif /* LPTIM1 */
  HAL_RCC_LP_RTCAPB_DisableClockInSleepMode();
  /* Disable buses clock */
  HAL_RCC_AHB1_DisableBusClock();
#if defined(XSPI1)
  HAL_RCC_AHB4_DisableBusClock();
#endif /* XSPI1 */
  HAL_RCC_APB1_DisableBusClock();
  HAL_RCC_APB2_DisableBusClock();
  HAL_RCC_APB3_DisableBusClock();
}
