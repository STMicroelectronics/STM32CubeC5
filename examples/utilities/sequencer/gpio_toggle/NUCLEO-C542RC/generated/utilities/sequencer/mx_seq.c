/**
  ******************************************************************************
  * @file           : mx_seq.c
  * @brief          : Sequencer initialization source file
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
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for the Sequencer utility                               */
/******************************************************************************/

/**
  * @brief  Initialize the Sequencer and register user tasks.
  */
void mx_seq_init(void)
{
  /* Sequencer initialization */
  SEQ_Init();

  /* Task registration */

  /* Register gpio_toggle as a Sequencer task */
  SEQ_RegTask(MX_TASK_TOGGLE, 0, gpio_toggle);

}

/* Weak task function definitions --------------------------------------------*/

__WEAK void gpio_toggle(void)
{
  /* Task functions must return to allow the Sequencer to continue scheduling. */
  return;
}

