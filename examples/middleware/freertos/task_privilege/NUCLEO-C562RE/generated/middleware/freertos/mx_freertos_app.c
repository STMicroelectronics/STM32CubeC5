/**
  ******************************************************************************
  * file           : mx_freertos_app.c
  * brief          : freertos application file
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
#include <stdio.h>

#include "mx_freertos_app.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t      address;
  uint32_t      psp;
}FaultInfo_t;

/* Private define ------------------------------------------------------------*/
#define NUMBER_OF_TASKS (2)

#define EXAMPLE_SHARED_MEMORY_SIZE (512U)

#define LED1_GPIO_Port GPIOA

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define shared memory */
static uint8_t sharedMemory[ EXAMPLE_SHARED_MEMORY_SIZE ] __attribute__( ( aligned( 32 ) ) );
uint8_t fault_count, task_count, Suspend_Flag;

MemoryRegion_t xRegions_Task1[ portNUM_CONFIGURABLE_REGIONS ];
MemoryRegion_t xRegions_Task2[ portNUM_CONFIGURABLE_REGIONS ];

static StackType_t Task1Stack[ 2 * configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );
static StackType_t Task2Stack[ 2 * configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );

static TaskParameters_t Task1_unprivileged_Parameters;
static TaskParameters_t Task2_unprivileged_Parameters;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static TaskHandle_t Main_Privileged;
static TaskHandle_t Supervisor_Privileged;
static TaskHandle_t TaskHandles[NUMBER_OF_TASKS];
static QueueHandle_t fault_queue;

#if defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* Single-definition MPU/linker boundary symbols for AC6. */
const uint32_t * __FLASH_segment_start__ = EXAMPLE_FLASH_START;
const uint32_t * __FLASH_segment_end__ = EXAMPLE_FLASH_END;
const uint32_t * __SRAM_segment_start__ = EXAMPLE_SRAM_START;
const uint32_t * __SRAM_segment_end__ = EXAMPLE_SRAM_END;

const uint32_t * __privileged_functions_start__ = EXAMPLE_PRIVILEGED_FUNC_START;
const uint32_t * __privileged_functions_end__ = EXAMPLE_PRIVILEGED_FUNC_END;
const uint32_t * __privileged_sram_start__ = EXAMPLE_PRIVILEGED_SRAM_START;
const uint32_t * __privileged_sram_end__ = EXAMPLE_PRIVILEGED_SRAM_END;

const uint32_t * __unprivileged_flash_start__ = ( uint32_t * ) &( Image$$ER_IROM_UNPRIVILEGED$$Base );
const uint32_t * __unprivileged_flash_end__ = ( uint32_t * ) &( Image$$ER_IROM_UNPRIVILEGED$$Limit );

const uint32_t * __syscalls_flash_start__ = ( uint32_t * ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base );
const uint32_t * __syscalls_flash_end__ = ( uint32_t * ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Limit );
#endif

/* Private prototypes --------------------------------------------------------*/
static void MainTask_entry(void *pvParameters);
static void SupervisorTask_entry(void *pvParameters);
static void Task1_unprivileged_entry(void *pvParameters);
static void Task2_unprivileged_entry(void *pvParameters);
#if defined(USE_TRACE) && USE_TRACE != 0
static const char *TaskStateToString(eTaskState state);
#endif
/**
  * @brief Initializes FreeRTOS kernel objects.
  * @param None
  * @retval int32_t Returns 0 on success, -1 on failure.
  */
int32_t app_synctasks_init(void)
{
/** ########## Step 1 ##########
  * Create a queue and three tasks.
  */
  BaseType_t ret;

  /* Create Main task */
  ret = xTaskCreate(MainTask_entry, "MainTask", 4 * configMINIMAL_STACK_SIZE, NULL,(configMAX_PRIORITIES -2)|portPRIVILEGE_BIT,
                    &Main_Privileged);
  if (ret != pdPASS)
  {
    return -1;
  }

  /* Create Supervisor task */
  ret = xTaskCreate(SupervisorTask_entry, "SupervisorTask", 4 * configMINIMAL_STACK_SIZE, NULL,(configMAX_PRIORITIES -1)|portPRIVILEGE_BIT,
                    &Supervisor_Privileged);
  if (ret != pdPASS)
  {
    return -1;
  }

  /* Create unprivileged Task1 with limited access to shared memory. */
  /* Task1 first Region configuration */
  xRegions_Task1[0].pvBaseAddress = sharedMemory;
  xRegions_Task1[0].ulLengthInBytes= EXAMPLE_SHARED_MEMORY_SIZE;
  xRegions_Task1[0].ulParameters = tskMPU_REGION_READ_ONLY |tskMPU_REGION_EXECUTE_NEVER;
  /* Task1 second Region configuration */
  xRegions_Task1[1].pvBaseAddress = LED1_GPIO_Port;
  xRegions_Task1[1].ulLengthInBytes= EXAMPLE_SHARED_MEMORY_SIZE;
  xRegions_Task1[1].ulParameters = tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER;
  /* Task1 third Region configuration */
  xRegions_Task1[2].pvBaseAddress = 0;
  xRegions_Task1[2].ulLengthInBytes = 0;
  xRegions_Task1[2].ulParameters = 0;

  /* Task1 parameter settings */
  Task1_unprivileged_Parameters.pvTaskCode     = Task1_unprivileged_entry,
  Task1_unprivileged_Parameters.pcName         = "Task1",
  Task1_unprivileged_Parameters.usStackDepth   = 2 * configMINIMAL_STACK_SIZE,
  Task1_unprivileged_Parameters.pvParameters   = NULL,
  Task1_unprivileged_Parameters.uxPriority     = (tskIDLE_PRIORITY +1);//|portPRIVILEGE_BIT,
  Task1_unprivileged_Parameters.puxStackBuffer = Task1Stack,
  Task1_unprivileged_Parameters.xRegions[0]       =  xRegions_Task1[0];
  Task1_unprivileged_Parameters.xRegions[1]       =  xRegions_Task1[1];
  Task1_unprivileged_Parameters.xRegions[2]       =  xRegions_Task1[2];

  /* Task1 restricted creation */
  ret = xTaskCreateRestricted(&Task1_unprivileged_Parameters, &TaskHandles[0]);

  if (ret != pdPASS)
  {
    return -1;
  }

  /* Task2 first Region configuration */
  xRegions_Task2[0].pvBaseAddress = sharedMemory;
  xRegions_Task2[0].ulLengthInBytes= EXAMPLE_SHARED_MEMORY_SIZE;
  xRegions_Task2[0].ulParameters = tskMPU_REGION_READ_ONLY | tskMPU_REGION_EXECUTE_NEVER;
  /* Task2 second Region configuration */
  xRegions_Task2[1].pvBaseAddress = LED1_GPIO_Port;
  xRegions_Task2[1].ulLengthInBytes= EXAMPLE_SHARED_MEMORY_SIZE;
  xRegions_Task2[1].ulParameters = tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER;
  /* Task2 third Region configuration */
  xRegions_Task2[2].pvBaseAddress = 0;
  xRegions_Task2[2].ulLengthInBytes= 0;
  xRegions_Task2[2].ulParameters = 0;

  /* Task2 parameter settings */
  Task2_unprivileged_Parameters.pvTaskCode     = Task2_unprivileged_entry,
  Task2_unprivileged_Parameters.pcName         = "Task2",
  Task2_unprivileged_Parameters.usStackDepth   = 2 * configMINIMAL_STACK_SIZE,
  Task2_unprivileged_Parameters.pvParameters   = NULL,
  Task2_unprivileged_Parameters.uxPriority     = tskIDLE_PRIORITY+1,
  Task2_unprivileged_Parameters.puxStackBuffer = Task2Stack,
  Task2_unprivileged_Parameters.xRegions[0]       =  xRegions_Task2[0];
  Task2_unprivileged_Parameters.xRegions[1]       =  xRegions_Task2[1];
  Task2_unprivileged_Parameters.xRegions[2]       =  xRegions_Task2[2];

  /* Task2 restricted creation */
  ret = xTaskCreateRestricted(&Task2_unprivileged_Parameters, &TaskHandles[1]);

  if (ret != pdPASS)
  {
    return -1;
  }

  /* fault_queue creation */
  fault_queue = xQueueCreate(5U, sizeof(FaultInfo_t));

  if (fault_queue == NULL)
  {
    return -1;
  }
  return 0;
}
/* Tasks entry function ------------------------------------------------------*/
/**
  * @brief Function implementing the Main Task_entry thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void MainTask_entry(void *pvParameters)
{
/** ########## Step 4 ##########
  * Main privileged task entry function.
  * Monitors the unprivileged tasks and reports their state.
  */
  TaskHandle_t background_task = xTaskGetHandle("Background Task");
  UNUSED(pvParameters);
  TaskStatus_t xTaskDetails;

  for(;;)
  {
    PRINTF("[INFO] Main Privileged Task is running \n");

    for(task_count = 0; task_count< NUMBER_OF_TASKS ;task_count++)
    {
      vTaskGetInfo(TaskHandles[task_count], &xTaskDetails, pdTRUE, eInvalid);
      PRINTF("[INFO] Task unprivileged %s is in state:  %s \n", xTaskDetails.pcTaskName,TaskStateToString(xTaskDetails.eCurrentState));

      if(xTaskDetails.eCurrentState == eSuspended)
      {
        PRINTF("[ALERT] User must investigate the memory fault cause \n");

        /* Notify the background task that the example runs as expected. */
        if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
        {
          PRINTF("[ERROR] Main Privileged Task - Notification failure!\n");
        }
      }
    }
    HAL_GPIO_TogglePin(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
/**
  * @brief Function implementing the Supervisor Task thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void SupervisorTask_entry(void *pvParameters)
{
/** ########## Step 3 ##########
  * Supervisor task entry function.
  * Monitors other tasks and handles memory fault reports.
  */
  TaskHandle_t background_task = xTaskGetHandle("Background Task");
  FaultInfo_t fault;
  UNUSED(pvParameters);

  for (;;)
  {
    xQueueReceive(fault_queue, &fault, portMAX_DELAY);

    PRINTF("[INFO] Supervisor Privileged Task is running \n");
    PRINTF("------------------------------\n");

    /* Increment fault count for task */
    fault_count++;

    /* Display info about fault */
    PRINTF("[INFO] MemManage fault occurred\r\n");
    PRINTF("\t[DATA] Address: 0x%X\n", (unsigned int) fault.address);

    /* Suspend Unprivileged Tasks*/
    for(task_count = 0; task_count < NUMBER_OF_TASKS; task_count++)
    {
      vTaskSuspend(TaskHandles[task_count]);
      Suspend_Flag = 1;
    }

    /* Notify the background task that the example runs as expected. */
    if (xTaskNotify(background_task, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
    {
      PRINTF("[ERROR] Supervisor Privileged Task - Notification failure!\n");
    }

    PRINTF("[INFO] Unprivileged Tasks were suspended.\n");
    PRINTF("------------------------------\n");
  }
}

/**
  * @brief Function implementing the Task1 unprivileged thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void Task1_unprivileged_entry(void *pvParameters)
{
/** ########## Step 4 ##########
  * Task1 unprivileged entry function.
  * Performs legal reads, then intentionally triggers an illegal write
  * to shared memory to demonstrate MPU fault handling.
  */
  volatile uint32_t read_data1 = 0;
  uint8_t read_count = 0;
  UNUSED(pvParameters);

  for(;;)
  {
    /* Try to perform a legal read from shared memory. */
    if(read_count < 3)
    {
      read_data1 = sharedMemory[read_count];
      UNUSED(read_data1);
      read_count ++;
    }
    else
    {
      /* Try to perform an illegal write to shared memory. */
      sharedMemory[EXAMPLE_SHARED_MEMORY_SIZE - 1] = 4U;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
/**
  * @brief Function implementing the Task2 unprivileged thread.
  * @param pvParameters: A pointer to the parameters passed to the task.
  * @retval None
  */
static void Task2_unprivileged_entry(void *pvParameters)
{
/** ########## Step 5 ##########
  * Task2 unprivileged entry function.
  * Toggles the LED and performs legal reads from shared memory.
  */
  volatile uint32_t read_data2 = 0;
  UNUSED(pvParameters);

  for(;;)
  {
    vTaskDelay(pdMS_TO_TICKS(500));

    HAL_GPIO_TogglePin(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN);

    vTaskDelay(pdMS_TO_TICKS(500));

    /* Try to perform a legal read from shared memory. */
    read_data2 = sharedMemory[EXAMPLE_SHARED_MEMORY_SIZE/2];
    UNUSED(read_data2);
  }
}

/**
  * @brief MemManage fault recovery.
  * @retval None
  */
void MemManage_Recover(void)
{
/** ########## Step 6 ##########
  * Memory Management fault recovery.
  * Logs the offending thread and sends fault information to the
  * supervisor task.
  */
  FaultInfo_t current_fault;

  /* Log the offending thread */
  current_fault.address  = SCB->MMFAR;
  current_fault.psp = (uint32_t)__get_PSP();

  /*
   * Clear DACCVIOL sticky bit in the MMFSR subregister.
   * If not cleared, subsequent fault addresses will not be stored in
   * the MMFAR register.*/
  STM32_SET_BIT(SCB->CFSR,SCB_CFSR_DACCVIOL_Msk);

  /* Send Fault info to Supervisor task */
  xQueueSendToBackFromISR(fault_queue, &current_fault, NULL);

  /* Current task yields. This forces a context switch and guarantees that
   * the Supervisor thread will be able to take action if needed before the current
   * thread continues.*/
  portYIELD_WITHIN_API();
}

/**
  * @brief Handle the Memory Management fault.
  * @retval None
  */
void MemManage_Handler(void)
{
  while (1)
  {
    MemManage_Recover();
    return;
  }
}

#if defined(USE_TRACE) && USE_TRACE != 0
/**
  * @brief Convert a FreeRTOS task state to a printable string.
  * @param state: The task state to convert.
  * @retval const char * A human-readable task state string.
  */
static const char *TaskStateToString(eTaskState state)
{
    switch (state)
    {
        case eRunning:   return "Running";
        case eReady:     return "Ready";
        case eBlocked:   return "Blocked";
        case eSuspended: return "Suspended";
        case eDeleted:   return "Deleted";
        case eInvalid:   return "Invalid";
        default:         return "Unknown";
    }
}
#endif
