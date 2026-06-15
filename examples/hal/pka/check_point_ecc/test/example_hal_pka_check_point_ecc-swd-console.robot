*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pka_check_point_ecc.
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
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    RNG and PKA initialization
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1.1: RNG initialization COMPLETED.
    Read Console Until    [INFO] Step 1.2: PKA initialization COMPLETED.

Example Step 2
    [Documentation]    Determine if a point is on a curve. This allows validating an external public key.

    Read Console Until    [INFO] Step 2.1: Set Montgomery parameter Configuration COMPLETED.
    Read Console Until    [INFO] Step 2.2: Compute of the montgomery R2 input parameter COMPLETED.
    Read Console Until    [INFO] Step 2.3: Retrieve Montgomery parameter COMPLETED.

Example Step 3
    [Documentation]    Determine if a point is on a curve. This allows validating an external public key.

    Read Console Until    [INFO] Step 3.1: PKA peripheral configuration COMPLETED.
    Read Console Until    [INFO] Step 3.2: Computation COMPLETED.
    Read Console Until    [INFO] Step 3.3: Verification of the calculated point is on the curve COMPLETED.
    Read Console Until    [INFO] Step 3.4: PKA peripheral configuration COMPLETED.
    Read Console Until    [INFO] Step 3.5: Computation COMPLETED.
    Read Console Until    [INFO] Step 3.6: Verification of the calculated point is not on the curve COMPLETED.

Example Step 4
    [Documentation]    PKA and RNG deinitialization before leaving the scenario.
    Read Console Until    [INFO] Step 4: PKA and RNG peripherals deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
