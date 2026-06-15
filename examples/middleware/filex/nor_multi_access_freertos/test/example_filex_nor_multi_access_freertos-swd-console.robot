*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_filex_nor_multi_access_freertos.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_filex    profile:printf


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

Read Console Until Or Already Seen
    [Arguments]    ${expected_output}    ${timeout}=30s
    ${history}=    Get Console History
    ${history_text}=    Evaluate    bytes($history).decode(errors="replace")
    ${already_seen}=    Run Keyword And Return Status    Should Contain    ${history_text}    ${expected_output}
    IF    not ${already_seen}
        Read Console Until    ${expected_output}    timeout=${timeout}
    END

Example Step 1
    [Documentation]    Creates an event and 3 tasks with the same priority.
    Read Console Until    [INFO] Step 1: tasks and event created.    timeout=30s

Example Step 2
    [Documentation]    Starts the FreeRTOS scheduler.
    Comment    Nothing to check in this step.

Example Step 3
    [Documentation]    Use 'task entry' to initialize the nor flash driver and open it as a FileX Media.
    ...    Formats the media if needed, then opens the external memory.
    ${ok}=    Run Keyword And Return Status    Read Console Until    [INFO] Step 3: Media opened successfully.    timeout=10s
    IF    not ${ok}
        ${TIMEOUT_FORMAT}=    Read Test Variable    ${TIMEOUT_FORMAT}
        Read Console Until    [INFO] Media formatted successfully.    timeout=${TIMEOUT_FORMAT}s
        Read Console Until    [INFO] Step 3: Media opened successfully.    timeout=30s
    END

Example Step 4
    [Documentation]    Use 'task one' to perform create, write, and read operations on the file fx_file_one.
    Read Console Until Or Already Seen    [INFO] Step 4: STM32_FILE1.TXT file creates and reads data.    timeout=30s

Example Step 5
    [Documentation]    Use 'task two' to execute creation, writing, and reading operations for the file fx_file_two.
    Read Console Until Or Already Seen    [INFO] Step 5: STM32_FILE2.TXT file creates and reads data.    timeout=30s
 
Example Step 6
    [Documentation]    Complete the scenario by ensuring that the media is properly closed.
    Comment    Step completion is covered by the expected console outputs.
