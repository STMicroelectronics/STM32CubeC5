<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_cipher_aes_gcm*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic library to perform
authenticated encryption/decryption with associated data (AEAD/ADAD) using the AES GCM algorithm.

This example demonstrates two ways of using the STM32 Cryptographic library services:

  1. The single call method: algorithm configuration and execution are done via a single API call.
  2. The multiple calls method: algorithm configuration and execution are done in several API calls,
allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided, the first is an authenticated encryption of a known plaintext, followed by the verification of the generated ciphertext. The second is an verified decryption of a known ciphertext, followed by the verification of the generated plaintext.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 __Step 1__: Initializes STM32 Cryptographic library.

 __Step 2__: Demonstrates a single call method.

Step 2.1: Authenticates encryption of a known plaintext, followed by a verification of the generated ciphertext.

Step 2.2: Verifies decryption of a known ciphertext, followed by a verification of the generated plaintext.

 __Step 3__: Demonstrates a multiple calls method.

Step 3.1: Sets the cipher context that is configured to perform AES GCM encryption operations.
Then initialization and setup of authentication tag length, encryption key, and Initialization Vector (IV).
Afterwards, injection of all this additional authenticated data into the cipher context variable.

Step 3.2: Authenticates encryption of plaintext in multiple steps by appending chunks of `CHUNK_SIZE` byte. Each step includes a verification of data size and content.

Step 3.3: Verifies the authentication tag and cleans up the encryption cipher context.

Step 3.4: Sets the cipher context that is configured to perform AES GCM decryption operations.
Then initialization and setup of authentication tag length, decryption key, and Initialization Vector (IV). Afterwards, injection of all this additional authenticated data into the cipher context variable.

Step 3.5: Verifies decryption the plaintext in multiple steps by appending chunks of `CHUNK_SIZE` byte. Each step includes a verification of data size and content.

Step 3.6: Verifies the authentication tag and cleans up the decryption cipher context.

 __Step 4.0__: Deinitializes STM32 Cryptographic library before leaving the scenario.

 __End of example__

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.0: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Authenticates Encryption of a known PlainText COMPLETED.
[INFO] Step 2.2: Verifies Decryption with a known ciphertext COMPLETED.
[INFO] Step 3.1: Constructs, initializes and setups needed data into encryption cipher context COMPLETED.
[INFO] Step 3.2: Authenticates Encryption of plainText in multiple steps COMPLETED.
[INFO] Step 3.3: Verifies authentication tag and cleans up encryption cipher context COMPLETED.
[INFO] Step 3.4: Constructs, initializes and setups needed data into decryption cipher context COMPLETED.
[INFO] Step 3.5: Verifies Decryption of PlainText in multiple steps COMPLETED.
[INFO] Step 3.6: Verifies authentication tag and cleans up decryption cipher context COMPLETED.
[INFO] Step 4.0: Finalizes STM32 Cryptographic library COMPLETED.
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
