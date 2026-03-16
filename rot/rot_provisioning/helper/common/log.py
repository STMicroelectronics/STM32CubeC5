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
import re
import colorlog
import os


class NoColorFormatter(logging.Formatter):
    ANSI_ESCAPE = re.compile(r'\x1b\[[0-9;]*m')

    def format(self, record):
        message = super().format(record)
        return self.ANSI_ESCAPE.sub('', message)


class ROT_Logger:
    """
    ROT_Logger provides a configurable logging utility with both colored console output and file logging.

    ### Features:
        - Colored console output using colorlog, with configurable log level (default: INFO).
        - Log output to file with DEBUG level (all messages).
        - Ability to change log file at runtime.
        - Simple interface for adjusting console log level.

    """

    def __init__(self, log_file_name: str) -> None:
        """
        Initializes the logger instance.

        Args:
            log_file_name (str): The path to the log file where logs will be written.
        """
        self.format_string = '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
        self.logger = logging.getLogger()
        self.file_handler = None
        self.configure_logger(log_file_name)
        self.log_folder = os.path.dirname(log_file_name)
        # Suppress overly verbose logs from external libraries
        logging.getLogger("asyncio").setLevel(logging.WARNING)

    def configure_logger(self, log_file_name: str) -> None:
        """
        Configures the logger for the current instance with both console and file handlers.
        This method sets the logger's level to DEBUG, removes any existing handlers, and adds:
          - A colorized console handler (using colorlog) that outputs log messages at INFO level or higher.
          - A file handler that writes all log messages (DEBUG and above) to the specified log file.
        The console output is color-coded based on the log level, while the file output uses a standard format.
        Args:
            log_file_name (str): The path to the log file where log messages will be written.
        Returns:
            None
        """
        self.logger.setLevel(logging.DEBUG)  # Set the logger's level

        # Remove all handlers associated with the logger
        for handler in self.logger.handlers[:]:
            self.logger.removeHandler(handler)

        # Create a color handler for console output
        self.color_handler = colorlog.StreamHandler()
        self.color_handler.setFormatter(colorlog.ColoredFormatter(
            '%(log_color)s%(asctime)s - %(levelname)s - %(message)s',
            log_colors={
                'DEBUG': 'cyan',
                'INFO': 'green',
                'WARNING': 'yellow',
                'ERROR': 'red',
                'CRITICAL': 'red,bg_white',
            },
            secondary_log_colors={},
            style='%'
        ))
        self.color_handler.setLevel(logging.INFO)
        self.logger.addHandler(self.color_handler)

        # Create a formatter for the log file
        self.file_formatter = logging.Formatter(self.format_string)
        # Create a file handler for writing to log file
        self.set_file_handler(log_file_name)
        self.add_file_handler()

    def set_print_debug_level(self, level: int) -> None:
        """
        Sets the debug level for the color handler used in logging (always logging.DEBUG in the output file).

        Args:
            level (int): The logging level to set for the color handler (e.g., logging.DEBUG, logging.INFO).

        Returns:
            None
        """
        self.color_handler.setLevel(level)

    def update_logger_file_name(self, file_name: str) -> None:
        """
        Updates the logger to use a new file for logging output, in the same folder as the initial log file.

        This method disables the current file handler (if any), creates a new file handler
        with the specified file name in the logger's folder, sets its logging level to DEBUG,
        applies the existing formatter, and adds it to the logger.

        Args:
            file_name (str): The name of the new log file to use for logging output.

        Returns:
            None
        """
        self.disable_file_handler()
        self.set_file_handler(os.path.join(self.log_folder, file_name))
        self.add_file_handler()

    def set_file_handler(self, file_path: str) -> None:
        """
        Sets the file handler for the logger to write log messages to a file.

        Args:
            file_path (str): The path to the log file to use.

        Returns:
            None
        """
        self.file_handler = logging.FileHandler(file_path, mode='w', encoding='utf-8')
        self.file_handler.setLevel(logging.DEBUG)
        self.file_handler.setFormatter(NoColorFormatter(self.format_string))

    def add_file_handler(self) -> None:
        """
        Adds the current file handler (if any) to the logger.
        """
        if self.file_handler:
            self.logger.addHandler(self.file_handler)

    def disable_file_handler(self) -> None:
        """
        Disables the current file handler (if any) for the logger.
        """
        if self.file_handler:
            self.logger.removeHandler(self.file_handler)
