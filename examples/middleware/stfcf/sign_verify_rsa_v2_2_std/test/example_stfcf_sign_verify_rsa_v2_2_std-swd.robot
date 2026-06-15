*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stfcf_sign_verify_rsa_v2_2_std.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_stfcf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the RNG instance and PSA Crypto API.
    ...    Step 1.1: Initializes the RNG instance.
    ...    Step 1.2: Initializes the PSA Crypto API.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Computes the signature of the message using the regular private key
    ...    representation (modulus + exponent), so that generated signature can be compared to the known signature.
    ...    2.1: Computes directly the signature using regular private key representation,
    ...    2.2: Verifies the message with the known signature.
    Comment    Check application (app_process) runs without error

    Comment    Nothing to do here

Example Step 3
    [Documentation]    Deinitializes the RNG instance and clears data of the PSA layer before leaving the scenario.
    ...    Step 3.1: Clears the data of the PSA layer.
    ...    Step 3.2: Deinitializes the RNG instance.
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Sleep    1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
