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

import logging
import os
import platform

from helper.common.singleton import Singleton
from helper.config.config_print_utils import print_config_dict
from helper.exception.rot_exception import RoTToolsEnvNotFoundError, RoTToolsNotFoundError

logger = logging.getLogger()


class Tools_Config(metaclass=Singleton):
    """
    Configuration handler for STM32 tools.

    This class locates and validates the STM32CubeProgrammer and STM32TrustedPackageCreator
    command-line tools based on a specified environment variable.
    """

    def __init__(self, env_name: str):
        """
        Initialize the Tools_Config object.

        Args:
            env_name (str): Name of the environment variable containing the tools path.

        Raises:
            ToolsEnvNotFoundError: If the environment variable is not set.
            ToolsNotFoundError: If tools are not found.
        """
        self.env_name = env_name
        self.prog_path = self._get_env_path()
        self.cube_prog_cli = self._build_tool_path("STM32_Programmer_CLI")
        self.tpc_cli = self._build_tool_path("STM32TrustedPackageCreator_CLI")
        self._validate_tools()
        self._log_tools()

    def _get_env_path(self) -> str:
        """
        Retrieve and expand the environment variable path.

        Returns:
            str: The expanded path from the environment variable.

        Raises:
            RoTToolsEnvNotFoundError: If the environment variable is not set.
        """
        path = os.getenv(self.env_name)
        if not path:
            raise RoTToolsEnvNotFoundError(self.env_name)
        return os.path.expanduser(path)

    def _build_tool_path(self, tool_name: str) -> str:
        """
        Build the full path to a tool executable.

        Args:
            tool_name (str): The base name of the tool executable.

        Returns:
            str: The full path to the tool executable.
        """
        tool_path = os.path.normpath(os.path.join(self.prog_path, tool_name))
        if platform.system() == "Windows":
            tool_path += ".exe"
        return tool_path

    def _validate_tools(self):
        """
        Validate the existence of required tool executables.

        Raises:
            RoTToolsNotFoundError: If any required tool is not found at the expected path.
        """
        if not os.path.exists(self.cube_prog_cli):
            raise RoTToolsNotFoundError("STM32CubeProgrammer", self.cube_prog_cli)
        if not os.path.exists(self.tpc_cli):
            raise RoTToolsNotFoundError("STM32TrustedPackageCreator", self.tpc_cli)

    def _log_tools(self):
        """
        Log the paths of the configured tools.
        """
        print_dict = {
            "STM32CubeProgrammer": self.cube_prog_cli,
            "STM32TrustedPackageCreator": self.tpc_cli
        }
        print_config_dict(print_config_items=print_dict, print_config_name="Tools Configuration")

    def get_cube_prog_path(self) -> str:
        """
        Get the path to the STM32CubeProgrammer CLI executable.

        Returns:
            str: Path to STM32CubeProgrammer CLI.
        """
        return self.cube_prog_cli

    def get_tpc_path(self) -> str:
        """
        Get the path to the STM32TrustedPackageCreator CLI executable.

        Returns:
            str: Path to STM32TrustedPackageCreator CLI.
        """
        return self.tpc_cli
