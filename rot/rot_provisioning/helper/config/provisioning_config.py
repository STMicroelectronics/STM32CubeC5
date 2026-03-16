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

from helper.config.generic_config import Generic_Config
from helper.config.example_config import Example_Config, LifecycleType
from helper.config.memory_config import Memory_Config
from helper.exception.rot_exception import RoTUnexpectedError, RoTValueError
import logging
from enum import Enum

logger = logging.getLogger()


class RDPLevel(Enum):
    """
    Enumeration representing different Readout Protection (RDP) levels for device provisioning.

    Attributes:
        RDP_0 (str): No readout protection.
        RDP_2_BS (str): Readout protection level 2 with boundary scan.
        RDP_2 (str): Full readout protection level 2.
    """
    RDP_0 = "RDP_0"
    RDP_2_BS = "RDP_2_BS"
    RDP_2 = "RDP_2"


class Provisioning_Config(Generic_Config):
    """
    Provisioning_Config is a configuration handler for boot projects.
    This class extends Generic_Config to provide specialized configuration retrieval
    and management for OEMiROT provisioning.

    Example ini file sections (path can be relative or absolute, relative paths are resolved
    relative to the location of the ini file):

        [provisioning]
        path = /path/to/rot_provisioning
        example_json_path = /path/to/example.json
        tools_bin_env_name = STM32_PRG_PATH

        [appli_config]
        appli_ini_path = path/project_appli.ini

        [appli_data_image]
        provisioning_xml_path = /path/to/appli_provisioning_data_image.xml
        fwu_xml_path = /path/to/appli_fwu_data_image.xml

        [oemirot_config]
        oemirot_ini_path = path/project_oemirot.ini
        keys_xml_path = /path/to/oemirot_keys_config.xml
        data_bin_path = /path/oemirot_nvcounters_init.bin
        memory_config_ini_path = /path/to/mx_memory_config.ini

        [rdp]
        transition_bs_key_path = /path/transition_bs_key.bin
        regression_rdp_key_path = /path/regression_rdp_key.bin
        min_rdp_level = RDP_0
        final_rdp_level = RDP_0

    Methods:
        get_config_keys_xml_path() -> str:
            Returns the absolute path to the config keys xml file.
        get_config_data_path() -> str:
            Returns the absolute path to the config data binary.
        get_appli_ini_path() -> str:
            Returns the absolute path to the application ini file.
        get_oemirot_ini_path() -> str:
            Returns the absolute path to the OEMiROT ini file.
        get_tools_bin_env_name() -> str:
            Returns the environment variable name defined in provisioning.ini containing the path to STM32CubeProgrammer bin folder.
        get_rot_provisioning_path() -> str:
            Returns the absolute rot_provisioning path for the project.
        get_example_json_path() -> str:
            Returns the absolute path to the example JSON file.
        get_memory_config_ini_path() -> str:
            Returns the absolute path to the memory config ini file.
        get_appli_data_xml_path(image_type: str) -> str:
            Returns the absolute path to the xml of the appli data
            for the provisioning or the fwu according to the image_type parameter.
        get_transition_bs_key_path() -> str:
            Returns the absolute path to the RDP transition BS key binary file.
        get_regression_rdp_key_path() -> str:
            Returns the absolute path to the RDP regression RDP key binary file.
        get_rdp_level(type: str) -> int:
            Returns the minimum or final RDP level to be set.
        set_rdp_level(type: str, rdp_level: str):
            Sets the minimum or final RDP level to be set.
    """

    def __init__(self, config_path: str):
        super().__init__(config_path)

    def _load_config(self):
        """
        This method retrieves the configuration required for the boot project:
            - The path to config keys xml file.
            - The path to config data binary.
            - The path to OEMiROT output binary.
            - The path to application configuration file.
            - The environment variable name containing the path to STM32CubeProgrammer bin folder.
            - The path to rot_provisioning directory.
            - The path to example JSON file.
            - The path to memory configuration ini file.
            If the data image number is equal to 1, it also retrieves:
            - The path to the appli data provisioning XML file.
            - The path to the appli data fwu XML file.
            If the lifecycle type is RDP, it also retrieves:
            - The path to RDP transition BS key.
            - The path to RDP regression RDP key.
            - The minimum RDP level to be set.
            - The final RDP level to be set.
        """
        self.config_keys_xml_path = self.get_absolute_path_from_config('oemirot_config', 'keys_xml_path')
        self.config_data_path = self.get_absolute_path_from_config('oemirot_config', 'data_bin_path')
        self.oemirot_ini_path = self.get_absolute_path_from_config('oemirot_config', 'oemirot_ini_path')
        self.appli_ini_path = self.get_absolute_path_from_config('appli_config', 'appli_ini_path')
        self.tools_bin_env_name = self.get_config_value('provisioning', 'tools_bin_env_name')
        self.rot_provisioning_path = self.get_absolute_path_from_config('provisioning', 'path')
        self.example_json_path = self.get_absolute_path_from_config('provisioning', 'example_json_path')
        self.example_config = Example_Config(self.example_json_path)
        self.memory_config_ini_path = self.get_absolute_path_from_config('oemirot_config', 'memory_config_ini_path')
        self._load_data_image_number()
        if self.data_image_number == 1:
            self.appli_data_xml_path = {"provisioning": self.get_absolute_path_from_config('appli_data_image', 'provisioning_xml_path'),
                                        "fwu": self.get_absolute_path_from_config('appli_data_image', 'fwu_xml_path')}
        self._load_lifecycle_type()
        if self.lifecycle_type == LifecycleType.RDP:
            self.transition_bs_key_path = self.get_absolute_path_from_config('rdp', 'transition_bs_key_path')
            self.regression_rdp_key_path = self.get_absolute_path_from_config('rdp', 'regression_rdp_key_path')
            self.rdp_level = {"min": self.get_config_value('rdp', 'min_rdp_level'),
                              "final": self.get_config_value('rdp', 'final_rdp_level')}

    def _load_lifecycle_type(self):
        self.lifecycle_type = self.example_config.get_lifecycle_type()

    def _load_data_image_number(self):
        self.data_image_number = int(Memory_Config(self.memory_config_ini_path,
                                     self.example_config).get_data_image_number())

    def _set_print_dict(self):
        self.print_config_items["Tools Environment Variable Name"] = self.tools_bin_env_name
        self.print_config_items["Example JSON Path"] = self.example_json_path
        self.print_config_items["Memory Config INI Path"] = self.memory_config_ini_path
        self.print_config_items["ROT Provisioning Path"] = self.rot_provisioning_path
        self.print_config_items["Config Keys XML Path"] = self.config_keys_xml_path
        self.print_config_items["Config Data Path"] = self.config_data_path
        self.print_config_items["Application INI Path"] = self.appli_ini_path
        self.print_config_items["OEMiROT INI Path"] = self.oemirot_ini_path
        if self.data_image_number > 0:
            self.print_config_items["Appli Data Provisioning XML Path"] = self.appli_data_xml_path["provisioning"]
            self.print_config_items["Appli Data FWU XML Path"] = self.appli_data_xml_path["fwu"]
        if self.lifecycle_type == LifecycleType.RDP:
            self.print_config_items["RDP Transition BS Key Path"] = self.transition_bs_key_path
            self.print_config_items["RDP Regression RDP Key Path"] = self.regression_rdp_key_path
            self.print_config_items["Minimum RDP Level"] = self.rdp_level["min"]
            self.print_config_items["Final RDP Level"] = self.rdp_level["final"]

    def get_config_keys_xml_path(self) -> str:
        """
        Returns the absolute path to the config keys xml file.
        Returns:
            str: The path to the config keys xml file.
        """
        return self.config_keys_xml_path

    def get_config_data_path(self) -> str:
        """
        Returns the absolute path to the config data.
        Returns:
            str: The path to the config data binary file.
        """
        return self.config_data_path

    def get_oemirot_ini_path(self) -> str:
        """
        Returns the absolute path to the OEMiROT ini file.
        Returns:
            str: The path to the OEMiROT ini file.
        """
        return self.oemirot_ini_path

    def get_appli_ini_path(self) -> str:
        """
        Returns the absolute path to the application ini file.
        Returns:
            str: The path to the application ini file.
        """
        return self.appli_ini_path

    def get_tools_bin_env_name(self) -> str:
        """
        Returns the environment variable name defined in provisioning.ini.

        Returns:
            str: The environment variable name where the STM32CubeProgrammer bin folder path is stored.
        """
        return self.tools_bin_env_name

    def get_rot_provisioning_path(self) -> str:
        """
        Returns the absolute rot_provisioning path for the project.

        Returns:
            str: The absolute provisioning path.
        """
        return self.rot_provisioning_path

    def get_example_json_path(self) -> str:
        """
        Returns the absolute path to the example JSON file.

        Returns:
            str: The absolute path to the example JSON file.
        """
        return self.example_json_path

    def get_memory_config_ini_path(self) -> str:
        """
        Returns the absolute path to the memory config ini file.

        Returns:
            str: The absolute path to the memory config ini file.
        """
        return self.memory_config_ini_path

    def get_appli_data_xml_path(self, image_type: str) -> str:
        """
        Returns the absolute path to the xml of the appli data
        for the provisioning or the fwu depending on the type parameter.

        Args:
            image_type (str): The type of the appli data image xml to retrieve.
        Returns:
            str: The absolute path to the xml of the appli data provisioning.
        Raises:
            RoTUnexpectedError: If the data image number is not equal to 1.
        """
        if self.data_image_number != 1:
            raise RoTUnexpectedError("Appli Data XMLs are only available when data image number is equal to 1.")
        return self.appli_data_xml_path[image_type]

    def get_transition_bs_key_path(self) -> str:
        """
        Returns the absolute path to the RDP transition BS key binary file.

        Returns:
            str: The absolute path to the RDP transition BS key binary file.
        Raises:
            RoTUnexpectedError: If the lifecycle type is not RDP.
        """
        if (self.lifecycle_type != LifecycleType.RDP):
            raise RoTUnexpectedError("Transition BS Key is only available for RDP lifecycle type.")
        return self.transition_bs_key_path

    def get_regression_rdp_key_path(self) -> str:
        """
        Returns the absolute path to the RDP regression RDP key binary file.

        Returns:
            str: The absolute path to the RDP regression RDP key binary file.
        Raises:
            RoTUnexpectedError: If the lifecycle type is not RDP.
        """
        if (self.lifecycle_type != LifecycleType.RDP):
            raise RoTUnexpectedError("Regression RDP Key is only available for RDP lifecycle type.")
        return self.regression_rdp_key_path

    def get_rdp_level(self, type: str) -> RDPLevel:
        """
        Returns the 'min' or 'final' RDP level to be set. Exits if the level is not valid.

        Args:
            type (str): The 'min' or 'final' RDP level to retrieve.
        Returns:
            RDPLevel: The 'min' or 'final' RDP level.
        Raises:
            RoTUnexpectedError: If the lifecycle type is not RDP.
            RoTValueError: If the RDP level is not valid.
        """
        cap_type = type.capitalize()
        if (self.lifecycle_type != LifecycleType.RDP):
            raise RoTUnexpectedError(f"{cap_type} RDP Level is only available for RDP lifecycle type.")
        try:
            return RDPLevel(self.rdp_level[type])
        except ValueError:
            raise RoTValueError(
                f"{cap_type} RDP Level '{self.rdp_level[type]}' set in '{self.config_path}' is not a valid RDP level. Please select RDP_0, RDP_2_BS or RDP_2.")

    def set_rdp_level(self, type: str, rdp_level: RDPLevel):
        """
        Sets the 'final' or 'min' RDP level to be set. Exits if the level is not valid.

        Args:
            type (str): The 'min' or 'final' RDP level to set.
            rdp_level (RDPLevel): The 'final' or 'min' RDP level.
        Raises:
            RoTUnexpectedError: If the lifecycle type is not RDP.
            RoTValueError: If the RDP level is not valid.
        """
        cap_type = type.capitalize()
        if (self.lifecycle_type != LifecycleType.RDP):
            raise RoTUnexpectedError(f"{cap_type} RDP Level is only available for RDP lifecycle type.")
        try:
            self.config['rdp'][f'{type}_rdp_level'] = RDPLevel(rdp_level).value
            self._update_config_file(self.config)
            self.rdp_level[type] = rdp_level
        except ValueError:
            raise RoTValueError(
                f"{cap_type} RDP Level '{rdp_level}' set in '{self.config_path}' is not a valid RDP level. Please select RDP_0, RDP_2_BS or RDP_2.")

    def _update_config_file(self, config: Generic_Config) -> None:
        """
        Updates the configuration file with the current settings.
        Args:
            config (Generic_Config): The configuration object to save.
        Returns:
            None
        """
        with open(self.config_path, 'w') as configfile:
            config.write(configfile)

    def check_rdp_key_format(self, type: str, rdp_key: list[str], expected_key_length: int, key_path: str) -> None:
        """
        Checks that the RDP key file at the given path contains the expected number of elements.

        Args:
            type (str): The type of RDP key to check ("transition" or "regression").
            rdp_key (list): A list containing the RDP key elements.
            expected_key_length (int): The expected number of elements in the RDP key.
            key_path (str): The path to the RDP key file.

        Returns:
            None

        Raises:
            RoTValueError: If the RDP key file does not contain the expected number of elements.
        """

        # check that the transition key is not the default value
        if type == "transition":
            if rdp_key[0] == "0xAAAAAAAA":
                raise RoTValueError("RDP transition BS key cannot be the default value '0xAAAAAAAA'")

        # check that the key contains the expected number of elements
        if len(rdp_key) != expected_key_length:
            raise RoTValueError(
                f"RDP key file at '{key_path}' must contain exactly {expected_key_length} element(s), got {len(rdp_key)}.")

    def get_rdp_key(self, type: str) -> list[str]:
        """
        Retrieves the RDP key.

        Args:
            type (str): The type of RDP key to retrieve ("transition" or "regression").

        Returns:
            list[str]: A list containing the RDP key elements.
        """

        if type == "transition":
            key_path = self.get_transition_bs_key_path()
            expected_key_length = 1
        elif type == "regression":
            key_path = self.get_regression_rdp_key_path()
            expected_key_length = 4

        with open(key_path, "rb") as f:
            rdp_key = f.read().decode("utf-8").strip().split()

        self.check_rdp_key_format(type, rdp_key, expected_key_length, key_path)

        return rdp_key
