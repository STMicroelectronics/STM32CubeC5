<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_aes_cipher_cbc_simple_suspension*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

 How to use the STM32 HAL API to suspend then resume the AES CBC processing of a message without modifying the AES CBC context.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes, configures the AES and the timer peripheral.
    Step 1.1: Initializes, configures the AES peripheral.
    Step 1.2: Initializes the timer peripheral.

__Step 2__: Demonstrates how to suspend the AES CBC encryption IT process, using the timer
to request suspend encryption, called in HAL_TIM_UpdateCallback, by setting the Suspendflag. Followed by encryption process in polling mode, then Resumes the IT encryption process.

    Step 2.1: Suspends the IT encryption process.
    Step 2.2: Configures the AES peripheral with the GCM algorithm.
    Step 2.3: Performs a verified decryption of a known text,followed by the generation of the authentication tag.
    Step 2.4: Resumes the IT encryption process.

__Step 3__: Deinitializes the AES and the timer peripherals before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED(turned on) and the ExecStatus variable (EXEC_STATUS_OK).

````USE_TRACE`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```"USE_TRACE"
[INFO] Step 1.1: Initialization, configuration of the AES peripheral with CBC algorithm COMPLETED.
[INFO] Step 1.2: Timer initialization COMPLETED.
[INFO] Step 2.1: Suspension of IT encryption process COMPLETED.
[INFO] Step 2.2: Resumption of IT encryption process COMPLETED.
[INFO] Step 2.3: Verification of computed cipher text as expected COMPLETED.
[INFO] Step 3: AES and timer deinitialization COMPLETED.

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the Boards that support AES.

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
