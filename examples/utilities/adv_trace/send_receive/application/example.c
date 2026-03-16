/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body (send receive with adv_trace utility API)
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define T_REG_OFF                      (0)     /*!< Log without bitmask */
#define MX_VERBOSE_LEVEL_MINIMUM       (0)     /*!< Verbose level 0 */
#define ADV_TRACE_PERIOD_IT            (100)   /*!< period of trace Tx in interrupt context */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

volatile uint32_t counterIt = 0;
volatile uint32_t CounterOverFlow = 0 ;

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the adv_trace utility.
  */
app_status_t app_init(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_adv_trace_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Application use case of trace logging for data transmit
  */
app_status_t app_process(void)
{
  if (ADV_TRACE_Send((const uint8_t *)"msg TX every 50ms\r\n", 19) != ADV_TRACE_OK)
  {
    /* Increment CounterOverFlow in case of detection of overflow at trace evacuation */
    CounterOverFlow++;
  }

  return EXEC_STATUS_OK;
} /* end app_process */


app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (ADV_TRACE_DeInit() == ADV_TRACE_OK)
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_deinit */


/**
  * This function handles SysTick Handler to send message every 100ms and increment variable counterIt.
  * and variable CounterOverFlow in case of trace overFlow.
  */
void HAL_CORTEX_SYSTICK_Callback(void)
{
  if ((HAL_GetTick() % ADV_TRACE_PERIOD_IT) == 0)
  {

    /* Send "log text" every 100 ms */
    if (ADV_TRACE_COND_FSend(MX_VERBOSE_LEVEL_MINIMUM, T_REG_OFF, MX_TS_OFF,
                             "Counter value in IT 100ms : %-d\r\n", counterIt) == ADV_TRACE_OK)
    {
      /* Increment CounterIT to count the number of messages sent */
      counterIt++;
    }
    else
    {
      /* Increment CounterOverFlow in case of detection of overflow at trace evacuation */
      CounterOverFlow++;
    }
  }
}

/**
  * Adv_Trace receive Callback function for handling the received data.
  *
  * p_data: Pointer to the buffer containing the received data.
  * size_byte: Number of bytes received in the buffer.
  * error: Error status of the reception.
  *
  */
void adv_trace_rx_user_callback(uint8_t *p_data, uint16_t size_byte, uint8_t error)
{
  /* Send the received message */
  ADV_TRACE_COND_FSend(MX_VERBOSE_LEVEL_MINIMUM, 0, MX_TS_OFF, "__Rx:%c\r\n", *p_data);
}
