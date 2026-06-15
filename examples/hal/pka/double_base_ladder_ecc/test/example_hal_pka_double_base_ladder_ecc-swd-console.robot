*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pka_double_base_ladder_ecc.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_pka    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the RNG and the PKA instances.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: RNG and PKA initialization COMPLETED.

Example Step 2
    [Documentation]    Computes of k*P + m*Q, where (P,Q) are two points on an elliptic curve and (k,m) are two scalars.

    Read Console Until    [INFO] Step 2.1: Configuration parameters COMPLETED.
    Read Console Until    [INFO] Step 2.2: Computation of the PKA ECC Double Base Ladder operation COMPLETED.
    Read Console Until    [INFO] Step 2.3: Retrieving the computation result COMPLETED.
    Read Console Until    [INFO] Step 2.4: Verification of the computed result COMPLETED.

Example Step 3
    [Documentation]    PKA and RNG deinitialization before leaving the scenario.
    Read Console Until    [INFO] Step 3: PKA and RNG peripherals deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
