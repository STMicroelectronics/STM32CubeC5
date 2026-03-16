/**
  ******************************************************************************
  * File Name          : app_lwip.h
  * Description        : This file provides the interface of the TCP echo LwIP
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
#ifndef APP_LWIP_H
#define APP_LWIP_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lwip/dns.h"
#include "example.h"
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Event group bit definitions */
/* Event when IP Address is set */
#define EVT_IP_ADDR_SET              1

/**
  * brief:  LwIP init function
  * param:  none
  * retval: None
  */
void app_lwip_init(void);

/**
  * brief:  LwIP de-init function
  * param:  none
  * retval: None
  */
void app_lwip_deinit(void);

app_status_t app_lwip_dns_resolve(const char *name, ip_addr_t *ipaddr, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_LWIP_H */
