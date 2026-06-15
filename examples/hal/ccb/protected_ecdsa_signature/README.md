
<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_ccb_protected_ecdsa_signature*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


This example demonstrates how to use the STM32 HAL CCB driver to perform a protected ECDSA signature flow. The application wraps an ECDSA private key into a protected blob using the hardware XOR software key mechanism (`HAL_CCB_KEY_HSW`), signs a hash with that wrapped key, computes the associated public key from the blob, and verifies the resulting signature with the PKA peripheral.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:




__Step 1__: initializes the CCB instance.

__Step 2.1__: wraps the ECDSA P-192 CPU private key using hardware XOR software key mode (`HAL_CCB_KEY_HSW`). The generated blob contains the IV, authentication tag, and wrapped private key.

__Step 2.2__: creates the protected ECDSA signature using the wrapped private key blob.

__Step 2.3__: computes the public key associated with the wrapped private key blob.

__Step 3.1__: initializes the PKA peripheral and configures the PKA ECDSA verification operation with the computed public key, the generated signature, and the input hash.

__Step 3.2__: launches the PKA signature verification processing.

__Step 3.3__: checks that the signature verification result is valid.

__Step 4__: deinitializes the CCB instance before exit.

__End of example__: The example ends after successful P-192 private key wrapping, signature generation, public key computation, signature verification, and deinitialization. If any operation fails, the example reports an error through the status LED and the `ExecStatus` variable.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2.1: ECDSA private key wrapping COMPLETED.
[INFO] Step 2.2: ECDSA signature generation COMPLETED.
[INFO] Step 2.3: ECDSA public key computation COMPLETED.
[INFO] Step 3.1: PKA configuration to process message verification COMPLETED.
[INFO] Step 3.2: PKA Signature verification computation COMPLETED.
[INFO] Step 3.3: Signature verification COMPLETED.
[INFO] Step 4: de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


The example relies on the following peripherals:

- __CCB__: wraps the ECDSA private key using hardware XOR software key mode (`HAL_CCB_KEY_HSW`), generates the signature from the wrapped blob, and computes the public key from the same protected blob.
- __PKA__: verifies the ECDSA signature using the public key computed from the wrapped private key.


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

 - Ensure that the CCB and PKA peripherals are enabled and initialized correctly by the generated platform code.
 - In a secure or privileged system configuration, verify that peripheral access rights are consistent with the CCB and PKA usage.


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
