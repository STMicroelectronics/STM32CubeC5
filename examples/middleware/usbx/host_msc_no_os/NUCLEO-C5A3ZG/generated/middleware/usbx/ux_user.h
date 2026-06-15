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
#define UX_STANDALONE
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
/* Define USBX Host Enum Thread Stack Size.*/
/* #define UX_HOST_ENUM_THREAD_STACK_SIZE 2048 */

/* Define USBX Host HCD Thread Stack Size. */
/* #define UX_HOST_HCD_THREAD_STACK_SIZE 2048 */

/* Define USBX Host HNP Polling Thread Stack Size.*/
/* #define UX_HOST_HNP_POLLING_THREAD_STACK 2048 */

/* Defined, this value controls host configuration instance creation, include all
   interfaces and endpoints physical resources.
   Possible settings:
    UX_HOST_STACK_CONFIGURATION_INSTANCE_CREATE_ALL (0) - The default, create all inside configuration.
    UX_HOST_STACK_CONFIGURATION_INSTANCE_CREATE_OWNED (1) - Create things owned by class driver.
   Not defined, default setting is applied.
 */
/* #define UX_HOST_STACK_CONFIGURATION_INSTANCE_CREATE_CONTROL UX_HOST_STACK_CONFIGURATION_INSTANCE_CREATE_OWNED */

/* Defined, this value will only enable the host side of usbx.  */
#define UX_HOST_SIDE_ONLY

/* Defined, this enables the assert checks inside usbx.  */
#define UX_ENABLE_ASSERT

/* This is the Thread priority value for the host controller thread */
/* #define UX_THREAD_PRIORITY_HCD 20 */

/* Defined, this value is the maximum number of classes that can be loaded by USBX. This value
   represents the class container and not the number of instances of a class. For instance, if a
   particular implementation of USBX needs the hub class, the printer class, and the storage
   class, then the ux_max_classes value can be set to 3 regardless of the number of devices
   that belong to these classes.  */
/* #define UX_MAX_CLASSES 2 */

/* UX_MAX_TT */
/* #define UX_MAX_TT 8 */

/* Defined, this macro enables host device class code validation.
   Only following USB-IF allowed device class code is allowed:
   0x00, 0x02 (CDC Control), 0x09 (Hub), 0x11 (Billboard), 0xDC (Diagnostic), 0xEF (MISC), 0xFF (Vendor)
   Refer to https://www.usb.org/defined-class-codes for more details.
 */
/* #define UX_HOST_DEVICE_CLASS_CODE_VALIDATION_ENABLE */

/* Defined, this value represents the maximum number of devices that can be attached to the USB.
   Normally, the theoretical maximum number on a single USB is 127 devices. This value can be
   scaled down to conserve memory. Note that this value represents the total number of devices
   regardless of the number of USB buses in the system.  */

/* #define UX_MAX_DEVICES 127 */
/* Defined, this value represents the maximum number of Ed, regular TDs and Isochronous TDs. These values
   depend on the type of host controller and can be reduced in memory constrained environments.  */
/* #define UX_MAX_ED 80 */
/* #define UX_MAX_TD 128 */
/* #define UX_MAX_ISO_TD 1 */
/* Defined, this value represents the number of different host controllers available in the system.
   For USB 1.1 support, this value will usually be 1. For USB 2.0 support, this value can be more
   than 1. This value represents the number of concurrent host controllers running at the same time.
   If, for instance, there are two instances of OHCI running, or one EHCI and one OHCI controller
   running, UX_MAX_HCD should be set to 2.
   for STM32 MCUs same host controller addresses USB 1.1 and USB 2.0 support so UX_MAX_HCD can be set to 1. */
/* #define UX_MAX_HCD 2 */
/* Defined, this value represents the maximum number of bytes that a storage payload can send/receive.
   The default is 8K bytes but can be reduced in memory constrained environments.  */
/* #define UX_HOST_CLASS_STORAGE_MEMORY_BUFFER_SIZE (1024 * 8) */

/* Define USBX Mass Storage Thread Stack Size. The default is to use ux_thread_stack_size. */
/* #define UX_HOST_CLASS_STORAGE_THREAD_STACK_SIZE (1024 * 2) */

/* Defined, This value represents the priority of storage class thread.  */
/* #define UX_OS_HOST_CLASS_STORAGE_THREAD_PRIORITY_CLASS 5 */

/* Defined, this value represents the maximum number of SCSI logical units represented in the
   host storage class driver.  */
/* #define UX_MAX_HOST_LUN 1 */

/* Defined, this value represents the maximum number of media for the host storage class.
   Default is 8 but for memory constrained resource systems this can ne reduced to 1. */
/* #define UX_HOST_CLASS_STORAGE_MAX_MEDIA 2 */

/* Defined, this value includes code to handle storage devices that use the CB
   or CBI protocol (such as floppy disks). It is off by default because these
   protocols are obsolete, being superseded by the Bulk Only Transport (BOT) protocol
   which virtually all modern storage devices use.
*/
/* #define UX_HOST_CLASS_STORAGE_INCLUDE_LEGACY_PROTOCOL_SUPPORT */

/* Defined, this value represents the maximum size of single transfers for the SCSI data phase.
   By default it's 1024.
*/
/* #define UX_HOST_CLASS_STORAGE_MAX_TRANSFER_SIZE (1024 * 1)  */

/* Defined, this macro will remove the FileX dependency of host storage.
   In this mode, sector access is offered instead of directly FileX FX_MEDIA support.
   Use following APIs for media obtain and access:
   - ux_host_class_storage_media_get : get instance of UX_HOST_CLASS_STORAGE_MEDIA
   - ux_host_class_storage_media_lock : lock specific media for further read/write
   - ux_host_class_storage_media_read : read sectors on locked media
   - ux_host_class_storage_media_write : write sectors on locked media
   - ux_host_class_storage_media_unlock : unlock media
   Note it's forced defined/enabled in standalone mode of usbx.
*/
/* #define UX_HOST_CLASS_STORAGE_NO_FILEX  */
#ifdef __cplusplus
}
#endif
#endif /* UX_USER_H */
