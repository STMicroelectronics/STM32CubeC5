# origin-pack: generated_STMicroelectronics::filex@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_filex_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_filex_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_filex_0_0_1 INTERFACE "SHELL:--preinclude $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
else()
    target_compile_options(generated_STMicroelectronics_filex_0_0_1 INTERFACE "SHELL:-include $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_filex_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:STM32CubeMX2 Config#Csub:FileX#Cversion:2.1.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.filex.0.0.1:FX Core Condition
# description: FX common condition :dependency to FileX Core
set(generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:FileX#.*Csub:Core(#.*|$)>,>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:FileX#.*Csub:No_OS(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:Operating System Used
# description: 
set(generated_STMicroelectronics.filex.0.0.1_Operating_System_Used "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:FileX#.*Csub:No_OS(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_Operating_System_Used enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE FAULT TOLERANCE ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_FAULT_TOLERANCE_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_FAULT_TOLERANCE_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE MMC DMA NO OS ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_MMC_DMA_NO_OS_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_MMC_DMA_NO_OS_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE MMC DMA OS ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_MMC_DMA_OS_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_MMC_DMA_OS_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE MMC POLLING ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_MMC_POLLING_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_MMC_POLLING_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE NAND ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_NAND_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_NAND_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE NOR ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_NOR_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_NOR_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE SD DMA NO OS ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_SD_DMA_NO_OS_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_SD_DMA_NO_OS_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE SD DMA OS ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_SD_DMA_OS_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_SD_DMA_OS_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:USE SD POLLING ENV
# description: 
set(generated_STMicroelectronics.filex.0.0.1_USE_SD_POLLING_ENV "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_USE_SD_POLLING_ENV enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:filex freertos condition
# description: 
set(generated_STMicroelectronics.filex.0.0.1_filex_freertos_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:FileX#.*Csub:No_OS(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_filex_freertos_condition enabled")


# condition: generated_STMicroelectronics.filex.0.0.1:filex standalone condition
# description: 
set(generated_STMicroelectronics.filex.0.0.1_filex_standalone_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:FileX#.*Csub:No_OS(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.filex.0.0.1_filex_standalone_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:STM32CubeMX2 Config#Csub:FileX#Cversion:2.1.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_File_System_STM32CubeMX2_Config_FileX_2_1_0")
  target_compile_definitions(generated_STMicroelectronics_filex_0_0_1 INTERFACE "$<${generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition}:-DCMSIS_USE_generated_File_System_STM32CubeMX2_Config_FileX_2_1_0=1>")
  target_include_directories(generated_STMicroelectronics_filex_0_0_1 INTERFACE "$<${generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_filex_0_0_1 INTERFACE "$<${generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_filex_app.c>")
  target_sources(generated_STMicroelectronics_filex_0_0_1 INTERFACE "$<${generated_STMicroelectronics.filex.0.0.1_FX_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_filex_interfaces.c>")
endif()

