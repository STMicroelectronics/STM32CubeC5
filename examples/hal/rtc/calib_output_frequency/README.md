<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_rtc_calib_output_frequency*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the RTC calibration output feature through the HAL API. This example demonstrates the synchronous calibration clock (1 Hz) routed on RTC Output.


## __1. Detailed scenario__

__Initialization phase__: At the beginning of the main() function, the `mx_system_init()` function is called to initialize all the peripherals, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The application initializes the RTC, enables the 1 Hz calibration output, and routes it to the board's RTC Output pin.

__End of example__: Enter the main loop and no further processing. The RTC autonomously drives a stable 1 Hz square wave on RTC Output. No user interaction required.

Use an oscilloscope or logic analyzer on the RTC Output pin. You should observe a period of 1.0 s (~50% duty cycle). Optionally measure frequency accuracy to check RTC main clock and precisely adjust it.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED
[INFO] 1 Hz calibration clock available on RTC Output pin (square wave).
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RTC__:

The RTC uses the LSE as its clock source to ensure stability. Prescalers are set so the calendar tick is 1 Hz. The calibration output feature is enabled in synchronous mode to generate a 1 Hz square wave on Calibration Output 1 routed to the board-specific RTC Output pin.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

Connect oscilloscope probe:

- Probe tip: RTC Output pin
- Probe ground: Board ground (e.g. CNx GND)

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
  |   PC13    |   RTC_OUT1    |     PC13      |

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
  |   PC13    |   RTC_OUT1    |     PC13      |

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
  |   PC13    |   RTC_OUT1    |      PC13      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Note: If the RTC Output pin shares a LED or user button, disconnect or avoid using it during measurement to preserve waveform integrity.


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
