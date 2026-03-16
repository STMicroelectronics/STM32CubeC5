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


class TPC(CLI):
    def __init__(self, tools_config):
        """
        Initializes the instance with the TPC path from tools configuration.

        Args:
            tools_config: An object containing configuration settings for tools.
        """
        super().__init__(tools_config.get_tpc_path())

    def gen_obk(self, xml):
        """
        Generates an OBK image with the provided XML file.

        Args:
            xml (str): The path to the XML file to be processed.

        Returns:
            None
        """
        self.call(f"-obk \"{xml}\"")

    def gen_image(self, xml):
        """
        Generates an image with the provided XML file.

        Args:
            xml (str): The path to the XML file to be processed.

        Returns:
            None
        """
        self.call(f"-pb \"{xml}\"")
