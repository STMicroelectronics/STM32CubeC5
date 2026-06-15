*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_ccb_protected_ecc_scalar_mul_blob_use.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_ccb    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2.1
    Example Step 2.2
    Example Step 2.3
    Example Step 2.4
    Example Step 3


*** Keywords ***

Example Step 1
    [Documentation]    Initialize the CCB instance.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2.1
    [Documentation]    Verify that the configuration and the symmetric key wrapping completed successfully.
    Read Console Until    [INFO] Step 2.1: Symmetric key wrapping COMPLETED.

Example Step 2.2
    [Documentation]    Verify that the ECC private key wrapping completed successfully.
    Read Console Until    [INFO] Step 2.2: ECC private key wrapping COMPLETED.

Example Step 2.3
    [Documentation]    Perform ECC scalar multiplication operation using the wrapped key and check for successful operation.
    Read Console Until    [INFO] Step 2.3: ECC scalar multiplication operation COMPLETED.

Example Step 2.4
    [Documentation]    Check the output point against expected values to confirm correct ECC scalar multiplication result.
    Read Console Until    [INFO] Step 2.4: ECC scalar multiplication result verified COMPLETED.

Example Step 3
    [Documentation]    De-initialize the CCB instance.
    Read Console Until    [INFO] Step 3: de-init.
