*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_spi_full_duplex_two_boards_com_dma_responder.
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

Test Tags           ip:hal_spi    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to the scenario described
    ...    in the README.

    Example Step 1

    ${sck_freq}=    Read Test Variable    SPI_SCK_FREQ
    Init Spi Controller    ${sck_freq}    MX_EXAMPLE_SPI_SCK_PIN    MX_EXAMPLE_SPI_MOSI_PIN    MX_EXAMPLE_SPI_MISO_PIN

    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}    ${MESSAGE_PING}
        Example Step 3

        Example Step 2    ${MESSAGE_PONG}    ${MESSAGE_PONG}
        Example Step 3
    END

    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the SPI instance.
    ...    Registers the user callbacks for SPI events: TX/RX transfer completed and transfer error.
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    initiates the communication with the controller in a full duplex mode, using interrupts,
    ...    by sending and receiving data simultaneously. The SPI messages exchanged are null-terminated strings.
    [Arguments]    ${tx_message}    ${expected}
    Check Spi Read Write    ${tx_message}    ${expected}    timeout=2s
    Read Console Until    [INFO] Responder - Tx/Rx Buffers IDENTICAL.

Example Step 3
    [Documentation]    waits for one of these SPI interrupts: write-read transfer complete or transfer error.
    Comment    Nothing to check here

End Of Example
    [Documentation]    checks that the sent and received buffers match.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
