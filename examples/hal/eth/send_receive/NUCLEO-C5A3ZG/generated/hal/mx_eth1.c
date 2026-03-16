/**
  ******************************************************************************
  * @file           : mx_eth.c
  * @brief          : ETH Peripheral initialization
  ******************************************************************************
  * @attention
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
#include "mx_eth1.h"
#include "string.h" /* for memset() */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Ethernet Rx DMA Descriptors */

#define HAL_ETH_RX_DESC_CNT 10UL
#define HAL_ETH_TX_DESC_CNT 10UL

/* Private functions prototype------------------------------------------------*/
static uint32_t Fletcher16(const uint16_t *data, int32_t words);
static void ComputeMacAddressFromUid(const uint32_t uid[3], uint8_t macAddr[6]);
static void ReadChipUid(uint32_t uid[3]);
/* Exported variables by reference--------------------------------------------*/
hal_eth_handle_t hETH1;
ETH_TypeDef *test;

/* Private functions ---------------------------------------------------------*/
static uint32_t Fletcher16(const uint16_t *data, int32_t words)
{
  uint32_t sum1 = 0;
  uint32_t sum2 = 0;

  int32_t index;
  for (index = 0; index < words; ++index)
  {
    sum1 = (sum1 + data[index]) % 0xffff;
    sum2 = (sum2 + sum1) % 0xffff;
  }
  return (sum2 << 16) | sum1;
}

static void ComputeMacAddressFromUid(const uint32_t uid[3], uint8_t macAddr[6])
{
  if (macAddr != NULL && uid != NULL)
  {
    uint32_t fletcher = Fletcher16((const uint16_t *)uid, sizeof(uint32_t[3]) / sizeof(uint16_t));

    macAddr[0] = 0x02; /* Locally administered / Unicast (individual) */
    macAddr[1] = 0x00; /* Not used, but could be used to add entropy*/
    memcpy(&macAddr[2], &fletcher, sizeof(fletcher));
  }
}

static void ReadChipUid(uint32_t uid[3])
{
  /* Changing MPU config to read UID is not needed on STM32 */
  uid[0] = LL_GetUID_Word0();
  uid[1] = LL_GetUID_Word1();
  uid[2] = LL_GetUID_Word2();
}

/* Public functions ----------------------------------------------------------*/
void *mx_eth1_hal_init(void)
{

  /* init hal eth */
  memset(&hETH1, 0, sizeof(hal_eth_handle_t));

  if (HAL_ETH_Init(&hETH1, HAL_ETH1) != HAL_OK)
  {
    return NULL;
  }

  test = (ETH_TypeDef *)hETH1.instance;


  /* Initialize ETH GPIO Pins */

  /*
                  RMII configuration
      RMII_REF_CLK -----------------------> PA8 (AF14)
      RMII_MDIO --------------------------> PA2 (AF11)
      RMII_MDC ---------------------------> PD7 (AF14)
      RMII_CRS_DV ------------------------> PB5 (AF11)
      RMII_RXD0 --------------------------> PB6 (AF14)
      RMII_RXD1 --------------------------> PB7 (AF14)
      RMII_TX_EN -------------------------> PA7 (AF14)
      RMII_TX0 ---------------------------> PA9 (AF11)
      RMII_TX1 ---------------------------> PA10 (AF8)
  */

  hal_gpio_config_t  GPIO_InitStruct;

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/

  /* Enable Peripheral clocks */
  HAL_RCC_ETH1_EnableClock();
  HAL_RCC_ETH1CK_EnableClock();
  HAL_RCC_ETH1TX_EnableClock();
  HAL_RCC_ETH1RX_EnableClock();


  HAL_RCC_ETH1REF_SetKernelClkSource(HAL_RCC_ETH1REF_CLK_SRC_RMII);
  HAL_RCC_ETH1_SetKernelClkSource(HAL_RCC_ETH1_CLK_SRC_HSE);
  LL_RCC_SetETH1ClockSource(LL_RCC_ETH1REF_CLKSOURCE_RMII);
  HAL_RCC_GPIOC_EnableClock();
  HAL_RCC_GPIOA_EnableClock();
  HAL_RCC_GPIOG_EnableClock();
  HAL_RCC_GPIOE_EnableClock();
  HAL_RCC_GPIOD_EnableClock();


  /**ETH GPIO Configuration
  PC1     ------> ETH_MDC
  PA1     ------> ETH_REF_CLK
  PA2 -> PE12     ------> ETH_MDIO
  PA7 -> PD1     ------> ETH_CRS_DV
  PC4     ------> ETH_RXD0
  PC5     ------> ETH_RXD1
  PB15 -> PG12     ------> ETH_TXD1
  PG11     ------> ETH_TX_EN
  PG13     ------> ETH_TXD0
    */


  GPIO_InitStruct.mode = HAL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = HAL_GPIO_PULL_NO;
  GPIO_InitStruct.speed = HAL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.alternate = HAL_GPIO_AF_10;


  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_1, &GPIO_InitStruct);
  HAL_GPIO_Init(HAL_GPIOD, HAL_GPIO_PIN_1, &GPIO_InitStruct);
  HAL_GPIO_Init(HAL_GPIOG, HAL_GPIO_PIN_11 | HAL_GPIO_PIN_12 | HAL_GPIO_PIN_13, &GPIO_InitStruct);

  HAL_GPIO_Init(HAL_GPIOE, HAL_GPIO_PIN_12, &GPIO_InitStruct);


  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_1, &GPIO_InitStruct);

  GPIO_InitStruct.alternate = HAL_GPIO_AF_12;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_4, &GPIO_InitStruct);
  GPIO_InitStruct.alternate = HAL_GPIO_AF_13;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_5, &GPIO_InitStruct);

  /* ethernet config */
  hal_eth_config_t eth_config = {0};

  uint32_t uid[3] = {0U, 0U, 0U};
  /* Use device ID to generate unique MAC address */
  ReadChipUid(uid);
  ComputeMacAddressFromUid(uid, eth_config.mac_addr);
  eth_config.media_interface = HAL_ETH_MEDIA_IF_RMII;
  HAL_RCC_SBS_EnableClock();

  HAL_ETH_SetConfig(&hETH1, &eth_config);

  /* ETH interrupt Init */
  NVIC_SetPriority(ETH1_IRQn, 0);
  NVIC_EnableIRQ(ETH1_IRQn);

  return (void *)&hETH1;
}

void mx_eth1_hal_deinit(void)
{
  (void)HAL_ETH_DeInit(&hETH1);
  /* Disable ETH clock */
  /* Disable ETH GPIOs Pins */
  /*
                RMII configuration
    RMII_REF_CLK -----------------------> PA8 (AF14)
    RMII_MDIO --------------------------> PA2 (AF11)
    RMII_MDC ---------------------------> PD7 (AF14)
    RMII_CRS_DV ------------------------> PB5 (AF11)
    RMII_RXD0 --------------------------> PB6 (AF14)
    RMII_RXD1 --------------------------> PB7 (AF14)
    RMII_TX_EN -------------------------> PA7 (AF14)
    RMII_TX0 ---------------------------> PA9 (AF11)
    RMII_TX1 ---------------------------> PA10 (AF8)
  */

  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_7);
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7);
  HAL_GPIO_DeInit(HAL_GPIOD, HAL_GPIO_PIN_7);

  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_2 | HAL_GPIO_PIN_9);
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_5);

  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_10);
}

void *mx_eth1_hal_gethandle(void)
{
  return (void *)&hETH1;
}

void ETH1_IRQHandler(void)
{
  HAL_ETH_IRQHandler(&hETH1);
}
