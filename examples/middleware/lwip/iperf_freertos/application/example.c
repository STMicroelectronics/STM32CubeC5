/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
  ******************************************************************************
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
#include "example.h"

#include "app_logger.h"
#include "app_lwip.h"
#include "app_iperf_tcp_server.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  app_logger_init();

  /** ########## Step 3 ##########
    * Initializes the LwIP stack
    */
  app_lwip_init();

  /** ########## Step 4 ##########
    * Starts the iPerf TCP server thread
    */
  app_iperf_tcp_server_start();

  return return_status;
}


app_status_t app_deinit(void)
{
  /** This API is not used in this example (infinite loop).
    * It is optimized out by the toolchain.
    */
  app_lwip_deinit();
  return EXEC_STATUS_ERROR;
}
