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

from helper.common.log import ROT_Logger
from helper.config.appli_config import Appli_Config
from helper.project.project_generic import Project_Generic
from helper.config.flash_layout_oemirot import FlashLayout_OEMiRoT


class Project_Appli(Project_Generic):
    def __init__(self, config: Appli_Config, logger: ROT_Logger):
        super().__init__(config, logger, has_postbuild=True, has_prebuild=True)

    def _custom_handle_prebuild(self, compiler: str):
        """
        Prebuild is updating:
        - the linker with ROM_APPLI_ADDRESS and ROM_APPLI_SIZE
        - the provisioning and fwu xml with input path, hex format and firmware area size
        - the appli_flash_layout.h header with relevant defines
        Args:
            compiler (str): The compiler type (e.g., 'iar').
        """
        # Updating linker
        self._update_linker_define(compiler, "ROM_APPLI_ADDRESS")
        self._update_linker_define(compiler, "ROM_APPLI_SIZE")

        # Updating provisioning xml
        self._update_image_xml_input_path('provisioning')
        self._update_image_xml_hex_format('provisioning')
        self._update_image_xml_firmware_area_size('provisioning')

        # Updating fwu xml
        self._update_image_xml_input_path('fwu')
        self._update_image_xml_firmware_area_size('fwu')

        # Updating header file
        self._update_app_flash_layout_header_file("DATA_IMAGE_NUMBER")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_DATA_SLOT_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_DATA_SLOT_SIZE")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_APP_SLOT_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_APP_SLOT_SIZE")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_APP_SLOT_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_APP_SLOT_SIZE")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_DATA_SLOT_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_DATA_SLOT_SIZE")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_APP_INSTALL_REQ_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_SECONDARY_DATA_INSTALL_REQ_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_APP_CONFIRM_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_DATA_CONFIRM_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_APP_VERSION_OFFSET")
        self._update_app_flash_layout_header_file("FLASH_PRIMARY_DATA_VERSION_OFFSET")

    def _custom_handle_postbuild(self):
        """
        Postbuild is:
        - updating the provisioning and fwu xml with keys and output path
        - generating the provisioning and fwu images
        """
        self._update_image_xml_output_path('provisioning')
        self._update_image_xml_output_path('fwu')
        self._update_image_xml_keys_path('provisioning')
        self._update_image_xml_keys_path('fwu')
        self._generate_image('provisioning')
        self._generate_image('fwu')

    def _compute_flash_layout(self):
        return FlashLayout_OEMiRoT(self.example_config, self.memory_config)

    def _update_app_flash_layout_header_file(self, define_name: str):
        """
        Updates the define in appli_flash_layout.h header file with the valuefrom flash layout.
        Args:
            define_name (str): The name of the define to update.
        """
        self._update_header_define("appli_flash_layout", define_name)
