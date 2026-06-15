# origin-pack: generated_STMicroelectronics::lps22hh_part_drivers@0.5.0
# file-format: 1.0.0
project(generated_STMicroelectronics_lps22hh_part_drivers_0_5_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_lps22hh_part_drivers_0_5_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_lps22hh_part_drivers_0_5_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LPS22HH#Cversion:0.11.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.lps22hh_part_drivers.0.5.0:LPS22HH CFG Condition
# description: STMicroelectronics configuration for LPS22HH part driver
set(generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_CFG_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH IO#.*Cvariant:I2C Polling(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_CFG_Condition enabled")


# condition: generated_STMicroelectronics.lps22hh_part_drivers.0.5.0:LPS22HH Core Condition
# description: STMicroelectronics LPS22HH Core Driver
set(generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_Core_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH Reg(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH IO(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_Core_Condition enabled")


# condition: generated_STMicroelectronics.lps22hh_part_drivers.0.5.0:LPS22HH Custom IO Condition
# description: 
set(generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_Custom_IO_Condition enabled")


# condition: generated_STMicroelectronics.lps22hh_part_drivers.0.5.0:LPS22HH IO I2C Polling Condition
# description: STMicroelectronics LPS22HH IO I2C Polling Driver
set(generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_IO_I2C_Polling_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:I2C(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:GPIO(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_IO_I2C_Polling_Condition enabled")


# condition: generated_STMicroelectronics.lps22hh_part_drivers.0.5.0:LPS22HH IO SPI Polling Condition
# description: STMicroelectronics LPS22HH IO SPI Polling Driver
set(generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_IO_SPI_Polling_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:SPI(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:GPIO(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_IO_SPI_Polling_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LPS22HH#Cversion:0.11.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_LPS22HH_0_11_0")
  target_compile_definitions(generated_STMicroelectronics_lps22hh_part_drivers_0_5_0 INTERFACE "$<${generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LPS22HH_0_11_0=1>")
  target_include_directories(generated_STMicroelectronics_lps22hh_part_drivers_0_5_0 INTERFACE "$<${generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_lps22hh_part_drivers_0_5_0 INTERFACE "$<${generated_STMicroelectronics.lps22hh_part_drivers.0.5.0_LPS22HH_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_lps22hh.c>")
endif()

