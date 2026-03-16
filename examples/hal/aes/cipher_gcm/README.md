<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_aes_cipher_gcm*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the AES peripheral to perform authenticated encryption/decryption with associated data (AEAD/ADAD) using the AES GCM algorithm.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: AES initialization

__Step 2__: Encrypts and decrypts with associated data, then verifies.

Step 2.1: Configuration of the AES peripheral to with the with Galois/Counter mode (GCM) by invoking the `HAL_AES_GCM_GMAC_SetConfig()`.
Step 2.2: Authenticated encryption with associated data of a known plaintext, followed by a verification of the generated ciphertext.
Step 2.3: Authenticated decryption with associated data of a known ciphertext, followed by a verification of the generated plaintext.
Step 2.4: Clears all data.

__Step 3__: AES deinitialization before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED(turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```"USE_TRACE"
[INFO] Step 1: AES initialization COMPLETED.
[INFO] Step 2.1: Configuration of the AES peripheral to with the with GCM mode COMPLETED.
[INFO] Step 2.2: Authenticated Encryption with associated data (AEAD) of a known plaintext COMPLETED.
[INFO] Step 2.3: Authenticated Decryption with associated data (AEAD) of a known ciphertext COMPLETED.
[INFO] Step 2.4: Data clearness COMPLETED.
[INFO] Step 3: AES deinitialization before leaving the scenario COMPLETED.

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


This example uses the following peripherals:
  __AES__: Configured to work with Galois/Counter mode (GCM)
      > **_NOTE:_** the driver only pads the missing words within a block (One block is equal to four words). When the user provides a data size not multiple of words, they must pad the missing bytes within their last word with zeros for GCM and CCM algorithms.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

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
