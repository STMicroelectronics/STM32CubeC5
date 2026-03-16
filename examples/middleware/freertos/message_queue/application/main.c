/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  main() calls the target system initialization, then calls the example entry point.
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
#include "mx_basic_stdio_app.h" /* logs */
#include "mx_freertos_app.h"      /* background task */
#include <limits.h>            /* ULONG_MAX       */
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
app_status_t ExecStatus = EXEC_STATUS_UNKNOWN; /* application status */
static TaskHandle_t BackgroundTask;            /* system background task handling the example status */

/* Private functions prototype -----------------------------------------------*/
static void error_handler(void);
static void background_task_entry(void *args);

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

    /** Create a dedicated background task for the system handling.
      * The idle task and its hook are fully available for the user code.
      */
    if (xTaskCreate(background_task_entry, "Background Task",
                    configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY + 1, &BackgroundTask) != pdPASS)
    {
      ExecStatus = EXEC_STATUS_ERROR;
      error_handler();
    }

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

/** brief:  This function the System background task of this example.
  * retval: none
  */
static void background_task_entry(void *args)
{
  /** This task checks the execution status of the example.
    * The FreeRTOS task notification mechanism is used to save resources.
    * RTOS task notifications can only be used when there is only one task that can be the recipient of the event.
    * We do the notification check each time this system background task is scheduled.
    * If a notification is received, the appropriate handler is called.
    * If no notification is received, the execution continues.
    */
  uint32_t ulNotifiedValue = NOTIFICATION_VALUE_UNKNOWN;

  while (1)
  {
    /* Read the pending notification if present, otherwise ulNotifiedValue is unchanged.                  */
    uint32_t xResult = xTaskNotifyWait(pdFALSE,           /* Don't clear bits on entry.                   */
                                       ULONG_MAX,         /* Clear all bits on exit.                      */
                                       &ulNotifiedValue,  /* Stores the notified value.                   */
                                       pdMS_TO_TICKS(0)); /* Do not block if no notification is pending.  */

    if (xResult == pdTRUE)
    {
      /* Process the notification value as needed */
      switch (ulNotifiedValue)
      {
        case NOTIFICATION_VALUE_SUCCESS:
          /* Memorize the success */
          ExecStatus = EXEC_STATUS_OK;
          /* Let the example run freely */
          break;
        case NOTIFICATION_VALUE_ERROR:
          /* Memorize the error */
          ExecStatus = EXEC_STATUS_ERROR;
          /* Suspend all tasks without cleaning them to allow debug on hot-attach */
          vTaskSuspendAll();
          /* Block in the Idle task to prevent the scheduler from restarting the tasks. */
          error_handler();
          break;
        case NOTIFICATION_VALUE_UNKNOWN:
          /* Do nothing: the example keeps running */
          break;
        default:
          /* Abnormal case: block in the Idle task. */
          error_handler();
          break;
      }
    }
    else
    {
      /** No pending notification,
        * wait for the next execution cycle.
        * An arbitrary value of 10ms is chosen.
        */
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
} /* end background_task_entry */
