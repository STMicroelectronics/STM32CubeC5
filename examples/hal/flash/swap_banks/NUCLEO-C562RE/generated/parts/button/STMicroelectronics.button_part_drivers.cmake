# origin-pack: generated_STMicroelectronics::button_part_drivers@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_button_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_button_part_drivers_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_button_part_drivers_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:Button#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.button_part_drivers.0.0.1:Button CFG Condition
# description: STMicroelectronics configuration for Button part driver
set(generated_STMicroelectronics.button_part_drivers.0.0.1_Button_CFG_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Button#.*Csub:gpio(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.button_part_drivers.0.0.1_Button_CFG_Condition enabled")


# condition: generated_STMicroelectronics.button_part_drivers.0.0.1:HAL GPIO
# description: STMicroelectronics GPIO HAL Driver
set(generated_STMicroelectronics.button_part_drivers.0.0.1_HAL_GPIO "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:GPIO(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.button_part_drivers.0.0.1_HAL_GPIO enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:Button#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_Button_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_button_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.button_part_drivers.0.0.1_Button_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_Button_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_button_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.button_part_drivers.0.0.1_Button_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_button_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.button_part_drivers.0.0.1_Button_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_button.c>")
endif()

