/**
  ******************************************************************************
  * File Name          : app_sntp_client.h
  * Description        : This file provides the interface of the SNTP client
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
#ifndef APP_SNTP_CLIENT_H
#define APP_SNTP_CLIENT_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "arch.h"
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief:  SNTP client start function
  * param:  none
  * retval: None
  */
void app_sntp_client_start(void);

void app_sntp_set_system_time(u32_t sec, u32_t usec);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_SNTP_CLIENT_H */
