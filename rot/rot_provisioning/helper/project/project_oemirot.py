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
from helper.config.oemirot_config import OEMiRoT_Config
from helper.project.project_generic import Project_Generic
from helper.config.flash_layout_oemirot import FlashLayout_OEMiRoT
from helper.config.provisioning_config import RDPLevel
from helper.config.example_config import LifecycleType
from helper.common.update_file_handler import update_define_in_file


class Project_OEMiRoT(Project_Generic):
    def __init__(self, config: OEMiRoT_Config, logger: ROT_Logger):
        super().__init__(config, logger, has_postbuild=False, has_prebuild=True)

    def _custom_handle_prebuild(self, compiler: str):
        """
        Prebuild is updating:
        - the linker with ROM_ROT_ADDRESS and ROM_ROT_SIZE
        - the mcuboot_config.h and flash_layout.h headers with relevant defines
        - the boot_hal_cfg.h header with OEMIROT_OB_RDP_LEVEL_MIN define
        Args:
            compiler (str): The compiler type (e.g., 'iar').
        """
        # Updating linker
        self._update_linker_define(compiler, "ROM_ROT_ADDRESS")
        self._update_linker_define(compiler, "ROM_ROT_SIZE")

        # Updating mcuboot_config header
        self._update_mcuboot_config_header_file("MCUBOOT_S_DATA_IMAGE_NUMBER")
        self._update_mcuboot_config_header_file("MCUBOOT_NS_DATA_IMAGE_NUMBER")

        # Updating boot_hal_cfg header
        if self.example_config.get_lifecycle_type() == LifecycleType.RDP:
            self._update_boot_hal_cfg_header_file()

        # Updating flash_layout header
        self._update_flash_layout_header_file("FLASH_ROT_CODE_OFFSET")
        self._update_flash_layout_header_file("FLASH_ROT_CODE_SIZE")
        self._update_flash_layout_header_file("FLASH_KEYS_OFFSET")
        self._update_flash_layout_header_file("FLASH_NVCNT_OFFSET")
        self._update_flash_layout_header_file("FLASH_HASH_REF_OFFSET")
        self._update_flash_layout_header_file("FLASH_PRIMARY_DATA_SLOT_OFFSET")
        self._update_flash_layout_header_file("FLASH_PRIMARY_DATA_SLOT_SIZE")
        self._update_flash_layout_header_file("FLASH_PRIMARY_APP_SLOT_OFFSET")
        self._update_flash_layout_header_file("FLASH_PRIMARY_APP_SLOT_SIZE")
        self._update_flash_layout_header_file("FLASH_SECONDARY_APP_SLOT_OFFSET")
        self._update_flash_layout_header_file("FLASH_SECONDARY_APP_SLOT_SIZE")
        self._update_flash_layout_header_file("FLASH_SECONDARY_DATA_SLOT_OFFSET")
        self._update_flash_layout_header_file("FLASH_SECONDARY_DATA_SLOT_SIZE")
        self._update_flash_layout_header_file("FLASH_ROT_WRP_START_OFFSET")
        self._update_flash_layout_header_file("FLASH_ROT_WRP_END_OFFSET")
        self._update_flash_layout_header_file("FLASH_ROT_HDP_END_OFFSET")

    def _compute_flash_layout(self):
        return FlashLayout_OEMiRoT(self.example_config, self.memory_config)

    def _update_mcuboot_config_header_file(self, define_name: str):
        """
        Updates the define in mcuboot_config.h header file with the valuefrom flash layout.
        Args:
            define_name (str): The name of the define to update.
        """
        self._update_header_define("mcuboot_config", define_name)

    def _update_flash_layout_header_file(self, define_name: str):
        """
        Updates the define in flash_layout.h header file with the value from flash layout.
        Args:
            define_name (str): The name of the define to update.
        """
        self._update_header_define("flash_layout", define_name)

    def _update_boot_hal_cfg_header_file(self):
        """
        Updates the define "OEMIROT_OB_RDP_LEVEL_MIN" in boot_hal_cfg.h header file
        with the min RDP level value retrieved from the provisioning config.
        By default the value is set to "HAL_FLASH_ITF_OB_RDP_LEVEL_0".
        If the min RDP level is RDP_2_BS, the value is set to "HAL_FLASH_ITF_OB_RDP_LEVEL_2_WBS".
        If the min RDP level is RDP_2, the value is set to "HAL_FLASH_ITF_OB_RDP_LEVEL_2".
        """
        header_path = self.config.get_header_path("boot_hal_cfg")
        rdp_min_level = self.provisioning_config.get_rdp_level("min")
        value = "HAL_FLASH_ITF_OB_RDP_LEVEL_0"
        match rdp_min_level:
            case RDPLevel.RDP_2_BS:
                value = "HAL_FLASH_ITF_OB_RDP_LEVEL_2_WBS"
            case RDPLevel.RDP_2:
                value = "HAL_FLASH_ITF_OB_RDP_LEVEL_2"
        update_define_in_file(header_path, "OEMIROT_OB_RDP_LEVEL_MIN", value)
