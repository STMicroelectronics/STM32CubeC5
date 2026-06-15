*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_spi_half_duplex_two_boards_com_polling_responder.
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
${TEST_ID} Check Init Status
    Reset Target
    Example Step 1

${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to the scenario described
    ...    in the README.

    # Initialize the SPI instance in half duplex mode
    ${baudrate}=    Read Test Variable    SPI_SCK_FREQ
    Init Spi Half Duplex Controller    ${baudrate}    MX_EXAMPLE_SPI_SCK_PIN    MX_EXAMPLE_SPI_MOSI_PIN    io_swap=True

    # Restart the responder
    Reset Target

    # Example Step 1 too quick to be tested now -> moved in "Check Init Status" test
    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}
        Example Step 3    ${MESSAGE_PING}
        Example Step 4
        Example Step 2    ${MESSAGE_PONG}
        Example Step 3    ${MESSAGE_PONG}
        Example Step 4
    END

    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the SPI instance.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}

Example Step 2
    [Documentation]    initiates communication in half-duplex blocking mode. The responder transmits data to the
    ...    responder.
    [Arguments]    ${expected}
    Spi Half Duplex Controller Send    ${expected}

Example Step 3
    [Documentation]    initiates communication in half-duplex blocking mode.
    [Arguments]    ${expected}
    Check Spi Half Duplex Controller Receive    ${expected}    ${BUFFER_SIZE}

Example Step 4
    [Documentation]    checks that the sent and received buffers match.
    Read Console Until    [INFO] Responder - Message received and sent back

End Of Example
    [Documentation]    Checks the ExecStatus variable at the end of the example.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
