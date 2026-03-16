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


def create_directory_if_missing(output_path: str) -> None:
    """
    Ensures that the directory for the given output path exists. If the directory
    does not exist, it creates it.

    Parameters:
    output_path (str): The path for which the directory should be ensured. This can
                       be a file path or a directory path.

    Returns:
    None

    Example:
    >>> create_directory_if_missing('/path/to/directory/file.txt')
    # This will create '/path/to/directory/' if it does not exist.

    Notes:
    - The function uses `os.makedirs` to create the directory, which will also
      create any necessary parent directories.
    - If the directory already exists, no action is taken.
    """
    # Extract the directory name from the output path
    output_dir = os.path.dirname(output_path)

    # Check if the directory exists
    if not os.path.exists(output_dir):
        # Create the directory if it does not exist
        os.makedirs(output_dir)
