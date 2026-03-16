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
from helper.common.singleton import Singleton
from helper.config.example_config import Example_Config
from helper.config.memory_config import Memory_Config
from helper.exception.rot_exception import RoTNotFoundError, RoTNegativeValueError, RoTNotImplementedError

logger = logging.getLogger()


class Generic_FlashLayout(metaclass=Singleton):
    """
    Generic_FlashLayout is a class that provide the basics functions to calculates and manages the flash memory layout.

    Args:
        example_config (Example_Config): An instance of Example_Config containing memory and mcuboot configurations.
        memory_config (Memory_Config): An instance of Memory_Config containing data size, data image number, and application size.
    Attributes:
        memory_config (Memory_Config): The memory configuration object.
        memory_hw (dict): A dictionary containing hardware memory details retrieved from example_config.
        mcuboot (dict): A dictionary containing mcuboot details retrieved from example_config.
        memory_block_size (int): The size of the memory block in bytes.
        memory_prog_size (int): The size of the memory programming in bytes.
        img_install_req_size (int): The required size for image installation in bytes.
        img_version_offset (int): The offset for the image version in bytes.
        data_size (int): The size of the data in bytes.
        nb_data_image (int): The number of data images.
        app_size (int): The size of the application in bytes.
        flash_layout (dict): A dictionary representing the calculated flash layout, initialized by calling _calculate_flash_layout().

    Methods:
        _calculate_flash_layout() -> dict:
            Abstract method to be implemented by subclass for calculates all flash layout defines
            and returns them as a dictionary.
        _validate_flash_layout() -> None:
            Abstract method to be implemented by subclass for validate the flash layout.
        print_flash_layout(flash_layout: dict):
            Prints the flash layout as a table in hexadecimal format.
        get_flash_layout(name: str) -> int:
            Retrieves the value of a specific define from the flash layout.
        get_all_flash_layout() -> dict:
            Retrieves all define values from the flash layout.
        _compute_data_size(data_image_size: int, nb_data_image: int) -> int:
            Computes the data size based on the number of data image.
        _compute_ob_end_offset(slot_offset: int) -> int:
            Computes whether HDP or WRP end offset.
        _compute_install_req_offset(slot_offset: int, slot_size: int, image_install_req_size: int) -> int:
            Computes the offset of the request install magic for a given slot.
        _compute_confirm_offset(slot_offset: int, slot_size: int, image_install_req_size: int, memory_prog_size: int) -> int:
            Computes the offset of the confirm flag for a given slot.
    Raises:
        RoTNotImplementedError: If _calculate_flash_layout method is not implemented by a subclass.
        RoTValueError: If computed offsets are negative.
        RoTNotFoundError: If the define name is not found in the flash layout.
    """

    def __init__(self, example_config: Example_Config, memory_config: Memory_Config):
        self.memory_config = memory_config
        self.memory_hw = example_config.get_memory()
        self.mcuboot = example_config.get_mcuboot()

        self.isolation = example_config.get_general()["isolation"]

        self.memory_block_size = int(self.memory_hw["block_size"], 16)
        self.memory_prog_size = int(self.memory_hw["prog_size"], 16)
        self.base = int(self.memory_hw["base"], 16)
        self.vtor_alignment = int(self.memory_hw["vtor_alignment"], 16)

        self.img_install_req_size = int(self.mcuboot["image_install_req_size"], 16)
        self.img_version_offset = int(self.mcuboot["image_version_offset"], 16)

        self.data_size = int(self.memory_config.get_data_size(), 16)
        self.nb_data_image = int(self.memory_config.get_data_image_number(), 16)
        self.app_size = int(self.memory_config.get_app_size(), 16)

        try:
            self.flash_layout = self._calculate_flash_layout()
            self._validate_flash_layout()
        except RoTNotImplementedError:
            self.flash_layout = {}

    def _calculate_flash_layout(self) -> dict:
        """
        This method should be implemented by subclasses to calculates all flash layout defines.


        Raises:
            RoTNotImplementedError: If the method is not implemented by a subclass.
        """
        raise RoTNotImplementedError("Subclasses should implement this method")

    def _validate_flash_layout(self) -> None:
        """
        This method should be implemented by subclasses to validate the flash layout.

        Raises:
            RoTNotImplementedError: If the method is not implemented by a subclass.
        """
        raise RoTNotImplementedError("Subclasses should implement this method")

    def print_flash_layout(self) -> None:
        """
        Prints the flash layout as a table in hexadecimal format.

        """
        logger.debug(f"{'Define Name':<40} {'Value (Hexadecimal)':<20}")
        logger.debug("-" * 60)
        for key, value in self.flash_layout.items():
            logger.debug(f"{key:<40} 0x{value:08X}")

    def get_flash_layout(self, name: str) -> int:
        """
        Retrieves the value of a specific define.

        Args:
            name (str): The name of the define to retrieve.

        Returns:
            int: The value of the define in decimal.

        Raises:
            RoTNotFoundError: If the define name is not found in the flash layout.
        """
        value = self.flash_layout.get(name, None)
        if value is None:
            raise RoTNotFoundError(f"Define '{name}' not found in flash layout.")
        return value

    def get_all_flash_layout(self) -> dict:
        """
        Retrieves all define values.

        Returns:
            dict: A dictionary containing all define values.
        """
        return self.flash_layout

    def _compute_data_size(self, data_image_size: int, nb_data_image: int) -> int:
        """
        Computes the data size based on the number of data image.

        Args:
            data_image_size (int): The size of a data image.
            nb_data_image (int): The number of data image.

        Returns:
            int: The total data size.
        """
        if nb_data_image == 1:
            return data_image_size
        return 0

    def _compute_ob_end_offset(self, slot_offset: int) -> int:
        """
        Computes either HDP or WRP end offset.

        Args:
            slot_offset (int): Offset of the first slot that is not under HDP or WRP.

        Returns:
            int: Offset of the HDP or WRP end.

        Raises:
            RoTNegativeValueError: If the computed offset is negative.
        """
        result = slot_offset - 1
        if result < 0:
            raise RoTNegativeValueError("ob end offset", result)
        return result

    def _compute_install_req_offset(self, slot_offset: int, slot_size: int,
                                    image_install_req_size: int) -> int:
        """
        Computes the offset of the request install magic for a given slot.

        Args:
            slot_offset (int): Offset of the slot where the request install magic must be retrieved.
            slot_size (int): Size of the slot.
            image_install_req_size (int): Size of the request install magic.

        Returns:
            int: Offset of the request install magic.

        Raises:
            RoTNegativeValueError: If the computed offset is negative.
        """
        result = slot_offset + slot_size - image_install_req_size
        if result < 0:
            raise RoTNegativeValueError("install req offset", result)
        return result

    def _compute_confirm_offset(self, slot_offset: int, slot_size: int,
                                image_install_req_size: int, memory_prog_size: int) -> int:
        """
        Computes the offset of the confirm flag for a given slot.

        Args:
            slot_offset (int): Offset of the slot where the confirm flag must be retrieved.
            slot_size (int): Size of the slot.
            image_install_req_size (int): Size of the request install magic.
            memory_prog_size (int): Size of the memory programmation.

        Returns:
            int: Offset of the confirm flag.

        Raises:
            RoTNegativeValueError: If the computed offset is negative.
        """
        result = slot_offset + slot_size - (image_install_req_size + memory_prog_size)
        if result < 0:
            raise RoTNegativeValueError("confirm offset", result)
        return result
