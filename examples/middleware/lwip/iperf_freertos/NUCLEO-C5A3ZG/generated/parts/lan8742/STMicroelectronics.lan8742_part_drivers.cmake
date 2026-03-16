# origin-pack: generated_STMicroelectronics::lan8742_part_drivers@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_lan8742_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_lan8742_part_drivers_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_lan8742_part_drivers_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LAN8742#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.lan8742_part_drivers.0.0.1:LAN8742 CFG Condition
# description: STMicroelectronics configuration for LAN8742 part driver
set(generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:LAN8742#.*Csub:LAN8742 Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:LAN8742#.*Csub:LAN8742 IO#.*Cvariant:MDIO(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition enabled")


# condition: generated_STMicroelectronics.lan8742_part_drivers.0.0.1:LAN8742 Core Condition
# description: STMicroelectronics LAN8742 Core Driver
set(generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:LAN8742#.*Csub:LAN8742 IO(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_Core_Condition enabled")


# condition: generated_STMicroelectronics.lan8742_part_drivers.0.0.1:LAN8742 Custom IO Condition
# description: 
set(generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_Custom_IO_Condition enabled")


# condition: generated_STMicroelectronics.lan8742_part_drivers.0.0.1:LAN8742 IO MDIO Condition
# description: STMicroelectronics LAN8742 IO MDIO Driver
set(generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_IO_MDIO_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:ETH(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_IO_MDIO_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LAN8742#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_LAN8742_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_lan8742_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LAN8742_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_lan8742_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_lan8742_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_lan8742.c>")
  target_sources(generated_STMicroelectronics_lan8742_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.lan8742_part_drivers.0.0.1_LAN8742_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_phy1.c>")
endif()

