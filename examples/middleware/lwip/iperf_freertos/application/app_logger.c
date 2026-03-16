/**
  ******************************************************************************
  * File Name          : app_logger.c
  * Description        : This file provides code the logger.
  ******************************************************************************
  * @attention
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
#include "app_logger.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* The mutex for safe printf */
static SemaphoreHandle_t PrintMutex = NULL;

/* Private functions ---------------------------------------------------------*/

void AppLoggerLock(void)
{
  if (PrintMutex != NULL)
  {
    xSemaphoreTake(PrintMutex, portMAX_DELAY);
  }
}

void AppLoggerUnlock(void)
{
  if (PrintMutex != NULL)
  {
    xSemaphoreGive(PrintMutex);
  }
}

void AppLoggerHeader(const char *level, const char *name)
{
  PRINTF("[%s] %s - ", level, name);
}

/* Public functions ---------------------------------------------------------*/

app_status_t app_logger_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  PrintMutex = xSemaphoreCreateMutex();
  if (PrintMutex == NULL)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
}
