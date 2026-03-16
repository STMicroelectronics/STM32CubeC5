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

logger = logging.getLogger()


class OEMiRoT_Config(Project_Config):
    '''
    OEMiRoT_Config is a configuration handler class to handle OEMiRoT ini file.

    INI file example (path can be relative or absolute, relative paths are resolved
    relative to the location of the ini file):
        [provisioning]
        ini_path = /path/to/provisioning.ini

        [build]
        binary_path = /path/to/oemirot/binary/output.bin

        [iar]
        linker = /path/to/iar/linker

    Methods (inherited from Project_Config):
        get_provisioning_ini_path() -> str:
            Returns the absolute provisioning ini path.
        get_compiler_linker(compiler: str) -> str:
            Retrieves the absolute path to the linker file for the specified compiler.
        get_build_binary_path() -> str:
            Retrieves the absolute path to the build output binary file.
        get_tools_bin_env_name() -> str:
            Returns the environment variable name defined in provisioning.ini containing the path to STM32CubeProgrammer bin folder.
    '''

    def __init__(self, config_path: str):
        super().__init__(config_path, image_types=[], header_list=["mcuboot_config", "flash_layout", "boot_hal_cfg"])
