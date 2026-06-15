/***************************************************************************
  * Copyright (c) 2025 Microsoft Corporation
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
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

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   User Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */
/*                                                                        */
/*    ux_user.h                                           PORTABLE C      */
/*                                                           6.5.0        */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains user defines for configuring USBX in specific    */
/*    ways. This file will have an effect only if the application and     */
/*    USBX library are built with UX_INCLUDE_USER_DEFINE_FILE defined.    */
/*    Note that all the defines in this file may also be made on the      */
/*    command line when building USBX library and application objects.    */
/*                                                                        */
/**************************************************************************/

#ifndef UX_USER_H
#define UX_USER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Define various build options for the USBX port.  The application should either make changes
   here by commenting or un-commenting the conditional compilation defined OR supply the defines
   though the compiler's equivalent of the -D option.  */
/* Defined, this macro will enable the standalone mode of usbx.  */
/* #define UX_STANDALONE */
/* This is the thread priority value for the USBX enumeration threads that monitor
   the bus topology.  */
/* #define UX_THREAD_PRIORITY_ENUM 5 */

/* This is the thread priority value for the USBX class threads that monitor
   the bus topology.  */
/* #define UX_THREAD_PRIORITY_CLASS 5 */

/* Define USBX Generic Thread Stack Size.  */
/* #define UX_THREAD_STACK_SIZE (2 * 1024) */

/* Defined, this option enables the basic USBX error checking. This define is typically used
   when the application is debugging and removed after the application is fully debugged.  */
/* #define UX_ENABLE_ERROR_CHECKING */

/* Defined, this enables function parameters checking. This define is typically used
   when the application is debugging and removed after the application is fully debugged.  */
#define UX_ENABLE_PARAM_CHECKING

/* This value actually defines the time slice that will be used for threads. */
/* #define UX_NO_TIME_SLICE (0ul) */

/* Defined, this value represents how many ticks per seconds for a specific hardware platform.
   The default is 1000 indicating 1 tick per millisecond.  */
/* #define UX_PERIODIC_RATE 100 */

/* Define control transfer timeout value in millisecond.
   The default is 10000 milliseconds.  */
/* #define UX_CONTROL_TRANSFER_TIMEOUT 10000 */

/* Define non control transfer timeout value in millisecond.
   The default is 50000 milliseconds.  */
/* #define UX_NON_CONTROL_TRANSFER_TIMEOUT 50000 */

/* Defined, this value forces the memory allocation scheme to enforce alignment
   of memory with the UX_SAFE_ALIGN field.
*/
/* #define UX_ENFORCE_SAFE_ALIGNMENT */

/* Defined, this value represents the size of the log pool.
*/
/* #define UX_DEBUG_LOG_SIZE (16 * 1024) */
/* Defined, this value represents minimal allocated memory alignment in number of bytes.
   The default is UX_ALIGN_8 (0x07) to align allocated memory to 8 bytes.  */
/* #define UX_ALIGN_MIN UX_ALIGN_8 */

/* Defined, the _name in structs are referenced by pointer instead of by contents.
   By default the _name is an array of string that saves characters, the contents are compared to confirm match.
   If referenced by pointer the address pointer to const string is saved, the pointers are compared to confirm match.
 */
/* #define UX_NAME_REFERENCED_BY_POINTER */
/* Defined, this value is the maximum number of classes in the device stack that can be loaded by
   USBX. */
/* #define UX_MAX_SLAVE_CLASS_DRIVER 2 */

/* Defined, this value is the maximum number of interfaces in the device framework.  */
/* #define UX_MAX_SLAVE_INTERFACES 16 */

/* Defined, this value represents the current number of SCSI logical units represented in the device
   storage class driver.  */
/* #define UX_MAX_SLAVE_LUN 1 */

/* Defined, this value represents the maximum number of bytes received on a control endpoint in
   the device stack. The default is 256 bytes but can be reduced in memory constrained environments.  */
/* #define UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH 256 */

/* Defined, this value represents the maximum number of bytes that can be received or transmitted
   on any endpoint. This value cannot be less than the maximum packet size of any endpoint. The default
   is 4096 bytes but can be reduced in memory constrained environments. For cd-rom support in the storage
   class, this value cannot be less than 2048.  */
/* #define UX_SLAVE_REQUEST_DATA_MAX_LENGTH 2048 */

/* Defined, this value will only enable the device side of usbx.  */
#define UX_DEVICE_SIDE_ONLY

/* This is the ThreadX priority value for the device controller thread.  */
/* #define UX_THREAD_PRIORITY_DCD 20 */

/* Works if ux_device_initialize_framework_scan_disable is defined.
   This value represents max number of endpoints (except EP0) activated at the same time.
 */
/* #define UX_MAX_DEVICE_ENDPOINTS 6 */

/* Works if ux_device_initialize_framework_scan_disable is defined.
   This value represents max number of interfaces activated at the same time.
 */
/* #define UX_MAX_DEVICE_INTERFACES 6 */

/* Defined, this macro enables device bi-directional-endpoint support.  */
#define UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT

/* Defined, this value represents the endpoint buffer owner.
   0 - The default, endpoint buffer is managed by core stack. Each endpoint takes ux_slave_request_data_max_length bytes.
   1 - Endpoint buffer managed by classes. In this case not all endpoints consume ux_slave_request_data_max_length bytes.
*/
/* #define UX_DEVICE_ENDPOINT_BUFFER_OWNER 0 */

/* Defined, this enables processing of Get String Descriptor requests with zero Language ID.
   The first language ID in the language ID framework will be used if the request has a zero
   Language ID.  */
/* #define UX_DEVICE_ENABLE_GET_STRING_WITH_ZERO_LANGUAGE_ID */

/* Defined, this macro disables interface alternate setting support.
   Device stalls
 */
/* #define UX_DEVICE_ALTERNATE_SETTING_SUPPORT_DISABLE */

/* Defined, this macro disables device framework scan, where max number of endpoints (except EP0)
   and max number of interfaces are calculated at runtime, as a base to allocate memory for
   interfaces and endpoints structures and their buffers.
   Undefined, the following two macros must be defined to initialize memory structures.
 */
/* #define UX_DEVICE_INITIALIZE_FRAMEWORK_SCAN_DISABLE */
/* Defined, this macro disables CDC ACM non-blocking transmission support.  */
#define UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE

/* Defined, class _write is pending ZLP automatically (complete transfer) after buffer is sent.  */
/* #define UX_DEVICE_CLASS_CDC_ACM_WRITE_AUTO_ZLP */

/* Defined, it enables device CDC ACM zero copy for bulk in/out endpoints (write/read).
    Enabled, the endpoint buffer is not allocated in class, application must
    provide the buffer for read/write, and the buffer must meet device controller driver (DCD)
    buffer requirements (e.g., aligned and cache safe).
    It only works if ux_device_endpoint_buffer_owner is 1 (endpoint buffer managed by class).
 */
/* #define UX_DEVICE_CLASS_CDC_ACM_ZERO_COPY */
#ifdef __cplusplus
}
#endif
#endif /* UX_USER_H */
