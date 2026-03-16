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

import json
import os
import logging

from enum import Enum
from typing import Dict, List, Union
from helper.common.singleton import Singleton
from helper.exception.rot_exception import RoTFileNotFoundError, RoTNotFoundError, RoTAbsolutePathError

logger = logging.getLogger()


class LifecycleType(Enum):
    RDP = "rdp"
    PRODUCT_STATE = "product_state"


class Example_Config(metaclass=Singleton):
    """
    Example_Config is a base class for retrieving configuration example in JSON-based files using the Singleton pattern.

    Args:
        json_path (str): The absolute path to the JSON configuration file to be loaded.

    Attributes:
        json_path (str): Stores the path to the Example configuration file.
        config (dict): The loaded configuration data as a Python dictionary.

    Methods:
        get_category(category : str) -> Union[Dict, List, str, int, float, bool, None]:
            Retrieves the value associated with a specified top-level category.

        get_board() -> Dict:
            Retrieves the value associated with board category.

        get_memory() -> Dict:
            Retrieves the value associated with memory category.

        get_general() -> Dict:
            Retrieves the value associated with general category.

        get_option_bytes() -> Dict:
            Retrieves the value associated with option_bytes category.

        get_mcuboot() -> Dict:
            Retrieves the value associated with mcuboot category.

        get_lifecycle_type() -> LifecycleType:
            Retrieves the lifecycle type from the board category.
    Raises:
        RoTAbsolutePathError: If the JSON path is not an absolute path.
        RoTFileNotFoundError: If the JSON configuration file does not exist at the specified path.
        json.JSONDecodeError: If the JSON file is not properly formatted.
    """

    def __init__(self, json_path: str):
        if (not os.path.isabs(json_path)):
            raise RoTAbsolutePathError(json_path)
        self.json_path = json_path
        if (os.path.exists(json_path)):
            with open(json_path, "r") as f:
                self.config = json.load(f)
        else:
            raise RoTFileNotFoundError(json_path)

    def get_category(self, category: str) -> Union[Dict, List, str, int, float, bool, None]:
        """
        Retrieves the value associated with a specified top-level category in the JSON.

        Args:
            category (str): The name of the top-level category to retrieve.

        Returns:
            object: The value associated with the specified category key.This can be:
                - a dict (if the category contains subcategories or nested data),
                - a list
                - a primitive type

        Raises:
            RoTNotFoundError: If the category does not exist in the JSON.

        Note:
            This method only retrieves top-level categories. For nested or subcategories,
            access them directly from the returned object.
        """
        try:
            return self.config[category]
        except KeyError:
            raise RoTNotFoundError(f"Category '{category}' not found in '{self.json_path}'.")

    def get_board(self) -> Dict:
        """
        Retrieves the value associated with board category in the JSON.

        Returns:
            Dict: A dictionary associated with the board category key.
        """
        return self.get_category("board")

    def get_memory(self) -> Dict:
        """
        Retrieves the value associated with memory category in the JSON.

        Returns:
            Dict: A dictionary associated with the memory category key.
        """
        return self.get_category("memory")

    def get_general(self) -> Dict:
        """
        Retrieves the value associated with general category in the JSON.

        Returns:
            Dict: A dictionary associated with the general category key.
        """
        return self.get_category("general")

    def get_option_bytes(self) -> Dict:
        """
        Retrieves the value associated with option_bytes category in the JSON.

        Returns:
            Dict: A dictionary associated with the option_bytes category key.
        """
        return self.get_category("option_bytes")

    def get_mcuboot(self) -> Dict:
        """
        Retrieves the value associated with mcuboot category in the JSON.

        Returns:
            Dict: A dictionary associated with the mcuboot category key.
        """
        return self.get_category("mcuboot")

    def get_lifecycle_type(self) -> LifecycleType:
        """
        Retrieves the lifecycle type from the board category in the JSON.

        Returns:
            str: The lifecycle type value.
        """
        board_config = self.get_board()
        return LifecycleType(board_config.get("lifecycle", "").lower())
