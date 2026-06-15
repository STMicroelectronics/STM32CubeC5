*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_ccb_protected_ecdsa_signature.
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
    Example Step 3.1
    Example Step 3.2
    Example Step 3.3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the CCB driver and check for successful device initialization.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2.1
    [Documentation]    Wrap the ECDSA private key into a protected blob. Check for successful operation.
    Read Console Until    [INFO] Step 2.1: ECDSA private key wrapping COMPLETED.

Example Step 2.2
    [Documentation]    Generate ECDSA signature using the wrapped private key blob. Check for successful operation.
    Read Console Until    [INFO] Step 2.2: ECDSA signature generation COMPLETED.

Example Step 2.3
    [Documentation]    Compute ECDSA public key from the protected private key blob. Check for successful operation.
    Read Console Until    [INFO] Step 2.3: ECDSA public key computation COMPLETED.

Example Step 3.1
    [Documentation]    Configure PKA to process message verification. Check for successful operation.
    Read Console Until    [INFO] Step 3.1: PKA configuration to process message verification COMPLETED.

Example Step 3.2
    [Documentation]    Launch PKA signature verification computation. Check for successful operation.
    Read Console Until    [INFO] Step 3.2: PKA Signature verification computation COMPLETED.

Example Step 3.3
    [Documentation]    Verify signature result. Check for successful operation.
    Read Console Until    [INFO] Step 3.3: Signature verification COMPLETED.

Example Step 4
    [Documentation]    Deinitialize the CCB instance and check for successful de-initialization.
    Read Console Until    [INFO] Step 4: de-init.
