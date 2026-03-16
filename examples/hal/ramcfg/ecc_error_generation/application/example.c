/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the timeout period in milliseconds for erasing data */
#define TIMEOUT 1000U
/* @user: SRAM_SINGLE_ERR_ADDR_BASE and SRAM_DOUBLE_ERR_ADDR_BASE must be inside SRAM region*/
#define SRAM_SINGLE_ERR_ADDR_BASE SRAM_BASE
#define SRAM_SINGLE_ERR_ADDR_SIZE (0x200U / 0x04U)
#define SRAM_DOUBLE_ERR_ADDR_BASE (SRAM_BASE + 0x200U)
#define SRAM_DOUBLE_ERR_ADDR_SIZE (0x200U / 0x04U)

#define DATA_VALUE                0xAA55AA55UL
#define SINGLE_ERR_DATA_VALUE     0xAA55AA54UL
#define DOUBLE_ERR_DATA_VALUE     0xAA55AA50UL

#define EXPECTED_SINGLE_ERR_NUM   SRAM_SINGLE_ERR_ADDR_SIZE
#define EXPECTED_DOUBLE_ERR_NUM   SRAM_DOUBLE_ERR_ADDR_SIZE
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_ramcfg_t RamcfgInstance;
/* Single and Double ECC error flags declaration */
uint32_t ECC_SingleError = 0U;
uint32_t ECC_DoubleError = 0U;
/* Single and double error data buffers */
uint32_t SingleErrorData_Buffer[SRAM_SINGLE_ERR_ADDR_SIZE] = {0U};
uint32_t DoubleErrorData_Buffer[SRAM_DOUBLE_ERR_ADDR_SIZE] = {0U};
/* Private functions prototype -----------------------------------------------*/
static void FillMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data);
static void ReadMemory(uint32_t *pMemory, uint32_t *pBuffer, uint16_t BufferSize);
static app_status_t CheckBuffer(uint32_t *pBuffer, uint16_t BufferSize, uint32_t CompareData);

/** ########## Step 1 ##########
  * The applicative code initializes the RAMCFG instance.
  */
app_status_t app_init(void)
{
  /* Getting Ramcfg Instance */
  RamcfgInstance = mx_example_ramcfg_init();
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  return EXEC_STATUS_INIT_OK;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Erases the given SRAM memory and writes known data with ECC enabled.
    */
  if (HAL_RAMCFG_MassErase(RamcfgInstance, TIMEOUT) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_RAMCFG_ECC_Enable(RamcfgInstance) != HAL_OK)
  {
    goto _app_process_exit;
  }
  FillMemory((uint32_t *)SRAM_SINGLE_ERR_ADDR_BASE, (SRAM_SINGLE_ERR_ADDR_SIZE * 2U), DATA_VALUE);
  PRINTF("[INFO] Step 2: Memory erasing and write known data with ECC on COMPLETED.\n");

  /** ########## Step 3 ##########
    * Disables ECC.
    */
  if (HAL_RAMCFG_ECC_Disable(RamcfgInstance) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: ECC disabled.\n");

  /** ########## Step 4 ##########
    * Fills memory with single and double error data.
    */
  FillMemory((uint32_t *)SRAM_SINGLE_ERR_ADDR_BASE, SRAM_SINGLE_ERR_ADDR_SIZE, SINGLE_ERR_DATA_VALUE);
  FillMemory((uint32_t *)SRAM_DOUBLE_ERR_ADDR_BASE, SRAM_DOUBLE_ERR_ADDR_SIZE, DOUBLE_ERR_DATA_VALUE);
  PRINTF("[INFO] Step 4: Memory filled with single and double error data.\n");

  /** ########## Step 5 ##########
    * Enables ECC.
    */
  if (HAL_RAMCFG_ECC_Enable_IT(RamcfgInstance, (HAL_RAMCFG_IT_ECC_SINGLE | HAL_RAMCFG_IT_ECC_DOUBLE)) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 5: ECC enabled.\n");

  /** ########## Step 6 ##########
    * Reads memory content and checks that single and double ECC errors are generated.
    */
  ReadMemory((uint32_t *)SRAM_SINGLE_ERR_ADDR_BASE, SingleErrorData_Buffer, SRAM_SINGLE_ERR_ADDR_SIZE);
  ReadMemory((uint32_t *)SRAM_DOUBLE_ERR_ADDR_BASE, DoubleErrorData_Buffer, SRAM_DOUBLE_ERR_ADDR_SIZE);
  /* Checks that all single error data are corrected via RAMCFG peripheral */
  if (CheckBuffer(SingleErrorData_Buffer, SRAM_SINGLE_ERR_ADDR_SIZE, DATA_VALUE) != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  /* Checks that all double error data are not corrected via RAMCFG peripheral */
  if (CheckBuffer(DoubleErrorData_Buffer, SRAM_DOUBLE_ERR_ADDR_SIZE, DOUBLE_ERR_DATA_VALUE) != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  /* Checks the number of ECC single error detected */
  if (ECC_SingleError != EXPECTED_SINGLE_ERR_NUM)
  {
    goto _app_process_exit;
  }
  /* Checks the number of ECC double error detected */
  if (ECC_DoubleError != EXPECTED_DOUBLE_ERR_NUM)
  {
    goto _app_process_exit;
  }
  return_status = EXEC_STATUS_OK;
  PRINTF("[INFO] Step 6: Single and double ECC errors checked successfully.\n");
_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 7 ##########
  * Disables and stops the ECC monitoring of the selected RAMCFG instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  if (HAL_RAMCFG_ECC_Disable(RamcfgInstance) == HAL_OK)
  {
    return_status = EXEC_STATUS_OK;
  }
  PRINTF("[INFO] Step 7: de-init.\n");
  return return_status;
} /* end app_deinit */


/** RAMCFG ECC ErrorCallback
  * @user This implementation of the RAMCFG error callback can be customized.
  */
hal_status_t HAL_RAMCFG_ECC_ErrorCallback(hal_ramcfg_t hramcfg)
{
  hal_ramcfg_ecc_info_t p_info;

  HAL_RAMCFG_ECC_GetInfo(hramcfg, &p_info);
  if (p_info.type == HAL_RAMCFG_ECC_SINGLE)
  {
    ECC_SingleError++;
  }
  else
  {
    ECC_DoubleError++;
  }
  return HAL_OK;
}

static void FillMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data)
{

  /* Repeats for all buffer size */
  for (uint32_t IndexTmp = 0; IndexTmp < MemorySize; IndexTmp++)
  {
    pMemory[IndexTmp] = Data;
  }
}

/** This pragma is added only for event counting in this example and is not
  * a functional requirement for ECC operation.
  */
#if defined(__ICCARM__)
#pragma optimize = none
#endif /* defined(__ICCARM__) */
static void ReadMemory(uint32_t *pMemory, uint32_t *pBuffer, uint16_t BufferSize)
{
  for (uint32_t IndexTmp = 0; IndexTmp < BufferSize; IndexTmp++)
  {
    pBuffer[IndexTmp] = pMemory[IndexTmp];
  }
}
#if defined(__ICCARM__)
#if defined(RELEASE_IAR)
#pragma optimize = high
#else
#pragma optimize = low
#endif /* defined(RELEASE_IAR) */
#endif /* defined(__ICCARM__) */

static app_status_t CheckBuffer(uint32_t *pBuffer, uint16_t BufferSize, uint32_t CompareData)
{
  /* Repeats for all buffer size */
  for (uint32_t IndexTmp = 0; IndexTmp < BufferSize; IndexTmp++)
  {
    if (pBuffer[IndexTmp] != CompareData)
    {
      return EXEC_STATUS_ERROR;
    }
  }
  return EXEC_STATUS_OK;
}
