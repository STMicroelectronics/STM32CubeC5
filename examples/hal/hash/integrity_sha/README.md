<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_hash_integrity_sha*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 HASH peripheral to digest a message, in case of a single buffer and multibuffer, using the SHA256 algorithm.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:
__Step 1__: Initializes the HASH instance.

__Step 2__: Demonstrates how to digest a message, using the SHA256 algorithm.
For a single buffer to be hashed, HAL_HASH_Compute() is called. For multibuffer input HAL_HASH_Update() and HAL_HASH_Finish() are called.

  - __Step 2.1__: Configures the HASH peripheral.
  - __Step 2.2__: Computes and verifies the HASH message.
  - __Step 2.3__: Updates and verifies the HASH message.

__Step 3__: HASH peripheral deinitialization.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: HASH initialization COMPLETED.
[INFO] Step 2.1: HASH message compute and verification COMPLETED.
[INFO] Step 2.2: HASH message update and verification COMPLETED.
[INFO] Step 3: HASH peripheral deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


This example uses the following peripheral:

__HASH__: Configured to select the data swapping (no swap or half word swap or bit swap or byte swap) and the algorithm (MD5 or SHA1 or SH224 or SH256).

**_NOTE:_** There are two families of API in the polling mode:

  - OneShot API: handling one single complete buffer and providing the HASH result.
  - Accumulation APIs: allowing the user to accumulate several buffers then provide the HASH result corresponding to the sum of these accumulated buffers.

__USART__: 115,200 bits/s, 8 bits, no parity, one stop bit, and no flow control.

__GPIO__: to control the status led.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the boards that support HASH.

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
