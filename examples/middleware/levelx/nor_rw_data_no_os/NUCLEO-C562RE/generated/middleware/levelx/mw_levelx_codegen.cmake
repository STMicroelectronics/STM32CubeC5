# origin-pack: generated_STMicroelectronics::levelx@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_levelx_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_levelx_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "SHELL:--preinclude $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
else()
    target_compile_options(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "SHELL:-include $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_levelx_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:STM32CubeMX2 Config#Csub:LevelX#Cversion:2.1.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.levelx.0.0.1:LX Core condition
# description: levelx core dependency
set(generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NOR Flash Core(#.*|$)>,>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NOR Flash Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Memory#.*Csub:W25Q128J Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:w25q128j_nor_dma_no_os_itf(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:No_OS(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:Operating System Used
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_Operating_System_Used "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:No_OS(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_Operating_System_Used enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX25LM51245G DMA NO OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_DMA_NO_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_DMA_NO_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX25LM51245G DMA OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_DMA_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_DMA_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX25LM51245G POLLING
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_POLLING "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX25LM51245G_POLLING enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX66UW1G45G DMA NO OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_DMA_NO_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_DMA_NO_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX66UW1G45G DMA OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_DMA_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_DMA_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE MX66UW1G45G POLLING
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_POLLING "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_MX66UW1G45G_POLLING enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25N01GVXX DMA NO OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_DMA_NO_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_DMA_NO_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25N01GVXX DMA OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_DMA_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_DMA_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25N01GVXX POLLING
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_POLLING "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25N01GVXX_POLLING enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25Q128J DMA NO OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_DMA_NO_OS "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NOR Flash Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:Memory#.*Csub:W25Q128J Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:w25q128j_nor_dma_no_os_itf(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_DMA_NO_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25Q128J DMA OS
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_DMA_OS "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_DMA_OS enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:USE W25Q128J POLLING
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_POLLING "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_USE_W25Q128J_POLLING enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:Use NAND Flash Core
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_Use_NAND_Flash_Core "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_Use_NAND_Flash_Core enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:Use NOR Flash Core
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_Use_NOR_Flash_Core "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NOR Flash Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_Use_NOR_Flash_Core enabled")


# condition: generated_STMicroelectronics.levelx.0.0.1:levelx freertos condition
# description: 
set(generated_STMicroelectronics.levelx.0.0.1_levelx_freertos_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:No_OS(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.levelx.0.0.1_levelx_freertos_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:STM32CubeMX2 Config#Csub:LevelX#Cversion:2.1.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_File_System_STM32CubeMX2_Config_LevelX_2_1_0")
  target_compile_definitions(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition}:-DCMSIS_USE_generated_File_System_STM32CubeMX2_Config_LevelX_2_1_0=1>")
  target_include_directories(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_levelx_app.c>")
  target_sources(generated_STMicroelectronics_levelx_0_0_1 INTERFACE "$<${generated_STMicroelectronics.levelx.0.0.1_LX_Core_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_levelx_interfaces.c>")
endif()

