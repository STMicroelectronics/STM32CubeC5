# origin-pack: generated_STMicroelectronics::w25q128j_part_drivers@0.1.0
# file-format: 1.0.0
project(generated_STMicroelectronics_w25q128j_part_drivers_0_1_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_w25q128j_part_drivers_0_1_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_w25q128j_part_drivers_0_1_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:W25Q128J#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.w25q128j_part_drivers.0.1.0:W25Q128J CFG Condition
# description: STMicroelectronics configuration for W25Q128J part driver
set(generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_CFG_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cvendor:STMicroelectronics#.*Cclass:Board Part#.*Cgroup:Memory#.*Csub:W25Q128J Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_CFG_Condition enabled")


# condition: generated_STMicroelectronics.w25q128j_part_drivers.0.1.0:W25Q128J Core Condition
# description: STMicroelectronics W25Q128J Core Driver
set(generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:SPI(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_Core_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:W25Q128J#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_W25Q128J_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_w25q128j_part_drivers_0_1_0 INTERFACE "$<${generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_W25Q128J_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_w25q128j_part_drivers_0_1_0 INTERFACE "$<${generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_w25q128j_part_drivers_0_1_0 INTERFACE "$<${generated_STMicroelectronics.w25q128j_part_drivers.0.1.0_W25Q128J_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_w25q128j.c>")
endif()

