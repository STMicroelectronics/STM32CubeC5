# origin-pack: generated_STMicroelectronics::stm32c5xx_hal_drivers@0.0.1
# created-by: conversion-manager@0.0.65
project(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE)

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




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE "SHELL:--preinclude ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
else()
    target_compile_options(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE "SHELL:-include ${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_EXTERNAL_ENV_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_Generated_Code_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_def_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORE_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_MPU_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_NVIC_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_GPIO_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_ICACHE_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_RCC_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_System_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_USART_0_2_0 true)
  set(CMSIS_USE_generated_Device_STM32CubeMX2_Config_ETH_0_2_0 true)
  
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1
# description: 
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1 true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1 enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:CMSIS Core
# description: STMicroelectronics STM32C5 and HAL Common
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:CMSIS_Core true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:CMSIS_Core enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL ADC
# description: HAL ADC dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_ADC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_ADC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL COMP
# description: HAL COMP dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_COMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_COMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CONF
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CONF Init
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF_Init true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF_Init enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CONF template
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF_template true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CONF_template enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORDIC
# description: HAL CORDIC dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORDIC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORDIC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE
# description: STMicroelectronics STM32C5 and HAL system
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE DMA
# description: STMicroelectronics STM32C5 and HAL with DMA
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE DMA Init
# description: STMicroelectronics HAL CORE and DMA Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE DMA Init and AES
# description: STMicroelectronics HAL AES Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_AES true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_AES enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE DMA Init and CORDIC
# description: STMicroelectronics HAL CORDIC Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_CORDIC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_CORDIC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE DMA Init and HASH
# description: STMicroelectronics HAL HASH Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_HASH true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_DMA_Init_and_HASH enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init
# description: STMicroelectronics HAL CORE, GPIO and DMA Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and ADC
# description: STMicroelectronics HAL ADC Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_ADC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_ADC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and DAC
# description: STMicroelectronics HAL DAC Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_DAC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_DAC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and I2C
# description: STMicroelectronics HAL I2C Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I2C true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I2C enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and I2S
# description: STMicroelectronics HAL I2S Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I2S true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I2S enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and I3C
# description: STMicroelectronics HAL I3C Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I3C true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_I3C enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and LPTIM
# description: STMicroelectronics HAL LPTIM Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_LPTIM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_LPTIM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and SMARTCARD
# description: STMicroelectronics HAL SMARTCARD Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_SMARTCARD true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_SMARTCARD enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and SPI
# description: STMicroelectronics HAL SPI Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_SPI true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_SPI enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and TIM
# description: STMicroelectronics HAL TIM Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_TIM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_TIM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and UART
# description: STMicroelectronics HAL UART Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_UART true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_UART enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and USART
# description: STMicroelectronics HAL USART Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_USART true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_USART enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO DMA Init and XSPI
# description: STMicroelectronics HAL SPI Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_XSPI true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_XSPI enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and COMP
# description: STMicroelectronics HAL COMP Init dependencies
if(((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_COMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_COMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and ETH
# description: STMicroelectronics HAL ETH Init dependencies
if(((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_ETH true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_ETH enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and FDCAN
# description: STMicroelectronics HAL FDCAN Init dependencies
if(((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_FDCAN true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_FDCAN enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and HCD
# description: STMicroelectronics HAL HCD Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_HCD true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_HCD enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and OPAMP
# description: STMicroelectronics HAL OPAMP Init dependencies
if(((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_OPAMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_OPAMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and PCD
# description: STMicroelectronics HAL PCD Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_PCD true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_PCD enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and RTC
# description: STMicroelectronics HAL RTC Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_RTC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_RTC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and SMBUS
# description: STMicroelectronics HAL SMBUS Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_SMBUS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_SMBUS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE GPIO Init and TAMP
# description: STMicroelectronics HAL TAMP Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_TAMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_Init_and_TAMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and CCB
# description: STMicroelectronics HAL CCB Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CCB true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CCB enabled")
endif()

# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and CCB
# description: STMicroelectronics HAL CCB Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ETH true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ETH enabled")
endif()

# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and CORTEX
# description: STMicroelectronics HAL CORTEX Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CORTEX true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CORTEX enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and CRC
# description: STMicroelectronics HAL CRC Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CRC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CRC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and CRS
# description: STMicroelectronics HAL CRS Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CRS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CRS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and FLASH
# description: STMicroelectronics HAL FLASH Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_FLASH true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_FLASH enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and ICACHE
# description: STMicroelectronics HAL ICACHE Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ICACHE true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ICACHE enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and IWDG
# description: STMicroelectronics HAL IWDG Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_IWDG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_IWDG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and PKA
# description: STMicroelectronics HAL PKA Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_PKA true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_PKA enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and RAMCFG
# description: STMicroelectronics HAL RAMCFG Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_RAMCFG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_RAMCFG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and RNG
# description: STMicroelectronics HAL RNG Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_RNG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_RNG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and SBS
# description: STMicroelectronics HAL SBS Init dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_SBS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_SBS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE Init and WWDG
# description: STMicroelectronics HAL WWDG Init dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND (((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_WWDG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_WWDG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE USB
# description: HAL CORE USB dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_USB true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_USB enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORE XSPI
# description: HAL CORE XSPI dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_XSPI true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_XSPI enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CORTEX
# description: HAL CORTEX dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORTEX true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORTEX enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CRC
# description: HAL CRC dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CRC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CRC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL CRS
# description: HAL CRS dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CRS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CRS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL Common
# description: STMicroelectronics STM32C5 and HAL common
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Common true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Common enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL DAC
# description: HAL DAC dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DAC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DAC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL DBGMCU
# description: HAL DBGMCU dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DBGMCU true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DBGMCU enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL DMA
# description: HAL DMA dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DMA true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_DMA enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL EXTI
# description: HAL EXTI dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_EXTI true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_EXTI enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL FLASH
# description: HAL FLASH dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_FLASH true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_FLASH enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL FLASH_ITF
# description: HAL FLASH_ITF dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_FLASH_ITF true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_FLASH_ITF enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL GPIO
# description: HAL GPIO dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_GPIO true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_GPIO enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL GPIO Init
# description: STMicroelectronics HAL Common Init
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_GPIO_Init true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_GPIO_Init enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL Generated code
# description: STMicroelectronics HAL components used for code Generation
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Generated_code true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Generated_code enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL I2C
# description: HAL I2C dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I2C true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I2C enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL I2S
# description: HAL I2S dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I2S true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I2S enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL I3C
# description: HAL I3C dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I3C true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_I3C enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL ICACHE
# description: HAL ICACHE dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_ICACHE true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_ICACHE enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL IWDG
# description: HAL IWDG dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_IWDG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_IWDG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL LPTIM
# description: HAL LPTIM dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_LPTIM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_LPTIM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL OPAMP
# description: HAL OPAMP dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_OPAMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_OPAMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL PKA
# description: HAL PKA dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_PKA true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_PKA enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL PWR
# description: HAL PWR dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_PWR true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_PWR enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL RAMCFG
# description: HAL RAMCFG dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RAMCFG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RAMCFG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL RCC
# description: HAL RCC dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RCC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RCC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL RNG
# description: HAL RNG dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RNG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RNG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL RTC
# description: HAL RTC dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RTC true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_RTC enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL SBS
# description: HAL SBS dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SBS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SBS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL SMARTCARD
# description: HAL SMARTCARD dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SMARTCARD true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SMARTCARD enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL SMBUS
# description: HAL SMBUS dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SMBUS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SMBUS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL SPI
# description: HAL SPI dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SPI true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_SPI enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL System Init
# description: STMicroelectronics HAL Common Init
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_System_Init true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_System_Init enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL TAMP
# description: HAL TAMP dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_TAMP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_TAMP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL TIM
# description: HAL TIM dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_TIM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_TIM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL UART
# description: HAL UART dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_UART true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_UART enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL USART
# description: HAL USART dependencies
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_USART true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_USART enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL WWDG
# description: HAL WWDG dependencies
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_WWDG true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_WWDG enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:LL
# description: STMicroelectronics STM32C5 and LL
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:LL true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:LL enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:MW FreeRTOS
# description: MW FreeRTOS used for HAL OS
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:MW_FreeRTOS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:MW_FreeRTOS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:NO OS
# description: NO MW used for HAL OS
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:NO_OS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:NO_OS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:RCC Init
# description: STMicroelectronics HAL Common Init
if((((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")) AND ((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:RCC_Init true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:RCC_Init enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:STM32C5
# description: STMicroelectronics STM32C5
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:STM32C5 true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:STM32C5 enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:TEMPLATE OS
# description: TEMPLATE OS used for HAL OS
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:TEMPLATE_OS true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:TEMPLATE_OS enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase HAL RTC ALARM
# description: HAL timebase based on RTC ALARM
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_RTC_ALARM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_RTC_ALARM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase HAL RTC WKUP
# description: HAL timebase based on RTC WKUP
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_RTC_WKUP true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_RTC_WKUP enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase HAL TIM
# description: HAL timebase based on TIM
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_TIM true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_HAL_TIM enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase Init
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_Init true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_Init enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase NONE
# description: HAL timebase NONE
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_NONE true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_NONE enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase template
# description: 
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_template true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Timebase_template enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use external env
# description: STMicroelectronics HAL condition to check if stm32_external_env.h must be generated
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use_external_env true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use_external_env enabled")


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use mutex
# description: STMicroelectronics HAL condition to check if USE_HAL_MUTEX is enabled
if(((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13") AND (CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13")))
  set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use_mutex true)
  message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:Use_mutex enabled")
endif()


# condition: generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:timebase_flavor
# description: Generate the stm32_hal_timebase.h used for the timebase
set(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:timebase_flavor true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:timebase_flavor enabled")

# Files and components in this package
if(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_EXTERNAL_ENV_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Codegen_EXTERNAL_ENV_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Generated_code)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Codegen_EXTERNAL_ENV_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_Generated_Code_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Codegen_Generated_Code_0_2_0")
  target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Codegen_Generated_Code_0_2_0=1)
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Codegen_def_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Codegen_def_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Common)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Codegen_def_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORE_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_CORE_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_Generated_code)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_CORE_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_MPU_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_CORTEX_MPU_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CORTEX)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_MPU_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_cortex_mpu.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_NVIC_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_CORTEX_NVIC_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_CORTEX)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_CORTEX_NVIC_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_cortex_nvic.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_GPIO_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_GPIO_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_GPIO_Init)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_GPIO_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_gpio_default.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_ICACHE_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_ICACHE_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ICACHE)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_ICACHE_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_icache.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_RCC_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_RCC_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:RCC_Init)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_RCC_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_rcc.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_System_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_System_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_System_Init)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_System_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_system.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_USART_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_USART_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_GPIO_DMA_Init_and_USART)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_USART_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_usart2.c)
  endif()
endif()

if(CMSIS_USE_generated_Device_STM32CubeMX2_Config_ETH_0_2_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Device_STM32CubeMX2_Config_ETH_0_2_0")
  if(generated_STMicroelectronics.stm32c5xx_hal_drivers.0.0.1:HAL_CORE_Init_and_ETH)
    target_compile_definitions(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Device_STM32CubeMX2_Config_ETH_0_2_0=1)
    target_include_directories(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE .)
    target_sources(generated_STMicroelectronics_stm32c5xx_hal_drivers_0_0_1 INTERFACE mx_eth1.c)
  endif()
endif()
