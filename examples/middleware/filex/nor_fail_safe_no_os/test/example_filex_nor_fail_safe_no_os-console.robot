*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for filex_nor_fail_safe_no_os (console/printf).
...                 This test suite verifies that the application prints the expected scenario traces.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_filex    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application runs according to the scenario (printf traces).
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    Initializes FileX and attempts to open the media.
    Comment    Can't check ExecStatus = EXEC_STATUS_INIT_OK: this value is transient.
    Read Console Until    [INFO] Step1: Initializing FileX...   timeout=30s

Example Step 2
    [Documentation]    Formats the media if needed, then opens it.
    ${ok}=    Run Keyword And Return Status    Read Console Until    [INFO] Step2: Media opened successfully.    timeout=10s
    IF    not ${ok}
        ${TIMEOUT_FORMAT}=    Read Test Variable    TIMEOUT_FORMAT
        Read Console Until    [INFO] Media formatted successfully.    timeout=${TIMEOUT_FORMAT}s
        Read Console Until    [INFO] Step2: Media opened successfully.    timeout=30s
    END

Example Step 3
    [Documentation]    Enables FileX fault tolerance
    Read Console Until    [INFO] Step 3: Fault tolerance enabled.    timeout=60s

Example Step 4
    [Documentation]    Starts generic file I/O operations on the target media (create, write, read, close).
    Read Console Until    [INFO] Step 4: Starting file I/O operations on the target media.    timeout=60s
    
Example Step 5
    [Documentation]    Deinitializes the XSPI instance.
    Read Console Until    [INFO] Step 5: Deinitializes the XSPI instance    timeout=150s