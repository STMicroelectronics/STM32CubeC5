
<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_ccb_protected_edsca_blob_creation*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


This example demonstrates how to securely create a protected ECDSA (Elliptic Curve Digital Signature Algorithm) private key blob using the STM32 HAL CCB driver. A symmetric user key is used as a wrapping key to generate the ECDSA private key blob according to ECDSA curve parameters. The example then performs blob creation and verification using the protected blob. The resulting blob contains pointers to reserved output buffers for IV, Tag, and the wrapped private key, enabling secure ECDSA operations only.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:




__Step 1__: initializes the CCB instance.

__Step 2__: create and use a protected ECDSA blob by performing the following sub-steps:

2.1 configures and performs symmetric key wrapping (AES context setup and key wrapping).
2.2 performs ECDSA private key wrapping (using curve parameters and wrapping the private key).
2.3 verifies blob creation for ECDSA.

__Step 3__: deinitializes the CCB instance.

__End of example__: The example completes after successful protected ECDSA blob creation, use, and deinitialization, or indicates failure via status LED and `ExecStatus` variable.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2.1: Symmetric Key wrapping COMPLETED.
[INFO] Step 2.2: EDSCA private key wrapping COMPLETED.
[INFO] Step 2.3: Blob creation for EDSCA verification COMPLETED.
[INFO] Step 3: de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


The CCB peripheral is used to securely manage cryptographic operations, with a focus in this example on:
 - ECDSA (Elliptic Curve Digital Signature Algorithm) private key blob creation
 - Symmetric key wrapping

The CCB integrates and orchestrates several auxiliary cryptographic modules:

__AES__: used for symmetric key wrapping (encrypting the user key into a protected blob)
__RNG__: used to generate cryptographic random values (for keys)
__PKA__: used to accelerate ECDSA operations

In this example:

The CCB first wraps the user symmetric key securely using the AES engine and the selected AES algorithm. Then, the wrapped user key is utilized by both the CCB and the PKA to create an ECDSA private key blob. This blob can be securely stored or used in further secure operations without exposing the raw key material.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example runs on STM32 boards supporting the CCB peripheral. Ensure the board is powered and connected as per standard STM32 hardware setup. No special connections are required for cryptographic operations.

### __3.2. Specific board setups__

Supported boards include STM32 NUCLEO, DISCO, and EVAL series with CCB support. No special pinout is required for cryptographic operations. Status LED used to indicate success or failure .

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

 - Ensure that the RNG, AES, and PKA peripherals are properly clocked and enabled.
 - If the CCB is operating in security mode, verify that the security and privilege protection settings for the SAES, PKA, and RNG peripherals are consistent and correctly configured.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
