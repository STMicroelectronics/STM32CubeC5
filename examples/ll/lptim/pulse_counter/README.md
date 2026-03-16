<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_lptim_pulse_counter*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the LPTIM (Low-Power Timer) to count external pulses through LL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the LPTIM instance and starts the LPTIM in interrupt mode.

__Step 2__: The device goes in low power mode and waits for an interrupt.

__Step 3__: Each time the LPTIM counter reaches the number of pulses to be counted, an interrupt is generated and wakes the MCU up.

__End of example__: If no error occurs, the device enters in low power mode indefinitely and each time the timer counts the defined number of pulses, the status LED is toggled.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__LPTIM__: The LPTIM is configured with these specific parameters:

  - external synchronous clock
  - continuous mode
  - period to 1000

The *LPTIM* is configured to count 1000 pulses on its input. Its internal clock is configured to be the LSI. To avoid missing any events, the frequency of changes on the external Input1 signal should never exceed the frequency of the internal clock provided to LPTIM.

The *LPTIM* may need additional clock configuration to be able to function in low-power mode.

- The RCC is configured to keep the LPTIM internal clock while in low power mode.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/example_ll_lptim_pulse_counter-setup.png}
                                     +------------------------------------------------------------+
                                     |                                                            |
                                     |  +------------------------------------------------------+  |
                                     |  | MCU                             +----------------+   |  |
                                     |  |        +-----------+            | STM32 LPTIMi   |   |  |
                                     |  |        | RCC       |            |                |   |  |
                                     |  |        |       LSI +---------+->* Internal Clock |   |  |
                                     |  |        +-----------+            |                |   |  |
  +--------------------------+       |  |                                 |                |   |  |
  | External Pulse Generator +----+->+->*------------------------------+->* Input Pin      |   |  |
  +--------------------------+       |  | GPIO                            +----------------+   |  |
                                     |  |                                                      |  |
                                     |  +------------------------------------------------------+  |
                                     |                                                            |
                                     | STM32_Board                                                |
                                     +------------------------------------------------------------+
@endditaa
@enduml
-->

![ll_lptim_pulse_counter-setup](doc/example_ll_lptim_pulse_counter-setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA1    |  LPTIM1_IN1   |      PA1      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA1    |  LPTIM1_IN1   |      PA1      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |   PB10    |  LPTIM1_IN1   |     PB10      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__External Input Limitation__: In this example, the LPTIM clock source is the LSI, so the external input is sampled with the LSI clock. If the external input has a higher frequency than the LSI, events might be missed and pulse counter will be incorrect.

__Source Clock__: The LPTIM instance is clocked by internal synchronous clock (LSI).

__Clock after Stop mode__: When exiting from low power mode, the system clock must be reconfigured (see the RCC peripheral section in the reference manual of your MCU).

__Systick interruption__: Any peripheral interrupt occurring when the AHB/APB clocks are present (if peripheral vector enabled in the NVIC) can wake up the system from low power mode (not only EXTI).

__Timeout management__: Polling flag instructions can cause the example to enter an infinite loop. To prevent this, a timeout mechanism is implemented. When the timeout is reached, the program exits the loop and reports the error at the application level. This mechanism is controlled by the USE_LL_APP_TIMEOUT compilation flag, which is disabled by default to reduce code footprint. If the example execution appears to be stuck in an infinite loop, enable this mechanism for debugging by setting USE_LL_APP_TIMEOUT to 1 in ll_example.h.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/bd/16/1d/53/4a/ef/4e/0e/DM00290631/files/DM00290631.pdf/jcr:content/translations/en.DM00290631.pdf)
explains common LPTIM usages, including asynchronous pulse counter.

You can also refer to this other example:

- ll_pwr_stop1: demonstrates the STOP1 mode

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
