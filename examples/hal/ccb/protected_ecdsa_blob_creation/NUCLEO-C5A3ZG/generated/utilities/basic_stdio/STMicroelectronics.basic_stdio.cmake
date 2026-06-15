# origin-pack: generated_STMicroelectronics::basic_stdio@2.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_basic_stdio_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_basic_stdio_2_0_0 INTERFACE)

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
if(CMSIS_ENTIRE_generated_STMicroelectronics_basic_stdio_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:STM32CubeMX2 Config#Csub:Basic stdio#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.basic_stdio.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.basic_stdio.2.0.0 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.basic_stdio.2.0.0 enabled")
else()
  set(generated_STMicroelectronics.basic_stdio.2.0.0 0)
endif()


# condition: generated_STMicroelectronics.basic_stdio.2.0.0:Basic stdio Config
# description: selection of io interface is required to use the component
set(generated_STMicroelectronics.basic_stdio.2.0.0_Basic_stdio_Config "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:Basic stdio#.*Csub:Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:Basic stdio#.*Csub:itf_io#.*Cvariant:UART(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.basic_stdio.2.0.0_Basic_stdio_Config enabled")


# condition: generated_STMicroelectronics.basic_stdio.2.0.0:Cortex-M ITM
# description: ARM Cortex-M ITM as low level component
set(generated_STMicroelectronics.basic_stdio.2.0.0_Cortex-M_ITM "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:CMSIS#.*Cgroup:CORE(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.basic_stdio.2.0.0_Cortex-M_ITM enabled")


# condition: generated_STMicroelectronics.basic_stdio.2.0.0:STM32 HAL UART
# description: STMicroelectronics UART HAL Driver as low level component
set(generated_STMicroelectronics.basic_stdio.2.0.0_STM32_HAL_UART "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:UART(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.basic_stdio.2.0.0_STM32_HAL_UART enabled")


# condition: generated_STMicroelectronics.basic_stdio.2.0.0:interface
# description: selection of io interface is required to use the component
set(generated_STMicroelectronics.basic_stdio.2.0.0_interface "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:Basic stdio#.*Csub:itf_io#.*Cvariant:UART(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:Basic stdio#.*Csub:itf_io#.*Cvariant:ITM(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:Basic stdio#.*Csub:itf_io#.*Cvariant:Template(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.basic_stdio.2.0.0_interface enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:STM32CubeMX2 Config#Csub:Basic stdio#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Utility_STM32CubeMX2_Config_Basic_stdio_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_basic_stdio_2_0_0 INTERFACE "$<${generated_STMicroelectronics.basic_stdio.2.0.0_Basic_stdio_Config}:-DCMSIS_USE_generated_Utility_STM32CubeMX2_Config_Basic_stdio_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_basic_stdio_2_0_0 INTERFACE "$<${generated_STMicroelectronics.basic_stdio.2.0.0_Basic_stdio_Config}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_basic_stdio_2_0_0 INTERFACE "$<${generated_STMicroelectronics.basic_stdio.2.0.0_Basic_stdio_Config}:${CMAKE_CURRENT_LIST_DIR}/mx_basic_stdio_app.c>")
endif()

