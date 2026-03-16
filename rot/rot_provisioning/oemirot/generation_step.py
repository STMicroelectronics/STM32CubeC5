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

import logging, os
from helper.common.hxml import extract_output_file_path, update_xml_value_by_title, update_xml_value_by_name
from helper.workflow.step import Step
from helper.tools.tpc import TPC
from helper.tools.tools_config import Tools_Config
from helper.config.appli_config import Appli_Config
from helper.config.oemirot_config import OEMiRoT_Config
from helper.config.memory_config import Memory_Config
from helper.config.provisioning_config import Provisioning_Config, RDPLevel
from helper.config.flash_layout_oemirot import FlashLayout_OEMiRoT
from helper.config.example_config import Example_Config
from helper.exception.rot_exception import RoTFatalError, RoTValueError

logger = logging.getLogger()

# Metadata for the overall generation step
generation_step = {
    "id": "generation",
    "name": "Generation",
    "wait_for_input": False,
    "success": "Successful generation",
    "skip_if_auto": False
}

# Log messages for binary generation
bin_log = {
    "processing": "Processing xml file ...",
    "success": "Successful file generation"
}

# Substeps for the generation process
generation_substeps = {
    "bin_generation": {
        "name": "{name} generation",
        "texts": [{"message": "Build the {name} ({path}) and continue"}],
        "skip_if_auto": True,
    },
}

# Substeps for the generation process
appli_data_step = {
    "id": "appli_data",
    "name": "Appli data generation",
    "wait_for_input": False,
    "success": "Successful appli data generation",
    "skip_if_auto": False
}

# Substeps for the data configuration process
appli_data_configuration_substeps = {
    "appli_data_config":
    {
        "name": "Appli Data {appli_data_xml_type} configuration",
        "texts": [
            {
                "message": "From TrustedPackageCreator (Image Gen tab in Secure panel)"
            },
            {
                "message": "Select {appli_data_xml} as the configuration file"
            },
            {
                "message": "Update the configuration (if/as needed) then generate the {appli_data_output_bin_path} file"
            }
        ],
        "process": bin_log["processing"],
        "success": bin_log["success"],
    },
}

# Metadata for the RDP configuration step
rdp_configuration_step = {
    "id": "rdp_configuration",
    "name": "RDP Configuration",
    "wait_for_input": False,
    "success": "Successful RDP configuration",
    "skip_if_auto": True
}

# Substeps for the RDP configuration process
rdp_configuration_substeps = {
    "rdp_key_update": {
        "name": "{name} update",
        "texts": [{"message": "Update the {name} ({path}) and continue"}],
        "skip_if_auto": True,
    },
    "rdp_selection": {
        "name": "{name} RDP level selection",
        "texts": [{"message": "Updating the {name} RDP level ({path})"}],
        "questions": {
            "message": "Select the {name} RDP level: ",
            "options": ["RDP_0", "RDP_2_BS", "RDP_2"]
        },
        "skip_if_auto": True,
        "wait_for_input": False,
    },
}

# Substeps for the key configuration process
key_configuration_substeps = {
    "key_config":
    {
        "name": "Keys Configuration",
        "texts": [
            {"message": "From TrustedPackageCreator (OBKey/Data Generation tab in Secure panel)"},
            {"message": "Select {key_config_xml} as the configuration file"},
            {"message": "Default keys must NOT be used in a product. Make sure to regenerate your own keys !", "type": "warning"},
            {"message": "Update the configuration (if/as needed) then generate the {key_config_output_bin_path} file"}
        ],
        "wait_for_input": True,
        "skip_if_auto": False,
        "process": bin_log["processing"],
        "success": bin_log["success"],
    },
}

# Substeps for the user memory configuration process
user_memory_config_substep = {
    "user_memory_config": {
        "name": "User Memory Configuration",
        "texts": [{"message": "Check the memory configuration in {path}"}],
        "questions": {
            "message": "Is this user memory configuration correct ? ",
            "options": ["Yes", "No"]
        },
        "wait_for_input": False,
        "skip_if_auto": True,
    }
}


class GenerationStep(Step):
    """
    Represents the main generation step in the workflow.
    """

    def __init__(self):
        super().__init__(generation_step)

    def process_step(self):
        pass


class RDPConfigurationStep(Step):
    """
    Represents the RDP configuration step in the workflow.
    """

    def __init__(self):
        super().__init__(rdp_configuration_step)

    def process_step(self):
        pass


class BinaryGeneration(Step):
    """
    Represents the binary generation substep in the generation process.

    Args:
        name (str): The name of the binary being generated.
        bin_path (str): The path where the binary will be generated.
    """

    def __init__(self, name: str, bin_path: str):
        super().__init__(generation_substeps["bin_generation"])
        self.log_param = {'name': name, 'path': bin_path}

    def process_step(self):
        pass


class AppliDataStep(Step):
    """
    Represents the appli data generation step in the workflow.
    """

    def __init__(self):
        super().__init__(appli_data_step)

    def process_step(self):
        pass


class AppliDataConfig(Step):
    """
    Represents the appli data configuration and generation substep.

    Args:
        config: The configuration object containing paths and settings.
    """

    def __init__(self, appli_data_xml_type: str, config: Provisioning_Config):
        super().__init__(appli_data_configuration_substeps["appli_data_config"])
        self.key_config_xml = config.get_config_keys_xml_path()
        self.appli_data_bin_xml = config.get_appli_data_xml_path(appli_data_xml_type)
        self.appli_data_type = appli_data_xml_type
        self.log_param = {
            'appli_data_xml_type': self.appli_data_type,
            'appli_data_xml': self.appli_data_bin_xml,
            'appli_data_output_bin_path': extract_output_file_path(self.appli_data_bin_xml)}
        self.tpc = TPC(Tools_Config(config.get_tools_bin_env_name()))
        self.example_config = Example_Config(config.get_example_json_path())
        self.flash_layout = FlashLayout_OEMiRoT(self.example_config,
                                                Memory_Config(config.get_memory_config_ini_path(), self.example_config))
        self.base = int(self.example_config.get_memory()["base"], 16)

    def _data_xml_update(self):
        if self.appli_data_type == "provisioning":
            data_size = hex(self.flash_layout.get_flash_layout("FLASH_PRIMARY_DATA_SLOT_SIZE"))
            data_offset = hex(self.flash_layout.get_flash_layout("DATA_HEX_FORMAT"))
            update_xml_value_by_name(self.appli_data_bin_xml, "Param",
                                     "Firmware download area offset", "Value", data_offset)
        elif self.appli_data_type == "fwu":
            data_size = hex(self.flash_layout.get_flash_layout("FLASH_SECONDARY_DATA_SLOT_SIZE"))
        else:
            raise RoTFatalError(f"Unknown type of the appli data xml: {self.appli_data_type}")
        update_xml_value_by_name(self.appli_data_bin_xml, "Param", "Firmware area size", "Value", data_size)
        rel_path_in_image_xml = os.path.relpath(
            self.key_config_xml, os.path.dirname(self.appli_data_bin_xml))
        update_xml_value_by_title(self.appli_data_bin_xml, "GlobalParam",
                                  "Data image generation", "LinkedXML", rel_path_in_image_xml)

    def process_step(self):
        self._data_xml_update()
        self.tpc.gen_image(self.appli_data_bin_xml)


class RDPKeyUpdate(Step):
    """
    Represents the RDP key update substep in the generation process.

    Args:
        name (str): The name of the RDP key being updated.
        rdp_key_path (str): The path where the RDP key will be updated.
    """

    def __init__(self, name: str, config: Provisioning_Config):
        super().__init__(rdp_configuration_substeps["rdp_key_update"])
        if name == "Regression RDP key file":
            rdp_key_path = config.get_regression_rdp_key_path()
            self.type = "regression"
        elif name == "Transition RDP bs key file":
            rdp_key_path = config.get_transition_bs_key_path()
            self.type = "transition"
        self.log_param = {'name': name, 'path': rdp_key_path}
        self.name = name
        self.config = config

    def process_step(self):
        message = f"Updating {self.log_param['name']} at {self.log_param['path']}"
        logger.debug(message)
        rdp_key = self.config.get_rdp_key(self.type)
        logger.debug(f"{self.log_param['name']}: {rdp_key}")


class RDPSelection(Step):
    """
    Represents the RDP selection substep in the generation process.

    Args:
        name (str): The name of the final RDP level being selected.
        config (Provisioning_Config): The configuration object containing paths and settings.
    """

    def __init__(self, name: str, config: Provisioning_Config):
        super().__init__(rdp_configuration_substeps["rdp_selection"])
        self.name = name
        self.log_param = {'name': self.name.capitalize(), 'path': config.config_path}
        self.config = config

    def process_step(self):
        # Compare final choice with min RDP level
        if self.name == "final":
            self._compare_final_with_min_rdp_levels()
        self.config.set_rdp_level(self.name, self.choice)

    def _compare_final_with_min_rdp_levels(self) -> None:
        """
        Compare the final choice RDP level with the min RDP levels from the config.
        Raises:
            RoTValueError: If the final choice RDP level is lower than the min RDP level.
        """
        rdp_order = {
            RDPLevel.RDP_0: 0,
            RDPLevel.RDP_2_BS: 1,
            RDPLevel.RDP_2: 2
        }
        min_level = RDPLevel(self.config.get_rdp_level("min"))
        final_level = RDPLevel(self.choice)
        if rdp_order[final_level] < rdp_order[min_level]:
            raise RoTValueError(
                f"Final RDP level '{self.choice}' cannot be lower than minimum RDP level ('{min_level.value}').")


class KeyConfigurationStep(Step):
    """
    Represents the key update substep in the generation process.

    Args:
        config: The configuration object containing paths and settings.
    """

    def __init__(self, config: Provisioning_Config):
        super().__init__(key_configuration_substeps["key_config"])
        self.key_config_xml = config.get_config_keys_xml_path()
        self.log_param = {'key_config_xml': self.key_config_xml,
                          'key_config_output_bin_path': extract_output_file_path(self.key_config_xml)}
        self.tpc = TPC(Tools_Config(config.get_tools_bin_env_name()))

    def process_step(self):
        self.tpc.gen_obk(self.key_config_xml)


class UserMemoryConfigurationStep(Step):
    """
    Represents the user memory configuration substep in the generation process.

    Args:
        memory_config_path (str): Path to mx_memory_config.ini.
    """

    def __init__(self, memory_config_path: str):
        super().__init__(user_memory_config_substep["user_memory_config"])
        self.log_param = {'path': memory_config_path}

    def process_step(self):
        if self.choice == "No":
            raise RoTValueError(
                f"Update the memory configuration in '{self.log_param['path']}' and restart the provisioning.")


def SetupGenerationStep(config: Provisioning_Config) -> Step:
    """
    Set up the generation step by adding all necessary substeps.

    Args:
        config: The configuration object containing paths and settings.

    Returns:
        Step: The fully configured generation step.
    """
    generationStep = GenerationStep()
    memoryConfigStep = UserMemoryConfigurationStep(config.get_memory_config_ini_path())
    oemirot_config = OEMiRoT_Config(config.get_oemirot_ini_path())
    generationStep.add_step(memoryConfigStep)
    key_config_step = KeyConfigurationStep(config)
    generationStep.add_step(key_config_step)
    generationStep.add_step(SetupRDPConfigurationStep(config))
    generationStep.add_step(BinaryGeneration("OEMiRoT boot binary",
                            oemirot_config.get_build_binary_path()))
    appli_config = Appli_Config(config.get_appli_ini_path())
    generationStep.add_step(BinaryGeneration("Application image", appli_config.get_image_binary_path("provisioning")))
    memory_config = Memory_Config(config.get_memory_config_ini_path(), Example_Config(config.get_example_json_path()))
    appli_data_nb = int(memory_config.get_data_image_number(), 16)
    if appli_data_nb > 0:
        generationStep.add_step(SetupAppliDataConfigurationStep(config))
    return generationStep


def SetupAppliDataConfigurationStep(config: Provisioning_Config) -> Step:
    """
    Set up the Appli Data configuration and generation step by adding all necessary substeps.

    Args:
        config: The configuration object containing paths and settings.

    Returns:
        Step: The fully configured Appli Data configuration step.
    """
    appliDataStep = AppliDataStep()
    appliDataStep.add_step(AppliDataConfig("provisioning", config))
    appliDataStep.add_step(AppliDataConfig("fwu", config))
    return appliDataStep


def SetupRDPConfigurationStep(config: Provisioning_Config) -> Step:
    """
    Set up the RDP configuration step by adding all necessary substeps.

    Args:
        config: The configuration object containing paths and settings.

    Returns:
        Step: The fully configured RDP configuration step.
    """
    rdpConfigStep = RDPConfigurationStep()
    rdpConfigStep.add_step(RDPKeyUpdate("Transition RDP bs key file", config))
    rdpConfigStep.add_step(RDPKeyUpdate("Regression RDP key file", config))
    rdpConfigStep.add_step(RDPSelection("min", config))
    rdpConfigStep.add_step(RDPSelection("final", config))
    return rdpConfigStep
