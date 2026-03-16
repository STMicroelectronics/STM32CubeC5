<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_sbs_fpu_interrupt*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the FPU so that an interrupt is triggered when an FPU exception occurs.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick

The application executes the following __example steps__:

__Step 1__: Enables the FPU interrupt on division by zero detection and enables the FPU interrupt in the NVIC.

__Step 2__: Runs a valid operation to make sure the FPU works fine.

__Step 3__: Triggers the floating-point division by zero.

__Step 4__: Handles the FPU exception via the FPU ISR.

__Step 5__: Disables the FPU interrupt.

__End of example__: After step 5, the example is completed. You can observe its status via the status LED. You can check the `ExecStatus` value with a debugger.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


We configure the __SBS__ FPU interrupt mask register with the HAL SBS API:

- Enable the Divide-by-zero interrupt.

We also enable the FPU interrupt in the NVIC.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup is needed for this example.

### __3.2. Specific board setups__

No specific board setup is needed for this example.

You may check the status LED of your board (already connected to the MCU) and, optionally, a debugger connected via STLINK.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


Here are the points of attention for this specific example:

__FPU availability__: This example deals with floating-point arithmetic with an FPU: make sure the SoC you use has an FPU.

__Divide-by-zero__: The FPU implements the IEEE 754 standard. A division by zero can return INFINITY if the FPU is not configured to raise a divide by zero exception.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an4044-floating-point-unit-demonstration-on-stm32-microcontrollers-stmicroelectronics.pdf)
explains further the FPU usage.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
