<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_opamp_standalone*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the OPAMP peripheral in standalone mode.

This example is based on the OPAMP HAL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configure the OPAMP peripheral in standalone mode.

__Step 2__: Start the OPAMP peripheral.

__End of example__: If no error occurs, the OPAMP remains started indefinitely and is available for any user application.

The example status is reported via the variable **`ExecStatus`**, and the status LED
remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the **OPAMP** peripheral configured as indicated below:

__OPAMP__ is configured in standalone mode and its speed mode is set to high.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

To use this example, connect external analog signals to the two OPAMP input pins:

- OPAMP_VINP
- OPAMP_VINM

Apply your test signal to these input pins as required by your application.

You can then measure the output signal on the output pin (OPAMP_VOUT) using an oscilloscope.

This setup allows you to verify the OPAMP functionality.

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
  |    PA1    | OPAMP1_VINP0  |      PA1      |
  |    PA6    |  OPAMP1_VOUT  |      PA6      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Voltage Range__: The OPAMP delivers a voltage between 0V and VDDA (not always 3.3 V).
Be careful when using 1.8 V boards.

__Comparator Use-Case__: Although an OPAMP can be used as a standalone comparator, it is recommended to use a COMP IP instead, as it offers more features.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5306-operational-amplifier-opamp-usage-in-stm32g4-series-stmicroelectronics.pdf)
explains further the OPAMP application for analog circuits.

You can also refer to these other examples:

- hal_opamp_calibration: demonstrates the calibration of the OPAMP.
- hal_opamp_follower: demonstrates the follower of the OPAMP.
- hal_opamp_interconnect: integrates this peripheral with DAC and ADC.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.