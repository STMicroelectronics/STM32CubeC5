# origin-pack: STMicroelectronics::lps22hh_part_drivers@1.0.0
# file-format: 1.0.0
project(STMicroelectronics_lps22hh_part_drivers_1_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_lps22hh_part_drivers_1_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH Reg#Cversion:3.0.2")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH Core#Cversion:0.8.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:I2C Polling#Cversion:0.2.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:SPI Polling#Cversion:0.2.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:Custom#Cversion:0.2.0")
endif()

# All conditions used by this package

# condition: STMicroelectronics.lps22hh_part_drivers.1.0.0:LPS22HH CFG Condition
# description: STMicroelectronics configuration for LPS22HH part driver
set(STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_CFG_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_CFG_Condition enabled")


# condition: STMicroelectronics.lps22hh_part_drivers.1.0.0:LPS22HH Core Condition
# description: STMicroelectronics LPS22HH Core Driver
set(STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Core_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH Reg(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Sensor#.*Csub:LPS22HH IO(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Core_Condition enabled")


# condition: STMicroelectronics.lps22hh_part_drivers.1.0.0:LPS22HH Custom IO Condition
# description: 
set(STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition enabled")


# condition: STMicroelectronics.lps22hh_part_drivers.1.0.0:LPS22HH IO I2C Polling Condition
# description: STMicroelectronics LPS22HH IO I2C Polling Driver
set(STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_I2C_Polling_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:I2C(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:GPIO(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_I2C_Polling_Condition enabled")


# condition: STMicroelectronics.lps22hh_part_drivers.1.0.0:LPS22HH IO SPI Polling Condition
# description: STMicroelectronics LPS22HH IO SPI Polling Driver
set(STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_SPI_Polling_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:SPI(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:GPIO(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_SPI_Polling_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH Reg#Cversion:3.0.2" IN_LIST CMSIS_COMPONENTS_LIST)  # https://github.com/STMicroelectronics/lps22hh-pid/releases/tag/v3.0.2
  message(DEBUG "Using component Board_Part_Sensor_LPS22HH_Reg_3_0_2")
  target_compile_definitions(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "-DCMSIS_USE_Board_Part_Sensor_LPS22HH_Reg_3_0_2=1")
  target_include_directories(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/regs")
  target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/regs/lps22hh_reg.c")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH Core#Cversion:0.8.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Driver for LPS22HH MEMS sensor
  message(DEBUG "Using component Board_Part_Sensor_LPS22HH_Core_0_8_0")
  target_compile_definitions(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Core_Condition}:-DCMSIS_USE_Board_Part_Sensor_LPS22HH_Core_0_8_0=1>")
  target_include_directories(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/lps22hh.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:I2C Polling#Cversion:0.2.0" IN_LIST CMSIS_COMPONENTS_LIST)  # I2C IO in polling mode for LPS22HH MEMS sensor
  message(DEBUG "Using component Board_Part_Sensor_LPS22HH_IO_I2C_Polling_0_2_0")
  target_compile_definitions(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_I2C_Polling_Condition}:-DCMSIS_USE_Board_Part_Sensor_LPS22HH_IO_I2C_Polling_0_2_0=1>")
  target_include_directories(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_I2C_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/i2c>")
  target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_I2C_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/i2c/lps22hh_io.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:SPI Polling#Cversion:0.2.0" IN_LIST CMSIS_COMPONENTS_LIST)  # SPI IO in polling mode for LPS22HH MEMS sensor
  message(DEBUG "Using component Board_Part_Sensor_LPS22HH_IO_SPI_Polling_0_2_0")
  target_compile_definitions(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_SPI_Polling_Condition}:-DCMSIS_USE_Board_Part_Sensor_LPS22HH_IO_SPI_Polling_0_2_0=1>")
  target_include_directories(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_SPI_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/spi>")
  target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_IO_SPI_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/spi/lps22hh_io.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:Sensor#Csub:LPS22HH IO#Cvariant:Custom#Cversion:0.2.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Custom I/O layer for LPS22HH IO
  message(DEBUG "Using component Board_Part_Sensor_LPS22HH_IO_Custom_0_2_0")
  target_compile_definitions(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition}:-DCMSIS_USE_Board_Part_Sensor_LPS22HH_IO_Custom_0_2_0=1>")
  target_include_directories(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom>")
  if(EXISTS ${CMSIS_RTE_FOLDER}/Board Part/lps22hh_io.c)
    target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition}:${CMSIS_RTE_FOLDER}/Board Part/lps22hh_io.c>")
  else()
    target_sources(STMicroelectronics_lps22hh_part_drivers_1_0_0 INTERFACE "$<${STMicroelectronics.lps22hh_part_drivers.1.0.0_LPS22HH_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom/lps22hh_io.c>")
  endif()
endif()

