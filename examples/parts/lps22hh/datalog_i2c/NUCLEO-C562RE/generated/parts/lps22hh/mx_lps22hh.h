/**
  ******************************************************************************
  * file           : mx_lps22hh.h
  * brief          : Code generation for the LPS22HH part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lps22hh_license.md file
  * in the same directory as the generated code.
  * If no mx_lps22hh_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LPS22HH_H
#define MX_LPS22HH_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lps22hh.h"
#include "mx_hal_def.h"

/* Exported macros --------------------------------------------------------- */


/* Part Config ID */
#define LPS22HH_0                              0 /* Main label */
#define MX_LPS22HH                                  0 /* Extra label */

/* alias for the getter function based on the extra label */
#define MX_LPS22HH_getobject() mx_lps22hh_0_getobject()

#define LPS22HH_0_I2C_GETHANDLE mx_i2c1_i2c_gethandle
#define LPS22HH_0_I2C_ADDRESS LPS22HH_I2C_ADD_H
lps22hh_object_t *mx_lps22hh_0_getobject(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPS22HH_H */
