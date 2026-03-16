<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_cipher_chachapoly*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube Firmware package.")

How to use the STM32 Cryptographic library to perform an authenticated encryption and a verified decryption using the Chacha20-Poly1305 algorithm.

This example demonstrates two ways of using the cryptographic services:
  1. The single call method: algorithm configuration and execution is done via a single API call.
  2. The multiple calls method: algorithm configuration and execution are done in several API calls, allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided, the first is an authenticated encryption of a known plaintext, followed by the verification of the generated ciphertext. The second is an verified decryption of a known ciphertext, followed by the verification of the generated plaintext


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 At the beginning of the `main()` function, the `mx_system_init()` function is called to reset all the peripherals,
 the Flash interface, the system clock, and the systick.

   __Step 1__: Initialization phase

    Initialization of: Basic stdio, instruction cache (2-ways set associative cache) and STM32 Cryptographic library.

   __Step 2__: Demonstrates encryption and decryption of a known ciphertext using the Chacha20-Poly1305 algorithm in the single API call method.
        Step 2.1: Encrypts and verifies directly a plainText and tag using the single call method,
        Step 2.2: Decrypts and verifies directly a ciphertext and tag using the single call method.

   __Step 3__: Demonstrates encryption and decryption of a known ciphertext using the Chacha20-Poly1305 algorithm in the multiple API calls method.
        Step 3.1: Creates the PSA keys,
        Step 3.2: Encrypts and verifies directly a plainText and tag using the multiple API calls method,
        Step 3.3: Set the decryption context for Chacha20-Poly1305 algorithm COMPLETED,
        Step 3.4: Decrypts and verifies directly a ciphertext and tag using the multiple API calls method.

 - __End of example__: Deinitializes STM32 Cryptographic library before leaving the scenario.

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
- If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: ICACHE and STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Encryption and verification of a known plainText COMPLETED.
[INFO] Step 2.2: Decryption and verification of a known ciphertext COMPLETED.
[INFO] Step 3.1: Set the encryption context for Chacha20-Poly1305 algorithm COMPLETED.
[INFO] Step 3.2: Encryption and verification using multiple API calls method COMPLETED.
[INFO] Step 3.3: Set the decryption context for Chacha20-Poly1305 algorithm COMPLETED.
[INFO] Step 3.4: Decryption and verification using multiple API calls method COMPLETED..
[INFO] Step 4: STM32 Cryptographic library deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube Firmware package.")

> **_NOTE:_** Vectors examples for Chacha20-Poly1305 are taken from "ChaCha20 and Poly1305 for IETF Protocols", available at: https://tools.ietf.org/html/rfc8439


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
