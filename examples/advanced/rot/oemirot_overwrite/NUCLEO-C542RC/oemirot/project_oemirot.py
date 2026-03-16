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

import os
import sys
import configparser


def get_ini_path() -> str:
    """
    Returns the normalized absolute path to the 'project_oemirot.ini' configuration file.

    The path is constructed by joining the directory of the current file with
    './project_oemirot.ini', and then normalizing it to remove any redundant
    separators or up-level references.

    Returns:
        str: The normalized path to the 'project_oemirot.ini' file.
    """
    return os.path.normpath(os.path.join(os.path.dirname(__file__), './project_oemirot.ini'))


def _get_project_config_parser(project_ini_path: str) -> configparser.ConfigParser:
    project_ini_path = project_ini_path
    config_project = configparser.ConfigParser()
    config_project.read(project_ini_path)
    return config_project


def _get_provisioning_ini_path_relative(provisioning_ini_path: str, project_ini_path: str) -> str:
    return os.path.abspath(
        os.path.join(os.path.dirname(project_ini_path), provisioning_ini_path)
    )


def _get_provisioning_ini_path(config_project: configparser.ConfigParser, project_ini_path: str) -> str:
    provisioning_ini_path = config_project['provisioning']['ini_path']
    if not os.path.isabs(provisioning_ini_path):
        provisioning_ini_path = _get_provisioning_ini_path_relative(provisioning_ini_path, project_ini_path)
    if not os.path.exists(provisioning_ini_path):
        raise Exception(f"Provisioning INI file not found {provisioning_ini_path}")
    return provisioning_ini_path


def _get_provisioning_config_parser(provisioning_ini_path: str) -> configparser.ConfigParser:
    config_provisioning = configparser.ConfigParser()
    config_provisioning.read(provisioning_ini_path)
    return config_provisioning


def _get_rot_provisioning_path(config_provisioning: configparser.ConfigParser, provisioning_ini_path: str) -> str:
    rot_provisioning_path = config_provisioning['provisioning']['path']
    return os.path.abspath(os.path.join(os.path.dirname(provisioning_ini_path), rot_provisioning_path))


def get_path_provisioning(project_ini_path: str) -> str:
    """
    Retrieves the path to the ROT provisioning directory based on the provided project INI file path.

    Args:
        project_ini_path (str): The file path to the project's INI configuration file.

    Returns:
        str: The resolved path to the ROT provisioning directory.
    """
    config_project = _get_project_config_parser(project_ini_path)
    provisioning_ini_path = _get_provisioning_ini_path(config_project, project_ini_path)
    config_provisioning = _get_provisioning_config_parser(provisioning_ini_path)
    return _get_rot_provisioning_path(config_provisioning, provisioning_ini_path)


project_ini_path = get_ini_path()
path_rot_provisioning = get_path_provisioning(project_ini_path)
sys.path.insert(0, path_rot_provisioning)
from helper.config.oemirot_config import OEMiRoT_Config
from helper.project.project_oemirot import Project_OEMiRoT
from helper.common.log import ROT_Logger
from helper.exception.rot_exception import handle_rot_exception


@handle_rot_exception
def main():
    rot_logger = ROT_Logger('oemirot.log')
    config = OEMiRoT_Config(project_ini_path)
    project_oemirot = Project_OEMiRoT(config, rot_logger)
    project_oemirot.process_args()


if __name__ == "__main__":
    main()
