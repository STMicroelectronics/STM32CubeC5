*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_sbs_fpu_interrupt.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_sbs


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
    [Documentation]    Enables the FPU interrupt on division by zero detection and enables the FPU interrupt in the NVIC.
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after the init.

Example Step 2
    [Documentation]    Runs a valid operation to make sure the FPU works fine.
    Comment    No time to do a check before the next step is executed.

Example Step 3
    [Documentation]    Triggers the floating-point division by zero.
    Comment    No time to do a check before the next step is executed.

Example Step 4
    [Documentation]    Handles the FPU exception via the FPU ISR.
    ...    After step 4, the example is completed.
    ...    You can observe its status via the status LED.
    ...    You can check the `ExecStatus` value with a debugger.
    Check Integer Variable    DivByZeroCaught    1
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
