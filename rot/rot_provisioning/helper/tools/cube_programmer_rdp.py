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

from helper.tools.cube_programmer import CubeProgrammer, CubeProgrammerMode, CubeProgrammerAccessPort
from helper.exception.rot_exception import RoTEmptyValueError


class CubeProgrammerRDP(CubeProgrammer):
    """
    CubeProgrammerRDP extends the CubeProgrammer class to implement
    specific functionalities for provisioning and managing RDP (Readout Protection) levels such as:
        - Provisioning passwords for RDP Level 2 with/without boundary scan
        - Performing regression from RDP Level 2 with/without boundary scan to Level 0
        - Provisioning boundary scan keys
        - Transitioning RDP from Level 2 with boundary scan to Level 2 with a provided key
    """

    def rdp_provision_password(self, password_list: list, mode: CubeProgrammerMode = CubeProgrammerMode.NORMAL,
                               access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_0) -> None:
        """
        Provisions a password on the device to allow the regression between RDP levels.
        This password allows the RDP transition from level 2 with/without boundary scan to level 0.

        Args:
            password_list (list): A list containing the password elements to be provisioned on the device.
                Example: ["0xAAAAAAAA", "0xBBBBBBBB", "0xAAAAAAAA", "0xBBBBBBBC"]
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.NORMAL.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_0.

        Returns:
            None

        Raises:
            RoTEmptyValueError: If the password list is empty.
        """
        if not password_list:
            raise RoTEmptyValueError("password list")

        password_values = " ".join(password_list)
        command = f"-lockRDP2 {password_values}"
        self.run_command(mode=mode, access_port=access_port, extra_args=command)

    def rdp_regression_password(self, password_list: list, access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_0) -> None:
        """
        Perform the regression from RDP Level 2 with/without boundary scan to Level 0.

        Args:
            password_list (list): A list containing the password elements to be used for regression.
                Example: ["0xAAAAAAAA", "0xBBBBBBBB", "0xAAAAAAAA", "0xBBBBBBBC"]
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_0.

        Returns:
            None

        Raises:
            RoTEmptyValueError: If the password list is empty.
        """
        if not password_list:
            raise RoTEmptyValueError("password list")

        password_values = " ".join(password_list)
        command = f"-unlockRDP2 {password_values}"
        self.run_command(access_port=access_port, extra_args=command)

    def rdp_provision_2bs_key(self, bs_key: str, mode: CubeProgrammerMode = CubeProgrammerMode.NORMAL,
                              access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_0) -> None:
        """
        Provisions a boundary scan key on the device.
        This key allows the RDP transition from level 2 with boundary scan to level 2.

        Args:
            bs_key (str): The boundary scan key to be provisioned on the device.
                Example: "0xAAAAAAAB"
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.NORMAL.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_0.

        Returns:
            None

        Raises:
            RoTEmptyValueError: If the boundary scan key is empty.
        """
        if not bs_key:
            raise RoTEmptyValueError("boundary scan key")

        command = f"-lockbs {bs_key}"
        self.run_command(mode=mode, access_port=access_port, extra_args=command)

    def rdp_transition_from_2bs_to_2(self, bs_key: str = "0xAAAAAAAA", mode: CubeProgrammerMode = CubeProgrammerMode.NORMAL,
                                     access_port: str = CubeProgrammerAccessPort.ACCESS_PORT_0) -> None:
        """
        Transition the RDP from level 2 with boundary scan to level 2 with the provided key.

        Args:
            bs_key (str, optional): The boundary scan key used to perform the transition.
                Defaults to "0xAAAAAAAA".
            mode (CubeProgrammerMode, optional): Connection mode. Defaults to CubeProgrammerMode.NORMAL.
            access_port (CubeProgrammerAccessPort, optional): Access port. Defaults to CubeProgrammerAccessPort.ACCESS_PORT_0.

        Returns:
            None
        """
        command = f"-unlockbs {bs_key}"
        self.run_command(mode=mode, access_port=access_port, extra_args=command)
