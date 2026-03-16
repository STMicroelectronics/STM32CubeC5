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

uint32_t NbButtonEvent = 0;

volatile button_state_t ButtonState;
volatile uint32_t EventFlag;

button_t *pUserButton;
/* Private functions prototype -----------------------------------------------*/
static void ButtonCb(button_t *pbutton, void *arg);

app_status_t app_init(void)
{
  /** ########## Step 1 ##########
    * Initialises the LED and Button
    */

  pUserButton = mx_button_getobject();

  button_init(pUserButton, MX_BUTTON);

  PRINTF("[INFO] Step 1: Button and LED initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    * Enables Part Button interrupt and registers the callback for Button events
    */

  /* Note: registering a callback for BUTTON_EVENT_PRESSED and another for BUTTON_EVENT_UNPRESSED is not supported. */
  button_register_callback(pUserButton, ButtonCb, BUTTON_EVENT_ANY, NULL);

  /* Clear the event flag to start tracking events */
  EventFlag = 0;

  button_enableit(pUserButton);
  PRINTF("[INFO] Step 2: Press the Button:\n");

  return EXEC_STATUS_INIT_OK;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 3 ##########
    * Logs the number of button events and updates the LED
    */

  /* Wait for the button to be pressed or unpressed */
  while (EventFlag != 1);

  /* Clear the interrupt flag to detect subsequent interrupts */
  EventFlag = 0;
  NbButtonEvent++;

  /* Update the LED state to match the button state */
  if (ButtonState == BUTTON_PRESSED)
  {
    led_on(MX_STATUS_LED);
    PRINTF("[INFO] Step 3: Button event nr. %" PRIu32 ": button pressed.\n", NbButtonEvent);
  }
  else
  {
    led_off(MX_STATUS_LED);
    PRINTF("[INFO] Step 3: Button event nr. %" PRIu32 ": button released.\n", NbButtonEvent);
  }

  return EXEC_STATUS_OK;
} /* end app_process */


/** ########## End of example ##########
  * In this example, app_deinit is never called and it is provided as a reference only.
  * Disables the button interrupt and de-initializes the gpio and exti instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  button_disableit(pUserButton);

  if (mx_gpio_button_deinit() != 0)
  {
    goto _app_deinit_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */


/**
  * brief: Button event callback
  * param: pbutton button object pointer
  * param: arg generic argument, unused in this example
  */
static void ButtonCb(button_t *pbutton, void *arg)
{
  /* Notify the main thread about the button event */
  ButtonState = button_get_state(pbutton);
  EventFlag = 1;
}
