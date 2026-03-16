<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_integrity_hash_shake*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


How to use the STM32 Cryptographic library to digest a message using the SHA3 SHAKE algorithm.
This example demonstrates two ways of using the cryptographic services:

  1. The single call method: algorithm configuration and execution are done via a single API call.
  2. The multiple calls method: algorithm configuration and execution are done in several API calls, allowing in particular a piecemeal injection of data to process.

  For each method, a hashing of a known message, followed by the verification of the generated digest.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 __Step 1__: Initializes STM32 Cryptographic library

    Initialization of: Basic stdio, instruction cache (2-ways set associative cache) and STM32 Cryptographic library.

 __Step 2__: Demonstrates how to use the STM32 Cryptographic library to digest a message using the SHAKE compliant algorithm in two ways:

    Step 2.1: Computes the digest using single API call method: Algorithm configuration and execution is done via a single API call.Computes directly the digest using single call: Algorithm configuration and execution is done via a single API call.
    Step 2.2: Computes the digest using multiple API calls method: Algorithm configuration and execution is done in several API calls, allowing in particular a piecemeal injection of data to process.

 __Step 3__: Deinitializes STM32 Cryptographic library before leaving the scenario

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Compute the digest using single API call method COMPLETED.
[INFO] Step 2.2: Compute the digest using multiple API calls method COMPLETED.
[INFO] Step 3: STM32 Cryptographic library deinitialization COMPLETED.
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

> **_NOTE:_** NIST vectors examples for SHAKE are taken from:
"SHA-3 XOF Test Vectors for Byte-Oriented Output", available at:  https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/secure-hashing


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
