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
import subprocess
from typing import Optional
from helper.exception.rot_exception import RoTCLINonZeroExitError, RoTCLIRuntimeError

logger = logging.getLogger()


class CLI():
    """
    CLI class for executing shell commands and processing their output.
    """

    def __init__(self, cli: str, no_double_quote: bool = False, cmd_output_file_name: str = None) -> None:
        """
        Initializes the CLI class with the specified command.

        Args:
            cli (str): The command line interface executable to be used.
            no_double_quote (bool, optional): If True, the command will not be wrapped in double quotes. Defaults to False.
            cmd_output_file_name (str, optional): If provided, executed commands will be logged to this file. Defaults to None.
        """
        self.cli = cli
        self.no_double_quote = no_double_quote
        self.log_array = []
        self.setup_cmd_output_file(cmd_output_file_name)

    def call(self, command_args: str, log_as_info: bool = False, match_substring: str = None) -> Optional[str]:
        """
        Executes a CLI command with logging, error handling, and optional output extraction.

        Args:
            command_args (str): Arguments to pass to the CLI command.
            log_as_info (bool, optional): If True, logs output at INFO level; otherwise, at DEBUG level. Defaults to False.
            match_substring (str, optional): If provided, returns the last output line containing this substring. Defaults to None.

        Returns:
            Optional[str]: The last output line containing `match_substring` if specified, otherwise None.

        Raises:
            RoTCLIRuntimeError: If the command execution fails.
            RoTCLINonZeroExitError: If the command returns a non-zero exit status.
        """
        if self.no_double_quote:
            cmd = f"{self.cli} {command_args}"
        else:
            cmd = f"\"{self.cli}\" {command_args}"
        self.append_cmd_to_cmd_output_file(cmd)
        self.log_array = []
        logger.debug(f"Executing command: {cmd}")
        extracted_line = None
        try:
            # Start the subprocess and open pipes to stdin, stdout, and stderr
            process = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                       stderr=subprocess.STDOUT, shell=True, text=True)

            # Read the output from the process
            extracted_line = self.read_all_process_output(process, log_as_info, match_substring)

            # Wait for the process to finish and handle errors. If exit_on_error is True, exit the program on error.
            self.handle_process_completion(process, cmd)

            return extracted_line
        except subprocess.CalledProcessError as e:
            raise RoTCLIRuntimeError(cmd, str(e))

    def extract_line(self, output: str, match_substring: str) -> Optional[str]:
        """
        Returns the output if it contains the specified substring, otherwise None.
        Args:
            output (str): The output line to check.
            match_substring (str): The substring to search for.
        """
        if match_substring in output:
            return output
        return None

    def log_output(self, output: str, log_as_info: bool) -> None:
        """
        Logs the provided output string using either info or debug level based on the log_as_info flag.

        Args:
            output (str): The message to be logged.
            log_as_info (bool): If True, logs the message at info level; otherwise, logs at debug level.

        Returns:
            None
        """
        if (log_as_info):
            logger.info(output.strip())
        else:
            logger.debug(output.strip())
        self.log_array.append(output.strip())

    def read_all_process_output(self, process: subprocess.Popen, force_log_info: bool = False, match_substring: str = None) -> Optional[str]:
        """
        Reads the output from a subprocess and logs it.

        Args:
            process (subprocess.Popen): The subprocess to read output from.
            force_log_info (bool, optional): If True, logs output at INFO level; otherwise, at DEBUG level. Defaults to False.
            match_substring (str, optional): If provided, searches for this substring in the output and returns the last matching line. Defaults to None.
        """
        matching_line = None
        extracted_local = None
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                if (match_substring):
                    extracted_local = self.extract_line(output, match_substring)
                    if extracted_local is not None:
                        matching_line = extracted_local
                self.log_output(output, force_log_info)
        return matching_line

    def handle_process_completion(self, process: subprocess.Popen, cmd: str) -> None:
        """
        Waits for the given process to complete and checks its exit status.

        If the process exits with a non-zero status, reads and logs any remaining
        output or errors from the process, logs an error message with the command
        and exit status, and raises a RoTCLINonZeroExitError.

        Args:
            process: A subprocess.Popen object representing the running process.
            cmd (str): The command string that was executed.

        Raises:
            RoTCLINonZeroExitError: If the process returns a non-zero exit status.
        """
        returncode = process.wait()
        if returncode != 0:
            # Read any remaining output or errors
            stdout, stderr = process.communicate()
            if stdout:
                logger.debug(stdout)
            if stderr:
                logger.error(stderr)
            raise RoTCLINonZeroExitError(returncode, cmd, self.log_array)

    def append_cmd_to_cmd_output_file(self, cmd: str) -> None:
        """
        Writes the executed command to a file if cmd_output_file_name is set.

        Args:
            cmd (str): The command string that was executed.
        """
        if self.cmd_output_file_name:
            try:
                with open(self.cmd_output_file_name, 'a') as file:
                    file.write(cmd + '\n')
            except Exception as e:
                logger.error(f"Failed to write command to file {self.cmd_output_file_name}: {e}")

    def setup_cmd_output_file(self, cmd_output_file_name) -> None:
        """
        Resets the command output file by clearing its contents if cmd_output_file_name is set.
        """
        self.cmd_output_file_name = cmd_output_file_name
        if self.cmd_output_file_name:
            try:
                with open(self.cmd_output_file_name, 'w') as file:
                    file.write('')
            except Exception as e:
                logger.error(f"Failed to reset command file {self.cmd_output_file_name}: {e}")
