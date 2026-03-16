<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_modular_exponentiation_protected*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the PKA to run Protected modular exponentiation operation.

Modular exponentiation operation is commonly used to perform a single-step RSA operation. It consists in the computation of A^e mod n.

RSA decryption operation involving secret information, so it must use the protected mode for security reason.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 __Step 1__: Initializes the RNG and PKA instances.

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

 __Step 2__: The computation of A^e mod n .
      This step is divided into three sub-steps:
      Step 2.1: Configures PKA to process protected modular exponentiation operation.
      Step 2.2: Computes (A^e mod n) in (protected) modular exponentiation mode.
      Step 2.3: Compares to expected result.

 __Step 3__: Deinitializes PKA and RNG before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PKA initialization COMPLETED.
[INFO] Step 2.1: PKA configuration to process protected modular exponentiation operation COMPLETED.
[INFO] Step 2.2: Computes (A^e mod n) using PKA protected modular exponentiation COMPLETED.
[INFO] Step 2.3: Compares to expected result COMPLETED.
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


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
