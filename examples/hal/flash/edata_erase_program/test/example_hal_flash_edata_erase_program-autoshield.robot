*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_flash_edata_erase_program.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_flash    profile:printf


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
    [Documentation]    Initializes the flash instance and unlocks the access to the flash control register.
    Check Option Byte    EDATA_EN    1
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED within EDATA area is ENABLED.

Example Step 2
    [Documentation]    Programs the flash EDATA area. Checks that the data is written in the desired location.
    Read Console Until    [INFO] Step 2: EDATA area programming COMPLETED.

Example Step 3
    [Documentation]    Back from the NMI handler, compare the read data with the expected data.
    ...    Locks the flash configuration and option bytes control. This prevents any unwanted modification.
    ${WriteBuffer}=    Read Raw Variable    WriteBuffer
    ${ReadBuffer}=    Read Raw Variable    ReadBuffer
    Should Not Be Equal    ${WriteBuffer}    ${ReadBuffer}
    Read Console Until    [INFO] Step 5: EDATA area is ERASED.

Example Step 4
    [Documentation]    FLASH peripheral deinitialization.
    ...    Checks that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 6: de-init
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
