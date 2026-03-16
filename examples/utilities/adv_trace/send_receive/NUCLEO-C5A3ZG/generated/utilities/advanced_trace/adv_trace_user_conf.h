/**
  **********************************************************************************************************************
  * @file    adv_trace_user_conf.h
  * @brief   advanced trace configuration file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_advanced_trace_license.md file
  * in the same directory as the generated code.
  * If no mx_advanced_trace_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef ADV_TRACE_USER_CONF_H
#define ADV_TRACE_USER_CONF_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include <cmsis_compiler.h>
#include <stdio.h>
#include <string.h>


/* Exported macros ---------------------------------------------------------------------------------------------------*/

#define ADV_TRACE_CONDITIONAL   /* Enable the conditional mode */



/**
  * @brief  Definition of the maximum size for the temporary trace formatting buffer.
  */
#define ADV_TRACE_MAX_FORMAT_BUF_SIZE        (128U)


/**
  * @brief Definition of the trace fifo size.
  */
#define ADV_TRACE_FIFO_SIZE                      (256U)

/* User memory address location of the trace buffer */
#define ADV_TRACE_MEMLOCATION


/**
  * @brief Macro used to initialize the critical section in trace feature.
  */
#define ADV_TRACE_INIT_CRITICAL_SECTION( )

/**
  * @brief Macro used to enter the critical section in trace feature.
  */
#define ADV_TRACE_ENTER_CRITICAL_SECTION( )     uint32_t primask_bit = __get_PRIMASK( );\
  __disable_irq( )

/**
  * @brief Macro used to exit the critical section in trace feature.
  */
#define ADV_TRACE_EXIT_CRITICAL_SECTION( )      __set_PRIMASK( primask_bit )

/**
  * @brief ADV_TRACE_MEMSET8 macro definition
  */
#define ADV_TRACE_MEMSET8( dest, value, size)   memset((dest),(value),(size));

/**
  * @brief ADV_TRACE_VSNPRINTF macro definition
  */
#define ADV_TRACE_VSNPRINTF(...)                vsnprintf(__VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /*ADV_TRACE_USER_CONF_H */
