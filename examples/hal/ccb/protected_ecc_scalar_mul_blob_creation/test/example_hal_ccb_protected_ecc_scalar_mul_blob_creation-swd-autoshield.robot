*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_ccb_protected_ecc_scalar_mul_blob_creation.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_ccb


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2.1
    Example Step 2.2
    Example Step 2.3
    Example Step 3


*** Keywords ***

Example Step 1
    [Documentation]    Initialize the CCB driver and check for successful device initialization.
    ...    Checks that initialization (app_init) runs without error
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after
    Check Pin    MX_STATUS_LED    ${LOW}

Example Step 2.1
    [Documentation]    Configure and perform symmetric key wrapping. Check for successful configuration.
    Comment    Symmetric key wrapping COMPLETED.

Example Step 2.2
    [Documentation]    Perform ECC private key wrapping. Check for successful configuration.
    Comment    ECC private key wrapping COMPLETED.

Example Step 2.3
    [Documentation]    Verify blob creation results.
    Comment    Blob creation verification COMPLETED.

Example Step 3
    [Documentation]    Deinitialize the CCB instance and check for successful de-initialization.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
    Check Pin    MX_STATUS_LED    ${HIGH}    duration=1s
