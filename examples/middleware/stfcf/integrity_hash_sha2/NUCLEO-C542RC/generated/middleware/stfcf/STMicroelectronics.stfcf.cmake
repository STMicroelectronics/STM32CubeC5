# origin-pack: generated_STMicroelectronics::stfcf@1.0.0
# file-format: 1.0.0
project(generated_STMicroelectronics_stfcf_1_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_stfcf_1_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_stfcf_1_0_0 INTERFACE "SHELL:--preinclude ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
else()
    target_compile_options(generated_STMicroelectronics_stfcf_1_0_0 INTERFACE "SHELL:-include ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_stfcf_1_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STFCF#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.stfcf.1.0.0: alt chachapoly condition
# description: Enable chachapoly_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0__alt_chachapoly_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0__alt_chachapoly_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:KWE condition
# description: Requires CCB HAL driver component
set(generated_STMicroelectronics.stfcf.1.0.0_KWE_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:CCB(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_KWE_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:Mbed TLS Alt HAL condition
# description: Mbed TLS Alt HAL condition
set(generated_STMicroelectronics.stfcf.1.0.0_Mbed_TLS_Alt_HAL_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:RNG(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_Mbed_TLS_Alt_HAL_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:Mbed TLS Alt STCryptoLib condition
# description: Requires STCryptoLib config component
set(generated_STMicroelectronics.stfcf.1.0.0_Mbed_TLS_Alt_STCryptoLib_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_Mbed_TLS_Alt_STCryptoLib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:PSA interface conditions
# description: Ensure PSA API are satisfied
set(generated_STMicroelectronics.stfcf.1.0.0_PSA_interface_conditions "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:MbedTLS#.*Csub:PSA API(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_PSA_interface_conditions enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:STFCF config condition
# description: STFCF possible implementations
set(generated_STMicroelectronics.stfcf.1.0.0_STFCF_config_condition "$<AND:$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:RNG(#.*|$)>,>>>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:Core(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_STFCF_config_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt aes condition
# description: Enable aes_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_aes_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_aes_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt ccm condition
# description: Enable ccm_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_ccm_condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_ccm_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt cmac condition
# description: Enable cmac_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_cmac_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_cmac_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt drbg condition
# description: Enable drbg_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_drbg_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_drbg_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt ecp condition
# description: Enable ecp_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_ecp_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_ecp_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt entropy_hw condition
# description: Enable entropy_hw_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_entropy_hw_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_entropy_hw_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt gcm condition
# description: Enable aes_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_gcm_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_gcm_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt rsa condition
# description: Enable rsa_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_rsa_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_rsa_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt sha1 condition
# description: Enable sha1_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_sha1_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_sha1_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt sha256 condition
# description: Enable sha256_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_sha256_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_sha256_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.0.0:alt sha512 condition
# description: Enable sha512_alt implementation
set(generated_STMicroelectronics.stfcf.1.0.0_alt_sha512_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.0.0_alt_sha512_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STFCF#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Security_STM32CubeMX2_Config_STFCF_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_stfcf_1_0_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.0.0_STFCF_config_condition}:-DCMSIS_USE_generated_Security_STM32CubeMX2_Config_STFCF_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_0_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.0.0_STFCF_config_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
endif()

