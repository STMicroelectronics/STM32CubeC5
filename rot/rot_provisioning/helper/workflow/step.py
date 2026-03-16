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
import questionary
from abc import ABC

from helper.exception.rot_exception import RoTUnexpectedError

logger = logging.getLogger()

custom_style = questionary.Style([
    ("qmark", "fg:#00ff00 bold"),      # question mark in green
    ("question", "fg:#00ff00 bold"),   # question in green
    ("answer", "fg:#ff00ff bold"),     # submitted answer in magenta
    ("pointer", "fg:#ff0000 bold"),    # pointer in red
    ("highlighted", "fg:#00ff00 bold"),  # highlighted choice in green
])


def safe_prompt(prompt_obj):
    result = prompt_obj.ask()
    # When questionary is interrupted (Ctrl+C), ask() returns None
    if result is None:
        raise KeyboardInterrupt
    return result


class Step(ABC):
    """
    Abstract base class representing a workflow step.
    The `Step` class provides a structured interface for defining and executing steps in a workflow.
    Each step can have substeps, display informational text, prompt user questions, and handle input.
    The class supports automatic and manual modes, hierarchical step levels, and customizable logging.
    Attributes:
        step (dict): The configuration dictionary for the current step.
        auto (bool): Indicates if the step runs in automatic mode, bypassing user prompts.
        log_param (dict): Parameters for formatting log and display messages if expected in the dictionary.
        choice: Stores the user's selection from a question prompt.
        level (int): The hierarchical level of the step (used for formatting).
        substeps (list[Step]): List of substeps belonging to this step.
    Methods:
        set_auto(auto: bool) -> None:
            Sets the 'auto' mode for the step.
        set_level(level: int) -> None:
            Sets the hierarchical level for the step.
        get_prefix() -> str:
            Returns a string of tab characters based on the step's level.
        run_step() -> None:
            Executes the step, including content display, progress, processing, substeps, and success message.
        process_step():
            Abstract method for step-specific logic, to be implemented by subclasses.
        get_step_from_dict(steps_dict: dict, step_id: str) -> Optional[dict]:
            Retrieves a step configuration from a dictionary by ID.
        format_name() -> str:
            Returns a formatted name for the step based on its level.
        add_step(substep: "Step") -> None:
            Adds a substep, sets its level and auto mode.
        print_step_content() -> None:
            Logs the step name and handles info text, questions, and input prompts.
        print_step_progress() -> None:
            Logs the progress message for the step.
        print_step_success() -> None:
            Logs the success message for the step.
        (Internal methods for formatting, logging, and user interaction are also provided.)
    """

    def __init__(self, step_dict: dict) -> None:
        self.step = step_dict
        self.auto = False
        self.log_param = {}
        self.choice = None
        self.level = 1
        self.substeps: list[Step] = []
        self.prefix = "\t"

    def set_auto(self, auto: bool) -> None:
        """
        Sets the 'auto' attribute to the specified boolean value.

        Args:
            auto (bool): If True, enables automatic mode; otherwise disables it.

        Returns:
            None
        """
        self.auto = auto
        for step in self.substeps:
            step.set_auto(auto)

    def set_level(self, level: int) -> None:
        """
        Set the workflow step's level. It will also set the level of all substeps to one level higher.

        Args:
            level (int): The level to assign to the workflow step.
        """
        self.level = level
        for step in self.substeps:
            step.set_level(level + 1)

    def get_prefix(self) -> str:
        """
        Returns a string prefix consisting of tab characters based on the current step's level.

        Returns:
            str: A string containing `self.level` number of tab characters.
        """
        return self.prefix * self.level

    def run_step(self) -> None:
        """
        Executes the workflow step by performing the following actions in order:
        1. Prints the content of the step.
        2. Prints the progress of the step.
        3. Processes the step's main logic.
        4. Iterates through all substeps, sets their 'auto' attribute, and runs each substep.
        5. Prints the success message for the step.

        If the step is configured to be skipped in automatic mode (i.e., the 'skip_if_auto' key
        is set to True and 'auto' is True), the step will be skipped entirely.
        """
        if ((self.step.get("skip_if_auto", False) and self.auto) or self.check_skip()):
            return

        self.print_step_content()
        self.print_step_process()
        self.process_step()
        for substep in self.substeps:
            substep.set_auto(self.auto)
            substep.run_step()
        self.print_step_success()

    def process_step(self):
        """
        Abstract method to be implemented if necessary by subclasses for step-specific processing.
        It will be called during the execution of run_step(), after printing the step content and progress and before running the substeps.
        """
        pass

    def check_skip(self) -> bool:
        """
        Checks if the current step should be skipped based on runtime conditions.

        Returns:
            bool: True if the step should be skipped, False otherwise.
        """
        return False

    def _format_first_level_name(self) -> str:
        """
        Formats the step name as a centered line within a fixed width, surrounded by solid line characters.

        The method creates a string where the step name is centered, padded on both sides with a specified line character
        to reach a total width of 100 characters.

        This is used for the first level steps (level 1).

        Returns:
            str: The formatted, centered step name line.
        """
        total_width = 100  # Adjust width as needed
        name_line = f" {self.step['name'].format(**self.log_param)} "
        line_char = "─"  # Use a solid line character
        dash_count = max(0, total_width - len(name_line))
        left_dashes = dash_count // 2
        right_dashes = dash_count - left_dashes
        border = f"{line_char * left_dashes}{name_line}{line_char * right_dashes}"
        return f"{border}"

    def _format_other_level_name(self) -> str:
        """
        Formats and returns the name of the current step with a bold prefix and a bullet separator.
        The output string is styled in bold using ANSI escape codes.

        This is used for steps that are not at the first level (level > 1).

        Returns:
            str: The formatted and styled step name.
        """
        bold = "\033[1m"
        reset = "\033[0m"
        return f"{bold}{self.get_prefix()[:-1]} • {self.step['name'].format(**self.log_param)}{reset}"

    def format_name(self) -> str:
        """
        Returns a formatted name for the workflow step based on its level.

        If the step is at the first level (level == 1), it uses the first level formatting method.
        Otherwise, it uses the formatting method for other levels.

        Returns:
            str: The formatted name for the workflow step.
        """
        if (self.level == 1):
            return self._format_first_level_name()
        else:
            return self._format_other_level_name()

    def _print_line(self, line_dict) -> None:
        """
        Logs a formatted message based on the provided line dictionary.
        Args:
            line_dict (dict): A dictionary containing message details. Expected keys:
                - "type" (str, optional): The type of message; must be blank, "info", or "warning".
                - "message" (str): The message template to be formatted with self.log_param.
        Raises:
            ValueError: If "type" is not blank and it's not "info" or "warning".
        Logs:
            - Uses logger.warning if "type" is "warning".
            - Uses logger.info otherwise.
        """

        message_type = line_dict.get("type")
        if (message_type is not None and message_type not in ["info", "warning"]):
            raise RoTUnexpectedError("line_dict 'type' must be blank, 'info' or 'warning'")
        message = f"{self.get_prefix()}{line_dict.get('message').format(**self.log_param)}"
        if (line_dict.get("type") == "warning"):
            logger.warning(message)
        else:
            logger.info(message)

    def _handle_question(self) -> None:
        """
        Handles the display of a question to the user and records their selection.

        Retrieves the question and options from the step configuration. If options are provided,
        formats them using the current log parameters. Presents the question and options to the user
        using a selection prompt, and stores the user's choice.

        Returns:
            None
        """
        question = self.step.get("questions", None)
        if question is None:
            return
        message = question.get("message", None)
        if (message is None):
            raise RoTUnexpectedError("Question must have a 'message' key.")
        message = message.format(**self.log_param)
        raw_options = question.get("options", [])
        options = [option.format(**self.log_param) for option in raw_options]
        answer = safe_prompt(questionary.select(
            message,
            choices=options,
            style=custom_style
        ))
        self.choice = answer

    def _handle_wfi(self) -> None:
        """
        If the current step requires waiting for user input (i.e., the 'wait_for_input'
        key in the step dictionary is set to True), prompts the user to press Enter
        to continue using a text input.

        Returns:
            None
        """
        if self.step.get("wait_for_input", True):
            safe_prompt(questionary.text("Press Enter to continue...", style=custom_style))

    def _handle_text(self) -> None:
        """
        Processes each line of text from the step's "texts" field and prints them.

        """
        text = self.step.get("texts", "")
        for line in text:
            self._print_line(line)

    def print_step_content(self) -> None:
        """
        Logs the formatted name of the step and, if the step is not automatic,
        handles displaying informational text, prompting questions, and waiting for input.

        This method performs the following actions:
            - Logs the step's formatted name using the logger.
            - If the step is not set to auto:
                - Displays informational text related to the step.
                - Prompts the user with a question.
                - Waits for further input or action.

        Returns:
            None
        """
        logger.info(f"{self.format_name()}")
        if (not self.auto):
            self._handle_text()
            self._handle_question()
            self._handle_wfi()

    def print_step_process(self) -> None:
        """
        Logs the process message for the current workflow step if available. Nothing printed if no process message is defined.

        Returns:
            None
        """
        process_msg = self.step.get('process')
        if process_msg:
            logger.info(f"{self.get_prefix()}{process_msg}")

    def print_step_success(self) -> None:
        """
        Logs a success message for the current workflow step if one is defined. Nothing printed if no success message is defined.

        Returns:
            None
        """
        success_msg = self.step.get('success')
        if success_msg:
            logger.info(f"{self.get_prefix()}{success_msg}")

    def add_step(self, substep: "Step") -> None:
        """
        Adds a substep to the current step.

        Args:
            substep (Step): The substep to be added.
        """
        self.substeps.append(substep)
