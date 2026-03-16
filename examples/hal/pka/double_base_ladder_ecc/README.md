<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_double_base_ladder_ecc*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the PKA to run ECC Double Base Ladder operation.

ECC double base ladder operation consists in the computation of k*P + m*Q, where (P,Q) are two points on an elliptic curve and (k,m) are two scalars.

The RNG initialization is mandatory for the PKA's operations.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initializes the RNG and PKA instances.

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

 - __Step 2__: Computes of k*P + m*Q, where (P,Q) are two points on an elliptic curve and (k,m) are two scalars.
    - __Step 2.1__: Configures the PKA configuration parameters.
    - __Step 2.2__: Starts the PKA ECC Double Base Ladder operation.
    - __Step 2.3__: Retrieves computation result.
    - __Step 2.4__: Checks retrieved result with expected result.

 - __Step 3__: Deinitializes the RNG and the PKA peripherals before leaving the scenario.

 __End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: RNG and PKA initialization COMPLETED.
[INFO] Step 2.1: Configuration parameters COMPLETED.
[INFO] Step 2.2: Computation of the PKA ECC Double Base Ladder operation COMPLETED.
[INFO] Step 2.3: Retrieving the computation result COMPLETED.
[INFO] Step 2.4: Verification of the computed result COMPLETED.
[INFO] Step 3: PKA and RNG peripherals deinitialization COMPLETED.
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


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
