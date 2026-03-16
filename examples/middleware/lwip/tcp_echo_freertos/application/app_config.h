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

/* Number of byte to send to the echo server 
   Set by default to TCP_WND to fill server TCP window*/
#ifndef APP_TCP_ECHO_CLIENT_BUF_LEN
#define APP_TCP_ECHO_CLIENT_BUF_LEN                (TCP_WND)
#endif /*APP_TCP_ECHO_CLIENT_BUF_LEN*/

/* TCP Echo Remote Server Address the client will connect to.
   It can be set to an IP address */
#ifndef APP_TCP_ECHO_REMOTE_SERVER_ADDR
#define APP_TCP_ECHO_REMOTE_SERVER_ADDR            "0.0.0.0"
#endif /*APP_TCP_ECHO_REMOTE_SERVER_ADDR*/

/* TCP Echo Remote Server Port the client will connect to */
#ifndef APP_TCP_ECHO_REMOTE_SERVER_PORT
#define APP_TCP_ECHO_REMOTE_SERVER_PORT            7
#endif /*APP_TCP_ECHO_REMOTE_SERVER_PORT*/

/* Number of TCP frames to send to the echo server during the same connection */
#ifndef APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION
#define APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION   5
#endif /*APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION*/

/* Delay in millisecond between two TCP connections to the echo server */
#ifndef APP_TCP_CLIENT_CONNECT_DELAY_MS
#define APP_TCP_CLIENT_CONNECT_DELAY_MS            10000
#endif /*APP_TCP_CLIENT_CONNECT_DELAY_MS*/

/* Timeout in second to connect to remote server */
#ifndef APP_TCP_CLIENT_ECHO_CONNECTION_TIMEOUT_S
#define APP_TCP_CLIENT_ECHO_CONNECTION_TIMEOUT_S   5
#endif /*APP_TCP_CLIENT_ECHO_CONNECTION_TIMEOUT_S*/

/* Timeout in milliseconds to send data to the echo server */
#ifndef APP_TCP_CLIENT_ECHO_SEND_TIMEOUT_MS
#define APP_TCP_CLIENT_ECHO_SEND_TIMEOUT_MS        10000U
#endif /*APP_TCP_CLIENT_ECHO_SEND_TIMEOUT_MS*/

/* Timeout in milliseconds to receive data to the echo server */
#ifndef APP_TCP_CLIENT_ECHO_RECV_TIMEOUT_MS
#define APP_TCP_CLIENT_ECHO_RECV_TIMEOUT_MS        10000U
#endif /*APP_TCP_CLIENT_ECHO_RECV_TIMEOUT_MS*/

/* Threads priorities*/
#define APP_LWIP_START_THREAD_PRIO                 24
#define APP_TCP_ECHO_CLIENT_THREAD_PRIO            24
#define APP_TCP_ECHO_SERVER_THREAD_PRIO            24

/* Server reception buffer length.
   Set by default to TCP window size to minimize TCP/IP receive and send calls */
#ifndef APP_TCP_ECHO_SERVER_BUF_LEN
#define APP_TCP_ECHO_SERVER_BUF_LEN                TCP_WND
#endif /*APP_TCP_ECHO_SERVER_BUF_LEN*/

/* TCP Echo Server IP Port the server will listen to */
#ifndef APP_TCP_ECHO_SERVER_LISTEN_PORT
#define APP_TCP_ECHO_SERVER_LISTEN_PORT            7
#endif /*APP_TCP_ECHO_SERVER_LISTEN_PORT*/

/* Timeout in milliseconds to receive TCP frames from the client */
#ifndef APP_TCP_SERVER_RECV_TIMEOUT_MS
#define APP_TCP_SERVER_RECV_TIMEOUT_MS             10000U
#endif /*APP_TCP_SERVER_RECV_TIMEOUT_MS*/

/* Timeout in milliseconds to send TCP frames back to the client */
#ifndef APP_TCP_SERVER_SEND_TIMEOUT_MS
#define APP_TCP_SERVER_SEND_TIMEOUT_MS             10000U
#endif /*APP_TCP_SERVER_SEND_TIMEOUT_MS*/

/* Timeout in second for incoming connection */
#ifndef APP_TCP_ECHO_SERVER_SELECT_TIMEOUT_S
#define APP_TCP_ECHO_SERVER_SELECT_TIMEOUT_S       20
#endif /*APP_TCP_ECHO_SERVER_SELECT_TIMEOUT_S*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_CONFIG_H */
