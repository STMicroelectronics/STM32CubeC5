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

import os
import sys
from helper.common.cli import CLI


class Appli_Cfg(CLI):
    def __init__(self):
        appli_cfg_path = os.path.normpath(os.path.join(os.path.dirname(__file__), "../applicfg/AppliCfg.py"))
        command = f"\"{sys.executable}\" \"{appli_cfg_path}\""

        super().__init__(command, no_double_quote=True)
