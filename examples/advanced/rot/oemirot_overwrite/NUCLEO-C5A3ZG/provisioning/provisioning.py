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
import logging
import configparser


def get_ini_path() -> str:
    return os.path.normpath(os.path.join(os.path.dirname(__file__), './provisioning.ini'))


def get_path_provisioning():
    provisioning_ini_path = get_ini_path()

    if (not os.path.exists(provisioning_ini_path)):
        raise Exception(f"Provisioning ini not found {provisioning_ini_path}")

    config_provisioning = configparser.ConfigParser()
    config_provisioning.read(provisioning_ini_path)
    rot_provisioning_path = config_provisioning['provisioning']['path']
    # Doesn't handle the case where rot_provisioning_path is absolute because it is not expected, which is not true
    return os.path.abspath(os.path.join(os.path.dirname(provisioning_ini_path), rot_provisioning_path))


path_rot_provisioning = get_path_provisioning()
sys.path.insert(0, path_rot_provisioning)
from oemirot.generic_provisioning import Provisioning
from helper.common.log import ROT_Logger
from helper.exception.rot_exception import handle_rot_exception


@handle_rot_exception
def main():
    rot_logger = ROT_Logger('provisioning.log')
    provisioning = Provisioning(get_ini_path(), rot_logger)
    provisioning.process_args()


if __name__ == "__main__":
    main()
