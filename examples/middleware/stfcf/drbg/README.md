<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stfcf_drbg*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Flexible Crypto Framework (STFCF) to Generation DRBG.

This example is designed in an agnostic manner to support multiple cryptographic implementations, including:

- MbedTLS ALT STCryptolib: An alternative cryptography implementation based on the STM32 Cryptographic Library.

- MbedTLS Crypto Software: A direct software-based implementation of Mbed TLS.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Initializes the RNG instance and PSA Crypto API.

      Step 1.1: Initializes the RNG instance.
      Step 1.2: Initializes the PSA Crypto API.

 - __Step 2__: Demonstrates how to use the STM32 Flexible Crypto Framework (STFCF)
               to generate a random numbers using the RandomGeneration DRBG.

      Step 2.1: Generates the first random data.
      Step 2.2: Generates the second random data.
      Step 2.3: Verifies the generated data are different.

 - __Step 3__: Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.

      Step 3.1: Clears the data of the PSA layer.
      Step 3.2: Deinitializes the RNG instance.


 __End of example__:


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```"USE_TRACE"
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PSA Crypto API initialization COMPLETED.
[INFO] Step 2.1: First random data generation COMPLETED.
[INFO] Step 2.2: Second random data generation COMPLETED.
[INFO] Step 2.3: Random data verification COMPLETED.
[INFO] Step 3.1: Clear data of the PSA layer COMPLETED.
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


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
