/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  main() calls the target system initialization,
  *                  then calls the example entry point.
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
#include "main.h"
#include "mx_basic_stdio_app.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
app_status_t ExecStatus = EXEC_STATUS_UNKNOWN; /* application status */

/* Private functions prototype -----------------------------------------------*/
static void error_handler(void);

/** brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this generated code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL|LL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    ExecStatus = EXEC_STATUS_ERROR; /* memorize the error */
  }
  else
  {
#if defined(USE_TRACE) && USE_TRACE != 0
    /* Initialize basic_stdio separately, but after system init. */
    mx_basic_stdio_init();
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

    /** ########## Step 1 ##########
      * Initialize the application by creating the example task
      */
    PRINTF("[INFO] Starting LwIP SNTP client FreeRTOS example \n");
    ExecStatus = app_init();

    if (ExecStatus != EXEC_STATUS_ERROR)
    {
      /** ########## Step 2 ##########
        * Starts the FreeRTOS kernel scheduler
        * by calling the vTaskStartScheduler().
        * Once started the FreeRTOS kernel runs indefinitely.
        */
      PRINTF("[INFO] Starting FreeRTOS Scheduler \n");
      vTaskStartScheduler();
    }
  }

  /** This point must not be reached:
    * unless an init fails,
    * or the FreeRTOS scheduler cannot start.
    */
  error_handler();
} /* end main */

/** Redefines the HardFault handler from the startup file.
  * brief:  Hard Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * 1. The example status can be updated.
  * 2. You can easily set a breakpoint to investigate the issue.
  */
void HardFault_Handler(void)
{
  /* The example encountered an unrecoverable error */
  ExecStatus = EXEC_STATUS_ERROR;

  /* Take a chance to turn the status LED off (this might fail) */
  led_off(MX_STATUS_LED);

  /* Unrecoverable error: infinite loop */
  while (1);
}

/** ----------------------------------------------------------
  * The functions below are used to report the example status.
  * ----------------------------------------------------------
  */

/**
  * brief:  Error notification
  * retval: None (infinite loop)
  */
static void error_handler(void)
{

  while (1)
  {
    /* Repeated flashing status LED (50ms on and 2sec off) when execution loop is exited */
    led_on(MX_STATUS_LED);
    HAL_Delay(50);
    led_off(MX_STATUS_LED);
    HAL_Delay(2000);
  }
} /* end error_handler */

