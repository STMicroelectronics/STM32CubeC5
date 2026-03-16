/**
  ******************************************************************************
  * File Name          : app_tcp_echo_client.h
  * Description        : This file provides the interface of the TCP echo client
  *                      example.
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
#ifndef APP_TCP_ECHO_SERVER_H
#define APP_TCP_ECHO_SERVER_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief:  TCP echo server start function
  * param:  none
  * retval: None
  */
void app_tcp_echo_server_start(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_TCP_ECHO_SERVER_H */
