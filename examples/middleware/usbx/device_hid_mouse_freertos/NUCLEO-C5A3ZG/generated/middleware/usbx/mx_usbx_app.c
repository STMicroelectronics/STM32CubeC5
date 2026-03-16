/**
  ******************************************************************************
  * @file    mx_usbx_app.c
  * @brief   USBX applicative source file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbx_license.md file
  * in the same directory as the generated code.
  * If no mx_usbx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_app.h"
#include "mx_usbx_device.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __ALIGNED(4) UCHAR ux_system_memory_buffer[APP_DEFAULT_STACK_SIZE];
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Application USBX Initialization.
  * @param  status
  */
UINT app_usbx_init(VOID)
{
  /**
    * ########## Step 2 ##########
    * Initialization of USBX stack
    *
    */
  UINT status = UX_SUCCESS;
  UCHAR *pointer;

  pointer = ux_system_memory_buffer;

  /* Initialize USBX Memory */
  status = ux_system_initialize(pointer, APP_DEFAULT_STACK_SIZE, UX_NULL, 0);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  status = app_usbx_device_init();

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
