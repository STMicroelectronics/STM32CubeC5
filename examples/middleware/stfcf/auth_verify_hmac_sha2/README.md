<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_auth_verify_hmac_sha2*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to perform the generation and the verification of a known message authentication code (MAC) of a plaintext using the HMAC SHA256 algorithm.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT HAL: An alternative cryptography implementation leveraging the STM32 hardware cryptographic accelerator.

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.

By default, the example is provided using the ALT HAL implementation, when the device integrates the required hardware blocks (AES/SAES, CRYP, HASH, PKA).


## __1. Detailed scenario__

This example demonstrates two steps of using the PSA Cryptographic services:
  1. The single call method: algorithm configuration and execution is done via a single API call.
  2. The multiple calls method: algorithm configuration and execution is done in several API calls, allowing in particular a piecemeal injection of data to process.
__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes and configures the PSA Crypto API.

 - __Step 2__: Generates and verifies a message authentication code (MAC) of a plaintext
   using the HMAC-SHA256 algorithm in two ways.

      Step 2.1: Generates and verifies a message authentication code (MAC) using single API call method.
      Step 2.2: Verifies the message with the given authentication tag using single API call method.
      Step 2.3: Generates and verifies a message authentication code (MAC) using multiple API calls method,
      Step 2.4: Verifies the message with the given authentication tag using multiple API calls method.

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Destroys the PSA key and clears all data.
      Step 3.2: Deinitializes the RNG instance.

 __End of example__


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA key configuration COMPLETED.
[INFO] Step 2.1: MAC generation and verification using single API call method COMPLETED.
[INFO] Step 2.2: MAC verification using single API call method COMPLETED.
[INFO] Step 2.3: MAC generation and verification using multiple API calls method COMPLETED.
[INFO] Step 2.4: MAC verification using multiple API calls method COMPLETED.
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

> **_NOTE:_** NIST vectors examples for HMAC are taken from:"Test Vectors", available at https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/message-authentication


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
