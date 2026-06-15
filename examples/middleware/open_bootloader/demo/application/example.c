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
#include "openbl_conf.h"
#include "mx_openbl.h"
#if defined (OPENBL_ITF_USB_ENABLE)
#include "mx_usbx_app.h"
#endif /* OPENBL_ITF_USB_ENABLE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the Open Bootloader instance and any other MW instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

#if defined (OPENBL_ITF_USB_ENABLE)
  if (app_usbx_init() != UX_SUCCESS)
  {
    return_status = EXEC_STATUS_ERROR;
  }
#endif /* OPENBL_ITF_USB_ENABLE */

  if (mx_openbl_init() != OPENBL_SUCCESS)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
}


/** ########## Step 2 ##########
  * Start Open Bootloader protocols detection process.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
#if defined (OPENBL_ITF_USB_ENABLE)
  uint32_t detected_itf;
#endif /* OPENBL_ITF_USB_ENABLE */

  while (return_status == EXEC_STATUS_OK)
  {
    /* Status OK : LED remains turned on */
    led_on(MX_STATUS_LED);

#if defined (OPENBL_ITF_USB_ENABLE)
    detected_itf = mx_openbl_get_detected_itf();

    if (detected_itf == OPENBL_ITF_TYPE_USB)
    {
      app_usbx_process();
    }
    else
#endif /* OPENBL_ITF_USB_ENABLE */
    {
      if (mx_openbl_process() != OPENBL_SUCCESS)
      {
        return_status = EXEC_STATUS_ERROR;
      }
    }
  }

  /* This point is only reached in case of error */

  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * De-Initializes Open Bootloader application.
  */
app_status_t app_deinit(void)
{
  /* Code must not reach this part as the de-init is only performed when jump command is used */

  return EXEC_STATUS_OK;
}
