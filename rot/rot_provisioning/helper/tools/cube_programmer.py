#
# ****************************************************************************
# @attention
#
# Copyright (c) 2026 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
# ****************************************************************************

from helper.common.cli import CLI
from helper.tools.tools_config import Tools_Config
from helper.exception.rot_exception import RoTEmptyValueError
from enum import Enum
import os


class CubeProgrammerSpeed(Enum):
    """
    Enumeration of connection speed modes for the CubeProgrammer interface.

    Attributes:
        RELIABLE (str): Represents a reliable, possibly slower connection speed.
        FAST (str): Represents a faster, potentially less stable connection speed.
    """
    RELIABLE = "reliable"
    FAST = "fast"


class CubeProgrammerMode(Enum):
    """
    Enumeration of connection modes for the CubeProgrammer interface.

    Attributes:
        NORMAL (str): Standard connection speed.
        UNDER_RESET (str): Enables connection to the target using a reset vector catch before executing any instruction.
        HOT_PLUG (str): Enables connection to the target without a halt or reset.
        POWER_DOWN (str): Allows to put the target in debug mode, even if the application has not started since the target power up.
        HARDWARE_RESET_PULSE (str): The tool generates a reset pulse and then connects to the target.
    """

    NORMAL = "normal"
    UNDER_RESET = "ur"
    HOT_PLUG = "hotplug"
    POWER_DOWN = "powerdown"
    HARDWARE_RESET_PULSE = "hwrstpulse"


class CubeProgrammerAccessPort(Enum):
    """
    Enumeration of access ports for the CubeProgrammer interface.

    Attributes:
        ACCESS_PORT_0 (str): Access Port 0.
        ACCESS_PORT_1 (str): Access Port 1.
    """
    ACCESS_PORT_0 = "0"
    ACCESS_PORT_1 = "1"


class CubeProgrammer(CLI):
    """
        CubeProgrammer provides a high-level Python interface to the STM32 CubeProgrammer command-line tool.

        This class enables automated interaction with STM32 devices for tasks such as:
            - Connecting to devices with configurable speed and verbosity
            - Resetting devices
            - Flashing binary files to memory
            - Erasing specific flash memory sectors
            - Filling memory regions with patterns
            - Enabling or disabling TrustZone security features
            - Setting and retrieving device lifecycle states
            - Reading and programming option bytes
    """

    def __init__(self, tools_config: Tools_Config, stlink_sn: str = None):
        """
        Initializes the instance with the CubeProgrammer path from tools configuration.

        Args:
            tools_config: An object containing configuration settings for tools.
            stlink_sn (str, optional): The ST-Link serial number to use for connecting to the device. Defaults to None.
        """
        self.stlink_sn = stlink_sn
        cmd_file_path = os.path.join(os.getcwd(), "cubeprogrammer_cmd.txt")
        super().__init__(tools_config.get_cube_prog_path(), cmd_output_file_name=cmd_file_path)

    def connect_command(self, verbose: bool = False, speed: CubeProgrammerSpeed = CubeProgrammerSpeed.RELIABLE) -> str:
        """
        Construct the connection command string for the device programmer interface.

        Args:
            verbose (bool, optional): If True, enables verbose mode in the command. Defaults to False.
            speed (CubeProgrammerSpeed, optional): Specifies the connection speed mode. Should be one of the predefined
                speed constants (e.g., CubeProgrammerSpeed.RELIABLE or CubeProgrammerSpeed.FAST).
                Defaults to CubeProgrammerSpeed.RELIABLE.

        Returns:
            str: The assembled command string to be used for connecting to the device.
        """
        command_list = ["-c", "port=SWD"]
        if (self.stlink_sn is not None):
            command_list.append(f"sn={self.stlink_sn}")
        if (verbose):
            command_list.append("-vb 1")
        if (speed == CubeProgrammerSpeed.FAST):
            command_list.append(f"speed={speed.value}")
        return " ".join(command_list)

    def build_command(self, verbose: bool = False, speed: CubeProgrammerSpeed = CubeProgrammerSpeed.RELIABLE, mode: CubeProgrammerMode = CubeProgrammerMode.HOT_PLUG,
                      access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1, reset: bool = False, extra_args: str = None) -> str:
        """
        Build the full CubeProgrammer command string with optional parameters.

        Args:
            verbose (bool, optional): Enable verbose mode in the command. Defaults to False.
            speed (CubeProgrammerSpeed, optional): Connection speed mode, typically one of the CubeProgrammerSpeed constants.
                Defaults to CubeProgrammerSpeed.RELIABLE.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.HOT_PLUG.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.
            reset (bool, optional): If True, includes a hard reset flag in the command. Defaults to False.
            extra_args (str, optional): Additional command line arguments to append. Defaults to None.

        Returns:
            str: The complete command string ready to be executed by the CubeProgrammer interface.
        """
        command_list = []
        command_list.append(self.connect_command(verbose=verbose, speed=speed))

        if (mode != CubeProgrammerMode.NORMAL):
            command_list.append(f"mode={mode.value}")

        if (access_port != CubeProgrammerAccessPort.ACCESS_PORT_0):
            command_list.append(f"ap={access_port.value}")

        if reset:
            command_list.append("-hardRst")

        if extra_args is not None:
            command_list.append(extra_args)

        return " ".join(command_list)

    def run_command(self, verbose: bool = False, speed: CubeProgrammerSpeed = CubeProgrammerSpeed.RELIABLE, mode: CubeProgrammerMode = CubeProgrammerMode.HOT_PLUG,
                    access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1, reset: bool = False, extra_args: str = None, match_substring: str = None) -> str:
        """
        Execute the CubeProgrammer command constructed with specified options.

        Args:
            verbose (bool, optional): Enable verbose mode for the command execution. Defaults to False.
            speed (CubeProgrammerSpeed, optional): Connection speed mode, typically one of the CubeProgrammerSpeed constants.
                Defaults to CubeProgrammerSpeed.RELIABLE.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.HOT_PLUG.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.
            reset (bool, optional): If True, includes a hard reset flag in the command. Defaults to False.
            extra_args (str, optional): Additional command line arguments to append. Defaults to None.
            match_substring (str, optional): If provided, filters the command output to include only lines containing this substring.
                Defaults to None.

        Returns:
            str: The output from executing the command, optionally filtered by `match_substring`.
        """
        command = self.build_command(verbose=verbose, speed=speed, mode=mode,
                                     access_port=access_port, reset=reset, extra_args=extra_args)
        return self.call(command_args=command, match_substring=match_substring)

    def reset(self, mode: CubeProgrammerMode = CubeProgrammerMode.HOT_PLUG, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Reset the connected device.

        This method triggers a device hardware reset.

        Args:
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.HOT_PLUG.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        self.run_command(reset=True, mode=mode, access_port=access_port)

    def fill_memory(self, address: str, size: str, pattern: str, mode: CubeProgrammerMode = CubeProgrammerMode.NORMAL,
                    access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Fill a memory region with a specified pattern starting from a given address.

        Args:
            address (str): The starting memory address where the fill operation begins.
            size (str): The size of the memory region to fill (e.g., in bytes or words).
            pattern (str): The pattern value to write repeatedly into the memory.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.NORMAL.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        command = f"-fillmemory {address} size={size} pattern={pattern}"
        self.run_command(speed=CubeProgrammerSpeed.FAST, mode=mode, access_port=access_port, extra_args=command)

    def flash_binary(self, binary_path: str, start_address: str = None, mode: CubeProgrammerMode = CubeProgrammerMode.UNDER_RESET,
                     access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Download a binary file to the device at the specified start address.

        Args:
            binary_path (str): The file path to the binary to be flashed.
            start_address (str, optional): The memory start address where the binary should be flashed.
                If None, the default start address is used.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.UNDER_RESET.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        command = f"-d \"{binary_path}\""
        if start_address is not None:
            command += f" {start_address}"
        command += " -v"
        command += " --skipErase"
        self.run_command(speed=CubeProgrammerSpeed.FAST, mode=mode, access_port=access_port, extra_args=command)

    def flash_obk(self, obk_path: str, mode: CubeProgrammerMode = CubeProgrammerMode.HOT_PLUG,
                  access_port: CubeProgrammerAccessPort = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Flashes an OBK to the device
        Args:
            obk_path (str): The file path to the OBK file to be flashed.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.HOT_PLUG.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.
        Returns:
            None
        """
        command = f"-sdp \"{obk_path}\""
        self.run_command(speed=CubeProgrammerSpeed.FAST, mode=mode, access_port=access_port, extra_args=command)

    def erase_flash_sectors(self, start: str, end: str, mode: CubeProgrammerMode = CubeProgrammerMode.UNDER_RESET,
                            access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Erases flash memory sectors from the specified start address to the end address.

        Args:
            start (str): The starting address or sector to begin erasing.
            end (str): The ending address or sector to stop erasing.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.UNDER_RESET.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None

        Example:
            erase_flash_sectors("0", "5")
        """
        command = f"--erase [{start} {end}]"
        self.run_command(speed=CubeProgrammerSpeed.FAST, mode=mode, access_port=access_port, extra_args=command)

    def erase_all_flash_sectors(self, mode: CubeProgrammerMode = CubeProgrammerMode.UNDER_RESET,
                                access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Erases all the flash memory sectors of the connected device.

        Args:
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.UNDER_RESET.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        command = f"--erase all"
        self.run_command(speed=CubeProgrammerSpeed.FAST, mode=mode, access_port=access_port, extra_args=command)

    def enable_trustzone(self, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Enables the TrustZone security feature by setting the appropriate option bytes.

        Args:
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Raises:
            Any exception raised by set_option_bytes if the operation fails.
        """
        self.set_option_bytes("TZEN", "0xB4", access_port=access_port)

    def disable_trustzone(self, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Disables the TrustZone security feature by setting the the appropriate option bytes'.

        Args:
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        self.set_option_bytes("TZEN", "0xC3", access_port=access_port)

    def set_lifecycle_state(self, obk_file: str, mode: CubeProgrammerMode = CubeProgrammerMode.HOT_PLUG,
                            access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> None:
        """
        Sets the lifecycle state of the device using the specified OBK file.

        Args:
            obk_file (str): The path to the OBK (Object Key) file used for setting the lifecycle state.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.HOT_PLUG.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            None
        """
        command = f"-sdp \"{obk_file}\""
        self.run_command(mode=mode, access_port=access_port, extra_args=command)

    def get_lifecycle_state(self, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> str:
        """
        Retrieves the current device lifecycle state by executing a debug authentication command.

        Args:
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            str: The current device lifecycle state as a string.

        """
        command = f"debugauth=2"
        lifecycle_state = self.run_command(access_port=access_port, extra_args=command, match_substring="PSA lifecycle")
        lifecycle_state = lifecycle_state.split(":")[2]
        return lifecycle_state.strip()

    def set_option_bytes(self, optbytes: dict, mode: CubeProgrammerMode = CubeProgrammerMode.UNDER_RESET,
                         access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1, reset: bool = False) -> None:
        """
        Sets multiple option bytes to the given values on the target device.

        Args:
            optbytes (dict): Dictionary containing option bytes settings.
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.UNDER_RESET.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.
            reset (bool, optional): If True, includes a hard reset flag in the command. Defaults to False.

        Raises:
            RoTEmptyValueError: If the optbytes dictionary is empty.

        Returns:
            None

        Example:
            set_option_bytes({"RDP_level": "0xAA", "TZEN": "0xB4"})
        """
        if not optbytes:
            raise RoTEmptyValueError("option bytes dictionary")
        command = "-ob " + " ".join([f"{optbyte}={optbyte_value}" for optbyte, optbyte_value in optbytes.items()])
        self.run_command(reset=reset, mode=mode, access_port=access_port, extra_args=command)

    def get_option_bytes(self, optbyte: str, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_1) -> str:
        """
        Retrieves the value of a specified option byte from the device.

        Args:
            optbyte (str): The name of the option byte to retrieve.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_1.

        Returns:
            str: The value of the specified option byte, stripped of leading and trailing whitespace.

        """
        command = f"-ob displ"
        option_byte = self.run_command(access_port=access_port, extra_args=command, match_substring=optbyte)
        option_byte = option_byte.split(":")[1]
        return option_byte.strip()
