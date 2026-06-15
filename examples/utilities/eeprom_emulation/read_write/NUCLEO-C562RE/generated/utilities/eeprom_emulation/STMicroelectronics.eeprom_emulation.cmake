# origin-pack: generated_STMicroelectronics::eeprom_emulation@2.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_eeprom_emulation_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_eeprom_emulation_2_0_0 INTERFACE)

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
if(CMSIS_ENTIRE_generated_STMicroelectronics_eeprom_emulation_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:STM32CubeMX2 Config#Csub:EEPROM_Emulation#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.eeprom_emulation.2.0.0 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0 enabled")
else()
  set(generated_STMicroelectronics.eeprom_emulation.2.0.0 0)
endif()


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:CRC_interface
# description: condition to include a CRC component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_CRC_interface "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC template(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_CRC_interface enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:Core
# description: 
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_Core "$<OR:$<AND:$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLITF_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:algo#.*Cvariant:FLITF(#.*|$)>,>>>,$<AND:$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:NVM_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:algo#.*Cvariant:NVM(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_Core enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:ECC_interface
# description: condition to include a CRC component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_ECC_interface "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC template(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_ECC_interface enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:EEPROM_Emulation Config
# description: Selection of the EEPROM Emulation dependencies based on the selected flash type and resource allocation
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_EEPROM_Emulation_Config "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:algo#.*Cvariant:FLITF(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLITF_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_EEPROM_Emulation_Config enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:FLASH FLITF
# description: EEPROM_Emulation required package for FLITF
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_FLASH_FLITF "$<AND:$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLITF_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:algo#.*Cvariant:FLITF(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_FLASH_FLITF enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:FLASH NVM
# description: EEPROM_Emulation required package for NVM
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_FLASH_NVM "$<AND:$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_crc#.*Cvariant:CRC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_ecc#.*Cvariant:ECC template(#.*|$)>,>>>,$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:NVM_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:algo#.*Cvariant:NVM(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_FLASH_NVM enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:FLITF_interface
# description: condition to include a flitf interface component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_FLITF_interface "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLITF_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_FLITF_interface enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:NVM_interface
# description: condition to include a nvm interface component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_NVM_interface "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:NVM_EDATA(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Utility#.*Cgroup:eeprom emulation#.*Csub:itf_flash#.*Cvariant:FLASH template(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_NVM_interface enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:STM32 HAL CRC
# description: STMicroelectronics CRC HAL Driver as low level component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_CRC "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:CRC(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_CRC enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:STM32 HAL FLASH
# description: STMicroelectronics FLASH HAL Driver as low level component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_FLASH "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:FLASH(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:FLASH_ITF(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_FLASH enabled")


# condition: generated_STMicroelectronics.eeprom_emulation.2.0.0:STM32 HAL NVM
# description: STMicroelectronics FLASH HAL Driver as low level component
set(generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_NVM "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:NVM(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:NVM_ITF(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.eeprom_emulation.2.0.0_STM32_HAL_NVM enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Utility#Cgroup:STM32CubeMX2 Config#Csub:EEPROM_Emulation#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Utility_STM32CubeMX2_Config_EEPROM_Emulation_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_eeprom_emulation_2_0_0 INTERFACE "$<${generated_STMicroelectronics.eeprom_emulation.2.0.0_EEPROM_Emulation_Config}:-DCMSIS_USE_generated_Utility_STM32CubeMX2_Config_EEPROM_Emulation_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_eeprom_emulation_2_0_0 INTERFACE "$<${generated_STMicroelectronics.eeprom_emulation.2.0.0_EEPROM_Emulation_Config}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_eeprom_emulation_2_0_0 INTERFACE "$<${generated_STMicroelectronics.eeprom_emulation.2.0.0_EEPROM_Emulation_Config}:${CMAKE_CURRENT_LIST_DIR}/mx_eeprom_emulation_app.c>")
endif()

