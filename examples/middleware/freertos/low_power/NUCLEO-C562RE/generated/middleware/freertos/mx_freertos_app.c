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
#define app_task_one_stack_size  512U
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*-------------------- Tasks definition --------------------------------------*/
/* Definitions for app_task_one */
static TaskHandle_t app_task_one_Handle;

/*-------------------- Binary Semaphores definition -------------------------*/
/* Definitions for BinarySemaphore */
static SemaphoreHandle_t BinarySemaphore_Handle;

/* Private functions prototype -----------------------------------------------*/
/* Tasks entry function ------------------------------------------------------*/
static void app_task_one_entry(void *pvParameters);
static void toggle_user_led(uint32_t delay_ms);

/**
  * @brief Initializes FreeRTOS kernel objects.
  * @param None
  * @retval int32_t Returns 0 on success, -1 on failure.
  */
int32_t app_synctasks_init (void)
{
/** ########## Step 1 ##########
  * Initialize task and semaphore.
  */
  BaseType_t ret;

  /* app_task_one creation-------------------------------------*/
  ret = xTaskCreate(app_task_one_entry, "app_task_one", app_task_one_stack_size,
                    (void*) NULL, 5, &app_task_one_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* BinarySemaphore creation--------------------------*/
  BinarySemaphore_Handle = xSemaphoreCreateBinary();

  if (BinarySemaphore_Handle == NULL)
  {
      return -1;
  }

  /* Initially give the binary semaphore to allow the task to start */
  xSemaphoreGive(BinarySemaphore_Handle);

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
 * Take semaphore, toggle LED, notify, and sleep.
 */
  ( void ) pvParameters;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_task_one functionality. */
    if (xSemaphoreTake(BinarySemaphore_Handle, portMAX_DELAY) == pdTRUE)
    {
      PRINTF("[INFO] Task One - Binary Semaphore Taken!\n");
      PRINTF("[INFO] Task One - Toggling the status LED 10 times with 500ms delay.\n");
      toggle_user_led(500);

      /* Notify the background task that the example runs as expected. */
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] Task One - Notification failure!\n");
      }

      /* Block again on the semaphore and allow system to enter low power */
      PRINTF("[INFO] Task One - Blocking on semaphore; system entering low-power until LPTIM interrupt.\n\n\n");
    }
  }
}

/**
  * @brief Function for pre-sleep processing in tickless idle mode.
  * @param ulExpectedIdleTime: The expected idle time in ticks before the next task is due to run.
  * @retval None
  */
void app_freertos_pre_sleep (uint32_t ulExpectedIdleTime)
{
/** ########## Step 4 ##########
 * Configure wake-up source and enter low-power mode.
 */
  ( void ) ulExpectedIdleTime;
  /* Start the LPTIM in interrupt mode to wake up the MCU */
  HAL_LPTIM_Start_IT(mx_example_lptim_gethandle());

  /* Suspend HAL Tick increment */
  HAL_SuspendTick();

  /* Enter low-power mode */
  mx_pwr_enter_low_power();
}

/**
  * @brief Function for post-sleep processing in tickless idle mode.
  * @param ulExpectedIdleTime: The expected idle time in ticks before the next task is due to run.
  * @retval None
  */
void app_freertos_post_sleep (uint32_t ulExpectedIdleTime)
{
/** ########## Step 5 ##########
 * Exit low-power mode.
 */
  ( void ) ulExpectedIdleTime;
  /* Exit low-power mode */
  mx_pwr_exit_low_power();

  /* Resume HAL Tick increment */
  HAL_ResumeTick();
}

/**
  * brief:  Utility function to toggle the status LED.
  * retval: None
  */
static void toggle_user_led(uint32_t delay_ms)
{
  for (uint32_t i = 0U; i < 10U; i++)
  {
    led_toggle(MX_STATUS_LED);
    HAL_Delay(delay_ms);
  }
}

void HAL_LPTIM_AutoReloadMatchCallback(hal_lptim_handle_t *hlptim)
{
/** ########## Step 5 ##########
 * From ISR, release semaphore and stop LPTIM..
 */
  ( void ) hlptim;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  /* Give semaphore to wake app_task_on */
  xSemaphoreGiveFromISR(BinarySemaphore_Handle, &xHigherPriorityTaskWoken);

  /* Request context switch if the unblocked task has higher priority */
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

  /* Stop LPTIM for one-shot wakeup */
  HAL_LPTIM_Stop_IT(mx_example_lptim_gethandle());
}
