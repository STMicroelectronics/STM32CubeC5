/**
  ******************************************************************************
  * file           : mx_w25q128j.h
  * brief          : Code generation for the W25Q128J part driver.
  ******************************************************************************
  * attention
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_W25Q128J_H
#define MX_W25Q128J_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "w25q128j.h"
#include "mx_hal_def.h"

/* Exported macro ------------------------------------------------------------*/
/* Part Config ID */
#define W25Q128J_0                              0

/* User-defined aliases for the part drivers instances IDs and getter functions */
#define MX_EXTERNAL_MEM                          W25Q128J_0
#define mx_external_mem_getobject()    mx_w25q128j_0_getobject()

/* Resource bindings */
#define W25Q128J_0_SPI_GETHANDLE    mx_spi1_gethandle

#define W25Q128J_0_CS_PIN           HAL_GPIO_PIN_6

#define W25Q128J_0_CS_PORT          HAL_GPIOC



/* Functions to retrieve the part object */
w25q128j_obj_t *mx_w25q128j_0_getobject(void);



#ifdef __cplusplus
}
#endif

#endif /* MX_W25Q128J_H */
