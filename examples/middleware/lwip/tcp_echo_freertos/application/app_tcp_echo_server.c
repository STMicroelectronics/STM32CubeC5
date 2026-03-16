/**
  ******************************************************************************
  * File Name          : app_tcp_echo_server.c
  * Description        : This file provides code for TCP echo server example.
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
#include "app_tcp_echo_server.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_logger.h"
#include "app_lwip.h"

#include "lwip/sockets.h"

#include "app_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define APP_NAME "AppTcpEchoServer"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile BaseType_t RunServer = pdTRUE;

static TaskHandle_t AppTcpEchoServerTaskHandle;

static uint8_t Buffer[APP_TCP_ECHO_SERVER_BUF_LEN];

extern EventGroupHandle_t AppLwipEvents;

/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  TCP echo server thread
  * param: arg: pointer on argument(not used here)
  *
  * retval: None
  */
static void AppTcpEchoServerThread(void *args);

/* Private functions ---------------------------------------------------------*/

static void AppTcpEchoServerThread(void *args)
{
  ssize_t tot_recv_len;
  ssize_t number_of_echo;
  ssize_t recv_len;
  ssize_t sent_len;
  int32_t socket_desc;
  int32_t sock;
  int32_t ret;
  uint32_t timeout;
  fd_set read_fds;
  struct sockaddr_in server_sa;
  struct sockaddr_in client_sa;
  struct timeval select_timeout;
  char ip4[INET_ADDRSTRLEN];
  socklen_t client_sl = sizeof(client_sa);
  app_status_t echo_status = EXEC_STATUS_OK;

  /* Wait for IP address */
  APP_LOG_INFO(APP_NAME, "Waiting for IP address\n");
  xEventGroupWaitBits(AppLwipEvents,
                      EVT_IP_ADDR_SET,
                      pdTRUE,
                      pdFALSE,
                      portMAX_DELAY);

  /** ########## Step 4.2 ##########
    * Sets up a TCP socket and wait for incoming connection.
  */

  /* create a TCP socket */
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
  {
    APP_LOG_ERROR(APP_NAME, "Socket creation failed (err %" PRId32 ", errno %d)\n", sock, errno);
    RunServer = pdFALSE;
  }
  else
  {
    /* bind to port at any address */
    server_sa.sin_family = AF_INET;
    server_sa.sin_port = htons(APP_TCP_ECHO_SERVER_LISTEN_PORT);
    server_sa.sin_addr.s_addr = INADDR_ANY;

    ret = bind(sock, (struct sockaddr *)&server_sa, sizeof(server_sa));
    if (ret < 0)
    {
      APP_LOG_ERROR(APP_NAME, "Bind socket (err %"PRId32", errno %d)\n", ret, errno);
      RunServer = pdFALSE;
    }
    else
    {
      /* listen for incoming socket */
      listen(sock, TCP_LISTEN_BACKLOG);
    }
  }

  while (RunServer == pdTRUE)
  {
    tot_recv_len = 0;
    number_of_echo = 0;

    APP_LOG_INFO(APP_NAME, "Waiting for connection on %s:%u\n", ip4addr_ntoa(netif_ip4_addr(netif_default)),
                 APP_TCP_ECHO_SERVER_LISTEN_PORT);

    /* Initialize the file descriptor set */
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);

    /* Set the select timeout value */
    select_timeout.tv_sec = APP_TCP_ECHO_SERVER_SELECT_TIMEOUT_S;
    select_timeout.tv_usec = 0;
    echo_status = EXEC_STATUS_OK;

    /* Use select to wait for incoming connections */
    ret = select(sock + 1, &read_fds, NULL, NULL, &select_timeout);
    if (ret < 0)
    {
      APP_LOG_ERROR(APP_NAME, "Socket select (ret = %"PRId32")\n", ret);
      close(sock);
      continue;
    }
    else if (ret == 0)
    {
      /* Timeout occurred, no incoming connections */
      APP_LOG_INFO(APP_NAME, "Select timeout\n");
      continue;
    }
    socket_desc = accept(sock, (struct sockaddr *)&client_sa, &client_sl);


    /** ########## Step 4.3 ##########
      * Processes incoming socket connection by getting the received data
      * and sending them back to the client
      */
    if (socket_desc >= 0)
    {

      inet_ntop(AF_INET, &(client_sa.sin_addr), ip4, INET_ADDRSTRLEN);

      APP_LOG_INFO(APP_NAME, "Echo start from %s (sock %"PRId32", socket_desc %"PRId32")\n", ip4, sock, socket_desc);
#if defined(USE_TRACE) && USE_TRACE != 0
      TickType_t tick_start = xTaskGetTickCount();
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

      timeout = APP_TCP_SERVER_SEND_TIMEOUT_MS;

      ret = setsockopt(socket_desc, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
      if (ret < 0)
      {
        APP_LOG_ERROR(APP_NAME, "Set send timeout failed (err %"PRId32", errno %d)\n", ret, errno);
        echo_status = EXEC_STATUS_ERROR;
      }

      timeout = APP_TCP_SERVER_RECV_TIMEOUT_MS;

      ret = setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
      if (ret < 0)
      {
        APP_LOG_ERROR(APP_NAME, "Set receive timeout failed (err %"PRId32", errno %d)\n", ret, errno);
        echo_status = EXEC_STATUS_ERROR;
      }

      /* Receive data from the new client connection */
      while (echo_status == EXEC_STATUS_OK)
      {
        recv_len = recv(socket_desc, Buffer, sizeof(Buffer), 0);
        if (recv_len > 0)
        {
          tot_recv_len += recv_len;
          number_of_echo++;

          sent_len = send(socket_desc, (const unsigned char *)(Buffer), recv_len, 0);
          if (sent_len <= 0)
          {
            /* Send error, close this connection and accept the next one */
            echo_status = EXEC_STATUS_ERROR;
            APP_LOG_ERROR(APP_NAME, "Send error %d\n", sent_len);
          }
        }
        else if (recv_len < 0)
        {
          if (errno == EAGAIN)
          {
            echo_status = EXEC_STATUS_ERROR;
            APP_LOG_ERROR(APP_NAME, "Receive timeout reached\n");
          }
          else if (errno == ENOTCONN)
          {
            /* The connection has been closed by the client, exit the loop on success */
            break;
          }
          else
          {
            /* Receive error, close this connection and accept the next one */
            echo_status = EXEC_STATUS_ERROR;
            APP_LOG_ERROR(APP_NAME, "Receive error (errno %d)\n", errno);
          }
        }
      }
      if (echo_status == EXEC_STATUS_OK)
      {
        APP_LOG_INFO(APP_NAME, "Echo done (Size %u bytes, Time to echo %lu ms, Number of echo %d)\n",
                     tot_recv_len, pdTICKS_TO_MS(xTaskGetTickCount() - tick_start), number_of_echo);
      }
      else
      {
        APP_LOG_INFO(APP_NAME, "Echo error\n");
      }
    }
    else
    {
      APP_LOG_ERROR(APP_NAME, "Accept error %"PRId32"\n", socket_desc);
    }

    /** ########## Step 4.4 ##########
      * Close the socket
      */
    close(socket_desc);
    APP_LOG_INFO(APP_NAME, "Connection closed\n");
  }

  /* Inspect task stack high water mark on exit. */
  APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%ld)\n", uxTaskGetStackHighWaterMark(NULL));

  /* Delete the task */
  APP_LOG_INFO(APP_NAME, "Delete Task\n");
  vTaskDelete(NULL);
}

/* Public functions ----------------------------------------------------------*/

/**
  * brief:  Start the TCP echo server (start its thread)
  * param:  none
  * retval: None
  */
void app_tcp_echo_server_start()
{
  BaseType_t xReturn;
  xReturn = xTaskCreate(AppTcpEchoServerThread, APP_NAME, configMINIMAL_STACK_SIZE,
                        (void *) NULL, APP_TCP_ECHO_SERVER_THREAD_PRIO, &AppTcpEchoServerTaskHandle);
  if (xReturn != pdPASS)
  {
    APP_LOG_INFO(APP_NAME, "Task Creation failed (%ld)\n", xReturn);
  }
}
