/**
  ******************************************************************************
  * @file           : mx_seq.h
  * @brief          : header for mx_seq.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_SEQ_H
#define MX_SEQ_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "sequencer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/******************************************************************************/
/* Exported defines for mx_seq in sequencer utility                           */
/******************************************************************************/

/**
  * @brief  user tasks ID definition
  */
#define  MX_TASK_MANAGE_PRIO  SEQ_TASK_0
#define  MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY  SEQ_TASK_1
#define  MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY  SEQ_TASK_2

/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for mx_seq in sequencer utility                         */
/******************************************************************************/

/**
  * @brief seq initialization function
  * This function call the seq init function
  */
void mx_seq_init(void);

/**
  * @brief task functions definition
  *
  */
void manage_prio(void);
void gpio_toggle_low_frequency(void);
void gpio_toggle_high_frequency(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_SEQ_H */
