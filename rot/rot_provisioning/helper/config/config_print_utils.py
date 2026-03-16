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
from typing import Dict, Any, Optional


logger = logging.getLogger()

# Module-level defaults
PRINT_CHAR = '-'
PRINT_WIDTH = 100


def _print_header(print_char: str, print_width: int, print_config_name: Optional[str]) -> None:
    """
    Print the header section for the configuration output.
    Example:
    ------------------------------------
               Configuration Name
    ------------------------------------
    Args:
        print_char (str): Character used for decorative lines.
        print_width (int): Width of the decorative lines.
        print_config_name (str, optional): Name of the configuration to display as a header.
    """
    logger.info(print_char * print_width)
    logger.info(print_config_name.center(print_width))
    logger.info(print_char * print_width)


def _print_items(print_config_items: Dict[str, Any]) -> None:
    """
    Print each configuration item in an aligned format.
    Example:
    Label1     Value1
    LabelLong  Value2
    Args:
        print_config_items (Dict[str, Any]): Dictionary of label-value pairs to print.
    """
    max_label_len = max((len(str(label)) for label in print_config_items), default=0)
    for label, value in print_config_items.items():
        logger.info(f"{str(label).ljust(max_label_len+1)} {value}")


def _print_footer(print_char: str, print_width: int) -> None:
    """
    Print the footer section for the configuration output.

    Args:
        print_char (str): Character used for decorative lines.
        print_width (int): Width of the decorative lines.
    """
    logger.info(print_char * print_width)


def print_config_dict(
    print_config_items: Dict[str, Any],
    print_config_name: str,
    print_char: str = PRINT_CHAR,
    print_width: int = PRINT_WIDTH,
) -> None:
    """
    Print configuration items in a formatted and aligned manner, including header and footer.
    Example:
    ------------------------------------
               Configuration Name
    ------------------------------------
    Label1     Value1
    LabelLong  Value2
    ------------------------------------
    Args:
        print_config_items (Dict[str, Any]): Dictionary of label-value pairs to print.
        print_char (str, optional): Character used for decorative lines. Default is PRINT_CHAR.
        print_width (int, optional): Width of the decorative lines. Default is PRINT_WIDTH.
        print_config_name (str, optional): Name of the configuration to display as a header.
    """
    _print_header(print_char, print_width, print_config_name)
    _print_items(print_config_items)
    _print_footer(print_char, print_width)
