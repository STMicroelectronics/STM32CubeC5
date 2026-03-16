/**
  ******************************************************************************
  * File Name          : app_config.h
  * Description        : This file provides the configuration file for the
  *                      application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* DHCP Timeout in milliseconds before assigning static IP address */
#ifndef APP_LWIP_DHCP_TIMEOUT_MS
#define APP_LWIP_DHCP_TIMEOUT_MS                   20000
#endif /*APP_LWIP_DHCP_TIMEOUT*/

/* mDNS hostname to overwrite default one */
#ifndef APP_LWIP_MDNS_HOSTNAME
#define APP_LWIP_MDNS_HOSTNAME                    ""
#endif /*APP_LWIP_MDNS_HOSTNAME*/

/* Manual IP address to set in case of DHCP timeout */
#ifndef APP_LWIP_MANUAL_IP_ADDR
#define APP_LWIP_MANUAL_IP_ADDR                    "0.0.0.0"
#endif /*APP_LWIP_MANUAL_IP_ADDR*/

/* Manual netmask to set in case of DHCP timeout */
#ifndef APP_LWIP_MANUAL_NETMASK
#define APP_LWIP_MANUAL_NETMASK                    "255.255.255.0"
#endif /*APP_LWIP_MANUAL_NETMASK*/

/* Manual gateway address to set in case of DHCP timeout */
#ifndef APP_LWIP_MANUAL_GW_ADDR
#define APP_LWIP_MANUAL_GW_ADDR                    "0.0.0.0"
#endif /*APP_LWIP_MANUAL_GW_ADDR*/

/* DNS server address to set in case of DHCP timeout */
#ifndef APP_LWIP_MANUAL_DNS_ADDR
#define APP_LWIP_MANUAL_DNS_ADDR                   "0.0.0.0"
#endif /*APP_LWIP_MANUAL_DNS_ADDR*/

/* DNS Resolution timeout in milliseconds */
#ifndef APP_LWIP_DNS_RESOLUTION_TIMEOUT_MS
#define APP_LWIP_DNS_RESOLUTION_TIMEOUT_MS         10000
#endif /*APP_LWIP_DNS_RESOLUTION_TIMEOUT_MS*/

/* Delay in millisecond between two current time log */
#ifndef APP_SNTP_CLIENT_SYSTEM_TIME_LOG_PERIOD_MS
#define APP_SNTP_CLIENT_SYSTEM_TIME_LOG_PERIOD_MS  10000
#endif /*APP_SNTP_CLIENT_SYSTEM_TIME_LOG_PERIOD_MS*/

/* SNTP server address */
#ifndef APP_SNTP_SERVER_ADDR
#define APP_SNTP_SERVER_ADDR                       "pool.ntp.org"
#endif /*APP_SNTP_SERVER_ADDR*/

/* Threads priorities*/
#define APP_LWIP_START_THREAD_PRIO                 24
#define APP_SNTP_CLIENT_THREAD_PRIO                24

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_CONFIG_H */
