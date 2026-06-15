*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tamp_erase_backup_registers.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_tamp    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6


*** Keywords ***
Example Step 1
    [Documentation]    initializes the peripheral and configures the tamper input.
    ...    Disables the backup protection domain to access to the backup registers.
    ...    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    ExecStatus = EXEC_STATUS_INIT_OK is transient and can't be checked
    Comment    Check that the tamper event flag is reset at the beginning of the example.
    Check Integer Variable    TamperDetected    ${0}

Example Step 2
    [Documentation]    sets the backup 0 register to the value 0xAAAAAAAA and checks the written data.
    Read Console Until    Backup register 0 WRITTEN.

Example Step 3
    [Documentation]    Starts the tamper and enables the interruption for the selected tamper input.
    Read Console Until    [INFO] Step 3: Tamper STARTED.\nPress the user button to generate a tamper detection....

Example Step 4
    [Documentation]    Waits for the tamper detection interrupt. It is generated when the user button is pressed.
    Comment    Emulate a press on the button to trigger a tamper detection interrupt on the TAMP input.
    Write Pulse    BUTTON_USER_PIN
    Read Console Until    [INFO] Step 4: User button PRESSED. Tamper event DETECTED.
    Check Integer Variable    TamperDetected    ${1}

Example Step 5
    [Documentation]    checks that the backup 0 register is cleared by hardware on tamper detection.
    Read Console Until    [INFO] Step 5: Backup register 0 CLEARED.

Example Step 6
    [Documentation]    stops the tamper input and deinitializes the peripheral before leaving the scenario.
    Read Console Until    [INFO] Step 6: de-init
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    STATUS_LED_PIN    ${active_level}    timeout=2s
