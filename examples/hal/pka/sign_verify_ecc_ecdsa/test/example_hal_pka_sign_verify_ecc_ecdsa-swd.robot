*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pka_sign_verify_ecc_ecdsa.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_pka


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    RNG and PKA initialization
    ...    Checks that initialization (app_init) runs without error
    Comment    Nothing to test

Example Step 2
    [Documentation]    Compute a signed message regarding the Elliptic curve digital signature algorithm (ECDSA). Then determine if this signature is valid or not.
    Comment    Nothing to test

Example Step 3
    [Documentation]    Determinates if the generated signature is valid or not.
    Comment    Nothing to test

Example Step 4
    [Documentation]    PKA and RNG deinitialization before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    ...    Assuming the test scenario lasts under 5s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
