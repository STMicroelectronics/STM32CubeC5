/**
  ******************************************************************************
  * file           : example.c
  * brief          : TAMP erase the backup register when an external tamper event occurs
  *                  using LL APIs.
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: Backup register 4-bytes data value */
#define TAMP_BACKUP_DATA_VALUE (0xAAAAAAAAU)
/* @user: Backup register index */
#define TAMP_BACKUP_REG_IDX (0U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TamperDetected = 0U; /* Set to 1 if a MX_TAMPx event is detected */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_TAMPx peripheral and configures the MX_TAMPx input.
  * The backup domain write protection is disabled to access to the backup registers.
  * The MX_TAMPx detection event is activated on rising-edge.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_tampx_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Sets the MX_TAMPx backup register to the value 0xAAAAAAAA and checks the written data.
    * - Disable the backup registers write protection.
    * - Write 0xAAAAAAAA to the backup register.
    * - Checks the written data
    */
  PWR_DisableRTCDomainWriteProtection();

  TAMP_WriteBackupRegisterValue(TAMP_BACKUP_REG_IDX, TAMP_BACKUP_DATA_VALUE);

  if (TAMP_ReadBackupRegisterValue(TAMP_BACKUP_REG_IDX) != TAMP_BACKUP_DATA_VALUE)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Starts the MX_TAMPx and enables the interruption for the selected MX_TAMPx input.
    */
  TAMP_StartPassive_IT();

  /** ########## Step 4 ##########
    * Waits for the MX_TAMPx detection interrupt.
    * It is generated when the user button is pressed.
    * - Enable the backup registers write protection after the interrupt handling.
    */
  while (TamperDetected == 0U)
  {
    /** Put the CPU in Wait For Interrupt state. A tamper interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  PWR_EnableRTCDomainWriteProtection();

  /** ########## Step 5 ##########
    * Checks that the MX_TAMPx backup register is automatically cleared by hardware on tamper detection.
    */
  if (TAMP_ReadBackupRegisterValue(TAMP_BACKUP_REG_IDX) != 0U)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Stops the MX_TAMPx input and deinitializes the peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_tampx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** MX_TAMPx callback is executed when a tamper event is generated.
  * @user: This implementation of the MX_TAMPx event callback can be customized.
  */
void TAMP_TamperEventCallback(void)
{
  /*Asynchronous processing related to step 4 */
  TamperDetected = 1U;
}
