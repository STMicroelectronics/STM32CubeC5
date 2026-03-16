
/**
  ******************************************************************************
  * @file           : mx_rcc.c
  * @brief          : STM32 RCC program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for RCC in LL layer */
/******************************************************************************/

/**
  * Configure the system core clock only and activate it using the LL RCC unitary APIs (footprint optimization)
  *         The system Clock is configured as follow :
  *            System Clock source            = HSIDIV3
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            Flash Latency(WS)              = 4
  */
system_status_t mx_rcc_init(void)
{
  /* LL_RCC_HSIDIV3_Enable();
  while(LL_RCC_HSIDIV3_IsReady() != 1U)
  {
  }
  */

  /** Initializes the CPU, AHB and APB busses clocks */
  LL_RCC_ConfigBusClock(LL_RCC_HCLK_PRESCALER_1 | LL_RCC_APB1_PRESCALER_1 |
                        LL_RCC_APB2_PRESCALER_1 | LL_RCC_APB3_PRESCALER_1);

  /* LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSIDIV3); */
  /* Wait till System clock is ready */
  /* while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3)
  {
  }
  */

  /** Frequency will be decreased */
  LL_FLASH_SetLatency(FLASH, LL_FLASH_LATENCY_4WS);
  LL_FLASH_SetProgrammingDelay(FLASH, LL_FLASH_PROGRAM_DELAY_2);

  LL_SetSystemCoreClock(48000000U);
  LL_Init1msTick(SystemCoreClock);

  /** Configure MCO (clock source, IO and divider) */

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    RCC GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA9     ------>   RCC_MCO2
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  /* LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_0); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);

  LL_RCC_ConfigMCO(LL_RCC_MCO2SOURCE_HSIDIV3, LL_RCC_MCO2_PRESCALER_15);

  return SYSTEM_OK;
}

void mx_rcc_deinit(void)
{
LL_RCC_WRITE_REG(CIER, RCC_CIER_Rst); /* Disable all interrupts */

  /* Reset System clock */
  LL_RCC_HSIDIV3_Enable();
  while(LL_RCC_HSIDIV3_IsReady() != 1U)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSIDIV3);
  /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3)
  {
  }

  /* Reset all bus dividers */
  LL_RCC_ConfigBusClock((uint32_t)LL_RCC_HCLK_PRESCALER_1 | (uint32_t)LL_RCC_APB1_PRESCALER_1 |
                        (uint32_t)LL_RCC_APB2_PRESCALER_1 |
                        (uint32_t)LL_RCC_APB3_PRESCALER_1);

  SystemCoreClock = HSI_RESET_VALUE;

  LL_RCC_SetSystickClockSource(LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8);

  /* Adapt Systick interrupt period */

  /* Reset all remaining oscillators not in backup domain */
  LL_RCC_WRITE_REG(CR1, RCC_CR1_Rst);
  LL_RCC_WRITE_REG(CR1, RCC_CR1_Rst); /* Called a second time to ensure HSE EXT and BYP disabled only when HSE has been disabled */

  LL_RCC_WRITE_REG(CR2, RCC_CR2_Rst);
  LL_RCC_LSI_Disable();

  /* Reset MCO, wake up system clock (not RTC prescaler) */
  uint32_t read_value = LL_RCC_READ_REG(CFGR1);
  LL_RCC_WRITE_REG(CFGR1, (read_value & RCC_CFGR1_RTCPRE) | (RCC_CFGR1_Rst & ~(RCC_CFGR1_RTCPRE)));

  /* Clear RCC flags */
  LL_RCC_ClearFlag(LL_RCC_IT_LSIRDY | LL_RCC_IT_LSERDY | LL_RCC_IT_HSIRDY | LL_RCC_IT_HSIDIV3RDY | \
                   LL_RCC_IT_HSIKRDY | LL_RCC_IT_PSIRDY | LL_RCC_IT_PSIDIV3RDY | LL_RCC_IT_PSIKRDY | \
                   LL_RCC_IT_HSERDY | LL_RCC_IT_HSECSS | LL_RCC_IT_LSECSS);

  LL_RCC_ForceClearResetFlags();
  LL_RCC_ReleaseClearResetFlags();}

/**
  * configures and activate the clocks used by all the peripherals selected within the project
  */
system_status_t mx_rcc_peripherals_clock_config(void)
{
  /* Peripherals using LSE (32.768 kHz):
    CRS
  */
  /* Disable RTC Domain Write Protection */
  LL_PWR_DisableRTCDomainWriteProtection();

  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_MEDIUMHIGH);
  LL_RCC_LSE_Enable();
  while(LL_RCC_LSE_IsReady() != 1U)
  {
  }
  /* In order to simplify the code generation and management for the user, the write protection is not enabled by
     default. In real case application, we advise to enable it once all the necessary configurations are done. */
  /* Enable RTC Domain Write Protection */
  //LL_PWR_EnableRTCDomainWriteProtection();

  return SYSTEM_OK;
}
