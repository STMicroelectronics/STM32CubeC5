
/**
  ******************************************************************************
  * @file           : mx_eth1.c
  * @brief          : ETH1 Peripheral initialization
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
#include "mx_eth1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static hal_eth_handle_t hETH1;

/* Private functions prototype------------------------------------------------*/
static void mx_eth1_set_mac_address(uint8_t *pMacAddress);

/* Private functions ---------------------------------------------------------*/

static void mx_eth1_set_mac_address(uint8_t *pMacAddress)
{
  if (pMacAddress != NULL)
  {
    uint32_t uid[3];
    uint32_t sum1 = 0;
    uint32_t sum2 = 0;
    int32_t index;
    uint32_t fletcher;
    const uint16_t *data = (const uint16_t *)uid;

    uid[0] = LL_GetUID_Word0();
    uid[1] = LL_GetUID_Word1();
    uid[2] = LL_GetUID_Word2();

    /* Fletcher16 computation */
    for (index = 0; index < (sizeof(uint32_t[3]) / sizeof(uint16_t)); ++index)
    {
      sum1 = (sum1 + data[index]) % 0xffff;
      sum2 = (sum2 + sum1) % 0xffff;
    }
    fletcher = ((sum2 << 16) | sum1);

    pMacAddress[0] = 0x02; /* Locally administered / Unicast (individual) */
    pMacAddress[1] = 0x00; /* Not used, but could be used to add entropy*/
    pMacAddress[2] = ((fletcher      ) & 0xFF);
    pMacAddress[3] = ((fletcher >>  8) & 0xFF);
    pMacAddress[4] = ((fletcher >> 16) & 0xFF);
    pMacAddress[5] = ((fletcher >> 24) & 0xFF);
  }
}

/******************************************************************************/
/* Exported functions for ETH1 in HAL layer (SW instance MyETH_1) */
/******************************************************************************/
hal_eth_handle_t *mx_eth1_init(void)
{
  /* Ethernet config */
  hal_eth_config_t eth_config;
  /* Ethernet MAC config */
  hal_eth_mac_config_t eth_mac_config;
  /* Ethernet DMA-Tx Channel Config */
  hal_eth_tx_channel_config_t eth_tx_channel_config;
  /* Ethernet DMA-Rx Channel Config */
  hal_eth_rx_channel_config_t eth_rx_channel_config;

  /****************************************************************************/
  /* Configuration of Clocks                                                  */
  /****************************************************************************/
  HAL_RCC_ETH1_EnableClock();

  HAL_RCC_ETH1TX_EnableClock();
  HAL_RCC_ETH1RX_EnableClock();
  HAL_RCC_ETH1CK_EnableClock();
          HAL_RCC_SBS_EnableClock();

  if (HAL_RCC_ETH1_SetKernelClkSource(HAL_RCC_ETH1_CLK_SRC_PSIS) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_RCC_ETH1REF_SetKernelClkSource(HAL_RCC_ETH1REF_CLK_SRC_RMII) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/
  HAL_RCC_GPIOA_EnableClock();

  HAL_RCC_GPIOC_EnableClock();

  HAL_RCC_GPIOE_EnableClock();

  HAL_RCC_GPIOD_EnableClock();

  HAL_RCC_GPIOG_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA10    ------>   ETH1_CLK
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_13;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_10, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC1     ------>   ETH1_MDC
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_10;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_1, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PE12    ------>   ETH1_MDIO
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_10;
  HAL_GPIO_Init(HAL_GPIOE, HAL_GPIO_PIN_12, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PD1     ------>   ETH1_RMII_CRS_DV
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_10;
  HAL_GPIO_Init(HAL_GPIOD, HAL_GPIO_PIN_1, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA1     ------>   ETH1_RMII_REF_CLK
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_10;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_1, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC4     ------>   ETH1_RMII_RXD0
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_12;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_4, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC5     ------>   ETH1_RMII_RXD1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_13;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_5, &gpio_config);

  /**
    ETH1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PG11    ------>   ETH1_RMII_TX_EN
       PG13    ------>   ETH1_RMII_TXD0
       PG12    ------>   ETH1_RMII_TXD1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_10;
  HAL_GPIO_Init(HAL_GPIOG, HAL_GPIO_PIN_11 | HAL_GPIO_PIN_13 | HAL_GPIO_PIN_12, &gpio_config);

  if (HAL_ETH_Init(&hETH1, HAL_ETH1) != HAL_OK)
  {
    return NULL;
  }

  /* Set the MAC Address */
  mx_eth1_set_mac_address(&eth_config.mac_addr[0]);

  /* Set the Media Interface type */
  eth_config.media_interface = HAL_ETH_MEDIA_IF_RMII;

  /*
   * Call the Ethernet HAL API HAL_ETH_SetConfig() to set Peripheral's
   * User Configuration.
   */
  if(HAL_ETH_SetConfig(&hETH1, &eth_config) != HAL_OK)
  {
    HAL_ETH_DeInit(&hETH1);
    return NULL;
  }

  /*
   * Call the Ethernet HAL API HAL_ETH_MAC_GetConfig() to get the default
   * Peripheral's MAC Configuration.
   */
  HAL_ETH_MAC_GetConfig(&hETH1, &eth_mac_config);

  /* Set the Ethernet MAC Link Configuration */
  eth_mac_config.link_config.speed        = HAL_ETH_MAC_SPEED_100M;
  eth_mac_config.link_config.duplex_mode  = HAL_ETH_MAC_FULL_DUPLEX_MODE;
  /* Set the Ethernet MAC Loopback Mode */
  eth_mac_config.loopback_mode            = HAL_ETH_MAC_LOOPBACK_DISABLE;

  /*
   * Call the Ethernet HAL API HAL_ETH_MAC_SetConfig() to set the Peripheral's
   * MAC Configuration.
   */
  if(HAL_ETH_MAC_SetConfig(&hETH1, &eth_mac_config) != HAL_OK)
  {
    HAL_ETH_DeInit(&hETH1);
    return NULL;
  }

  /*
   * Call the Ethernet HAL API HAL_ETH_GetConfigTxChannel() to get the default
   * configuration for the Tx Channel 0.
   */
  HAL_ETH_GetConfigTxChannel(&hETH1, HAL_ETH_TX_CHANNEL_0,
                             &eth_tx_channel_config);

  /* Set the Tx Channel 0 Configuration */
  eth_tx_channel_config.max_app_buffers_num                         = 10UL;
  eth_tx_channel_config.req_desc_size_align_byte                    = 1UL;
  eth_tx_channel_config.fifo_event_config.event_mode                = HAL_ETH_FIFO_EVENT_ALWAYS;
  eth_tx_channel_config.dma_channel_config.tx_dma_burst_length      = HAL_ETH_DMA_TX_BLEN_4_BEAT;
  eth_tx_channel_config.dma_channel_config.tx_pbl_x8_mode           = HAL_ETH_DMA_TX_PBL_X8_DISABLE;
  eth_tx_channel_config.dma_channel_config.tx_second_pkt_operate    = HAL_ETH_DMA_TX_SEC_PKT_OP_ENABLE;
  eth_tx_channel_config.mtl_queue_config.queue_size_byte            = HAL_ETH_MTL_TX_QUEUE_SZ_2048_BYTE;
  eth_tx_channel_config.mtl_queue_config.transmit_queue_mode        = HAL_ETH_MTL_TX_Q_STORE_AND_FORWARD;
  eth_tx_channel_config.mtl_queue_config.queue_op_mode              = HAL_ETH_MTL_TX_QUEUE_ENABLED;

  /*
   * Call the Ethernet HAL API HAL_ETH_SetConfigTxChannel() to set the
   * Tx channel 0 Configuration.
   */
  if(HAL_ETH_SetConfigTxChannel(&hETH1, HAL_ETH_TX_CHANNEL_0,
                                &eth_tx_channel_config) != HAL_OK)
  {
    HAL_ETH_DeInit(&hETH1);
    return NULL;
  }

  /*
   * Call the Ethernet HAL API HAL_ETH_GetConfigRxChannel() to get the default
   * configuration for the Rx channel 0.
   */
  HAL_ETH_GetConfigRxChannel(&hETH1, HAL_ETH_RX_CHANNEL_0,
                             &eth_rx_channel_config);

  /* Set the Rx Channel 0 Configuration */
  eth_rx_channel_config.dma_channel_config.rx_buffer_len_byte       = 1520UL;
  eth_rx_channel_config.max_app_buffers_num                         = 10UL;
  eth_rx_channel_config.req_desc_size_align_byte                    = 1UL;
  eth_rx_channel_config.fifo_event_config.event_mode                = HAL_ETH_FIFO_EVENT_ALWAYS;
  eth_rx_channel_config.dma_channel_config.rx_dma_burst_length      = HAL_ETH_DMA_RX_BLEN_4_BEAT;
  eth_rx_channel_config.mtl_queue_config.queue_size_byte            = HAL_ETH_MTL_RX_QUEUE_SZ_2048_BYTE;
  eth_rx_channel_config.mtl_queue_config.receive_queue_mode         = HAL_ETH_MTL_RX_Q_STORE_AND_FORWARD;
  eth_rx_channel_config.mtl_queue_config.queue_op_mode              = HAL_ETH_MTL_RX_QUEUE_ENABLED;
  eth_rx_channel_config.mtl_queue_config.drop_tcp_ip_csum_error_pkt = HAL_ETH_MTL_RX_DROP_CS_ERR_ENABLE;
  eth_rx_channel_config.mtl_queue_config.fwd_error_pkt              = HAL_ETH_MTL_RX_FWD_ERR_PKT_DISABLE;
  eth_rx_channel_config.mtl_queue_config.fwd_undersized_good_pkt    = HAL_ETH_MTL_RX_FWD_USZ_PKT_ENABLE;

  /*
   * Call the Ethernet HAL API HAL_ETH_SetConfigRxChannel() to set the
   * Rx channel 0 Configuration.
   */
  if(HAL_ETH_SetConfigRxChannel(&hETH1, HAL_ETH_RX_CHANNEL_0,
                                &eth_rx_channel_config) != HAL_OK)
  {
    HAL_ETH_DeInit(&hETH1);
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of NVIC                                                    */
  /****************************************************************************/
  HAL_CORTEX_NVIC_SetPriority(ETH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_7, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(ETH1_IRQn);

  return &hETH1;
}

void mx_eth1_deinit(void)
{
  /* Disable the ETH Global interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(ETH1_IRQn);

  /* Deinitialize the ETH peripheral */
  (void)HAL_ETH_DeInit(&hETH1);

  /* De-initialize all GPIO pins associated with ETH1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_1 | HAL_GPIO_PIN_10);

  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_1 | HAL_GPIO_PIN_4 | HAL_GPIO_PIN_5);

  HAL_GPIO_DeInit(HAL_GPIOE, HAL_GPIO_PIN_12);

  HAL_GPIO_DeInit(HAL_GPIOD, HAL_GPIO_PIN_1);

  HAL_GPIO_DeInit(HAL_GPIOG, HAL_GPIO_PIN_11 | HAL_GPIO_PIN_12 | HAL_GPIO_PIN_13);
}

hal_eth_handle_t *mx_eth1_gethandle(void)
{
  return &hETH1;
}

/******************************************************************************/
/*                             Ethernet interrupt                             */
/******************************************************************************/

void ETH1_IRQHandler(void)
{
  HAL_ETH_IRQHandler(&hETH1);
}
