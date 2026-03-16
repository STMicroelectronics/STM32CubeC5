<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_aes_cipher_cbc_dma*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

 How to use the STM32 HAL API to perform encryption and decryption using the HAL AES CBC algorithm.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes and configures the AES and DMA instances, and register the callbacks.

Step 1.1: Configures the AES peripheral with the CBC algorithm with encryption method.
Step 1.2: Initializes the DMA instance.
Step 1.3: Registers callbacks.

__Step 2__:   Encrypts in DMA mode a known plaintext and verifies the generated ciphertext.
              Then decrypts in DMA mode a known ciphertext and verifies the generated plaintext.

Step 2.1: Encrypts in DMA mode a known plaintext then verifies the generated ciphertext.
Step 2.2: Configures the AES peripheral with the CBC algorithm with decryption method.
Step 2.3: Decrypts in DMA mode a known ciphertext then verifies the generated plaintext.
Step 2.4: Clears all data.

__Step 3__: Deinitializes the AES and DMA instances before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED(turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```"USE_TRACE"

[INFO] Step 1.1: AES, DMA initialization and configuration to work with CBC, DMA mode COMPLETED.
[INFO] Step 1.2: Callbacks registration COMPLETED.
[INFO] Step 2.1: Encryption and verification in the DMA mode of a known plaintext COMPLETED.
[INFO] Step 2.2: Configuration of the AES peripheral to work with CBC in decryption mode COMPLETED.
[INFO] Step 2.3: Decryption and verification in the DMA mode of a known ciphertext COMPLETED.
[INFO] Step 2.4: Data clearness COMPLETED.
[INFO] Step 3: AES and DMA deinitialization COMPLETED.

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the boards that support AES.

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
