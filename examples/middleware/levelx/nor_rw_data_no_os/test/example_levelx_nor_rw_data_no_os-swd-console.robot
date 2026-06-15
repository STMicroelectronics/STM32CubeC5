*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_levelx_nor_rw_data_no_os.
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
    [Documentation]    Initializes and opens the NOR flash driver.
    Read Console Until    [INFO] Step1: Initializing LevelX NOR flash...
    Read Console Until    [INFO] LevelX NOR flash initialized successfully.    timeout=5s
    Read Console Until    [INFO] Formatting LevelX NOR flash...
    ${TIMEOUT_FORMAT}=    Read Test Variable    TIMEOUT_FORMAT
    Read Console Until    [INFO] LevelX NOR flash formatted successfully.    timeout=${TIMEOUT_FORMAT}s
    Read Console Until    [INFO] Opening NOR flash driver...
    Read Console Until    [INFO] NOR flash driver opened successfully.    timeout=8s

Example Step 2
    [Documentation]    Writes data to the test sector.
    Read Console Until    [INFO] Step2: Writing data to the test sector
    Read Console Until    [INFO] Successfully wrote data to the test sector

Example Step 3
    [Documentation]    Reads data from the test sector.
    Read Console Until    [INFO] Step3: Reading data from the test sector
    Read Console Until    [INFO] Successfully read data from the test sector

Example Step 4
    [Documentation]    Verifies data integrity.
    Read Console Until    [INFO] Step4: Verifying data integrity
    Read Console Until    [INFO] Data integrity check passed

Example Step 5
    [Documentation]    Releases the test sector.
    Read Console Until    [INFO] Step5: Releasing the test sector...
    Read Console Until    [INFO] Successfully released the test sector

Example Step 6
    [Documentation]    Closes the NOR flash driver.
    Read Console Until    [INFO] Step6: NOR flash driver closed successfully.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=40s
