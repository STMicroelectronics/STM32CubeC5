/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C direct polling target example with LL API
  ******************************************************************************
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

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
#define DEVICE_TARGET_DYNAMIC_ADDR       (0x32U)
#endif /* USE_LL_APP_ERROR */

/* @user Set this value to the expected Max Read Length (MRL) for the target. */
#define TARGET_CHECKED_MAX_READ_LENGTH   (0x04U)

/*  @user Set this value to the expected Max Write Length (MWL) for the target.*/
#define TARGET_CHECKED_MAX_WRITE_LENGTH  (0x04U)

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Waits until the Dynamic Address Update flag (DAUPD) is raised by the controller.
    * This step optionally checks the assigned dynamic address, then clears the DAUPD flag.
    */
  while (I3C_IsActiveFlagDynamicAddr() == 0U)
  {
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Verify that the controller assigned the expected dynamic address. */
  if (I3C_GetOwnDynamicAddress() != (uint32_t)DEVICE_TARGET_DYNAMIC_ADDR)
  {
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  I3C_ClearFlagDynamicAddr();


  /** ########## Step 3 ##########
    * Waits for the Max Read Length (MRLUPD) and Max Write Length (MWLUPD) updates sent by the controller.
    * Each received value is validated and its corresponding update flag is cleared.
    */

  /* Wait until the Max Read Length update flag is set by the controller. */
  while (I3C_IsActiveFlagMaxReadLength() == 0U)
  {
  }

  if (I3C_GetMaxReadLength() == TARGET_CHECKED_MAX_READ_LENGTH)
  {
    I3C_ClearFlagMaxReadLength();
  }
  else
  {
    goto _app_process_exit;
  }

  /* Wait until the Max Write Length update flag is set by the controller. */
  while (I3C_IsActiveFlagMaxWriteLength() == 0U)
  {
  }

  if (I3C_GetMaxWriteLength() == TARGET_CHECKED_MAX_WRITE_LENGTH)
  {
    I3C_ClearFlagMaxWriteLength();

  }
  else
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */

