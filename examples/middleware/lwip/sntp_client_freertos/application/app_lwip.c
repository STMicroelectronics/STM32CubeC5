/**
  ******************************************************************************
  * File Name          : app_lwip.c
  * Description        : This file provides code for TCP echo LwIP example.
  ******************************************************************************
  * @attention
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
#include "app_lwip.h"
#include "example.h"

#include "mx_lwip.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "lwip/api.h"
#include "lwip/tcpip.h"

#include "lwip/dhcp.h"
#include "timers.h"
#include "lwip/inet.h"
#include "lwip/netifapi.h"
#include "lwip/dns.h"
#include "lwip/apps/mdns.h"

#include "app_config.h"
#include "app_logger.h"

#include <string.h>
#include <arpa/inet.h>

#if !(defined(LWIP_DHCP) && (LWIP_DHCP == 1))
#error "LWIP_DHCP must be defined and set to 1 in lwipopts.h to run this example"
#endif /* !(defined(LWIP_DHCP) && (LWIP_DHCP == 1)) */

#if !(defined(LWIP_MDNS_RESPONDER) && (LWIP_MDNS_RESPONDER == 1))
#error "LWIP_MDNS_RESPONDER must be defined and set to 1 in lwipopts.h to run this example"
#endif /* !(defined(LWIP_MDNS_RESPONDER) && (LWIP_MDNS_RESPONDER == 1)) */

#if !(defined(LWIP_NETIF_HOSTNAME) && (LWIP_NETIF_HOSTNAME == 1))
#error "LWIP_NETIF_HOSTNAME must be defined and set to 1 in lwipopts.h to run this example"
#endif /* !(defined(LWIP_NETIF_HOSTNAME) && (LWIP_NETIF_HOSTNAME == 1)) */

#if !(defined(LWIP_IPV4) && (LWIP_IPV4 == 1))
#error "LWIP_IPV4 must be defined and set to 1 in lwipopts.h to run this example"
#endif /* !(defined(LWIP_IPV4) && (LWIP_IPV4 == 1)) */

#if (defined(LWIP_IPV6) && (LWIP_IPV6 == 1))
#error "LWIP_IPV6 not supported for this example"
#endif /* (defined(LWIP_IPV6) && (LWIP_IPV6 == 1)) */

/* Private typedef -----------------------------------------------------------*/
typedef struct app_lwip_dns_arg
{
  TaskHandle_t task_handle;
  ip_addr_t ipaddr;
  app_status_t status;
} app_lwip_dns_arg_t;

/* Private define ------------------------------------------------------------*/
#define APP_NAME      "AppLwIP"
#define APP_LINK_DOWN 0
#define APP_LINK_UP   1
/* Internal events */
#define EVT_DNS_FOUND 0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

EventGroupHandle_t AppLwipEvents;

static TaskHandle_t AppLwipTask;

static TimerHandle_t AppLwipDhcpTimerHandle;

/* The LwIP netif structure to be used. */
static struct netif Netif1;

static netif_ext_callback_t AppLwipNetifExtCallbackListener;

/* Private functions prototype -----------------------------------------------*/
static void AppLwipNetifExtCallbackFn(struct netif *netif, netif_nsc_reason_t reason,
                                      const netif_ext_callback_args_t *p_args);
static void AppLwipThread(void *args);

static void AppLwipDnsFoundCallback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

static void AppLwipSetManualIPv4Addr(struct netif *netif);

static void AppLwipDhcpTimeoutCallback(TimerHandle_t xTimer);

static void AppLwipMdnsSetup(struct netif *netif);

static void AppLwipMdnsResultCb(struct netif *netif, u8_t result, s8_t slot);

/* Private functions ---------------------------------------------------------*/


void AppLwipDnsFoundCallback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
  char ip_str[INET_ADDRSTRLEN];

  app_lwip_dns_arg_t *app_arg = (app_lwip_dns_arg_t *)callback_arg;

  if (ipaddr != NULL)
  {
    app_arg->ipaddr = *ipaddr;

    inet_ntop(AF_INET, &(app_arg->ipaddr), ip_str, INET_ADDRSTRLEN);
    APP_LOG_INFO(APP_NAME, "DNS resolution callback done: %s -> %s\n", name, ip_str);

    app_arg->status = EXEC_STATUS_OK;
  }
  else
  {
    APP_LOG_INFO(APP_NAME, "DNS resolution callback failed for %s\n", name);

    app_arg->status = EXEC_STATUS_ERROR;
  }

  xTaskNotifyGiveIndexed(app_arg->task_handle, EVT_DNS_FOUND);
}

static void AppLwipSetManualIPv4Addr(struct netif *netif)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  ip_addr_t ip_addr_dns;

  APP_LOG_INFO(APP_NAME, "Netif(%c%c): Setting manual IP address\n", netif->name[0], netif->name[1]);

  /* Set the DNS server address if DHCP failed or if DHCP is not enabled */
  ip_addr_dns.addr = inet_addr(APP_LWIP_MANUAL_DNS_ADDR);
  dns_setserver(0, &ip_addr_dns);

  ip4addr_aton(APP_LWIP_MANUAL_IP_ADDR, &ipaddr);
  ip4addr_aton(APP_LWIP_MANUAL_NETMASK, &netmask);
  ip4addr_aton(APP_LWIP_MANUAL_GW_ADDR, &gw);

  netifapi_netif_set_addr(netif, &ipaddr, &netmask, &gw);

  /* DHCP specific configuration */
  APP_LOG_INFO(APP_NAME, "Inform DHCP server of manual configuration\n");
  netifapi_dhcp_inform(netif);
  /* End of DHCP specific configuration */
}

static void AppLwipDhcpTimeoutCallback(TimerHandle_t xTimer)
{
  struct netif *netif = (struct netif *)pvTimerGetTimerID(xTimer);

  /* Check if we have an IP */
  if (ip4_addr_isany_val(*netif_ip4_addr(netif)))
  {
    APP_LOG_INFO(APP_NAME, "DHCP timeout, stop DHCP client\n");

    netifapi_dhcp_stop(netif);

    AppLwipSetManualIPv4Addr(netif);
  }
  else
  {
    /* We already have an IP, nothing to do (normally the timer is stopped earlier). */
  }
}

static void AppLwipMdnsSetup(struct netif *netif)
{
  if (strlen(APP_LWIP_MDNS_HOSTNAME) != 0)
  {
    netif_set_hostname(netif, APP_LWIP_MDNS_HOSTNAME);
  }

  mdns_resp_init();
  mdns_resp_add_netif(netif, netif_get_hostname(netif));
  mdns_resp_register_name_result_cb(AppLwipMdnsResultCb);
}

static void AppLwipMdnsResultCb(struct netif *netif, u8_t result, s8_t slot)
{
  if (result == MDNS_PROBING_SUCCESSFUL)
  {
    APP_LOG_INFO(APP_NAME, "Netif(%c%c):mDNS hostname \"%s\" registered successfully (slot %d)\n",
                 netif->name[0], netif->name[1], netif_get_hostname(netif), slot);
  }
  else
  {
    APP_LOG_ERROR(APP_NAME, "Netif(%c%c):mDNS hostname \"%s\" registration failed (result %d, slot %d)\n",
                  netif->name[0], netif->name[1], netif_get_hostname(netif), result, slot);
  }
}

static void AppLwipNetifExtCallbackFn(struct netif *netif, netif_nsc_reason_t reason,
                                      const netif_ext_callback_args_t *p_args)
{
  (void)netif;
  (void)p_args;

  if ((LWIP_NSC_IPV4_ADDR_VALID & reason) == LWIP_NSC_IPV4_ADDR_VALID)
  {
    APP_LOG_INFO(APP_NAME, "Netif(%c%c): IPv4 address valid:\n", netif->name[0], netif->name[1]);
    APP_LOG_INFO(APP_NAME, "IP addr: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    APP_LOG_INFO(APP_NAME, "Netmask: %s\n", ip4addr_ntoa(netif_ip4_netmask(netif)));
    APP_LOG_INFO(APP_NAME, "Gateway: %s\n", ip4addr_ntoa(netif_ip4_gw(netif)));
    for (int i = 0; i < DNS_MAX_SERVERS; i++)
    {
      const ip_addr_t *addr = dns_getserver(i);
      if (!ip_addr_isany(addr))
      {
        APP_LOG_INFO(APP_NAME, "DNS[%d]:  %s\n", i, ip4addr_ntoa(ip_2_ip4(addr)));
      }
    }

    /* mDNS specific configuration */
    mdns_resp_restart(netif);
    /* End of mDNS specific configuration */

    /* Wake all tasks waiting for IP address */
    xEventGroupSetBits(AppLwipEvents, EVT_IP_ADDR_SET);

    /* DHCP specific configuration */
    xTimerStop(AppLwipDhcpTimerHandle, 0);
    /* End of DHCP specific configuration */
  }
  if ((LWIP_NSC_LINK_CHANGED & reason) == LWIP_NSC_LINK_CHANGED)
  {
    netif_ext_callback_args_t *p_link_args = (netif_ext_callback_args_t *)p_args;
    if (p_link_args != NULL)
    {
      if (p_link_args->status_changed.state == APP_LINK_UP)
      {
        APP_LOG_INFO(APP_NAME, "Netif(%c%c): Ethernet Link is up\n", netif->name[0], netif->name[1]);
        /* Link is up */
        /* DHCP specific configuration */
        APP_LOG_INFO(APP_NAME, "Netif(%c%c): Start DHCP client\n", netif->name[0], netif->name[1]);
        netifapi_dhcp_start(netif);
        xTimerStart(AppLwipDhcpTimerHandle, 0);
        /* End of DHCP specific configuration */
      }
      else if (p_link_args->status_changed.state == APP_LINK_DOWN)
      {
        APP_LOG_INFO(APP_NAME, "Netif(%c%c): Ethernet Link is down\n", netif->name[0], netif->name[1]);
        /* DHCP specific configuration */
        APP_LOG_INFO(APP_NAME, "Netif(%c%c): Stop DHCP client\n", netif->name[0], netif->name[1]);
        netifapi_dhcp_stop(netif);
        xTimerStop(AppLwipDhcpTimerHandle, 0);
        /* End of DHCP specific configuration */
      }
    }
  }
}

/**
  * brief:  lwip start thread
  * param: arg: pointer on argument(not used here)
  * retval: None
  */
static void AppLwipThread(void *args)
{
  err_t err;

  APP_LOG_INFO(APP_NAME, "Initializing LwIP\n");

  /* DHCP specific configuration */
  AppLwipDhcpTimerHandle = xTimerCreate("DHCP Timer Timeout",
                                        pdMS_TO_TICKS(APP_LWIP_DHCP_TIMEOUT_MS),
                                        pdFALSE,       /* one-shot timer */
                                        (void *)&Netif1, /* timer ID = netif pointer */
                                        AppLwipDhcpTimeoutCallback);
  if (AppLwipDhcpTimerHandle == NULL)
  {
    APP_LOG_INFO(APP_NAME, "DHCP Timer creation failed\n");
  }
  /* End of DHCP specific configuration */

  /** ########## Step 3.1 ##########
    * Initializes the LwIP stack
    * (only once during the entire life of the application.)
    */
  err = mx_lwip_init();
  if (err != ERR_OK)
  {
    APP_LOG_ERROR(APP_NAME, "mx lwip init failed (%d)\n", err);
  }

  /** ########## Step 3.2 ##########
    * Initializes and registers network interface
    */
  /* Add applicative NetifExtCallback*/
  netif_add_ext_callback(&AppLwipNetifExtCallbackListener, AppLwipNetifExtCallbackFn);

  /* Set remove callback */
  netif_set_remove_callback(&Netif1, &mx_lwip_deinit_interface_1);

  /* Add the network interface */
  netifapi_netif_add(&Netif1,
                     NULL, NULL, NULL,
                     mx_lwip_get_interface_context_1(), &mx_lwip_init_interface_1, &tcpip_input);

  /* Register the default network interface. */
  netifapi_netif_set_default(&Netif1);

  /* Set the network interface to an operational state, enabling network connectivity*/
  netifapi_netif_set_up(&Netif1);

  /* mDNS specific configuration */
  AppLwipMdnsSetup(&Netif1);
  /* End of mDNS specific configuration */

  /** ########## Step 3.3 ##########
    * Acquires IP address
    */
  APP_LOG_INFO(APP_NAME, "Waiting for IP address\n");
  xEventGroupWaitBits(AppLwipEvents,
                      EVT_IP_ADDR_SET,
                      pdTRUE,
                      pdFALSE,
                      portMAX_DELAY);

  /* Inspect task stack high water mark on exit. */
  APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%lu)\n", uxTaskGetStackHighWaterMark(NULL));

  /* Delete the task */
  APP_LOG_INFO(APP_NAME, "Delete Task\n");
  vTaskDelete(NULL);
}

/* Public functions ----------------------------------------------------------*/

void app_lwip_init(void)
{
  BaseType_t xReturn;
  xReturn = xTaskCreate(AppLwipThread, APP_NAME, configMINIMAL_STACK_SIZE,
                        (void *) NULL, APP_LWIP_START_THREAD_PRIO, &AppLwipTask);
  if (xReturn != pdPASS)
  {
    APP_LOG_ERROR(APP_NAME, "Task Creation failed (%ld)\n", xReturn);
  }

  AppLwipEvents = xEventGroupCreate();
  if (AppLwipEvents == NULL)
  {
    APP_LOG_ERROR(APP_NAME, "Event group creation failed\n");
  }

}

void app_lwip_deinit(void)
{
  err_t err;
  err = mx_lwip_deinit();
  if (err != ERR_OK)
  {
    APP_LOG_ERROR(APP_NAME, "mx lwip deinit failed (%d)\n", err);
  }

  /* DHCP specific configuration */
  BaseType_t xResult;
  xResult = xTimerDelete(AppLwipDhcpTimerHandle, 0);
  if (xResult != pdPASS)
  {
    APP_LOG_ERROR(APP_NAME, "DHCP Timer deletion failed (%ld)\n", xResult);
  }
  /* End of DHCP specific configuration */

}

app_status_t app_lwip_dns_resolve(const char *name, ip_addr_t *ipaddr, uint32_t timeout_ms)
{
  BaseType_t xResult;
  int32_t ret;
  app_status_t status = EXEC_STATUS_OK;
  app_lwip_dns_arg_t dns_arg;
  char ip_str[INET_ADDRSTRLEN];

  dns_arg.task_handle = xTaskGetCurrentTaskHandle();

  APP_LOG_INFO(APP_NAME, "Start DNS resolution for: %s (timeout %" PRId32 " ms)\n", name, timeout_ms);
  ret = dns_gethostbyname(name, ipaddr, AppLwipDnsFoundCallback, &dns_arg);
  if (ret == ERR_OK)
  {
    /* Already in DNS cache table*/
  }
  else if (ret == ERR_INPROGRESS)
  {
    /* Wait for DNS resolution  */
    xResult = ulTaskNotifyTakeIndexed(EVT_DNS_FOUND, pdTRUE, pdMS_TO_TICKS(timeout_ms));
    if (xResult == 0)
    {
      APP_LOG_ERROR(APP_NAME, "DNS resolution timeout\n");
      status = EXEC_STATUS_ERROR;
    }
    else
    {
      status = dns_arg.status;
      *ipaddr = dns_arg.ipaddr;
    }
  }
  else
  {
    APP_LOG_ERROR(APP_NAME, "DNS resolution error %" PRId32 "\n", ret);
    status = EXEC_STATUS_ERROR;
  }

  if (status == EXEC_STATUS_OK)
  {
    inet_ntop(AF_INET, ipaddr, ip_str, INET_ADDRSTRLEN);
    APP_LOG_INFO(APP_NAME, "DNS resolution done: %s -> %s\n", name, ip_str);
  }

  return status;
}
