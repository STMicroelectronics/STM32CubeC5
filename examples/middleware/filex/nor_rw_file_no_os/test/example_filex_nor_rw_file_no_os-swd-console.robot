*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_filex_nor_rw_file_no_os.
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
Example Step 1
    [Documentation]    Initializes FileX.
    ...    Checks that initialization (app_init) runs without error
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after
    Read Console Until    [INFO] Step 1: Initializing FileX...

Example Step 2
    [Documentation]    Formats the media if needed, then open the external memory.
    ${ok}=    Run Keyword And Return Status    Read Console Until    [INFO] Step 2: Media opened successfully.    timeout=10s
    IF    not ${ok}
        ${TIMEOUT_FORMAT}=    Read Test Variable    ${TIMEOUT_FORMAT}
        Read Console Until    [INFO] Media formatted successfully.    timeout=${TIMEOUT_FORMAT}s
        Read Console Until    [INFO] Step 2: Media opened successfully.    timeout=30s
    END

Example Step 3
    [Documentation]    create file called STM32.TXT in the root directory.
    Read Console Until    [INFO] Step3: Creating STM32.TXT file...
    Read Console Until    [INFO] STM32.TXT file created successfully.

Example Step 4
    [Documentation]    Write a data to the test file.
    Read Console Until    [INFO] Step 4: Writing data to STM32.TXT...
    Read Console Until    [INFO] Data written successfully.  timeout=30s

Example Step 5
    [Documentation]    Read data from the file into read_buffer.
    Read Console Until    [INFO] Step 5: Reading read_buffer...
    Read Console Until    [INFO] Data reading successfully.  timeout=30s
Example Step 6
    [Documentation]    Deinitializes the XSPI instance before leaving the scenario.
    Read Console Until    [INFO] Step 6: Deinitializes the XSPI instance    timeout=30s
