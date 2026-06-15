*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i3c_switch_to_target.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_i3c    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I3C Target   I3C_SCL    I3C_SDA    enable_switch_ctrl=${True}

    Set I3C Target Listen Mode    2

    Reset Target

    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the I3C peripheral and registers the controller event callbacks.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=2s
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    Initiates and manages the Dynamic Address Assignment process for the controller.
    Read Console Until    [INFO] Step 2: DAA process COMPLETED.

Example Step 3
    [Documentation]    Configures the assigned target parameters and enables controller role request acknowledgment.
    Read Console Until    [INFO] Step 3: Device configuration COMPLETED.
    Switch I3C Controller Receive CCC Mode

Example Step 4
    [Documentation]    Activates Controller Role Request notification and waits for completion.
    Read Console Until    [INFO] Step 4: CRR notification RECEIVED.

Example Step 5
    [Documentation]    Performs a direct GETACCCR read and verifies the requesting target dynamic address.
    Read Console Until    [INFO] Step 5: GETACCCR data reception completed.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
