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

import re
import logging

from helper.exception.rot_exception import RoTNotFoundError

logger = logging.getLogger()


def update_define_in_file(file_path: str, define_name: str, new_value: str) -> None:
    """
    Updates the value of a define in a file.
    It updates the value while keeping the same spacing before or after the define.
    It handles both patterns:
    - '#define name value' : It can be found in the header, source or .sct files
    - 'define symbol name = value;' : It can be found in the .icf files

    Args:
        file_path (str): The path to the file to be updated.
        define_name (str): The name of the define to update.
        new_value (str): The new value to assign to the define.
            It can be a string, int, macro, or calculation.

    Assumptions:
     - The file path is an existing absolute path (no check is performed).
     - The value is on the same line as the define or continued on the next lines using backslashes.

    Raises:
        RoTNotFoundError: If the define is not found in the file.
    """
    # Regular expression to match both patterns
    define_pattern = re.compile(
        rf"^(\s*)(#define|define\s+symbol)\s+{re.escape(define_name)}(\s*=\s*|\s+)(.*?)(;?)(\r?\n|$)",
        re.MULTILINE
    )

    # Read the file content
    with open(file_path, "r") as file:
        content = file.read()

    # Search for the define and capture the leading whitespace, spacing, and value
    match = define_pattern.search(content)
    if not match:
        raise RoTNotFoundError(f"The define '{define_name}' was not found in the file '{file_path}'.")

    # Extract the leading whitespace, define type, spacing, and line ending
    leading_ws = match.group(1)   # Leading whitespace
    define_type = match.group(2)  # Either '#define' or 'define symbol'
    spacing = match.group(3)      # Spacing or '='
    line_ending = match.group(6)  # Line ending (\r\n, \n, or empty)

    # Always include the semicolon for 'define symbol' pattern
    trailing_semicolon = ""
    if define_type == "define symbol":
        trailing_semicolon = ";"

    # Replace the old value with the new value, ensuring the semicolon is included
    new_define_line = f"{leading_ws}{define_type} {define_name}{spacing}{new_value}{trailing_semicolon}{line_ending}"
    updated_content = define_pattern.sub(new_define_line, content)

    # Write the updated content back to the file
    with open(file_path, "w") as file:
        file.write(updated_content)

    logger.debug(f"Updated '{define_name}' in '{file_path}' to: {new_value}")


def update_variable_in_file(file_path: str, variable_name: str, new_value: str) -> None:
    """
    Updates the value of a variable in a file with the pattern 'NAME = VALUE;'.
    All occurrences are updated.

    Args:
        file_path (str): The path to the file to be updated.
        variable_name (str): The name of the variable to update.
        new_value (str): The new value to assign to the variable. It can be a computation.

    Assumptions:
     - The file path is an existing absolute path (no check is performed).
     - The value is on the same line as the define or continued on the next lines using backslashes.

    Raises:
        RoTNotFoundError: If the variable is not found in the file.
    """
    # Regular expression to match the pattern 'NAME = VALUE;' with optional leading spaces
    variable_pattern = re.compile(
        rf"^(\s*){re.escape(variable_name)}(\s*=\s*)(.*?);",
        re.MULTILINE
    )

    # Read the file content
    with open(file_path, "r") as file:
        content = file.read()

    # Search for the variable and capture the leading spacing, spacing around '=', and value
    match = variable_pattern.search(content)
    if not match:
        raise RoTNotFoundError(f"The variable '{variable_name}' was not found in the file '{file_path}'.")

    # Extract the leading spacing and spacing around '='
    leading_spacing = match.group(1)
    spacing_around_equals = match.group(2)

    # Replace the old value with the new value, preserving the spacing
    new_variable_line = f"{leading_spacing}{variable_name}{spacing_around_equals}{new_value};"
    updated_content = variable_pattern.sub(new_variable_line, content)

    # Write the updated content back to the file
    with open(file_path, "w") as file:
        file.write(updated_content)

    logger.debug(f"Updated '{variable_name}' in '{file_path}' to: {new_value}")
