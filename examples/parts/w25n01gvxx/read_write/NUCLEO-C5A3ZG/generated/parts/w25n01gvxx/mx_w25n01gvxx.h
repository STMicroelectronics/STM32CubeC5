/**
  ******************************************************************************
  * file           : mx_w25n01gvxx.h
  * brief          : Code generation for the W25N01GVxx part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_w25n01gvxx_license.md file
  * in the same directory as the generated code.
  * If no mx_w25n01gvxx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_W25N01GVXX_H
#define MX_W25N01GVXX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "w25n01gvxx.h"
#include "mx_hal_def.h"

/* Exported macro ------------------------------------------------------------*/
/* Part Config ID */
#define W25N01GVXX_0                              0 /* Main label */

/* User-defined aliases for the part drivers instances IDs and getter functions */
#define MX_W25N01GVXX_1                          W25N01GVXX_0
#define mx_w25n01gvxx_1_getobject()    mx_w25n01gvxx_0_getobject()

/* Resource bindings */
#define W25N01GVXX_0_SPI_GETHANDLE    mx_spi1_gethandle

#define W25N01GVXX_0_CS_PIN           HAL_GPIO_PIN_5

#define W25N01GVXX_0_CS_PORT          HAL_GPIOB
 
 

/* Functions to retrieve the part object */
w25n01gvxx_obj_t *mx_w25n01gvxx_0_getobject(void);
 
 

#ifdef __cplusplus
}
#endif

#endif /* MX_W25N01GVXX_H */
