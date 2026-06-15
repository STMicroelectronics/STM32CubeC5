<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_ccb_protected_rsa_modular_exp_blob_creation*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates how to use the STM32 HAL CCB driver to create a protected RSA private key blob. The flow first wraps a software-provided AES key, then uses it to protect the RSA private key components (`d` and `phi`), and finally verifies protected blob outputs and CCB state.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: initializes the CCB instance.

__Step 2.1__: configures the software AES wrapping context and wraps the symmetric key.

__Step 2.2__: creates the protected RSA private key blob from clear private exponent (`d`) and Euler's totient (`phi`) values.

__Step 2.3__: verifies protected blob outputs (`IV`, `tag`, wrapped exponent, wrapped phi) and CCB state.

__Step 3__: deinitializes the CCB instance before exit.

__End of example__: The example ends after successful symmetric key wrapping, RSA private key blob creation, blob creation checks, and deinitialization. If any operation fails, the example reports an error through the status LED and the `ExecStatus` variable.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2.1: Symmetric Key wrapping COMPLETED.
[INFO] Step 2.2: RSA private key wrapping COMPLETED.
[INFO] Step 2.3: RSA private key blob creation check COMPLETED.
[INFO] Step 3: de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

The example relies on the following peripheral:

- __CCB__: wraps a software AES key, protects RSA private key components into a blob (IV, tag, wrapped exponent, wrapped phi), and validates blob creation status.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example runs on STM32 boards embedding the CCB peripheral. No external cryptographic device is required. The board only needs its standard power supply, clock source, and debug connection.

### __3.2. Specific board setups__

The example project is provided for the STM32C5 family.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Points of attention for this example:

- Ensure that the CCB peripheral is enabled and initialized correctly by the generated platform code.
- Ensure that input key material and buffers use sizes matching the configured RSA modulus length.
- In a secure or privileged system configuration, verify that CCB peripheral access rights are correctly configured.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
