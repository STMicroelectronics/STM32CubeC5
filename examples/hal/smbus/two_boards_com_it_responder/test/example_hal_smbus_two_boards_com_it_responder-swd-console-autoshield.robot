*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_smbus_two_boards_com_it_responder.
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

Test Tags           ip:hal_smbus    profile:printf    robot:skip
# SWD: reading ExecStatus while in sleep mode fails.


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init Smbus
    ...    SMBUS_SCL
    ...    SMBUS_SDA
    ...    ${CONTROLLER_ADDRESS}
    ...    baudrate=${BAUDRATE}
    ...    with_pullups=${True}
    ...    slave_txloop_enable=${true}

    Reset Target

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
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to send a message to the responder board (the Autoshield).

    Init Smbus
    ...    SMBUS_SCL
    ...    SMBUS_SDA
    ...    ${TARGET_ADDRESS}
    ...    baudrate=${BAUDRATE}
    ...    with_pullups=${True}
    ...    slave_read_enable=${false}
    Reset Target

    Example Step 1

    Example Step 2    ${MESSAGE_PING}
    # Read only the 1st byte of the received buffer
    Read Smbus As Controller    ${TARGET_ADDRESS}    1    check_pec=${False}    timeout=1s
    Read Console Until    [ERROR] Responder - Communication ERROR    timeout=3s

    Init Smbus
    ...    SMBUS_SCL
    ...    SMBUS_SDA
    ...    ${TARGET_ADDRESS}
    ...    baudrate=${BAUDRATE}
    ...    with_pullups=${True}
    ...    slave_txloop_enable=${true}
    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4    ${MESSAGE_PING}
    Example Step 5

${TEST_ID} Message reception failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to receive back the message from the Autoshield.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_write_enable=${false}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}
    Reset Target

    Example Step 1

    # Transmit a buffer larger than expected and expect an error
    Run Keyword And Expect Error    AutoshieldRuntimeError*    Example Step 2    ${MESSAGE_PING}

    Reset Target

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${true}

    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4    ${MESSAGE_PING}
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the SMBUS instance and the NVIC.
    ...    Checks that initialization (app_init) runs without error
    Comment    Checking ExecStatus and the LED status is not currently possible,
    ...    as the example immediately starts SMBUS communications
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=2s
    # Check Pin    MX_STATUS_LED    ${LOW}

Example Step 2
    [Documentation]    The responder expects to receive a message from the controller board, in DMA mode.
    [Arguments]    ${expected_msg}
    Write Smbus As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=500ms

Example Step 3
    [Documentation]    waits for one of these SMBUS interrupts: read transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 4
    [Documentation]    The responder sends back the received message in DMA mode.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads back the message from the responder board
    Check Smbus As Controller    ${TARGET_ADDRESS}    ${expected_msg}    timeout=1s
    Read Console Until    [INFO] Responder - Message received and sent back:
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=2s

Example Step 5
    [Documentation]    waits for one of these SMBUS interrupts: write transfer completed or transfer error.
    Comment    Test
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
