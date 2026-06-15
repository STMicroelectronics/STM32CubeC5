
<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_ccb_protected_ecdsa_public_key_computation*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


This example demonstrates how to securely create a protected ECDSA private key blob using the STM32 HAL CCB driver. The example configures ECDSA curve parameters, wraps a private key into a protected blob using the CCB hardware key, computes the corresponding ECDSA public key from the protected blob, then compares the computed public key coordinates (Qx, Qy) against reference expected values.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:




__Step 1__: initializes the CCB instance.

__Step 2__: creates and uses a protected ECDSA blob by performing the following sub-steps:

2.1: configures ECDSA curve parameters and performs ECDSA private key wrapping using the hardware key. The wrapped private key, IV, and Tag are stored in output buffers.
2.2: computes the ECDSA public key from the protected private key blob.
2.3: compares the computed public key coordinates (PublicKeyXout_256, PublicKeyYout_256) to the expected vectors (expected_Qx, expected_Qy) provided by NIST.

__Step 3__: deinitializes the CCB instance.

__End of example__: The example completes after successful protected ECDSA blob creation, public key computation, public key comparison, and deinitialization, or indicates failure via status LED and `ExecStatus` variable.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2.1: ECDSA private key wrapping COMPLETED.
[INFO] Step 2.2: ECDSA public key computation COMPLETED.
[INFO] Step 2.3: ECDSA public key check COMPLETED.
[INFO] Step 3: de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


The CCB peripheral is used to securely manage cryptographic operations, with a focus in this example on:
 - ECDSA (Elliptic Curve Digital Signature Algorithm) private key blob creation
 - ECDSA public key computation from a protected blob
 - Public key output comparison against expected reference vectors from NIST standards.

The CCB integrates and orchestrates several auxiliary cryptographic modules:

__AES__: used for key wrapping and encryption
__RNG__: used to generate cryptographic random values (for keys)
__PKA__: used to accelerate ECDSA operations

In this example:

The CCB wraps the ECDSA private key into a protected blob using the hardware key slot and ECDSA curve parameters. The protected blob can then be used to compute the public key securely, without exposing the raw private key material.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example runs on STM32 boards supporting the CCB peripheral. Ensure the board is powered and connected as per standard STM32 hardware setup. No special connections are required for cryptographic operations.

### __3.2. Specific board setups__

Supported boards include STM32 NUCLEO, DISCO, and EVAL series with CCB support. No special pinout is required for cryptographic operations. Status LED used to indicate success or failure .

<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board pin   | MCU pin | Signal name          | ARDUINO <br/> connector pin |
  | :---:       | :---:   | :---:                | :---:        |
  | CN5-6       | PA5     | LED                  | -            |
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
