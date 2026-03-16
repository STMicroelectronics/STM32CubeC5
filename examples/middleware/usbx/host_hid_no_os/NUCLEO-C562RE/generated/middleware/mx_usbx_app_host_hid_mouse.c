/**
  ******************************************************************************
  * @file    app_usbx_device_hid_mouse.c
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
#include "mx_usbx_app_host_hid_mouse.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LONG  old_Pos_x = 0;
LONG  old_Pos_y = 0;
LONG  actual_Pos_x = 0;
LONG  actual_Pos_y = 0;
ULONG actual_mouse_buttons = 0;
ULONG old_mouse_buttons = 0;
SLONG actual_mouse_wheel = 0;
SLONG old_mouse_wheel = 0;
/* Exported variables --------------------------------------------------------*/
extern UX_HOST_CLASS_HID_MOUSE *mouse;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  usbh_hid_mouse_process
  *         Run HID task
  * @param  none
  * @retval none
  */
VOID usbh_hid_mouse_process(VOID)
{

  /* Start if the hid client is a mouse and connected */
  if ((mouse != NULL) && (mouse -> ux_host_class_hid_mouse_state == (ULONG) UX_HOST_CLASS_INSTANCE_LIVE))
  {
    /* Get Mouse position */
    if (ux_host_class_hid_mouse_position_get(mouse, &actual_Pos_x, &actual_Pos_y) == UX_SUCCESS)
    {
      if (((actual_Pos_x != old_Pos_x) || (actual_Pos_y != old_Pos_y)) && (actual_Pos_x != 0) && (actual_Pos_y != 0))
      {
        USBH_USRLOG("Pos_x = %ld Pos_y= %ld", actual_Pos_x, actual_Pos_y);

        /* Update (x,y) old position */
        old_Pos_x = actual_Pos_x;
        old_Pos_y = actual_Pos_y;
      }
    }

    /* Get Mouse buttons value */
    if (ux_host_class_hid_mouse_buttons_get(mouse, &actual_mouse_buttons) == UX_SUCCESS)
    {
      if (actual_mouse_buttons != old_mouse_buttons)
      {
        /* Check which button is pressed */
        if (actual_mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_1_PRESSED)
        {
          USBH_USRLOG("Left Button Pressed ");
        }

        if (actual_mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_2_PRESSED)
        {
          USBH_USRLOG("Right Button Pressed ");
        }

        if (actual_mouse_buttons & UX_HOST_CLASS_HID_MOUSE_BUTTON_3_PRESSED)
        {
          USBH_USRLOG("Middle Button Pressed ");
        }

        /* Update button old value */
        old_mouse_buttons = actual_mouse_buttons;
      }
    }

    /* Get hid wheel mouse position */
    if (ux_host_class_hid_mouse_wheel_get(mouse, &actual_mouse_wheel) == UX_SUCCESS)
    {
      if ((actual_mouse_wheel != old_mouse_wheel) && (actual_mouse_wheel != 0))
      {
        USBH_USRLOG("Pos_wheel = %ld ", actual_mouse_wheel);

        /* Update wheel mouse movement value */
        old_mouse_wheel = actual_mouse_wheel;
      }
    }
  }
}

