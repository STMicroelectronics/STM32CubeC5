/**
  ******************************************************************************
  * @file           : mx_lwip.h
  * @brief          : LwIP STM32 ethernet interface
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

#ifndef MX_LWIP_H
#define MX_LWIP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lwip/err.h"
#include "lwip/netif.h"
#include "stm32_hal.h"
#include "eth_interface_freertos.h"
#include "FreeRTOS.h"
#include "queue.h"

#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define MX_LWIP_MAX_INTERFACE_NB                10   /**< Maximum number of network interfaces. */
#define MX_LWIP_DATA_WORKER_QUEUE_SIZE          20   /**< Size of the message queue. */
#define MX_LWIP_DATA_WORKER_QUEUE_TX_TIMEOUT_MS 1000 /**< Timeout for low-level output in the message queue. */
#define MX_LWIP_DATA_WORKER_STACK_SIZE          2048 /**< Stack size (in bytes) for the data worker thread. */
#define MX_LWIP_DATA_WORKER_PRIORITY            25   /**< Data Worker Thread priority. */
#define MX_LWIP_LINK_MONITOR_PERIOD_MS          100  /**< Link monitoring period in milliseconds. */
#define MX_LWIP_LINK_MONITOR_STACK_SIZE         512  /**< Stack size (in bytes) for the link monitor thread. */
#define MX_LWIP_LINK_MONITOR_PRIORITY           25   /**< Link monitoring Thread priority. */
#define MX_LWIP_MAX_ETH_TX_BUFFERS              5    /**< Maximum number of Ethernet TX buffers. */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the LwIP stack and Ethernet interface.
 * @return ERR_OK on success, error code otherwise.
 */
err_t mx_lwip_init(void);


/**
 * @brief  De-initialize the LwIP stack and Ethernet interface.
 * @return ERR_OK on success, error code otherwise.
 */
err_t mx_lwip_deinit(void);


/**
 * @brief  Get the context structure for Ethernet interface 1.
 * @return Pointer to lwip_eth_interface_netif_context_t for interface 1.
 */
lwip_eth_interface_netif_context_t *mx_lwip_get_interface_context_1(void);


/**
 * @brief  Initialize Ethernet interface 1 and configure the provided netif.
 * @param  p_netif Pointer to the LwIP network interface structure to initialize.
 * @return ERR_OK on success, error code otherwise.
 */
err_t mx_lwip_init_interface_1(struct netif *p_netif);


/**
 * @brief  De-initialize Ethernet interface 1 and clear its context.
 * @param  p_netif Pointer to the LwIP network interface structure to de-initialize.
 */
void mx_lwip_deinit_interface_1(struct netif *p_netif);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LWIP_H */
