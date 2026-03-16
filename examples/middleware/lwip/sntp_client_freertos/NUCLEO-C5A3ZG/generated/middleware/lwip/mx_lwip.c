/**
  ******************************************************************************
  * @file           : mx_lwip.c
  * @brief          : LwIP STM32 ethernet interface.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lwip_license.md file
  * in the same directory as the generated code.
  * If no mx_lwip_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_lwip.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/etharp.h"
#if LWIP_IPV6 && LWIP_ETHERNET
#include "lwip/ethip6.h"
#endif /* LWIP_IPV6 && LWIP_ETHERNET */
#include "lwip/memp.h"
#include "lwip/netifapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdbool.h"
#include "string.h"
#include "mx_eth1.h"

/* Private defines -----------------------------------------------------------*/
#define ETH_INTERFACE_1_RX_CHANNEL HAL_ETH_RX_CHANNEL_0
#define ETH_INTERFACE_1_TX_CHANNEL HAL_ETH_TX_CHANNEL_0

#define ETH_INTERFACE_1_NETIF_HOSTNAME_PREFIX "stm32_host_mx_1"
#define ETH_INTERFACE_1_NETIF_NAME            "01"

#define ETH_INTERFACE_1_VLAN_ID 0 /* VLAN not used for this interface*/

#define ETH_INTERFACE_DESC_SIZE_ALIGN_BYTE 1

#define ETH_INTERFACE_MAX_HOSTNAME_LEN     64

#define HAL_RNG_TIMEOUT_MS 1000

#ifndef MX_PHY_INTERFACE_VERSION
#error "MX PHY interface not defined"
#if (MX_PHY_INTERFACE_VERSION != 1)
#error "MX PHY interface version incompatible"
#endif
#endif

/* For each interface, check if LwIP ETHARP_SUPPORT_VLAN must be enabled*/
#if ETH_INTERFACE_1_VLAN_ID != 0
#if !defined (ETHARP_SUPPORT_VLAN) || (defined( ETHARP_SUPPORT_VLAN) && (ETHARP_SUPPORT_VLAN==0))
#error "LwIP ETHARP_SUPPORT_VLAN must be enabled"
#endif
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
lwip_eth_interface_netif_context_t netif_context_1;
TaskHandle_t xLinkMonitorTaskHandle1;
lwip_eth_interface_hardware_t eth_interface_hardware_1;
char netif_1_hostname[ETH_INTERFACE_MAX_HOSTNAME_LEN] = {0};
static void(*sntp_set_system_time_fn)(u32_t sec, u32_t usec) = NULL;

/* Private function prototypes -----------------------------------------------*/
static err_t mx_lwip_igmp_mac_filter(struct netif *netif,
       const ip4_addr_t *group, enum netif_mac_filter_action action);
/* Functions Definition ------------------------------------------------------*/

static err_t mx_lwip_igmp_mac_filter(struct netif *netif,
                                     const ip4_addr_t *group, enum netif_mac_filter_action action)
{
  (void) netif;
  (void) group;
  (void) action;
  return ERR_OK;
}

/**
  * @brief  LwIP initialisation function.
  * @param  none
  * @retval none
  */
err_t mx_lwip_init(void)
{
  hal_status_t hal_status;
  err_t err;
  uint32_t random = 0;

  tcpip_init(NULL, NULL);

  err = lwip_eth_interface_init();

  if(err == ERR_OK)
  {
     mx_rng_init();
    /* set the C library random seed from HAL_RNG_GenerateRandomNumber() */
    /* to have random TCP port numbers in LwIP */
    /* Note: RNG IP must be initialised */
    hal_status = HAL_RNG_GenerateRandomNumber(mx_rng_gethandle(), &random, 1, HAL_RNG_TIMEOUT_MS);
    if(hal_status == HAL_OK)
    {
      srand(random);
    }
    else 
    {
      err = ERR_IF;
    }
  }

  
  return err;
}

err_t mx_lwip_deinit(void)
{
  err_t err;

  err = lwip_eth_interface_deinit();

  return err;
}

err_t mx_lwip_init_interface_1(struct netif *p_netif)
{ 
  lwip_eth_interface_netif_context_t *p_netif_context = NULL;
  mx_phy_status_t phy_status;

  hal_eth_config_t eth_config;
  
  if(p_netif == NULL || (p_netif->state == NULL))
  {
    return ERR_VAL;
  }

  /* PHY init Conditional in MX */
  phy_status = mx_phy1_init();
  if(phy_status != MX_PHY_STATUS_OK)
  {
    LWIP_ASSERT("phy init failed", false);
    return ERR_MEM;
  }
  
  p_netif_context = (lwip_eth_interface_netif_context_t *)(p_netif->state);

  memset(p_netif_context, 0, sizeof(lwip_eth_interface_netif_context_t));

  /* Save Netif pointer */
  p_netif_context->p_netif = p_netif;

  /* Configure VLAN ID */
  p_netif_context->vlan_id = ETH_INTERFACE_1_VLAN_ID; 

  /* Configure hardware interface */
  eth_interface_hardware_1.p_phy = mx_phy1_get_interface();
  eth_interface_hardware_1.p_eth = mx_eth1_gethandle();
  p_netif_context->p_hardware = &eth_interface_hardware_1;

  /* Configure TX Channel */
  p_netif_context->tx_channel_id = ETH_INTERFACE_1_TX_CHANNEL;

  /* Configure RX Channel */
  p_netif_context->rx_channel_id = ETH_INTERFACE_1_RX_CHANNEL;
  
  /* set output() function for frames with unknown destination MAC address (need ARP) */
  p_netif->output = etharp_output;

#if LWIP_IPV6
  p_netif->output_ip6 = ethip6_output;
  p_netif->ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6 */

  /* set linkoutput() function for frames with known destination MAC address */
  p_netif->linkoutput = lwip_eth_interface_low_level_output;

  /* Set the Maximum Transfer Unit. */
  p_netif->mtu = (u16_t) HAL_ETH_MAX_PAYLOAD_SIZE_BYTE;

  /* Set device capabilities. Don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one */
  p_netif->flags = (u8_t)(NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET);

  /* Do whatever else is needed to initialize interface. */
#if LWIP_IGMP
  p_netif->flags |= NETIF_FLAG_IGMP;
  netif_set_igmp_mac_filter(p_netif, mx_lwip_igmp_mac_filter);
#endif /* LWIP_IGMP */
#if LWIP_NETIF_HOSTNAME
  HAL_ETH_GetConfig(p_netif_context->p_hardware->p_eth, &eth_config);
  /* Initialize interface host name: prefix-MacAddr */
  snprintf(netif_1_hostname, sizeof(netif_1_hostname),
           "%s-%02x%02x%02x%02x%02x%02x",
           ETH_INTERFACE_1_NETIF_HOSTNAME_PREFIX,
           eth_config.mac_addr[0], eth_config.mac_addr[1], eth_config.mac_addr[2],
           eth_config.mac_addr[3], eth_config.mac_addr[4], eth_config.mac_addr[5]);
  netif_set_hostname(p_netif, netif_1_hostname);
#else
  (void) eth_config;
#endif /* LWIP_NETIF_HOSTNAME */

  /* Initialize interface name */
  p_netif->name[0] = ETH_INTERFACE_1_NETIF_NAME[0];
  p_netif->name[1] = ETH_INTERFACE_1_NETIF_NAME[1];

  return lwip_eth_interface_low_level_init(p_netif);
}

void mx_lwip_deinit_interface_1(struct netif *p_netif)
{
  err_t err;
  err = lwip_eth_interface_low_level_deinit(p_netif);
  if(err == ERR_OK)
  {
    memset(&netif_context_1, 0, sizeof(lwip_eth_interface_netif_context_t));

    /* PHY deinit Conditional in MX */
    (void) mx_phy1_deinit();
  }
}

lwip_eth_interface_netif_context_t *mx_lwip_get_interface_context_1(void)
{
  return &netif_context_1;
}

void mx_lwip_sntp_register_set_system_time_fn(void(*fn)(u32_t sec, u32_t usec))
{
  sntp_set_system_time_fn = fn;
}

/**
 * @brief This function is called by the SNTP client when a time update is received.
 * It is register to lwip sntp client via macro SNTP_SET_SYSTEM_TIME_US in lwipopts.h
 * 
 * @param sec 
 * @param usec 
 */
void mx_lwip_sntp_set_system_time(u32_t sec, u32_t usec) {
  if(sntp_set_system_time_fn != NULL)
  {
    sntp_set_system_time_fn(sec, usec);
  }
}

