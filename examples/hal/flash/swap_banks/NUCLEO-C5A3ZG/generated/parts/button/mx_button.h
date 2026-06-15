/**
  ******************************************************************************
  * file           : mx_button.h
  * brief          : Code generation for the Button part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_button_license.md file
  * in the same directory as the generated code.
  * If no mx_button_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef MX_BUTTON_H
#define MX_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------- */
#include "button.h"
#include "mx_hal_def.h"

/* Exported macros --------------------------------------------------------- */
/* Part Config ID */
#define BUTTON_0                              0

/* User-defined aliases for the part drivers instances IDs and getter functions */
#define MX_BUTTON                          BUTTON_0
#define mx_button_getobject()    mx_button_0_getobject()

/* Resource bindings */
#define BUTTON_0_GPIO_PORT          HAL_GPIOC
#define BUTTON_0_GPIO_PIN           HAL_GPIO_PIN_13
#define BUTTON_0_GPIO_ACTIVE_STATE  HAL_GPIO_PIN_SET

#define BUTTON_0_EXTI_GETHANDLE()     mx_gpio_default_exti13_gethandle()

#define BUTTON_0_DEBOUNCE_DURATION  1u

 


 
 


/* Exported functions ------------------------------------------------------ */
button_t *mx_button_0_getobject(void);
 
 

#ifdef __cplusplus
}
#endif

#endif /* MX_BUTTON_H */
