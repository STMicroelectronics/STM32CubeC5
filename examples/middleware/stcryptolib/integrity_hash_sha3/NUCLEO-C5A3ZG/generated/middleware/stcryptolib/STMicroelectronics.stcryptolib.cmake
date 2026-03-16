# origin-pack: generated_STMicroelectronics::stcryptolib@1.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_stcryptolib_1_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_stcryptolib_1_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dcore)
  set(CMSIS_Dcore "CMSIS_Dcore not set")
  message(DEBUG "CMSIS_Dcore is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()

if(NOT DEFINED CMSIS_Tcompiler)
  set(CMSIS_Tcompiler "CMSIS_Tcompiler not set")
  message(DEBUG "CMSIS_Tcompiler is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_stcryptolib_1_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STCryptoLib#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.stcryptolib.1.0.0:ARMCC GCC IAR
# description: 
if((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR"))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_ARMCC_GCC_IAR "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_ARMCC_GCC_IAR enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_ARMCC_GCC_IAR 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:Common APIs
# description: Ensure STCryptoLib common APIs are selected before
set(generated_STMicroelectronics.stcryptolib.1.0.0_Common_APIs "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STCryptoLib#.*Csub:CMOX API(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:CRC(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_Common_APIs enabled")


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:Core condition
# description: Required dependency to HAL CRC
set(generated_STMicroelectronics.stcryptolib.1.0.0_Core_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:CRC(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_Core_condition enabled")


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:Crypto Core
# description: Ensure STCryptoLib Core was selected before
set(generated_STMicroelectronics.stcryptolib.1.0.0_Crypto_Core "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STCryptoLib#.*Csub:Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_Crypto_Core enabled")


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M0
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M0")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M0 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M0 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M0 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M3
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M3")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M3 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M3 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M3 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M33
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M33")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M33 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M33 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M33 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M4
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M4")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M4 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M4 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M4 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M7
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M7")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M7 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M7 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M7 0)
endif()


# condition: generated_STMicroelectronics.stcryptolib.1.0.0:M85
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M85")))
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M85 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stcryptolib.1.0.0_M85 enabled")
else()
  set(generated_STMicroelectronics.stcryptolib.1.0.0_M85 0)
endif()

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STCryptoLib#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Security_STM32CubeMX2_Config_STCryptoLib_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_stcryptolib_1_0_0 INTERFACE "$<${generated_STMicroelectronics.stcryptolib.1.0.0_Common_APIs}:-DCMSIS_USE_generated_Security_STM32CubeMX2_Config_STCryptoLib_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_stcryptolib_1_0_0 INTERFACE "$<${generated_STMicroelectronics.stcryptolib.1.0.0_Common_APIs}:${CMAKE_CURRENT_LIST_DIR}/.>")
endif()

