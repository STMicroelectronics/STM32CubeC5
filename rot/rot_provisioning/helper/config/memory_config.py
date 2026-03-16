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
import logging

from helper.config.example_config import Example_Config
from helper.exception.rot_exception import RoTValueError
logger = logging.getLogger()


class Memory_Config(Generic_Config):
    """
    Memory_Config is a configuration handler for user memory settings.
    This class extends Generic_Config to provide specialized configuration retrieval
    and management for memory configurations.

    Example ini file sections:

        [image_number]
        data_image = 0

        [partition_size]
        oemirot_size = 0x18000
        app_size = 0xC0000
        data_size = 0x2000

    Methods:
        get_data_image_number() -> int:
            Returns the data image number.
        get_oemirot_size() -> int:
            Returns the OEMiROT size.
        get_app_size() -> int:
            Returns the application size.
        get_data_size() -> int:
            Returns the data size.
    """

    def __init__(self, config_path: str, example_config: Example_Config):
        self.example_config = example_config
        super().__init__(config_path)

    def _load_config(self):
        """
        This method retrieves the user memory configuration required for the flash layout computation:
            - The number of data image.
            - The size of the OEMiROT size.
            - The size of the application.
            - The size of the data.
        """
        self.data_image_nb = self.get_config_value('image_number', 'data_image')
        self.oemirot_size = self.get_config_value('partition_size', 'oemirot_size')
        self.app_size = self.get_config_value('partition_size', 'app_size')
        self.data_size = self.get_config_value('partition_size', 'data_size')
        self._validate_config()

    def _set_print_dict(self):
        self.print_config_items["Data Image Number"] = self.data_image_nb
        self.print_config_items["OEMiRoT Size"] = self.oemirot_size
        self.print_config_items["Application Size"] = self.app_size
        self.print_config_items["Data Size"] = self.data_size

    def get_data_image_number(self) -> int:
        """
        Returns the number of data image from the configuration.

        Returns:
            int: The data image number.
        """
        return self.data_image_nb

    def get_oemirot_size(self) -> int:
        """
        Returns the OEMiRoT size from the configuration.

        Returns:
            int: The OEMiRoT size.
        """
        return self.oemirot_size

    def get_app_size(self) -> int:
        """
        Returns the application size from the configuration.

        Returns:
            int: The application size.
        """
        return self.app_size

    def get_data_size(self) -> int:
        """
        Returns the data size from the configuration.

        Returns:
            int: The data size.
        """
        return self.data_size

    def _validate_config(self) -> bool:
        """
        Validates the memory configuration parameters.

        Returns:
            bool: True if all configuration parameters are valid, False otherwise.
        """
        memory = self.example_config.get_memory()
        memory_block_size = int(memory["block_size"], 16)
        wrp_grp_size = memory["wrpgrp_size"]
        has_obk = self.example_config.get_board()["obk"]

        self._validate_data_config()
        self._validate_oemirot_size(memory_block_size, wrp_grp_size, has_obk)
        self._validate_app_size(memory_block_size)
        if self.data_image_nb == "1":
            self._validate_data_size(memory_block_size)

    def _validate_data_config(self) -> bool:
        """
        Validates the data memory configuration parameters.

        Raises:
            RoTValueError: If any configuration parameter is invalid.
        """
        # Checking data image
        if (self.data_image_nb != "0" and self.data_image_nb != "1"):
            raise RoTValueError(f"Data image number must be 0 or 1. Please update '{self.config_path}'.")

    def _validate_oemirot_size(self, memory_block_size: int, wrp_grp_size: int, has_obk: bool) -> bool:
        """
        Validates the OEMiRoT size configuration parameter.

        Arguments:
            memory_block_size (int): The memory block size.
            wrp_grp_size (int): The WRP group size.
            has_obk (bool): Indicates if OBK is enabled.

        Raises:
            RoTValueError: If the OEMiRoT size is invalid.
        """
        oemirot_size_int = int(self.oemirot_size, 16)
        if ((oemirot_size_int % memory_block_size) != 0):
            raise RoTValueError(
                f"OEMiRoT size ({self.oemirot_size}) must be a multiple of the memory block size ({hex(memory_block_size)}). Please update {self.config_path}.")
        if ((not has_obk) and ((oemirot_size_int + memory_block_size) % (wrp_grp_size * memory_block_size)) != 0):
            raise RoTValueError(
                f"OEMiRoT size + memory block size ({hex(oemirot_size_int + memory_block_size)}) must be a multiple of the memory block size * WRP group size ({hex(wrp_grp_size * memory_block_size)}). Please update {self.config_path}.")
        if (has_obk and (oemirot_size_int % (wrp_grp_size * memory_block_size)) != 0):
            raise RoTValueError(
                f"OEMiRoT size ({self.oemirot_size}) must be a multiple of the memory block size * WRP group size ({hex(wrp_grp_size * memory_block_size)}). Please update {self.config_path}.")

    def _validate_app_size(self, memory_block_size: int) -> bool:
        """
        Validates the application size configuration parameter.

        Arguments:
            memory_block_size (int): The memory block size.

        Raises:
            RoTValueError: If the application size is invalid.
        """
        app_size_int = int(self.app_size, 16)
        if (app_size_int % memory_block_size != 0):
            raise RoTValueError(
                f"Application size ({self.app_size}) is not aligned to memory block size ({hex(memory_block_size)}). Please update {self.config_path}.")

    def _validate_data_size(self, memory_block_size: int) -> bool:
        """
        Validates the data size configuration parameter.

        Arguments:
            memory_block_size (int): The memory block size.

        Raises:
            RoTValueError: If the data size is invalid.
        """

        data_size_int = int(self.data_size, 16)
        if not data_size_int:
            raise RoTValueError(
                f"Data size is zero. Please update {self.config_path}.")
        if (data_size_int % memory_block_size != 0):
            raise RoTValueError(
                f"Data size ({self.data_size}) is not aligned to memory block size ({hex(memory_block_size)}). Please update {self.config_path}.")
