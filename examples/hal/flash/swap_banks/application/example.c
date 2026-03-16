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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
button_t *pUserButton;
volatile uint32_t EventFlag;
volatile button_state_t ButtonState = (button_state_t) 1U;

/* Private variables ---------------------------------------------------------*/
hal_flash_handle_t *pFLASH;  /* pointer referencing the FLASH handle from the generated code */

/* Private macro -------------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void ButtonCb(button_t *pbutton, void *arg);


/** ########## Step 1 ##########
  * The init of the Flash instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFLASH = mx_example_flash_init();

  /** Unlocking the FLASH configuration settings.
    * Use the HAL_FLASH_ITF component to configure Option Bytes and control settings.
    * This requires unlocking write access to the registers involved in this configuration.
    */
  HAL_FLASH_ITF_Unlock(HAL_FLASH);

  /**
    * Unlock the Option Bytes (OB) control.
    * OB_Unlock is required for option bytes access and forces ITF_Unlock if not already done.
    */
  HAL_FLASH_ITF_OB_Unlock(HAL_FLASH);

  if (pFLASH != NULL)
  {
    if (HAL_FLASH_ITF_OB_IsLocked(HAL_FLASH) == HAL_FLASH_ITF_OB_UNLOCKED
        && HAL_FLASH_ITF_IsLocked(HAL_FLASH) == HAL_FLASH_ITF_UNLOCKED)
    {
      PRINTF("[INFO] Step 1: Device initialization COMPLETED and the Flash access is unlocked.\n");
      return_status = EXEC_STATUS_INIT_OK;
    }
    else
    {
      PRINTF("[ERROR] Step 1: Device initialization ERROR: Flash control operations are still locked.\n");
      return_status = EXEC_STATUS_ERROR;
    }
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  /* User button initialization */
  pUserButton = mx_button_getobject();
  button_init(pUserButton, MX_BUTTON);
  button_register_callback(pUserButton, ButtonCb, BUTTON_EVENT_ANY, NULL);
  /* Clear the event flag to start tracking events */
  EventFlag = 0U;
  button_enableit(pUserButton);

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Use HAL_FLASH_ITF_OB_GetBankSwap to determine the current bank swap state.
  * If the banks are not swapped, enable the swap (Bank1 --> Bank2).
  * If the banks are already swapped, disable the swap (Bank2 --> Bank1).
  * Enabling the swap places bank 1 at the lower address range.
  * Disabling the swap places bank 1 at the higher address range.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Wait for the button to be unpressed */
  if (EventFlag == 1U && ButtonState == BUTTON_UNPRESSED)
  {

    /* Clear the interrupt flag to detect subsequent interrupts */
    EventFlag = 0U;


    if (HAL_FLASH_ITF_OB_GetBankSwap(HAL_FLASH) == HAL_FLASH_ITF_OB_BANK_NO_SWAP)
    {
      /*Swap to bank2 */
      if (HAL_FLASH_ITF_OB_SetBankSwap(HAL_FLASH, HAL_FLASH_ITF_OB_BANK_SWAP) == HAL_OK)
      {
        PRINTF("[INFO] Step 2: Swap bank is activated. Running from bank2.\n");
      }
    }
    else
    {
      /* Swap to bank1 */
      if (HAL_FLASH_ITF_OB_SetBankSwap(HAL_FLASH, HAL_FLASH_ITF_OB_BANK_NO_SWAP) == HAL_OK)
      {
        PRINTF("[INFO] Step 2: Swap bank is deactivated. Running from bank1.\n");
      }
    }
    if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) == HAL_OK)
    {
      /* Reset the MCU and restart from the swapped bank */
      HAL_CORTEX_NVIC_SystemReset();
    }
  }

  led_on(MX_STATUS_LED);
  HAL_Delay(EXAMPLE_WAIT_DELAY_ON_MS);
  led_off(MX_STATUS_LED);
  HAL_Delay(EXAMPLE_WAIT_DELAY_OFF_MS);


  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


/**
  * brief: Button event callback
  * param: pbutton button object pointer
  * param: arg generic argument, unused in this example
  */
static void ButtonCb(button_t *pbutton, void *arg)
{
  /* Notify the main thread about the button event */
  ButtonState = button_get_state(pbutton);
  EventFlag = 1U;
}
