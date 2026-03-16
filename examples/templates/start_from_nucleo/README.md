<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

**Example version:** 2.0.0

# __Template: *example_template_start_from_nucleo*__

This template is a starting point to create __bare-metal C projects leveraging the STM32Cube HAL__
with `STM32CubeMX2` or one of the `Integrated Development Environments (IDEs)` supported in the STM32Cube ecosystem.
It illustrates how to start from a nucleo board where the user **LED**, the user **button**, and the **console** interface for traces are already configured and ready to use.


## __1. Content Description__

This template application project initiates a bare-metal C project.
It integrates all the required startup files and HAL drivers to initialize the system.
This template application creates a single binary that can use the entire memory layout and system resources.

The template files are used to:

- `Start from a board` with STM32CubeMX2.
- `Start an IDE project` with your preferred IDE.

These template files encompass:

- Platform and IDE agnostic template files in the `application/` folder.
- Target specific files (one folder per STM32 board, for example: `NUCLEO-C562RE/`).

These target specific files belong to several categories.

  - Project files, depending on the IDE (`cmake/`, `ewarm/`, and `open-cmsis/`):
  - Hardware-specific C code:
    - Application specific files in `user_modifiable/Device/` (linker scripts, device startup files...).
    - STM32 utility initialization and configuration files in `generated/utility/` (syscalls and basic_stdio).
    - STM32 peripherals initialization and configuration files in `generated/hal/` (HAL code for clock configuration...).
    - STM32 parts initialization and configuration files in `generated/parts/` (led and button).

## __2. Software Environment and how to use the template__

This template helps the user to get started with a minimal board configuration where the **LED**, **button** and **console** are pre-configured and ready to use. The template turns on the status LED, then uses a button callback to detect press events and toggles the LED each time the button is pressed, printing `"LED toggled"` on the console. The sequence runs indefinitely.

### __2.1. With STM32CubeMX2__

You just need to start a project from a board within the tool itself.
The template projects are already known by the tool.

### __2.2. With one of the STM32Cube ecosystem IDEs__

You just need to open the IDE project files present in the folder of the `STM32 board` you are interested in.
The IDE initiates the project with the reference files, and you can complete it as per your needs.

### __2.3. With any other IDE of your choice__

You just need to create a project of your own and import the reference C code (`application/` and `generated/` folders).

## __3. Reconfiguring the template application__

You can also reconfigure this template application with `STM32CubeMX2`.

To do so:

- Open the `.ioc2` file provided in the board folder
- Reconfigure the system as per your needs (clock, pinout, peripherals...)
- Generate the IDE project of your choice

At the end of these operations:

- The content of the `generated/` folder is updated with your latest choices.
- The IDE project that you created is ready to be opened with your selected IDE.

## __4. Troubleshooting__

Here are some typical points of attention in case of issues.

### __4.1. General Debug Tips__

#### __4.1.1. Flashing issues__

1. Try flashing using `STM32CubeProgrammer`.
2. Check the device configuration against the description in the STM32 system memory boot mode application note.

#### __4.1.2 Startup issues__

1. Read the flash or RAM content using `STM32CubeProgrammer` to make sure the code you expect to run is loaded in the device.
2. Check the boot options of your device (boot pins...) in the reference manual.
3. Try connecting to the board and use a debugger to spot any hard fault in the code.

#### __4.1.3 Issues to connect to the board__

Possible issues are:

1. The firmware running on the target is forcing a low-power mode.
The communication with STLINK is disabled when the board is in low-power mode.

2. The target is secured or debug-disabled.

#### __4.1.4. HAL timing issues__

Care must be taken when using `HAL_Delay()`,
this function provides accurate delay (in milliseconds) based on variable incremented in SysTick ISR.

This implies that if `HAL_Delay()` is called from a peripheral ISR process,
then the SysTick interrupt must have higher priority (numerically lower) than the peripheral interrupt.
Otherwise, the caller ISR process is blocked.
To change the SysTick interrupt priority you have to use `HAL_NVIC_SetPriority()` function.

The example needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL operation.

### __4.2 Fault Exceptions__

Arm Cortex®-M processors implement an efficient exception model that traps illegal memory accesses and
several incorrect program conditions.
The HardFault exception is always enabled and has a fixed priority (higher than other interrupts and exceptions,
but lower than non-maskable interrupt NMI). The HardFault exception is therefore executed in cases where a
fault exception is disabled or when a fault occurs during the execution of a fault exception handler.

All other fault exceptions (MemManage fault, BusFault, UsageFault...) have a programmable priority.
After reset, these exceptions are disabled and may be enabled in the system or application software
using the registers in the System Control Block (SCB).

### __4.3 Debug Toolbox__

Refer to the [STM32 debug toolbox](https://www.st.com/resource/en/application_note/an4989-stm32-microcontroller-debug-toolbox-stmicroelectronics.pdf)
for further information about how to troubleshoot the problem.

## __5. See Also__

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.