*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_levelx_nor_multi_access_freertos.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_levelx    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6


*** Keywords ***
Example Step 1
    [Documentation]    Prepare concurrent execution context. Creates an event and 3 tasks with the same priority.
    Read Console Until    [INFO] Step 1: Prepare concurrent execution context

Example Step 2
    [Documentation]    Initialize LevelX NOR flash.
    Read Console Until    [INFO] Step 2: Initialize LevelX NOR flash

Example Step 3
    [Documentation]    Start the FreeRTOS scheduler.
    Read Console Until    [INFO] Step 3: Starting FreeRTOS Scheduler    timeout=10s

Example Step 4
    [Documentation]    Format and open LevelX NOR flash.
    Read Console Until    [INFO] Step 4: Format and open LevelX NOR flash    timeout=10s

Example Step 5
    [Documentation]    Each task performs 5 iterations of read, write, and data integrity check on random NOR flash sectors, logging each successful operation.
    Read Console Until
    ...    [INFO] Step 5: All concurrent read, write, and data integrity checks completed successfully for both tasks
    ...    timeout=100s

Example Step 6
    [Documentation]    Release the test sector and close the NOR flash driver.
    Read Console Until    [INFO] Step 6: Close the NOR flash driver
    ${TIMEOUT_FORMAT}=    Read Test Variable    TIMEOUT_FORMAT
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=${TIMEOUT_FORMAT}s
