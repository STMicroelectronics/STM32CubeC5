<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_modular_exponentiation_crt*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to perform decryption using the Chinese Remainder Theorem (CRT) optimization with the RSA algorithm.

For this example, two .pem file have been created using openssl external tool. (https://www.openssl.org).

Src/rsa_pub_2048.pem and Src/rsa_priv_2048.pem, contains respectively the public and private key used in this example.

To ease the usage of this .pem files, 2 set of files, Src/rsa_pub_2048.c and Inc/rsa_pub_2048.h and Src/rsa_priv_2048.c and Inc/rsa_priv_2048.h are present to reflect their content. The explanation of how to create those file from a .pem are embedded inside both .c files.

A ciphered buffer is deciphered using the CRT modular exponentiation and the private key.
The result is stored in the decipheredBuffer. After this, the buffer is identical to the plain text.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

__Step 1__: Initializes the RNG and PKA instances.

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

__Step 2__: Performs decryption using the Chinese Remainder Theorem (CRT) optimization with the RSA.
   This step is divided into three sub-steps:
      Step 2.1: Configures PKA to process RSA CRT exponentiation operation.
      Step 2.2: Decipher "ciphertext" using PKA modular exponentiation.
      Step 2.3: Compares to expected result.

__Step 3__: Deinitializes PKA and RNG before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1.1: RNG initialization COMPLETED.
[INFO] Step 1.2: PKA initialization COMPLETED.
[INFO] Step 2.1: PKA configuration to process RSA CRT exponentiation operation COMPLETED.
[INFO] Step 2.2: Decipher ciphertext using PKA modular exponentiation COMPLETED.
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
