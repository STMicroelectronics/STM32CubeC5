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

from helper.workflow.step import Step
from helper.tools.cube_programmer import CubeProgrammer, CubeProgrammerMode
from helper.tools.cube_programmer_rdp import CubeProgrammerRDP
from helper.config.example_config import Example_Config
from helper.config.memory_config import Memory_Config
from helper.config.flash_layout_oemirot import FlashLayout_OEMiRoT
from helper.config.option_bytes import ob_oemirot_apply
from helper.tools.tools_config import Tools_Config
from helper.config.appli_config import Appli_Config
from helper.config.provisioning_config import Provisioning_Config
from helper.config.provisioning_config import RDPLevel
from helper.config.oemirot_config import OEMiRoT_Config
from helper.common.hxml import extract_output_file_path
from helper.exception.rot_exception import RoTValueError

logger = logging.getLogger()

# Metadata for the overall installation step
installation_step = {
    "id": "installation",
    "name": "Installation",
    "texts": [{"message": "The {board_name} must be in level 0 of Read Out Protection (RDP)"}],
    "wait_for_input": True,
    "success": "Successful installation"
}

# Substeps of the installation process
installation_substeps = {
    "remove_ob":
    {
        "name": "Option Bytes removing",
        "wait_for_input": False,
        "process": "Option Bytes removing starts ...",
        "success": "Successful Option Bytes removing"
    },
    "install":
    {
        "name": "Installation",
        "wait_for_input": False,
        "process": "Provisioning starting ...",
        "success": "Successful Provisioning"
    },
    "program_ob":
    {
        "name": "Option Bytes programming",
        "wait_for_input": False,
        "process": "Option Bytes programming starts ...",
        "success": "Successful Option Bytes programming"
    },
    "program_rdp_key":
    {
        "name": "RDP keys programming",
        "wait_for_input": False,
        "process": "RDP keys programming starts ...",
        "success": "Successful RDP keys programming"
    },
    "program_final_rdp_level":
    {
        "name": "Final RDP Level programming",
        "texts": [{"message": "Programming RDP, regression is impossible without the key !", "type": "warning"}],
        "wait_for_input": True,
        "process": "Final RDP Level programming starts ...",
        "success": "Successful Final RDP Level programming"
    },
}


class InstallationStep(Step):
    """
    Represents the main installation step in the workflow.

    Args:
        board_name (str): The name of the board being provisioned.
    """

    def __init__(self, board_name: str):
        super().__init__(installation_step)
        self.log_param = {"board_name": board_name}


class RemoveOBStep(Step):
    """
    Represents the substep for removing option bytes.

    Args:
        example_config (Example_Config): example.json object for retrieving the default option bytes values.
        cube_programmer (CubeProgrammer): The CubeProgrammer instance for programming the option bytes.

    """

    def __init__(self, example_config: Example_Config, cube_programmer: CubeProgrammer, flash_layout: FlashLayout_OEMiRoT):
        super().__init__(installation_substeps["remove_ob"])
        self.example_config = example_config
        self.cube_programmer = cube_programmer
        self.flash_layout = flash_layout

    def process_step(self):
        ob_oemirot_apply("default", self.example_config, self.flash_layout, self.cube_programmer)
        self.cube_programmer.erase_all_flash_sectors()


class InstallationSubstep(Step):
    """
    Represents the substep for flashing binaries during the installation process.

    Args:
        config (Provisioning_Config): The provisioning object to retrieve binary path.
        cube_programmer (CubeProgrammer): The CubeProgrammer instance for flashing the binaries.
    """

    def __init__(self, config: Provisioning_Config, cube_programmer: CubeProgrammer):
        super().__init__(installation_substeps["install"])
        self.provisioning_config = config
        self.appli_config = Appli_Config(self.provisioning_config.get_appli_ini_path())
        self.oemirot_config = OEMiRoT_Config(self.provisioning_config.get_oemirot_ini_path())
        self.example_config = Example_Config(self.provisioning_config.get_example_json_path())
        self.memory_config = Memory_Config(
            self.provisioning_config.get_memory_config_ini_path(), self.example_config)
        self.flash_layout = FlashLayout_OEMiRoT(self.example_config,
                                                self.memory_config)
        self.cube_programmer = cube_programmer
        self.base = int(self.example_config.get_memory()["base"], 16)

    def process_step(self):
        key_path_bin = extract_output_file_path(self.provisioning_config.get_config_keys_xml_path())
        config_data_offset = hex(self.base + self.flash_layout.get_flash_layout("FLASH_NVCNT_OFFSET"))
        key_offset = hex(self.base + self.flash_layout.get_flash_layout("FLASH_KEYS_OFFSET"))

        # Applying preflash OB
        ob_oemirot_apply("preflash", self.example_config, self.flash_layout, self.cube_programmer)

        # Flash all the binaries
        self.cube_programmer.flash_binary(self.provisioning_config.get_config_data_path(), config_data_offset)
        self.cube_programmer.flash_binary(key_path_bin, key_offset)
        self.cube_programmer.flash_binary(self.appli_config.get_image_binary_path("provisioning"))
        self.cube_programmer.flash_binary(self.oemirot_config.get_build_binary_path())
        if self.flash_layout.get_flash_layout("DATA_IMAGE_NUMBER") > 0:
            appli_data_path_bin = extract_output_file_path(
                self.provisioning_config.get_appli_data_xml_path("provisioning"))
            self.cube_programmer.flash_binary(appli_data_path_bin)


class ProgramOBStep(Step):
    """
    Represents the substep for programming option bytes.

    Args:
        example_config (Example_Config): example.json object for retrieving the option bytes values.
        cube_programmer (CubeProgrammer): The CubeProgrammer instance for programming the option bytes.

    """

    def __init__(self, example_config: Example_Config, cube_programmer: CubeProgrammer, flash_layout: FlashLayout_OEMiRoT):
        super().__init__(installation_substeps["program_ob"])
        self.example_config = example_config
        self.cube_programmer = cube_programmer
        self.flash_layout = flash_layout

    def process_step(self):
        ob_oemirot_apply("postflash", self.example_config, self.flash_layout, self.cube_programmer)


class ProgramRDPKeyStep(Step):
    """
    Represents the substep for programming RDP keys.

    Args:
        config (Provisioning_Config): The provisioning configuration object for retrieving RDP key file paths.
        cube_programmer_rdp (CubeProgrammerRDP): The CubeProgrammerRDP instance for programming the RDP keys.

    """

    def __init__(self, config: Provisioning_Config, cube_programmer_rdp: CubeProgrammerRDP):
        super().__init__(installation_substeps["program_rdp_key"])
        self.provisioning_config = config
        self.cube_programmer_rdp = cube_programmer_rdp

    def process_step(self):
        # Get the regression and transition RDP keys
        regression_rdp_key = self.provisioning_config.get_rdp_key("regression")
        transition_bs_key = self.provisioning_config.get_rdp_key("transition")

        # Program the RDP2 key using CubeProgrammerRDP
        self.cube_programmer_rdp.rdp_provision_password(password_list=regression_rdp_key)

        # program the boundary scan key using CubeProgrammerRDP
        self.cube_programmer_rdp.rdp_provision_2bs_key(bs_key=transition_bs_key[0])


class ProgramFinalRDPLevelStep(Step):
    """
    Represents the substep for setting the final RDP level.

    Args:
        config (Provisioning_Config): The provisioning configuration object for retrieving the RDP level values.
        cube_programmer (CubeProgrammer): The CubeProgrammer instance for setting the RDP level.

    """

    def __init__(self, config: Provisioning_Config, cube_programmer: CubeProgrammer):
        super().__init__(installation_substeps["program_final_rdp_level"])
        self.provisioning_config = config
        self.cube_programmer = cube_programmer

    def check_skip(self):
        return self.provisioning_config.get_rdp_level("final").value == RDPLevel.RDP_0.value

    def process_step(self):
        final_rdp_level = self.provisioning_config.get_rdp_level("final").value
        if final_rdp_level == RDPLevel.RDP_0.value:
            final_rdp_value = "0xED"
            return  # No need to program RDP level 0
        elif final_rdp_level == RDPLevel.RDP_2_BS.value:
            final_rdp_value = "0xD1"
        elif final_rdp_level == RDPLevel.RDP_2.value:
            final_rdp_value = "0x72"
        else:
            raise ValueError(f"Invalid final RDP level: {final_rdp_level}")
        self.cube_programmer.set_option_bytes({"RDP_level": final_rdp_value},
                                              reset=False, mode=CubeProgrammerMode.HOT_PLUG)


def SetupInstallationStep(config: Provisioning_Config, stlink_sn: str = None) -> Step:
    """
    Set up the installation step by adding all necessary substeps.

    Args:
        config (Provisioning_Config): The provisioning configuration object.
        stlink_sn (str, optional): The serial number of the ST-Link device. Defaults to None.

    Returns:
        Step: The fully configured installation step.
    """
    example_config = Example_Config(config.get_example_json_path())
    memory_config = Memory_Config(config.get_memory_config_ini_path(), example_config)
    flash_layout = FlashLayout_OEMiRoT(example_config, memory_config)
    tools_config = Tools_Config(config.get_tools_bin_env_name())
    cube_programmer = CubeProgrammerRDP(tools_config, stlink_sn=stlink_sn)
    board = example_config.get_board()
    installation_step = InstallationStep(board_name=board.get("name"))
    installation_step.add_step(RemoveOBStep(example_config, cube_programmer, flash_layout))
    installation_step.add_step(ProgramRDPKeyStep(config, cube_programmer))
    installation_step.add_step(InstallationSubstep(config, cube_programmer))
    installation_step.add_step(ProgramOBStep(example_config, cube_programmer, flash_layout))
    installation_step.add_step(ProgramFinalRDPLevelStep(config, cube_programmer))
    return installation_step
