/**
  ******************************************************************************
  * file           : example.c
  * brief          : PWR PVD detection with HAL API
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_exti_handle_t *pPvdEXTI; /* pointer referencing the EXTI handle linked to PVD from the generated code */

/** Variable to check the PVD interrupt detection via the IRQ CallBack.
  * Declared as volatile because updated from an external event (interrupt callback).
  */
volatile uint8_t PvdDetected = 0;

/* Private functions prototype -----------------------------------------------*/
static void PvdDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger);

/** ########## Step 1 ##########
  * The applicative code initializes the PVD instance and its interrupt.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  mx_example_pvd_irq_init();
  pPvdEXTI = mx_example_pvd_irq_gethandle();

  if (pPvdEXTI != NULL)
  {
    /**
      * Set the CPU sleep mode configuration.
      * @user :  sleep_mode Parameter to hal_pwr_core_sleep_mode_t enumeration.
      */
    HAL_PWR_SetCoreSleepMode(HAL_PWR_CORE_DEEP_SLEEP);

    /* Associates pvd callback function to any interrupt edge event*/
    if (HAL_EXTI_RegisterTriggerCallback(pPvdEXTI, &PvdDetectedExtiCallback) == HAL_OK)
    {
      PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
      return_status = EXEC_STATUS_INIT_OK;
    }
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Starts waiting for the PVD event.
    */
  if (HAL_EXTI_Enable(pPvdEXTI, HAL_EXTI_MODE_INTERRUPT) == HAL_OK)
  {
    PRINTF("[INFO] Step 2: Waiting for the configured PVD interrupt.\n");
    while (PvdDetected == 0)
    {
      /* Awaiting Step 3 completion */
    }

    return_status = EXEC_STATUS_OK;
    PRINTF("[INFO] Step 3: PVD Interrupt detected.\n");
  }

  return return_status;
} /* end app_process */


/** ########## Step 3 - PVD Interrupt ##########
  *  Redefines the default PVD callback weak function from the HAL.
  */
static void PvdDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  PvdDetected = 1;
}


/** ########## Step 4 ##########
  * Deinitializes the GPIO before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /* Stop the EXTI mode for the selected EXTI line */
  HAL_EXTI_Disable(pPvdEXTI);

  /* Disable the PVD Output */
  HAL_PWR_DisableProgrammableVoltageDetector();

  mx_example_pvd_irq_deinit();

  PRINTF("[INFO] Step 4: Device deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
