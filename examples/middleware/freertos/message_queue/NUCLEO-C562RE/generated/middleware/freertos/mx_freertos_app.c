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
#define app_producer_task_stack_size  256U
#define app_consumer_task_stack_size  256U
#define app_gen_task_stack_size  256U
#define app_receive_task_stack_size  256U
#define queue_buffer_size  16U
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define CONSUMER_VALUE 10
#define GEN_VALUE 20
#define PRODUCER_MAX_VALUE 25
/*-------------------- Tasks definition --------------------------------------*/
/* Definitions for app_producer_task */
static TaskHandle_t app_producer_task_Handle;
/* Definitions for app_consumer_task */
static TaskHandle_t app_consumer_task_Handle;
/* Definitions for app_gen_task */
static TaskHandle_t app_gen_task_Handle;
/* Definitions for app_receive_task */
static TaskHandle_t app_receive_task_Handle;

/*-------------------- Queues definition ------------------------------------*/
/* Definitions for queue */
static QueueHandle_t queue_Handle;

/* Private functions prototype -----------------------------------------------*/
/* Tasks entry function ------------------------------------------------------*/
static void app_producer_task_entry(void *pvParameters);
static void app_consumer_task_entry(void *pvParameters);
static void app_gen_task_entry(void *pvParameters);
static void app_receive_task_entry(void *pvParameters);

uint32_t ProducerValue = 10;

/**
  * @brief Initializes FreeRTOS kernel objects.
  * @param None
  * @retval int32_t Returns 0 on success, -1 on failure.
  */
int32_t app_synctasks_init (void)
{
/** ########## Step 1 ##########
  * Create a queue and 4 tasks
  */
  BaseType_t ret;

  /* app_producer_task creation-------------------------------------*/
  ret = xTaskCreate(app_producer_task_entry, "app_producer_task", app_producer_task_stack_size,
                    (void*) NULL, 5, &app_producer_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_consumer_task creation-------------------------------------*/
  ret = xTaskCreate(app_consumer_task_entry, "app_consumer_task", app_consumer_task_stack_size,
                    (void*) NULL, 5, &app_consumer_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_gen_task creation-------------------------------------*/
  ret = xTaskCreate(app_gen_task_entry, "app_gen_task", app_gen_task_stack_size,
                    (void*) NULL, 4, &app_gen_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* app_receive_task creation-------------------------------------*/
  ret = xTaskCreate(app_receive_task_entry, "app_receive_task", app_receive_task_stack_size,
                    (void*) NULL, 4, &app_receive_task_Handle);

  if (ret != pdPASS)
  {
      return -1;
  }

  /* queue creation-------------------------------------*/
  queue_Handle = xQueueCreate(queue_buffer_size, sizeof(uint16_t));

  if (queue_Handle == NULL)
  {
      return -1;
  }

  return 0;
}

/* Tasks entry function ------------------------------------------------------*/
/**
  * @brief Function implementing the app_producer_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_producer_task_entry(void *pvParameters)
{
/** ########## Step 3 ##########
  * Sends message (incrementing value "ProducerValue") to the queue
  */
  ( void ) pvParameters;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_producer_task functionality. */
    if (ProducerValue < PRODUCER_MAX_VALUE)
    {
      /* Set the message to the queue */
      PRINTF("[INFO] [Producer Task]: Sending ProducerValue msg to the queue\n\n");
      xQueueSend(queue_Handle, &ProducerValue, portMAX_DELAY);

      ProducerValue = ProducerValue + 10;

      /* Notify the background task that the example runs as expected. */
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] [Producer Task] - Notification failure!\n");
      }

      /* sleep for 2 seconds to let the other tesks or the background task get scheduled */
      vTaskDelay(pdMS_TO_TICKS(2000U));
    }
    else
    {
      /* Delete Producer Task */
      vTaskDelete(app_producer_task_Handle);
    }
  }
}

/**
  * @brief Function implementing the app_consumer_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_consumer_task_entry(void *pvParameters)
{
/** ########## Step 4 ##########
  * Gets message from the queue
  * Checks if it's the correct message (ProducerValue == ConsumerValue)
  * Prints message to the serial port
  */
  ( void ) pvParameters;
  uint32_t osQueueMsg = 0;

  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_consumer_task functionality. */
    /* Get the message from the queue */
    xQueueReceive(queue_Handle, &osQueueMsg, portMAX_DELAY);

    /* Check if it is the correct message */
    PRINTF("[INFO] [Consumer Task]: Getting the msg from queue & checking if it's the correct msg\n");
    if (osQueueMsg == CONSUMER_VALUE)
    {
      PRINTF("[INFO] [Consumer Task]: Correct msg verified\n\n");
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] [Consumer Task] - Notification failure!\n");
      }
    }
    else
    {
      PRINTF("[INFO] [Consumer Task]: Received msg not correct\n\n");
      if (xTaskNotify(background_task, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite) == pdFAIL)
      {
        PRINTF("[ERROR] [Consumer Task] - Notification failure!\n");
      }
    }

    /* Delete Consumer Task */
    vTaskDelete(app_consumer_task_Handle);
  }
}

/**
  * @brief Function implementing the app_gen_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_gen_task_entry(void *pvParameters)
{
/** ########## Step 5 ##########
  * Gen Task entry function
  * Waits for the message on the queue to reach a specific "GenerationValue"
  * Sends a notification to Receive Task
  */
  ( void ) pvParameters;
  uint32_t osQueueMsg = 0;

  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_gen_task functionality. */
    /* Get the message from the queue */
    xQueueReceive(queue_Handle, &osQueueMsg, portMAX_DELAY);

    /* Check if it is the correct message */
    PRINTF("[INFO] [Gen Task]: Getting the msg from queue & checking if the GEN_VALUE is reached\n");
    if (osQueueMsg == GEN_VALUE)
    {
      /* Send notification to ReceiveThread */
      PRINTF("[INFO] [Gen Task]: GenerationValue reached -> Notify Receive Task\n\n");
      xTaskNotifyGive(app_receive_task_Handle);
    }

    /* Notify the background task that the example runs as expected. */
    if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
    {
      PRINTF("[ERROR] [Gen Task] - Notification failure!\n");
    }

    /* Delete Gen Task */
    vTaskDelete(app_gen_task_Handle);
  }
}

/**
  * @brief Function implementing the app_receive_task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void app_receive_task_entry(void *pvParameters)
{
/** ########## Step 6 ##########
  * Receive Task entry function
  * Wait until being notified
  * Toggles MX_STATUS_LED
  */
  ( void ) pvParameters;
  TaskHandle_t background_task = xTaskGetHandle("Background Task");

  for(;;)
  {
    /* Infinite loop executing app_receive_task functionality. */
    /* Wait until being notified by GenThread*/
    PRINTF("[INFO] [Receive Task]: Wait until being notified\n\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Toggle MX_STATUS_LED */
    PRINTF("[INFO] [Receive Task]: Notification received -> Toggle MX_STATUS_LED\n");
    led_on(MX_STATUS_LED);

    /* Notify the background task that the example runs as expected. */
    if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
    {
      PRINTF("[ERROR] [Receive Task] - Notification failure!\n");
    }

    /* Delete Receive Task */
    vTaskDelete(app_receive_task_Handle);
  }
}

