*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_ccb_protected_ecdsa_blob_creation.
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
    Example Step 3


*** Keywords ***

Example Step 1
    [Documentation]    Initialize the CCB driver and check for successful device initialization.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2.1
    [Documentation]    Configure and perform symmetric key wrapping. Check for successful configuration.
    Read Console Until    [INFO] Step 2.1: Symmetric Key wrapping COMPLETED.

Example Step 2.2
    [Documentation]    Perform ECDSA private key wrapping. Check for successful configuration.
    Read Console Until    [INFO] Step 2.2: ECDSA private key wrapping COMPLETED.

Example Step 2.3
    [Documentation]    Verify protected blob creation for ECDSA and check for successful operation.
    Read Console Until    [INFO] Step 2.3: Blob creation for ECDSA verification COMPLETED.

Example Step 3
    [Documentation]    Deinitialize the CCB instance and check for successful de-initialization.
    Read Console Until    [INFO] Step 3: de-init.
