/**
  ******************************************************************************
  * @file           : mx_wwdg.h
  * @brief          : Header for mx_wwdg.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_WWDG_H
#define MX_WWDG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_WWDG

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for WWDG in LL layer */
/******************************************************************************/
/**
  * @brief mx_wwdg init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
WWDG_TypeDef *mx_wwdg_init(void);

/**
  * @brief   Start the WWDG. Before exiting the function, the watchdog is refreshed to have a correct time base.
  * @param   None
  * @retval  SYSTEM_OK  Operation completed successfully.
  * @retval  SYSTEM_PERIPHERAL_ERROR Operation completed with error.
  */
system_status_t mx_wwdg_start(void);
 #ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_WWDG_H */
