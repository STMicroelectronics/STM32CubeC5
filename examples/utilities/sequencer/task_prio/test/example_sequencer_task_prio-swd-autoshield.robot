*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_sequencer_task_prio.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:utilities_sequencer


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1-2
    Example Step 3-4-5
    Example Step 6


*** Keywords ***
Example Step 1-2
    [Documentation]    Sequencer and GPIO configuration.
    Comment    Check that the initialization function (app_init()) runs without errors
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}

Example Step 3-4-5
    [Documentation]    Creation of the three tasks' functions: manage_prio(), gpio_toggle_low_frequency() and gpio_toggle_high_frequency().
    Comment    Nothing to check in this step.

Example Step 6
    [Documentation]    Infinite loop calling (SEQ_RUN()) to toggle the GPIO state for 5 seconds at a frequency of 0.5 Hz, and for 5 seconds at a frequency of 4 Hz.
    Comment    Check that the toggling frequency is 4Hz.
    Check Frequency    MX_STATUS_LED    4Hz    duty_cycle=0.5    timeout=10s
