# origin-pack: STMicroelectronics::stcryptolib@2.0.0
# file-format: 1.0.0
project(STMicroelectronics_stcryptolib_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(STMicroelectronics_stcryptolib_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dcore)
  set(CMSIS_Dcore "CMSIS_Dcore not set")
  message(DEBUG "CMSIS_Dcore is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dname)
  set(CMSIS_Dname "CMSIS_Dname not set")
  message(DEBUG "CMSIS_Dname is not set to any value")
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




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "SHELL:--preinclude ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
else()
    target_compile_options(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "SHELL:-include ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_stcryptolib_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STCryptoLib#Csub:Core#Cversion:4.0.4")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STCryptoLib#Csub:CMOX API#Cversion:4.0.4")
endif()

# All conditions used by this package

# condition: STMicroelectronics.stcryptolib.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(STMicroelectronics.stcryptolib.2.0.0 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:ARMCC GCC IAR
# description: 
if((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR"))
  set(STMicroelectronics.stcryptolib.2.0.0_ARMCC_GCC_IAR "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_ARMCC_GCC_IAR enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_ARMCC_GCC_IAR 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:Common APIs
# description: Ensure STCryptoLib common APIs are selected before
set(STMicroelectronics.stcryptolib.2.0.0_Common_APIs "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STCryptoLib#.*Csub:CMOX API(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_Common_APIs enabled")


# condition: STMicroelectronics.stcryptolib.2.0.0:Core condition
# description: Required dependency to HAL CRC
set(STMicroelectronics.stcryptolib.2.0.0_Core_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:CRC(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_Core_condition enabled")


# condition: STMicroelectronics.stcryptolib.2.0.0:Crypto Core
# description: Ensure STCryptoLib Core was selected before
set(STMicroelectronics.stcryptolib.2.0.0_Crypto_Core "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STCryptoLib#.*Csub:Core(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_Crypto_Core enabled")


# condition: STMicroelectronics.stcryptolib.2.0.0:M0
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M0")))
  set(STMicroelectronics.stcryptolib.2.0.0_M0 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M0 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M0 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:M3
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M3")))
  set(STMicroelectronics.stcryptolib.2.0.0_M3 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M3 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M3 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:M33
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M33")))
  set(STMicroelectronics.stcryptolib.2.0.0_M33 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M33 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M33 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:M4
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M4")))
  set(STMicroelectronics.stcryptolib.2.0.0_M4 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M4 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M4 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:M7
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M7")))
  set(STMicroelectronics.stcryptolib.2.0.0_M7 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M7 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M7 0)
endif()


# condition: STMicroelectronics.stcryptolib.2.0.0:M85
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M85")))
  set(STMicroelectronics.stcryptolib.2.0.0_M85 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.stcryptolib.2.0.0_M85 enabled")
else()
  set(STMicroelectronics.stcryptolib.2.0.0_M85 0)
endif()

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STCryptoLib#Csub:Core#Cversion:4.0.4" IN_LIST CMSIS_COMPONENTS_LIST)  # STM32 Cryptographic library according to the Cortex-M architecture
  message(DEBUG "Using component Security_STCryptoLib_Core_4_0_4")
  target_compile_definitions(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Core_condition}:-DCMSIS_USE_Security_STCryptoLib_Core_4_0_4=1>")
  if(EXISTS ${CMSIS_RTE_FOLDER}/Security/cmox_low_level.c)
    target_sources(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Core_condition}:${CMSIS_RTE_FOLDER}/Security/cmox_low_level.c>")
  else()
    target_sources(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Core_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/cmox_crc/cmox_low_level.c>")
  endif()
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M0}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM0_CM0PLUS.a>")
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M3}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM3.a>")
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M33}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM33.a>")
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M4}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM4.a>")
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M7}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM7.a>")
  target_link_libraries(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.stcryptolib.2.0.0_Core_condition},${STMicroelectronics.stcryptolib.2.0.0_M85}>:${CMAKE_CURRENT_LIST_DIR}/lib/libSTM32Cryptographic_CM85.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STCryptoLib#Csub:CMOX API#Cversion:4.0.4" IN_LIST CMSIS_COMPONENTS_LIST)  # Common STCryptoLib APIs
  message(DEBUG "Using component Security_STCryptoLib_CMOX_API_4_0_4")
  target_compile_definitions(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:-DCMSIS_USE_Security_STCryptoLib_CMOX_API_4_0_4=1>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/cipher>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/cipher/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/drbg>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/drbg/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/ecc>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/ecc/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/hash>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/hash/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/mac>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/mac/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/rsa>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/rsa/>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/utils>")
  target_include_directories(STMicroelectronics_stcryptolib_2_0_0 INTERFACE "$<${STMicroelectronics.stcryptolib.2.0.0_Crypto_Core}:${CMAKE_CURRENT_LIST_DIR}/include/utils/>")
endif()

