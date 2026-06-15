*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_aes_cipher_cbc_simple_suspension.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_aes    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initializes, configures the AES and the timer peripherals.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1.1: Initialization, configuration of the AES peripheral with CBC algorithm COMPLETED.
    Read Console Until    [INFO] Step 1.2: Timer initialization COMPLETED.

Example Step 2
    [Documentation]    Demonstrates how to suspend the AES CBC encryption IT process,
    ...    using the timer to request suspend encryption, called in HAL_TIM_UpdateCallback,
    ...    by setting the Suspendflag. Followed by encryption process in polling mode,
    ...    then Resumes the IT encryption process.
    Read Console Until    [INFO] Step 2.1: Suspension of IT encryption process COMPLETED.
    Read Console Until    [INFO] Step 2.2: Resumption of IT encryption process COMPLETED.
    Read Console Until    [INFO] Step 2.3: Verification of computed cipher text as expected COMPLETED.

Example Step 3
    [Documentation]    Deinitializes the AES and the timer peripherals before leaving the scenario.
    Read Console Until    [INFO] Step 3: AES and timer deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
