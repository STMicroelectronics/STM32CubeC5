<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_lptim_timeout*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to implement a low power timeout to wake-up the system using the Low-Power Timer (LPTIM), through the HAL LPTIM API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the LPTIM instance. Registers the callback for the compare match interruption and starts the LPTIM in interrupt mode.

__Step 2__: The device goes in stop mode and waits for an interrupt: ETR or Timeout. The first ETR edge raises an interruption and starts the timeout. Any successive ETR resets the timeout. If no trigger appears during the programmed timeout, CompareMatch interruption will occur.

__End of example__: If no error occurs, the device enters in stop mode indefinitely and each time the timer reaches the timeout value, the status LED is toggled.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__LPTIM__: The LPTIM is configured with these specific parameters:

  - internal clock source
  - timeout mode
  - period counter value should be greater than the pulse value

The LPTIM uses the following features:

  - external trigger input on rising and falling edge to use a GPIO as trigger to reset the LPTIM counter. It allows to firstly start the timeout, and secondly reset the counter and thus prevent the compare match.
  - pulse value is set to have a 1s timeout

The *LPTIM* may need additional clock configuration to be able to function in low-power mode.

- The RCC is configured to keep the LPTIM internal clock while in Stop mode.

  <details>
    <summary>Pulse calculation details</summary>

      Timeout = CMP / LPTIM_CLK
      CMP = Timeout * LPTIM_CLK

  </details>


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

Please find below the hardware setup principle that applies to any board.

<!--
@startuml
@startditaa{doc/example_hal_lptim_timeout-setup.png}

                            +------------------------------------------------------+
                            | MCU                             +----------------+   |
                            |        +-----------+            | STM32 LPTIMi   |   |
                            |        | RCC       |            |                |   |
                            |        |       LSI +---------+->* Internal Clock |   |
                            |        +-----------+            |                |   |
  +------------------+      |                                 |                |   |
  | External Trigger +---+->*------------------------------+->* LPTIM_ETR pin  |   |
  +------------------+      |                                 +----------------+   |
                            |                                                      |
                            +------------------------------------------------------+


@endditaa
@enduml
-->

![hal_lptim_timeout-setup](doc/example_hal_lptim_timeout-setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->
<details>
  <summary>On STM32C5 series.</summary>

 **Compare register numerical application**:

  The LPTIM is clocked by the LSI which is equal to 32 kHz for the STM32C5 series. The purpose is to get a 1s timeout, so:

    CMP = Timeout * LPTIM_CLK
    CMP = 1 s * 32000 Hz
    CMP = 32000
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PA15    |  LPTIM1_ETR   |   NetR16_2    |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PA15    |  LPTIM1_ETR   |   NetR16_2    |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |   PA15    |  LPTIM1_ETR   |   NetR53_2    |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Interruption management__: The interruption due to the compare match is not the only one to appear. The external trigger interruption also occurs and wakes the device up.

__Clock after Stop mode__: When exiting from STOP mode, the system clock must be reconfigured (see the RCC peripheral section in the reference manual of your MCU).

__Systick interruption__: Any peripheral interrupt occurring when the AHB/APB clocks are present (if peripheral vector enabled in the NVIC) can wake up the system from STOP mode (not only EXTI). That is the reason why the SysTick interrupt is switched off before entering Stop mode.

__Clock accuracy__: The LPTIM may use the LSI clock as input clock. If used, the accuracy of this one can impact the real timeout value.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/bd/16/1d/53/4a/ef/4e/0e/DM00290631/files/DM00290631.pdf/jcr:content/translations/en.DM00290631.pdf)
explains common LPTIM usages, including timeout.

You can also refer to this other example:

- hal_pwr_stop0: demonstrates the STOP0 mode

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
