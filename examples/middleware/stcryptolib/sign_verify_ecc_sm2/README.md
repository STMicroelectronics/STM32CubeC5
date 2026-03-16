<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_sign_verify_ecc_sm2*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic library to sign and to verify a message using the SM2 algorithm.

This example demonstrates how to use the predefined curves parameters to perform ECC operation, the configuration of the algorithm and the operation are done in one single API call.

During the single call method, there are two examples provided:

Preliminary: Compute the user identity hash (ZA) and generate digest of the known message
  1. The signature of the computed digest of a known message using a known random, so that computed signature can be compared to the known signature
  2. The verification of the known signature.
  3. The signature of the computed digest of a known message using a true random.
  4. The verification of this newly generated signature.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

__Step 1__: Initializes the STM32 Cryptographic library and the RNG instance.

    Step 1.1: Initializes the STM32 Cryptographic library.
    Step 1.2: Initializes the RNG instance.


__Step 2__: Demonstrates how to use the STM32 Cryptographic Library to sign and to verify a message with a known random then a true random using the SM2 algorithm.

    Step 2.1: Computation and verification of the user identity hash (ZA).
    Step 2.2: Construction of hash context that is configured to perform SM3 digest operations. After that, initialization of hash context, and append of the computed ZA and the message to generate the digest message. Then verification of computed value.
    Step 2.3: Computation and verification of the signature of the computed digest of a known message using a known random.
    Step 2.4: Computation and verification of the signature of the computed digest of a known message using a true random.


__Step 3__: Deinitializes STM32 Cryptographic library and the RNG instance before leaving the scenario.

    Step 3.1: Deinitializes the STM32 Cryptographic library.
    Step 3.2: Deinitializes the RNG instance.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.
[INFO] Step 1.2: RNG initialization COMPLETED.
[INFO] Step 2.1: Computation and verification of the user identity hash (ZA) COMPLETED.
[INFO] Step 2.2: Computation and verification of the digest of a known message COMPLETED.
[INFO] Step 2.3: Computation and verification of the signature of the computed digest of a known message using a known random COMPLETED.
[INFO] Step 2.4: Computation and verification of the signature of the computed digest of a known message using a true random COMPLETED.
[INFO] Step 3.1: STM32 Cryptographic library deinitialization COMPLETED.
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

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube Firmware package.")

> **_NOTE:_** NIST vectors examples for SM2 are taken from "SM2 Digital Signature Algorithm / draft-shen-sm2-ecdsa-02", available at https://tools.ietf.org/html/draft-shen-sm2-ecdsa-02

> **_NOTE:_** This example uses the specific `cmox_ecc_construct()` that allows to construct an ECC context,specifying mathematics implementation and working buffer for later processing. So the use of `ALIGNED()` for the working buffer is mandatory, to avoid the misaligned data that prevents computation errors.


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.