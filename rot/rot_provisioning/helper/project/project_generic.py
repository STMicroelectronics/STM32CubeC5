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

import argparse
import logging
import os

from helper.common.log import ROT_Logger

from helper.tools.tpc import TPC
from helper.tools.tools_config import Tools_Config

from helper.config.project_config import Project_Config
from helper.config.provisioning_config import Provisioning_Config
from helper.config.example_config import Example_Config
from helper.config.provisioning_config import Provisioning_Config
from helper.config.memory_config import Memory_Config
from helper.config.generic_flash_layout import Generic_FlashLayout

from helper.common.misc import create_directory_if_missing
from helper.common.hxml import extract_output_file_path, update_xml_value_by_name, update_xml_value_by_title
from helper.common.update_file_handler import update_define_in_file

from helper.exception.rot_exception import RoTNotImplementedError

logger = logging.getLogger()


class Project_Generic:
    def __init__(self, config: Project_Config, logger: ROT_Logger, has_postbuild=False, has_prebuild=False):
        self.config = config
        self.logger = logger
        self.has_postbuild = has_postbuild
        self.has_prebuild = has_prebuild
        self._load_additional_configs()
        self.flash_layout = self._compute_flash_layout()
        self.parser = self._set_up_argparse()

    def _load_additional_configs(self):
        """
        Loads additional configuration files that can be used.

        This method initializes the following configuration objects:
            - Provisioning_Config: Loaded from the provisioning INI file path found in the project config.
            - Tools_Config: Loaded from the tools environment variable defined in the provisioning config.
            - Example_Config: Loaded from the example JSON file path specified in the provisioning config.
            - Memory_Config: Loaded from the memory config INI file path specified in the provisioning config.

        Each configuration object is assigned to an instance variable for later use.
        """
        self.provisioning_config = Provisioning_Config(self.config.get_provisioning_ini_path())
        self.tools_config = Tools_Config(self.config.get_tools_bin_env_name())
        self.example_config = Example_Config(self.provisioning_config.get_example_json_path())
        self.memory_config = Memory_Config(self.provisioning_config.get_memory_config_ini_path(), self.example_config)

    def _compute_flash_layout(self) -> Generic_FlashLayout:
        """
        Computes the flash layout for the project. It can later be used by accessing self.flash_layout.

        This method should be implemented by subclasses to load the right flash layout type.
        Returns:
            Generic_FlashLayout: The computed flash layout object.

        Raises:
            RoTNotImplementedError: If the method is not implemented by a subclass.
        """
        raise RoTNotImplementedError("_compute_flash_layout method must be implemented by subclasses.")

    def _set_up_argparse(self):
        """
        Sets up the argparse for the application project.
        This parser includes:
            - -v/--verbose flag for enabling debug logging.
            - -h/--help flag for displaying help information.
            if the project has postbuild:
            - postbuild subcommand for running postbuild.
            if the project has prebuild:
            - prebuild subcommand for running prebuild, with a required --compiler argument.
        Returns:
            argparse.ArgumentParser: Configured argument parser for the application.
        """
        # Create the main parser
        parser = argparse.ArgumentParser(
            description='Application project helper')

        # Create the subparsers
        subparsers = parser.add_subparsers(
            title='subcommands', dest='subcommand')

        if (self.has_postbuild):
            postbuild_parser = subparsers.add_parser(
                'postbuild', help='Run post-build tasks')

        if (self.has_prebuild):
            prebuild_parser = subparsers.add_parser(
                'prebuild', help='Run pre-build tasks')
            prebuild_parser.add_argument(
                '--compiler', required=True, help='Compiler used for the build (e.g [iar])')

        # Add the verbose arg
        parser.add_argument('-v', '--verbose',
                            action="store_true", help='Set log to debug')
        return parser

    def _handle_prebuild(self, compiler: str):
        """
        Handles the prebuild process for the project if prebuild is enabled.

        This method checks if the project has a prebuild step. If so, it updates the logger's file name to "prebuild.log"
        and calls a custom prebuild handler with the specified compiler.

        Args:
            compiler (str): The name of the compiler to use for the prebuild process.
        """
        if (self.has_prebuild):
            self.logger.update_logger_file_name("prebuild.log")
            self._custom_handle_prebuild(compiler)
            logger.info("Prebuild completed successfully.")

    def _custom_handle_prebuild(self, compiler: str):
        """
        Handles custom prebuild steps for the project.

        This method should be overridden in subclasses to implement specific prebuild logic
        based on the provided compiler.

        Args:
            compiler (str): The name of the compiler to use for prebuild steps.

        Raises:
            RoTNotImplementedError: If the method is not overridden in a subclass.
        """
        raise RoTNotImplementedError

    def _handle_postbuild(self):
        """
        Handles the postbuild process for the project if postbuild is enabled.

        This method checks if the project has a postbuild step. If so, it updates the logger's file name to "postbuild.log"
        and calls a custom postbuild handler.
        """
        if (self.has_postbuild):
            self.logger.update_logger_file_name("postbuild.log")
            self._custom_handle_postbuild()
            logger.info("Postbuild completed successfully.")

    def _custom_handle_postbuild(self):
        """
        Handles custom postbuild steps for the project.

        This method should be overridden in subclasses to implement specific postbuild logic.

        Raises:
            RoTNotImplementedError: If the method is not overridden in a subclass.
        """
        raise RoTNotImplementedError

    def process_args(self):
        """
        Processes command-line arguments for the application project.
        Handles manual interruption (KeyboardInterrupt) by logging an error message.
        """
        try:
            args = self.parser.parse_args()

            if args.verbose:
                self.logger.set_print_debug_level(logging.DEBUG)
            if (self.has_postbuild and args.subcommand == 'postbuild'):
                self._handle_postbuild()
            if (self.has_prebuild and args.subcommand == 'prebuild'):
                self._handle_prebuild(args.compiler)
        except KeyboardInterrupt:
            self.logger.logger.error("Application project handling process was manually interrupted. Exiting now.")

    def _generate_image(self, type: str):
        """
        Generates an image using TPC, loading xml from [binary_{type}] in the config file.
        If output directory does not exist, it is created.

        Args:
            type (str): The type of image to generate.

        """
        tpc = TPC(self.tools_config)
        xml_to_process = self.config.get_image_xml_path(type)
        self._create_directory_if_missing_directory_from_image_output(xml_to_process)
        tpc.gen_image(xml_to_process)

    def _create_directory_if_missing_directory_from_image_output(self, xml_path: str):
        """
        Ensures that the directory for the output file specified in the XML path exists.

        This method extracts the output file path from the given XML file path and creates
        the directory if it does not already exist.

        Output is extracted from XML like:
        <Root>
            <Output>
                <Value>relative/or/absolute/path/to/binary.bin</Value>
            </Output>
        </Root>

        Args:
            xml_path (str): The path to the XML file containing the output file information.
        """
        output_file = extract_output_file_path(xml_path)
        create_directory_if_missing(output_file)

    def _update_image_xml(self, type: str, tag_name: str, field_name: str, field_to_update: str, new_value: str):
        """
        Updates the image XML file for the specified type by changing the value of a given field.

        Example:
            To update the output file path in the provisioning image XML defined in the project config ini file:

            ```
            <?xml version="1.0" encoding="UTF-8"?>
            <Root>
                <McubootFormat>
                <Output>
                    <Name>Image output file</Name>
                    <Value>../../bin/appli_fwu_enc_sign.bin</Value>
                    <Default>appli_fwu_enc_sign.bin</Default>
                </Output>
                </McubootFormat>
            </Root>
            ```
            The following call updates the `<Value>` tag under `<Output>` to `new/output/path.bin`:

            `_update_image_xml('provisioning', 'Output', 'Image output file', 'Value', 'new/output/path.bin')`

        Args:
            type (str): The type of binary output (e.g. 'provisioning' or 'fwu').
            tag_name (str): The XML tag name to update.
            field_name (str): The name of the field to be updated in the XML file.
            field_to_update (str): The specific field attribute or sub-element to update.
            new_value (str): The new value to assign to the field.
        """
        image_xml_path = self.config.get_image_xml_path(type)
        update_xml_value_by_name(image_xml_path, tag_name, field_name, field_to_update, new_value)

    def _update_image_xml_output_path(self, type: str):
        """
        Updates the output file path ("Image output file") in the image XML for the specified type.

        Args:
            type (str): The type of the image (defined in project config ini file, e.g `provisioning` or `fwu`).
        """
        binary_path = self.config.get_image_binary_path(type)
        image_xml_path = self.config.get_image_xml_path(type)
        rel_path_in_image_xml = os.path.relpath(
            binary_path, os.path.dirname(image_xml_path))
        self._update_image_xml(type, "Output", "Image output file", "Value", rel_path_in_image_xml)

    def _update_image_xml_keys_path(self, type: str):
        """
        Updates the keys path ("Firmware image generation") in the image XML for the specified type.

        Args:
            type (str): The type of the image (defined in project config ini file, e.g `provisioning` or `fwu`).
        """
        binary_path = self.provisioning_config.get_config_keys_xml_path()
        image_xml_path = self.config.get_image_xml_path(type)
        rel_path_in_image_xml = os.path.relpath(
            binary_path, os.path.dirname(image_xml_path))
        update_xml_value_by_title(image_xml_path, "GlobalParam", "Firmware image generation",
                                  "LinkedXML", rel_path_in_image_xml)

    def _update_image_xml_hex_format(self, type: str):
        """
        Updates the firmware download area offset ("Firmware download area offset") in the image XML for the specified type.
        Args:
            type (str): The type of the image (defined in project config ini file, e.g `provisioning` or `fwu`).
        """
        value = f"0x{self.flash_layout.get_flash_layout('ROM_APPLI_HEX_FORMAT'):X}"
        self._update_image_xml(type, "Param", "Firmware download area offset", "Value", value)

    def _update_image_xml_firmware_area_size(self, type: str):
        """
        Updates the firmware area size ("Firmware area size") in the image XML for the specified type.
        Args:
            type (str): The type of the image (defined in project config ini file, e.g `provisioning` or `fwu`).
        """
        value = f"0x{self.flash_layout.get_flash_layout('FLASH_PRIMARY_APP_SLOT_SIZE'):X}"
        self._update_image_xml(type, "Param", "Firmware area size", "Value", value)

    def _update_image_xml_input_path(self, type: str):
        """
        Updates the input ("Firmware binary input file") in the image XML file with the relative path to the firmware binary input file.

        This method computes the relative path from the image XML file to the build binary,
        and updates the XML file's "Firmware binary input file" parameter accordingly.

        Args:
            type (str): The type of image or build configuration.
            compiler (str): The compiler used for building the binary.

        Returns:
            None
        """
        binary_path = self.config.get_build_binary_path()
        image_xml_path = self.config.get_image_xml_path(type)
        rel_path_in_image_xml = os.path.relpath(
            binary_path, os.path.dirname(image_xml_path))
        self._update_image_xml(type, "Param", "Firmware binary input file", "Value", rel_path_in_image_xml)

    def _update_linker_define(self, compiler: str, define: str):
        """
        Updates the define value in the linker file for the specified compiler from project ini file.
        It uses the flash layout to get the value associated with the define.
        Value from flash layout is converted to hexadecimal format before updating the linker file.

        Args:
            compiler (str): The name of the compiler whose linker file is to be updated.
            define (str): The define to be updated in the linker file.
        """
        linker_path = self.config.get_compiler_linker(compiler)
        value = f"0x{self.flash_layout.get_flash_layout(define):X}"
        update_define_in_file(linker_path, define, value)

    def _update_header_define(self, header_name: str, define_name: str):
        """
        Updates the define value in the specified header file from project ini file.
        It uses the flash layout to get the value associated with the define.
        Value from flash layout is converted to hexadecimal format before updating the header file.
        Args:
            header_name (str): The name of the header file to be updated.
            define_name (str): The define to be updated in the header file.
        """
        header_path = self.config.get_header_path(header_name)
        value = f"(0x{self.flash_layout.get_flash_layout(define_name):X}U)"
        update_define_in_file(header_path, define_name, value)
