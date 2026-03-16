<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_i3c_switch_to_controller*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


How to handle an I3C switching role from target to controller.

The example implements the controller's code, using interrupts (IT).

**Note that the terminology Controller/Target characterizes the role taken by each device in the I3C communication, corresponding respectively to the I3C master and I3C slave in legacy terminology.**


## __1. Detailed scenario__

In this example, the target application performs a controller role request procedure to an I3C controller. The actual application flow, as implemented in `application/example.c`, is as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.


The application executes the following __example steps__:

__Step 1__: Initialization of the I3C peripheral and configuration of the transfer context and registers all user callbacks for controller events.

__Step 2__: DAA process and verification. Activate notification for the Dynamic Address Assignment (DAA) process, wait for completion, and verify the assigned dynamic address.

__Step 3__: Control Role Request and transfer completion. Request to take control of the I3C bus, wait for the handoff to complete, and handle the transfer complete or error event.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.


If you enable **`USE_TRACE`**, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: DAA process and verification COMPLETED.
[INFO] Target - Switch to controller COMPLETED.
[ERROR] Target - Communication ERROR: hal_status = HAL_ERROR.          TRYING AGAIN.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals.


__I3C__: is configured as indicated below:

- The bus usage, including the I3C bus and its duty cycle timings, is calculated by STM32CubeMX2 in accordance with the I3C initialization section of the reference manual.
- The bus is configured as an `I3C pure bus`, dedicated to private I3C-to-I3C communication between controller and target devices.

- The I3C bus is configured to run at the maximum supported speed to demonstrate its highest performance.
  See `__I3C maximum speed__` in section [3.2 Specific board setups](#32-specific-board-setups).
- The event and error interrupts of the I3C instance are configured and enabled in the NVIC.

### Key Terms

**CCC (Common Command Codes):**
Commands defined by the I3C specification that are used for bus management and communication between controller and target devices. CCCs include operations such as dynamic address assignment, device reset, and enabling/disabling features on the bus.

**DAA (Dynamic Address Assignment):**
The process by which the I3C controller assigns unique dynamic addresses to each target device on the bus. This allows devices to be addressed individually and is a key feature of I3C communication.

**CRR (Controller Role Request):**
A procedure where a target device requests to take over the controller role on the I3C bus. This enables dynamic switching of the controller function between devices, supporting advanced use cases and flexible bus management.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

- The controller board is connected to the target board through the two I3C lines and a common GND.

<!--
@startuml
@startditaa{doc/setup.svg} -E -S
    /-------------------------\                     /-------------------------\
    |    /--------------------+                     +--------------\          |
    |    |STM32 I3Ci          |                     |  STM32 I3Ci  |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |I3Ci_SCL------------+---------------------+ I3Ci_SCL     |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |I3Ci_SDA------------+---------------------+ I3Ci_SDA     |          |
    |    |               c4BE |                     |       c4BE   |          |
    |    \--------------------+                     +--------------/          |
    |                         |                     |                         |
    |                     GND +---------------------+ GND                     |
    |                         |                     |                         |
    |     STM32 MCU on        |                     |     STM32 MCU on        |
    |     Controller board    |                     |     target board        |
    \-------------------------/                     \-------------------------/

@endditaa
@endumldd
-->


![setup](doc/setup.svg)

### __3.2. Specific board setups__

The I3C serial clock (SCL) and data (SDA) lines can be observed by connecting an oscilloscope or a logic analyzer to the corresponding board connectors.

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>I3C maximum speed</summary>

  The maximum speed configured for these series is 12.5MHz.

  </details>

  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

  1. If there are no I3C signals observed, check the following:
     - The GND pins of the controller and target boards are connected.
     - Use the shortest possible wires between the boards to improve signal integrity.

  2. For correct synchronization, always run the target application before running the controller. This ensures the target is ready to respond to the controller's I3C communication requests.

  3. The example relies on interrupt-driven callbacks for all address assignment and data transfer events. If the expected callbacks are not triggered, verify the NVIC configuration and that interrupts are enabled for the I3C peripheral.

  4. If repeated communication errors are logged (see terminal output), check the physical connection and ensure both boards are powered and properly initialized before starting the scenario.

  5. If the Dynamic Address Assignment (DAA) process fails or does not complete:
     - Ensure that all devices on the I3C bus support DAA and are properly connected.
     - Check that the controller initiates the DAA process and that no address conflicts occur.
     - Verify that the I3C lines are not held low or high by any device, which can prevent address assignment.
     - Confirm that the firmware enables and handles the DAA-related interrupts correctly.

  6. If the Controller Role Request (CRR) does not succeed:
     - Make sure the target device is allowed to request the controller role according to the bus configuration.
     - Check that the CRR is issued only after the DAA process is complete and the device has a valid dynamic address.
     - Ensure that the controller acknowledges the CRR and the bus is idle before the handover.
     - Review the implementation of the CRR event and completion callbacks in the firmware.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- You can refer to the *hal_i3c_switch_to_target* example pack to have a look at the controller application.

- You can find the application note AN5879 related to the I3C MANUAL on the [AN5879](https://www.st.com/resource/en/application_note/an5879-introduction-to-i3c-for-stm32-mcus-stmicroelectronics.pdf) website if you want to go further on some technical details of the I3C bus

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>
