*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_freertos_low_power.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Steps 4 & 5


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the system by creating a binary semaphore and a single task (Task_one).
    Comment    Initialization complete.

Example Step 2
    [Documentation]    Starts the FreeRTOS scheduler.
    Comment    No check is performed at this step.

Example Step 3
    [Documentation]    Verifies that Task One successfully takes the semaphore and begins toggling the status LED.
    Check Time Between Events
    ...    evt1_pin=MX_STATUS_LED
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_STATUS_LED
    ...    evt2_edge=rising edge
    ...    t_min= 0.9
    ...    t_max=1.1
    ...    time_window=2.5s

Example Steps 4 & 5
    [Documentation]    Confirm that Task One blocks on the semaphore and that the system is ready to enter low-power until the next LPTIM wake-up.
    Comment    No check is performed at this step.
