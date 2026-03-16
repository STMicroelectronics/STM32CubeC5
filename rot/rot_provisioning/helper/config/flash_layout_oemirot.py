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

from helper.config.generic_flash_layout import Generic_FlashLayout
from helper.exception.rot_exception import RoTNotImplementedError, RoTValueError


class FlashLayout_OEMiRoT(Generic_FlashLayout):
    """
    FlashLayout is a class that calculates and manages the flash memory layout for a specific hardware configuration.

    Methods:
        _calculate_flash_layout() -> dict:
            Calculates all flash layout defines and returns them as a dictionary.
        _validate_flash_layout() -> None:
            Validates the computed flash layout to ensure memory regions do not exceed total available memory.
    Raises:
        RoTNotImplementedError: If the isolation flag is activated.
        RoTValueError: If the computed flash layout exceeds the total memory size.
    """

    def _calculate_flash_layout(self) -> dict:
        """
        Calculates the flash layout definitions for memory regions.
        This method computes the following flash layout definitions:
        - FLASH_ROT_CODE_OFFSET: Offset for the Root of Trust (RoT) code region.
        - FLASH_ROT_CODE_SIZE: Size of the RoT code region.
        - FLASH_KEYS_OFFSET: Offset for the keys region.
        - FLASH_NVCNT_OFFSET: Offset for the non-volatile counter region.
        - FLASH_HASH_REF_OFFSET: Offset for the hash reference region.
        - FLASH_PRIMARY_DATA_SLOT_OFFSET: Offset for the primary data slot.
        - FLASH_PRIMARY_DATA_SLOT_SIZE: Size of the primary data slot.
        - FLASH_PRIMARY_APP_SLOT_OFFSET: Offset for the primary application slot.
        - FLASH_PRIMARY_APP_SLOT_SIZE: Size of the primary application slot.
        - FLASH_SECONDARY_APP_SLOT_OFFSET: Offset for the secondary application slot.
        - FLASH_SECONDARY_APP_SLOT_SIZE: Size of the secondary application slot.
        - FLASH_SECONDARY_DATA_SLOT_OFFSET: Offset for the secondary data slot.
        - FLASH_SECONDARY_DATA_SLOT_SIZE: Size of the secondary data slot.
        - FLASH_ROT_WRP_START_OFFSET: Start offset for the write protection region.
        - FLASH_ROT_WRP_END_OFFSET: End offset for the write protection region.
        - FLASH_ROT_HDP_END_OFFSET: End offset for the high data protection region.
        - DATA_IMAGE_NUMBER: Number of data images.
        - FLASH_SECONDARY_APP_INSTALL_REQ_OFFSET: Offset for the secondary application install request.
        - FLASH_SECONDARY_DATA_INSTALL_REQ_OFFSET: Offset for the secondary data install request.
        - FLASH_PRIMARY_APP_CONFIRM_OFFSET: Offset for the primary application confirmation.
        - FLASH_PRIMARY_DATA_CONFIRM_OFFSET: Offset for the primary data confirmation.
        - FLASH_PRIMARY_APP_VERSION_OFFSET: Offset for the primary application version.
        - FLASH_PRIMARY_DATA_VERSION_OFFSET: Offset for the primary data version.

        Returns:
            dict: A dictionary containing all calculated flash layout definitions and their corresponding values.
        """
        flash_layout = {}

        # Calculate flash layout definitions
        # Define the offset and size for the RoT (Root of Trust) code region
        rot_code_offset = 0
        rot_code_size = int(self.memory_config.get_oemirot_size(), 16)

        # Calculate the offset for RoT internal handling
        keys_offset = rot_code_offset + rot_code_size
        nvcnt_offset = keys_offset + self.memory_block_size
        hash_ref_offset = nvcnt_offset + self.memory_block_size

        # Calculate the size and offset of data and application slots
        primary_data_slot_offset = hash_ref_offset + self.memory_block_size
        data_slot_size = self._compute_data_size(self.data_size, self.nb_data_image)
        primary_app_slot_offset = primary_data_slot_offset + data_slot_size
        app_slot_size = self.app_size
        secondary_app_slot_offset = primary_app_slot_offset + app_slot_size
        secondary_data_slot_offset = secondary_app_slot_offset + app_slot_size

        # Compute the end offset for WRP and HDP region for RoT internal handling
        rot_wrp_end_offset = self._compute_ob_end_offset(nvcnt_offset)
        rot_hdp_end_offset = self._compute_ob_end_offset(primary_data_slot_offset)

        # Compute the offset for Application internal handling
        secondary_app_install_req_offset = self._compute_install_req_offset(
            secondary_app_slot_offset, app_slot_size, self.img_install_req_size
        )
        primary_app_confirm_offset = self._compute_confirm_offset(
            primary_app_slot_offset, app_slot_size, self.img_install_req_size, self.memory_prog_size
        )
        primary_app_version_offset = primary_app_slot_offset + self.img_version_offset

        if self.nb_data_image == 0:
            secondary_data_install_req_offset = 0
            primary_data_confirm_offset = 0
            primary_data_version_offset = 0
        else:
            secondary_data_install_req_offset = self._compute_install_req_offset(
                secondary_data_slot_offset, data_slot_size, self.img_install_req_size
            )
            primary_data_confirm_offset = self._compute_confirm_offset(
                primary_data_slot_offset, data_slot_size, self.img_install_req_size, self.memory_prog_size
            )
            primary_data_version_offset = primary_data_slot_offset + self.img_version_offset

        rom_appli_address = self.base + primary_app_slot_offset + \
            self.vtor_alignment
        # Fill the dictionary with calculated flash layout definitions
        flash_layout["FLASH_ROT_CODE_OFFSET"] = rot_code_offset
        flash_layout["FLASH_ROT_CODE_SIZE"] = rot_code_size
        flash_layout["FLASH_KEYS_OFFSET"] = keys_offset
        flash_layout["FLASH_NVCNT_OFFSET"] = nvcnt_offset
        flash_layout["FLASH_HASH_REF_OFFSET"] = hash_ref_offset

        flash_layout["FLASH_PRIMARY_DATA_SLOT_OFFSET"] = primary_data_slot_offset
        flash_layout["FLASH_PRIMARY_DATA_SLOT_SIZE"] = data_slot_size
        flash_layout["FLASH_PRIMARY_APP_SLOT_OFFSET"] = primary_app_slot_offset
        flash_layout["FLASH_PRIMARY_APP_SLOT_SIZE"] = app_slot_size
        flash_layout["FLASH_SECONDARY_APP_SLOT_OFFSET"] = secondary_app_slot_offset
        flash_layout["FLASH_SECONDARY_APP_SLOT_SIZE"] = app_slot_size
        flash_layout["FLASH_SECONDARY_DATA_SLOT_OFFSET"] = secondary_data_slot_offset
        flash_layout["FLASH_SECONDARY_DATA_SLOT_SIZE"] = data_slot_size

        flash_layout["FLASH_ROT_WRP_START_OFFSET"] = flash_layout["FLASH_ROT_CODE_OFFSET"]
        flash_layout["FLASH_ROT_WRP_END_OFFSET"] = rot_wrp_end_offset
        flash_layout["FLASH_ROT_HDP_END_OFFSET"] = rot_hdp_end_offset

        flash_layout["DATA_IMAGE_NUMBER"] = self.nb_data_image
        flash_layout["FLASH_SECONDARY_APP_INSTALL_REQ_OFFSET"] = secondary_app_install_req_offset
        flash_layout["FLASH_SECONDARY_DATA_INSTALL_REQ_OFFSET"] = secondary_data_install_req_offset
        flash_layout["FLASH_PRIMARY_APP_CONFIRM_OFFSET"] = primary_app_confirm_offset
        flash_layout["FLASH_PRIMARY_DATA_CONFIRM_OFFSET"] = primary_data_confirm_offset
        flash_layout["FLASH_PRIMARY_APP_VERSION_OFFSET"] = primary_app_version_offset
        flash_layout["FLASH_PRIMARY_DATA_VERSION_OFFSET"] = primary_data_version_offset

        # For application
        flash_layout["ROM_APPLI_ADDRESS"] = rom_appli_address
        flash_layout["ROM_APPLI_SIZE"] = app_slot_size - self.vtor_alignment
        flash_layout["ROM_APPLI_HEX_FORMAT"] = self.base + primary_app_slot_offset

        # For application data
        flash_layout["DATA_HEX_FORMAT"] = self.base + primary_data_slot_offset

        # For oemirot
        flash_layout["ROM_ROT_ADDRESS"] = self.base
        flash_layout["ROM_ROT_SIZE"] = rot_code_size
        if (self.isolation):
            raise RoTNotImplementedError("Isolation mode is not supported yet.")
        else:
            flash_layout["MCUBOOT_S_DATA_IMAGE_NUMBER"] = self.nb_data_image
            flash_layout["MCUBOOT_NS_DATA_IMAGE_NUMBER"] = 0
        return flash_layout

    def _validate_flash_layout(self) -> None:
        """
        Validates the computed flash layout to ensure memory regions do not exceed total available memory.

        Raises:
            RoTValueError: If the computed flash layout exceeds the total memory size.
        """
        end_offset = self.flash_layout.get("FLASH_SECONDARY_DATA_SLOT_OFFSET", None) + \
            self.flash_layout.get("FLASH_SECONDARY_DATA_SLOT_SIZE", None) - 1
        total_memory_size = int(self.memory_hw["total_size"], 16)
        if end_offset >= total_memory_size:
            raise RoTValueError(
                f"Computed flash layout exceeds total memory size. Please update '{self.memory_config.config_path}'.")
