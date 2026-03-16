<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_hash_integrity_sha_suspension_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the HASH peripheral to suspend and to resume a HASH computation process in the interrupt mode using SHA256 algorithm.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

- __Step 1__: HASH and timer peripherals initialization.
    Step 1.1: Initializes the HASH peripheral.
    Step 1.2: Initializes and starts the timer peripheral.

- __Step 2__: Demonstrates how to suspend and to resume a HASH computation process in the interrupt mode, using the timer to request suspend computation called in HAL_TIM_UpdateCallback, by setting the Suspendflag.

    Step 2.1: Requests to suspend IT computation process.
    Step 2.2: Suspends IT computation process.
    Step 2.3: Computes and verifies the second message in polling mode.
    Step 2.4: Resumes and verifies the computation process in interrupt mode.

- __Step 3__: HASH and timer peripherals deinitialization before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: HASH initialization COMPLETED.
[INFO] Step 1.2: Timer initialization and start COMPLETED.
[INFO] Step 2.1: Suspension of computation IT process COMPLETED.
[INFO] Step 2.2: Computation and verification of the second message in polling mode COMPLETED.
[INFO] Step 2.3: Resumption and verification of HASH computation process COMPLETED.
[INFO] Step 3: HASH and timer peripherals deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


This example uses the following peripherals:

  - HASH: Configured to select either the data swpping (no swap or half word swap or bit swap or byte swap) and the   algorithm (MD5 or SHA1 or SH224 or SH256).
      > **_NOTE:_** There are 2 families of API in the polling mode:
      - OneShot API:: handling one single/complete buffer and providing the HASH result.
      - Accumulation APIs: allowing the user to accumulate several buffers then provide the HASH result corresponding
      to the data provided by the sum of these accumulated buffers.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the Boards that support HASH.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** NIST vectors examples for SHA256 are taken from:"Test Vectors", available at: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/shs/shabytetestvectors.zip


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
