# origin-pack: generated_STMicroelectronics::open_bootloader@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_open_bootloader_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_open_bootloader_0_0_1 INTERFACE)

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
if(CMSIS_ENTIRE_generated_STMicroelectronics_open_bootloader_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:STM32CubeMX2 Config#Csub:OpenBootloader#Cversion:7.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.open_bootloader.0.0.1:OpenBL_C5
# description: Open Bootloader ready to use files on STM2C5
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.open_bootloader.0.0.1_OpenBL_C5 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_OpenBL_C5 enabled")
else()
  set(generated_STMicroelectronics.open_bootloader.0.0.1_OpenBL_C5 0)
endif()


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use CAN
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_CAN "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_CAN enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use EEPROM
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_EEPROM "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_EEPROM enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use ENGI
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_ENGI "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_ENGI enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use EXTMEM
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_EXTMEM "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_EXTMEM enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use FDCAN
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_FDCAN "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_FDCAN enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use FLASH
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_FLASH "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Flash Module#.*Csub:Flash_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_FLASH enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use I2C
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_I2C "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader I2C Module#.*Csub:I2C_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_I2C enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use I3C
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_I3C "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_I3C enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use ICP
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_ICP "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_ICP enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use IWDG
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_IWDG "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader IWDG Module#.*Csub:IWDG_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_IWDG enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use OB
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_OB "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader OB Module#.*Csub:OB_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_OB enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use OTP
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_OTP "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_OTP enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use RAM
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_RAM "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Ram Module#.*Csub:Ram_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_RAM enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use RO
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_RO "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader RO Bytes Module#.*Csub:RO_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_RO enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use SPI
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_SPI "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader SPI Module#.*Csub:SPI_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_SPI enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use USART
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_USART "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader USART Module#.*Csub:USART_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_USART enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use USB
# description: 
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_USB "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader USB Module#.*Csub:USB_Module(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_USB enabled")


# condition: generated_STMicroelectronics.open_bootloader.0.0.1:Use full features
# description: OpenBootloader global configuration :dependency to different files
set(generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader USART Module#.*Csub:USART_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader I2C Module#.*Csub:I2C_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader SPI Module#.*Csub:SPI_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader USB Module#.*Csub:USB_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Flash Module#.*Csub:Flash_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Ram Module#.*Csub:Ram_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader OB Module#.*Csub:OB_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader RO Bytes Module#.*Csub:RO_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader IWDG Module#.*Csub:IWDG_Module(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Core#.*Csub:Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Interface#.*Csub:Interface(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:STM32CubeMX2 Config#Csub:OpenBootloader#Cversion:7.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_BOOTLOADER_STM32CubeMX2_Config_OpenBootloader_7_0_0")
  target_compile_definitions(generated_STMicroelectronics_open_bootloader_0_0_1 INTERFACE "$<${generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features}:-DCMSIS_USE_generated_BOOTLOADER_STM32CubeMX2_Config_OpenBootloader_7_0_0=1>")
  target_include_directories(generated_STMicroelectronics_open_bootloader_0_0_1 INTERFACE "$<${generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_open_bootloader_0_0_1 INTERFACE "$<${generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features}:${CMAKE_CURRENT_LIST_DIR}/mx_openbl.c>")
  target_sources(generated_STMicroelectronics_open_bootloader_0_0_1 INTERFACE "$<${generated_STMicroelectronics.open_bootloader.0.0.1_Use_full_features}:${CMAKE_CURRENT_LIST_DIR}/mx_openbl_user.c>")
endif()

