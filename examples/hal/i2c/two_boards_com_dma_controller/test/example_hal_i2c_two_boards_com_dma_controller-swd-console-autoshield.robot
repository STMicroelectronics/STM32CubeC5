*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i2c_two_boards_com_dma_controller.
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

Test Tags           ip:hal_i2c    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init I2c    I2C_SCL    I2C_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${False}

    Reset Target

    Example Step 1

    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}
        Example Step 3
        Example Step 4
        Example Step 5
        Example Step 6

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3
        Example Step 4
        Example Step 5
        Example Step 6
    END

${TEST_ID} Message transmission failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to send a message to the responder board (the Autoshield).

    Init I2c
    ...    I2C_SCL
    ...    I2C_SDA
    ...    ${TARGET_ADDRESS}
    ...    baudrate=${BAUDRATE}
    ...    slave_read_enable=${false}
    ...    slave_txloop_enable=${False}

    Reset Target

    Example Step 1

    Read Console Until    [ERROR] Controller - Communication ERROR

    Init I2c    I2C_SCL    I2C_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${False}

    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6

${TEST_ID} Message reception failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to receive back the message from the Autoshield.

    Init I2c    I2C_SCL    I2C_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_write_enable=${false}

    Reset Target

    Example Step 1
    Example Step 2    ${MESSAGE_PING}
    Read Console Until    [ERROR] Controller - Communication ERROR

    Init I2c    I2C_SCL    I2C_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${False}

    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6

${TEST_ID} Exchanged messages inconsistency
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it receives back a wrong message from the Autoshield.

    Init I2c    I2C_SCL    I2C_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${False}
    # push a "wrong" message in the queue, so that the first exchange will be inconsistent.
    # no impact on later exchanges, they will work "as usual"
    Write I2C As Target    ${MESSAGE_PONG}

    Reset Target

    Example Step 1
    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Read Console Until    [ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.
    Example Step 2    ${MESSAGE_PING}
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6

${TEST_ID} Message transfer abort
    [Documentation]    This test checks that the message transfer between the two boards is stopped,
    ...    when the maximum number of attempts is reached.

    # No I2C setup needed here
    Reset Target

    Example Step 1
    Repeat Keyword    ${MAX_ATTEMPTS} times
    ...    Read Console Until    [ERROR] Controller - Communication ERROR

    # Checks that the LED runs the error pattern (50ms ON / 2s OFF, about 0.49Hz).
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    IF    ${active_level}
        Check Low PWM    MX_STATUS_LED    thigh=50ms    tlow=2s    precision=0.2
    ELSE
        Check Low PWM    MX_STATUS_LED    thigh=2s    tlow=50ms    precision=0.2
    END


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the I2C instance, the NVIC and the DMA channels.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    Checking ExecStatus and the LED status is not currently possible,
    ...    as the example immediately starts I2C communications

Example Step 2
    [Documentation]    The controller starts the communication, in DMA mode, by sending a message to the responder.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads the message sent by the controller board
    Check I2C As Target    ${expected_msg}    timeout=2s

Example Step 3
    [Documentation]    waits for one of these I2C interrupts: write transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 4
    [Documentation]    The controller expects to receive the message back in DMA mode.
    Comment    The Autoshield automatically sends back the message to the controller board - no action needed

Example Step 5
    [Documentation]    waits for one of these I2C interrupts: read transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 6
    [Documentation]    The controller checks that the sent and received buffers match.
    Read Console Until    [INFO] Controller - Tx/Rx Buffers IDENTICAL.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
