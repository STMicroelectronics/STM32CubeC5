*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i2c_two_boards_com_polling_responder.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_i2c


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I2c    I2C_SCL    I2C_SDA    ${CONTROLLER_ADDRESS}    baudrate=${BAUDRATE}    with_pullups=${True}

    Example Step 1

    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}
        Example Step 3    ${MESSAGE_PING}

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3    ${MESSAGE_PONG}
    END


*** Keywords ***
Example Step 1
    [Documentation]    The init of the I2C instance is triggered by the applicative code.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=2s
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}

Example Step 2
    [Documentation]    Receives in polling mode a buffer, within a specific timeout.
    [Arguments]    ${expected_msg}
    Write I2C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=500ms

Example Step 3
    [Documentation]    Transmits in polling mode the received buffer, within a specific timeout.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads back the message from the responder board
    Check I2C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=500ms
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
