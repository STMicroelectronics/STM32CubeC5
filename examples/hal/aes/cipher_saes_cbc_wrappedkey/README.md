<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_aes_cipher_saes_cbc_wrappedkey*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

 How to use the STM32 HAL API to perform encryptionand decryption
 with the SAES Hardware key using the AES CBC algorithm.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the SAES instance, configures the AES Key with a HW key source and CBC algorithm.

__Step 2__: Encrypts a known plaintext and verifies the generated ciphertext
            then decrypts a known ciphertext and verifies the generated plaintext using the a wrapped key.

    - __Step 2.1__: Encrypts an application key with SAES HW keys(wrapper key).
    - __Step 2.2__: Configures the AES Key with a HW key source and CBC algorithm.
    - __Step 2.3__: Decrypts an application key with SAES HW keys(wrapper key).
    - __Step 2.4__: Configures the AES peripheral with the CBC algorithm with encryption method
    - __Step 2.5__: Encrypts of a known plaintext then verifies the generated ciphertext.
    - __Step 2.6__: Configures the AES peripheral with the CBC algorithm with decryption method.
    - __Step 2.7__: Decrypts of a known ciphertext then verifies the generated plaintext.
    - __Step 2.8__: Clears all data.

__Step 3__: Deinitializes the SAES instance before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED(turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: SAES initialization, configuration of the HW key source and CBC algorithm COMPLETED COMPLETED.
[INFO] Step 2.1: Encryption of an application key with SAES HW key(wrapper key) COMPLETED.
[INFO] Step 2.2: Configuration of the HW key source and CBC algorithm COMPLETED.
[INFO] Step 2.3: Decryption of an application key with SAES HW key(wrapper key) COMPLETED.
[INFO] Step 2.4: Configuration of the AES peripheral to work with CBC mode COMPLETED.
[INFO] Step 2.5: Encryption and verification of a known plaintext COMPLETED.
[INFO] Step 2.6: Configuration of the AES peripheral to work with CBC mode COMPLETED.
[INFO] Step 2.7: Decryption and verification of a known ciphertext COMPLETED.
[INFO] Step 2.8: Data clearness COMPLETED.
[INFO] Step 3: SAES deinitialization COMPLETED.
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
