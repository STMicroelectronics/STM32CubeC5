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

import sys
import os
import argparse
import logging

from helper.common.log import ROT_Logger
from helper.workflow.process import Process

from oemirot.generation_step import SetupGenerationStep
from oemirot.regression_step import SetupRegressionStep
from oemirot.installation_step import SetupInstallationStep

from helper.config.provisioning_config import Provisioning_Config


class Provisioning():
    def __init__(self, path: str, logger: ROT_Logger):
        self.config_path = path
        self.logger = logger
        self.parser = self.set_up_argparse()

    def set_up_argparse(self):
        """
        Sets up the argparse for the ROT provisioning.
        This parser includes:
            - -v/--verbose flag for enabling debug logging.
            - -h/--help flag for displaying help information.
            - --no-gen Only do the installation of a previously generated configuration.
            - -a/--auto flag for removing the need of user input
            - --stlink-sn to specify the ST-Link serial number to use for provisioning.
        Returns:
            argparse.ArgumentParser: Configured argument parser for the ROT provisioning.
        """
        # Create the main parser
        parser = argparse.ArgumentParser(description='ROT provisioning script')

        # Add the arg
        parser.add_argument('--no-gen', action="store_true",
                            help="Only do the installation of a previously generated configuration")
        parser.add_argument('-a', '--auto', action="store_true", help='Remove needs of user input')
        parser.add_argument('-v', '--verbose', action="store_true", help='Set log to debug')
        parser.add_argument('-r', '--regression', action="store_true", help='Run only regression step')
        parser.add_argument('-wr', '--with-regression', action="store_true",
                            help='Run regression step along with other steps')

        parser.add_argument('--stlink-sn', action="store", default=None,
                            help='ST-Link serial number to use for provisioning')

        return parser

    def process_args(self):
        """
        Processes command-line arguments for the ROT provisioning.
        """
        args = self.parser.parse_args()

        auto = args.auto
        provisioning = Process(auto=auto)

        # Load provisioning configuration
        config = Provisioning_Config(self.config_path)

        # Set log level
        if args.verbose:
            self.logger.set_print_debug_level(logging.DEBUG)
        # Process regression step
        if args.regression or args.with_regression:
            regression_step = SetupRegressionStep(config, stlink_sn=args.stlink_sn, remove_ob=args.regression)
            provisioning.add_step(regression_step)
            if args.regression:
                # Execute only the regression step
                provisioning.run_process()
                return

        # Process generation step
        if not args.no_gen:
            generation_step = SetupGenerationStep(config)
            provisioning.add_step(generation_step)

        # Process installation step
        installation_step = SetupInstallationStep(config, stlink_sn=args.stlink_sn)
        provisioning.add_step(installation_step)

        # Execute the provisioning workflow, which includes generation
        # and installation steps based on the provided arguments.
        provisioning.run_process()
