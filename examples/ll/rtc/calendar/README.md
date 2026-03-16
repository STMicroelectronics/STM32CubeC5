<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_rtc_calendar*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the RTC calendar through the LL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The applicative code initializes the RTC instance and configures it with the initial date and time of the calendar.

__Step 2__: Retrieve the updated time and date.

__End of example__: This example is repeated endlessly (The current time and date are updated and can be retrieved on RTCTime and RTCDate).
You can verify that the example runs properly via the status LED.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RTC__:

The RTC is configured with asynchronous and synchronous prescalers so that the calendar is updated at 1Hz (every second).
The RTC source is the LSE oscillator, when available, for a better precision (otherwise LSI).
The RTC calendar is configured in Binary Coded Decimal (BCD) mode, with a 24 hours format.
Subseconds are not used in this example.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

It is possible to connect VBAT Pin to an External Power Supply (Battery).
With a Battery Connected to the VBAT pin, the RTC configuration won't be lost when Power-On Reset happens.

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |   User Label   |
  |:---------:|:-------------:|:--------------:|
  |    PH0    |  RCC_OSC_IN   |   PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT   |
  |   PC14    | RCC_OSC32_IN  | PC14_OSC32_IN  |
  |   PC15    | RCC_OSC32_OUT | PC15_OSC32_OUT |
  |    PA5    |     GPIO      | MX_STATUS_LED  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__RTC kernel clock__: The RTC kernel clock is usually the LSE at 32.768 kHz although it is possible to select other clock sources in the RCC (refer to RCC for more details). Some functions are not available in some low-power modes or VBAT when the selected clock is not LSE.

__LSI clock__: In case LSI is used instead of LSE, prescaler should be recomputed to guaranty 1Hz calendar update.

__LL API Counting Base__: The LL API uses a sexagesimal (base-60) system for minutes and seconds, a range of 0-12 or 0-24 for hours, and a range of 0-999 for milliseconds and microseconds.

__Timeout management__: Polling flag instructions can cause the example to enter an infinite loop. To prevent this, a timeout mechanism is implemented. When the timeout is reached, the program exits the loop and reports the error at the application level. This mechanism is controlled by the USE_LL_APP_TIMEOUT compilation flag, which is disabled by default to reduce code footprint. If the example execution appears to be stuck in an infinite loop, enable this mechanism for debugging by setting USE_LL_APP_TIMEOUT to 1 in ll_example.h.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN4759](https://www.st.com/resource/en/application_note/an4759-using-the-hardware-realtime-clock-rtc-and-the-tamper-management-unit-tamp-with-stm32-microcontrollers-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) and the tamper management unit
(TAMP) with STM32 microcontrollers

- [Application Note AN3371](https://www.st.com/resource/en/application_note/an3371-using-the-hardware-realtime-clock-rtc-in-stm32-f0-f2-f3-f4-and-l1-series-of-mcus-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) in STM32F0, F2, F3, F4 and L1 series of MCUs

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
