/**
  ******************************************************************************
  * File Name          : app_tcp_echo_client.c
  * Description        : This file provides code for TCP echo client example.
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
#include "app_tcp_echo_client.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_lwip.h"
#include "app_logger.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"

#include "app_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_NAME "AppTcpEchoClient"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile BaseType_t RunClient = pdTRUE;

static TaskHandle_t AppTcpEchoClientTaskHandle;

static uint8_t RecvBuffer[APP_TCP_ECHO_CLIENT_BUF_LEN];
static uint8_t SendBuffer[APP_TCP_ECHO_CLIENT_BUF_LEN];

extern EventGroupHandle_t AppLwipEvents;

/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  TCP echo client thread
  * param: arg: pointer on argument(not used here)
  * retval: None
  */
static void AppTcpEchoClientThread(void *args);

/**
  * brief: connect to remote server
  *
  * param:[out] pSock pointer socket descriptor
  * param: remote_server_ip IP address of remote server
  * param: remote_server_port IP port of remote server
  * param: recv_timeout_s timeout in milliseconds to receive the data
  * param: send_timeout_s timeout in milliseconds to send the data
  * retval: app_status_t
  */
static app_status_t AppConnect(int32_t *pSock, const char *remote_server_ip, const uint16_t remote_server_port,
                               uint32_t recv_timeout_ms, uint32_t send_timeout_ms);

/**
  * brief: send data to remote server
  *
  * param: sock socket descriptor
  * param: buffer buffer containing data to send
  * param: buffer_len size of the buffer
  * retval: app_status_t
  */
static app_status_t AppSend(int32_t sock, const uint8_t *buffer, uint32_t buffer_len);

/**
  * brief: receive data from remote server
  *
  * param: sock socket descriptor
  * param: buffer buffer to store data to receive
  * param: buffer_len size of the buffer and size to receive
  * retval: app_status_t
  */
static app_status_t AppRecv(int32_t sock, uint8_t *buffer, uint32_t buffer_len);
/**
  * brief: disconnect from remote server
  *
  * param: sock socket descriptor
  */
static void AppDisconnect(int32_t sock);

static void AppFillBuffer(uint8_t *buffer, uint32_t size, int32_t number);

static app_status_t AppCompareBuffers(uint8_t *buffer_1, uint32_t size_1, uint8_t *buffer_2, uint32_t size_2);

/* Private functions ---------------------------------------------------------*/

static app_status_t AppConnect(int32_t *pSock, const char *remote_server_ip, const uint16_t remote_server_port,
                               uint32_t recv_timeout_ms, uint32_t send_timeout_ms)
{
  app_status_t status = EXEC_STATUS_OK;
  int32_t ret;
  int32_t sock;
  struct sockaddr_in server_addr;
  ip_addr_t remote_addr;
  char remote_ip_str[INET_ADDRSTRLEN];

  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
  {
    APP_LOG_ERROR(APP_NAME, "Socket creation (err %"PRId32", errno %d)\n", sock, errno);
    status = EXEC_STATUS_ERROR;
  }
  else
  {
    *pSock = sock;
  }

  ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &recv_timeout_ms, sizeof(recv_timeout_ms));
  if (ret < 0)
  {
    APP_LOG_ERROR(APP_NAME, "Set send timeout failed (err %"PRId32", errno %d)\n", ret, errno);
    status = EXEC_STATUS_ERROR;
  }

  ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &send_timeout_ms, sizeof(send_timeout_ms));
  if (ret < 0)
  {
    APP_LOG_ERROR(APP_NAME, "Set receive timeout failed (err %"PRId32", errno %d)\n", ret, errno);
    status = EXEC_STATUS_ERROR;
  }

  if (status == EXEC_STATUS_OK)
  {
    status = app_lwip_dns_resolve(remote_server_ip, &remote_addr, APP_LWIP_DNS_RESOLUTION_TIMEOUT_MS);
    if (status != EXEC_STATUS_OK)
    {
      APP_LOG_ERROR(APP_NAME, "DNS resolution error %d\n", status);
    }
  }

  if (status == EXEC_STATUS_OK)
  {
    /* Prepare the sockaddr_in structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = remote_addr.addr;
    server_addr.sin_port = htons(remote_server_port);

    /* Initiate the connection */
    inet_ntop(AF_INET, &remote_addr, remote_ip_str, INET_ADDRSTRLEN);
    APP_LOG_INFO(APP_NAME, "Connecting to %s:%d\n", remote_ip_str, remote_server_port);
    ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
      APP_LOG_ERROR(APP_NAME, "Socket connection (ret %"PRId32", errno %d)\n", ret, errno);
      close(sock);
      status = EXEC_STATUS_ERROR;
    }
    else
    {
      APP_LOG_INFO(APP_NAME, "Connected  to %s:%d\n", remote_server_ip, remote_server_port);
    }
  }

  return status;
}

static app_status_t AppSend(int32_t sock, const uint8_t *buffer, uint32_t buffer_len)
{
  app_status_t status = EXEC_STATUS_OK;
  int32_t sent_len = 0U;
  int32_t len;

  while (sent_len < buffer_len)
  {
    len = send(sock, buffer + sent_len, buffer_len - sent_len, 0);
    if (len < 0)
    {
      if (errno == EAGAIN)
      {
        APP_LOG_ERROR(APP_NAME, "Send timeout reached\n");
      }
      else
      {
        APP_LOG_ERROR(APP_NAME, "Send error (len %"PRId32", errno %d)\n", len, errno);
      }
      status = EXEC_STATUS_ERROR;
      break;
    }
    else
    {
      sent_len += len;
    }
  }

  return status;
}

static app_status_t AppRecv(int32_t sock, uint8_t *buffer, uint32_t buffer_len)
{
  app_status_t status = EXEC_STATUS_OK;
  int32_t recv_len = 0;
  int32_t len;

  while (recv_len < buffer_len)
  {
    len = recv(sock, buffer + recv_len, buffer_len - recv_len, 0);
    if (len < 0)
    {
      if (errno == EAGAIN)
      {
        APP_LOG_ERROR(APP_NAME, "Receive timeout reached\n");
      }
      else
      {
        APP_LOG_ERROR(APP_NAME, "Receive error (errno %d)\n", errno);
      }
      status = EXEC_STATUS_ERROR;
      break;
    }
    else
    {
      recv_len += len;
    }
  }

  return status;
}

static void AppDisconnect(int32_t sock)
{
  close(sock);
}

static void AppFillBuffer(uint8_t *buffer, uint32_t size, int32_t number)
{

  /* Terminate with Line Feed (LF).
   * some TCP echo servers, like tcpbin.com echo messages only when LF terminated
   *
   * +--+--+--+--+---------------+------+------+------+------+------+
   * | 0| 1| 2| 3|  ............ |size-5|size-4|size-3|size-2|size-1|
   * +--+--+--+--+---------------+------+------+------+------+------+
   * |   number  |  ............ |           number          |  LF  |
   * +-------------------------------------------------------+------+
   */

  int32_t trailer_index = size - sizeof(number) - sizeof(uint8_t);

  /* Fill header */
  memcpy(buffer, &number, sizeof(number));

  /* Fill trailer */
  memcpy(buffer + trailer_index, &number, sizeof(number));
  buffer[size - 1] = '\n';
}

static app_status_t AppCompareBuffers(uint8_t *buffer_1, uint32_t size_1, uint8_t *buffer_2, uint32_t size_2)
{
  app_status_t status = EXEC_STATUS_ERROR;
  int32_t trailer_index;
  int32_t trailer_size;

  if (size_1 == size_2)
  {
    if (memcmp(buffer_1, buffer_2, sizeof(int32_t)) == 0)
    {
      trailer_size = sizeof(int32_t) + sizeof(uint8_t);
      trailer_index = size_1 - trailer_size;
      if (memcmp(buffer_1 + trailer_index, buffer_2 + trailer_index, trailer_size) == 0)
      {
        status = EXEC_STATUS_OK;
      }
    }
  }
  return status;
}

static void AppTcpEchoClientThread(void *args)
{
  int32_t sock = -1;
  int32_t i;
  app_status_t echo_status = EXEC_STATUS_OK;

  APP_LOG_INFO(APP_NAME, "Waiting for IP address\n");
  xEventGroupWaitBits(AppLwipEvents,
                      EVT_IP_ADDR_SET,
                      pdTRUE,
                      pdFALSE,
                      portMAX_DELAY);

  while (RunClient == pdTRUE)
  {
    /** ########## Step 5.2 ##########
      * Sets up a TCP socket and connect to the server
      */
    echo_status = AppConnect(&sock, APP_TCP_ECHO_REMOTE_SERVER_ADDR, APP_TCP_ECHO_REMOTE_SERVER_PORT,
                             APP_TCP_CLIENT_ECHO_RECV_TIMEOUT_MS, APP_TCP_CLIENT_ECHO_SEND_TIMEOUT_MS);

    if (echo_status == EXEC_STATUS_OK)
    {
      for (i = 0 ; i < APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION ; i++)
      {
        AppFillBuffer(SendBuffer, sizeof(SendBuffer), i);

        /** ########## Step 5.3 ##########
          * Sends the data to the TCP echo server
          */
        APP_LOG_INFO(APP_NAME, "Echo start [%"PRId32"/%d] (Size %u bytes)\n", i + 1,
                     APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION, sizeof(SendBuffer));

#if defined(USE_TRACE) && USE_TRACE != 0
        TickType_t tick_start = xTaskGetTickCount();
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

        echo_status = AppSend(sock, SendBuffer, sizeof(SendBuffer));

        if (echo_status == EXEC_STATUS_OK)
        {
          echo_status = AppRecv(sock, RecvBuffer, sizeof(RecvBuffer));
        }

        if (echo_status == EXEC_STATUS_OK)
        {
          echo_status = AppCompareBuffers(RecvBuffer, sizeof(RecvBuffer), SendBuffer, sizeof(SendBuffer));
        }

        if (echo_status == EXEC_STATUS_OK)
        {
          APP_LOG_INFO(APP_NAME, "Echo completed [%"PRId32"/%d] (Size %u bytes, Round Trip Time %lu ms)\n",
                       i + 1, APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION, sizeof(RecvBuffer),
                       pdTICKS_TO_MS(xTaskGetTickCount() - tick_start));
        }
        else
        {
          APP_LOG_ERROR(APP_NAME, "Echo error [%"PRId32"/%d]\n", i + 1, APP_TCP_CLIENT_ECHO_NUMBER_BY_CONNECTION);
          break;
        }

      }

      AppDisconnect(sock);
      APP_LOG_INFO(APP_NAME, "Disconnected\n");
    }

    APP_LOG_INFO(APP_NAME, "TCP client connection attempt in %d ms\n", APP_TCP_CLIENT_CONNECT_DELAY_MS);
    vTaskDelay(pdMS_TO_TICKS(APP_TCP_CLIENT_CONNECT_DELAY_MS));

  }

  /* Inspect task stack high water mark on exit. */
  APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%ld)\n", uxTaskGetStackHighWaterMark(NULL));

  /* Delete the task */
  APP_LOG_INFO(APP_NAME, "Delete Task\n");
  vTaskDelete(NULL);
}

/* Public functions ----------------------------------------------------------*/

void app_tcp_echo_client_start()
{
  /** ########## Step 5.1 ##########
    * Creates and starts the TCP echo client thread
    */
  BaseType_t xReturn;
  xReturn = xTaskCreate(AppTcpEchoClientThread, APP_NAME, configMINIMAL_STACK_SIZE,
                        (void *) NULL, APP_TCP_ECHO_CLIENT_THREAD_PRIO, &AppTcpEchoClientTaskHandle);
  if (xReturn != pdPASS)
  {
    APP_LOG_ERROR(APP_NAME, "Task Creation failed (%ld)\n", xReturn);
  }
}
