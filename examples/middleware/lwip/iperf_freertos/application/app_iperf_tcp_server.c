/**
  ******************************************************************************
  * File Name          : app_iperf_tcp_server.c
  * Description        : This file provides code for iPerf TCP server example.
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
#include "app_iperf_tcp_server.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_lwip.h"
#include "app_logger.h"

#include "app_config.h"

#include "lwip/apps/lwiperf.h"
#include "lwip/netif.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_NAME "AppIperfTcpServer"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static volatile BaseType_t RunServer = pdTRUE;

static TaskHandle_t AppIperfTcpServerTaskHandle;

extern EventGroupHandle_t AppLwipEvents;

/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  iPerf TCP server thread
  * param: arg: pointer on argument(not used here)
  * retval: None
  */
static void AppIperfTcpServerThread(void *args);

/**
  * brief: iPerf TCP server report callback function
  */
static void AppIperfTcpServerReportFn(void *arg,
                                      enum lwiperf_report_type report_type,
                                      const ip_addr_t *local_addr,
                                      u16_t local_port,
                                      const ip_addr_t *remote_addr,
                                      u16_t remote_port,
                                      u32_t bytes_transferred,
                                      u32_t ms_duration,
                                      u32_t bandwidth_kbitpsec);

/* Private functions ---------------------------------------------------------*/

static void AppIperfTcpServerThread(void *args)
{
  APP_LOG_INFO(APP_NAME, "Waiting for IP address\n");

  void *iperf_session = NULL;

  while (RunServer == pdTRUE)
  {
    xEventGroupWaitBits(AppLwipEvents,
                        EVT_IP_ADDR_SET,
                        pdTRUE,
                        pdFALSE,
                        portMAX_DELAY);

    /** ########## Step 4.2 ##########
      * Start the iPerf TCP server
      */
    iperf_session = lwiperf_start_tcp_server_default(AppIperfTcpServerReportFn, NULL);
    APP_LOG_INFO(APP_NAME, "Started iPerf TCP server on %s:%d...\n", ip4addr_ntoa(netif_ip4_addr(netif_default)),
                 LWIPERF_TCP_PORT_DEFAULT);
  }

  /** ########## Step 4.3 ##########
    * Stop the iPerf TCP server
    */
  lwiperf_abort(iperf_session);

  /* Inspect task stack high water mark on exit. */
  APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%lu)\n", uxTaskGetStackHighWaterMark(NULL));

  /* Delete the task */
  APP_LOG_INFO(APP_NAME, "Delete Task\n");
  vTaskDelete(NULL);
}

static void AppIperfTcpServerReportFn(void *arg,
                                      enum lwiperf_report_type report_type,
                                      const ip_addr_t *local_addr,
                                      u16_t local_port,
                                      const ip_addr_t *remote_addr,
                                      u16_t remote_port,
                                      u32_t bytes_transferred,
                                      u32_t ms_duration,
                                      u32_t bandwidth_kbitpsec)
{
  APP_LOG_INFO(APP_NAME, "iPerf TCP Server report:\n");
#if LWIP_IPV4
  char ip_str[16]; /* Enough for IPv4 */
  ipaddr_ntoa_r(local_addr, ip_str, sizeof(ip_str));
  APP_LOG_INFO(APP_NAME, "- Local Server : %s:%d\n",
               ip_str,
               local_port);
  ipaddr_ntoa_r(remote_addr, ip_str, sizeof(ip_str));
  APP_LOG_INFO(APP_NAME, "- Remote Client : %s:%d\n",
               ip_str,
               remote_port);
#endif /*LWIP_IPV4*/
  APP_LOG_INFO(APP_NAME, "- Bytes transferred: %"PRIu32"\n", bytes_transferred);
  APP_LOG_INFO(APP_NAME, "- Duration: %"PRIu32" ms\n", ms_duration);
  APP_LOG_INFO(APP_NAME, "- Bandwidth: %"PRIu32" kbits/sec\n", bandwidth_kbitpsec);
}

/* Public functions ----------------------------------------------------------*/

void app_iperf_tcp_server_start()
{
  /** ########## Step 4.1 ##########
    * Creates and starts the iPerf TCP server thread
    */
  BaseType_t xReturn;
  xReturn = xTaskCreate(AppIperfTcpServerThread, APP_NAME, configMINIMAL_STACK_SIZE,
                        (void *) NULL, APP_IPERF_TCP_SERVER_THREAD_PRIO, &AppIperfTcpServerTaskHandle);
  if (xReturn != pdPASS)
  {
    APP_LOG_ERROR(APP_NAME, "Task Creation failed (%ld)\n", xReturn);
  }
}
