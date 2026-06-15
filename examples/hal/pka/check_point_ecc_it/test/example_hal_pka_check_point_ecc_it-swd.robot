*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pka_check_point_ecc_it.
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
    [Documentation]    initializes the RNG and PKA instances.
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error

    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Determines the Montgomery parameter.
    ...    Step 2.1: Sets Montgomery parameter Configuration.
    ...    Step 2.2: Computes montgomery R2 input parameters.
    ...    Step 2.3: Retrieves Montgomery parameter.
    Comment    Check application (app_process) runs without error

    Comment    Nothing to do here

Example Step 3
    [Documentation]    Determines if a point is on a curve. This allows validating an external public key.
    ...    Step 3.1: Configures the PKA peripheral to validate an external public key.
    ...    Step 3.2: Computes point P(x,y) on the curve primv256V1.
    ...    Step 3.3: Verifies if the calculated point is on the curve.
    ...    Step 3.4: Configures x and y coordinates.
    ...    Step 3.5: Computes point P(x,y) not on the curve primv256V1.
    ...    Step 3.6: Verifies if the calculated point is not on the curve.
    Comment    Check application (app_process) runs without error

    Comment    Nothing to do here

Example Step 4
    [Documentation]    Deinitializes the PKA and RNG peripherals before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    ...    Assuming the test scenario lasts under 5s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
