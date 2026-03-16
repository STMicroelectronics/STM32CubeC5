<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *lwip_iperf_freertos*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to run an iPerf TCP server application using the LwIP stack and FreeRTOS.  
The iPerf TCP server listens for incoming TCP connections and measures network throughput.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the application by creating the example task.

__Step 2__: Starts the FreeRTOS scheduler.

__Step 3__: Initializes the LwIP stack.  
__Step 3.1__: Initializes the LwIP stack.  
__Step 3.2__: Initializes and registers network interface.  
__Step 3.3__: Acquires IP address.  

__Step 4__: Starts the iPerf TCP server.  
__Step 4.1__: Creates and starts the iPerf TCP server thread.  
__Step 4.2__: Starts the iPerf TCP server to listen for incoming connections.  
__Step 4.3__: Stops the iPerf TCP server when no longer needed.  

__End of example__: The iPerf TCP server runs continuously to accept and measure TCP throughput from clients.

__Step 4 is detailed with the following sequence diagram:__

<!--
@startuml{doc/iperf_sequence_diagram_plantUML.svg}

box STM32
  participant "iPerf TCP Server Thread" as Server
end box
participant "iPerf TCP Client" as Client

loop Continuous operation
    alt Incoming connection
    Client --> Server: Connect\n    Address: IPERF_SERVER_ADDR\n    Port: IPERF_SERVER_PORT
    loop
      Client -> Server: Send TCP data
      Server -> Client: Acknowledge data
    end loop
    Client --> Server: Disconnect
    Server -> Server: print results
    end

end
@enduml
-->

![Sequence Chart](doc/iperf_sequence_diagram_plantUML.svg)


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

#### Network Configuration

- Ensure your network allows direct TCP connections (no restrictive firewall or proxy).
- For LAN setups, connect all devices to the same network segment.

#### DHCP Configuration

- The example uses DHCP by default to obtain an IP address. Ensure a DHCP server is available on your LAN.
- If DHCP is not available, the board will fall back to manual configuration after a timeout (see `APP_LWIP_DHCP_TIMEOUT_MS` in `application/app_config.h`).
- Manual IP, netmask, and gateway can be set via `APP_LWIP_MANUAL_IP_ADDR`, `APP_LWIP_MANUAL_NETMASK`, and `APP_LWIP_MANUAL_GW_ADDR` in `application/app_config.h`.
- LwIP middleware must have DHCP enabled (`LWIP_DHCP=1`). If you disable DHCP, remove related code and set a static IP for the LwIP Netif.

#### mDNS Configuration

- The board can announce its hostname on the LAN using mDNS. The default hostname can be set with `APP_LWIP_MDNS_HOSTNAME` in `application/app_config.h`.
- If mDNS fails, use the board's IP address (printed on the STLINK COM port) for remote clients.
- LwIP middleware must have the mDNS responder enabled (`LWIP_MDNS_RESPONDER=1`). If you disable mDNS, remove related code.

### __2.2 Application Setup__

- **One STM32 MCU and a PC on LAN (PC runs iPerf client):**

<!--
@startuml
@startditaa{doc/ASCII_ditaa_network_setup.png}
    +-------------------------+                     +-------------------------+
    |                         |                     |                         |
    |       TCP iPerf Client  |---------------------|                         |
    |                         |                     |                         |
    | PC                      |                     |                         |
    +-------------------------+                     |                         |
                                                    |                         |
    +-------------------------+                     |                         |
    |                         |                     |                         |
    |       TCP iPerf Server  |---------------------|                         |
    |                         |                     |                         |
    | STM32 MCU               |                     |    Network Router       |
    +-------------------------+                     +-------------------------+
@endditaa
@enduml
-->

![network setup](doc/ASCII_ditaa_network_setup.png)

- On the STM32 MCU:
    - Set `APP_LWIP_MDNS_HOSTNAME` to `stm32_host_custom_1`.

- Run iPerf Client (v2) on Linux computer:
```sh
IPERF_SERVER_ADDR="stm32_host_custom_1"
IPERF_SERVER_PORT=5001
iperf -c $IPERF_SERVER_ADDR -p $IPERF_SERVER_PORT
```


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

### __3.2. Specific board setups__

FreeRTOS exclusively uses the SysTick as its timebase. Thus, `TIM6`, is used as a separate timebase for the HAL.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
