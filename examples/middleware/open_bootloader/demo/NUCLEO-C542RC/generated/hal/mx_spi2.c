/**
  ******************************************************************************
  * @file           : mx_spi2.c
  * @brief          : SPI2 Peripheral initialization
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
#include "mx_spi2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_spi_handle_t hSPI2;

/******************************************************************************/
/* Exported functions for SPI in HAL layer */
/******************************************************************************/
hal_spi_handle_t *mx_spi2_init(void)
{
  hal_spi_config_t spi_config;

  if (HAL_SPI_Init(&hSPI2, HAL_SPI2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_SPI2_EnableClock();

  if (HAL_RCC_SPI2_SetKernelClkSource(HAL_RCC_SPI2_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  spi_config.mode = HAL_SPI_MODE_SLAVE;
  spi_config.direction = HAL_SPI_DIRECTION_FULL_DUPLEX;
  spi_config.data_width = HAL_SPI_DATA_WIDTH_8_BIT;
  spi_config.clock_polarity = HAL_SPI_CLOCK_POLARITY_LOW;
  spi_config.clock_phase = HAL_SPI_CLOCK_PHASE_1_EDGE;
  spi_config.baud_rate_prescaler = HAL_SPI_BAUD_RATE_PRESCALER_2;
  spi_config.first_bit = HAL_SPI_MSB_FIRST;
  spi_config.nss_pin_management = HAL_SPI_NSS_PIN_MGMT_INTERNAL;

  if (HAL_SPI_SetConfig(&hSPI2, &spi_config) != HAL_OK)
  {
    return NULL;
  }

  hal_spi_underrun_config_t spi_underrun_config;

  spi_underrun_config.underrun_behavior = HAL_SPI_UNDERRUN_BEHAV_REGISTER_PATTERN;

  if (HAL_SPI_SLAVE_SetConfigUnderrun(&hSPI2, &spi_underrun_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### SPI2 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PB13    ------>   SPI2_SCK   ------>  PB13
       PB14    ------>   SPI2_MISO   ------>  PB14
       PB15    ------>   SPI2_MOSI   ------>  PB15
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOB, PB13_PIN | PB14_PIN | PB15_PIN, &gpio_config);

  /* Enable the interrupt for SPI */
  HAL_CORTEX_NVIC_SetPriority(SPI2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(SPI2_IRQn);

  return &hSPI2;
}

void mx_spi2_deinit(void)
{
  /* Disable the interrupt for SPI */
  HAL_CORTEX_NVIC_DisableIRQ(SPI2_IRQn);

  (void)HAL_SPI_DeInit(&hSPI2);

  HAL_RCC_SPI2_Reset();

  HAL_RCC_SPI2_DisableClock();

  /* De-initialize all GPIOB pins associated with SPI2 */
  HAL_GPIO_DeInit(HAL_GPIOB, PB13_PIN | PB14_PIN | PB15_PIN);
}

hal_spi_handle_t *mx_spi2_gethandle(void)
{
  return &hSPI2;
}

/******************************************************************************/
/*          SPI2 global interrupt is managed directly in user code.           */
/******************************************************************************/
/* void SPI2_IRQHandler(void)
{
}
  */
