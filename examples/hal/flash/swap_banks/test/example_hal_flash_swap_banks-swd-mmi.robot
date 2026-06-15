*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_flash_swap_banks.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Dialogs
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_flash    using:mmi


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Comment    This manual test exists because robot framework currently does not support autoshield Flash swap banks
    Test Setup
    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Test Setup
    [Documentation]    Explaining the test setup
    Comment
    ...    Flash two binaries into two memory flash banks each one with one of the following methods (STM32CubeProgrammer/IAR).
    ...    Use STM32CubeProgrammer to flash memory banks with swap activated and deactivated
    ...    Use IAR to flash each project in a flash bank
    ...    Once the two banks are flashed push the reset button to start program

Example Step 1
    [Documentation]    Initializes the Flash interface, user button, and status LED.
    ...    Unlocks Flash control and Option Bytes (OB) control registers to allow memory operations.
    Comment    Checking that initialization (app_init) runs without error.
    ...    ExecStatus is EXEC_STATUS_INIT_OK

    Pause Execution
    ...    Run program and press USER_BUTTON to swap between banks, LED toggle frequency switch from slow to fast and vice versa for each swap.

Example Step 2
    [Documentation]    Waits for the user to press and release the button.
    ...    After button release, performs a swap between Flash memory Bank 1 and Bank 2.
    ...    The status LED indicates the currently active bank by blinking at a specific frequency:
    ...    Running from Bank 1: LED blinks at a slow frequency.
    ...    Running from Bank 2: LED blinks at a fast frequency.

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
