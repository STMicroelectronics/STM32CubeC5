<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_crs_sync_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the CRS peripheral to adjust the 48MHz internal clock using the IT method.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The init of the CRS instance is triggered by the applicative code. The HSI48 clock is configured to be output on the MCO pin.

__Step 2__: Starts the CRS synchronization with LSE in interrupt mode.

__Step 3__: Wait for the end of synchronization or synchronization error.

__Step 4__: In the final step, the CRS peripheral is deinitialized.

__End of example__: After step 4, the example is completed. You can check its status via the status LED. The HSI48 frequency can be checked on the MCO pin using an oscilloscope.


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: CRS Synchronization started.
[INFO] Step 3: CRS synchronization finished.
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- *CRS*: The CRS instance is configured as:
  - Synchronization divider of 1 (default value)
  - LSE as synchronization source (LSE must be available and activated)
  - Falling polarity
  - Error limit as default
  - Trimming value as default
  - Auto trimming disabled (default value)

The reload value for the CRS is automatically calculated based on the HSI48 and LSE frequencies using the macro HAL_CRS_CALCULATE_RELOAD(ftarget, fsync), defined as:

  Reload = ((ftarget) / (fsync)) - 1

  Reload = (48000000 Hz / 32768 Hz) - 1

  Reload = 1463.8 = 0x5B7


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware dependencies for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>

 For the NUCLEO-C562RE, the HSI48 clock is referred as HSI_DIV3.

  The LSE clock drive capability is configured to medium high to provide a stronger signal and enhanced robustness.
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA9    |   RCC_MCO2    |      PA9      |
  |    PA2    |   USART2_TX   |      PA2      |

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
  |    PA9    |   RCC_MCO2    |      PA9      |
  |    PA2    |   USART2_TX   |      PA2      |

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
  |    PA9    |   RCC_MCO2    |      PA9       |
  |    PA2    |   USART2_TX   |  DBGIN_VCP_TX  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to this other example:

- usbx_device_hid_mouse_no_os: demonstrates a typical use of CRS in USB with start of frame synchronization.

- hal_crs_sync_it: same example with LL.

- hal_crs_sync_polling: demonstrates how to use CRS peripheral to adjust the 48MHz internal clock using the polling method.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
