<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_check_point_ecc_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the PKA peripheral in interrupt mode to determine if a point is on a curve. This allows validating an external public key.

For this example, 2 test vectors have been extracted from National Institute of Standards and Technology (NIST): Cryptographic Algorithm Validation Program (CAVP) in order to demonstrate the usage of the hal.

Those two test vectors have been chosen to demonstrate the behavior in a case where the input point validates the curve equation and in a case where the input point does not verify the curve equation.
Their definition is included in 'pkv.c'. You can refer to this file for more information.

The selected curve for this example is P-256 (ECDSA-256) published by NIST in Federal Information Processing Standards Publication FIPS PUB 186-4. The description of this curve is present in file 'prime256v1.c'.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initializes the RNG and PKA instances.

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

 - __Step 2__: Determines the Montgomery parameter.
    - __Step 2.1__: Sets Montgomery parameter Configuration.
    - __Step 2.2__: Computes montgomery R2 input parameter.s
    - __Step 2.3__: Retrieves Montgomery parameter.

 - __Step 3__: Uses the PKA peripheral to determine if a point is on a curve.
   This allows validating an external public key following these steps:
    - __Step 3.1__: Configures the PKA peripheral to validate an external public key.
    - __Step 3.2__: Computes point P(x,y) on the curve primv256V1.
    - __Step 3.3__: Verifies if the calculated point is on the curve.
    - __Step 3.4__: Configures x and y coordinates.
    - __Step 3.5__: Computes point P(x,y) not on the curve primv256V1.
    - __Step 3.6__: Verifies if the calculated point is not on the curve.

 - __Step 4__: Deinitializes the RNG and the PKA peripherals before leaving the scenario.

 __End of example__: After step 4, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: RNG and PKA initialization COMPLETED.
[INFO] Step 2.1: Set Montgomery parameter Configuration COMPLETED.
[INFO] Step 2.2: Compute of the montgomery R2 input parameter COMPLETED
[INFO] Step 2.3: Retrieve Montgomery parameter COMPLETED.
[INFO] Step 3.1: PKA peripheral configuration COMPLETED.
[INFO] Step 3.2: Computation using IT COMPLETED
[INFO] Step 3.3: Verification of the calculated point is on the curve COMPLETED.
[INFO] Step 3.4: PKA peripheral configuration COMPLETED.
[INFO] Step 3.5: Computation using IT COMPLETED.
[INFO] Step 3.6: Verification of the calculated point is not on the curve COMPLETED.
[INFO] Step 4: PKA and RNG peripherals deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


  - USART: 115,200 bits/s, 8 bits, no parity, one stop bit, and no flow control.
  - GPIO


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the Boards that support PKA.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** NIST vectors examples for PKA are taken from:"Test Vectors", available at "http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip (ZIP PKV.rsp)"


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
