*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i2c_two_boards_wakeup_from_stop_it_responder.
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

Test Tags           ip:hal_i2c    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I2c    I2C_SCL    I2C_SDA    ${CONTROLLER_ADDRESS}    baudrate=${BAUDRATE}    with_pullups=${True}

    Example Step 1

    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}
        Example Step 3
        Example Step 4    ${MESSAGE_PING}
        Example Step 5

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3
        Example Step 4    ${MESSAGE_PONG}
        Example Step 5
    END

${TEST_ID} Message transmission failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the responder's code,
    ...    when it fails once to send a message to the controller board (the Autoshield).

    Init I2c    I2C_SCL    I2C_SDA    ${CONTROLLER_ADDRESS}    baudrate=${BAUDRATE}    with_pullups=${True}

    Example Step 1

    Example Step 2    ${MESSAGE_PING}

    # Read only the 1st byte of the received buffer
    Read I2c As Controller    ${TARGET_ADDRESS}    1
    Read Console Until    [ERROR] Responder - Communication ERROR    timeout=3s

    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4    ${MESSAGE_PING}
    Example Step 5

${TEST_ID} Message reception failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the responder's code,
    ...    when it fails once to receive a message from the Autoshield.

    Init I2c    I2C_SCL    I2C_SDA    ${CONTROLLER_ADDRESS}    baudrate=${BAUDRATE}    with_pullups=${True}

    Example Step 1

    # Transmit a buffer larger than expected and expect an error
    Run Keyword And Expect Error    AutoshieldRuntimeError*    Example Step 2    ${MESSAGE_PING}+++

    Reset Target

    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4    ${MESSAGE_PING}
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the I2C instance and the NVIC.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=2s

Example Step 2
    [Documentation]    The responder expects to receive a message from the controller board, in interrupt mode.
    [Arguments]    ${expected_msg}
    Write I2C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=500ms

Example Step 3
    [Documentation]    The responder enters stop mode and waits an interrupt to wakeup the MCU.
    Comment    Nothing to check here

Example Step 4
    [Documentation]    The responder wakes up and sends back the received message in interrupt mode.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads back the message from the responder board
    Check I2C As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=500ms
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s

Example Step 5
    [Documentation]    The responder enters stop mode and waits an interrupt to wakeup the MCU.
    Comment    Nothing to check here
