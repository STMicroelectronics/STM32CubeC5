# origin-pack: generated_STMicroelectronics::led_part_drivers@0.0.1
# created-by: conversion-manager@0.0.65
project(generated_STMicroelectronics_led_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.20)
add_library(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_led_part_drivers_0_0_1)
  set(CMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LED_GPIO_0_3_0 true)
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.led_part_drivers.0.0.1:HAL GPIO
# description: STMicroelectronics LED GPIO HAL Driver
set(generated_STMicroelectronics.led_part_drivers.0.0.1:HAL_GPIO true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.led_part_drivers.0.0.1:HAL_GPIO enabled")


# condition: generated_STMicroelectronics.led_part_drivers.0.0.1:HAL PWM
# description: STMicroelectronics LED PWM HAL Driver
set(generated_STMicroelectronics.led_part_drivers.0.0.1:HAL_PWM true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.led_part_drivers.0.0.1:HAL_PWM enabled")


# condition: generated_STMicroelectronics.led_part_drivers.0.0.1:LED GPIO CFG Condition
# description: STMicroelectronics configuration for LED part driver
set(generated_STMicroelectronics.led_part_drivers.0.0.1:LED_GPIO_CFG_Condition true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.led_part_drivers.0.0.1:LED_GPIO_CFG_Condition enabled")


# condition: generated_STMicroelectronics.led_part_drivers.0.0.1:Part MFX
# description: STMicroelectronics LED IOE Driver
set(generated_STMicroelectronics.led_part_drivers.0.0.1:Part_MFX true)
message(DEBUG "CMSIS condition generated_STMicroelectronics.led_part_drivers.0.0.1:Part_MFX enabled")

# Files and components in this package
if(CMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LED_GPIO_0_3_0)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_LED_GPIO_0_3_0")
  if(generated_STMicroelectronics.led_part_drivers.0.0.1:LED_GPIO_CFG_Condition)
    target_compile_definitions(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE -DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LED_GPIO_0_3_0=1)
    target_include_directories(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE .)
  endif()
endif()

