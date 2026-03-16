<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_crc_user_defined_polynomial*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube firmware package.")

How to configure and drive the CRC peripheral with a user-defined polynomial and an 8-bits CRC
through the HAL API. A CRC is computed for a one-word buffer and compared with an expected value.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: CRC is initialized by app_init() with an 8-bits length and a user-defined polynomial.
The polynomial is set to 0x9B, that is, X^8 + X^7 + X^4 + X^3 + X + 1. This value is used as an illustration
since it is common, for instance in 3GPP protocols.

__Step 2__: the CRC is calculated for a 32-bits data words buffer and compared to the
expected result. You can update DataBuffer or CRC parameters used by HAL_CRC_Init()
but in this case, you also need to update ExpectedValue accordingly. The CRC calculation on STM32
is done on uint32_t, so if you use an external tool to find the reference CRC value, zero padding
is needed.

__Step 3__: In the final step, the CRC peripheral is deinitialized.

__End of example__: After step 3, the example is completed. You can check its status via the status LED, and the CRC value with a debugger.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Correct CRC value (0x98)
[INFO] Step 3: de-init
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

This example demonstrates the following peripheral:

- CRC: configuration with a user-defined polynomial and an 8-bits length. Others parameters are the
standard ones (0xFF default initialization value, no input, or output inversion, data in words format).


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the boards that support CRC.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube firmware package.")

1. This CRC implementation provides some changes in endianness management compared to the previous STM32CubeFW solution. Now, the CRC calculation does not depend on the input data format (bytes, half-words, or words). Due to this modification, and depending on your input data format, you may have a different CRC value if comparing with the previous solution.

2. If you use an online CRC calculator tool, be careful with the endianness. To check this example value, you may have to input 0x34 0x12 0x00 0x00 (instead of 0x00 0x00 0x12 0x34)


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an4187-using-the-crc-peripheral-in-the-stm32-family-stmicroelectronics.pdf)
explains further this CRC feature.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
