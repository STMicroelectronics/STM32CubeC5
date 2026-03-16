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
from helper.config.project_config import Project_Config
from helper.config.provisioning_config import Provisioning_Config

logger = logging.getLogger()


class Appli_Config(Project_Config):
    '''
    Appli_Config is a configuration handler class to handle application ini file.

    INI file example (path can be relative or absolute, relative paths are resolved
    relative to the location of the ini file):
        [provisioning]
        ini_path = /path/to/provisioning.ini

        [build]
        binary_path = /path/to/application/binary/output.hex

        [image_provisioning]
        xml = /path/to/provisioning/image/xml/file.xml
        path = /path/to/provisioning/binary/output.hex

        [image_fwu]
        xml = /path/to/fwu/image/xml/file.xml
        path = /path/to/fwu/binary/output.bin

        [header]
        appli_flash_layout_path = /path/to/flash/layout/appli_flash_layout.h

        # User input for compiler/linker paths
        [iar]
        linker = /path/to/iar/linker
        build_output = /path/to/iar/output/file.bin

    Methods (inherited from Project_Config):
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
        get_tools_bin_env_name() -> str:
            Returns the environment variable name defined in provisioning.ini containing the path to STM32CubeProgrammer bin folder.
    '''

    def __init__(self, config_path: str):
        super().__init__(config_path, ["provisioning", "fwu"], ["appli_flash_layout"])
