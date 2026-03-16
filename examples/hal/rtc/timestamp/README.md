<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_rtc_timestamp*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the RTC timestamp through the HAL API.


## __1. Detailed scenario__

__Initialization phase__: At the beginning of the `main()` function, the `mx_system_init()` function is called to initialize all the peripherals, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The applicative code initializes the RTC instance, configures the calendar with the initial time 12:00:00 and date Saturday, 31 July 2026, and enable the timestamp interruption.

__Step 2__: The application writes the current time and date to the console every second.

__Step 3__: The application checks for a timestamp event.
            When a high level is applied with the user push-button, a timestamp event is detected.
            The current calendar (time and date) is saved in the timestamp structures and displayed.

__End of example__: The application runs continuously, updating and displaying the current time, date, timestamp, and datestamp on the terminal. Proper operation can be verified via the status LED or by enabling USE_TRACE. After each timestamp event the application returns to __Step 2__.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED
[INFO] Step 2: Current Time : 12: 0: 0
[INFO] Step 2: Current Date : 7-31-2026
[INFO] Step 3: Timestamp    : 12: 0: 4
[INFO] Step 3: Datestamp    : 7-31
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RTC__:

The RTC is configured to update its calendar at 1Hz (every second).
The RTC source is the LSE oscillator, when available, for a better precision (otherwise LSI).
The RTC calendar is configured in Binary Coded Decimal (BCD) mode, with a 24 hours format.
Subseconds are not used in this example.
The RTC timestamp is configured to be triggered by the user push-button pin.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup is needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA2    |   USART2_TX   |      PA2      |
  |   PC13    |    RTC_TS     |     PC13      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA2    |   USART2_TX   |      PA2      |
  |   PC13    |    RTC_TS     |     PC13      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |   User Label   |
  |:---------:|:-------------:|:--------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED  |
  |    PH0    |  RCC_OSC_IN   |   PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT   |
  |   PC14    | RCC_OSC32_IN  | PC14_OSC32_IN  |
  |   PC15    | RCC_OSC32_OUT | PC15_OSC32_OUT |
  |    PA2    |   USART2_TX   |  DBGIN_VCP_TX  |
  |   PC13    |    RTC_TS     |      PC13      |

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

__RTC Timestamp overflow__: If the RTC timestamp counter exceeds its maximum range (e.g., seconds rolling over from 59 to 0), an overflow occurs, causing the timestamp to reset unexpectedly. This can lead to incorrect time readings or missed events. To avoid this, ensure the RTC prescalers are correctly, and read the timestamp frequently enough to handle the rollover properly in your application


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN4759](https://www.st.com/resource/en/application_note/an4759-using-the-hardware-realtime-clock-rtc-and-the-tamper-management-unit-tamp-with-stm32-microcontrollers-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) and the tamper management unit
(TAMP) with STM32 microcontrollers

- [Application Note AN3371](https://www.st.com/resource/en/application_note/an3371-using-the-hardware-realtime-clock-rtc-in-stm32-f0-f2-f3-f4-and-l1-series-of-mcus-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) in STM32F0, F2, F3, F4 and L1 series of MCUs

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
