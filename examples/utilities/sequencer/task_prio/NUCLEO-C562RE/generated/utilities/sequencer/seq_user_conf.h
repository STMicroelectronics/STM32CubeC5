/**
  **********************************************************************************************************************
  * @file    seq_user_conf.h
  * @brief   Sequencer configuration header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_sequencer_license.md file
  * in the same directory as the generated code.
  * If no mx_sequencer_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef SEQ_USER_CONF_H
#define SEQ_USER_CONF_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "cmsis_compiler.h"
#include <string.h>

/* Exported macros ---------------------------------------------------------------------------------------------------*/

/**
  * @brief  Macro used to initialize the critical section.
  */
#define SEQ_INIT_CRITICAL_SECTION( )

/**
  * @brief  Macro used to enter the critical section.
  */
#define SEQ_ENTER_CRITICAL_SECTION( )      uint32_t primask_bit = __get_PRIMASK( );\
  __disable_irq( )

/**
  * @brief  Macro used to exit the critical section.
  */
#define SEQ_EXIT_CRITICAL_SECTION( )       __set_PRIMASK( primask_bit )

/**
  * @brief  Number of tasks.
  */
#define SEQ_CONF_TASK_NBR                  (3U)

/**
  * @brief  Number of priority levels.
  */
#define SEQ_CONF_PRIO_NBR                  (3U)


/**
  * @brief  Memory fill macro.
  */
#define SEQ_MEMSET8(dest, value, size)     memset((dest),(value),(size));

/**
  * @brief  Macro used to enter the critical section before calling the idle function.
  * @note   In a basic configuration, this macro should be identical to
  *         SEQ_ENTER_CRITICAL_SECTION. Redefine it if specific operations are required.
  */
#define SEQ_ENTER_CRITICAL_SECTION_IDLE( )    SEQ_ENTER_CRITICAL_SECTION( )

/**
  * @brief  Macro used to exit the critical section after the idle function returns.
  * @note   The behavior of this macro must be symmetrical with
  *         SEQ_ENTER_CRITICAL_SECTION_IDLE.
  */
#define SEQ_EXIT_CRITICAL_SECTION_IDLE( )     SEQ_EXIT_CRITICAL_SECTION( )


#ifdef __cplusplus
}
#endif

#endif /*SEQ_USER_CONF_H */
