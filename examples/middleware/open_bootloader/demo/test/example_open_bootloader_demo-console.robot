*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_open_bootloader_demo.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_open_bootloader    profile:printf


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
    [Documentation]    Create a binary semaphore and a single application task (Task_one).
    Comment    No console output expected.

Example Step 2
    [Documentation]    Starts OpenBootloader scheduler.
    Read Console Until    [INFO] Starting OpenBootloader Scheduler

Example Step 3
    [Documentation]    Verify that Task One takes the semaphore and starts toggling the status LED.
    Read Console Until    [INFO] Task One - Binary Semaphore Taken!
    Read Console Until    [INFO] Task One - Toggling the status LED 10 times with 500ms delay.
    Sleep    2s

Example Steps 4 & 5
    [Documentation]    Confirm that Task One blocks on the semaphore and that the system is ready to enter low-power until the next LPTIM wake-up.
    Read Console Until    [INFO] Entering STOP mode.
