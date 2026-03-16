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

from helper.workflow.step import Step
from helper.tools.tools_config import Tools_Config
from helper.config.provisioning_config import Provisioning_Config
from helper.config.example_config import Example_Config
from helper.config.memory_config import Memory_Config
from helper.config.flash_layout_oemirot import FlashLayout_OEMiRoT
from helper.tools.cube_programmer_rdp import CubeProgrammerRDP
from helper.exception.rot_exception import RoTValueError
from oemirot.installation_step import RemoveOBStep

# Metadata for the overall regression step
regression_step = {
    "id": "regression",
    "name": "Regression",
    "wait_for_input": False,
    "success": "Successful regression",
    "skip_if_auto": False
}
# Substeps for the RDP configuration process
rdp_regression_substeps = {
    "rdp_regression": {
        "name": "{name} regression",
        "texts": [{"message": "RDP regression using the {name} key ({path})", "type": "warning"}, {"type": "warning", "message": "This will erase all the content of the device!"}],
        "skip_if_auto": False,
        "wait_for_input": True,
    },
}


class RegressionStep(Step):
    """
    Represents the main regression step in the workflow.
    """

    def __init__(self):
        super().__init__(regression_step)

    def process_step(self):
        pass


class RDPRegressionStep(Step):
    """
    Represents the substep for perform an RDP regression.

    Args:
        config (Provisioning_Config): The provisioning configuration object for retrieving RDP key file paths.
        cube_programmer_rdp (CubeProgrammerRDP): The CubeProgrammerRDP instance for programming the RDP keys.

    """

    def __init__(self, config: Provisioning_Config, cube_programmer_rdp: CubeProgrammerRDP):
        super().__init__(rdp_regression_substeps["rdp_regression"])
        self.log_param = {'name': "RDP", 'path': config.get_regression_rdp_key_path()}
        self.provisioning_config = config
        self.cube_programmer_rdp = cube_programmer_rdp

    def process_step(self):
        # check and get the regression RDP key
        regression_rdp_key = self.provisioning_config.get_rdp_key("regression")

        # Program the RDP2 key using CubeProgrammerRDP
        self.cube_programmer_rdp.rdp_regression_password(password_list=regression_rdp_key)


def SetupRegressionStep(config: Provisioning_Config, stlink_sn: str = None, remove_ob: bool = True) -> Step:
    """
    Set up the regression step by adding all necessary substeps.

    Args:
        config: The provisioning configuration object containing paths and settings.
        stlink_sn: Optional ST-Link serial number to use to perform RDP regression.

    Returns:
        Step: The fully configured regression step.
    """
    cube_programmer_rdp = CubeProgrammerRDP(Tools_Config(config.get_tools_bin_env_name()), stlink_sn=stlink_sn)
    regressionStep = RegressionStep()
    regressionStep.add_step(RDPRegressionStep(config, cube_programmer_rdp))

    if remove_ob:
        example_config = Example_Config(config.get_example_json_path())
        memory_config = Memory_Config(config.get_memory_config_ini_path(), example_config)
        flash_layout = FlashLayout_OEMiRoT(example_config, memory_config)
        regressionStep.add_step(RemoveOBStep(example_config, cube_programmer_rdp, flash_layout))

    return regressionStep
