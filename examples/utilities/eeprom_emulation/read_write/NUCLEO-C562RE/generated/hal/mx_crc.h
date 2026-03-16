/**
  ******************************************************************************
  * file           : mx_crc.h
  * brief          : Header for mx_crc.c file.
  ******************************************************************************
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_CRC_H
#define MX_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* The user defined polynomial*/
#define CRC_POLYNOMIAL_16B 0x109B

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
  * brief crc_CFG1 init function
  * This function configures the hardware resources used in this example
  * retval pointer to handle or NULL in case of failure
  */
hal_crc_handle_t *crc_instance0_cfg1_init(void);

/**
  * brief  De-initialize crc instance and return it.
  * retval None
  */
void crc_deinit(void);

/**
  * brief  Get the CRC object.
  * retval Pointer on the CRC Handle
  */
hal_crc_handle_t *mx_crc_hal_crc_gethandle(void);


#ifdef __cplusplus
}
#endif

#endif /* MX_CRC_H */
