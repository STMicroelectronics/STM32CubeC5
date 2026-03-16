<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_crc_default*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the CRC LL API to get a CRC code of a given buffer of data words (32-bit),
based on a fixed default polynomial.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: CRC is initialized in app_init(). The default polynomial configuration for the CRC is referenced in the LL CRC drivers.

__Step 2__: the CRC is calculated for a 32-bits data words buffer and compared to the expected result.
You can update DataBuffer but in this case, you also need to update ExpectedValue accordingly. The CRC calculation on STM32 is done
on uint32_t, so if you use an external tool to find the reference CRC value, zero padding is needed.

__Step 3__: In the final step, the CRC peripheral is deinitialized.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals:

__CRC__: no configuration is needed because the example uses default initialization parameters.
So, the default parameters are (according to the driver documentation):
    - The polynomial coefficient is set to 0x04C11DB7U
    - The polynomial size is set to 32-bits
    - The CRC initialization value is set to 0xFFFFFFFFU
    - The input reversibility mode is set to none
    - The output reversibility mode is set to none


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup needed for this example.

### __3.2. Specific board setups__

No specific board setup needed for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

__Migration__:This CRC implementation provides some changes on endianness management compared to the previous STM32Cube solution. Now, the CRC calculation does not depend
     on the input data format (bytes, half-word, or words). Due to this modification and depending on your input data format, you may have a different CRC calculation.

__Online CRC compute__: If you use an online CRC calculator tool, be careful to endianness. To check this example value, you may have to input '0x34 0x12 0x00 0x00' (instead of '0x00 0x00 0x12 0x34')


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

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
