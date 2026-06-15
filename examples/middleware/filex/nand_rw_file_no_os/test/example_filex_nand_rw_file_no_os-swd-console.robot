*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_filex_nand_rw_file_no_os.
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
    [Documentation]    Initializes FileX and attempts to open the media.
    Comment    Can't check ExecStatus = EXEC_STATUS_INIT_OK: this value is transient.
    Read Console Until    [INFO] Step1: Initializing FileX...

Example Step 2
    [Documentation]    Formats the media if needed, then opens it.
    ${ok}=    Run Keyword And Return Status    Read Console Until    [INFO] Step2: Media opened successfully.  timeout=5s
    IF    not ${ok}
        Read Console Until    [INFO] Step2: No Filesystem found! formatting...
        Read Console Until    [INFO] Media formatted successfully.    timeout=120s
    END

Example Step 3
    [Documentation]    Creates the STM32.TXT file.
    Read Console Until    [INFO] Step3: Creating STM32.TXT file...   timeout=5s
    Read Console Until    [INFO] STM32.TXT file created successfully.

Example Step 4
    [Documentation]    Writes data into STM32.TXT.
    Read Console Until    [INFO] Step4: Writing data to STM32.TXT...
    Read Console Until    [INFO] Data written successfully.

Example Step 5
    [Documentation]    Re-opens STM32.TXT, reads back and validates the content.
    Read Console Until    [INFO] Step5: Reading back and validating data...
    Read Console Until    [INFO] Data integrity check passed.

Example Step 6
    [Documentation]    Closes the media.
    Read Console Until    [INFO] Step6: Media closed successfully.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=40s
