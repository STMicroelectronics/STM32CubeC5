<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_lptim_input_capture*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This example shows how to use the LPTIM peripheral in input capture mode to measure the frequency of an external signal in low power mode using the LSE as a counter clock, through the HAL LPTIM API.


## __1. Detailed scenario__

The external signal is received on LPTIM input pin and is routed to a capture/compare register CCRx.
The SystemClock_Config() function is used to configure the maximum system clock frequency.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The applicative code initializes the LPTIM instance in Input Capture mode, with an external signal connected to the LPTIM channel, and explicitly disables the DMA half-transfer interrupt. This is required to prevent the system from waking up before the DMA transfer is fully completed.

__Step 2__: The device goes in stop mode. When the input signal is active, the DMA starts the transfer data from CCR1 register to DmaCapturedValue buffer (of the first period only).

__Step 3__: Upon the full completion of the transfer, the DMA generates an interrupt to wake up the MCU. Calculate the measured input signal frequencies then compare to expected value.

__End of example__: If the expected and measured input signal frequencies match, the status LED is turned on.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: The device goes in stop mode, waiting for wakeup from the DMA complete transfer.
[INFO] Step 3: The transfer completes and the DMA generates an interrupt to wake up the MCU.
[INFO] Step 3: The expected and measured input signal frequencies match.
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__LPTIM__: The LPTIM is configured with these specific parameters:

  - Internal clock source
  - Continuous mode
  - Input capture mode
  - Rising edge polarity

The *LPTIM* may need additional clock configuration to be able to function in low-power mode.

- The RCC is configured to keep the LPTIM internal clock while in Stop mode.
- When the STM32 series supports it, the RCC is configured in autonomous mode to allow the LPTIM to wake up from Stop mode.

- The DMA is configured with these specific parameters:

  - Peripheral to memory direction
  - Source address fixed
  - Destination address incremented
  - Direct transfer

The *LPDMA* may need additional clock configuration to be able to function in low-power mode.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup is needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>Common configuration.</summary>

 The expected input signal frequency is 100Hz.
</details>
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
  |   PB13    |  LPTIM1_CH1   |     PB13      |

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
  |   PB13    |  LPTIM1_CH1   |     PB13      |

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
  |   PB13    |  LPTIM1_CH1   |      PB13      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Clock accuracy__: The LPTIM may use the LSI clock as input clock. If used, the accuracy of this one can impact the real timeout value.
The application needs to ensure that the SysTick time base is always set to 1 millisecond
to have correct HAL operation.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/bd/16/1d/53/4a/ef/4e/0e/DM00290631/files/DM00290631.pdf/jcr:content/translations/en.DM00290631.pdf)
explains common LPTIM usages, including timeout.

You can also refer to these other examples:

- hal_lptim_timeout.
- hal_lptim_pulse_counter.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2022 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
