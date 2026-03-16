/**
  ******************************************************************************
  * @file    app_usbx.c
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
#include "mx_usbx_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __ALIGNED(4) UCHAR ux_host_memory_buffer[USBX_HOST_MEMORY_STACK_SIZE];

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * ########## Step 2 ##########
  * Initialization of USBX stack
  *
  */
/**
  * @brief  Application USBX Initialization.
  * @param  status
  */
UINT app_usbx_init(VOID)
{
  UINT status = UX_SUCCESS;
  UCHAR *pointer;

  pointer = ux_host_memory_buffer;

  /* Initialize USBX Memory */
  status = ux_system_initialize(pointer, USBX_HOST_MEMORY_STACK_SIZE, UX_NULL, 0);

  if (status != UX_SUCCESS)
  {
    return status;
  }

  status = app_usbx_host_init();

  if (status != UX_SUCCESS)
  {
    return status;
  }

  return status;
}

/**
  * @brief  Application USBX De-Initialization.
  * @retval none
  */
UINT app_usbx_deinit(VOID)
{
  UINT ret = UX_SUCCESS;

  return ret;
}

/**
  * @brief  Application USBX Process.
  * @retval none
  */
VOID app_usbx_process(VOID)
{
  while (1)
  {
    app_usbx_host_process();
  }

}

/**
  * @brief  USB utility interrupt disable.
  * @retval none
  */
ALIGN_TYPE _ux_utility_interrupt_disable(VOID)
{
  UINT interrupt_save;

  interrupt_save = __get_PRIMASK();
  __disable_irq();

  return interrupt_save;
}

/**
  * @brief  USB utility interrupt restore.
  * @retval none
  */
VOID _ux_utility_interrupt_restore(ALIGN_TYPE flags)
{
  __set_PRIMASK(flags);
}

/**
  * @brief  Get Time Tick for host timing.
  * @retval time tick
  */
ULONG _ux_utility_time_get(VOID)
{
  return HAL_GetTick();
}
