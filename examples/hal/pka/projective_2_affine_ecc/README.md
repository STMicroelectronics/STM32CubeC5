<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_projective_2_affine_ecc*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the PKA to run ECC Projective to Affine operation.
ECC projective to affine operation computes the conversion between the representation of a point P in homogeneous projective coordinates and the representation of the point P in affine coordinates.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

__Step 1__: Initializes the RNG and PKA instances.

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

 - __Step 2__: Demonstrates how compute the conversion between the representation of a point P in homogeneous projective coordinates and the representation of the point P in affine coordinates.
    This step is divided into five sub-steps:
    - __Step 2.1__: Configures Projective to affine
    - __Step 2.2__: Computes the representation of the point P in affine coordinates
    - __Step 2.3__: Compares to expected result

__Step 3__: Deinitializes PKA and RNG before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PKA initialization COMPLETED.
[INFO] Step 2.1: PKA configuration ECC projective to affine COMPLETED.
[INFO] Step 2.2: Retrieve the point P in affine coordinates COMPLETED.
[INFO] Step 2.3: Compare to expected result COMPLETED.
[INFO] Step 3: PKA and RNG deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


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
