/**
  ******************************************************************************
  * @file           : mx_freertos_app.c
  * @brief          : FreeRTOS initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_freertos_license.md file
  * in the same directory as the generated code.
  * If no mx_freertos_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_freertos_app.h"

/* Private define ------------------------------------------------------------*/
#define app_main_task_stack_size  256U
#define app_exti_task_stack_size  256U
#define app_timer_task_stack_size  256U
#define Timer1_period  ( pdMS_TO_TICKS( 1000UL ) )
#define SEQUENCE_MAX_VALUE         3
#define EVENT_EXTI                 1
#define EVENT_TIMER                2
#define EVENT_PUSH_BUTTON          4
#define EVENT_PERIOD_ELAPSED       8
#define EVENT_TO_CLEAR_ON_ENTRY    0x0000
#define EVENT_TO_CLEAR_ON_EXIT     0xFFFF
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t SequenceValue = 0;
hal_exti_handle_t *pEXTI;
/*-------------------- Tasks definition --------------------------------------*/
/* Definitions for app_main_task */
static TaskHandle_t app_main_task_Handle;
/* Definitions for app_exti_task */
static TaskHandle_t app_exti_task_Handle;
/* Definitions for app_timer_task */
static TaskHandle_t app_timer_task_Handle;

/*-------------------- Software Timers definition ---------------------------*/
/* Definitions for Timer1 */
static TimerHandle_t Timer1_Handle;

/* Private functions prototype -----------------------------------------------*/
/* Tasks entry function ------------------------------------------------------*/
static void app_main_task_entry(void *pvParameters);
static void app_exti_task_entry(void *pvParameters);
static void app_timer_task_entry(void *pvParameters);

/* Software Timers Callbacks function ----------------------------------------*/
void vTimerCallback(TimerHandle_t xTimer);
system_status_t post_system_init_hook(void);

/**
  * @brief Initializes FreeRTOS kernel objects.
  * @param None
  * @retval int32_t Returns 0 on success, -1 on failure.
  */
int32_t app_synctasks_init (void)
{
/** ########## Step 1 ##########
  * Create a software timer and 3 tasks
  */
  BaseType_t ret;

  /* app_main_task creation-------------------------------------*/
  ret = xTaskCreate(app_main_task_entry, "app_main_task", app_main_task_stack_size,
                    (void*) NULL, 5, &app_main_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_exti_task creation-------------------------------------*/
  ret = xTaskCreate(app_exti_task_entry, "app_exti_task", app_exti_task_stack_size,
                    (void*) NULL, 4, &app_exti_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_timer_task creation-------------------------------------*/
  ret = xTaskCreate(app_timer_task_entry, "app_timer_task", app_timer_task_stack_size,
                    (void*) NULL, 3, &app_timer_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* Timer1 creation------------------------------------*/
  Timer1_Handle = xTimerCreate("Timer1", Timer1_period, pdFALSE, (void *) NULL, vTimerCallback);

  if (Timer1_Handle == NULL)
  {
      return -1;
  }

  return 0;
}

/* Tasks entry function ------------------------------------------------------*/
/**
  * @brief Function implementing the app_main_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_main_task_entry(void *pvParameters)
{
/** ########## Step 3 ##########
  * Main Task entry function
  * Waits for external events from EXTI task and Timer task
  */
  ( void ) pvParameters;
  uint32_t NotifiedValue = 0;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  PRINTF("[INFO] [Main Task]: Waiting notification from Event Tasks \n");

  for(;;)
  {
    /* Infinite loop executing app_main_task functionality. */
    xTaskNotifyWait(EVENT_TO_CLEAR_ON_ENTRY, EVENT_TO_CLEAR_ON_EXIT, &NotifiedValue, portMAX_DELAY);

    if ((NotifiedValue & EVENT_EXTI) != 0)
    {
      /* Notification is set (Push Button is pressed) */
      PRINTF("[INFO] [Main Task]: EXTI Event received \n");
    }
    if ((NotifiedValue & EVENT_TIMER) != 0)
    {
      /* Notification is set (Software timer period reached) */
      PRINTF("[INFO] [Main Task]: Software Timer Event received \n\n");
    }
    /* Notify the background task that the example runs as expected. */
    if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
    {
      PRINTF("[ERROR] [Main Task] - Notification failure!\n");
    }
    /* sleep for 2 seconds to let the other tasks or the background task get scheduled */
    vTaskDelay(pdMS_TO_TICKS(2000U));

    /** ########## Step 6 ##########
      * sequence value reaches the SEQUENCE_MAX_VALUE
      * Stop EXTI event, print message and Toggle MX_STATUS_LED
      * delete Main task
      */
    /* End of the sequence, Toggle the USER_LED, delete the main Task and stop the EXTI interrupt*/
    if (SequenceValue >= SEQUENCE_MAX_VALUE)
    {
      HAL_EXTI_Disable(pEXTI);

      PRINTF("[INFO] [Main Task]: Sequence finished -> Toggle MX_STATUS_LED \n");
      led_on(MX_STATUS_LED);

      vTaskDelete(app_main_task_Handle);
    }
  }
}

/**
  * @brief Function implementing the app_exti_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_exti_task_entry(void *pvParameters)
{
/** ########## Step 4 ##########
  * EXTI Task entry function
  * Waits for the Push Button event
  * Send notification to to Main Task
  * Prints message to the serial port
  */
  ( void ) pvParameters;
  uint32_t NotifiedValue = 0;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  PRINTF("[INFO] [EXTI Task]: Waiting notification from Push Button \n");

  for(;;)
  {
    /* Infinite loop executing app_exti_task functionality. */
    xTaskNotifyWait(EVENT_TO_CLEAR_ON_ENTRY, EVENT_TO_CLEAR_ON_EXIT, &NotifiedValue, portMAX_DELAY);

    if (NotifiedValue & EVENT_PUSH_BUTTON)
    {
      /* Send notification to Main Task */
      PRINTF("[INFO] [EXTI Task]: Push Button pressed -> Notify Main Task \n");
      xTaskNotify(app_main_task_Handle, EVENT_EXTI, eSetValueWithOverwrite);
    }
    /* Notify the background task that the example runs as expected. */
    if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
    {
      PRINTF("[ERROR] [EXTI Task] - Notification failure!\n");
    }
    /* sleep for 1 seconds to let the other tasks or the background task get scheduled */
    vTaskDelay(pdMS_TO_TICKS(1000U));

    if (SequenceValue > SEQUENCE_MAX_VALUE)
    {
      vTaskDelete(app_exti_task_Handle);
    }
  }
}

/**
  * @brief Function implementing the app_timer_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_timer_task_entry(void *pvParameters)
{
/** ########## Step 5 ##########
  * Timer Task entry function
  * Waits for the softawre timer to reach a specific "period"
  * Sends a notification to Main Task
  */
  ( void ) pvParameters;
  uint32_t NotifiedValue = 0;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  PRINTF("[INFO] [Timer Task]: Waiting notification from Software timer \n\n");

  for(;;)
  {
    /* Infinite loop executing app_timer_task functionality. */
    xTaskNotifyWait(EVENT_TO_CLEAR_ON_ENTRY, EVENT_TO_CLEAR_ON_EXIT, &NotifiedValue, portMAX_DELAY);

    if (NotifiedValue & EVENT_PERIOD_ELAPSED)
    {
      /* Check if it is the correct message */
      PRINTF("[INFO] [Timer Task]: Getting notification from Software timer\n");

      /* Send notification to mainThread */
      PRINTF("[INFO] [Timer Task]: Timer period reached -> Notify Main Task \n");
      xTaskNotify(app_main_task_Handle, EVENT_TIMER, eSetValueWithOverwrite);

      /* Notify the background task that the example runs as expected. */
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] [Timer Task] - Notification failure!\n");
      }
      /* sleep for 1 seconds to let the other tesks or the background task get scheduled */
      vTaskDelay(pdMS_TO_TICKS(1000U));

      if (SequenceValue > SEQUENCE_MAX_VALUE)
      {
        vTaskDelete(app_timer_task_Handle);
      }
    }
  }
}

/* Software Timers Callbacks function ----------------------------------------*/
/**
  * @brief Function implementing the Timer1 callback.
  * @param xTimer: Handle to the timer that triggered the callback.
  * @retval None
  */
void vTimerCallback(TimerHandle_t xTimer)
{
  ( void ) xTimer;
  if (xTimer == Timer1_Handle)
  {
    xTaskNotify(app_timer_task_Handle, EVENT_PERIOD_ELAPSED, eSetValueWithOverwrite);
  }
}

/** brief:  The EXTI Callback function
  * retval: none
  */
static void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  STM32_UNUSED(trigger);
  pEXTI = mx_example_exti_trigger_gethandle();

  if (SequenceValue <= SEQUENCE_MAX_VALUE)
  {
    /* Once the Push button is preseed, notify the EXTI Task */
    xTaskNotify(app_exti_task_Handle, EVENT_PUSH_BUTTON, eSetValueWithOverwrite);
    /* Start the software timer if the push button is pressed */
    xTimerStart(Timer1_Handle, portMAX_DELAY);
    /* Increment the sequence value */
    SequenceValue ++;
  }
}

/**
  * @brief  User hook function called after the HAL_Init() and Peripheral init functions
  */
system_status_t post_system_init_hook(void)
{
  /* NOTE : This function must not be modified, when the callback is needed,
            the post_system_init_hook must be implemented in the user file
   */
  mx_example_exti_trigger_init();
  pEXTI = mx_example_exti_trigger_gethandle();

  /* Associates user callback function to any interrupt edge event*/
  HAL_EXTI_RegisterTriggerCallback(pEXTI, &UserDetectedExtiCallback);

  HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT);

  return SYSTEM_OK;
}
