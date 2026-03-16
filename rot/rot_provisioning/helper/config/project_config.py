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
from helper.config.generic_config import Generic_Config
from helper.config.provisioning_config import Provisioning_Config
from helper.exception.rot_exception import RoTNotFoundError

logger = logging.getLogger()


class Project_Config(Generic_Config):
    """
    Project_Config is a configuration handler class to handle project ini file, to provide build, linker, image & provisioning info.

    INI file example (path can be relative or absolute, relative paths are resolved
    relative to the location of the ini file):
        [provisioning]
        ini_path = /path/to/provisioning.ini

        [build]
        binary_path = /path/to/application/binary/output.hex

        # if initialized with image_type containing 'provisioning' and 'fwu':
        [image_provisioning]
        xml = /path/to/provisioning/image/xml/file.xml
        path = /path/to/provisioning/binary/output.hex

        [image_fwu]
        xml = /path/to/fwu/image/xml/file.xml
        path = /path/to/fwu/binary/output.bin

        # Additional headers can be defined here
        [header]
        appli_flash_layout_path = /path/to/flash/layout/appli_flash_layout.h
        other_header = /path/to/other/header/other_header.h

        # User input for compiler/linker paths
        [iar]
        linker = /path/to/iar/linker

    Methods:
        get_provisioning_ini_path() -> str:
            Returns the absolute provisioning ini path.
        get_image_xml_path(image_type: str) -> str:
            Returns the absolute path to the image XML file for the specified image type.
        get_image_binary_path(image_type: str) -> str:
            Returns the absolute path to the image binary for the specified image type.
        get_compiler_linker(compiler: str) -> str:
            Retrieves the absolute path to the linker file for the specified compiler.
        get_build_binary_path() -> str:
            Retrieves the absolute path to the build output binary file.
        get_header_path(header_name: str) -> str:
            Returns the absolute path to the specified header file.
        get_tools_bin_env_name() -> str:
            Returns the environment variable name defined in provisioning.ini containing the path to STM32CubeProgrammer bin folder.
    """

    def __init__(self, config_path: str, image_types: list[str] = [], header_list: list[str] = []):
        self.image_types = image_types
        self.header_list = header_list

        self.image_information = {}
        self.headers_dict = {}

        super().__init__(config_path)

    def _load_config(self):
        """
        Load the configuration specific to the project application.
        This method retrieves paths for:
            - The provisioning ini path.
            - The build binary output path.
            - The binary and image XML files used during provisioning.
            - The binary and image XML files used for OTA (Over-The-Air) firmware updates.
        """
        for img_type in self.image_types:
            self.image_information[img_type] = {"xml": self.get_absolute_path_from_config(
                f'image_{img_type}', 'xml', check_exists=True), "path": self.get_absolute_path_from_config(
                    f'image_{img_type}', 'path', check_exists=False)}
        self.provisioning_ini_path = self.get_absolute_path_from_config('provisioning', 'ini_path', check_exists=True)
        for header in self.header_list:
            self.headers_dict[header] = self.get_absolute_path_from_config(
                'header', f"{header}_path", check_exists=True)
        self.build_binary_path = self.get_absolute_path_from_config('build', 'binary_path', check_exists=False)

    def _set_print_dict(self):
        self.print_config_items["Provisioning INI Path"] = self.provisioning_ini_path
        self.print_config_items["Build Binary Path"] = self.build_binary_path
        for img_type in self.image_types:
            self.print_config_items[f"{img_type.capitalize()} Image Binary Path"] = self.image_information[img_type]["path"]
            self.print_config_items[f"{img_type.capitalize()} Image XML Path"] = self.image_information[img_type]["xml"]
        for header_name, header_path in self.headers_dict.items():
            self.print_config_items[f"{header_name.replace('_', ' ').capitalize()} Path"] = header_path

    def get_provisioning_ini_path(self) -> str:
        """
        Returns the absolute provisioning ini path for the project.

        Returns:
            str: The absolute provisioning ini path.
        """
        return self.provisioning_ini_path

    def get_image_xml_path(self, image_type: str) -> str:
        """
        Returns the absolute path to the image XML file used for the specified image type.

        Args:
            image_type (str): The type of image (e.g., 'provisioning', 'fwu').
        Returns:
            str: The absolute path to the image XML file.
        """
        return self.image_information[image_type]["xml"]

    def get_image_binary_path(self, image_type: str) -> str:
        """
        Returns the absolute path to the image binary used for the specified image type.

        Args:
            image_type (str): The type of image (e.g., 'provisioning', 'fwu').
        Returns:
            str: The absolute path to the image binary file.
        """
        return self.image_information[image_type]["path"]

    # Compiler/linker paths are defined dynamically per compiler, so we can't load them in _load_config
    def get_compiler_linker(self, compiler: str) -> str:
        """
        Retrieve the absolute path to the linker for a given compiler.

        Args:
            compiler (str): The name of the compiler. It has to be the name of a section in the config file.

        Returns:
            str: The absolute path to the linker associated with the specified compiler.

        """
        linker = self.get_absolute_path_from_config(compiler, 'linker')
        return linker

    def get_build_binary_path(self) -> str:
        """
        Retrieves the build output binary path.

        Returns:
            str: The absolute path to the build output binary file as specified in the configuration.
        """
        return self.build_binary_path

    def get_header_path(self, header_name: str) -> str:
        """
        Returns the absolute path to the specified header file.

        Args:
            header_name (str): The name of the header as defined in the [header] section of the config file.

        Returns:
            str: The absolute path to the specified header file.
        """
        return self.headers_dict[header_name]

    def get_tools_bin_env_name(self) -> str:
        """
        Returns the environment variable name defined in provisioning.ini containing the path to STM32CubeProgrammer bin folder.

        Returns:
            str: The environment variable name where the STM32CubeProgrammer bin folder path is stored.
        """
        return Provisioning_Config(self.get_provisioning_ini_path()).get_tools_bin_env_name()
