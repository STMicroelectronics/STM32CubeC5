<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_cipher_aes_gcm*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to perform authenticated encryption/decryption with associated data (AEAD/ADAD) using the AES GCM algorithm.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT HAL: An alternative cryptography implementation leveraging the STM32 hardware cryptographic accelerator.

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Key Wrapping Engine (KWE): A PSA crypto driver that exposes APIs for secure cryptographic operations on STM32 internal hardware accelerators, using both symmetric and asymmetric wrapped keys.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.

By default, the example is provided using the ALT HAL implementation, when the device integrates the required hardware blocks (AES/SAES, CRYP, HASH, PKA).
It can be adapted to the KWE implementation when hardware such as CCB or SAES is available, and it can always be configured to run with the software-based implementation.


## __1. Detailed scenario__

This example demonstrates two ways of using the Cryptographic services:

   1. An authenticated encryption of a known plaintext, followed by the verification of the generated ciphertext.
   2. an authenticated decryption of a known ciphertext, followed by the verification of the generated plaintext.

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes and configures the PSA Crypto API.

 - __Step 2__: Encrypts and decrypts with associated data, then verifies.

      Step 2.1: Performs authenticated encryption with associated data (AEAD).
      Step 2.2: Performs authenticated decryption with associated data (ADAD).

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Destroys the PSA key and clears all data.
      Step 3.2: Deinitializes the RNG instance.

 __End of example__


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA key configuration COMPLETED.
[INFO] Step 2.1: Authenticated encryption with associated data (AEAD) COMPLETED.
[INFO] Step 2.2: Authenticated decryption with associated data (ADAD) COMPLETED.
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

> **_NOTE:_** NIST vectors examples for AES GCM are taken from:
[Recommendation for Block Cipher Modes of Operation: Galois/Counter Mode (GCM) and GMAC](https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/mac/gcmtestvectors.zip)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
