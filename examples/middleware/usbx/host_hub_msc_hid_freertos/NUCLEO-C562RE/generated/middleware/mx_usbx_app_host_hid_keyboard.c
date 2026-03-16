/**
  ******************************************************************************
  * @file    app_usbx_host_hid_keyboard.c
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_app_host_hid_keyboard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ULONG keyboard_key;
ULONG keyboard_state;
/* Exported variables --------------------------------------------------------*/
extern UX_HOST_CLASS_HID_KEYBOARD *keyboard;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  usbh_hid_keyboard_thread_entry
  *         Run HID task
  * @param  none
  * @retval none
  */
void usbh_hid_keyboard_thread_entry(void *args)
{
  while (1)
  {
    /* Start if the hid client is a keyboard and connected */
    if ((keyboard != NULL) && (keyboard->ux_host_class_hid_keyboard_state == (ULONG)UX_HOST_CLASS_INSTANCE_LIVE))
    {
      /* Get the keyboard key pressed */
      if (ux_host_class_hid_keyboard_key_get(keyboard, &keyboard_key, &keyboard_state) == UX_SUCCESS)
      {
        /* Print the key pressed */
        USBH_USRLOG("%c", (CHAR)keyboard_key);
      }
    }

    /* Pause execution for a while */
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
