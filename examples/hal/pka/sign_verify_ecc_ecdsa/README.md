<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_sign_verify_ecc_ecdsa*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to compute and verify a signed message regarding the Elliptic curve digital signature algorithm (ECDSA).


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 - __Step 1__: RNG and PKA initialization

    Initializes RNG HW that is used by the PKA (Public Key Accelerator).

 - __Step 2__: Computes signature.
    This step is divided into five sub-steps:
    - Step 2.1: Configures PKA for Signature Generation
    - Step 2.2: Performs Signature Generation
    - Step 2.3: Compares to expected result

 - __Step 3__: Verifies signature.
    - Step 3.1: Configures PKA for Signature Verification
    - Step 3.2: Performs Signature Verification
    - Step 3.3: Compares to expected result

 - __Step 4__: Deinitializes PKA and RNG before leaving the scenario.

__End of example__: After step 4, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PKA initialization COMPLETED.
[INFO] Step 2.1: PKA configuration to process message signature using elliptic curves COMPLETED.
[INFO] Step 2.2: Signature computation COMPLETED.
[INFO] Step 2.3: Compares to expected result COMPLETED.
[INFO] Step 3.1: PKA configuration to process message verification using elliptic curves COMPLETED.
[INFO] Step 3.2: Signature computation COMPLETED.
[INFO] Step 3.3: Signature verification COMPLETED.
[INFO] Step 4: PKA and RNG deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


This example uses the following peripherals:
  - PKA, RNG
  - USART: 115,200 bits/s, 8 bits, no parity, one stop bit, and no flow control.
  - GPIO


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the boards that support PKA.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** Vectors examples for RSA are taken from "psa_arch_tests\api-tests\dev_apis\crypto", available at https://github.com/ARM-software/psa-arch-tests.git

> **_NOTE:_** NIST vectors examples for ECDSA are taken from "ECDSA Test vectors / FIPS 186-4", available at https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures

> **_NOTE:_** `Known_Signature` is modified for matching `k` used by ARM (not the same as `k` of NIST)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
