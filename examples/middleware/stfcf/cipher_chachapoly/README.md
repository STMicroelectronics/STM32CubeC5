<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_cipher_chachapoly*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to perform an authenticated encryption and a verified decryption using the Chacha20-Poly1305 algorithm.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.


## __1. Detailed scenario__

   1. Encryption a known plaintext, followed by the verification of the generated ciphertext
   2. Decryption of a known ciphertext, followed by the verification of the generated plaintext.
This example is scheduled as follows:
__Initialization phase__: At the beginning of the `main()` function, `mx_system_init()` is called to initialize all the peripherals, the Flash interface, the system clock, the systick and the instruction cache (2-ways set associative cache).

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes and configures the PSA Crypto API.

 - __Step 2__: Encrypts a known plaintext, followed by the verification of the generated ciphertext and decrypts a known ciphertext, followed by the verification of the generated plaintext.

       Step 2.1: Encrypts and verifies directly a plainText passing all the needed parameters.
       Step 2.2: Decrypts and verifies directly a ciphertext and a tag passing all the needed parameters.

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Destroys the PSA key and clears all data.
      Step 3.2: Deinitializes the RNG instance.

 __End of example__


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA key configuration COMPLETED.
[INFO] Step 2.1: Encryption and verification COMPLETED.
[INFO] Step 2.2: Decryption and verification COMPLETED.
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

> **_NOTE:_** Vectors examples for Chacha20-Poly1305 are taken from "ChaCha20 and Poly1305 for IETF Protocols", available at: https://tools.ietf.org/html/rfc8439


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
