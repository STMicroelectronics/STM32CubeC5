<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_rtc_alarm*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the RTC alarm through the HAL API.


## __1. Detailed scenario__

At startup, the backup domain is reset to avoid reusing any previous RTC calendar and alarm.

So, the RTC peripheral registers and the RTC clock source (LSE or LSI) are in their reset states whatever was programmed before.

This is implemented in the function `pre_system_init_hook()`, which is called before the LSE or the LSI is properly configured in `system_clock_config()`.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The applicative code initializes the RTC instance.
            It configures the initial date and time of the calendar:  2hour 20min 0sec, Monday 7 December 20.
            The alarm is configured to expire 30 seconds later: 2hour 20min 30sec, Monday 7 December.

__Step 2__: Starts the RTC alarm A in interrupt mode.

__Step 3__: Waits for the RTC alarm interrupt.

__End of example__: After step 3, the example is completed.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RTC__:

The RTC is configured with asynchronous and synchronous prescalers so that the calendar is updated at 1Hz (every second).
The RTC source is the LSE oscillator, when available, for a better precision (otherwise LSI).
The RTC calendar is configured in Binary Coded Decimal (BCD) mode, with a 24 hours format.
The RTC alarm A is configured in interrupt mode.
Subseconds are not used in this example.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup needed for this example.

### __3.2. Specific board setups__

<details>
<summary>On STM32C5 series</summary>
<details>
  <summary>On board NUCLEO-C542RC</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO <br> connector pin |
  | :---:             | :---:   | :---:           | :---:                      |
  | CN5-6             | PA5     | ARDUINO LED LD1 | D13                        |

</details>

<details>
  <summary>On board NUCLEO-C562RE</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO <br> connector pin |
  | :---:             | :---:   | :---:           | :---:                      |
  | CN5-6             | PA5     | ARDUINO LED LD1 | D13                        |

</details>

<details>
  <summary>On board NUCLEO-C5A3ZG</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO <br> connector pin |
  | :---:             | :---:   | :---:           | :---:                      |
  | CN5-6             | PA5     | ARDUINO LED LD1 | D13                        |

</details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__RTC kernel clock__: The RTC kernel clock is usually the LSE at 32.768 kHz although it is possible to select other clock sources in the RCC (refer to RCC for more details). Some functions are not available in some low-power modes or VBAT when the selected clock is not LSE.

__LSI clock__: In case LSI is used instead of LSE, prescaler should be recomputed to guaranty 1Hz calendar update.

__HAL API Counting Base__: The HAL API uses a sexagesimal (base-60) system for minutes and seconds, a range of 0-12 or 0-24 for hours, and a range of 0-999 for milliseconds and microseconds.
```c
   rtc_time.min = 20U;  /* 20 minutes */
```   


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
