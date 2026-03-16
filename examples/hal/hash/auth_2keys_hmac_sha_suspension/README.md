<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_hash_auth_2keys_hmac_sha_suspension*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 HAL API to generate and hash data with HMAC sha256 algorithms in suspension mode,
with different key for each handle.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initializes the HASH and TIM instances.

- __Step 2__: Hash data with HMAC sha256 algorithms in suspension mode, with different key.

  - __Step 2.1__: Starts and Suspends the IT computation process.
  - __Step 2.2__: Generates and verifies a MAC using SHA265 algorithm with different vectors.
  - __Step 2.3__: Resumes the IT Computation process.

- __Step 3__: Deinitializes the HASH and the TIM peripherals before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: HASH and TIM initialization COMPLETED
[INFO] Step 2.1: Suspension of the IT Computation COMPLETED.
[INFO] Step 2.2: MAC generation and verification using SHA256 in polling mode COMPLETED.
[INFO] Step 2.3: Resume the IT process: MAC generation and verification in IT mode COMPLETED.
[INFO] Step 3: HASH and TIM peripherals deinitialization COMPLETED.
```


## __2. Example configuration__

The following **sequence diagram** is used to describe the example behavior.
![template_sequence_diagram](doc/template_sequence_diagram.svg)


[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


  - USART: 115,200 bits/s, 8 bits, no parity, one stop bit, and no flow control.
  - GPIO


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the Boards that support HASH.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** NIST vectors examples for HMAC are taken from:"Test Vectors", available at https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/message-authentication


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
