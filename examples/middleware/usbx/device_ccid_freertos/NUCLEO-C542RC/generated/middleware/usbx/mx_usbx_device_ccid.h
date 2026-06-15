/**
  ******************************************************************************
  * @file    mx_usbx_device_ccid.h
  * @brief   USBX Device CCID applicative header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_USBX_DEVICE_CCID_H
#define MX_USBX_DEVICE_CCID_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_ccid.h"
#include "mx_usbx_device.h"
#include "mx_usbx_device.h"
/* Exported types ------------------------------------------------------------*/
/* Define CCID APDU related Struct.  */
typedef struct _CCID_APDU_Command
{
  UCHAR   *buffer;    /* Command buffer of packed data.  */
  ULONG   length;     /* Command length of packed data.  */

  UCHAR   *data;      /* Extracted pointer to data bytes.  */
  UCHAR   *Le;        /* Extracted pointer to Number of data expected in response.  */

  UCHAR   CLA;        /* Extracted class byte.  */
  UCHAR   INS;        /* Extracted instruction byte.  */
  UCHAR   P1;         /* Extracted parameter 1.  */
  UCHAR   P2;         /* Extracted parameter 2.  */
  UCHAR   Lc;         /* Extracted parameter Number of data bytes send to card.  */
} CCID_APDU_Command;

typedef struct _CCID_APDU_Response
{
  UCHAR   *buffer;    /* Response buffer of packed data.  */
  ULONG   length;     /* Response buffer max length.  */
  USHORT  Le;         /* Actual response length 0~256.   */
  UCHAR   SW1;        /* Extracted status byte 1.  */
  UCHAR   SW2;        /* Extracted status byte 2.  */
} CCID_APDU_Response;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions prototypes -------------------------------------------- */
VOID usbd_ccid_activate(VOID *ccid_instance);
VOID usbd_ccid_deactivate(VOID *ccid_instance);
UINT usbd_ccid_icc_power_on(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_icc_power_off(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_get_slot_status(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_xfr_block(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_get_parameters(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_reset_parameters(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_set_parameters(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_escape(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_icc_clock(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_t0_apdu(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_secure(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_mechanical(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_abort(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);
UINT usbd_ccid_set_data_rate_and_clock_frequency(ULONG slot, UX_DEVICE_CLASS_CCID_MESSAGES *io_msg);

VOID usbd_ccid_thread_entry(VOID *args);
VOID usbd_ccid_apdu_process(CCID_APDU_Command *, CCID_APDU_Response *);
VOID usbd_ccid_apdu_read_data(CCID_APDU_Command *apdu_cmd, CCID_APDU_Response *apdu_rsp);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_DEVICE_CCID_H */
