/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include "lps22hh.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables */
lps22hh_object_t *pLps22hh0; /* pointer referencing the LPS22HH object instance */
float gTempData; /* This variable store the temperature measurement (unit: deg C) */
float gPressData; /* This variable store the pression measurement (unit: hPa) */

/** ########## Step 1 ##########
  * The init of LPS22HH is triggered by the applicative code
  */

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* Retrieve and store the LPS22HH object pointer */
  pLps22hh0 = MX_LPS22HH_getobject();
  /* Initialize the LPS22HH device 0 */
  if (lps22hh_drv_init(pLps22hh0, MX_LPS22HH) != 0)
  {
    PRINTF("[ERROR] Step 1: LPS22HH sensor init error\r\n");
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: LPS22HH sensor init completed\r\n");

  /* LPS22HH device 0: enable the TEMP and PRESS features */
  if (lps22hh_drv_enable(pLps22hh0) != 0)
  {
    PRINTF("[ERROR] Step 1: Enabling TEMP and PRESS features error\r\n");
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: Enabling TEMP and PRESS features completed\r\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
}

/** ########## Step 2 ##########
  * Gets the values of the temperature in Celsius and of the pressure in hPa.
  * The values are displayed on the terminal.
  * output: EXEC_STATUS_OK if OK, EXEC_STATUS_ERROR in case of error
  */

app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* LPS22HH device 0: get the temperature value and print it */
  if (lps22hh_drv_get_temperature(pLps22hh0, &gTempData) != 0)
  {
    PRINTF("[ERROR] Step 2: Reading temperature error\r\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: TEMP: %" PRIi32 " degC\n", (int32_t)gTempData);

  /* LPS22HH device 0: get the pressure value and print it */
  if (lps22hh_drv_get_pressure(pLps22hh0, &gPressData) != 0)
  {
    PRINTF("[ERROR] Step 2: Reading pressure error\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: PRESS: %" PRIi32 " hPa\n", (int32_t)gPressData);

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}

/** ########## Step 3 ##########
  * In this example, app_deinit is never called and is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  lps22hh_drv_deinit(pLps22hh0);

  return EXEC_STATUS_OK;
}
