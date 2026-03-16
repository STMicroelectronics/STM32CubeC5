<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_sign_verify_ecc_ecdsa*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to perform the signature and verification of a known message using the ECDSA algorithm over SECP256 curve.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT HAL: An alternative cryptography implementation leveraging the STM32 hardware cryptographic accelerator.

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Key Wrapping Engine (KWE): A PSA crypto driver that exposes APIs for secure cryptographic operations on STM32 internal hardware accelerators, using both symmetric and asymmetric wrapped keys.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.

By default, the example is provided using the ALT HAL implementation, when the device integrates the required hardware blocks (AES/SAES, CRYP, HASH, PKA).
It can be adapted to the KWE implementation when hardware such as CCB or SAES is available, and it can always be configured to run with the software-based implementation.


## __1. Detailed scenario__

This example demonstrates two ways of using the Cryptographic services:
   1. The signature of a known message digest using a known random, followed by the verification with the generated data.
   2. The verification of a know message digest using a true random, followed by the verification with the generated data.
This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes and configures the PSA Crypto API.

 - __Step 2__: PSA crypto initialization, computes digest, computes and verifies signature.
    This step is divided into the following sub-steps:

      Step 2.1: Computes and verifies the digest.
      Step 2.2: Computes and verifies signature using a known random.
      Step 2.3: Computes and verifies signature using a true random.

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Destroys the PSA key and clears all data.
      Step 3.2: Deinitializes the RNG instance.

 __End of example__


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA key configuration COMPLETED.
[INFO] Step 2.1: Compute and verify digest COMPLETED.
[INFO] Step 2.2: Compute and verify signature using a known random COMPLETED.
[INFO] Step 2.3: Compute and verify signature using a true random COMPLETED.
[INFO] Step 3.1: Cleanup COMPLETED.
[INFO] Step 3.2: RNG deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

Vectors examples for RSA are taken from "psa_arch_tests\api-tests\dev_apis\crypto", available at https://github.com/ARM-software/psa-arch-tests.git
NIST vectors examples for ECDSA are taken from "ECDSA Test vectors / FIPS 186-4", available at https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures

> **_NOTE:_** `Known_Signature` is modified for matching `k` used by ARM (not the same as `k` of NIST)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
