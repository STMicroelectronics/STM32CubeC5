<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_rng_candidate_germanbsi_config*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the RNG peripheral to generate random numbers with a specific entropy configuration.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: initializes RNG with the "German Candidate BSI AIS-31" compliant configuration.

__Step 2__: generates several 32-bit random numbers and stores them into an array.

__Step 3__: deinitializes the RNG peripheral.

__End of example__: After step 3, the example is completed. You can check the random numbers with a debugger.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Generate 32-bit random number.
[INFO] Step 3: Device de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RNG__:

The configuration is set to "German Candidate BSI AIS-31" among the following compliant settings:
    - German BSI AIS-31 tests (clock error detection enabled) in candidate version.
    - NIST SP800-90B tests (clock error detection enabled) in candidate or certified version.
    - Custom configuration

Read the reference manual to get more information about these settings.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup needed for this example.

### __3.2. Specific board setups__

No specific boards setup needed for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Timeout__: Processing times to generate random numbers can be long, in particular when you request many numbers. You can check in the reference manual to choose a suitable timeout value.

__Clock__: The RNG engine might need a dedicated clock, asynchronous to the AHB clock domain. We recommend enabling the clock detection error, and check the clock error flag before using generated numbers.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/product_training/stm32l4_security_rng.pdf) explains the RNG features and their applications (cryptography and gaming).

This [application note](https://www.st.com/content/ccc/resource/technical/document/application_note/4a/6a/82/05/8e/9e/4e/94/DM00073853.pdf/files/DM00073853.pdf/jcr:content/translations/en.DM00073853.pdf) gives details about RNG validation using the NIST statistical test suite for STM32 microcontrollers.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
