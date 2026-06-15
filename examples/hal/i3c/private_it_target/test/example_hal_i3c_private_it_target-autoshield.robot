*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i3c_private_it_target.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_i3c


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I3C Controller    I3C_SCL    I3C_SDA
    Reset Target
    Example Step 1
    Example Step 2

    FOR    ${i}    IN RANGE    5
        Example Step 3    ${MESSAGE_PING}
        Example Step 4
        Example Step 5    ${MESSAGE_PING}
        Example Step 6

        Example Step 3    ${MESSAGE_PONG}
        Example Step 4
        Example Step 5    ${MESSAGE_PONG}
        Example Step 6
    END


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the I3C instance and the NVIC.
    Comment    Check status LED is inactive
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=2s

Example Step 2
    [Documentation]    Activates the notification, waits for DAA completion, and checks the assigned address.
    Send I3C Dynamic Address    ${TARGET_ADDRESS}    option=1    timeout=1s
    Sleep    1s

Example Step 3
    [Documentation]    The responder expects to receive a message from the controller board, in interrupt mode.
    [Arguments]    ${expected_msg}
    Write I3C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=1s

Example Step 4
    [Documentation]    waits for one of these I3C interrupts: read transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 5
    [Documentation]    The responder sends back the received message in interrupt mode.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads back the message from the responder board
    Check I3C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=1s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s

Example Step 6
    [Documentation]    waits for one of these I3C interrupts: write transfer completed or transfer error.
    Comment    Nothing to check here
