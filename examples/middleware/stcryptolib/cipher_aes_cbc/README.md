<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_cipher_aes_cbc*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic Library to perform encryption and decryption using the AES CBC algorithm.

This example demonstrates two ways of using the cryptographic services:

  1. The single call method: algorithm configuration and execution is done via a single API call.
  2. The multiple calls method: algorithm configuration and execution is done in several API calls,
allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided, the first is an authenticated encryption of a known plaintext, followed by the verification of the generated ciphertext. The second is an verified decryption of a known ciphertext, followed by the verification of the generated plaintext.

> **_NOTE:_** NIST vectors examples for AES CBC are taken from:
[Recommendation for Block Cipher Modes of Operation, 2001 Edition](https://csrc.nist.gov/publications/detail/sp/800-38a/final)


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 - __Step 1__: Initialization phase

    Initialization of: Trace, instruction cache (2-ways set associative cache) and STM32 Cryptographic library.

 - __Step 2__: Demonstrates single call method

        Step 2.1: Encryption of a known plaintext, followed by a verification of generated ciphertext.
        Step 2.2: Decryption of a known ciphertext, followed by a verification of the generated plaintext.

 - __Step 3__: Demonstrates multiple call method

        Step 3.1: Construction of cipher context that is configured to perform AES CBC encryption operations. Then initialize and setup of encryption key, Initialization Vector (IV), and inject them into the cipher context variable.
        Step 3.2: Encryption of plaintext in multiple steps by appending chunks of `CHUNK_SIZE` byte Each step includes a verification of data size and content.
        Step 3.3: Cleanup encryption cipher context.
        Step 3.4: Construction of cipher context that is configured to perform AES CBC decryption operations. Then initialize and setup of decryption key, Initialization Vector (IV), and inject them into cipher context variable.
        Step 3.5: Decryption the plaintext in multiple steps by appending chunks of `CHUNK_SIZE` byte Each step includes a verification of data size and content.
        Step 3.6: Cleanup decryption cipher context.

 __End of example__: Finalize STM32 Cryptographic library.

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Encryption of a known plainText COMPLETED.
[INFO] Step 2.2: Decryption of a known ciphertext COMPLETED.
[INFO] Step 3.1: Construct, initialize and setup needed data into encryption cipher context.
[INFO] Step 3.2: Encryption of plainText in multiple steps COMPLETED.
[INFO] Step 3.3: Clean-up encryption cipher context COMPLETED.
[INFO] Step 3.4: Construct, initialize and setup needed data into decryption cipher context.
[INFO] Step 3.5: Decryption of plainText in multiple steps COMPLETED.
[INFO] Step 3.6: Clean-up decryption cipher context COMPLETED.
[INFO] Step 4.0: Finalize STM32 Cryptographic library COMPLETED.
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

Here are the points of attention for this specific example:

- Verify the HAL timings according to [8.1](#81 - general - debug - tips) to have proper delays, especially [this point](https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Cube_2.0_EmbSW_WiKi#HAL_timing_issues).


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube Firmware package.")

You can find the hardware layout and configuration of a board in its user manual.
The connectors are described in this document, as well as the status LEDs and the buttons.
Example of documents available for STM32U585xx MCUs mounted on the Discovery kit for IoT node board:

- see section 4 of [STM32U585xx](https://www.st.com/resource/en/datasheet/stm32u585ai.pdf).
- see section 7 of [Discovery kit for IoT node with STM32U5 Series](https://www.st.com/resource/en/user_manual/um2839-discovery-kit-for-iot-node-with-stm32u5-series-stmicroelectronics.pdf).\
      More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).

> **_NOTE:_** NIST vectors examples for AES CBC are taken from:[Recommendation for Block Cipher Modes of Operation, 2001 Edition](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf)

> **_NOTE:_** `CMOX_AES_CBC_ENC_ALGO` refers to the default AES implementation selected in cmox_default_config.h. To use a specific implementation, user can
directly choose:
      - `CMOX_AESFAST_CBC_ENC` to select the AES fast implementation
      - `CMOX_AESSMALL_CBC_ENC` to select the AES small implementation

> **_NOTE:_** `CMOX_AES_CBC_DEC_ALGO` refers to the default AES implementation selected in cmox_default_config.h. To use a specific implementation, user can
directly choose:
      - `CMOX_AESFAST_CBC_DEC_ALGO` to select the AES fast implementation
      - `CMOX_AESSMALL_CBC_DEC_ALGO` to select the AES small implementation.


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
