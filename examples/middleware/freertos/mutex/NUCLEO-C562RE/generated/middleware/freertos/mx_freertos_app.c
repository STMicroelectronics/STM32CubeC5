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
#define app_task_one_stack_size  256U
#define app_task_two_stack_size  256U
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*-------------------- Tasks definition --------------------------------------*/
/* Definitions for app_task_one */
static TaskHandle_t app_task_one_Handle;
/* Definitions for app_task_two */
static TaskHandle_t app_task_two_Handle;

/*-------------------- Mutexes definition -----------------------------------*/
/* Definitions for mutex */
static SemaphoreHandle_t mutex_Handle;

/* Private functions prototype -----------------------------------------------*/
/* Tasks entry function ------------------------------------------------------*/
static void app_task_one_entry(void *pvParameters);
static void app_task_two_entry(void *pvParameters);
static void toggle_user_led(uint32_t delay_ms);

/**
  * @brief Initializes FreeRTOS kernel objects.
  * @param None
  * @retval int32_t Returns 0 on success, -1 on failure.
  */
int32_t app_synctasks_init (void)
{
  BaseType_t ret;
/** ########## Step 1 ##########
  * Creates a mutex and 2 tasks with the same priority.
  */
  /* app_task_one creation-------------------------------------*/
  ret = xTaskCreate(app_task_one_entry, "app_task_one", app_task_one_stack_size,
                    (void*) NULL, 5, &app_task_one_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_task_two creation-------------------------------------*/
  ret = xTaskCreate(app_task_two_entry, "app_task_two", app_task_two_stack_size,
                    (void*) NULL, 5, &app_task_two_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* mutex creation-----------------------------------*/
  mutex_Handle = xSemaphoreCreateMutex();

  if (mutex_Handle == NULL)
  {
      return -1;
  }

  return 0;
}

/* Tasks entry function ------------------------------------------------------*/
/**
  * @brief Function implementing the app_task_one thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_task_one_entry(void *pvParameters)
{
/** ########## Step 3 ##########
  * Take the mutex without timeout.On success, toggle the green led 10 times each 500 ms.
  * Release the mutex.
  * Sleep for 2 seconds to let the kernel reschedule the second thread.
  */
  ( void ) pvParameters;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_task_one functionality. */
    /* try to acquire the mutex without timeout */
    if (xSemaphoreTake(mutex_Handle, 0) == pdTRUE)
    {
      PRINTF("[INFO] Task One - Mutex Taken!\n");
      PRINTF("[INFO] Task One - Toggling the status LED 10 times (500 ms)\n");
      toggle_user_led(500);

      /* release the mutex */
      PRINTF("[INFO] Task One - Mutex Given!\n");

      xSemaphoreGive(mutex_Handle);

      /* Notify the background task that the example runs as expected. */
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] Task One - Notification failure!\n");
      }

      /* sleep for 2 seconds to let the 'App Task Two' or the background task get scheduled */
      vTaskDelay(pdMS_TO_TICKS(2000U));
    }
  }
}

/**
  * @brief Function implementing the app_task_two thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_task_two_entry(void *pvParameters)
{
/** ########## Step 4 ##########
  * Take the mutex without timeout.On success, toggle the green led 10 times each 100 ms.
  * Release the mutex.
  * Sleep for 2 seconds to let the kernel reschedule the other thread.
  */
  ( void ) pvParameters;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_task_two functionality. */
    /* try to acquire the mutex without timeout */
    if (xSemaphoreTake(mutex_Handle, 0) == pdTRUE)
    {
      PRINTF("[INFO] Task Two - Mutex Taken!\n");
      PRINTF("[INFO] Task Two - Toggling the status LED 10 times (100 ms)\n");
      toggle_user_led(100);

      /* release the mutex */
      PRINTF("[INFO] Task Two - Mutex Given!\n");
      PRINTF("------------\n");

      xSemaphoreGive(mutex_Handle);

      /* Notify the background task that the example runs as expected. */
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] Task Two - Notification failure!\n");
      }

      /* sleep for 2 seconds to let the 'App Task One' or the background task get scheduled */
      vTaskDelay(pdMS_TO_TICKS(2000U));
    }
  }
}

/**
  * brief:  Utility function to toggle the status LED.
  * retval: None
  */
static void toggle_user_led(uint32_t delay_ms)
{
  for (uint32_t i = 0U; i < 10U; i++)
  {
    HAL_GPIO_TogglePin(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN);
    HAL_Delay(delay_ms);
  }
}
