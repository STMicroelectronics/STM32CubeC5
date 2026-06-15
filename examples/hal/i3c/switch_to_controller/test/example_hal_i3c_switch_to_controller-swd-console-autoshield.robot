*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i3c_switch_to_controller.
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

Test Tags           ip:hal_i3c    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I3C Controller    I3C_SCL    I3C_SDA
    Reset Target
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the I3C instance and the NVIC.
    Comment    Check status LED is inactive
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=2s
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED

Example Step 2
    [Documentation]    Activates the notification, waits for DAA completion.
    Comment    option=1 : I3C_ENTDAA_ONLY
    Send I3C Dynamic Address    ${TARGET_ADDRESS}    option=1
    Read Console Until    [INFO] Step 2: DAA process and verification COMPLETED.
    Configure I3C Controller Devices
    Comment    5 = mode Listen for CRIE
    Set I3C Target Listen Mode    5

Example Step 3
    [Documentation]    Sends IBI request from target and waits for completion.
    Switch I3C Target Receive CCC Mode
    Read Console Until    [INFO] Target - Switch to controller COMPLETED.
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin  MX_STATUS_LED    ${active_level}    duration=1s
