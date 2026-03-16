/**
  ******************************************************************************
  * @file : mx_seq.c
  * @brief : sequencer mx file
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

/* Includes ------------------------------------------------------------------*/
#include "mx_seq.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for mx_seq in sequencer utility  */
/******************************************************************************/

void mx_seq_init(void)
{
  /* Sequencer initialization */
  SEQ_Init();

  /* Tasks initialization */

  /* Register gpio_toggle task in the sequencer */
  SEQ_RegTask(MX_TASK_TOGGLE, 0, gpio_toggle);

}

/* Weak declaration of tasks function */
__WEAK void gpio_toggle(void)
{
  /* Task function must have a return to allow task scheduling */
  return;
}

