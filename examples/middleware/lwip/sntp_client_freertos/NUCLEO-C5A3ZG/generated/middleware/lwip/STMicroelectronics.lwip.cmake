# origin-pack: generated_STMicroelectronics::lwip@2.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_lwip_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_lwip_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dname)
  set(CMSIS_Dname "CMSIS_Dname not set")
  message(DEBUG "CMSIS_Dname is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_lwip_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Network#Cgroup:STM32CubeMX2 Config#Csub:LwIP#Cversion:2.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.lwip.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.lwip.2.0.0 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.lwip.2.0.0 enabled")
else()
  set(generated_STMicroelectronics.lwip.2.0.0 0)
endif()


# condition: generated_STMicroelectronics.lwip.2.0.0:RTOS condition
# description: RTOS condition
set(generated_STMicroelectronics.lwip.2.0.0_RTOS_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lwip.2.0.0_RTOS_condition enabled")


# condition: generated_STMicroelectronics.lwip.2.0.0:lwip condition
# description: lwip condition
set(generated_STMicroelectronics.lwip.2.0.0_lwip_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Network#.*Cgroup:LwIP#.*Csub:Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lwip.2.0.0_lwip_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Network#Cgroup:STM32CubeMX2 Config#Csub:LwIP#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Network_STM32CubeMX2_Config_LwIP_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_lwip_2_0_0 INTERFACE "$<${generated_STMicroelectronics.lwip.2.0.0_lwip_condition}:-DCMSIS_USE_generated_Network_STM32CubeMX2_Config_LwIP_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_lwip_2_0_0 INTERFACE "$<${generated_STMicroelectronics.lwip.2.0.0_lwip_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_lwip_2_0_0 INTERFACE "$<${generated_STMicroelectronics.lwip.2.0.0_lwip_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_lwip.c>")
endif()

