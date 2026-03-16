# origin-pack: generated_STMicroelectronics::freertos@2.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_freertos_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_freertos_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dcore)
  set(CMSIS_Dcore "CMSIS_Dcore not set")
  message(DEBUG "CMSIS_Dcore is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dsecure)
  set(CMSIS_Dsecure "CMSIS_Dsecure not set")
  message(DEBUG "CMSIS_Dsecure is not set to any value")
endif()

if(NOT DEFINED CMSIS_Tcompiler)
  set(CMSIS_Tcompiler "CMSIS_Tcompiler not set")
  message(DEBUG "CMSIS_Tcompiler is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_freertos_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:RTOS#Cgroup:STM32CubeMX2 Config#Csub:FreeRTOS#Cversion:2.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.freertos.2.0.0:CM0 GCC Condition
# description: Cortex M0 / GCC compiler
if((CMSIS_Dcore STREQUAL "Cortex-M0+" AND (CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "ARMCC")))
  set(generated_STMicroelectronics.freertos.2.0.0_CM0_GCC_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM0_GCC_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM0_GCC_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM0 IAR Condition
# description: Cortex M0 / IAR compiler
if((CMSIS_Dcore STREQUAL "Cortex-M0+" AND CMSIS_Tcompiler STREQUAL "IAR"))
  set(generated_STMicroelectronics.freertos.2.0.0_CM0_IAR_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM0_IAR_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM0_IAR_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM33 TZ Disabled GCC Condition
# description: Cortex M33 TZ Disabled / GCC compiler
if((CMSIS_Dcore STREQUAL "Cortex-M33" AND CMSIS_Dsecure STREQUAL "TZ-disabled" AND (CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "ARMCC")))
  set(generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_GCC_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_GCC_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_GCC_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM33 TZ Disabled IAR Condition
# description: Cortex M33 TZ Disabled / IAR compiler
if((CMSIS_Dcore STREQUAL "Cortex-M33" AND CMSIS_Dsecure STREQUAL "TZ-disabled" AND CMSIS_Tcompiler STREQUAL "IAR"))
  set(generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_IAR_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_IAR_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM33_TZ_Disabled_IAR_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM7 CM4 GCC Condition
# description: Cortex M7 / GCC compiler
if(((CMSIS_Dcore STREQUAL "Cortex-M7" OR CMSIS_Dcore STREQUAL "Cortex-M4") AND 
   (CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "ARMCC")))
  set(generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_GCC_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_GCC_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_GCC_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM7 CM4 IAR Condition
# description: Cortex M7 (newer than r0p1) and CM4 / IAR compiler
if(((CMSIS_Dcore STREQUAL "Cortex-M7" OR CMSIS_Dcore STREQUAL "Cortex-M4") AND 
   CMSIS_Tcompiler STREQUAL "IAR"))
  set(generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_IAR_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_IAR_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM7_CM4_IAR_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM85 GCC Condition
# description: Cortex M85 / GCC compiler
if(((CMSIS_Dsecure STREQUAL "Secure" OR CMSIS_Dsecure STREQUAL "Secure-only") AND 
   (CMSIS_Dcore STREQUAL "Cortex-M85" AND (CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "ARMCC"))))
  set(generated_STMicroelectronics.freertos.2.0.0_CM85_GCC_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM85_GCC_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM85_GCC_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:CM85 IAR Condition
# description: Cortex M85 / IAR compiler
if(((CMSIS_Dsecure STREQUAL "Secure" OR CMSIS_Dsecure STREQUAL "Secure-only") AND 
   (CMSIS_Dcore STREQUAL "Cortex-M85" AND CMSIS_Tcompiler STREQUAL "IAR")))
  set(generated_STMicroelectronics.freertos.2.0.0_CM85_IAR_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_CM85_IAR_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_CM85_IAR_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:FreeRTOS Init condition
# description: STMicroelectronics FreeRTOS Init
set(generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Init_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:MemMang#.*Cvariant:Heap_4(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Init_condition enabled")


# condition: generated_STMicroelectronics.freertos.2.0.0:FreeRTOS Memory Heap condition
# description: FreeRTOS Memory Heap condition
set(generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Memory_Heap_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:MemMang#.*Cvariant:Heap_4(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Memory_Heap_condition enabled")


# condition: generated_STMicroelectronics.freertos.2.0.0:GCC Compiler Condition
# description: Combined condition to check for GCC and AC6 compilers
if((CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "ARMCC"))
  set(generated_STMicroelectronics.freertos.2.0.0_GCC_Compiler_Condition "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_GCC_Compiler_Condition enabled")
else()
  set(generated_STMicroelectronics.freertos.2.0.0_GCC_Compiler_Condition 0)
endif()


# condition: generated_STMicroelectronics.freertos.2.0.0:Stream Buffers condition
# description: Stream Buffers condition
set(generated_STMicroelectronics.freertos.2.0.0_Stream_Buffers_condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.freertos.2.0.0_Stream_Buffers_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:RTOS#Cgroup:STM32CubeMX2 Config#Csub:FreeRTOS#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_RTOS_STM32CubeMX2_Config_FreeRTOS_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_freertos_2_0_0 INTERFACE "$<${generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Init_condition}:-DCMSIS_USE_generated_RTOS_STM32CubeMX2_Config_FreeRTOS_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_freertos_2_0_0 INTERFACE "$<${generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Init_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_freertos_2_0_0 INTERFACE "$<${generated_STMicroelectronics.freertos.2.0.0_FreeRTOS_Init_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_freertos_app.c>")
endif()

