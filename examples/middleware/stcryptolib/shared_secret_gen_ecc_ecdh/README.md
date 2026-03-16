<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_shared_secret_gen_ecc_ecdh*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic library to establish and to verify a shared secret using the ECDH algorithm over SECP256R1 curve.

This example demonstrates, one way of using the STM32 cryptographic library services, the configuration of the algorithm and the operation are done in one single API call.
During the single call method, there are two examples provided:

   1. The establishement of a shared secret.
   2. The verification of the shared secret.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 - __Step 1__: Initialization phase.

    Initialization of STM32 Cryptographic library.

 - __Step 2__: Demonstrates how to use the STM32 Cryptographic Library to establish and to verify a shared secret  using the ECDH algorithm over SECP256R1 curve.

         Step 2.1: Establishes the shared secret using known remote public and known local Key.
         Step 2.2: Compares the generated secret X coordinate with the expected one.

 - __Step 3__: Deinitializes STM32 Cryptographic library before leaving the scenario.

 __End of example__

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Establish the shared secret using known remote public and known local Key COMPLETED.
[INFO] Step 2.2: Compare the generated secret X coordinate with the expected one COMPLETED.
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

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube Firmware package.")

> **_NOTE:_** NIST vectors examples for ECDH are taken from "SP 800-56A / ECCCDH Primitive Test Vectors", available at: https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/Component-Testing

> **_NOTE:_** This example uses the specific `cmox_ecc_construct()` that allows to construct an ECC context,specifying mathematics implementation and working buffer for later processing. So the use of `ALIGNED()` for the working buffer is mandatory, to avoid the
misaligned data that prevents computation errors.


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
