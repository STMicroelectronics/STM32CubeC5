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

import configparser
import logging
import os

from helper.common.singleton import Singleton
from helper.config.config_print_utils import print_config_dict
from helper.exception.rot_exception import RoTFileNotFoundError, RoTAbsolutePathError, RoTNotFoundError, RoTNotImplementedError, RoTValueError
logger = logging.getLogger()


class Generic_Config(metaclass=Singleton):
    """
    Generic_Config is a base class for managing configuration files using the Singleton pattern.
    Args:
        config_path (str): The absolute path to the configuration file.
    Attributes:
        config_path (str): Stores the path to the configuration file.
        config (configparser.ConfigParser): The loaded configuration object.
    Methods:
        _load_config():
            Abstract method to be implemented by subclasses for loading specific configuration sections.
            Raises NotImplementedError if not overridden.
        load_all_config():
            Loads all configuration sections by calling the subclass-implemented _load_config method.
            Raises Exception if the configuration is not loaded.
        print_config():
            Prints the current configuration details in a formatted manner.
        write_config():
            Writes the current configuration to the file.
        get_absolute_path_from_config(section, value):
            Returns the absolute path for a given section and value from the configuration file.
    Raises:
        RoTAbsolutePathError: If the provided config_path is not an absolute path.
        RoTNotImplementedError: If _load_config or _set_print_dict methods are not implemented by a subclass.
        RoTFileNotFoundError: If the configuration file does not exist at the specified path.
    """

    def __init__(self, config_path: str):
        # Initialize the configuration with the provided path.
        if (not os.path.isabs(config_path)):
            raise RoTAbsolutePathError(config_path)
        self.config_path = config_path

        # Print configuration
        self.print_config_items = {}
        self.print_config_name = os.path.basename(config_path)

        # Load configuration if the file exists
        if (os.path.exists(config_path)):
            self.load_all_config()
            self.print_config()
        else:
            raise RoTFileNotFoundError(config_path)

    def _load_config_parser(self):
        self.config = configparser.ConfigParser()
        self.config.read(self.config_path)

    def _load_config(self):
        """
        Load the configuration settings.

        This method should be implemented by subclasses to load and initialize
        the necessary configuration parameters for the application.

        Raises:
            RoTNotImplementedError: If the method is not implemented by a subclass.
        """
        raise RoTNotImplementedError("Subclasses should implement this method")

    def load_all_config(self):
        """
        Loads all configuration sections by calling the subclass-implemented _load_config method.
        """
        self._load_config_parser()
        self._load_config()

    def _set_print_dict(self):
        """
        Abstract method to set up a dictionary for printing configuration details.
        Subclasses must implement this method to define how their configuration
        should be represented as a dictionary for logging purposes.

        Raises:
            RoTNotImplementedError: If the method is not implemented by a subclass.
        """
        raise RoTNotImplementedError("Subclasses should implement this method")

    def print_config(self):
        """
        Prints the current configuration details in a formatted manner.
        """
        self._set_print_dict()
        print_config_dict(print_config_items=self.print_config_items, print_config_name=self.print_config_name)

    def get_config_value(self, section: str, key: str) -> str:
        """
        Retrieves a configuration value from the specified section and key.

        Args:
            section (str): The section in the configuration dictionary.
            key (str): The key within the section whose value is to be retrieved.
        Returns:
            str: The value corresponding to the specified section and key.
        Raises:
            RoTNotFoundError: If the specified section or key is not found in the configuration
        """
        if section not in self.config:
            raise RoTNotFoundError(f"Section '{section}' is not found in '{self.config_path}'.")
        if key not in self.config[section]:
            raise RoTNotFoundError(f"Key '{key}' is not found in section '{section}' in '{self.config_path}'.")
        return self.config[section][key]

    def get_absolute_path_from_config(self, section: str, value: str, check_exists: bool = True) -> str:
        """
        Returns the absolute path for a given configuration value.

        If the path specified in the configuration is relative, it is joined with the directory
        containing the configuration file to form an absolute path. If the path is already absolute,
        it is returned as-is. The resulting path is normalized.

        Args:
            section (str): The section in the configuration dictionary.
            value (str): The key within the section whose path value is to be resolved.
            check_exists (bool): Check if the resolved path exists.

        Returns:
            str: The normalized absolute path corresponding to the configuration value.

        Raises:
            RoTNotFoundError: If `check_exists` is True and the resolved path does not exist.
        """
        abs_path = None
        path = self.get_config_value(section, value)
        if (not os.path.isabs(path)):
            abs_path = os.path.join(os.path.dirname(self.config_path), path)
        else:
            abs_path = path
        if (check_exists and not os.path.exists(abs_path)):
            raise RoTNotFoundError(f"{path} defined by ['{section}'].{value} in {self.config_path} does not exists.")
        return os.path.normpath(abs_path)

    def write_config(self):
        """
        Writes the current configuration to the file specified by self.config_path.

        Opens the configuration file in write mode and saves the contents of the
        self.config object to it. Overwrites any existing content in the file.
        """
        with open(self.config_path, 'w') as configfile:
            self.config.write(configfile)
