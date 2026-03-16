<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_sign_verify_rsa_v1_5*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic library to perform the signature and the verification of a message using the RSA PKCS#1 v1.5 compliant algorithm.

This example demonstrates, one way of using the STM32 cryptographic library services, the configuration of the algorithm and the operation are done in one single API call.

During the single call method, there are two examples provided:

Preliminary - Generate digest of the known message

  1. The signature of the known message digest using the regular private key representation, so that generated signature can be compared to the known signature.
  2. The signature of the known message digest using the chinese remainder theorem private key representation, so that generated signature can be compared to the known signature.
  3. The verification of the message digest with the known signature.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 - __Step 1__: Initializes STM32 Cryptographic library

    Initialization of: Basic stdio, instruction cache (2-ways set associative cache) and STM32 Cryptographic library.

 - __Step 2__: Demonstrates single API call method

    Step 2.1: Generates digest of the known message.
    Step 2.2: Computes directly the signature using regular private key representation.
    Step 2.3: Computes directly the signature using the CRT private key representation
    Step 2.4: Verifies the message with the known signature

 - __Step 3__: Deinitializes STM32 Cryptographic library before leaving the scenario

 __End of example__

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 2.1: Generate digest of the known message COMPLETED.
[INFO] Step 2.2: Compute signature using the regular private key representation COMPLETED.
[INFO] Step 2.3: Compute signature using the CRT private key representation COMPLETED.
[INFO] Step 2.4: Verify the message with the known signature COMPLETED.
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

Vectors examples for RSA are taken from "pyca cryptography / pkcs1v15sign-vectors.txt", available at https://github.com/pyca/cryptography/tree/master/vectors/cryptography_vectors/asymmetric/RSA

> **_NOTE:_** This example uses the specific `cmox_rsa_construct()` that allows to construct a RSA context,specifying mathematics implementation and working buffer for later processing. So the use of `ALIGNED()` for the working buffer is mandatory, to avoid the
misaligned data that prevents computation errors.

> **_NOTE:_** This example uses the specific `cmox_rsa_setKeyCRTwithFACM()` that allows to be protected against the Bellcore attack. For more details on this attack, see https://eprint.iacr.org/2012/553.pdf


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
