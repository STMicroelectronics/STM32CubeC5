/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body (task prio using sequencer utility)
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
/* Tasks priorities IDs definition */
#define PRIO_HIGH       (0) /* High priority */
#define PRIO_MEDIUM     (1) /* Medium priority */
#define PRIO_LOW        (2) /* Low priority */

/* Gpio toggle task period definitions (in milliseconds) */
#define LOW_FREQ_TOGGLE_PERIOD      (1000/1) /* Toggle every 1000ms (0.5 HZ) */
#define HIGH_FREQ_TOGGLE_PERIOD     (1000/8) /* Toggle every 125ms (4 HZ) */

/* Gpio toggle task timing definition (in milliseconds) */
#define GPIO_TOGGLE_DURATION        (5000) /* Task runs for 5000ms (5 seconds)*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Counters for toggle tracking */
uint32_t LowFreqToggleCounter = 0U;
uint32_t HighFreqToggleCounter = 0U;

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  /** ########## Step 1 ##########
    * The applicative code initializes the GPIO.
    */
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_example_gpio_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  /** ########## Step 2 ##########
    * Initialize the sequencer and tasks.
    */
  mx_seq_init();

  /* Request execution for MANAGE_PRIO task */
  SEQ_SetTask(MX_TASK_MANAGE_PRIO, PRIO_LOW);

  return return_status;
} /* end app_init */


/** ########## Step 3 ##########
  * manage_prio() task function implementation.
  */
void manage_prio(void)
{
  /* Reset gpio pin */
  HAL_GPIO_WritePin(MX_EXAMPLE_GPIO_PORT, MX_EXAMPLE_GPIO_PIN, HAL_GPIO_PIN_RESET);

  /* Reset "LowFreqToggleCounter" and "HighFreqToggleCounter" variables */
  LowFreqToggleCounter = 0U;
  HighFreqToggleCounter = 0U;

  /* Set the priority of tasks in the sequencer */
  SEQ_SetTask(MX_TASK_MANAGE_PRIO, PRIO_LOW); /* Set manage_prio task to low priority */
  SEQ_SetTask(MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY, PRIO_MEDIUM); /* Set low_frequency toggle task
                                                                      to medium priority */
  SEQ_SetTask(MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY, PRIO_HIGH); /* Set high_frequency toggle task
                                                                      to high priority */
}/* end manage_prio */


/** ########## Step 4 ##########
  * gpio_toggle_high_frequency() task function implementation.
  */
void gpio_toggle_high_frequency(void)
{
  /* Store the start time in milliseconds */
  uint32_t high_freq_toggle_time_start = HAL_GetTick();
  /* Loop to toggle the gpio pin at high frequency */
  do
  {
    HAL_GPIO_TogglePin(MX_EXAMPLE_GPIO_PORT, MX_EXAMPLE_GPIO_PIN);

    /* Increment "HighFreqToggleCounter" variable */
    HighFreqToggleCounter++;

    /*  Wait for a predefined delay (HIGH_FREQ_TOGGLE_PERIOD) */
    HAL_Delay(HIGH_FREQ_TOGGLE_PERIOD);

    /* Continue lopping until the toggle time limit is reached  */
  } while ((HAL_GetTick() - high_freq_toggle_time_start) < GPIO_TOGGLE_DURATION);
}/* end gpio_toggle_high_frequency */


/** ########## Step 5 ##########
  * gpio_toggle_low_frequency() task function implementation.
  */
void gpio_toggle_low_frequency(void)
{
  /* Store the start time in milliseconds */
  uint32_t low_freq_toggle_time_start = HAL_GetTick();
  /* Loop to toggle the gpio pin at low frequency */
  do
  {
    HAL_GPIO_TogglePin(MX_EXAMPLE_GPIO_PORT, MX_EXAMPLE_GPIO_PIN);
    /* Increment the "LowFreqToggleCounter" variable */
    LowFreqToggleCounter++;

    /*  Wait for a predefined delay (LOW_FREQ_TOGGLE_PERIOD) */
    HAL_Delay(LOW_FREQ_TOGGLE_PERIOD);

    /* Continue lopping until the toggle time limit is reached */
  } while ((HAL_GetTick() - low_freq_toggle_time_start) < GPIO_TOGGLE_DURATION);
}/* end gpio_toggle_low_frequency */


/** ########## Step 6 ##########
  * Starts the sequencer scheduler.
  */
app_status_t app_process(void)
{
  /* Run all registered tasks */
  SEQ_Run(SEQ_DEFAULT);

  /* Return execution status as OK  */
  return EXEC_STATUS_OK;
} /* end app_process */


app_status_t app_deinit(void)
{
  /** This API is not used in this example (infinite loop).
    * It is optimized out by the toolchain.
    */
  return EXEC_STATUS_ERROR;
} /* end app_deinit */
