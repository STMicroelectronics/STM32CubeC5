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

class Process:
    """
    Represents a workflow process that manages and executes a sequence of steps.

    Attributes:
        auto: Describes whether the process should run automatically.
        steps (list): A list of Step instances that make up the workflow.

    Methods:
        __init__(auto):
            Initializes the Process with the given auto parameter and an empty list of steps.
        run_process():
        add_step(step):
            Adds a Step instance to the workflow.
    """

    def __init__(self, auto: bool) -> None:
        """
        Initializes the object with the given auto parameter and sets up an empty list for steps.

        Args:
            auto: Describes whether the process should run automatically.
        """
        self.auto = auto
        self.steps = []

    def run_process(self) -> None:
        """
        Executes each step in the workflow.

        Returns:
            None
        """
        for step in self.steps:
            step.run_step()

    def add_step(self, step: "Step") -> None:
        """
        Adds a step to the workflow, with level set to 1 and auto inherited from the process.

        Args:
            step (Step): The step to be added to the workflow.

        Returns:
            None
        """
        self.steps.append(step)
        step.set_auto(self.auto)
        step.set_level(1)
