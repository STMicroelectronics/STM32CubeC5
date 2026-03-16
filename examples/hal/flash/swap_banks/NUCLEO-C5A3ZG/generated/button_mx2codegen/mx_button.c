/**
  ******************************************************************************
  * file           : mx_button.c
  * brief          : Code generation for the Button part driver.
  ******************************************************************************
  * attention
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

/* Includes ---------------------------------------------------------------- */
#include "mx_button.h"

static button_t  button_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t button_io_init(button_t *pio)
{
  /* Part Status */
  int32_t ret = 0;

  switch (pio->id)
  {
    case BUTTON_0:
      pio->port      = BUTTON_0_GPIO_PORT;
      pio->pin       = BUTTON_0_GPIO_PIN;
      pio->active_state = BUTTON_0_GPIO_ACTIVE_STATE;
      pio->exti_handle = BUTTON_0_EXTI_GETHANDLE();
#if defined (BUTTON_DEBOUNCE) && (BUTTON_DEBOUNCE == 1)
      pio->debounce_duration       = BUTTON_0_DEBOUNCE_DURATION;
      pio->last_event_tick       = 0u;
      if (HAL_GPIO_ReadPin(pio->port, pio->pin) == HAL_GPIO_PIN_SET)
      {
        pio->last_event = HAL_EXTI_TRIGGER_RISING;
      }
      else
      {
        pio->last_event = HAL_EXTI_TRIGGER_FALLING;
      }
#endif /* BUTTON_DEBOUNCE */
      break;


    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object Button_0.
  * @retval part object.
  */
button_t *mx_button_0_getobject(void)
{
  return &button_0_obj;
}

