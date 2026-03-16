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
import traceback
from colorama import Fore


class RoTException(Exception):
    """
    Base exception class for all RoT provisioning errors.

    All custom RoT exceptions should inherit from this class to allow unified error handling.
    """
    pass


class RoTNotFoundError(RoTException):
    pass


class RoTFileNotFoundError(RoTNotFoundError):
    """
    Exception raised when a required file is not found during RoT provisioning or configuration.

    Args:
        file (str): The file that was not found.
        defined_in (str, optional): The context or file where the missing file was referenced.
    """

    def __init__(self, file: str):
        self.file = file
        super().__init__(f"File not found: {file}.")


class RoTCLIError(RoTException):
    """
    Exception raised for errors in the command-line interface (CLI) during RoT operations.
    Used as a base class for more specific CLI-related exceptions.
    """
    pass


class RoTCLIRuntimeError(RoTCLIError):
    """
    Exception raised for runtime errors during CLI command execution.

    Args:
        command (str): The command that was executed.
        error_message (str): The error message returned by the command.
    """

    def __init__(self, command: str, error_message: str):
        self.command = command
        self.error_message = error_message
        super().__init__(f"Runtime error while executing command '{command}': {error_message}")


class RoTCLINonZeroExitError(RoTCLIError):
    """
    Exception raised when a CLI command returns a non-zero exit status.

    Args:
        exit_code (int): The exit code returned by the command.
        command (str): The command that was executed.
        log_array (list): The log output from the command execution.
    """

    def __init__(self, exit_code: int, command: str, log_array):
        self.exit_code = exit_code
        self.command = command
        self.log_array = log_array
        super().__init__(f"Command '{command}' exited with code {exit_code}.")


class RoTToolsConfigError(RoTException):
    """
    Exception raised for errors in the tools configuration during RoT provisioning.
    Used as a base class for more specific tool configuration errors.
    """
    pass


class RoTToolsEnvNotFoundError(RoTToolsConfigError):
    """
    Exception raised when a required environment variable is not found in the environment.

    Args:
        env_name (str): The name of the missing environment variable.
    """

    def __init__(self, env_name: str):
        self.env_name = env_name
        super().__init__(f"Environment variable '{env_name}' is not set.")


class RoTToolsNotFoundError(RoTToolsConfigError):
    """
    Exception raised when a required tool is not found at the specified path.

    Args:
        tool_name (str): The name of the missing tool.
        tool_path (str): The path where the tool was expected.
    """

    def __init__(self, tool_name: str, tool_path: str):
        self.tool_name = tool_name
        self.tool_path = tool_path
        message = f"{tool_name} not found at {tool_path}."
        super().__init__(message)


class RoTValueError(RoTException):
    pass


class RoTNegativeValueError(RoTValueError):
    """
    Exception raised when a negative value is provided where only positive values are allowed.

    Args:
        value_name (str): The name of the value.
        value (int): The negative value provided.
    """

    def __init__(self, value_name: str, value: int):
        self.value_name = value_name
        self.value = value
        super().__init__(f"The {value_name} cannot be negative: {value}.")


class RoTAbsolutePathError(RoTValueError):
    """
    Exception raised when an absolute path is provided where a relative path is expected.

    Args:
        path (str): The path that caused the error.
    """

    def __init__(self, path: str):
        self.path = path
        super().__init__(f"{path} must be an absolute path.")


class RoTEmptyValueError(RoTValueError):
    """
    Exception raised when a required value is empty or missing.

    Args:
        value_name (str): The name of the value that must not be empty.
    """

    def __init__(self, value_name: str):
        self.value_name = value_name
        super().__init__(f"The {value_name} must not be empty.")


class RoTFatalError(RoTException):
    """
    Exception for fatal errors that must crash the process and display the full stack trace.
    Use for unrecoverable bugs or logic failures that require immediate developer attention.
    """
    pass


class RoTUnexpectedError(RoTFatalError):
    """
    Exception raised for unexpected or unhandled errors during RoT provisioning.

    This exception is intended for use by developers to catch situations that should never occur
    in normal operation. If this exception is raised, it indicates a bug or an unforeseen edge case
    in the code. Customers should not encounter this error; it is primarily for debugging and internal diagnostics.

    Args:
        details(str): Details about the unexpected error.
    """

    def __init__(self, details: str):
        self.details = details
        super().__init__(f"An unexpected error occurred: {details}.")


class RoTNotImplementedError(RoTFatalError):
    """
    Exception raised when a requested feature or method is not yet implemented in RoT code.
    Used to indicate planned but unimplemented functionality.
    """
    pass


def handle_rot_exception(func):
    """
    Decorator to catch and handle RoTException and its subclasses in the wrapped function.

    - Logs errors for all RoT exceptions.
    - For RoTUnexpectedError and RoTNotImplementedError, logs and re - raises for developer attention.
    - For RoTCLINonZeroExitError, logs and prints the log array in red, then exits.
    - For other RoTExceptions, logs and exits.
    - For KeyboardInterrupt, logs and exits.

    Args:
        func(callable): The function to wrap.
    Returns:
        callable: The wrapped function with exception handling.
    """
    def wrapper(*args, **kwargs):
        logger = logging.getLogger()
        try:
            return func(*args, **kwargs)
        except RoTFatalError as e:
            logger.error(e)
            raise e
        except RoTCLINonZeroExitError as e:
            logger.error(e)
            for log_line in e.log_array:
                print(Fore.RED + log_line + Fore.RESET)
            exit(-1)
        except RoTException as e:
            tb = traceback.extract_tb(e.__traceback__)[-1]
            logger.debug(f"Exception raised in {tb.filename}, line {tb.lineno}")
            logger.error(e)
            exit(-1)
        except KeyboardInterrupt:
            logger.error("Process was manually terminated. Exiting now.")
            exit(-1)
    return wrapper
