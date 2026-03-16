<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_cipher_aes_cbc*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to perform encryption and decryption using the AES CBC algorithm.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT HAL: An alternative cryptography implementation leveraging the STM32 hardware cryptographic accelerator.

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Key Wrapping Engine (KWE): A PSA crypto driver that exposes APIs for secure cryptographic operations on STM32 internal hardware accelerators, using both symmetric and asymmetric wrapped keys.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.

By default, the example is provided using the ALT HAL implementation, when the device integrates the required hardware blocks (AES/SAES, CRYP, HASH, PKA).
It can be adapted to the KWE implementation when hardware such as CCB or SAES is available, and it can always be configured to run with the software-based implementation.


## __1. Detailed scenario__

This example demonstrates two steps of using the PSA Cryptographic services:

  1. An encryption of a known plaintext, followed by the verification of the generated ciphertext.
  2. An decryption of a known ciphertext, followed by the verification of the generated plaintext.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes and configures the PSA Crypto API.

__Step 2__: Encryption and decryption using the AES CBC algorithm.

      Step 2.1: Performs an encryption and a decryption using single API call method.
      Step 2.2: Performs an encryption and a decryption using multiple API calls method, allowing in particular a piecemeal injection of data to process.

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Destroys the PSA key and clears all data.
      Step 3.2: Deinitializes the RNG instance.

__End of example__:


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```"USE_TRACE"
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA key configuration COMPLETED.
[INFO] Step 2.2: AES CBC encryption and decryption using single API call method COMPLETED.
[INFO] Step 2.3: AES CBC encryption and decryption using multiple API calls method COMPLETED.
[INFO] Step 3.1: Cleanup COMPLETED.
[INFO] Step 3.2: RNG deinitialization COMPLETED.

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


This example uses the following peripherals:

  - AES
  - RNG
  - USART: 115,200 bits/s, 8 bits, no parity, one stop bit, and no flow control.
  - GPIO


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->
<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>
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


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** NIST vectors examples for AES CBC are taken from: "Recommendation for Block Cipher Modes of Operation, 2001 Edition", available at:["sp800-38a"] (https://csrc.nist.gov/publications/detail/sp/800-38a/final)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
