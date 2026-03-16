# origin-pack: generated_STMicroelectronics::usbx@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_usbx_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_usbx_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "SHELL:--preinclude ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
else()
    target_compile_options(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "SHELL:-include ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_usbx_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:Device_Core#Cversion:2.0.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:HID_Mouse_Device_Class#Cversion:2.0.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Config#Csub:USBX#Cversion:2.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.usbx.0.0.1:Operating System Used
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Operating_System_Used "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX#.*Csub:No_OS(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Operating_System_Used enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Audio Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Audio_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Audio_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Audio Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Audio_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Audio_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use CCID Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_CCID_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_CCID_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use CDC_ACM Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_CDC_ACM_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_CDC_ACM_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use CDC_ACM Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_CDC_ACM_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_CDC_ACM_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use DFU Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_DFU_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_DFU_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Device Controllers
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Device_Controllers "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Controllers Driver#.*Csub:Device(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Device_Controllers enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Device Core
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Device_Core "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Core#.*Csub:Device(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Device Descriptors Driver#.*Csub:Device(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:Device_Core(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Device_Core enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use GSER Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_GSER_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_GSER_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Custom Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Custom_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Custom_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Device_Class "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Device Class#.*Csub:HID(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Keyboard Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Keyboard_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Keyboard_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Keyboard Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Keyboard_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Keyboard_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Mouse Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Device_Class "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:HID_Mouse_Device_Class(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID Mouse Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HID RCU Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HID_RCU_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HID_RCU_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use HUB Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_HUB_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_HUB_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Host Controllers
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Host_Controllers "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Host_Controllers enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Host Core
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Host_Core "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Host_Core enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use MSC Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_MSC_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_MSC_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use MSC Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_MSC_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_MSC_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use PIMA_MTP Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_PIMA_MTP_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_PIMA_MTP_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use PIMA_MTP Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_PIMA_MTP_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_PIMA_MTP_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use PROLIFIC Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_PROLIFIC_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_PROLIFIC_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Printer Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Printer_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Printer_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Printer Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Printer_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Printer_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use SWAR Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_SWAR_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_SWAR_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Video Device Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Video_Device_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Video_Device_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:Use Video Host Class
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_Use_Video_Host_Class "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_Use_Video_Host_Class enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:usbx core condition
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_usbx_core_condition "$<AND:$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX#.*Csub:No_OS(#.*|$)>,>>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Core#.*Csub:Device(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Device Descriptors Driver#.*Csub:Device(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:Device_Core(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Controllers Driver#.*Csub:Device(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Device Class#.*Csub:HID(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:HID_Mouse_Device_Class(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX Core#.*Csub:Common(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_usbx_core_condition enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:usbx freertos condition
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_usbx_freertos_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX#.*Csub:No_OS(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_usbx_freertos_condition enabled")


# condition: generated_STMicroelectronics.usbx.0.0.1:usbx standalone condition
# description: 
set(generated_STMicroelectronics.usbx.0.0.1_usbx_standalone_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBX#.*Csub:No_OS(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbx.0.0.1_usbx_standalone_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:Device_Core#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Codegen_Device_Core_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_Device_Core}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Codegen_Device_Core_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_Device_Core}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_Device_Core}:${CMAKE_CURRENT_LIST_DIR}/mx_usbx_device.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:HID_Mouse_Device_Class#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Codegen_HID_Mouse_Device_Class_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Device_Class}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Codegen_HID_Mouse_Device_Class_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Device_Class}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_Use_HID_Mouse_Device_Class}:${CMAKE_CURRENT_LIST_DIR}/mx_usbx_device_hid_mouse.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Config#Csub:USBX#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Config_USBX_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_usbx_core_condition}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Config_USBX_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_usbx_core_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_usbx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbx.0.0.1_usbx_core_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_usbx_app.c>")
endif()

