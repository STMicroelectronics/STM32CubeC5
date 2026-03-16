/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to automatically erase the backup 0 register
  *                  when an external tamper event occurs.
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
/* 4-bytes data to write to the backup 0 register */
#define BKP0_REG_DATA     0xAAAAAAAA
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TamperDetected = 0U; /* Set to 1 if a tamper event is detected */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the peripheral and configures the tamper input.
  * The backup domain write protection is disabled to access to the backup registers.
  * The tamper detection event is activated on rising-edge.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_example_tamp_init() == SYSTEM_OK)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Sets the backup 0 register to the value 0xAAAAAAAA and checks the written data.
    * @user: Update BKP0_REG_DATA to modify the data to write.
    *        Change TAMP Backup register to select the backup register to write.
    *        HAL_TAMP_BACKUP_REG_0: TAMP Backup register 0
    */
  if (HAL_TAMP_WriteBackupRegisterValue(HAL_TAMP_BACKUP_REG_0, BKP0_REG_DATA) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Read back the backup 0 register to check the written data */
  if (HAL_TAMP_ReadBackupRegisterValue(HAL_TAMP_BACKUP_REG_0) != BKP0_REG_DATA)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: Backup register 0 WRITTEN. \n");

  /** ########## Step 3 ##########
    * Starts the tamper and enables the interruption for the selected tamper input.
    */
  if (HAL_TAMP_PASSIVE_Start(MX_TAMP_INPUT, MX_TAMP_IT_EN) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: Tamper STARTED.\nPress the user button to generate a tamper detection.... \n");

  /** ########## Step 4 ##########
    * Waits for the tamper detection interrupt.
    * It is generated when the user button is pressed.
    */
  while (TamperDetected == 0U)
  {
    /** Put the CPU in Wait For Interrupt state. A TAMP interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }
  PRINTF("[INFO] Step 4: User button PRESSED. Tamper event DETECTED. \n");

  /** ########## Step 5 ##########
    * Checks that the backup 0 register is automatically cleared by hardware on tamper detection.
    */
  if (HAL_TAMP_ReadBackupRegisterValue(HAL_TAMP_BACKUP_REG_0) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 5: Backup register 0 CLEARED. \n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Stops the tamper input and deinitializes the peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  HAL_TAMP_PASSIVE_Stop(MX_TAMP_INPUT);
  mx_example_tamp_deinit();
  PRINTF("[INFO] Step 6: de-init \n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Tamper event callback
  * @user This implementation of the Tamper event callback can be customized.
  * This function is executed if a tamper detection event is triggered.
  */
void HAL_TAMP_TamperEventCallback(uint32_t tampers)
{
  /*Asynchronous processing related to step 4 */
  TamperDetected = 1U;
}
